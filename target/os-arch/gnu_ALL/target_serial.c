/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license.
 ***************************************************************/

/** @file target_serial.c (gnu_ALL — Linux / POSIX desktop)
 *
 * Stubbed serial console HAL. POSIX targets normally have a TTY-backed
 * stdio that needs no per-target setup, so the implementations are
 * thin no-ops / standard libc wrappers. Replace with a real backend
 * if a non-default console (e.g. a specific /dev/tty) is needed.
 *
 * Reference (working) implementation: see
 *   target/os-arch/esp32s3_freertos-xtensa/target_serial.c
 */

#include "hal_serial_console.h"

ehs_bool EhsSerial_Init(void)
{
    /* TODO: real impl when this target wires up a serial console. */
    return EHS_TRUE;
}
void EhsSerial_FlushTx(void)
{
    /* TODO: fflush(stdout) when the console task ports to POSIX. */
}

void EhsSerial_Write(const char* buf, ehs_uint32 n)
{
    /* TODO: write(fileno(stdout), buf, n) when the console task ports. */
    (void)buf;
    (void)n;
}
