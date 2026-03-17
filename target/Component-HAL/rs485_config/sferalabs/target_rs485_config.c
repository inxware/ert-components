/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/**
 * @file target_rs485_config.c
 * @brief Sfera Labs sysfs RS-485 configuration HAL implementation.
 *
 * Supported boards: Iono Pi Max, Strato Pi, Strato Pi Max.
 * Exo Sense Pi and Iono Pi have no RS-485 configuration via sysfs and return -1.
 *
 * Sysfs paths:
 *
 * Iono Pi Max:
 *   Local echo:   /sys/class/ionopimax/serial/rs232_rs485_inv (write "0" for RS-485, "1" for RS-232)
 *   Termination:  /sys/class/ionopimax/rs485/termination (write "on"/"off")
 *
 * Strato Pi:
 *   Local echo:   /sys/class/stratopi/rs485/mode (write "on"/"off" for local echo enable)
 *   Termination:  /sys/class/stratopi/rs485/termination (write "on"/"off")
 *
 * Strato Pi Max (per port — state->port_num selects the RS-485 slot):
 *   Local echo:   /sys/class/stratopimax/rs485_s<N>/local_echo (write "on"/"off")
 *   Termination:  /sys/class/stratopimax/rs485_s<N>/termination (write "on"/"off")
 */

#include "globals.h"
#include "hal_rs485_config.h"
#include "sferalabs_hal.h"
#include "hal_logger.h"
#include <string.h>

/* -------------------------------------------------------------------------
 * Helper: write "on"/"off" string to a sysfs path
 * ------------------------------------------------------------------------- */
static int write_on_off(const char *path, ehs_bool value)
{
    if (value)
        return sferalabs_sysfs_write(path, "on", 2);
    else
        return sferalabs_sysfs_write(path, "off", 3);
}

/* -------------------------------------------------------------------------
 * Helper: read "on"/"off" from a sysfs path into an ehs_bool
 * ------------------------------------------------------------------------- */
static int read_on_off(const char *path, ehs_bool *out)
{
    char buf[8];
    int fd = open(path, O_RDONLY);
    if (fd < 0) return -1;
    int n = (int)read(fd, buf, sizeof(buf) - 1);
    close(fd);
    if (n <= 0) return -1;
    buf[n] = '\0';
    *out = (strncmp(buf, "on", 2) == 0) ? EHS_TRUE : EHS_FALSE;
    return 0;
}

/* -------------------------------------------------------------------------
 * Board-specific implementations
 * ------------------------------------------------------------------------- */

#if defined(EHS_SFERALABS_BOARD_IONOPIMAX)

EHS_GLOBAL int EhsTRs485Config(ehs_rs485_config_state_type *state)
{
    if (write_on_off(SFERALABS_SERIAL_RS485_FMT, state->echo) != 0)
    {
        EHSH_LOG_ERROR("RS-485 config: failed to write echo to %s", SFERALABS_SERIAL_RS485_FMT);
        return -1;
    }
    const char *term_path = SFERALABS_CLASS_PATH "/rs485/termination";
    if (write_on_off(term_path, state->termination) != 0)
    {
        EHSH_LOG_ERROR("RS-485 config: failed to write termination to %s", term_path);
        return -1;
    }
    return 0;
}

EHS_GLOBAL int EhsTRs485Read(ehs_rs485_config_state_type *state)
{
    if (read_on_off(SFERALABS_SERIAL_RS485_FMT, &state->echo) != 0)
    {
        EHSH_LOG_ERROR("RS-485 config: failed to read echo from %s", SFERALABS_SERIAL_RS485_FMT);
        return -1;
    }
    const char *term_path = SFERALABS_CLASS_PATH "/rs485/termination";
    if (read_on_off(term_path, &state->termination) != 0)
    {
        EHSH_LOG_ERROR("RS-485 config: failed to read termination from %s", term_path);
        return -1;
    }
    return 0;
}

#elif defined(EHS_SFERALABS_BOARD_STRATOPI)

EHS_GLOBAL int EhsTRs485Config(ehs_rs485_config_state_type *state)
{
    if (write_on_off(SFERALABS_SERIAL_RS485_FMT, state->echo) != 0)
    {
        EHSH_LOG_ERROR("RS-485 config: failed to write echo/mode to %s", SFERALABS_SERIAL_RS485_FMT);
        return -1;
    }
    const char *term_path = SFERALABS_CLASS_PATH "/rs485/termination";
    if (write_on_off(term_path, state->termination) != 0)
    {
        EHSH_LOG_ERROR("RS-485 config: failed to write termination to %s", term_path);
        return -1;
    }
    return 0;
}

EHS_GLOBAL int EhsTRs485Read(ehs_rs485_config_state_type *state)
{
    if (read_on_off(SFERALABS_SERIAL_RS485_FMT, &state->echo) != 0)
    {
        EHSH_LOG_ERROR("RS-485 config: failed to read echo/mode from %s", SFERALABS_SERIAL_RS485_FMT);
        return -1;
    }
    const char *term_path = SFERALABS_CLASS_PATH "/rs485/termination";
    if (read_on_off(term_path, &state->termination) != 0)
    {
        EHSH_LOG_ERROR("RS-485 config: failed to read termination from %s", term_path);
        return -1;
    }
    return 0;
}

#elif defined(EHS_SFERALABS_BOARD_STRATOPIMAX)

EHS_GLOBAL int EhsTRs485Config(ehs_rs485_config_state_type *state)
{
    char echo_path[SFERALABS_SYSFS_BUF_SIZE];
    char term_path[SFERALABS_SYSFS_BUF_SIZE];
    snprintf(echo_path, sizeof(echo_path),
             SFERALABS_CLASS_PATH "/rs485_s%d/local_echo", (int)state->port_num);
    snprintf(term_path, sizeof(term_path),
             SFERALABS_CLASS_PATH "/rs485_s%d/termination", (int)state->port_num);

    if (write_on_off(echo_path, state->echo) != 0)
    {
        EHSH_LOG_ERROR("RS-485 config: failed to write echo to %s", echo_path);
        return -1;
    }
    if (write_on_off(term_path, state->termination) != 0)
    {
        EHSH_LOG_ERROR("RS-485 config: failed to write termination to %s", term_path);
        return -1;
    }
    return 0;
}

EHS_GLOBAL int EhsTRs485Read(ehs_rs485_config_state_type *state)
{
    char echo_path[SFERALABS_SYSFS_BUF_SIZE];
    char term_path[SFERALABS_SYSFS_BUF_SIZE];
    snprintf(echo_path, sizeof(echo_path),
             SFERALABS_CLASS_PATH "/rs485_s%d/local_echo", (int)state->port_num);
    snprintf(term_path, sizeof(term_path),
             SFERALABS_CLASS_PATH "/rs485_s%d/termination", (int)state->port_num);

    if (read_on_off(echo_path, &state->echo) != 0)
    {
        EHSH_LOG_ERROR("RS-485 config: failed to read echo from %s", echo_path);
        return -1;
    }
    if (read_on_off(term_path, &state->termination) != 0)
    {
        EHSH_LOG_ERROR("RS-485 config: failed to read termination from %s", term_path);
        return -1;
    }
    return 0;
}

#else /* board does not support RS-485 config via sysfs */

EHS_GLOBAL int EhsTRs485Config(ehs_rs485_config_state_type *state)
{
    (void)state;
    EHSH_LOG_ERROR("RS-485 config: not supported on this Sfera Labs board");
    return -1;
}

EHS_GLOBAL int EhsTRs485Read(ehs_rs485_config_state_type *state)
{
    if (state)
    {
        state->echo        = EHS_FALSE;
        state->termination = EHS_FALSE;
    }
    EHSH_LOG_ERROR("RS-485 config: not supported on this Sfera Labs board");
    return -1;
}

#endif /* board selection */
