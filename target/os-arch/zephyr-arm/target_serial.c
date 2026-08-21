/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license.
 ***************************************************************/

/** @file target_serial.c (zephyr-arm)
 *
 * Stubbed serial console HAL for Zephyr ARM targets. A real
 * implementation would map onto k_msleep() and the chosen Zephyr
 * console driver (the inx-uart0 alias on most boards — see
 * docs/llm-dev-contexts/CLAUDE-zephyr.md).
 *
 * Reference (working) implementation: see
 *   target/os-arch/esp32s3_freertos-xtensa/target_serial.c
 */

#include "hal_serial_console.h"

ehs_bool EhsSerial_Init(void)
{
    return EHS_TRUE;
}
void EhsSerial_FlushTx(void)
{
}

void EhsSerial_Write(const char* buf, ehs_uint32 n)
{
    (void)buf;
    (void)n;
}
