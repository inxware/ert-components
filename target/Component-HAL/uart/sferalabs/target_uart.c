/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/**
 * @file target_uart.c
 * @brief Sfera Labs UART / RS-485 HAL — Linux termios serial I/O.
 *
 * The serial port itself (data path) uses standard POSIX termios, so there
 * is no dependency on Sfera Labs headers or libraries.
 *
 * RS-485 mode: the Sfera Labs kernel driver handles TX-enable automatically.
 * We optionally write the rs232_rs485_inv sysfs attribute to switch the port
 * into RS-485 mode when TgtUart_RS485Setup() is called.
 *
 * Receive model: identical to the generic linux HAL — a background pthread
 * blocks in select() with a 100 ms timeout and invokes the registered
 * callback whenever bytes arrive.
 */

#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/select.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include "globals.h"
#include "target_uart.h"
#include "hal_logger.h"

/* -------------------------------------------------------------------------
 * Per-port state
 * ------------------------------------------------------------------------- */

static const char *s_dev_paths[UART_COUNT] = {
    SFERALABS_UART0_DEV,
#if UART_COUNT > 1
    SFERALABS_UART1_DEV,
#endif
};

static int             s_fd[UART_COUNT];
static pthread_t       s_thread[UART_COUNT];
static volatile int    s_threadRun[UART_COUNT];
static uart_cb_func_t  s_cb[UART_COUNT];
static ehs_bool        s_configured[UART_COUNT];

/* -------------------------------------------------------------------------
 * Internal: map integer baud rate to termios constant
 * ------------------------------------------------------------------------- */

static speed_t baud_to_termios(int baud)
{
    switch (baud)
    {
        case 1200:   return B1200;
        case 2400:   return B2400;
        case 4800:   return B4800;
        case 9600:   return B9600;
        case 19200:  return B19200;
        case 38400:  return B38400;
        case 57600:  return B57600;
        case 115200: return B115200;
        case 230400: return B230400;
        default:     return B9600;
    }
}

/* -------------------------------------------------------------------------
 * Background receive thread
 * ------------------------------------------------------------------------- */

static void *uart_recv_thread(void *arg)
{
    int port = (int)(intptr_t)arg;
    char buf[UART_RX_BUFFER_SIZE];

    while (s_threadRun[port])
    {
        fd_set rfds;
        struct timeval tv = { .tv_sec = 0, .tv_usec = 100000 };
        FD_ZERO(&rfds);
        FD_SET(s_fd[port], &rfds);

        int r = select(s_fd[port] + 1, &rfds, NULL, NULL, &tv);
        if (r <= 0)
            continue;

        ssize_t n = read(s_fd[port], buf, sizeof(buf));
        if (n > 0 && s_cb[port])
            s_cb[port](buf, (int)n);
    }
    return NULL;
}

/* -------------------------------------------------------------------------
 * Public API
 * ------------------------------------------------------------------------- */

int TgtUart_Stage0(int UART_num)
{
    if (UART_num < 0 || UART_num >= UART_COUNT)
        return TgtUART_INEXIST;
    if (s_configured[UART_num])
        return TgtUART_INUSE;
    s_fd[UART_num]         = -1;
    s_threadRun[UART_num]  = 0;
    s_cb[UART_num]         = NULL;
    s_configured[UART_num] = EHS_FALSE;
    return TgtUART_OK;
}

int TgtUart_Start(int UART_num, int tx_io, int rx_io, int rts_io, int cts_io,
                  int baudrate, int databits, int parity, int stop_bits, int flow_control)
{
    (void)tx_io; (void)rx_io; (void)rts_io; (void)cts_io;

    if (UART_num < 0 || UART_num >= UART_COUNT)
        return TgtUART_INEXIST;
    if (s_configured[UART_num])
        return TgtUART_INUSE;

    int fd = open(s_dev_paths[UART_num], O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (fd < 0)
    {
        EHSH_LOG_ERROR("SferaLabs UART%d: open %s failed: %s",
                       UART_num, s_dev_paths[UART_num], strerror(errno));
        return TgtUART_ERROR;
    }

    struct termios tty;
    if (tcgetattr(fd, &tty) != 0)
    {
        EHSH_LOG_ERROR("SferaLabs UART%d: tcgetattr failed: %s", UART_num, strerror(errno));
        close(fd);
        return TgtUART_CONFIG_ERROR;
    }

    speed_t spd = baud_to_termios(baudrate);
    cfsetispeed(&tty, spd);
    cfsetospeed(&tty, spd);

    tty.c_cflag &= ~CSIZE;
    switch (databits)
    {
        case 5: tty.c_cflag |= CS5; break;
        case 6: tty.c_cflag |= CS6; break;
        case 7: tty.c_cflag |= CS7; break;
        default:tty.c_cflag |= CS8; break;
    }

    if (stop_bits == 2)
        tty.c_cflag |= CSTOPB;
    else
        tty.c_cflag &= ~CSTOPB;

    if (parity == 0)
    {
        tty.c_cflag &= ~PARENB;
    }
    else
    {
        tty.c_cflag |= PARENB;
        if (parity == 1) tty.c_cflag |= PARODD;
        else             tty.c_cflag &= ~PARODD;
    }

    if (flow_control == 3)
        tty.c_cflag |= CRTSCTS;
    else
        tty.c_cflag &= ~CRTSCTS;

    tty.c_cflag |= (CREAD | CLOCAL);
    tty.c_lflag  = 0;
    tty.c_iflag  = 0;
    tty.c_oflag  = 0;
    tty.c_cc[VMIN]  = 0;
    tty.c_cc[VTIME] = 0;

    if (tcsetattr(fd, TCSANOW, &tty) != 0)
    {
        EHSH_LOG_ERROR("SferaLabs UART%d: tcsetattr failed: %s", UART_num, strerror(errno));
        close(fd);
        return TgtUART_CONFIG_ERROR;
    }

    tcflush(fd, TCIOFLUSH);

    s_fd[UART_num]         = fd;
    s_configured[UART_num] = EHS_TRUE;

    s_threadRun[UART_num] = 1;
    if (pthread_create(&s_thread[UART_num], NULL,
                       uart_recv_thread, (void *)(intptr_t)UART_num) != 0)
    {
        EHSH_LOG_ERROR("SferaLabs UART%d: pthread_create failed: %s",
                       UART_num, strerror(errno));
        close(fd);
        s_fd[UART_num]         = -1;
        s_configured[UART_num] = EHS_FALSE;
        return TgtUART_ERROR;
    }

    EHSH_LOG_INFO("SferaLabs UART%d: started on %s at %d baud",
                  UART_num, s_dev_paths[UART_num], baudrate);
    return TgtUART_OK;
}

int TgtUart_Config(int UART_num, int baudrate, int databits, int parity,
                   int stop_bits, int flow_control)
{
    if (UART_num < 0 || UART_num >= UART_COUNT) return TgtUART_INEXIST;
    if (!s_configured[UART_num] || s_fd[UART_num] < 0) return TgtUART_OK;

    struct termios tty;
    if (tcgetattr(s_fd[UART_num], &tty) != 0) return TgtUART_CONFIG_ERROR;

    speed_t spd = baud_to_termios(baudrate);
    cfsetispeed(&tty, spd);
    cfsetospeed(&tty, spd);

    tty.c_cflag &= ~CSIZE;
    switch (databits) {
        case 5:  tty.c_cflag |= CS5; break;
        case 6:  tty.c_cflag |= CS6; break;
        case 7:  tty.c_cflag |= CS7; break;
        default: tty.c_cflag |= CS8; break;
    }

    if (stop_bits == 2) tty.c_cflag |= CSTOPB;
    else                tty.c_cflag &= ~CSTOPB;

    if (parity == 0) {
        tty.c_cflag &= ~PARENB;
    } else {
        tty.c_cflag |= PARENB;
        if (parity == 1) tty.c_cflag |= PARODD;
        else             tty.c_cflag &= ~PARODD;
    }

    if (flow_control == 3) tty.c_cflag |= CRTSCTS;
    else                   tty.c_cflag &= ~CRTSCTS;

    return (tcsetattr(s_fd[UART_num], TCSANOW, &tty) == 0) ? TgtUART_OK : TgtUART_CONFIG_ERROR;
}

/**
 * Configure the port for RS-485 mode via the Sfera Labs kernel driver sysfs.
 * The driver handles TX-enable automatically once this attribute is written.
 * Call after TgtUart_Start().
 */
int TgtUart_RS485Setup(int UART_num)
{
    if (UART_num < 0 || UART_num >= UART_COUNT)
        return TgtUART_INEXIST;
    if (!s_configured[UART_num])
        return TgtUART_NOTCONFIGURED;

#if defined(SFERALABS_SERIAL_RS485_FMT)
    /* Write "0" = RS-485 mode (no inversion), TX-enable handled by driver */
    if (sferalabs_sysfs_write(SFERALABS_SERIAL_RS485_FMT, "0", 1) != 0)
    {
        EHSH_LOG_ERROR("SferaLabs UART%d: RS-485 sysfs write failed", UART_num);
        return TgtUART_ERROR;
    }
    EHSH_LOG_INFO("SferaLabs UART%d: RS-485 mode enabled via sysfs", UART_num);
#else
    EHSH_LOG_INFO("SferaLabs UART%d: RS-485 sysfs not configured for this board", UART_num);
#endif
    return TgtUART_OK;
}

int TgtUart_Stop(int UART_num)
{
    if (UART_num < 0 || UART_num >= UART_COUNT)
        return TgtUART_INEXIST;
    if (!s_configured[UART_num])
        return TgtUART_NOTCONFIGURED;

    s_threadRun[UART_num] = 0;
    pthread_join(s_thread[UART_num], NULL);

    close(s_fd[UART_num]);
    s_fd[UART_num]         = -1;
    s_cb[UART_num]         = NULL;
    s_configured[UART_num] = EHS_FALSE;
    EHSH_LOG_INFO("SferaLabs UART%d: stopped", UART_num);
    return TgtUART_OK;
}

int TgtUart_Send(int UART_num, char *payload, unsigned int length)
{
    if (UART_num < 0 || UART_num >= UART_COUNT) return TgtUART_INEXIST;
    if (!s_configured[UART_num])                return TgtUART_NOTCONFIGURED;
    if (!payload || length == 0)                return TgtUART_ERROR;

    ssize_t written = write(s_fd[UART_num], payload, length);
    if (written < 0)
    {
        EHSH_LOG_ERROR("SferaLabs UART%d: write failed: %s", UART_num, strerror(errno));
        return TgtUART_ERROR;
    }
    tcdrain(s_fd[UART_num]);
    return TgtUART_OK;
}

void TgtUART_SendThread(int UART_num, char *payload, unsigned int length)
{
    int ret = TgtUart_Send(UART_num, payload, length);
#ifdef EHS_UART_SUPPORT
    Common_UART_onSendComplete(ret);
#else
    (void)ret;
#endif
}

int TgtUART_SendInThread(int UART_num)
{
    (void)UART_num;
    return TgtUART_OK;
}

int TgtUART_RecvPoll(int UART_num, char *payload, unsigned int length)
{
    if (UART_num < 0 || UART_num >= UART_COUNT) return TgtUART_INEXIST;
    if (!s_configured[UART_num])                return TgtUART_NOTCONFIGURED;
    if (!payload || length == 0)                return TgtUART_ERROR;

    fd_set rfds;
    struct timeval tv = { .tv_sec = 0, .tv_usec = 100000 };
    FD_ZERO(&rfds);
    FD_SET(s_fd[UART_num], &rfds);
    int r = select(s_fd[UART_num] + 1, &rfds, NULL, NULL, &tv);
    if (r <= 0)
        return TgtUART_OK;

    ssize_t n = read(s_fd[UART_num], payload, length);
    if (n < 0)
    {
        EHSH_LOG_ERROR("SferaLabs UART%d: read failed: %s", UART_num, strerror(errno));
        return TgtUART_ERROR;
    }
    if (n > 0 && s_cb[UART_num])
        s_cb[UART_num](payload, (int)n);
    return (int)n;
}

int TgtUART_Intr_register(int UART_num, uart_cb_func_t cb_func)
{
    if (UART_num < 0 || UART_num >= UART_COUNT) return TgtUART_INEXIST;
    if (!cb_func)                               return TgtUART_INVALID_CALLBACK;
    s_cb[UART_num] = cb_func;
    return TgtUART_OK;
}

int TgtUart_SetComPort(int UART_num, int com_port_number)
{
    /* COM port numbers are a Windows concept; no-op on Sfera Labs targets. */
    (void)UART_num; (void)com_port_number;
    return TgtUART_OK;
}

int TgtUart_SetDevicePath(int UART_num, const char *path)
{
    /* Sfera Labs device paths are compile-time configured via SFERALABS_UARTx_DEV.
     * Runtime override is not supported on this target. */
    (void)UART_num; (void)path;
    return TgtUART_OK;
}

