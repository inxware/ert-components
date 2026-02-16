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

#ifdef EHS_GUI_SUPPORT_MODE_B_QT
#include "widget.h"
#endif

// EHS tick callback - called from a Qt timer
static void ehs_tick_callback(void * user_data)
{
    Ehs_ConsoleCommand_Type cmd;
    EhsTimer_tick(); // Run any timer events  that might have expired.
    // Single-step the kernel; returns any unhandled commands, but QT wont be interested (unless perhaps it was an exit request)
    cmd = EhsMainLoopSingle(NULL, NULL);
    
    if (EhsCheckAppExitLoop(cmd) == EHS_TRUE)
    {
        EHSH_LOG_INFO("EHS requesting quit");
        ertqt_quit();
    }
    //printf(".");fflush(stdout); // debug
}



// Initialise Qt and load the QML file. Note this must be done after we have run the application state machine so we know what app we are running.
ehs_bool EhsTV_initQt(int argc, char ** argv)
{
    const char qml_path[EHS_TD_FILES_MAX_PATH];
    ertqt_status st;

    EhsHMetagetCurrentAppDir(qml_path);
    EhsStrncat(qml_path, "/app.qml", EHS_TD_FILES_MAX_PATH - EhsStrlen(qml_path) - 1);
    //printf("Constructed QML path: %s\n", qml_path);
     if (!EhsTF_exists(qml_path))
     {
         EHSH_LOG_ERROR("QML file not found: %s", qml_path);
         return EHS_FALSE;
     }
    //qml_path = "../appdata/default/app.qml";

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
    unsigned int interval_ms = 10;  // 100 Hz - @TODO: define this in central configuration somewhere
    ertqt_status st;
printf("\n¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬Registering EHS tick callback with Qt timer, interval %u ms\n", interval_ms);
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
