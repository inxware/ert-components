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
 */

#include <termios.h>
#include <sys/select.h>
#include <errno.h>
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
 * Public API
 * ------------------------------------------------------------------------- */

int TgtUart_Stage0(int UART_num)
{
    if (UART_num < 0 || UART_num >= UART_COUNT)
        return TgtUART_INEXIST;
    s_fd[UART_num]         = -1;
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

    /* Baud rate */
    speed_t spd = baud_to_termios(baudrate);
    cfsetispeed(&tty, spd);
    cfsetospeed(&tty, spd);

    /* Data bits */
    tty.c_cflag &= ~CSIZE;
    switch (databits)
    {
        case 5: tty.c_cflag |= CS5; break;
        case 6: tty.c_cflag |= CS6; break;
        case 7: tty.c_cflag |= CS7; break;
        default:tty.c_cflag |= CS8; break;
    }

    /* Stop bits: stop_bits=0 → 1 stop bit, stop_bits=2 → 2 stop bits */
    if (stop_bits == 2)
        tty.c_cflag |= CSTOPB;
    else
        tty.c_cflag &= ~CSTOPB;

    /* Parity: 0=none, 1=odd, 2=even */
    if (parity == 0)
    {
        tty.c_cflag &= ~PARENB;
    }
    else
    {
        tty.c_cflag |= PARENB;
        if (parity == 1)
            tty.c_cflag |= PARODD;
        else
            tty.c_cflag &= ~PARODD;
    }

    /* Hardware flow control */
    if (flow_control == 3)
        tty.c_cflag |= CRTSCTS;
    else
        tty.c_cflag &= ~CRTSCTS;

    /* Raw mode — no echo, no signals, no special processing */
    tty.c_cflag |= (CREAD | CLOCAL);
    tty.c_lflag  = 0;
    tty.c_iflag  = 0;
    tty.c_oflag  = 0;

    /* Non-blocking read with 100 ms timeout */
    tty.c_cc[VMIN]  = 0;
    tty.c_cc[VTIME] = 1;

    if (tcsetattr(fd, TCSANOW, &tty) != 0)
    {
        EHSH_LOG_ERROR("SferaLabs UART%d: tcsetattr failed: %s", UART_num, strerror(errno));
        close(fd);
        return TgtUART_CONFIG_ERROR;
    }

    tcflush(fd, TCIOFLUSH);

    s_fd[UART_num]         = fd;
    s_configured[UART_num] = EHS_TRUE;
    EHSH_LOG_INFO("SferaLabs UART%d: started on %s at %d baud",
                  UART_num, s_dev_paths[UART_num], baudrate);
    return TgtUART_OK;
}

/**
 * Configure the port for RS-485 mode via Sfera Labs sysfs.
 * The Sfera Labs driver handles TX-enable automatically once this is set.
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
    EHSH_LOG_INFO("SferaLabs UART%d: RS-485 sysfs not available on this board", UART_num);
#endif
    return TgtUART_OK;
}

int TgtUart_Stop(int UART_num)
{
    if (UART_num < 0 || UART_num >= UART_COUNT)
        return TgtUART_INEXIST;
    if (!s_configured[UART_num])
        return TgtUART_NOTCONFIGURED;

    close(s_fd[UART_num]);
    s_fd[UART_num]         = -1;
    s_configured[UART_num] = EHS_FALSE;
    s_cb[UART_num]         = NULL;
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

int TgtUART_RecvPoll(int UART_num, char *payload, unsigned int length)
{
    if (UART_num < 0 || UART_num >= UART_COUNT) return TgtUART_INEXIST;
    if (!s_configured[UART_num])                return TgtUART_NOTCONFIGURED;
    if (!payload || length == 0)                return TgtUART_ERROR;

    /* Use select for a 100 ms non-blocking poll */
    fd_set rfds;
    struct timeval tv = { .tv_sec = 0, .tv_usec = 100000 };
    FD_ZERO(&rfds);
    FD_SET(s_fd[UART_num], &rfds);
    int r = select(s_fd[UART_num] + 1, &rfds, NULL, NULL, &tv);
    if (r <= 0)
        return TgtUART_OK; /* timeout or error — return 0 bytes */

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

/* Weak defaults — the serial component will override these */
__attribute__((weak)) void Common_UART_onReceive(char *recv_msg, int length)
{
    (void)recv_msg; (void)length;
}
__attribute__((weak)) void Common_UART_onSendComplete(int retCode)
{
    (void)retCode;
}
