/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license.
 ***************************************************************/

/** @file target_serial.c (android_ALL)
 *
 * Stubbed serial console HAL. Android does not normally expose a
 * physical serial console to apps; routing console output to the
 * Android log buffer (logcat) or a hostside adb shell would be the
 * typical real implementation.
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
