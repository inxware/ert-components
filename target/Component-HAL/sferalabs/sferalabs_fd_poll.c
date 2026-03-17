/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/**
 * @file sferalabs_fd_poll.c
 * @brief Sfera Labs fast sysfs change-detection using POSIX poll().
 *
 * A single POSIX thread blocks in poll() on all registered file descriptors.
 * When a POLLPRI or POLLERR event fires (indicating a sysfs value change), the
 * thread seeks the fd back to the start, reads the new value, and fires the
 * registered callback.
 *
 * A self-pipe is included in every poll() call so that sferalabs_fd_poll_stop()
 * can unblock the thread immediately without waiting for the next sysfs event.
 *
 * POSIX poll() sysfs requirements:
 *   The sysfs node's kernel driver must implement the .poll file operation.
 *   After open(), an initial read() is required to arm the interrupt mechanism
 *   before the first poll() call, and after each POLLPRI event the fd must be
 *   lseek()'d back to 0 and re-read to re-arm for the next event.  Nodes that
 *   do not implement .poll will never raise POLLPRI and should use the periodic
 *   read-and-compare thread in sferalabs_poll.c instead.
 *
 * Thread safety:
 *   - The entry table and fd list are protected by s_mutex.
 *   - Callbacks are invoked with s_mutex released to prevent deadlock.
 *   - EhsCallbackQueue_execute() is safe to call from any thread.
 */

#include "sferalabs_fd_poll.h"
#include "hal_logger.h"

#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <pthread.h>


/* ------------------------------------------------------------------ */
/*  Internal state                                                      */
/* ------------------------------------------------------------------ */

typedef struct
{
    char                   path[256];
    int                    fd;
    sferalabs_fd_poll_cb_t cb;
    void                  *ctx;
    char                   last_val[SFERALABS_FD_POLL_VAL_LEN];
} sferalabs_fd_poll_entry_t;

static sferalabs_fd_poll_entry_t  s_entries[SFERALABS_FD_POLL_MAX_ENTRIES];
static int                        s_num_entries  = 0;
static pthread_t                  s_thread;
static volatile int               s_running      = 0;
static int                        s_stop_pipe[2] = {-1, -1};
static pthread_mutex_t            s_mutex        = PTHREAD_MUTEX_INITIALIZER;


/* ------------------------------------------------------------------ */
/*  Internal helpers                                                    */
/* ------------------------------------------------------------------ */

/**
 * Read a sysfs attribute from an already-open fd.
 * Seeks to start first (re-arms the sysfs poll mechanism).
 * Strips trailing whitespace/newline.
 * Returns 0 on success, -1 on error.
 */
static int fd_read_value(int fd, char *buf, int buflen)
{
    if (lseek(fd, 0, SEEK_SET) < 0)
        return -1;

    int n = (int)read(fd, buf, (size_t)(buflen - 1));
    if (n <= 0)
        return -1;

    buf[n] = '\0';

    while (n > 0 &&
           (buf[n - 1] == '\n' || buf[n - 1] == '\r' || buf[n - 1] == ' '))
        buf[--n] = '\0';

    return 0;
}


/* ------------------------------------------------------------------ */
/*  Poll thread                                                         */
/* ------------------------------------------------------------------ */

static void *fd_poll_thread_func(void *arg)
{
    (void)arg;

    /* +1 for the stop-pipe read end */
    struct pollfd pfds[SFERALABS_FD_POLL_MAX_ENTRIES + 1];

    while (1)
    {
        pthread_mutex_lock(&s_mutex);
        int n = s_num_entries;

        /* pfds[0] = self-pipe for clean shutdown */
        pfds[0].fd      = s_stop_pipe[0];
        pfds[0].events  = POLLIN;
        pfds[0].revents = 0;

        for (int i = 0; i < n; i++)
        {
            pfds[i + 1].fd      = s_entries[i].fd;
            pfds[i + 1].events  = POLLPRI | POLLERR;
            pfds[i + 1].revents = 0;
        }

        pthread_mutex_unlock(&s_mutex);

        int rc = poll(pfds, (nfds_t)(n + 1), -1);   /* block indefinitely */

        if (rc < 0)
        {
            EHSH_LOG_ERROR("sferalabs_fd_poll: poll() error");
            break;
        }

        /* Stop signal via self-pipe */
        if (pfds[0].revents & POLLIN)
            break;

        /* Handle sysfs events */
        pthread_mutex_lock(&s_mutex);

        for (int i = 0; i < n; i++)
        {
            if (!(pfds[i + 1].revents & (POLLPRI | POLLERR)))
                continue;

            char cur[SFERALABS_FD_POLL_VAL_LEN];
            if (fd_read_value(s_entries[i].fd, cur, (int)sizeof(cur)) != 0)
                continue;

            if (strcmp(cur, s_entries[i].last_val) == 0)
                continue;

            char old_val[SFERALABS_FD_POLL_VAL_LEN];
            strncpy(old_val, s_entries[i].last_val, sizeof(old_val) - 1);
            old_val[sizeof(old_val) - 1] = '\0';

            strncpy(s_entries[i].last_val, cur, sizeof(s_entries[i].last_val) - 1);
            s_entries[i].last_val[sizeof(s_entries[i].last_val) - 1] = '\0';

            sferalabs_fd_poll_cb_t cb  = s_entries[i].cb;
            void                  *ctx = s_entries[i].ctx;

            /* Release mutex before callback to prevent deadlock */
            pthread_mutex_unlock(&s_mutex);
            cb(ctx, old_val, cur);
            pthread_mutex_lock(&s_mutex);
        }

        pthread_mutex_unlock(&s_mutex);
    }

    return NULL;
}


/* ------------------------------------------------------------------ */
/*  Public API                                                          */
/* ------------------------------------------------------------------ */

int sferalabs_fd_poll_register(const char            *path,
                                sferalabs_fd_poll_cb_t cb,
                                void                  *ctx)
{
    pthread_mutex_lock(&s_mutex);

    if (s_num_entries >= SFERALABS_FD_POLL_MAX_ENTRIES)
    {
        pthread_mutex_unlock(&s_mutex);
        EHSH_LOG_ERROR("sferalabs_fd_poll: registration table full (max %d)",
                       SFERALABS_FD_POLL_MAX_ENTRIES);
        return -1;
    }

    for (int i = 0; i < s_num_entries; i++)
    {
        if (strcmp(s_entries[i].path, path) == 0)
        {
            pthread_mutex_unlock(&s_mutex);
            EHSH_LOG_WARNING("sferalabs_fd_poll: path already registered: %s",
                             path);
            return -1;
        }
    }

    /* Open the fd and do the mandatory initial read to arm the poll mechanism */
    int fd = open(path, O_RDONLY | O_NONBLOCK);
    if (fd < 0)
    {
        pthread_mutex_unlock(&s_mutex);
        EHSH_LOG_ERROR("sferalabs_fd_poll: cannot open %s", path);
        return -1;
    }

    char baseline[SFERALABS_FD_POLL_VAL_LEN];
    if (fd_read_value(fd, baseline, (int)sizeof(baseline)) != 0)
    {
        close(fd);
        pthread_mutex_unlock(&s_mutex);
        EHSH_LOG_ERROR("sferalabs_fd_poll: initial read failed for %s", path);
        return -1;
    }

    sferalabs_fd_poll_entry_t *e = &s_entries[s_num_entries++];
    strncpy(e->path, path, sizeof(e->path) - 1);
    e->path[sizeof(e->path) - 1] = '\0';
    e->fd  = fd;
    e->cb  = cb;
    e->ctx = ctx;
    strncpy(e->last_val, baseline, sizeof(e->last_val) - 1);
    e->last_val[sizeof(e->last_val) - 1] = '\0';

    pthread_mutex_unlock(&s_mutex);

    EHSH_LOG_INFO("sferalabs_fd_poll: registered %s (baseline='%s')",
                  path, baseline);
    return 0;
}

/* ** Note this is not curretly used because it is uncertain that sferalabs syfs nodes support poll events. **/

int sferalabs_fd_poll_start(void)
{
    if (s_running)
        return 0;   /* Already started — idempotent */

    if (pipe(s_stop_pipe) != 0)
    {
        EHSH_LOG_ERROR("sferalabs_fd_poll: pipe() failed");
        return -1;
    }

    s_running = 1;

    int rc = pthread_create(&s_thread, NULL, fd_poll_thread_func, NULL);
    if (rc != 0)
    {
        s_running = 0;
        close(s_stop_pipe[0]);
        close(s_stop_pipe[1]);
        s_stop_pipe[0] = s_stop_pipe[1] = -1;
        EHSH_LOG_ERROR("sferalabs_fd_poll: pthread_create failed (rc=%d)", rc);
        return -1;
    }

    EHSH_LOG_INFO("sferalabs_fd_poll: poll thread started (%d entries)",
                  s_num_entries);
    return 0;
}

void sferalabs_fd_poll_stop(void)
{
    if (!s_running)
        return;

    s_running = 0;

    /* Unblock the poll() thread via the self-pipe */
    char b = 1;
    (void)write(s_stop_pipe[1], &b, 1);

    pthread_join(s_thread, NULL);

    close(s_stop_pipe[0]);
    close(s_stop_pipe[1]);
    s_stop_pipe[0] = s_stop_pipe[1] = -1;

    /* Close all registered fds */
    pthread_mutex_lock(&s_mutex);
    for (int i = 0; i < s_num_entries; i++)
    {
        close(s_entries[i].fd);
        s_entries[i].fd = -1;
    }
    pthread_mutex_unlock(&s_mutex);

    EHSH_LOG_INFO("sferalabs_fd_poll: poll thread stopped");
}
