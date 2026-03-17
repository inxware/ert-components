/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file hal_rs485_config.h
 * Hardware abstraction layer for RS-485 port configuration (local echo, termination).
 *
 * @author: inx limited
 */

#ifndef _HAL_RS485_CONFIG_H
#define _HAL_RS485_CONFIG_H

#include "globals.h"

typedef struct
{
    ehs_sint32 port_num;
    ehs_bool   echo;
    ehs_bool   termination;
} ehs_rs485_config_state_type;

/** Apply RS-485 configuration (echo and termination) from state.
 *  Returns 0 on success, -1 on error. */
EHS_GLOBAL int EhsTRs485Config(ehs_rs485_config_state_type *state);

/** Read current RS-485 configuration into state->echo and state->termination.
 *  Returns 0 on success, -1 on error. */
EHS_GLOBAL int EhsTRs485Read(ehs_rs485_config_state_type *state);

#endif /* _HAL_RS485_CONFIG_H */
