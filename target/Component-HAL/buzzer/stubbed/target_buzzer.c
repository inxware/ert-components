/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/*
 * target_buzzer.c — stubbed buzzer HAL.
 * All operations are no-ops.  Selected when EHS_BUZZER_SUPPORT=stubbed.
 */

#include "globals.h"
#include "hal_buzzer.h"

EHS_GLOBAL int EhsTBuzzerBeep(ehs_buzzer_state_type *state, ehs_sint32 freq_hz, ehs_sint32 duration_ms)
{
    (void)state;
    (void)freq_hz;
    (void)duration_ms;
    return 0;
}

EHS_GLOBAL int EhsTBuzzerStop(ehs_buzzer_state_type *state)
{
    (void)state;
    return 0;
}
