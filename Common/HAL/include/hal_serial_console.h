/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file hal_serial_console.h
 *
 * Per-target hardware-abstraction hooks used by the cross-platform
 * serial console (Common/Ehs/serial_console.c).
 *
 * Note: distinct from Common/HAL/include/hal_console.h — that file is
 * the EHS console-queue API used by the kernel for line-buffered
 * read/print and the TCPIP debug console. This file is specifically
 * the raw serial-UART abstraction (sleep, TX flush, raw write) that
 * the human-facing command-prompt task in Common/Ehs/serial_console.c
 * needs in order to be platform-agnostic.
 *
 * The serial console itself (banner, command dispatch, prompt loop,
 * WiFi status reporting, …) is platform-independent. Anything that
 * touches the actual UART hardware, the OS scheduler, or the host
 * stdio plumbing lives behind this contract — implemented in each
 * target's target/os-arch/<arch>/target_serial.c.
 *
 * The current backends (and their reference implementations):
 *   FreeRTOS + ESP-IDF  → target/os-arch/esp32{,s3}_freertos-xtensa/target_serial.c
 *   POSIX (linux, ...)  → target/os-arch/gnu_ALL/target_serial.c               (stub)
 *   Android NDK         → target/os-arch/android_ALL/target_serial.c           (stub)
 *   Zephyr RTOS         → target/os-arch/zephyr-arm/target_serial.c            (stub)
 *   FreeRTOS + redlib   → target/os-arch/nxp-redlib-freertos-arm/target_serial.c (stub)
 *   FreeRTOS + xcore    → target/os-arch/xcore_freertos-xcore/target_serial.c  (stub)
 *
 * @author: inx limited
 */

#ifndef EHS_HAL_SERIAL_CONSOLE_H
#define EHS_HAL_SERIAL_CONSOLE_H

#include "globals.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * One-shot init of the per-target serial console hardware.
 *
 * Called once from the common console task before the first byte is
 * read or written. Targets where stdio comes pre-wired (POSIX, IDF
 * default UART) can return EHS_TRUE without doing anything; targets
 * that need to install a UART driver, configure pins, or attach a USB
 * CDC-ACM endpoint do that here.
 *
 * @return EHS_TRUE on success, EHS_FALSE if the console hardware
 *         couldn't be brought up (the common task will then fall back
 *         to a no-op loop and continue without a console).
 */
ehs_bool EhsSerial_Init(void);

/* Note: thread sleep is not in this contract. The common serial-console
 * code uses the existing kernel-time HAL `EhsSleep(EHS_TIME_ms(N))` from
 * Common/HAL/include/hal_time.h (already pulled in via hal.h). Adding a
 * second sleep entry point here would just duplicate that abstraction. */

/**
 * Flush stdout and wait for the console UART TX FIFO to drain.
 *
 * Called before any subsequent blocking read so the user sees the
 * prompt before being asked for input, and before reset / long-block
 * paths so log lines are not lost on the wire.
 *
 * On ESP-IDF this is fflush(stdout) + esp_rom_uart_tx_wait_idle();
 * on POSIX it is typically just fflush(stdout); on bare-metal targets
 * it may need to poll a TX-empty status bit.
 */
void EhsSerial_FlushTx(void);

/**
 * Write n raw bytes directly to the console output, bypassing any
 * stdio buffering.
 *
 * Needed for partial-line echo (single-character feedback like '*'
 * for a password char, or "\b \b" for backspace) on stacks where
 * stdio's line buffer would hold the byte until '\n'. ESP-IDF's USB
 * Serial JTAG driver is the canonical example: a bare printf("%c",
 * ch) without a newline parks the byte on-chip until the FIFO fills.
 *
 * Implementations must not append a newline or any framing — write
 * exactly the bytes given.
 *
 * @param buf Pointer to bytes to write (not necessarily NUL-terminated).
 * @param n   Number of bytes.
 */
void EhsSerial_Write(const char* buf, ehs_uint32 n);

#ifdef __cplusplus
}
#endif

#endif /* EHS_HAL_SERIAL_CONSOLE_H */
