/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/**
 * @file rs485_config.c
 * @brief Function block implementation for RS-485 port configuration.
 *
 * Exposes two functions:
 *   config (0x01) — set local echo and termination resistor state
 *   read   (0x02) — read current echo and termination state
 *
 * The RS-485 port number is set via the Parameter field in the CDF.
 */

#include "inx-component.h"
#include "rs485_config.h"
#include "hal_rs485_config.h"

EHS_FB_FUNCTIONS_START(rs485_config)
EHS_FB_FUNCTION_ENTRY("config", 0x01, rs485_config_config)
EHS_FB_FUNCTION_ENTRY("read",   0x02, rs485_config_read)
EHS_FB_FUNCTIONS_END

/* Port argument numbers (from CDF <Function argument="N">) */
#define INX_rs485_config_ARG_echo        1   /* InputPort:  echo (fn1, arg=1)             */
#define INX_rs485_config_ARG_termination 2   /* InputPort:  termination (fn1, arg=2)      */
#define INX_rs485_config_ARG_done        1   /* FinishPort: done (fn1, arg=1)             */
#define INX_rs485_config_ARG_error_id    1   /* OutputPort: error_id (fn1+fn2, arg=1)     */
#define INX_rs485_config_ARG_read_done   1   /* FinishPort: read_done (fn2, arg=1)        */


EHS_FB_IDENTIFY_FUNCTION(rs485_config)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(ehs_rs485_config_state_type);
}

EHS_FB_INIT_FUNCTION(rs485_config)
{
    ehs_rs485_config_state_type *state = (ehs_rs485_config_state_type *)EHS_FB_INIT_CONTEXT;
    state->port_num    = 0;
    state->echo        = EHS_FALSE;
    state->termination = EHS_FALSE;
    EhsSscanf(EHS_FB_INIT_PARAMETERS, "%d", &state->port_num);
    return EHS_TRUE;
}

/* Apply RS-485 configuration */
EHS_FB_RUN_FUNCTION(rs485_config_config)
{
    ehs_rs485_config_state_type *state = (ehs_rs485_config_state_type *)EHS_FB_RUN_CONTEXT;
    state->echo        = EHS_FB_IN_B_API2(INX_rs485_config_ARG_echo);
    state->termination = EHS_FB_IN_B_API2(INX_rs485_config_ARG_termination);
    int rc = EhsTRs485Config(state);
    EHS_FB_OUT_I_API2(INX_rs485_config_ARG_error_id) = rc;
    EHS_FB_FINISH_API2(INX_rs485_config_ARG_done);
}

/* Read RS-485 configuration */
EHS_FB_RUN_FUNCTION(rs485_config_read)
{
    ehs_rs485_config_state_type *state = (ehs_rs485_config_state_type *)EHS_FB_RUN_CONTEXT;
    int rc = EhsTRs485Read(state);
    EHS_FB_OUT_I_API2(INX_rs485_config_ARG_error_id) = rc;
    EHS_FB_FINISH_API2(INX_rs485_config_ARG_read_done);
}

EHS_FB_DESTROY_FUNCTION(rs485_config)
{
    return EHS_TRUE;
}
