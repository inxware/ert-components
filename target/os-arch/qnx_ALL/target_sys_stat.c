/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file target_sys_stat.c
 * QNX Neutrino AArch64 — hardware system-statistics stubs.
 *
 * CPU temperature readback requires board-specific resource manager support
 * (e.g. a Raspberry Pi 4 temperature driver).  Until that driver is
 * implemented these functions return 0 so the runtime can start cleanly.
 *
 * @TODO: implement EhsTGetCpuTemp() using the RPi4 QNX thermal resource manager.
 *
 * @author: inx limited
 */

#include "hal_target_sys_stat.h"
#include "target_types.h"

ehs_sint32 EhsTGetCpuTemp(void)
{
    return 0;
}

ehs_sint32 EhsTGetCpuUsagePercent(void)
{
    return 0;
}

ehs_sint32 EhsTGetRamUsagePercent(void)
{
    return 0;
}
