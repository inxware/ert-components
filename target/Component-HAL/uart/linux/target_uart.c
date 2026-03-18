/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license.
 ***************************************************************/

/**
 * @file target_uart.c  (linux)
 * @brief Generic Linux POSIX serial UART HAL implementation.
 *
 * Opens /dev/ttyUSBx (or any path set via TgtUart_SetDevicePath) using
 * standard POSIX termios.  A background pthread is spawned per port in
 * TgtUart_Start(); it blocks on read() with a 100 ms select() timeout
 * and calls the registered callback whenever bytes arrive.
 *
 * Receive model
 * -------------
 * The receive thread blocks in select() waiting for the file descriptor
 * to become readable.  It wakes immediately when bytes arrive — latency
 * is OS scheduling jitter (typically <1 ms), NOT the 100 ms timeout.
 * The 100 ms timeout is only the worst-case empty-loop interval when
 * there is no incoming traffic.  This mirrors the interrupt-driven
 * behaviour of the ESP32 target and allows the LoRaWAN sendWaitUntilComplete()
 * spin loop to work correctly with the WIO-E5 modem.
 */

#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/select.h>
#include <errno.h>
#include <string.h>
#include <sched.h>
#include <stdio.h>
#include "globals.h"
#include "target_uart.h"
#include "hal_logger.h"

/* -----------------------------------------------------------------------
 * Pin arrays — all -1 (device-node serial ports have no GPIO pin numbers)
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
 * Device path table — mutable so TgtUart_SetDevicePath() can override
 * ----------------------------------------------------------------------- */

#define LINUX_DEV_PATH_LEN 64
static char s_dev_paths[UART_COUNT][LINUX_DEV_PATH_LEN] = {
    LINUX_UART0_DEV,
#if UART_COUNT > 1
    LINUX_UART1_DEV,
#endif
#if UART_COUNT > 2
    LINUX_UART2_DEV,
#endif
#if UART_COUNT > 3
    LINUX_UART3_DEV,
#endif
};

/* -----------------------------------------------------------------------
 * Per-port state
 * ----------------------------------------------------------------------- */

static int             s_fd[UART_COUNT];
static pthread_t       s_thread[UART_COUNT];
static volatile int    s_threadRun[UART_COUNT];
static uart_cb_func_t  s_cb[UART_COUNT];
static ehs_bool        s_configured[UART_COUNT];

/* -----------------------------------------------------------------------
 * Internal: map integer baud rate to termios speed_t
 * ----------------------------------------------------------------------- */

static speed_t baud_to_termios(int baud)
{
    switch (baud)
    {
        case 1200:    return B1200;
        case 2400:    return B2400;
        case 4800:    return B4800;
        case 9600:    return B9600;
        case 19200:   return B19200;
        case 38400:   return B38400;
        case 57600:   return B57600;
        case 115200:  return B115200;
        case 230400:  return B230400;
        case 460800:  return B460800;
        case 921600:  return B921600;
        default:      return B9600;
    }
}

/* -----------------------------------------------------------------------
 * Background receive thread
 *
 * Blocks in select() with 100 ms timeout.  Wakes immediately when data
 * arrives and calls the registered callback.
 * ----------------------------------------------------------------------- */

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
            continue; /* timeout or signal — loop */

        ssize_t n = read(s_fd[port], buf, sizeof(buf));
        if (n > 0 && s_cb[port])
            s_cb[port](buf, (int)n);
    }
    return NULL;
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

    s_fd[UART_num]         = -1;
    s_threadRun[UART_num]  = 0;
    s_cb[UART_num]         = NULL;
    s_configured[UART_num] = EHS_FALSE;
    return TgtUART_OK;
}

int TgtUart_SetDevicePath(int UART_num, const char *path)
{
    if (UART_num < 0 || UART_num >= UART_COUNT) return TgtUART_INEXIST;
    if (s_configured[UART_num])                 return TgtUART_INUSE;
    if (!path || path[0] == '\0')               return TgtUART_OK;
    snprintf(s_dev_paths[UART_num], LINUX_DEV_PATH_LEN, "%s", path);
    return TgtUART_OK;
}

int TgtUart_SetComPort(int UART_num, int com_port_number)
{
    /* COM port numbers are a Windows concept; no-op on Linux. */
    (void)UART_num; (void)com_port_number;
    return TgtUART_OK;
}

int TgtUart_Start(int UART_num, int tx_io, int rx_io, int rts_io, int cts_io,
                  int baudrate, int databits, int parity, int stop_bits,
                  int flow_control)
{
    (void)tx_io; (void)rx_io; (void)rts_io; (void)cts_io;

    if (UART_num < 0 || UART_num >= UART_COUNT)
        return TgtUART_INEXIST;
    if (s_configured[UART_num])
        return TgtUART_INUSE;

    int fd = open(s_dev_paths[UART_num], O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (fd < 0)
    {
        EHSH_LOG_ERROR("Linux UART%d: open %s failed: %s",
                       UART_num, s_dev_paths[UART_num], strerror(errno));
        return TgtUART_ERROR;
    }

    struct termios tty;
    if (tcgetattr(fd, &tty) != 0)
    {
        EHSH_LOG_ERROR("Linux UART%d: tcgetattr failed: %s", UART_num, strerror(errno));
        close(fd);
        return TgtUART_CONFIG_ERROR;
    }

    speed_t spd = baud_to_termios(baudrate);
    cfsetispeed(&tty, spd);
    cfsetospeed(&tty, spd);

    /* Data bits */
    tty.c_cflag &= ~CSIZE;
    switch (databits)
    {
        case 5:  tty.c_cflag |= CS5; break;
        case 6:  tty.c_cflag |= CS6; break;
        case 7:  tty.c_cflag |= CS7; break;
        default: tty.c_cflag |= CS8; break;
    }

    /* Stop bits: 0→1, 2→2 */
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
        if (parity == 1) tty.c_cflag |= PARODD;
        else             tty.c_cflag &= ~PARODD;
    }

    /* Hardware flow control: 3 = RTS+CTS */
    if (flow_control == 3)
        tty.c_cflag |= CRTSCTS;
    else
        tty.c_cflag &= ~CRTSCTS;

    /* Raw mode */
    tty.c_cflag |= (CREAD | CLOCAL);
    tty.c_lflag  = 0;
    tty.c_iflag  = 0;
    tty.c_oflag  = 0;
    tty.c_cc[VMIN]  = 0;
    tty.c_cc[VTIME] = 0;

    if (tcsetattr(fd, TCSANOW, &tty) != 0)
    {
        EHSH_LOG_ERROR("Linux UART%d: tcsetattr failed: %s", UART_num, strerror(errno));
        close(fd);
        return TgtUART_CONFIG_ERROR;
    }

    tcflush(fd, TCIOFLUSH);

    s_fd[UART_num]         = fd;
    s_configured[UART_num] = EHS_TRUE;

    /* Start background receive thread */
    s_threadRun[UART_num] = 1;
    if (pthread_create(&s_thread[UART_num], NULL,
                       uart_recv_thread, (void *)(intptr_t)UART_num) != 0)
    {
        EHSH_LOG_ERROR("Linux UART%d: pthread_create failed: %s",
                       UART_num, strerror(errno));
        close(fd);
        s_fd[UART_num]         = -1;
        s_configured[UART_num] = EHS_FALSE;
        return TgtUART_ERROR;
    }

    EHSH_LOG_INFO("Linux UART%d: started on %s at %d baud",
                  UART_num, s_dev_paths[UART_num], baudrate);
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

int TgtUart_RS485Setup(int UART_num)
{
    if (UART_num < 0 || UART_num >= UART_COUNT) return TgtUART_INEXIST;
    if (!s_configured[UART_num])               return TgtUART_NOTCONFIGURED;
    /* RS-485 TX-enable requires platform-specific sysfs or ioctl.
     * Use a USB-RS485 adapter with automatic TX-enable on generic Linux. */
    EHSH_LOG_INFO("Linux UART%d: RS-485 auto TX-enable not configured; use an adapter with auto TX-enable",
                  UART_num);
    return TgtUART_OK;
}

int TgtUart_Stop(int UART_num)
{
    if (UART_num < 0 || UART_num >= UART_COUNT) return TgtUART_INEXIST;
    if (!s_configured[UART_num])               return TgtUART_NOTCONFIGURED;

    s_threadRun[UART_num] = 0;
    pthread_join(s_thread[UART_num], NULL);

    close(s_fd[UART_num]);
    s_fd[UART_num]         = -1;
    s_cb[UART_num]         = NULL;
    s_configured[UART_num] = EHS_FALSE;
    EHSH_LOG_INFO("Linux UART%d: stopped", UART_num);
    return TgtUART_OK;
}

int TgtUart_Send(int UART_num, char *payload, unsigned int length)
{
    if (UART_num < 0 || UART_num >= UART_COUNT) return TgtUART_INEXIST;
    if (!s_configured[UART_num])               return TgtUART_NOTCONFIGURED;
    if (!payload || length == 0)               return TgtUART_ERROR;

    ssize_t written = write(s_fd[UART_num], payload, length);
    if (written < 0)
    {
        EHSH_LOG_ERROR("Linux UART%d: write failed: %s", UART_num, strerror(errno));
        return TgtUART_ERROR;
    }
    tcdrain(s_fd[UART_num]);
    return TgtUART_OK;
}

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

int TgtUART_RecvPoll(int UART_num, char *payload, unsigned int length)
{
    if (UART_num < 0 || UART_num >= UART_COUNT) return TgtUART_INEXIST;
    if (!s_configured[UART_num])               return TgtUART_NOTCONFIGURED;
    if (!payload || length == 0)               return TgtUART_ERROR;

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
        EHSH_LOG_ERROR("Linux UART%d: read failed: %s", UART_num, strerror(errno));
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

/* Weak defaults */
__attribute__((weak)) void Common_UART_onReceive(char *recv_msg, int length)
{
    (void)recv_msg; (void)length;
}
__attribute__((weak)) void Common_UART_onSendComplete(int retCode)
{
    (void)retCode;
}
