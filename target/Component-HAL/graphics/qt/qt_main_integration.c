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
#include "hal.h"
#include "hal_logger.h"
// This is for the app functions - that should probably be in the KAPI instead?
#include "app_data.h"
#include "timer.h"

#ifdef EHS_GUI_SUPPORT_MODE_B_QT
/* We need to read the widget attribute here to look up objects and attributes in QML */
#include "widget.h"
#endif

// Helper: construct the QML path for the current app and load it
static void load_current_app_qml(void)
{
    char qml_path[EHS_TD_FILES_MAX_PATH];
    EhsHMetagetCurrentAppDir(qml_path);
    EhsStrncat(qml_path, "/app.qml", EHS_TD_FILES_MAX_PATH - EhsStrlen(qml_path) - 1);

    if (!EhsTF_exists(qml_path))
    {
        EHSH_LOG_ERROR("QML file not found: %s", qml_path);
        return;
    }

    EHSH_LOG_INFO("Loading QML file: %s", qml_path);

    ertqt_status st = ertqt_load_app(qml_path);
    if (st != ERTQT_OK)
    {
        EHSH_LOG_ERROR("ertqt_load_app() failed: %d", (int)st);
    }
}

// Track previous command to edge-detect reload transitions
static Ehs_ConsoleCommand_Type g_prev_cmd = EHS_CONTINUE;

// EHS tick callback - called from a Qt timer
static unsigned int g_tick_count = 0;

/* See headerfor detailed description of this function's purpose */

static void ehs_tick_callback(void * user_data)
{
    Ehs_ConsoleCommand_Type cmd;
    ertqt_app_state state_before, state_after;

    g_tick_count++;

    EhsTimer_tick(); // Run any timer events that might have expired.

    state_before = ertqt_get_app_state();

    // Load QML and rebuild the object table BEFORE the kernel runs so that
    // widget creation (pfCreateFunc) finds objects in the table on the same tick.
    if (EhsGetAndClearNewAppLoaded())
    {
        EHSH_LOG_INFO("EHS app reloaded, loading new QML");
        load_current_app_qml();
        ertqt_refresh_objects();
    }

    // Single-step the kernel — QML objects are now available if app just loaded
    cmd = EhsMainLoopSingle(NULL, NULL);

    if (EhsCheckAppExitLoop(cmd) == EHS_TRUE)
    {
        EHSH_LOG_INFO("EHS requesting quit");
        //printf("EHS requesting quit\n");    fflush(stdout);
        //ertqt_quit();
    }

    g_prev_cmd = cmd;

    // Advance the ertqt state machine (handles OBJECTS_READY -> SCANNED -> IDLE)
    state_after = ertqt_process_state();

    if (state_before != state_after)
    {

    }

    if (state_after == ERTQT_APP_STATE_SCANNED)
    {
        EHSH_LOG_INFO("Qt object table rebuilt — widgets can now bind");
    }
}



// Initialise Qt infrastructure. QML is loaded later from the tick callback
// once the EHS app loading state machine signals a reload.
ehs_bool EhsTV_initQt(int argc, char ** argv)
{
    ertqt_status st;

    st = ertqt_init(argc, argv);
    if (st != ERTQT_OK)
    {
        EHSH_LOG_ERROR("ertqt_init() failed: %d", (int)st);
        return EHS_FALSE;
    }

#ifdef ERTQT_SINGLETON_SCAN
    // The Rooms singleton (pragma Singleton) is not a child of any root object
    // so findChildren() misses it. Register it for explicit singleton scanning
    // so that all Room instances inside it appear in the object table.
    ertqt_add_singleton_scan("Rooms");
#endif /* ERTQT_SINGLETON_SCAN */

    return EHS_TRUE;
}


// Register EHS tick callback with Qt timer
void EhsTV_registerTickCallback(void)
{
    unsigned int interval_ms = 10;  // 100 Hz - @TODO: define this in central configuration somewhere
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
    //printf("Entering Qt event loop\n");fflush(stdout);

    EHSH_LOG_INFO("Entering Qt event loop");

    st = ertqt_run();
    if (st != ERTQT_OK)
    {
        EHSH_LOG_ERROR("ertqt_run() failed: %d", (int)st);
        return 1;
    }

    return 0;
}
