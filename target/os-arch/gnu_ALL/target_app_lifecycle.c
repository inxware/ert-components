/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license
 * with this file. If not, please visit
 *  <https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file target_app_lifecycle.c — gnu_ALL implementation of EhsApp_teardown.
 *
 * GNU/Linux desktop targets host one or more feature-support component
 * HALs (UI, networking, audio, …). This file is the per-os-arch
 * dispatcher: it conditionally invokes the teardown entry points exposed
 * by whichever component HALs are built into the current target.
 *
 * Each #ifdef block here corresponds to a build-time feature flag (set
 * in the platform's config.mk). Adding a new feature support layer that
 * needs per-app teardown means adding (a) its own teardown function in
 * its component-HAL directory and (b) a guarded call to it here.
 */

#include "globals.h"
#include "hal_app_lifecycle.h"

#ifdef EHS_GUI_SUPPORT_MODE_B_QT
/* Qt UI subsystem teardown — destroys the QQmlApplicationEngine,
 * QQuickWindow, all subscription state and the ertqt object table. */
#include "qt_app_lifecycle.h"
#endif

void EhsApp_teardown(void)
{
#ifdef EHS_GUI_SUPPORT_MODE_B_QT
    EhsTV_qtAppTeardown();
#endif
    /* Add other feature-support component-HAL teardowns here, each
     * under its own build-time guard. */
}
