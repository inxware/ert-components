/***************************************************************
 * Copyright (C) 2025 inx limited, UK - All Rights Reserved
 * LGPLv3 license — see https://www.gnu.org/licenses/lgpl-3.0.txt
 ***************************************************************/

/** @file target_display.h
 * Stub display/backlight API for Zephyr ARM targets.
 * Backlight control is not universally available on Zephyr boards;
 * provide a no-op so the inx-display_backlight component compiles.
 */

#ifndef _TARGET_DISPLAY_H_
#define _TARGET_DISPLAY_H_

#include "globals.h"

static inline void target_setBacklight(ehs_sint32 intensity)
{
    (void)intensity; /* no backlight hardware on this target */
}

#endif /* _TARGET_DISPLAY_H_ */
