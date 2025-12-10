/*
 * Qt main() integration for eRT
 * Provides EhsTV_* functions called from target_main.c
 */

#include <stdlib.h>
#include <stdio.h>

#include "globals.h"
#include "ertqt.h"
#include "ehs_main.h"
#include "hal_logger.h"


static Ehs_ConsoleCommand_Type cmd;

static long int count = 0;

// EHS tick callback - called from a Qt timer
static void ehs_tick_callback(void *user_data)
{
    // EHSH_LOG_INFO("tick callback entry %d\n", count++);

    // Force the kernel into single-stepping mode
    EhsSingleStepFlag = EHS_TRUE;

    // Progress the EHS state machine
    cmd = EhsMainLoop(NULL, NULL);
    cmd = EhsProcessInAppStateMachine(cmd);
    cmd = EhsProcessExAppStateMachine(cmd);

    if (EhsCheckAppExitLoop(cmd) == EHS_TRUE)
    {
        EHSH_LOG_INFO("EHS requesting quit\n");
        ertqt_quit();
    }
}


// Initialise Qt and load the QML file
ehs_bool EhsTV_initQt(int argc, char **argv)
{
    const char *qml_path;
    ertqt_status st;

    // Get QML file path from environment or default
    qml_path = getenv("EHS_QML_FILE");
    if (!qml_path || qml_path[0] == '\0')
    {
        qml_path = "apps/default/app.qml";
    }

    EHSH_LOG_INFO("Loading QML file: %s\n", qml_path);

    st = ertqt_init(qml_path, argc, argv);
    if (st != ERTQT_OK)
    {
        EHSH_LOG_ERROR("ertqt_init() failed: %d\n", (int)st);
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
        EHSH_LOG_WARNING("ertqt_set_tick_callback() failed: %d\n", (int)st);
    } else {
        EHSH_LOG_INFO("EHS tick registered: %u ms interval\n", interval_ms);
    }
}


// Run the core Qt event loop (blocking)
int EhsTV_runQt(void)
{
    ertqt_status st;

    EHSH_LOG_INFO("Entering Qt event loop\n");

    st = ertqt_run();
    if (st != ERTQT_OK)
    {
        EHSH_LOG_ERROR("ertqt_run() failed: %d\n", (int)st);
        return 1;
    }

    return 0;
}
