/***************************************************************
 * Copyright (C) 2008-2025 inx limited, UK - All Rights Reserved.
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license
 * with this file. If not, please visit:
 *  <https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/*
 * Qt main() integration for eRT
 * Provides EhsTV_* functions called from target_main.c
 */

#define EHSL_MODULE_ID (EHSH_LOG_MODULE_GRAPHICS)

#include <stdlib.h>
#include <stdio.h>

#include "globals.h"
#include "ertqt.h"
#include "ehs_main.h"
#include "hal_logger.h"

#ifdef EHS_GUI_SUPPORT_MODE_B_QT
#include "widget.h"
#endif


static Ehs_ConsoleCommand_Type cmd;

static long int count = 0;
static ehs_bool widgets_auto_created = EHS_FALSE;

#ifdef EHS_GUI_SUPPORT_MODE_B_QT
// Forward declaration from widget table
extern EhsWidgetTableClass EhsWidgetTable;
#endif

// EHS tick callback - called from a Qt timer
static void ehs_tick_callback(void * user_data)
{
    extern EhsKEStateType EhsKEState;  // Declare external

    count++;

    // Force the kernel into single-stepping mode
    EhsSingleStepFlag = EHS_TRUE;

    // Transition READY -> RUNNING (like console commands do)
    if (EhsKEState == EHSKE_STATE_READY)
    {
        EhsKEState = EHSKE_STATE_RUNNING;
    }

    // Log state every few iterations
    if ((count & 0xFu) == 0)
    {
        EHSH_LOG_INFO("tick callback #%ld, EhsKEState=%d, SingleStepFlag=%d",
        count, (int)EhsKEState, (int)EhsSingleStepFlag);
    }

    // // Only output this occasionally, as it's very spammy!
    // if ((count & 0xFu) == 0)
    // {
    //     EHSH_LOG_INFO("tick callback entry %ld", count);
    // }

    // Force the kernel into single-stepping mode
    //EhsSingleStepFlag = EHS_TRUE;

    // EHSH_LOG_INFO("About to call `EhsMainLoop()`...");

    // Progress the EHS state machine
    cmd = EhsMainLoop(NULL, NULL);

    // EHSH_LOG_INFO("About to call `EhsProcessInAppStateMachine()`...");

    cmd = EhsProcessInAppStateMachine(cmd);

    // EHSH_LOG_INFO("About to call `EhsProcessExAppStateMachine()`...");

    cmd = EhsProcessExAppStateMachine(cmd);

    // EHSH_LOG_INFO("Finishing `ehs_tick_callback`...");

// #ifdef EHS_GUI_SUPPORT_MODE_B_QT
//     // After a few ticks, manually trigger widget creation for any widgets
//     // that are initialized but not created. This is necessary because in Mode B Qt,
//     // widgets don't auto-create - they rely on the application to trigger create events.
//     // But if the application doesn't have that wiring, we need to do it manually.
//     if (!widgets_auto_created && count == 10)
//     {
//         EHSH_LOG_INFO("Auto-creating widgets after EHS initialization period");

//         // Iterate through widget table and create any initialized-but-not-created widgets
//         for (ehs_uint16 i = 0; i < EHS_MAX_WIDGET_INSTANCES; i++)
//         {
//             EhsWidgetClass * pWidget = &EhsWidgetTable.xWidget[i];

//             // Check if widget is initialized but not created
//             if ((pWidget->nState & EHS_WIDGET_STATE_INIT) &&
//                 !(pWidget->nState & EHS_WIDGET_STATE_CREATED))
//             {
//                 EHSH_LOG_INFO("  Auto-creating widget %d (nState=0x%02x, pFIData=%p)",
//                               i, pWidget->nState, (void*)pWidget->pFIData);

//                 // pFIData currently contains inx_gui_widget_state pointer (stored in INIT as a hack)
//                 // We'll keep it as is - it will be used as the function instance pointer
//                 // The event callback will receive this pointer and use it to trigger events

//                 // Call the widget's create function
//                 if (pWidget->pfCreateFunc)
//                 {
//                     EHSH_LOG_INFO("    Calling pfCreateFunc=%p (pFIData will remain=%p)",
//                                   (void*)pWidget->pfCreateFunc, (void*)pWidget->pFIData);
//                     pWidget->pfCreateFunc(pWidget);
//                     EHSH_LOG_INFO("    Widget created, new nState=0x%02x, event_callback=%p",
//                                   pWidget->nState, (void*)EHS_WIDGET_UI(pWidget).event_callback);
//                 }
//             }
//         }

//         widgets_auto_created = EHS_TRUE;
//     }
// #endif

    if (EhsCheckAppExitLoop(cmd) == EHS_TRUE)
    {
        EHSH_LOG_INFO("EHS requesting quit");
        ertqt_quit();
    }
}


// Initialise Qt and load the QML file
ehs_bool EhsTV_initQt(int argc, char ** argv)
{
    const char *qml_path;
    ertqt_status st;

    // Get QML file path from environment or default
    qml_path = getenv("EHS_QML_FILE");
    if (!qml_path || qml_path[0] == '\0')
    {
        qml_path = "apps/default/app.qml";
    }

    EHSH_LOG_INFO("Loading QML file: %s", qml_path);

    st = ertqt_init(qml_path, argc, argv);
    if (st != ERTQT_OK)
    {
        EHSH_LOG_ERROR("ertqt_init() failed: %d", (int)st);
        return EHS_FALSE;
    }

    return EHS_TRUE;
}


// Register EHS tick callback with Qt timer
void EhsTV_registerTickCallback(void)
{
    unsigned int interval_ms = 10;  // 100 Hz - TODO: define this in central configuration somewhere
    ertqt_status st;

    st = ertqt_set_tick_callback(interval_ms, ehs_tick_callback, NULL);
    if (st != ERTQT_OK)
    {
        EHSH_LOG_WARNING("ertqt_set_tick_callback() failed: %d", (int)st);
    } else {
        EHSH_LOG_INFO("EHS tick registered: %u ms interval", interval_ms);
    }
}


// Run the core Qt event loop (blocking)
int EhsTV_runQt(void)
{
    ertqt_status st;

    EHSH_LOG_INFO("Entering Qt event loop");

    st = ertqt_run();
    if (st != ERTQT_OK)
    {
        EHSH_LOG_ERROR("ertqt_run() failed: %d", (int)st);
        return 1;
    }

    return 0;
}
