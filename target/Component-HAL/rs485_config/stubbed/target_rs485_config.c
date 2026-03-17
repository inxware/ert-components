/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/*
 * target_rs485_config.c — stubbed RS-485 config HAL.
 * All operations are no-ops.  Selected when EHS_RS485_CONFIG_SUPPORT=stubbed.
 */

#include "globals.h"
#include "hal_rs485_config.h"

EHS_GLOBAL int EhsTRs485Config(ehs_rs485_config_state_type *state)
{
    (void)state;
    return 0;
}

EHS_GLOBAL int EhsTRs485Read(ehs_rs485_config_state_type *state)
{
    if (state)
    {
        state->echo        = EHS_FALSE;
        state->termination = EHS_FALSE;
    }
    return 0;
}
