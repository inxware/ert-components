/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file serial_console.h
 *
 * Cross-target interactive serial-console API.
 *
 * The implementation in Common/Ehs/serial_console.c is platform-
 * independent — every reference to a UART driver, a FreeRTOS sleep,
 * or stdio buffering quirks is delegated to the per-target backend
 * declared in Common/HAL/include/hal_serial_console.h.
 *
 * The split mirrors the banner pattern (Common/Ehs/banner.{c,h}):
 *   serial_console.h   — the *content and behaviour* of the human
 *                        command prompt, callable from the target's
 *                        main task.
 *   hal_serial_console.h — the *raw serial-UART abstraction* the
 *                        common code calls down into (sleep, flush,
 *                        write).
 *
 * @author: inx limited
 */

#ifndef EHS_SERIAL_CONSOLE_H
#define EHS_SERIAL_CONSOLE_H

#include "globals.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************/
/* Buffer sizing  */

/* Maximum length of one user-typed line. If you change this, also bump
 * EHS_WIFI_SSID_BUFF_MAX so the SSID prompt buffer matches. */
#ifndef EHS_PROMPT_READ_MAX
#define EHS_PROMPT_READ_MAX 64
#endif

/*****************************************************************************/
/* Echo modes for the line-reader  */

typedef enum {
    ECHO_NORMAL,    /* echo characters as typed                    */
    ECHO_PASSWORD,  /* echo an asterisk for each character         */
    ECHO_HIDDEN     /* no echo at all                              */
} command_prompt_echo_mode_t;

/*****************************************************************************/
/* Output helpers  */

/** Print a line followed by '\n', flush, and pause briefly so the
 *  user can read it before any subsequent prompt. */
void command_prompt_println(const char* text);

/** Print without trailing newline; flushes so a subsequent blocking
 *  read sees the prompt before stdin is touched. */
void command_prompt_print(const char* text);

/** Show the "> " prompt and flush. */
void command_prompt_show_prompt(void);

/** Echo a single received command character then '\n'. Skips
 *  unprintable / control characters silently. */
void command_prompt_echo_command(char cmd);

/*****************************************************************************/
/* Input helpers  */

/** Read one printable character with the chosen echo behaviour.
 *  Skips leading newlines / carriage returns. */
ehs_bool command_prompt_read_char(char* ch, command_prompt_echo_mode_t echo_mode);

/** Read a line up to EHS_PROMPT_READ_MAX-1 chars terminated by Enter.
 *  Supports backspace, Ctrl+C (cancel), Ctrl+U (clear line), Esc
 *  (cancel). Returns the line length (zero on cancel). */
ehs_uint32 command_prompt_read_with_echo(char* buffer,
                                         command_prompt_echo_mode_t echo_mode);

/** Convenience wrapper: read a line with normal echo. */
ehs_uint32 command_prompt_read(char* buffer);

/** Convenience wrapper: read a line with masked echo (asterisks). */
ehs_uint32 command_prompt_read_password(char* buffer);

/** Print a y/n question and block until the user answers. Re-prompts
 *  on invalid input; never returns until y/Y or n/N is given. */
ehs_bool command_prompt_ask_yes_no(const char* question);

/*****************************************************************************/
/* Command handlers — invoked from command_prompt_task on each key  */

void command_prompt_help(void);
void command_prompt_reboot(void);
void command_prompt_ip_addr(void);

/** Delete the currently-selected app after y/n confirmation. Refuses to
 *  delete the default app. If a previous version of the deleted app
 *  exists it is reinstated; otherwise metadata is rolled to the default
 *  app for the next boot. */
void command_prompt_delete_current_app(void);

#ifdef EHS_NETWORK_WIFI_SUPPORT
void command_prompt_wifi_conf(void);
void command_prompt_wifi_reconnect(void);
void command_prompt_wifi_ssid(void);
void command_prompt_wifi_disconnect(void);
void command_prompt_wifi_forget(void);
void command_prompt_list_ssid_bssid(void);
void command_prompt_wifi_stop_scan(void);
#endif

#ifdef EHS_LORAWAN_SUPPORT
void command_prompt_lorawan_info(void);
#endif

/*****************************************************************************/
/* Main task  */

/** Top-level interactive serial-console task. Spawned once at boot by
 *  the target main with xTaskCreate / pthread_create / equivalent. */
void command_prompt_task(void* params);

#ifdef __cplusplus
}
#endif

#endif /* EHS_SERIAL_CONSOLE_H */
