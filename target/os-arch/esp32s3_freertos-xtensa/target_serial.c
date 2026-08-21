/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license.
 ***************************************************************/

/** @file target_serial.c (esp32s3_freertos-xtensa)
 *
 * Per-target serial console HAL for ESP32-S3 / ESP-IDF v5.1.
 *
 * Backs the contract declared in Common/HAL/include/hal_serial_console.h. The
 * cross-platform console code in Common/Ehs/serial_console.c calls
 * only the EhsSerial_* entry points below — every reference to
 * vTaskDelay / esp_rom_uart_tx_wait_idle / usb_serial_jtag_ll_*
 * is contained in this file.
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
    /* IDF configures the console UART for us via the bootloader and
     * the early-boot init sequence run by app_main's environment.
     * Nothing extra to do here. */
    return EHS_TRUE;
}

void EhsSerial_FlushTx(void)
{
    fflush(stdout);
    /* Wait for the UART TX FIFO to drain at hardware level —
     * fflush on its own only pushes from stdio into the driver, not
     * out of the chip. CONFIG_ESP_CONSOLE_UART_NUM defaults to 0. */
    esp_rom_uart_tx_wait_idle(CONFIG_ESP_CONSOLE_UART_NUM);
}

void EhsSerial_Write(const char* buf, ehs_uint32 n)
{
    /* Bypass stdio's line buffer. IDF's USB-Serial-JTAG VFS write only
     * triggers the hardware TX FIFO flush on '\n', so single-char
     * prints (echo, '*', "\b \b") would park on-chip until the FIFO
     * fills if we went through printf. */
    write(fileno(stdout), buf, (size_t)n);
#if CONFIG_ESP_CONSOLE_USB_SERIAL_JTAG
    usb_serial_jtag_ll_txfifo_flush();
#endif
}
