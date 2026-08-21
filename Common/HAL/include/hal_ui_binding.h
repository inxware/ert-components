/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license
 * with this file. If not, please visit
 *  <https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file hal_ui_binding.h
 *
 * Platform-agnostic UI-binding HAL — the API used by the ui_data_* and
 * ui_event function blocks to send property values and signals to a UI, and
 * to receive change notifications back.
 *
 * Targets that have a UI implement these functions; targets without a UI
 * simply do not build the ui_data_* / ui_event FBs (guarded by
 * EHS_UI_BINDING_SUPPORT in the components.mk).
 *
 * Currently implemented for:
 *   - Qt (QML)  →  target/Component-HAL/graphics/qt/hal_ui_binding_qt.c
 *
 * The contract — "Object" and "property"/"signal" are strings the FB parsed
 * from its CDF parameters. Inside this HAL they are looked up against the
 * underlying UI framework's object table (e.g. ertqt_get_object_by_name).
 * Lookups may be deferred (the UI may load after the FB is initialised);
 * the HAL is responsible for retrying internally until the object appears
 * or the FB is destroyed.
 *
 * Threading: all `EhsUI_*` functions are called from the EHS kernel thread.
 * The HAL must marshal across to the UI thread internally if the framework
 * has thread-affinity requirements (Qt does).
 */

#ifndef EHS_HAL_UI_BINDING_H
#define EHS_HAL_UI_BINDING_H

#include "globals.h"
#include "callback_queue.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------ */
/* Push values from EHS → UI                                                */
/* ------------------------------------------------------------------------ */

/** Set an integer property on a named UI object.
 *  @return EHS_TRUE on success (or when the object is not yet available — the
 *          HAL may buffer the value internally and retry). EHS_FALSE on a
 *          permanent error.
 */
ehs_bool EhsUI_set_property_int   (const ehs_char * object, const ehs_char * property, ehs_sint32 value);

/** Set a real-valued property on a named UI object. */
ehs_bool EhsUI_set_property_real  (const ehs_char * object, const ehs_char * property, ehs_float  value);

/** Set a boolean property on a named UI object. */
ehs_bool EhsUI_set_property_bool  (const ehs_char * object, const ehs_char * property, ehs_bool   value);

/** Set a string property on a named UI object. UTF-8. */
ehs_bool EhsUI_set_property_string(const ehs_char * object, const ehs_char * property, const ehs_char * value);

/** Emit a parameterless signal on a named UI object. */
ehs_bool EhsUI_emit_signal        (const ehs_char * object, const ehs_char * signal);

/* ------------------------------------------------------------------------ */
/* Subscribe to UI → EHS changes                                            */
/* ------------------------------------------------------------------------ */
/*
 * Registration model — the FB calls this once at INIT and stores the returned
 * handle in its state. The HAL binds the corresponding "<property>Changed"
 * signal (Qt convention) or the named signal (for EhsUI_register_signal).
 * When the signal fires:
 *
 *   1. The HAL reads the current property value (typed) and writes it into
 *      `*out_value` for property registrations. For signal registrations
 *      there is no value buffer.
 *   2. The HAL calls EhsCallbackQueue_execute(queue) which dispatches the
 *      callback on the EHS thread — the FB's `*_changed_cb` / `*_received_cb`
 *      run-function reads the value from the global and writes it to its
 *      OutputPort.
 *
 * `out_value` MUST point to a static or module-level storage location that
 * outlives the registration. The HAL stores the pointer; it does not copy.
 * For strings, out_value points to a buffer of at least EHS_STRING_LENGTH_MAX
 * bytes.
 *
 * Each call adds another subscription. Two FBs registering against the same
 * (object, property) each receive their own callback when the property
 * changes — no de-duplication.
 *
 * LIFECYCLE — the FB MUST call EhsUI_unregister() with the returned handle
 * from its DESTROY function. Without this, the subscription leaks past the
 * FB's lifetime: on SODL reload the FB is destroyed but its subscription
 * lingers, eventually pointing to freed callback queues / FB instance
 * memory. That breaks the bindings of the *next* app, causes mis-routed
 * events, and (when on_signal_fired runs with stale state) can lock up the
 * kernel inside EhsCallbackQueue_execute.
 *
 * @return Opaque handle to the subscription on success. EHS_UI_HANDLE_INVALID
 *         on permanent error (NULL args, registry full). The handle is only
 *         meaningful as the input to EhsUI_unregister().
 */
typedef void * EhsUI_subscription_handle;
#define EHS_UI_HANDLE_INVALID ((EhsUI_subscription_handle)0)

EhsUI_subscription_handle EhsUI_register_property_changed_int   (const ehs_char * object, const ehs_char * property,
                                                                 EhsCallbackQueueType * queue, ehs_sint32 * out_value);
EhsUI_subscription_handle EhsUI_register_property_changed_real  (const ehs_char * object, const ehs_char * property,
                                                                 EhsCallbackQueueType * queue, ehs_float  * out_value);
EhsUI_subscription_handle EhsUI_register_property_changed_bool  (const ehs_char * object, const ehs_char * property,
                                                                 EhsCallbackQueueType * queue, ehs_bool   * out_value);
EhsUI_subscription_handle EhsUI_register_property_changed_string(const ehs_char * object, const ehs_char * property,
                                                                 EhsCallbackQueueType * queue, ehs_char   * out_value /* size = EHS_STRING_LENGTH_MAX */);

/** Register a callback for the named signal. When the UI fires it, `queue`
 *  is dispatched. No value buffer — the FB just receives an event. */
EhsUI_subscription_handle EhsUI_register_signal                 (const ehs_char * object, const ehs_char * signal,
                                                                 EhsCallbackQueueType * queue);

/** Tear down a subscription previously returned by EhsUI_register_*.
 *  MUST be called from the FB's DESTROY function. Safe to call with
 *  EHS_UI_HANDLE_INVALID (no-op). After this call the handle is dead and
 *  must not be reused. */
void EhsUI_unregister(EhsUI_subscription_handle handle);

#ifdef __cplusplus
}
#endif

#endif /* EHS_HAL_UI_BINDING_H */
