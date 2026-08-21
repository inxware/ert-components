/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 ***************************************************************/

/** @file hal_ui_binding_qt.c
 *
 * Qt implementation of the platform-agnostic UI-binding HAL declared in
 * Common/HAL/include/hal_ui_binding.h. Used by the ui_data_* / ui_event
 * function blocks to push and receive typed values and events to/from QML
 * objects identified by name.
 *
 * Lazy binding: the FB INIT phase fires before the QML is loaded into the
 * Qt engine, so object lookups during register/set may fail. This file
 * keeps a list of pending operations and retries them from
 * `EhsUIBindingQt_processPending()`, which the Qt tick callback in
 * qt_main_integration.c invokes on every iteration.
 *
 * Threading: all entry points are called from the EHS kernel thread (which
 * is the Qt main thread under EHS_MAIN_LOOP_ITERATIVE — the kernel is
 * single-stepped from a Qt timer). The Qt signal-bind callbacks below are
 * invoked on the Qt main thread and immediately call
 * EhsCallbackQueue_execute, which posts the dispatch back through the
 * kernel — so cross-thread concerns are minimal in this target.
 */

#include "globals.h"
#include "callback_queue.h"
#include "hal_string.h"
#include "hal_logger.h"
#include "hal_ui_binding.h"
#include "ertqt.h"
#include "hal_ui_binding_qt.h"

#define EHSL_MODULE_ID EHSH_LOG_MODULE_GRAPHICS

/* ------------------------------------------------------------------------ */
/* Registry of subscriptions                                                */
/* ------------------------------------------------------------------------ */

/* One slot per ui_data_ / ui_event FB instance in the app. A dashboard binds one
 * per gauge/telltale, so 32 was reachable; ~168 bytes per slot on a Qt target. */
#define EHS_UI_BINDING_QT_MAX_SUBSCRIPTIONS 64
#define EHS_UI_BINDING_QT_MAX_NAME          64

typedef enum
{
    SUB_KIND_PROPERTY_INT,
    SUB_KIND_PROPERTY_REAL,
    SUB_KIND_PROPERTY_BOOL,
    SUB_KIND_PROPERTY_STRING,
    SUB_KIND_SIGNAL,
} sub_kind_t;

typedef struct
{
    ehs_bool                in_use;
    ehs_bool                bound;             /* Qt connection established */
    sub_kind_t              kind;
    ehs_char                object[EHS_UI_BINDING_QT_MAX_NAME];
    ehs_char                name[EHS_UI_BINDING_QT_MAX_NAME]; /* property or signal */
    EhsCallbackQueueType *  queue;
    void *                  out_value;         /* typed per kind; NULL for SIGNAL */
} subscription_t;

static subscription_t g_subs[EHS_UI_BINDING_QT_MAX_SUBSCRIPTIONS];

static subscription_t * subs_alloc(void)
{
    for (int i = 0; i < EHS_UI_BINDING_QT_MAX_SUBSCRIPTIONS; ++i)
    {
        if (!g_subs[i].in_use)
        {
            g_subs[i].in_use = EHS_TRUE;
            g_subs[i].bound  = EHS_FALSE;
            return &g_subs[i];
        }
    }
    return NULL;
}

static void copy_name(ehs_char * dst, const ehs_char * src)
{
    if (!src) { dst[0] = '\0'; return; }
    if (EhsStrlen(src) >= EHS_UI_BINDING_QT_MAX_NAME)
    {
        /* Silent truncation binds the wrong object/property and looks like a
         * missing UI element, so say so. */
        EHSH_LOG_WARNING("ui_binding name truncated to %d chars: %s",
                         EHS_UI_BINDING_QT_MAX_NAME - 1, src);
    }
    /* EhsStrncpy is a per-target macro for strncpy — see hal_string.h notes. */
    EhsStrncpy(dst, src, EHS_UI_BINDING_QT_MAX_NAME - 1);
    dst[EHS_UI_BINDING_QT_MAX_NAME - 1] = '\0';
}

/* ------------------------------------------------------------------------ */
/* Qt signal callback — single trampoline for all subscriptions.            */
/*                                                                          */
/* user_data is the subscription_t pointer. The callback reads the current  */
/* property value via ertqt_get_property_*, writes it into the FB's global  */
/* via the stored out_value pointer, then fires the EhsCallbackQueue.       */
/* ------------------------------------------------------------------------ */

static void on_signal_fired(void * user_data)
{
    subscription_t * s = (subscription_t *)user_data;
    if (!s || !s->in_use) return;

    ertqt_object_handle h = ertqt_get_object_by_name(s->object);
    if (h < 0) return;

    switch (s->kind)
    {
    case SUB_KIND_PROPERTY_INT:
        if (s->out_value)
        {
            int v = 0;
            if (ertqt_get_property_int(h, s->name, &v) == ERTQT_OK)
                *(ehs_sint32 *)s->out_value = (ehs_sint32)v;
        }
        break;
    case SUB_KIND_PROPERTY_REAL:
        if (s->out_value)
        {
            double v = 0;
            if (ertqt_get_property_double(h, s->name, &v) == ERTQT_OK)
                *(ehs_float *)s->out_value = (ehs_float)v;
        }
        break;
    case SUB_KIND_PROPERTY_BOOL:
        if (s->out_value)
        {
            bool v = false;
            if (ertqt_get_property_bool(h, s->name, &v) == ERTQT_OK)
                *(ehs_bool *)s->out_value = v ? EHS_TRUE : EHS_FALSE;
        }
        break;
    case SUB_KIND_PROPERTY_STRING:
        if (s->out_value)
        {
            size_t required = 0;
            ertqt_get_property_string(h, s->name,
                                      (char *)s->out_value, EHS_STRING_LENGTH_MAX,
                                      &required);
        }
        break;
    case SUB_KIND_SIGNAL:
        break;
    }

    if (s->queue)
        EhsCallbackQueue_execute(s->queue);
}

/* ------------------------------------------------------------------------ */
/* Bind one subscription to its Qt signal.                                  */
/*                                                                          */
/* Property subscriptions bind to the "<property>Changed" signal — the QML  */
/* / Qt convention is to emit <prop>Changed whenever a property's value     */
/* changes. The 6-char overhead is added in a per-call scratch buffer.      */
/* ------------------------------------------------------------------------ */

static ehs_bool try_bind(subscription_t * s)
{
    if (s->bound) return EHS_TRUE;

    ertqt_object_handle h = ertqt_get_object_by_name(s->object);
    if (h < 0) return EHS_FALSE;  /* object not in table yet */

    ertqt_status st;
    if (s->kind == SUB_KIND_SIGNAL)
    {
        st = ertqt_bind_signal(h, s->name, on_signal_fired, s);
    }
    else
    {
        ehs_char sig_name[EHS_UI_BINDING_QT_MAX_NAME + 8];
        /* Snake-case property names don't get camelCased — Qt's *Changed
         * convention only applies to camelCase property names. Treat
         * "<name>Changed" as the canonical signal regardless; callers should
         * use the QML property name as written. */
        EhsStrncpy(sig_name, s->name, sizeof(sig_name) - 8);
        sig_name[sizeof(sig_name) - 8] = '\0';
        /* concat "Changed" */
        size_t n = 0;
        while (sig_name[n]) n++;
        const char tail[] = "Changed";
        for (size_t i = 0; i < sizeof(tail); ++i) sig_name[n + i] = tail[i];

        st = ertqt_bind_signal(h, sig_name, on_signal_fired, s);
    }

    if (st == ERTQT_OK)
    {
        s->bound = EHS_TRUE;
        EHSH_LOG_ENTER("ui_binding bound: object=%s name=%s kind=%d",
                      s->object, s->name, (int)s->kind);
        // No initial-value pull. Previously we called on_signal_fired(s) here
        // to give the FB a sensible starting value, but on rebind (after a
        // SODL reload) the FB's callback queue can be in a transient state
        // — entries with NULL fpRunFunc — which trips
        // EHS_TRUSTLESS_NULL_FATAL inside EhsCallbackQueue_execute and locks
        // the kernel. FBs that need an initial value should set one in their
        // own INIT phase; the first real QML property change will then
        // overwrite it via the slot we just connected.
        return EHS_TRUE;
    }
    return EHS_FALSE;
}

/* ------------------------------------------------------------------------ */
/* Public — called from qt_main_integration.c's tick when the object       */
/* table has just been (re)built (state transitions to SCANNED).            */
/* ------------------------------------------------------------------------ */

void EhsUIBindingQt_processPending(void)
{
    for (int i = 0; i < EHS_UI_BINDING_QT_MAX_SUBSCRIPTIONS; ++i)
    {
        if (g_subs[i].in_use && !g_subs[i].bound)
            (void)try_bind(&g_subs[i]);
    }
}

void EhsUIBindingQt_invalidateAll(void)
{
    // Currently a no-op.
    //
    // Kept as an API hook because there are plausible future scenarios where
    // it earns its keep — e.g. the QML engine is recreated *without* a full
    // SODL reload (so FBs persist, no DESTROY calls), and the QSignalMapper
    // bridges parented to the old QML root die silently. In that scenario
    // every in_use sub needs its `bound` flag reset so the next
    // processPending re-establishes the Qt connection against the new root.
    //
    // Under the normal reload flow (SODL change → FB DESTROY → unregister →
    // new FB INIT → register with bound=false → processPending binds against
    // the new root), there is nothing to do here. register_property does
    // NOT eagerly try_bind, so subs are never bound to a doomed root in the
    // first place.
}

void EhsUIBindingQt_shutdown(void)
{
    // Currently a no-op.
    //
    // Kept as an API hook for a true process-exit teardown — when the Qt
    // subsystem is being destroyed and there is no guarantee the FBs will
    // get a chance to run their DESTROY functions to call EhsUI_unregister.
    // A wipe of g_subs[] under those conditions prevents post-exit memory
    // reports flagging "leaked" subscription entries.
    //
    // The normal app-reload path does NOT need this — per-FB DESTROY +
    // EhsUI_unregister handles every active subscription cleanly.
}

/* ------------------------------------------------------------------------ */
/* hal_ui_binding.h API — implementations                                   */
/* ------------------------------------------------------------------------ */

ehs_bool EhsUI_set_property_int(const ehs_char * object, const ehs_char * property, ehs_sint32 value)
{
    if (!object || !property) return EHS_FALSE;
    ertqt_object_handle h = ertqt_get_object_by_name(object);
    if (h < 0) return EHS_TRUE;   /* not loaded yet — silently drop, matches register semantics */
    return ertqt_set_property_int(h, property, (int)value) == ERTQT_OK ? EHS_TRUE : EHS_FALSE;
}

ehs_bool EhsUI_set_property_real(const ehs_char * object, const ehs_char * property, ehs_float value)
{
    if (!object || !property) return EHS_FALSE;
    ertqt_object_handle h = ertqt_get_object_by_name(object);
    if (h < 0) return EHS_TRUE;
    return ertqt_set_property_double(h, property, (double)value) == ERTQT_OK ? EHS_TRUE : EHS_FALSE;
}

ehs_bool EhsUI_set_property_bool(const ehs_char * object, const ehs_char * property, ehs_bool value)
{
    if (!object || !property) return EHS_FALSE;
    ertqt_object_handle h = ertqt_get_object_by_name(object);
    if (h < 0) return EHS_TRUE;
    return ertqt_set_property_bool(h, property, value ? true : false) == ERTQT_OK ? EHS_TRUE : EHS_FALSE;
}

ehs_bool EhsUI_set_property_string(const ehs_char * object, const ehs_char * property, const ehs_char * value)
{
    if (!object || !property) return EHS_FALSE;
    ertqt_object_handle h = ertqt_get_object_by_name(object);
    if (h < 0) return EHS_TRUE;
    return ertqt_set_property_string(h, property, value ? value : "") == ERTQT_OK ? EHS_TRUE : EHS_FALSE;
}

ehs_bool EhsUI_emit_signal(const ehs_char * object, const ehs_char * signal)
{
    if (!object || !signal) return EHS_FALSE;
    ertqt_object_handle h = ertqt_get_object_by_name(object);
    if (h < 0) return EHS_TRUE;
    return ertqt_invoke_signal(h, signal) == ERTQT_OK ? EHS_TRUE : EHS_FALSE;
}

static EhsUI_subscription_handle register_property(const ehs_char * object, const ehs_char * property,
                                                   EhsCallbackQueueType * queue, void * out_value, sub_kind_t kind)
{
    if (!object || !property || !queue) return EHS_UI_HANDLE_INVALID;
    subscription_t * s = subs_alloc();
    if (!s) { EHSH_LOG_WARNING("ui_binding subscription table full"); return EHS_UI_HANDLE_INVALID; }
    copy_name(s->object, object);
    copy_name(s->name, property);
    s->kind = kind;
    s->queue = queue;
    s->out_value = out_value;
    // DELIBERATE: do NOT call try_bind here. FB INITs run during the kernel
    // SODL parse, which happens *before* the new QML is loaded.     
    // Binding is deferred to EhsUIBindingQt_processPending(), which runs
    // after load_current_app_qml() has put a fresh QML root in place. Every
    // sub gets bound exactly once, against the correct (current) root.
    return (EhsUI_subscription_handle)s;
}

EhsUI_subscription_handle EhsUI_register_property_changed_int(const ehs_char * object, const ehs_char * property,
                                                              EhsCallbackQueueType * queue, ehs_sint32 * out_value)
{
    return register_property(object, property, queue, out_value, SUB_KIND_PROPERTY_INT);
}

EhsUI_subscription_handle EhsUI_register_property_changed_real(const ehs_char * object, const ehs_char * property,
                                                               EhsCallbackQueueType * queue, ehs_float * out_value)
{
    return register_property(object, property, queue, out_value, SUB_KIND_PROPERTY_REAL);
}

EhsUI_subscription_handle EhsUI_register_property_changed_bool(const ehs_char * object, const ehs_char * property,
                                                               EhsCallbackQueueType * queue, ehs_bool * out_value)
{
    return register_property(object, property, queue, out_value, SUB_KIND_PROPERTY_BOOL);
}

EhsUI_subscription_handle EhsUI_register_property_changed_string(const ehs_char * object, const ehs_char * property,
                                                                 EhsCallbackQueueType * queue, ehs_char * out_value)
{
    return register_property(object, property, queue, out_value, SUB_KIND_PROPERTY_STRING);
}

EhsUI_subscription_handle EhsUI_register_signal(const ehs_char * object, const ehs_char * signal,
                                                EhsCallbackQueueType * queue)
{
    return register_property(object, signal, queue, NULL, SUB_KIND_SIGNAL);
}

void EhsUI_unregister(EhsUI_subscription_handle handle)
{
    if (handle == EHS_UI_HANDLE_INVALID) return;
    subscription_t * s = (subscription_t *)handle;
    // Bounds-check the handle against our static table to defend against
    // stale or corrupted handles.
    if (s < &g_subs[0] || s >= &g_subs[EHS_UI_BINDING_QT_MAX_SUBSCRIPTIONS])
    {
        EHSH_LOG_WARNING("EhsUI_unregister: handle %p outside subscription table", (void *)s);
        return;
    }
    if (!s->in_use) return;  /* idempotent */
    s->in_use    = EHS_FALSE;
    s->bound     = EHS_FALSE;
    s->queue     = NULL;
    s->out_value = NULL;
    s->object[0] = '\0';
    s->name[0]   = '\0';
    /* Note: any Qt-side QSignalMapper bridge connected to the QML object will
     * be cleaned up automatically when its parent QObject is destroyed. We
     * don't track per-sub mapper pointers, so we rely on that. If the QML
     * object outlives the FB (e.g. FB destroyed while UI persists), the
     * mapper will continue to run cb(s) on s — but s is now zeroed and
     * `in_use=false`, so on_signal_fired returns early. */
}
