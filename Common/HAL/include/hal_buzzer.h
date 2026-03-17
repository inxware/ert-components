/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file hal_buzzer.h
 * Hardware abstraction layer for the buzzer peripheral.
 *
 * @author: inx limited
 */

#ifndef _HAL_BUZZER_H
#define _HAL_BUZZER_H

#include "globals.h"

typedef struct
{
    ehs_sint32 default_freq_hz;
    ehs_sint32 default_duration_ms;
} ehs_buzzer_state_type;

/** Emit a tone at freq_hz for duration_ms milliseconds.
 *  On hardware that does not support frequency or duration (e.g. Exo Sense Pi
 *  fixed-tone buzzer) the parameters are accepted but may be ignored.
 *  Returns 0 on success, -1 on error. */
EHS_GLOBAL int EhsTBuzzerBeep(ehs_buzzer_state_type *state, ehs_sint32 freq_hz, ehs_sint32 duration_ms);

/** Stop the buzzer immediately.
 *  Returns 0 on success, -1 on error. */
EHS_GLOBAL int EhsTBuzzerStop(ehs_buzzer_state_type *state);

#endif /* _HAL_BUZZER_H */
