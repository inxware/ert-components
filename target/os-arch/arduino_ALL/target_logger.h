/***************************************************************
 * Copyright (C) 2008-2025 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

/** @file target_logger.h
 * In this file, all of the hardware abstraction layer functions provided for EHS are given.
 *
 * @author: inx limited
 *
 */

#ifndef EHS_TARGET_LOGGER_H
#define EHS_TARGET_LOGGER_H

#ifndef EHS_TARGET_PRINTF_BUF_SIZE 
#define EHS_TARGET_PRINTF_BUF_SIZE 256
#endif

void arduino_serial_printf(const char* format, ...);

int arduino_serial_read(char* buffer, int size);

void EhsPrintHeapMemoryUseage();
void EhsPrintStackMemoryUseage(unsigned int thread_id);

#if defined(EHS_RUNTIME_LOGGER_ENABLED) || defined(EHS_COMPONENTS_CONSOLE_IO)

// make sure the printf buffer size isn't smaller than logger buffer
#define EHSH_LOG_MAX_MSG EHS_TARGET_PRINTF_BUF_SIZE

#ifndef EhsStdioPrintf
#define EhsStdioPrintf(...) arduino_serial_printf(__VA_ARGS__)
#endif
#ifndef EhsStdioSimplePrintf
#define EhsStdioSimplePrintf(...) arduino_serial_printf(__VA_ARGS__)
#endif

#else

#ifndef EHSStdioPrintf
#define EhsStdioPrintf(...)
#endif
#ifndef EhsStdioSimplePrintf
#define EhsStdioSimplePrintf(...)
#endif

#endif // EHS_RUNTIME_LOGGER_ENABLED

#endif // EHS_TARGET_LOGGER_H