/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/**
 * @file usb_power.c
 * @brief Function block implementation for USB port power control.
 *
 * Exposes three functions:
 *   enable      (0x01) — enable power on the configured USB port
 *   disable     (0x02) — disable power on the configured USB port
 *   read_status (0x03) — read current power state of the USB port
 *
 * The USB port number is set via the Parameter field in the CDF.
 */

#include "globals.h"
#include "usb_power.h"
#include "hal_usb_power.h"

EHS_FB_FUNCTIONS_START(usb_power)
EHS_FB_FUNCTION_ENTRY("enable",      0x01, usb_power_enable)
EHS_FB_FUNCTION_ENTRY("disable",     0x02, usb_power_disable)
EHS_FB_FUNCTION_ENTRY("read_status", 0x03, usb_power_read_status)
EHS_FB_FUNCTIONS_END

/* Port argument numbers */
#define INX_usb_power_ARG_error_id 1   /* Output: error code (enable/disable)       */
#define INX_usb_power_ARG_powered  1   /* Output: powered bool (read_status)        */


EHS_FB_IDENTIFY_FUNCTION(usb_power)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(ehs_usb_power_state_type);
}

EHS_FB_INIT_FUNCTION(usb_power)
{
    ehs_usb_power_state_type *state = (ehs_usb_power_state_type *)EHS_FB_INIT_CONTEXT;
    state->port_num = 1;
    state->powered  = EHS_FALSE;
    EhsSscanf(EHS_FB_INIT_PARAMETERS, "%d", &state->port_num);
    return EHS_TRUE;
}

/* Enable USB port power */
EHS_FB_RUN_FUNCTION(usb_power_enable)
{
    ehs_usb_power_state_type *state = (ehs_usb_power_state_type *)EHS_FB_RUN_CONTEXT;
    int rc = EhsTUsbPowerEnable(state);
    EHS_FB_OUT_I_API2(INX_usb_power_ARG_error_id, rc);
    EHS_FB_FINISH_PORT("enabled");
}

/* Disable USB port power */
EHS_FB_RUN_FUNCTION(usb_power_disable)
{
    ehs_usb_power_state_type *state = (ehs_usb_power_state_type *)EHS_FB_RUN_CONTEXT;
    int rc = EhsTUsbPowerDisable(state);
    EHS_FB_OUT_I_API2(INX_usb_power_ARG_error_id, rc);
    EHS_FB_FINISH_PORT("disabled");
}

/* Read USB port power status */
EHS_FB_RUN_FUNCTION(usb_power_read_status)
{
    ehs_usb_power_state_type *state = (ehs_usb_power_state_type *)EHS_FB_RUN_CONTEXT;
    EhsTUsbPowerReadStatus(state);
    EHS_FB_OUT_B_API2(INX_usb_power_ARG_powered, state->powered);
    EHS_FB_FINISH_PORT("status_done");
}

EHS_FB_DESTROY_FUNCTION(usb_power)
{
    return EHS_TRUE;
}
