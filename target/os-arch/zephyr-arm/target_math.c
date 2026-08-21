/***************************************************************
 * Copyright (C) 2025 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file target_math.c
 * Zephyr RTOS implementation of target-specific math functions.
 *
 * Integer trig uses a 91-entry cosine lookup table (no FPU required).
 * Float log functions delegate to picolibc/libm which on Cortex-M33 uses
 * the hardware FPU single-precision instructions.
 *
 * @author: inx limited
 */

#define EHS_TGT_CODE
#include "globals.h"
#include <math.h>

/* -----------------------------------------------------------------------
 * Integer trigonometry (lookup table, no floating-point required)
 * ----------------------------------------------------------------------- */

static const ehs_sint32 sZephyrCosTable[91] =
{
    1000000, 999848, 999391, 998630, 997564, 996195, 994522, 992546, 990268, 987688,
    984808, 981627, 978148, 974370, 970296, 965926, 961262, 956305, 951057, 945519,
    939693, 933580, 927184, 920505, 913545, 906308, 898794, 891007, 882948, 874620,
    866025, 857167, 848048, 838671, 829038, 819152, 809017, 798636, 788011, 777146,
    766044, 754710, 743145, 731354, 719340, 707107, 694658, 681998, 669131, 656059,
    642788, 629320, 615661, 601815, 587785, 573576, 559193, 544639, 529919, 515038,
    500000, 484810, 469472, 453990, 438371, 422618, 406737, 390731, 374607, 358368,
    342020, 325568, 309017, 292372, 275637, 258819, 241922, 224951, 207912, 190809,
    173648, 156434, 139173, 121869, 104528,  87156,  69756,  52336,  34899,  17452,
    0
};

ehs_sint32 EhsTgtInt_cos(ehs_sint32 nMult, ehs_sint32 nAngle)
{
    ehs_sint32 nDiv = 1000000 / nMult;
    ehs_sint32 normalisedAngle = (nAngle < 0) ? (-(nAngle % 360)) : (nAngle % 360);
    ehs_sint32 ret;

    if (normalisedAngle <= 90)
    {
        ret = sZephyrCosTable[normalisedAngle] / nDiv;
    }
    else if (normalisedAngle <= 180)
    {
        ret = -sZephyrCosTable[180 - normalisedAngle] / nDiv;
    }
    else if (normalisedAngle <= 270)
    {
        ret = -sZephyrCosTable[normalisedAngle - 180] / nDiv;
    }
    else
    {
        ret = sZephyrCosTable[360 - normalisedAngle] / nDiv;
    }
    return ret;
}

ehs_sint32 EhsTgtInt_sin(ehs_sint32 nMult, ehs_sint32 nAngle)
{
    return EhsTgtInt_cos(nMult, nAngle - 90);
}

/* -----------------------------------------------------------------------
 * Floating-point logarithms
 * ----------------------------------------------------------------------- */

ehs_float EhsTgtFloat_log10(ehs_float in)
{
#if EHS_FLOAT_AS_FLOAT_TYPE == 1
    return log10f(in);
#else
    return log10(in);
#endif
}

ehs_float EhsTgtFloat_loge(ehs_float in)
{
#if EHS_FLOAT_AS_FLOAT_TYPE == 1
    return logf(in);
#else
    return log(in);
#endif
}

/* -----------------------------------------------------------------------
 * getentropy stub for picolibc stack-protector initialisation.
 *
 * picolibc's __stack_chk_init() calls getentropy() to seed the canary.
 * Older Zephyr (e.g. 4.1) does not provide getentropy() unless
 * CONFIG_ENTROPY_DEVICE_LEGACY_API is enabled and an entropy driver is present.
 * On MCU targets the stack canary seed quality is less critical; use zero to
 * satisfy the linker.
 *
 * Declared 'weak' so that on newer Zephyr / nRF Connect SDK (Zephyr 4.3+ under
 * CONFIG_POSIX_API provides its own getentropy() in lib/posix/c_lib_ext) the
 * real implementation wins and this fallback is silently discarded — otherwise
 * the two collide with a multiple-definition link error.
 * ----------------------------------------------------------------------- */
#include <stddef.h>
__attribute__((weak)) int getentropy(void *buf, size_t buflen)
{
    if (buf && buflen)
    {
        __builtin_memset(buf, 0, buflen);
    }
    return 0;
}

/* -----------------------------------------------------------------------
 * settimeofday stub.
 *
 * Zephyr's POSIX layer provides gettimeofday() but not settimeofday().
 * The HAL's EhsHSetDateTimeSec() is guarded in hal_systime.c with
 * EHS_ZEPHYR_RTOS, so this stub is here only to keep the linker happy if
 * some path still reaches it.
 * ----------------------------------------------------------------------- */
#include <sys/time.h>
#ifndef settimeofday
int settimeofday(const struct timeval *tv, const struct timezone *tz)
{
    (void)tv;
    (void)tz;
    return -1;
}
#endif
