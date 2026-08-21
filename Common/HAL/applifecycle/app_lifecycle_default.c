/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license
 * with this file. If not, please visit
 *  <https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file app_lifecycle_default.c
 *
 * Weak default implementation of the per-app teardown hook.
 *
 * Targets that have UI / HAL state to tear down between app loads
 * provide a stronger implementation in their os-arch directory
 * (target/os-arch/<arch>/target_app_lifecycle.c). Linkers pick the
 * stronger non-weak symbol over this one automatically.
 *
 * Targets with nothing to tear down (no UI, no per-app HAL component
 * state) link against this empty default and inherit the no-op
 * behaviour without needing a per-arch stub file.
 */

#include "globals.h"
#include "hal_app_lifecycle.h"

#if defined(__GNUC__) || defined(__clang__)
__attribute__((weak))
#endif
void EhsApp_teardown(void)
{
    /* No-op default. */
}
