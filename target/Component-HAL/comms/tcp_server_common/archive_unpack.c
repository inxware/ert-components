/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/** @file archive_unpack.c
 *
 * libarchive-backed implementation of EhsConsoleUnpackArchive().
 *
 * This file is only compiled into platforms built with libarchive available
 * (EHS_LIBARCHIVE_SUPPORT=yes in the os-arch target.mk). Platforms without
 * libarchive compile archive_unpack_stub.c instead, which provides a no-op
 * implementation of the same function.
 *
 * The console "load" command (kernel side, EhsCommandLoad) saves a received
 * file via EhsConsoleToFile into the "temp" directory under appdata. When the
 * file is a tarball it then calls EhsConsoleUnpackArchive to extract it into
 * the current application's live directory.
 *
 * @author: inx limited
 */

#include "globals.h"

#include "hal-api.h"        /* logging */
#include "hal_console.h"    /* EhsConsoleUnpackArchive prototype */
#include "hal_file.h"       /* EhsTF_tryCanonicPath, path separators */
#include "hal_devapps.h"    /* EhsAppMakeLiveDirString */
#include "hal_appstorage.h" /* EHS_SYS_APP_TEMP_NAME */
#include "hal.h"            /* EhsHMetaAppGetCurrent */

/* Use the same generic archive library used by the app getter (devappget.c) */
#include <archive.h>
#include <archive_entry.h>

/* Pump the data blocks of one entry from the read archive to the disk writer.
 *
 * Note: the off_t/size_t typedefs used by libarchive can differ in width from
 * those used by EHS, so (as in devappget.c) we let libarchive own those values
 * via local variables it writes into. */
static ehs_sint32 EhsArchive_copyData(struct archive *ar, struct archive *aw)
{
    const void *databuf;
    size_t blocksize;
#ifdef __LA_INT64_T
    __LA_INT64_T offset;
#else
    off_t offset;
#endif
    ehs_sint32 r;

    for (;;)
    {
        r = archive_read_data_block(ar, &databuf, &blocksize, &offset);
        if (r == ARCHIVE_EOF)
        {
            return ARCHIVE_OK;
        }
        if (r != ARCHIVE_OK && r != ARCHIVE_RETRY)
        {
            EHSH_LOG_ERROR("Archive read error: %s", archive_error_string(ar));
            return r;
        }
        r = archive_write_data_block(aw, databuf, blocksize, offset);
        if (r != ARCHIVE_OK && r != ARCHIVE_RETRY)
        {
            EHSH_LOG_ERROR("Archive write error: %s", archive_error_string(aw));
            return r;
        }
    }
}

ehs_bool EhsConsoleUnpackArchive(const ehs_char *name, ehs_bool removeArchive)
{
    struct archive *a = NULL;        /* reader */
    struct archive *ext = NULL;      /* disk writer */
    struct archive_entry *entry;
    ehs_char relPath[EHS_MAXPATHLENGTH];
    ehs_char srcPath[EHS_MAXPATHLENGTH];
    ehs_char destDir[EHS_MAXPATHLENGTH];
    ehs_char entryPath[EHS_TD_FILES_MAX_PATH];
    const ehs_char *currentApp;
    ehs_bool bRet = EHS_FALSE;
    ehs_sint32 flags;
    ehs_sint32 r;

    if (NULL == name)
    {
        return EHS_FALSE;
    }

    /* Resolve the saved tarball path: appdata/temp/<name> (same location that
     * EhsConsoleToFile wrote to). */
    EhsStrcpy(relPath, EHS_SYS_APP_TEMP_NAME);
    EhsStrcat(relPath, EHS_TD_FILES_SEPARATOR_STR);
    EhsStrcat(relPath, name);
    if (!EhsTF_tryCanonicPath(srcPath, EHS_RUNTIME_APPDATA_DIR, relPath, EHS_FALSE))
    {
        EHSH_LOG_ERROR("Could not resolve archive path for [%s]", name);
        return EHS_FALSE;
    }

    /* Resolve the destination: the current application's live directory. */
    currentApp = EhsHMetaAppGetCurrent();
    if ((NULL == currentApp) || ('\0' == currentApp[0]))
    {
        EHSH_LOG_ERROR("No current application - cannot unpack archive [%s]", name);
        return EHS_FALSE;
    }
    if (!EhsAppMakeLiveDirString(destDir, currentApp))
    {
        EHSH_LOG_ERROR("Could not resolve live app directory for [%s]", currentApp);
        return EHS_FALSE;
    }

    EHSH_LOG_INFO("Unpacking archive [%s] into application directory [%s]", srcPath, destDir);

    /* Preserve timestamps but NOT permissions/ownership/ACLs/file-flags from the
     * archive (these tarballs are application payloads, not system images).
     * ARCHIVE_EXTRACT_SECURE_NODOTDOT rejects entries containing ".." and
     * ARCHIVE_EXTRACT_SECURE_SYMLINKS refuses to write through symlinks, so a
     * malicious archive cannot escape the application directory. */
    flags = ARCHIVE_EXTRACT_TIME;
    flags |= ARCHIVE_EXTRACT_SECURE_NODOTDOT;
    flags |= ARCHIVE_EXTRACT_SECURE_SYMLINKS;

    a = archive_read_new();
    if (NULL == a)
    {
        EHSH_LOG_ERROR("Could not create archive reader");
        goto done;
    }
    archive_read_support_compression_all(a);
    archive_read_support_format_all(a);

    ext = archive_write_disk_new();
    if (NULL == ext)
    {
        EHSH_LOG_ERROR("Could not create archive disk writer");
        goto done;
    }
    archive_write_disk_set_options(ext, flags);
    archive_write_disk_set_standard_lookup(ext);

    if (archive_read_open_filename(a, srcPath, 16384) != ARCHIVE_OK)
    {
        EHSH_LOG_ERROR("Could not open archive [%s]: %s", srcPath, archive_error_string(a));
        goto done;
    }

    bRet = EHS_TRUE; /* assume success unless an entry fails */
    for (;;)
    {
        r = archive_read_next_header(a, &entry);
        if (r == ARCHIVE_EOF)
        {
            break;
        }
        if (r != ARCHIVE_OK)
        {
            EHSH_LOG_ERROR("Error reading archive header: %s", archive_error_string(a));
            bRet = EHS_FALSE;
            break;
        }

        /* Rebase the entry path under the destination application directory. */
        EhsStrcpy(entryPath, destDir);
        EhsStrcat(entryPath, EHS_TD_FILES_SEPARATOR_STR);
        EhsStrcat(entryPath, archive_entry_pathname(entry));
        archive_entry_set_pathname(entry, entryPath);

        r = archive_write_header(ext, entry);
        if (r != ARCHIVE_OK)
        {
            EHSH_LOG_ERROR("Could not write header for [%s]: %s", entryPath, archive_error_string(ext));
            bRet = EHS_FALSE;
            break;
        }

        if (archive_entry_size(entry) > 0)
        {
            if (EhsArchive_copyData(a, ext) != ARCHIVE_OK)
            {
                bRet = EHS_FALSE;
                break;
            }
        }
        archive_write_finish_entry(ext);
    }

done:
    /* @todo archive_read_finish/archive_write_finish are deprecated in libarchive 3;
     * matched here to devappget.c for consistency with the toolchain's library version. */
    if (a)   archive_read_finish(a);
    if (ext) archive_write_finish(ext);

    if (bRet == EHS_TRUE)
    {
        EHSH_LOG_INFO("Archive [%s] unpacked OK into [%s]", name, destDir);
        if (removeArchive)
        {
            /* The contents are now extracted - drop the (often large) source tarball. */
            EhsHRemove(srcPath);
        }
    }
    return bRet;
}
