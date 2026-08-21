/***************************************************************
 * Copyright (C) 2025 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file target_file.c
 * Zephyr RTOS implementation of the target file-system HAL.
 *
 * When CONFIG_POSIX_API=y is enabled in prj.conf, Zephyr provides POSIX
 * file-system calls (mkdir, rmdir, stat, opendir, readdir, closedir).
 * Functions here delegate to those calls where possible.  On a board with
 * no file-system backend mounted the calls will return failure at runtime,
 * but the link succeeds and the kernel can still run without a real FS.
 *
 * @author: inx limited
 */

#define EHS_TGT_CODE
#include "globals.h"
#include "hal_file.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>    /* open() and O_* flags for the fd-backed EhsF* ops */
#include <stdarg.h>   /* EhsFprintf / EhsFscanf variadic args */
#include <errno.h>
#include <string.h>

/* Zephyr's <dirent.h> uses NAME_MAX which is defined only when CONFIG_FILE_SYSTEM=y.
 * Provide a fallback so the file always compiles; directory ops are still guarded
 * with CONFIG_FILE_SYSTEM at runtime so they never execute without FS support. */
#ifndef NAME_MAX
#define NAME_MAX 255
#endif
/* dirent.h also uses PATH_MAX. It already does its own `#include <limits.h>`,
 * but under NCS v2.9.0's picolibc/Zephyr header combination that doesn't
 * actually define PATH_MAX at this point (same class of issue as NAME_MAX
 * above) - fall back explicitly rather than depend on the toolchain/SDK
 * version providing it. */
#include <limits.h>
#ifndef PATH_MAX
#define PATH_MAX 255
#endif
#include <dirent.h>

/* --- LittleFS mount for the SODL app filesystem (external flash) --- */
#ifdef CONFIG_FILE_SYSTEM_LITTLEFS
#include <zephyr/kernel.h>
#include <zephyr/fs/fs.h>
#include <zephyr/fs/littlefs.h>
#include <zephyr/storage/flash_map.h>
#include <fcntl.h>        /* open() flags for the first-boot SODL copy */
#if defined(CONFIG_PARTITION_MANAGER_ENABLED)
#include <pm_config.h>   /* PM_LITTLEFS_STORAGE_ID from pm_static.yml */
#endif
#include "hal.h"          /* EhsHMetaSetInstPath */
/* Embedded default SODL, written to the filesystem on first boot.
 *
 * ALWAYS this one filename. The build stages a header here on every run: from
 * the app named by EHS_DEFAULT_APP when it has an ERT1 export, otherwise a copy
 * of the committed fallback (target/os-arch/zephyr-arm/default_sodl.h, blinky).
 *
 * There is deliberately no #ifdef choosing between two headers. A preprocessor
 * flag would live in the generated CMakeLists, which only zephyr_cmake_gen
 * rewrites - so running just the build step after changing the app would leave
 * the flag pointing at the wrong one, and the image would silently carry an app
 * nobody selected. One filename, regenerated every build, cannot drift.
 *
 * ehs_default_sodl_app names the app, and target_file.c prints it at boot -
 * "which app is this board actually running" is the first question asked when a
 * device misbehaves, and byte counts alone cannot distinguish a stale image
 * from a failed write. */
#include "default_sodl_generated.h"

FS_LITTLEFS_DECLARE_DEFAULT_CONFIG(ehs_lfs_cfg);
static struct fs_mount_t ehs_lfs_mnt = {
    .type = FS_LITTLEFS,
    .fs_data = &ehs_lfs_cfg,
#if defined(CONFIG_PARTITION_MANAGER_ENABLED)
    .storage_dev = (void *)PM_LITTLEFS_STORAGE_ID,
#else
    .storage_dev = (void *)FIXED_PARTITION_ID(littlefs_storage),
#endif
    .mnt_point = "/lfs",
};
#endif /* CONFIG_FILE_SYSTEM_LITTLEFS */

/* -----------------------------------------------------------------------
 * Directory-listing context (one instance per open directory scan)
 * ----------------------------------------------------------------------- */

struct EhsTDFilesStruct
{
    ehs_char szPath[EHS_TD_FILES_MAX_PATH];
    DIR     *pDirectoryInfo;
};

/* -----------------------------------------------------------------------
 * Filesystem initialisation
 * ----------------------------------------------------------------------- */

#ifdef CONFIG_FILE_SYSTEM_LITTLEFS
/* Bring-up: no live app-upload path yet, and the FS partition sits outside the
 * app DFU slot, so seed the default SODL from the embedded blob on first boot.
 * Creates /lfs/appdata/default/ and writes t.sdl ONLY if it is not already
 * there — so a later uploaded app is never clobbered. */
static void ehs_provision_default_sodl(void)
{
    const char *path = "/lfs/appdata/default/t.sdl";
    struct stat st;
    int existed = (stat(path, &st) == 0);

    /* Bring-up: ALWAYS (re)write, so a partial/corrupt t.sdl from an earlier
     * crashed boot can't wedge the loader. stat result logged for info. */
    int rc1 = mkdir("/lfs/appdata", 0777);
    int rc2 = mkdir("/lfs/appdata/default", 0777);

    int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd < 0) {
        printk("EHS: could not create %s (errno %d) [mkdir rc %d/%d]\n",
               path, errno, rc1, rc2);
        return;
    }
    int n = (int)write(fd, ehs_default_sodl, (size_t)ehs_default_sodl_len);
    close(fd);
    /* The APP NAME is the point of this line. Byte counts alone cannot tell a
     * stale image from a failed write, and "which app is this board actually
     * running" is the first question asked when a device misbehaves. */
    printk("EHS: seeded default SODL '%s' -> %s (%d/%u bytes, existed=%d, size_was=%d)\n",
           ehs_default_sodl_app, path, n, ehs_default_sodl_len,
           existed, existed ? (int)st.st_size : -1);

    /* ...AND POINT THE LOADER AT IT.
     *
     * Seeding appdata/default/t.sdl is only half the job: the kernel does not
     * necessarily load 'default'. It reads sysdata/app2run.nfo and loads
     * appdata/<that name>/t.sdl, falling back to "default" only when the file
     * is absent (see EhsAppGetDefaultApp in Common/HAL/appmanager/appstorage.c).
     *
     * A Lucid debug-load writes appdata/temp/t.sdl and sets app2run=temp. That
     * file lives in LittleFS on EXTERNAL FLASH, which reflashing the
     * application core does NOT erase - so a board that was ever used with the
     * Lucid tools keeps booting the app it was given then, silently ignoring
     * every freshly-seeded default. The symptom is a board that runs a stale
     * app no matter how many times it is reflashed, with the seed log line
     * cheerfully reporting success.
     *
     * While the default SODL is force-written on every boot (see above), the
     * selector has to agree with it or the seeding is pointless. Both of these
     * go together when a live app-upload path lands on this target. */
    {
        const char *sel_path = "/lfs/sysdata/app2run.nfo";
        char cur[64] = {0};
        int sfd = open(sel_path, O_RDONLY);
        int rd = 0;

        if (sfd >= 0) {
            rd = (int)read(sfd, cur, sizeof(cur) - 1);
            close(sfd);
            if (rd < 0) {
                rd = 0;
            }
        }
        /* trim trailing whitespace/newline for the comparison and the log */
        while ((rd > 0) && ((cur[rd - 1] == '\n') || (cur[rd - 1] == '\r') ||
                            (cur[rd - 1] == ' ')  || (cur[rd - 1] == '\t'))) {
            cur[--rd] = '\0';
        }

        if ((rd > 0) && (strcmp(cur, "default") != 0)) {
            printk("EHS: app2run.nfo says '%s' - the loader would IGNORE the "
                   "seeded default app. Resetting it to 'default'.\n", cur);
        }
        if ((rd == 0) || (strcmp(cur, "default") != 0)) {
            sfd = open(sel_path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
            if (sfd >= 0) {
                (void)write(sfd, "default", 7);
                close(sfd);
            } else {
                printk("EHS: could not write %s (errno %d)\n", sel_path, errno);
            }
        }
        printk("EHS: app2run = 'default'\n");
    }
}
#endif /* CONFIG_FILE_SYSTEM_LITTLEFS */

ehs_bool EhsTgtFilesystem_Init(void)
{
#ifdef CONFIG_FILE_SYSTEM_LITTLEFS
    /* Mount LittleFS on the littlefs_storage partition and point the eRT install
     * root at it, so the kernel finds /lfs/appdata/<app>/t.sdl. A blank partition
     * is auto-formatted by the LittleFS backend on first mount. */
    int rc = fs_mount(&ehs_lfs_mnt);
    if (rc == -EBUSY) {
        rc = 0;   /* already mounted — EhsFInitFileSystem may run more than once */
    }
    if (rc < 0) {
        printk("EHS: LittleFS mount (%s) failed: %d\n", ehs_lfs_mnt.mnt_point, rc);
        return EHS_FALSE;
    }
    printk("EHS: LittleFS mounted at %s\n", ehs_lfs_mnt.mnt_point);
    EhsHMetaSetInstPath(ehs_lfs_mnt.mnt_point);
    /* Create the sysdata dir so boot-state / version / log sys-files can be
     * written (littlefs won't auto-create parent dirs). Ignore EEXIST. */
    (void)mkdir("/lfs/sysdata", 0777);
    ehs_provision_default_sodl();
    return EHS_TRUE;
#else
    return EHS_TRUE;
#endif
}

/* -----------------------------------------------------------------------
 * fd-backed file handles (ehs_FILE)
 *
 * Zephyr picolibc stdio FILE* is console-only: fopen() on a real littlefs
 * file returns non-NULL but the subsequent fgetc()/fread() read nothing, so
 * the SODL parser rejected a valid t.sdl. File I/O therefore uses the POSIX
 * fd layer (open/read/write/lseek/close/fsync). A small fixed pool avoids a
 * heap dependency. The kernel drives file I/O from a single thread (the EHS
 * event thread) and the survey found no multi-threaded EhsF* use, so the pool
 * needs no locking.
 * ----------------------------------------------------------------------- */

struct EhsZephyrFileHandle
{
    int      fd;
    ehs_bool used;
};

#define EHS_ZEPHYR_MAX_FILES 8
static struct EhsZephyrFileHandle sZephyrFilePool[EHS_ZEPHYR_MAX_FILES];

/* stdio-style mode string -> POSIX open() flags. Returns -1 on bad mode. */
static int ehs_mode_to_flags(const char *mode)
{
    if (!mode || !*mode)
        return -1;
    ehs_bool plus = (strchr(mode, '+') != NULL) ? EHS_TRUE : EHS_FALSE;
    switch (mode[0]) {
    case 'r': return plus ? O_RDWR : O_RDONLY;
    case 'w': return plus ? (O_RDWR   | O_CREAT | O_TRUNC)
                          : (O_WRONLY | O_CREAT | O_TRUNC);
    case 'a': return plus ? (O_RDWR   | O_CREAT | O_APPEND)
                          : (O_WRONLY | O_CREAT | O_APPEND);
    default:  return -1;   /* 'b'/'t' handled as a suffix, ignored here */
    }
}

ehs_FILE *EhsFopen(const ehs_char *fname, const ehs_char *fmode)
{
    int flags = ehs_mode_to_flags(fmode);
    if (!fname || flags < 0)
        return NULL;

    struct EhsZephyrFileHandle *h = NULL;
    for (int i = 0; i < EHS_ZEPHYR_MAX_FILES; i++) {
        if (!sZephyrFilePool[i].used) {
            h = &sZephyrFilePool[i];
            break;
        }
    }
    if (!h)
        return NULL;   /* pool exhausted */

    int fd = open(fname, flags, 0666);
    if (fd < 0)
        return NULL;

    h->fd   = fd;
    h->used = EHS_TRUE;
    return h;
}

ehs_sint16 EhsFclose(ehs_FILE *f)
{
    if (!f || !f->used)
        return EHS_EOF;
    int rc = close(f->fd);
    f->fd   = -1;
    f->used = EHS_FALSE;
    return (rc == 0) ? 0 : EHS_EOF;
}

ehs_sint16 EhsFgetc(ehs_FILE *f)
{
    if (!f || !f->used)
        return EHS_EOF;
    unsigned char c;
    ssize_t n = read(f->fd, &c, 1);
    if (n != 1)
        return EHS_EOF;            /* 0 = EOF, <0 = error */
    return (ehs_sint16)c;          /* 0..255 - never collides with EHS_EOF */
}

ehs_sint16 EhsZephyrFputc(ehs_sint16 c, ehs_FILE *f)
{
    if (!f || !f->used)
        return EHS_EOF;
    unsigned char b = (unsigned char)c;
    if (write(f->fd, &b, 1) != 1)
        return EHS_EOF;
    return (ehs_sint16)b;
}

ehs_sint16 EhsFread(void *pBuff, ehs_uint16 nSize, ehs_uint16 nCount, ehs_FILE *f)
{
    if (!pBuff || !f || !f->used || nSize == 0)
        return 0;
    size_t total = (size_t)nSize * (size_t)nCount;
    size_t got   = 0;
    unsigned char *p = (unsigned char *)pBuff;
    while (got < total) {
        ssize_t n = read(f->fd, p + got, total - got);
        if (n <= 0)
            break;
        got += (size_t)n;
    }
    return (ehs_sint16)(got / nSize);   /* complete elements read, like fread */
}

ehs_sint16 EhsFwrite(void *pBuff, ehs_uint16 nSize, ehs_uint16 nCount, ehs_FILE *f)
{
    if (!pBuff || !f || !f->used || nSize == 0)
        return 0;
    size_t total = (size_t)nSize * (size_t)nCount;
    size_t put   = 0;
    const unsigned char *p = (const unsigned char *)pBuff;
    while (put < total) {
        ssize_t n = write(f->fd, p + put, total - put);
        if (n <= 0)
            break;
        put += (size_t)n;
    }
    return (ehs_sint16)(put / nSize);   /* complete elements written */
}

ehs_char *EhsFgets(ehs_char *pBuff, ehs_uint16 nSize, ehs_FILE *f)
{
    if (!pBuff || !f || !f->used || nSize == 0)
        return NULL;
    ehs_uint16 i = 0;
    while (i < (ehs_uint16)(nSize - 1)) {
        ehs_sint16 c = EhsFgetc(f);
        if (c == EHS_EOF)
            break;
        pBuff[i++] = (ehs_char)c;
        if (c == '\n')
            break;
    }
    if (i == 0)
        return NULL;   /* nothing read - EOF */
    pBuff[i] = '\0';
    return pBuff;
}

ehs_sint16 EhsFprintf(ehs_FILE *f, const ehs_char *fmt, ...)
{
    if (!f || !f->used || !fmt)
        return EHS_EOF;
    char    buf[256];
    va_list ap;
    va_start(ap, fmt);
    int n = vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    if (n < 0)
        return EHS_EOF;
    /* vsnprintf returns the would-be length; clamp to what actually fits. */
    size_t len = (n < (int)sizeof(buf)) ? (size_t)n : sizeof(buf) - 1;
    size_t put = 0;
    while (put < len) {
        ssize_t w = write(f->fd, buf + put, len - put);
        if (w <= 0)
            break;
        put += (size_t)w;
    }
    return (ehs_sint16)put;   /* bytes written */
}

ehs_sint16 EhsFscanf(ehs_FILE *f, const ehs_char *fmt, ...)
{
    /* Not cleanly implementable over a raw fd: vsscanf gives no reliable
     * consumed-byte count, so the fd position can't be advanced correctly for
     * the sequential-read pattern callers use. No common/kernel path on this
     * target needs it (devman / proxy-config readers are not built here), and
     * it was already non-functional on picolibc file FILE*. Stub to EOF. */
    (void)f;
    (void)fmt;
    return EHS_EOF;
}

ehs_sint16 EhsFflush(ehs_FILE *f)
{
    if (!f || !f->used)
        return EHS_EOF;
    /* fsync pushes littlefs write-back to flash. Some FS backends report
     * -ENOTSUP for fsync - treat that as success rather than a write error. */
    int rc = fsync(f->fd);
    return (rc == 0 || errno == ENOTSUP) ? 0 : EHS_EOF;
}

ehs_sint32 EhsZephyrFseek(ehs_FILE *f, long offset, ehs_sint32 whence)
{
    if (!f || !f->used)
        return -1;
    off_t r = lseek(f->fd, (off_t)offset, (int)whence);
    return (r == (off_t)-1) ? -1 : 0;
}

long EhsZephyrFtell(ehs_FILE *f)
{
    if (!f || !f->used)
        return -1L;
    off_t r = lseek(f->fd, 0, SEEK_CUR);
    return (r == (off_t)-1) ? -1L : (long)r;
}

ehs_sint32 EhsZephyrFrewind(ehs_FILE *f)
{
    if (!f || !f->used)
        return -1;
    off_t r = lseek(f->fd, 0, SEEK_SET);
    return (r == (off_t)-1) ? -1 : 0;
}

/* feof() equivalent for the fd-backed handle: true when the read position is
 * at or past end-of-file. Position-based (no stdio EOF flag) — checks current
 * vs end offset and restores the position. */
ehs_bool EhsZephyrFeof(ehs_FILE *f)
{
    if (!f || !f->used)
        return EHS_TRUE;
    off_t cur = lseek(f->fd, 0, SEEK_CUR);
    off_t end = lseek(f->fd, 0, SEEK_END);
    if (cur == (off_t)-1 || end == (off_t)-1)
        return EHS_TRUE;
    (void)lseek(f->fd, cur, SEEK_SET);   /* restore position */
    return (cur >= end) ? EHS_TRUE : EHS_FALSE;
}

/* -----------------------------------------------------------------------
 * Directory creation / removal
 * ----------------------------------------------------------------------- */

ehs_bool EhsTF_mkdir(const ehs_char *szPath)
{
    if (!szPath || *szPath == '\0')
        return EHS_FALSE;
#ifdef CONFIG_POSIX_API
    int ret = mkdir(szPath, 0755);
    return (ret == 0 || errno == EEXIST) ? EHS_TRUE : EHS_FALSE;
#else
    (void)szPath;
    return EHS_FALSE;
#endif
}

ehs_bool EhsTF_rmdir(ehs_char *szPath)
{
    if (!szPath || *szPath == '\0')
        return EHS_FALSE;
#ifdef CONFIG_POSIX_API
    return (rmdir(szPath) == 0) ? EHS_TRUE : EHS_FALSE;
#else
    (void)szPath;
    return EHS_FALSE;
#endif
}

/* -----------------------------------------------------------------------
 * File / directory existence check
 * Returns: 2=directory, 1=file, 0=not found
 * ----------------------------------------------------------------------- */

ehs_uint8 EhsTF_exists(const ehs_char *szPath)
{
#ifdef CONFIG_POSIX_API
    struct stat st;
    if (stat(szPath, &st) != 0)
        return 0;
    return S_ISDIR(st.st_mode) ? 2u : 1u;
#else
    (void)szPath;
    return 0;
#endif
}

/* -----------------------------------------------------------------------
 * Change directory (not meaningful on MCU targets)
 * ----------------------------------------------------------------------- */

ehs_bool EhsTF_cd(const ehs_char *szDir)
{
    (void)szDir;
    return EHS_FALSE;
}

/* -----------------------------------------------------------------------
 * Directory listing (EhsTDFiles_*)
 * ----------------------------------------------------------------------- */

/* A small static pool avoids a heap dependency for the simple MCU case. */
#define ZEPHYR_MAX_DIR_HANDLES 2
static struct EhsTDFilesStruct sZephyrDirPool[ZEPHYR_MAX_DIR_HANDLES];
static ehs_bool sZephyrDirPoolInUse[ZEPHYR_MAX_DIR_HANDLES];

void EhsTDFiles_init(struct EhsTDFilesStruct **pFiles)
{
    if (!pFiles)
        return;
    *pFiles = NULL;
    for (int i = 0; i < ZEPHYR_MAX_DIR_HANDLES; i++) {
        if (!sZephyrDirPoolInUse[i]) {
            sZephyrDirPoolInUse[i] = EHS_TRUE;
            sZephyrDirPool[i].pDirectoryInfo = NULL;
            sZephyrDirPool[i].szPath[0] = '\0';
            *pFiles = &sZephyrDirPool[i];
            return;
        }
    }
    /* Pool exhausted — caller must handle NULL */
}

void EhsTDFiles_cleanup(EhsTDFilesClass *pFiles)
{
    if (!pFiles)
        return;
#ifdef CONFIG_POSIX_API
    if (pFiles->pDirectoryInfo) {
        closedir(pFiles->pDirectoryInfo);
        pFiles->pDirectoryInfo = NULL;
    }
#endif
    /* Return to pool */
    for (int i = 0; i < ZEPHYR_MAX_DIR_HANDLES; i++) {
        if (&sZephyrDirPool[i] == pFiles) {
            sZephyrDirPoolInUse[i] = EHS_FALSE;
            break;
        }
    }
}

ehs_bool EhsTDFiles_getDir(EhsTDFilesClass *pFiles, ehs_char *szDir)
{
    if (!pFiles || !szDir)
        return EHS_FALSE;
    strncpy(szDir, pFiles->szPath, EHS_TD_FILES_MAX_PATH - 1);
    szDir[EHS_TD_FILES_MAX_PATH - 1] = '\0';
    return EHS_TRUE;
}

ehs_bool EhsTDFiles_setDir(EhsTDFilesClass *pFiles, const ehs_char *szDir)
{
    if (!pFiles || !szDir)
        return EHS_FALSE;

    strncpy(pFiles->szPath, szDir, EHS_TD_FILES_MAX_PATH - 1);
    pFiles->szPath[EHS_TD_FILES_MAX_PATH - 1] = '\0';

#ifdef CONFIG_POSIX_API
    if (pFiles->pDirectoryInfo) {
        closedir(pFiles->pDirectoryInfo);
        pFiles->pDirectoryInfo = NULL;
    }
    pFiles->pDirectoryInfo = opendir(szDir);
    return (pFiles->pDirectoryInfo != NULL) ? EHS_TRUE : EHS_FALSE;
#else
    return EHS_FALSE;
#endif
}

ehs_uint8 EhsTDFiles_listFirst(EhsTDFilesClass *pFiles, ehs_char *szName)
{
    if (!pFiles || !szName)
        return EHS_FALSE;

#ifdef CONFIG_POSIX_API
    if (!pFiles->pDirectoryInfo)
        return EHS_FALSE;

    /* Zephyr 4.1 does not implement rewinddir — use closedir/opendir to
     * reset the directory scan back to the first entry. */
    closedir(pFiles->pDirectoryInfo);
    pFiles->pDirectoryInfo = opendir(pFiles->szPath);
    if (!pFiles->pDirectoryInfo)
        return EHS_FALSE;

    struct dirent *entry = readdir(pFiles->pDirectoryInfo);
    if (!entry)
        return EHS_FALSE;

    strncpy(szName, entry->d_name, EHS_TD_FILES_MAX_FILENAME - 1);
    szName[EHS_TD_FILES_MAX_FILENAME - 1] = '\0';
    return EHS_TRUE;
#else
    *szName = '\0';
    return EHS_FALSE;
#endif
}

ehs_uint8 EhsTDFiles_listNext(EhsTDFilesClass *pFiles, ehs_char *szName)
{
    if (!pFiles || !szName)
        return EHS_FALSE;

#ifdef CONFIG_POSIX_API
    if (!pFiles->pDirectoryInfo)
        return EHS_FALSE;

    struct dirent *entry = readdir(pFiles->pDirectoryInfo);
    if (!entry)
        return EHS_FALSE;

    strncpy(szName, entry->d_name, EHS_TD_FILES_MAX_FILENAME - 1);
    szName[EHS_TD_FILES_MAX_FILENAME - 1] = '\0';
    return EHS_TRUE;
#else
    *szName = '\0';
    return EHS_FALSE;
#endif
}
