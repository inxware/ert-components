/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/**
 * @file sferalabs_poll.c
 * @brief Sfera Labs sysfs change-detection polling thread implementation.
 *
 * A single POSIX thread wakes every SFERALABS_POLL_INTERVAL_MS milliseconds,
 * reads all registered sysfs paths, and fires the registered callback for any
 * path whose string value has changed since the last read.
 *
 * Why periodic read-and-compare rather than POSIX poll()/select():
 *
 *   poll()/select() with POLLPRI|POLLERR only works for sysfs nodes that
 *   explicitly implement the poll file operation in their kernel driver — in
 *   practice this is limited to GPIO edge-trigger nodes
 *   (/sys/class/gpio/gpioN/value), which are themselves deprecated in favour
 *   of libgpiod.  General sysfs show/store attributes (such as the Strato Pi
 *   ups/power_source and ups/battery_v nodes) do not implement poll; calling
 *   poll() on them either returns immediately with no events (busy-loop) or
 *   blocks indefinitely.  inotify likewise does not work on sysfs.  Periodic
 *   read-and-compare is therefore the correct and standard approach for this
 *   class of sysfs attribute.
 *
 * Thread safety:
 *   - Registrations are protected by s_mutex.
 *   - The callback is invoked with s_mutex released to prevent deadlock.
 *   - EhsCallbackQueue_execute() is safe to call from any thread.
 *   - s_running is volatile; the thread checks it each iteration.
 */

#include "sferalabs_poll.h"
#include "hal_logger.h"

#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>


/* ------------------------------------------------------------------ */
/*  Internal state                                                      */
/* ------------------------------------------------------------------ */

typedef struct
{
    char                path[256];
    sferalabs_poll_cb_t cb;
    void               *ctx;
    char                last_val[SFERALABS_POLL_VAL_LEN];
    int                 initialized;    /* 0 until first successful read */
} sferalabs_poll_entry_t;

static sferalabs_poll_entry_t  s_entries[SFERALABS_POLL_MAX_ENTRIES];
static int                     s_num_entries = 0;
static pthread_t               s_thread;
static volatile int            s_running     = 0;
static pthread_mutex_t         s_mutex       = PTHREAD_MUTEX_INITIALIZER;


/* ------------------------------------------------------------------ */
/*  Internal helpers                                                    */
/* ------------------------------------------------------------------ */

/**
 * Read a sysfs attribute into buf (max buflen-1 chars + NUL).
 * Strips trailing newline/whitespace.
 * Returns 0 on success, -1 on I/O error.
 */
static int poll_read_value(const char *path, char *buf, int buflen)
{
    int fd = open(path, O_RDONLY);
    if (fd < 0)
        return -1;

    int n = (int)read(fd, buf, (size_t)(buflen - 1));
    close(fd);

    if (n <= 0)
        return -1;

    buf[n] = '\0';

    /* Strip trailing whitespace / newline */
    while (n > 0 &&
           (buf[n - 1] == '\n' || buf[n - 1] == '\r' || buf[n - 1] == ' '))
        buf[--n] = '\0';

    return 0;
}


/* ------------------------------------------------------------------ */
/*  Poll thread                                                         */
/* ------------------------------------------------------------------ */

static void *poll_thread_func(void *arg)
{
    (void)arg;

    struct timespec sleep_ts;
    sleep_ts.tv_sec  =  SFERALABS_POLL_INTERVAL_MS / 1000;
    sleep_ts.tv_nsec = (SFERALABS_POLL_INTERVAL_MS % 1000) * 1000000L;

    while (s_running)
    {
        pthread_mutex_lock(&s_mutex);

        for (int i = 0; i < s_num_entries; i++)
        {
            sferalabs_poll_entry_t *e = &s_entries[i];
            char cur[SFERALABS_POLL_VAL_LEN];

            if (poll_read_value(e->path, cur, (int)sizeof(cur)) != 0)
                continue;

            if (!e->initialized)
            {
                /* Establish baseline on first read — no callback */
                strncpy(e->last_val, cur, sizeof(e->last_val) - 1);
                e->last_val[sizeof(e->last_val) - 1] = '\0';
                e->initialized = 1;
                continue;
            }

            if (strcmp(cur, e->last_val) != 0)
            {
                /* Snapshot old value, update stored value */
                char old_val[SFERALABS_POLL_VAL_LEN];
                strncpy(old_val, e->last_val, sizeof(old_val) - 1);
                old_val[sizeof(old_val) - 1] = '\0';

                strncpy(e->last_val, cur, sizeof(e->last_val) - 1);
                e->last_val[sizeof(e->last_val) - 1] = '\0';

                /* Release the mutex before the callback to avoid deadlock
                 * if the callback itself calls sferalabs_poll_register(). */
                sferalabs_poll_cb_t cb  = e->cb;
                void               *ctx = e->ctx;

                pthread_mutex_unlock(&s_mutex);
                cb(ctx, old_val, cur);
                pthread_mutex_lock(&s_mutex);
            }
        }

        pthread_mutex_unlock(&s_mutex);
        nanosleep(&sleep_ts, NULL);
    }

    return NULL;
}


/* ------------------------------------------------------------------ */
/*  Public API                                                          */
/* ------------------------------------------------------------------ */

int sferalabs_poll_register(const char          *path,
                             sferalabs_poll_cb_t  cb,
                             void                *ctx)
{
    pthread_mutex_lock(&s_mutex);

    if (s_num_entries >= SFERALABS_POLL_MAX_ENTRIES)
    {
        pthread_mutex_unlock(&s_mutex);
        EHSH_LOG_ERROR("sferalabs_poll: registration table full (max %d)",
                       SFERALABS_POLL_MAX_ENTRIES);
        return -1;
    }

    /* Guard against duplicate registrations */
    for (int i = 0; i < s_num_entries; i++)
    {
        if (strcmp(s_entries[i].path, path) == 0)
        {
            pthread_mutex_unlock(&s_mutex);
            EHSH_LOG_WARNING("sferalabs_poll: path already registered: %s", path);
            return -1;
        }
    }

    sferalabs_poll_entry_t *e = &s_entries[s_num_entries++];
    strncpy(e->path, path, sizeof(e->path) - 1);
    e->path[sizeof(e->path) - 1] = '\0';
    e->cb          = cb;
    e->ctx         = ctx;
    e->last_val[0] = '\0';
    e->initialized = 0;

    pthread_mutex_unlock(&s_mutex);

    EHSH_LOG_INFO("sferalabs_poll: registered %s", path);
    return 0;
}

int sferalabs_poll_start(void)
{
    if (s_running)
        return 0;   /* Already started — idempotent */

    s_running = 1;

    int rc = pthread_create(&s_thread, NULL, poll_thread_func, NULL);
    if (rc != 0)
    {
        s_running = 0;
        EHSH_LOG_ERROR("sferalabs_poll: pthread_create failed (rc=%d)", rc);
        return -1;
    }

    EHSH_LOG_INFO("sferalabs_poll: poll thread started (%d ms interval)",
                  SFERALABS_POLL_INTERVAL_MS);
    return 0;
}

void sferalabs_poll_stop(void)
{
    if (!s_running)
        return;

    s_running = 0;
    pthread_join(s_thread, NULL);
    EHSH_LOG_INFO("sferalabs_poll: poll thread stopped");
}
