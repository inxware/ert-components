#include <Arduino.h>
#include "target_network.h"
#include "target_adcdac.h"
#include "hal.h"
#include "ehs_main.h"
#include "hal_logger.h"

////////////////////////////////////////////////////////////////////
// @TODO - temporary solution
// ! using sodl bin converted to buffer for testing !
#ifdef INX_SODL_IN_FLASH
#include "sodl_bin.c"
ehs_char* EhsL_buff = sodl_binary_data;
#endif
////////////////////////////////////////////////////////////////////

static Ehs_ConsoleCommand_Type Command;

/* eRT appliaction loading status callback invoked inside the eRT kernel */
void app_load_status_handler(ehs_uint32 status)
{
    switch(status)
    {
        case EHS_APP_LOAD_STARTED : {
            EhsStdioPrintf("******* App loading started *******\n");
            break;
        }
        case EHS_APP_LOAD_SUCCESFULL : {
            EhsStdioPrintf("******* App loaded sucessfully *******\n");
            break;
        }
        case EHS_APP_LOAD_RESTARTING : {
            EhsStdioPrintf("******* App restarting *******\n");
            break;
        }
        case EHS_APP_LOAD_FAILED : {
            EhsStdioPrintf("******* App loading failed *******\n");
            break;
        }
        default: {
            EhsStdioPrintf("Unknow app loading status!\n");
            break;
        }
    }
#ifdef EHS_COMPONENT_NETWORKING_SUPPORT
    EhsTgtNetworkSetAppStatus(status);
#endif
}

/* TODO2026 Is this the same as targetos_init.c????/ */
void eRT_setup()
{
    EhsStdioPrintf("EHS Init\n");
    // Set app loading callback
    EhsHSetAppLoadStatusCallback(app_load_status_handler);
#ifdef EHS_PERIPHERALS_ADC_DAC_SUPPORT
    EhsTgtAdcDacInit();
#endif
#ifdef EHS_COMPONENT_NETWORKING_SUPPORT
    EhsTgtNetworkInit();
#endif
    EhsInit();
    EhsAppLoadingStateMachine(NULL, NULL);
}

/* TODO2026 replace this with EhsMainLoopSingle()*/
void eRT_loop()
{
    Command = EhsMainLoop(NULL, NULL);
    Command = EhsProcessInAppStateMachine(Command);
    Command = EhsProcessExAppStateMachine(Command);
    if (EhsCheckAppExitLoop(Command) == EHS_TRUE)
        EhsExit(0);
}


bool eRT_wifi(const char* ssid, const char* password)
{
#ifdef EHS_COMPONENT_NETWORKING_SUPPORT
    return EhsTgtNetworkStartWifi(ssid, password);
#else
    EhsStdioPrintf("WiFi is NOT supported!\n");
    return false;
#endif
}

/* This is used when arduino eRT is not built as a lib */
#ifndef EHS_PLUGIN_LIBRARY
void setup()
{
#ifdef EHS_RUNTIME_LOGGER_ENABLED
    Serial.begin(9600);
    while (!Serial); // wait for the serial to connect before starting ehs
#endif
    eRT_setup();
}
void loop()
{
    eRT_loop();
}
#endif
