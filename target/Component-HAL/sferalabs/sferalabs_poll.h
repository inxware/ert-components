/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/**
 * @file sferalabs_poll.h
 * @brief Sfera Labs sysfs change-detection polling thread.
 *
 * Provides a single shared background thread that periodically reads
 * registered sysfs nodes and fires a callback whenever a node's string
 * value changes.  Designed for Strato Pi / Ionopi boards where sysfs
 * attributes are plain text files that do not support inotify or
 * interrupt-driven POLLPRI.
 *
 * Usage pattern (from any sferalabs HAL module):
 *
 *   1.  sferalabs_poll_register(path, my_cb, ctx);  // at HAL init
 *   2.  sferalabs_poll_start();                      // idempotent
 *
 * The callback is invoked from the poll thread; it must be async-safe.
 * Use EhsCallbackQueue_execute() to schedule eRT function-block work.
 */

#ifndef _SFERALABS_POLL_H_
#define _SFERALABS_POLL_H_

#ifdef __cplusplus
extern "C" {
#endif

/** Maximum number of simultaneously registered sysfs paths. */
#define SFERALABS_POLL_MAX_ENTRIES   16

/** Maximum length of a polled sysfs value string (including NUL). */
#define SFERALABS_POLL_VAL_LEN       32

/** Polling interval in milliseconds. */
#define SFERALABS_POLL_INTERVAL_MS   250

/**
 * Callback fired when a registered sysfs node's value changes.
 *
 * Called from the poll thread — must not block for long.
 * To schedule eRT function-block work, call EhsCallbackQueue_execute().
 *
 * @param ctx      Opaque context pointer supplied at registration.
 * @param old_val  Previous string value (NUL-terminated, newline stripped).
 * @param new_val  New string value (NUL-terminated, newline stripped).
 */
typedef void (*sferalabs_poll_cb_t)(void *ctx,
                                    const char *old_val,
                                    const char *new_val);

/**
 * Register a sysfs path for polling.
 *
 * Safe to call before sferalabs_poll_start().  The first successful
 * read after start establishes the baseline value; no callback fires
 * for the initial read.
 *
 * @param path   Absolute sysfs path to poll.
 * @param cb     Callback to fire on value change.
 * @param ctx    Opaque context pointer passed verbatim to cb.
 * @return 0 on success, -1 if the table is full or path is duplicate.
 */
int sferalabs_poll_register(const char *path,
                             sferalabs_poll_cb_t cb,
                             void *ctx);

/**
 * Start the background polling thread.
 *
 * Idempotent — may be called by multiple HAL modules; only one thread
 * is ever started.
 *
 * @return 0 on success, -1 on pthread_create failure.
 */
int sferalabs_poll_start(void);

/**
 * Stop the polling thread and block until it exits.
 *
 * For production use the thread normally runs for the process lifetime.
 * Call this during clean shutdown if required.
 */
void sferalabs_poll_stop(void);

#ifdef __cplusplus
}
#endif

#endif /* _SFERALABS_POLL_H_ */
