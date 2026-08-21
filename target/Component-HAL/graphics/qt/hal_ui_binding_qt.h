/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 ***************************************************************/

/** @file hal_ui_binding_qt.h
 *
 * Qt-internal interface for the UI-binding HAL. The public per-FB API is in
 * Common/HAL/include/hal_ui_binding.h; this header exposes only the tick
 * hook that qt_main_integration.c calls to retry pending bindings once the
 * QML object table is ready.
 */

#ifndef EHS_HAL_UI_BINDING_QT_H
#define EHS_HAL_UI_BINDING_QT_H

#ifdef __cplusplus
extern "C" {
#endif

/* Retry any subscriptions whose Qt object wasn't available at registration
 * time. Safe to call every tick — bound subscriptions are skipped. */
void EhsUIBindingQt_processPending(void);

/* Currently no-op API hooks, kept for future scenarios that need them:
 *
 *   - invalidateAll: if the QML engine ever gets recreated WITHOUT a SODL
 *     reload (FBs persist), the QSignalMapper bridges die with the old QML
 *     root but the FBs don't get DESTROY-time cleanup. Calling this would
 *     reset every sub's `bound` flag so processPending rebinds them.
 *
 *   - shutdown: a true process-exit wipe of g_subs[] for cases where the
 *     FBs don't run their DESTROY functions (e.g. abrupt shutdown).
 *
 * Under the normal reload flow neither is needed — register_property does
 * not eagerly try_bind, and per-FB DESTROY → EhsUI_unregister handles
 * subscription cleanup. The functions remain available so callers can wire
 * them up if a new lifecycle case appears. */
void EhsUIBindingQt_invalidateAll(void);
void EhsUIBindingQt_shutdown(void);

#ifdef __cplusplus
}
#endif

#endif /* EHS_HAL_UI_BINDING_QT_H */
