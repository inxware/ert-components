/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/**
 * @file sferalabs_fd_poll.h
 * @brief Sfera Labs fast sysfs change-detection using POSIX poll().
 *
 * For sysfs nodes whose kernel driver implements the poll file operation
 * (raising POLLPRI|POLLERR on value change), this provides interrupt-driven
 * notification via a single background thread that blocks in poll() until an
 * event arrives.  Latency is bounded by the kernel interrupt path rather than
 * a fixed timer tick, making it suitable for GPIO inputs and other fast
 * signals.
 *
 * Applicable sysfs nodes:
 *   - GPIO edge-trigger nodes: /sys/class/gpio/gpioN/value (legacy sysfs GPIO,
 *     requires "both"/"rising"/"falling" written to the 'edge' attribute first)
 *   - Any Sfera Labs kernel driver attribute that implements .poll in its
 *     kobj_attribute or device_attribute — check the driver source or test
 *     with: poll -t 5000 /sys/class/stratopi/di/di1 (exits immediately if
 *     unsupported, blocks until timeout if supported)
 *
 * For sysfs nodes that do NOT support poll() (most plain show/store
 * attributes including UPS power_source, ADC readings, etc.) use the
 * periodic read-and-compare thread in sferalabs_poll.h instead.
 *
 * Current status — GPIO inputs and ADC:
 *   GPIO inputs and ADC channels are currently read synchronously by their
 *   respective eRT function blocks (the application calls the HAL directly
 *   on demand).  Neither is wired to sferalabs_fd_poll yet.  If testing
 *   confirms that the Sfera Labs DI sysfs nodes raise POLLPRI on edge
 *   transitions, they could be registered here to get async InternalPort
 *   callbacks without application-side polling.
 *
 * Usage:
 *   1.  sferalabs_fd_poll_register(path, my_cb, ctx);  // at HAL init
 *   2.  sferalabs_fd_poll_start();                      // idempotent
 *
 * The callback fires from the poll thread; call EhsCallbackQueue_execute()
 * to schedule eRT function-block work.
 */

#ifndef _SFERALABS_FD_POLL_H_
#define _SFERALABS_FD_POLL_H_

#ifdef __cplusplus
extern "C" {
#endif

/** Maximum number of simultaneously registered file descriptors. */
#define SFERALABS_FD_POLL_MAX_ENTRIES   16

/** Maximum length of a polled sysfs value string (including NUL). */
#define SFERALABS_FD_POLL_VAL_LEN       32

/**
 * Callback fired when a registered sysfs node raises POLLPRI|POLLERR.
 *
 * Called from the poll thread — must not block for long.
 * To schedule eRT function-block work, call EhsCallbackQueue_execute().
 *
 * @param ctx      Opaque context pointer supplied at registration.
 * @param old_val  Value read at registration time or last event (NUL-terminated).
 * @param new_val  Value read after the poll event (NUL-terminated).
 */
typedef void (*sferalabs_fd_poll_cb_t)(void       *ctx,
                                       const char *old_val,
                                       const char *new_val);

/**
 * Register a sysfs path for fd-based poll() monitoring.
 *
 * Opens the file, performs an initial read to establish the baseline value,
 * and adds the fd to the poll set.  Safe to call before
 * sferalabs_fd_poll_start().
 *
 * @param path   Absolute sysfs path — must support POLLPRI|POLLERR.
 * @param cb     Callback to fire on POLLPRI event.
 * @param ctx    Opaque context pointer passed verbatim to cb.
 * @return 0 on success, -1 if the path cannot be opened, table is full,
 *         or the path is already registered.
 */
int sferalabs_fd_poll_register(const char           *path,
                                sferalabs_fd_poll_cb_t cb,
                                void                 *ctx);

/**
 * Start the background poll() thread.
 *
 * Idempotent — may be called by multiple HAL modules; only one thread
 * is ever started.
 * 
 * ** Note this is not curretly used because it is uncertain that sferalabs syfs nodes support poll events. **
 *
 * @return 0 on success, -1 on failure.
 */
int sferalabs_fd_poll_start(void);

/**
 * Stop the poll() thread.  Blocks until the thread exits and closes
 * all registered file descriptors.
 */
void sferalabs_fd_poll_stop(void);

#ifdef __cplusplus
}
#endif

#endif /* _SFERALABS_FD_POLL_H_ */
