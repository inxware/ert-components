/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license
 * with this file. If not, please visit
 *  <https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file qt_app_lifecycle.h — Qt component-HAL teardown entry point.
 *
 * Called by the per-os-arch app-lifecycle dispatcher
 * (target/os-arch/<arch>/target_app_lifecycle.c) under
 * EHS_GUI_SUPPORT_MODE_B_QT. Provides full Qt-side teardown of all
 * per-app runtime state — engine, root window, subscription table and
 * object table — so the next app load starts from a clean slate. */

#ifndef EHS_QT_APP_LIFECYCLE_H
#define EHS_QT_APP_LIFECYCLE_H

#ifdef __cplusplus
extern "C" {
#endif

/** Tear down all Qt-side runtime state for the current app.
 *
 *   - Frees every subscription via EhsUIBindingQt_shutdown.
 *   - Destroys the QQmlApplicationEngine and its associated QQuickWindow
 *     via ertqt_destroy_engine, including the QML object graph and any
 *     QSignalMapper bridges parented to the QML root.
 *   - Resets the cached engine-ready flag and the ertqt object table.
 *
 * Idempotent: safe to call from a clean state (no engine alive, first
 * boot). */
void EhsTV_qtAppTeardown(void);

#ifdef __cplusplus
}
#endif

#endif /* EHS_QT_APP_LIFECYCLE_H */
