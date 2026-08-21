/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license.
 ***************************************************************/

/** @file target_serial.c (esp32_freertos-xtensa)
 *
 * Per-target serial console HAL for the original ESP32 / ESP-IDF.
 * Same shape as the esp32s3 backend — both share the IDF UART driver
 * and the FreeRTOS scheduler.
 */

#include "hal_serial_console.h"

#include <stdio.h>
#include <unistd.h>

#include "esp_rom_uart.h"
#include "sdkconfig.h"

#if CONFIG_ESP_CONSOLE_USB_SERIAL_JTAG
#include "hal/usb_serial_jtag_ll.h"
#endif

ehs_bool EhsSerial_Init(void)
{
    return EHS_TRUE;
}

void EhsSerial_FlushTx(void)
{
    fflush(stdout);
    esp_rom_uart_tx_wait_idle(CONFIG_ESP_CONSOLE_UART_NUM);
}

void EhsSerial_Write(const char* buf, ehs_uint32 n)
{
    write(fileno(stdout), buf, (size_t)n);
#if CONFIG_ESP_CONSOLE_USB_SERIAL_JTAG
    usb_serial_jtag_ll_txfifo_flush();
#endif
}
