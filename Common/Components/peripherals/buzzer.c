/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/**
 * @file buzzer.c
 * @brief Function block implementation for the buzzer peripheral.
 *
 * Exposes two functions:
 *   beep (0x01) — emit a tone at a given frequency for a given duration
 *   stop (0x02) — stop the buzzer immediately
 */

#include "inx-component.h"
#include "buzzer.h"
#include "hal_buzzer.h"

EHS_FB_FUNCTIONS_START(buzzer)
EHS_FB_FUNCTION_ENTRY("beep", 0x01, buzzer_beep)
EHS_FB_FUNCTION_ENTRY("stop", 0x02, buzzer_stop)
EHS_FB_FUNCTIONS_END

/* Port argument numbers (from CDF <Function argument="N">) */
#define INX_buzzer_ARG_freq_hz     1   /* InputPort:  freq_hz (fn1, arg=1)    */
#define INX_buzzer_ARG_duration_ms 2   /* InputPort:  duration_ms (fn1, arg=2) */
#define INX_buzzer_ARG_done        1   /* FinishPort: done (fn1, arg=1)       */
#define INX_buzzer_ARG_error_num   1   /* OutputPort/I: error_num (fn1, arg=1) */
#define INX_buzzer_ARG_stopped     1   /* FinishPort: stopped (fn2, arg=1)    */


EHS_FB_IDENTIFY_FUNCTION(buzzer)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(ehs_buzzer_state_type);
}

EHS_FB_INIT_FUNCTION(buzzer)
{
    ehs_buzzer_state_type *state = (ehs_buzzer_state_type *)EHS_FB_INIT_CONTEXT;
    state->default_freq_hz     = 1000;
    state->default_duration_ms = 200;
    EhsSscanf(EHS_FB_INIT_PARAMETERS, "%d %d",
              &state->default_freq_hz, &state->default_duration_ms);
    return EHS_TRUE;
}

/* Emit a beep */
EHS_FB_RUN_FUNCTION(buzzer_beep)
{
    ehs_buzzer_state_type *state = (ehs_buzzer_state_type *)EHS_FB_RUN_CONTEXT;
    ehs_sint32 freq_hz     = EHS_FB_IN_I_API2(INX_buzzer_ARG_freq_hz);
    ehs_sint32 duration_ms = EHS_FB_IN_I_API2(INX_buzzer_ARG_duration_ms);
    EHS_FB_OUT_I_API2(INX_buzzer_ARG_error_num) = EhsTBuzzerBeep(state, freq_hz, duration_ms);
    EHS_FB_FINISH_API2(INX_buzzer_ARG_done);
}

/* Stop the buzzer */
EHS_FB_RUN_FUNCTION(buzzer_stop)
{
    ehs_buzzer_state_type *state = (ehs_buzzer_state_type *)EHS_FB_RUN_CONTEXT;
    EhsTBuzzerStop(state);
    EHS_FB_FINISH_API2(INX_buzzer_ARG_stopped);
}

EHS_FB_DESTROY_FUNCTION(buzzer)
{
    return EHS_TRUE;
}
