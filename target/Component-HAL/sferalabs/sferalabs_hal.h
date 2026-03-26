/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/**
 * @file sferalabs_hal.h
 * @brief Shared sysfs path definitions for Sfera Labs hardware boards.
 *
 * Set EHS_SFERALABS_BOARD_<VARIANT> in the platform config.mk to select the board.
 * Supported variants:
 *   EHS_SFERALABS_BOARD_IONOPI       - Iono Pi (RPi industrial PLC)
 *   EHS_SFERALABS_BOARD_IONOPIMAX    - Iono Pi Max (RPi CM PLC) [default]
 *   EHS_SFERALABS_BOARD_STRATOPI     - Strato Pi (Base/UPS/CAN)
 *   EHS_SFERALABS_BOARD_STRATOPIMAX  - Strato Pi Max (CM4/CM5 server)
 *   EHS_SFERALABS_BOARD_EXOSENSEPI   - Exo Sense Pi (multi-sensor)
 *
 * All hardware access is via sysfs file I/O only — no Sfera Labs headers or
 * libraries are used, so there is no GPL licence conflict with LGPLv3.
 */

#ifndef SFERALABS_HAL_H
#define SFERALABS_HAL_H

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

/* Default to Iono Pi Max if no board variant is specified */
#if !defined(EHS_SFERALABS_BOARD_IONOPI)     && \
    !defined(EHS_SFERALABS_BOARD_IONOPIMAX)  && \
    !defined(EHS_SFERALABS_BOARD_STRATOPI)   && \
    !defined(EHS_SFERALABS_BOARD_STRATOPIMAX)&& \
    !defined(EHS_SFERALABS_BOARD_EXOSENSEPI)
#define EHS_SFERALABS_BOARD_IONOPIMAX
#endif

/* Common buffer size for sysfs path construction */
#define SFERALABS_SYSFS_BUF_SIZE 256

/* -------------------------------------------------------------------------
 * Iono Pi
 * Digital outputs: relays o1-o4, open collectors oc1-oc2
 * Digital inputs:  di1-di4
 * Analog inputs:   ai1-ai6 (millivolts)
 * No analog outputs.
 * pin_id 1-4 = relay, pin_id 101-102 = open collector
 * ------------------------------------------------------------------------- */
#if defined(EHS_SFERALABS_BOARD_IONOPI)

#define SFERALABS_CLASS_PATH            "/sys/class/ionopi"
#define SFERALABS_DO_RELAY_FMT          SFERALABS_CLASS_PATH "/relay/o%d"
#define SFERALABS_DO_OC_FMT             SFERALABS_CLASS_PATH "/open_coll/oc%d"
#define SFERALABS_DI_FMT                SFERALABS_CLASS_PATH "/digital_in/di%d"
#define SFERALABS_DI_DEB_FMT            SFERALABS_CLASS_PATH "/digital_in/di%d_deb"
#define SFERALABS_AI_VOLT_FMT           SFERALABS_CLASS_PATH "/analog_in/ai%d_mv"
#define SFERALABS_AI_VOLT_CHANNELS      6
#define SFERALABS_AI_CURR_CHANNELS      0
#define SFERALABS_AI_TEMP_CHANNELS      0
#define SFERALABS_AO_CHANNELS           0
#define SFERALABS_DO_RELAY_COUNT        4
#define SFERALABS_DO_OC_COUNT           2
#define SFERALABS_DI_COUNT              4
#define SFERALABS_OC_PIN_OFFSET         100   /* pin_id > 100 treated as OC */
/* SFERALABS_SERIAL_RS485_FMT intentionally not defined — Iono Pi is RS-232 only */
#define SFERALABS_UART0_DEV             "/dev/ttyAMA0"

/* -------------------------------------------------------------------------
 * Iono Pi Max
 * Digital outputs: relay o1-o4, open collectors oc1-oc4
 * Digital inputs:  di1-di4
 * Configurable I/O: dt1-dt4
 * Analog inputs:   av1-av4 (voltage mV/100), ai1-ai4 (current µA), at1-at2 (temp °C/100)
 * Analog outputs:  ao1-ao2 (voltage or current)
 * pin_id 1-4 = relay, pin_id 101-104 = open collector
 * ------------------------------------------------------------------------- */
#elif defined(EHS_SFERALABS_BOARD_IONOPIMAX)

#define SFERALABS_CLASS_PATH            "/sys/class/ionopimax"
#define SFERALABS_DO_RELAY_FMT          SFERALABS_CLASS_PATH "/digital_out/o%d"
#define SFERALABS_DO_OC_FMT             SFERALABS_CLASS_PATH "/digital_out/oc%d"
#define SFERALABS_DI_FMT                SFERALABS_CLASS_PATH "/digital_in/di%d"
#define SFERALABS_DI_DEB_FMT            SFERALABS_CLASS_PATH "/digital_in/di%d_deb"
#define SFERALABS_DIO_FMT               SFERALABS_CLASS_PATH "/digital_io/dt%d"
#define SFERALABS_DIO_MODE_FMT          SFERALABS_CLASS_PATH "/digital_io/dt%d_mode"
#define SFERALABS_AI_VOLT_FMT           SFERALABS_CLASS_PATH "/analog_in/av%d"
#define SFERALABS_AI_VOLT_MODE_FMT      SFERALABS_CLASS_PATH "/analog_in/av%d_mode"
#define SFERALABS_AI_CURR_FMT           SFERALABS_CLASS_PATH "/analog_in/ai%d"
#define SFERALABS_AI_CURR_MODE_FMT      SFERALABS_CLASS_PATH "/analog_in/ai%d_mode"
#define SFERALABS_AI_TEMP_FMT           SFERALABS_CLASS_PATH "/analog_in/at%d"
#define SFERALABS_AI_TEMP_MODE_FMT      SFERALABS_CLASS_PATH "/analog_in/at%d_mode"
#define SFERALABS_AO_FMT                SFERALABS_CLASS_PATH "/analog_out/ao%d"
#define SFERALABS_AO_MODE_FMT           SFERALABS_CLASS_PATH "/analog_out/ao%d_mode"
#define SFERALABS_AI_VOLT_CHANNELS      4   /* av1-av4: ADC ch 0-3  */
#define SFERALABS_AI_CURR_CHANNELS      4   /* ai1-ai4: ADC ch 4-7  */
#define SFERALABS_AI_TEMP_CHANNELS      2   /* at1-at2: ADC ch 8-9  */
#define SFERALABS_AO_CHANNELS           2   /* ao1-ao2: DAC ch 0-1  */
#define SFERALABS_DO_RELAY_COUNT        4
#define SFERALABS_DO_OC_COUNT           4
#define SFERALABS_DI_COUNT              4
#define SFERALABS_OC_PIN_OFFSET         100
#define SFERALABS_SERIAL_RS485_FMT      SFERALABS_CLASS_PATH "/serial/rs232_rs485_inv"
#define SFERALABS_UART0_DEV             "/dev/ttyAMA0"

/* -------------------------------------------------------------------------
 * Strato Pi (Base / UPS / CAN / CM)
 * Digital output: single relay (status), LED (pin_id 100)
 * Digital input:  button (pin_id 1)
 * No analog I/O.
 * No OC outputs.
 * ------------------------------------------------------------------------- */
#elif defined(EHS_SFERALABS_BOARD_STRATOPI)

#define SFERALABS_CLASS_PATH            "/sys/class/stratopi"
/* Single relay — no channel number in path */
#define SFERALABS_DO_RELAY_FMT          SFERALABS_CLASS_PATH "/relay/status"
/* LED exposed as secondary output on pin_id 100 */
#define SFERALABS_DO_LED_PATH           SFERALABS_CLASS_PATH "/led/status"
/* Single button input — no channel number */
#define SFERALABS_DI_FMT                SFERALABS_CLASS_PATH "/button/status"
#define SFERALABS_AI_VOLT_CHANNELS      0
#define SFERALABS_AI_CURR_CHANNELS      0
#define SFERALABS_AI_TEMP_CHANNELS      0
#define SFERALABS_AO_CHANNELS           0
#define SFERALABS_DO_RELAY_COUNT        1
#define SFERALABS_DO_OC_COUNT           0
#define SFERALABS_DI_COUNT              1
#define SFERALABS_OC_PIN_OFFSET         100
#define SFERALABS_SERIAL_RS485_FMT      SFERALABS_CLASS_PATH "/rs485/mode"
#define SFERALABS_UART0_DEV             "/dev/ttyAMA0"

/* -------------------------------------------------------------------------
 * Strato Pi Max (CM4/CM5 server with expansion slots)
 * Digital outputs: do1-do7 per slot (up to 4 slots)
 * Digital inputs:  di1-di7 per slot
 * pin_id encoding: (slot << 8) | channel  (slot 1-4, channel 1-7)
 * Analog I/O: av/ai/at per slot expansion board.
 * ------------------------------------------------------------------------- */
#elif defined(EHS_SFERALABS_BOARD_STRATOPIMAX)

#define SFERALABS_CLASS_PATH            "/sys/class/stratopimax"
/* slot = (pin_id >> 8) & 0xF, ch = pin_id & 0xFF */
#define SFERALABS_DO_RELAY_FMT          SFERALABS_CLASS_PATH "/digital_out_s%d/do%d"
#define SFERALABS_DI_FMT                SFERALABS_CLASS_PATH "/digital_in_s%d/di%d"
#define SFERALABS_AI_VOLT_FMT           SFERALABS_CLASS_PATH "/analog_in_s%d/av%d"
#define SFERALABS_AI_CURR_FMT           SFERALABS_CLASS_PATH "/analog_in_s%d/ai%d"
#define SFERALABS_AI_TEMP_FMT           SFERALABS_CLASS_PATH "/analog_in_s%d/at%d"
#define SFERALABS_AO_FMT                SFERALABS_CLASS_PATH "/analog_out_s%d/ao%d"
#define SFERALABS_AI_VOLT_CHANNELS      4
#define SFERALABS_AI_CURR_CHANNELS      4
#define SFERALABS_AI_TEMP_CHANNELS      2
#define SFERALABS_AO_CHANNELS           2
#define SFERALABS_DO_RELAY_COUNT        7
#define SFERALABS_DO_OC_COUNT           0
#define SFERALABS_DI_COUNT              7
#define SFERALABS_OC_PIN_OFFSET         0   /* No OC on Strato Pi Max */
#define SFERALABS_SERIAL_RS485_FMT      SFERALABS_CLASS_PATH "/rs485_s%d/local_echo"
#define SFERALABS_UART0_DEV             "/dev/ttyAMA1"
/* For Strato Pi Max, slot is extracted from pin_id upper byte */
#define SFERALABS_SLOT_FROM_PINID(p)    (((p) >> 8) & 0x0F)
#define SFERALABS_CH_FROM_PINID(p)      ((p) & 0xFF)

/* -------------------------------------------------------------------------
 * Exo Sense Pi (multi-sensor module)
 * Digital inputs:  di1-di8
 * Digital output:  single relay do1 (pin_id 1)
 * No analog outputs.
 * ------------------------------------------------------------------------- */
#elif defined(EHS_SFERALABS_BOARD_EXOSENSEPI)

#define SFERALABS_CLASS_PATH            "/sys/class/exosensepi"
#define SFERALABS_DO_RELAY_FMT          SFERALABS_CLASS_PATH "/digital_out/do1"
#define SFERALABS_DI_FMT                SFERALABS_CLASS_PATH "/digital_in/di%d"
#define SFERALABS_DI_DEB_FMT            SFERALABS_CLASS_PATH "/digital_in/di%d_deb"
#define SFERALABS_AI_VOLT_CHANNELS      0
#define SFERALABS_AI_CURR_CHANNELS      0
#define SFERALABS_AI_TEMP_CHANNELS      0
#define SFERALABS_AO_CHANNELS           0
#define SFERALABS_DO_RELAY_COUNT        1
#define SFERALABS_DO_OC_COUNT           0
#define SFERALABS_DI_COUNT              8
#define SFERALABS_OC_PIN_OFFSET         0
#define SFERALABS_SERIAL_RS485_FMT      NULL
#define SFERALABS_UART0_DEV             "/dev/ttyAMA0"

#endif /* board selection */

/* Convenience: read one char ('0' or '1') from a sysfs attribute */
static inline int sferalabs_sysfs_read_char(const char *path, char *out)
{
    int fd = open(path, O_RDONLY);
    if (fd < 0)
    {
        printf("SferaLabs: open('%s') failed: %s\n", path, strerror(errno));
        return -1;
    }
    int n = (int)read(fd, out, 1);
    close(fd);
    if (n != 1)
    {
        printf("SferaLabs: read('%s') failed: %s\n", path, strerror(errno));
        return -1;
    }
    return 0;
}

/* Convenience: write a short string to a sysfs attribute */
static inline int sferalabs_sysfs_write(const char *path, const char *val, int len)
{
    int fd = open(path, O_WRONLY);
    if (fd < 0)
    {
        printf("SferaLabs: open('%s') failed: %s\n", path, strerror(errno));
        return -1;
    }
    int n = (int)write(fd, val, (size_t)len);
    close(fd);
    if (n != len)
    {
        printf("SferaLabs: write('%s') failed: %s\n", path, strerror(errno));
        return -1;
    }
    return 0;
}

/* Convenience: read a signed integer from a sysfs attribute */
static inline int sferalabs_sysfs_read_int(const char *path, long *out)
{
    char buf[32];
    int fd = open(path, O_RDONLY);
    if (fd < 0)
    {
        printf("SferaLabs: open('%s') failed: %s\n", path, strerror(errno));
        return -1;
    }
    int n = (int)read(fd, buf, sizeof(buf) - 1);
    close(fd);
    if (n <= 0)
    {
        printf("SferaLabs: read('%s') failed: %s\n", path, strerror(errno));
        return -1;
    }
    buf[n] = '\0';
    *out = strtol(buf, NULL, 10);
    return 0;
}

/* Convenience: write a signed integer to a sysfs attribute */
static inline int sferalabs_sysfs_write_int(const char *path, long val)
{
    char buf[32];
    int len = snprintf(buf, sizeof(buf), "%ld", val);
    return sferalabs_sysfs_write(path, buf, len);
}

#endif /* SFERALABS_HAL_H */
