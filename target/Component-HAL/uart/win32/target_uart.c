/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license.
 ***************************************************************/

/**
 * @file target_uart.c  (win32)
 * @brief Win32 serial COM-port UART HAL implementation.
 *
 * Provides the standard target_uart.h API over Win32 serial ports
 * (COM1, COM3, etc.) using CreateFile / ReadFile / WriteFile.
 *
 * A background receive thread is spawned per port in TgtUart_Start().
 * The thread blocks on ReadFile and invokes the callback registered via
 * TgtUART_Intr_register() whenever bytes arrive.
 *
 * ReadIntervalTimeout is set to 3.5 character-times at the configured baud
 * rate (MODBUS RTU frame-boundary detection): ReadFile returns as soon as a
 * gap longer than that elapses between received bytes, delivering one frame
 * per callback call.  ReadTotalTimeoutConstant=100 ms provides the fallback
 * that lets the thread exit cleanly when TgtUart_Stop() clears s_threadRun.
 *
 * COM port assignment:
 *   UART_num 0 → WIN32_UART0_COM_PORT  (default: \\\\.\\COM3)
 *   UART_num 1 → WIN32_UART1_COM_PORT  (default: \\\\.\\COM4)
 * Override these macros before including target_uart.h or via CFLAGS.
 *
 * TgtUART_RecvPoll() is also provided for use by the generic inx-uart
 * component.  Do not mix RecvPoll with the background thread on the same
 * port — use TgtUART_Intr_register() for interrupt-style reception and
 * RecvPoll only when no callback is registered.
 *
 * TgtUART_SendThread() / TgtUART_SendInThread() perform a synchronous
 * write on the calling thread; the "threaded send" abstraction is not
 * required on Windows since WriteFile returns once the bytes are queued.
 */

#include <stdio.h>
#include <string.h>
#include "globals.h"
#include "target_uart.h"
#include "hal_logger.h"

/* -----------------------------------------------------------------------
 * Pin arrays — not applicable on Win32 (COM ports have no GPIO pin numbers).
 * Defined here to satisfy the extern declarations in target_uart.h; all
 * values are -1 (unused).  inx-uart_config and inx-modbus_config reference
 * these arrays when passing pin arguments to TgtUart_Start, which ignores
 * them on this platform.
 * ----------------------------------------------------------------------- */

const int gTargetUartPinTx[]  = {
    TARGET_UART_PIN_TX_PORT(0),
    TARGET_UART_PIN_TX_PORT(1),
    TARGET_UART_PIN_TX_PORT(2),
    TARGET_UART_PIN_TX_PORT(3),
};

const int gTargetUartPinRx[]  = {
    TARGET_UART_PIN_RX_PORT(0),
    TARGET_UART_PIN_RX_PORT(1),
    TARGET_UART_PIN_RX_PORT(2),
    TARGET_UART_PIN_RX_PORT(3),
};

const int gTargetUartPinCts[] = {
    TARGET_UART_PIN_CTS_PORT(0),
    TARGET_UART_PIN_CTS_PORT(1),
    TARGET_UART_PIN_CTS_PORT(2),
    TARGET_UART_PIN_CTS_PORT(3),
};

const int gTargetUartPinRts[] = {
    TARGET_UART_PIN_RTS_PORT(0),
    TARGET_UART_PIN_RTS_PORT(1),
    TARGET_UART_PIN_RTS_PORT(2),
    TARGET_UART_PIN_RTS_PORT(3),
};

/* -----------------------------------------------------------------------
 * COM port name table — indexed by UART_num
 * ----------------------------------------------------------------------- */

/* Mutable so that TgtUart_SetComPort() can override at runtime */
#define WIN32_COM_PORT_NAME_LEN 16
static char s_port_names[UART_COUNT][WIN32_COM_PORT_NAME_LEN] = {
    WIN32_UART0_COM_PORT,
#if UART_COUNT > 1
    WIN32_UART1_COM_PORT,
#endif
#if UART_COUNT > 2
    WIN32_UART2_COM_PORT,
#endif
#if UART_COUNT > 3
    WIN32_UART3_COM_PORT,
#endif
};

/* -----------------------------------------------------------------------
 * Per-port state
 * ----------------------------------------------------------------------- */

static HANDLE           s_hPort[UART_COUNT];
static HANDLE           s_hThread[UART_COUNT];
static volatile BOOL    s_threadRun[UART_COUNT];
static uart_cb_func_t   s_cb[UART_COUNT];
static ehs_bool         s_configured[UART_COUNT];

/* -----------------------------------------------------------------------
 * Background receive thread
 *
 * Loops until s_threadRun[port] is cleared by TgtUart_Stop().
 * ReadFile blocks for up to READ_TIMEOUT_MS then returns 0 bytes.
 * ----------------------------------------------------------------------- */

#define READ_TIMEOUT_MS 100

static DWORD WINAPI uart_recv_thread(LPVOID lpParam)
{
    int port = (int)(intptr_t)lpParam;
    char buf[UART_RX_BUFFER_SIZE];

    while (s_threadRun[port])
    {
        DWORD bytesRead = 0;
        BOOL  ok = ReadFile(s_hPort[port], buf, (DWORD)sizeof(buf), &bytesRead, NULL);
        if (ok && bytesRead > 0 && s_cb[port])
            s_cb[port](buf, (int)bytesRead);
    }
    return 0;
}

/* -----------------------------------------------------------------------
 * Public API
 * ----------------------------------------------------------------------- */

int TgtUart_Stage0(int UART_num)
{
    if (UART_num < 0 || UART_num >= UART_COUNT)
        return TgtUART_INEXIST;

    if (s_configured[UART_num])
        return TgtUART_INUSE;

    s_hPort[UART_num]      = INVALID_HANDLE_VALUE;
    s_hThread[UART_num]    = NULL;
    s_threadRun[UART_num]  = FALSE;
    s_cb[UART_num]         = NULL;
    s_configured[UART_num] = EHS_FALSE;
    return TgtUART_OK;
}

int TgtUart_SetComPort(int UART_num, int com_port_number)
{
    if (UART_num < 0 || UART_num >= UART_COUNT) return TgtUART_INEXIST;
    if (s_configured[UART_num])                 return TgtUART_INUSE;
    if (com_port_number <= 0)                   return TgtUART_OK; /* 0 = keep default */
    snprintf(s_port_names[UART_num], WIN32_COM_PORT_NAME_LEN,
             "\\\\.\\COM%d", com_port_number);
    return TgtUART_OK;
}

int TgtUart_SetDevicePath(int UART_num, const char *path)
{
    /* Device paths are a Linux concept; no-op on Win32. */
    (void)UART_num; (void)path;
    return TgtUART_OK;
}

int TgtUart_Start(int UART_num, int tx_io, int rx_io, int rts_io, int cts_io,
                  int baudrate, int databits, int parity, int stop_bits,
                  int flow_control)
{
    /* Pin numbers are not applicable to Windows COM ports */
    (void)tx_io; (void)rx_io; (void)rts_io; (void)cts_io;

    if (UART_num < 0 || UART_num >= UART_COUNT)
        return TgtUART_INEXIST;
    if (s_configured[UART_num])
        return TgtUART_INUSE;

    /* Open the COM port.  The \\.\ prefix is required for COM numbers > 9. */
    HANDLE h = CreateFileA(
        s_port_names[UART_num],
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    if (h == INVALID_HANDLE_VALUE)
    {
        EHSH_LOG_ERROR("Win32 UART%d: cannot open %s (err %lu)",
                       UART_num, s_port_names[UART_num], (unsigned long)GetLastError());
        return TgtUART_ERROR;
    }

    /* Inter-character gap: 3.5 × character-time, per MODBUS RTU frame-boundary
     * specification.  Uses 11 bits/char (start + 8 data + parity + stop) as the
     * worst-case character width.  ReadFile returns immediately when a gap longer
     * than this elapses between received bytes, delivering exactly one frame per
     * callback invocation.  Minimum 1 ms (Windows timer resolution floor).
     *
     * ReadTotalTimeoutConstant is kept at READ_TIMEOUT_MS so the receive thread
     * can still exit cleanly when s_threadRun is cleared (the thread unblocks
     * after at most READ_TIMEOUT_MS even when no data arrives). */
    DWORD gap_ms = (38500U + (DWORD)baudrate - 1U) / (DWORD)baudrate;
    if (gap_ms < 1U) gap_ms = 1U;

    COMMTIMEOUTS to;
    memset(&to, 0, sizeof(to));
    to.ReadIntervalTimeout         = gap_ms;
    to.ReadTotalTimeoutConstant    = READ_TIMEOUT_MS;
    to.ReadTotalTimeoutMultiplier  = 0;
    to.WriteTotalTimeoutConstant   = 2000;
    to.WriteTotalTimeoutMultiplier = 0;
    SetCommTimeouts(h, &to);

    /* Build DCB from the standard parameters */
    DCB dcb;
    memset(&dcb, 0, sizeof(dcb));
    dcb.DCBlength = sizeof(dcb);
    if (!GetCommState(h, &dcb))
    {
        EHSH_LOG_ERROR("Win32 UART%d: GetCommState failed (err %lu)",
                       UART_num, (unsigned long)GetLastError());
        CloseHandle(h);
        return TgtUART_CONFIG_ERROR;
    }

    /* Baud rate */
    dcb.BaudRate = (DWORD)baudrate;

    /* Data bits (5-8) */
    dcb.ByteSize = (BYTE)((databits >= 5 && databits <= 8) ? databits : 8);

    /* Stop bits: 0→1, 2→2 (1.5 not supported; falls through to 1) */
    dcb.StopBits = (stop_bits == 2) ? TWOSTOPBITS : ONESTOPBIT;

    /* Parity: 0=none, 1=odd, 2=even */
    switch (parity)
    {
        case 1:
            dcb.Parity  = ODDPARITY;
            dcb.fParity = TRUE;
            break;
        case 2:
            dcb.Parity  = EVENPARITY;
            dcb.fParity = TRUE;
            break;
        default:
            dcb.Parity  = NOPARITY;
            dcb.fParity = FALSE;
            break;
    }

    /* Hardware flow control: 3 = RTS+CTS */
    if (flow_control == 3)
    {
        dcb.fRtsControl  = RTS_CONTROL_HANDSHAKE;
        dcb.fOutxCtsFlow = TRUE;
    }
    else
    {
        dcb.fRtsControl  = RTS_CONTROL_DISABLE;
        dcb.fOutxCtsFlow = FALSE;
    }

    dcb.fBinary = TRUE;

    if (!SetCommState(h, &dcb))
    {
        EHSH_LOG_ERROR("Win32 UART%d: SetCommState failed (err %lu)",
                       UART_num, (unsigned long)GetLastError());
        CloseHandle(h);
        return TgtUART_CONFIG_ERROR;
    }

    /* Flush any stale bytes */
    PurgeComm(h, PURGE_RXCLEAR | PURGE_TXCLEAR);

    s_hPort[UART_num]      = h;
    s_configured[UART_num] = EHS_TRUE;

    /* Start background receive thread */
    s_threadRun[UART_num] = TRUE;
    s_hThread[UART_num] = CreateThread(
        NULL, 0, uart_recv_thread,
        (LPVOID)(intptr_t)UART_num, 0, NULL
    );
    if (!s_hThread[UART_num])
    {
        EHSH_LOG_ERROR("Win32 UART%d: CreateThread failed (err %lu)",
                       UART_num, (unsigned long)GetLastError());
        CloseHandle(h);
        s_hPort[UART_num]      = INVALID_HANDLE_VALUE;
        s_configured[UART_num] = EHS_FALSE;
        return TgtUART_ERROR;
    }

    EHSH_LOG_INFO("Win32 UART%d: started on %s at %d baud",
                  UART_num, s_port_names[UART_num], baudrate);
    return TgtUART_OK;
}

/*
 * Reconfigure a running UART port.
 * No-op (returns OK) if the port is not yet open — the new settings will be
 * picked up from the gUART* globals at the next TgtUart_Start call.
 */
int TgtUart_Config(int UART_num, int baudrate, int databits, int parity,
                   int stop_bits, int flow_control)
{
    if (UART_num < 0 || UART_num >= UART_COUNT)          return TgtUART_INEXIST;
    if (!s_configured[UART_num] ||
        s_hPort[UART_num] == INVALID_HANDLE_VALUE)        return TgtUART_OK;

    DCB dcb;
    memset(&dcb, 0, sizeof(dcb));
    dcb.DCBlength = sizeof(dcb);
    if (!GetCommState(s_hPort[UART_num], &dcb))           return TgtUART_CONFIG_ERROR;

    dcb.BaudRate  = (DWORD)baudrate;
    dcb.ByteSize  = (BYTE)((databits >= 5 && databits <= 8) ? databits : 8);
    dcb.StopBits  = (stop_bits == 2) ? TWOSTOPBITS : ONESTOPBIT;
    switch (parity) {
        case 1:  dcb.Parity = ODDPARITY;  dcb.fParity = TRUE; break;
        case 2:  dcb.Parity = EVENPARITY; dcb.fParity = TRUE; break;
        default: dcb.Parity = NOPARITY;   dcb.fParity = FALSE; break;
    }
    if (flow_control == 3) {
        dcb.fRtsControl  = RTS_CONTROL_HANDSHAKE;
        dcb.fOutxCtsFlow = TRUE;
    } else {
        dcb.fRtsControl  = RTS_CONTROL_DISABLE;
        dcb.fOutxCtsFlow = FALSE;
    }
    dcb.fBinary = TRUE;

    /* Also update ReadIntervalTimeout to match the new baud rate */
    COMMTIMEOUTS to;
    memset(&to, 0, sizeof(to));
    if (GetCommTimeouts(s_hPort[UART_num], &to))
    {
        DWORD gap_ms = (38500U + (DWORD)baudrate - 1U) / (DWORD)baudrate;
        if (gap_ms < 1U) gap_ms = 1U;
        to.ReadIntervalTimeout = gap_ms;
        SetCommTimeouts(s_hPort[UART_num], &to);
    }

    return SetCommState(s_hPort[UART_num], &dcb) ? TgtUART_OK : TgtUART_CONFIG_ERROR;
}

int TgtUart_RS485Setup(int UART_num)
{
    if (UART_num < 0 || UART_num >= UART_COUNT)
        return TgtUART_INEXIST;
    if (!s_configured[UART_num])
        return TgtUART_NOTCONFIGURED;
    /* RS-485 TX-enable is not directly controllable via Win32 serial API.
     * Use a USB-RS485 adapter with automatic TX-enable instead. */
    EHSH_LOG_INFO("Win32 UART%d: RS-485 TX-enable not supported; use an adapter with auto TX-enable",
                  UART_num);
    return TgtUART_OK;
}

int TgtUart_Stop(int UART_num)
{
    if (UART_num < 0 || UART_num >= UART_COUNT)
        return TgtUART_INEXIST;
    if (!s_configured[UART_num])
        return TgtUART_NOTCONFIGURED;

    /* Signal the receive thread to exit and wait for it */
    s_threadRun[UART_num] = FALSE;
    if (s_hThread[UART_num])
    {
        WaitForSingleObject(s_hThread[UART_num], (DWORD)(READ_TIMEOUT_MS * 3));
        CloseHandle(s_hThread[UART_num]);
        s_hThread[UART_num] = NULL;
    }

    CloseHandle(s_hPort[UART_num]);
    s_hPort[UART_num]      = INVALID_HANDLE_VALUE;
    s_cb[UART_num]         = NULL;
    s_configured[UART_num] = EHS_FALSE;
    EHSH_LOG_INFO("Win32 UART%d: stopped", UART_num);
    return TgtUART_OK;
}

int TgtUart_Send(int UART_num, char *payload, unsigned int length)
{
    if (UART_num < 0 || UART_num >= UART_COUNT) return TgtUART_INEXIST;
    if (!s_configured[UART_num])               return TgtUART_NOTCONFIGURED;
    if (!payload || length == 0)               return TgtUART_ERROR;

    DWORD written = 0;
    if (!WriteFile(s_hPort[UART_num], payload, (DWORD)length, &written, NULL))
    {
        EHSH_LOG_ERROR("Win32 UART%d: WriteFile failed (err %lu)",
                       UART_num, (unsigned long)GetLastError());
        return TgtUART_ERROR;
    }
    return TgtUART_OK;
}

/**
 * TgtUART_SendThread / TgtUART_SendInThread
 *
 * On Win32 the WriteFile call in TgtUart_Send() is already non-blocking
 * from the caller's perspective (bytes are buffered by the driver), so
 * the threaded-send abstraction is satisfied by a direct synchronous write.
 * The send-complete callback is invoked inline after the write.
 */
void TgtUART_SendThread(int UART_num, char *payload, unsigned int length)
{
    int ret = TgtUart_Send(UART_num, payload, length);
    Common_UART_onSendComplete(ret);
}

int TgtUART_SendInThread(int UART_num)
{
    (void)UART_num;
    return TgtUART_OK;
}

/**
 * TgtUART_RecvPoll — blocking poll with READ_TIMEOUT_MS timeout.
 *
 * Note: if TgtUART_Intr_register() has been called the background receive
 * thread is already consuming data from the port.  Do not mix RecvPoll
 * with the interrupt-callback mode on the same port.
 */
int TgtUART_RecvPoll(int UART_num, char *payload, unsigned int length)
{
    if (UART_num < 0 || UART_num >= UART_COUNT) return TgtUART_INEXIST;
    if (!s_configured[UART_num])               return TgtUART_NOTCONFIGURED;
    if (!payload || length == 0)               return TgtUART_ERROR;

    DWORD bytesRead = 0;
    if (!ReadFile(s_hPort[UART_num], payload, (DWORD)length, &bytesRead, NULL))
        return TgtUART_ERROR;

    if (bytesRead > 0 && s_cb[UART_num])
        s_cb[UART_num](payload, (int)bytesRead);

    return (int)bytesRead;
}

int TgtUART_Intr_register(int UART_num, uart_cb_func_t cb_func)
{
    if (UART_num < 0 || UART_num >= UART_COUNT) return TgtUART_INEXIST;
    if (!cb_func)                               return TgtUART_INVALID_CALLBACK;
    s_cb[UART_num] = cb_func;
    return TgtUART_OK;
}

/* Weak defaults — the serial component overrides these */
__attribute__((weak)) void Common_UART_onReceive(char *recv_msg, int length)
{
    (void)recv_msg; (void)length;
}
__attribute__((weak)) void Common_UART_onSendComplete(int retCode)
{
    (void)retCode;
}
