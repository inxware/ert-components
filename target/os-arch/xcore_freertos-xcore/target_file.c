/***************************************************************
 * Copyright (C) 2008-2025 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/**
 * @file target_file.c
 * File system HAL for the XMOS xcore.ai / FreeRTOS target.
 *
 * Uses the POSIX file API provided by fwk_rtos.  Directory listing
 * (EhsTDFiles_*) is stubbed — xcore bring-up does not require it.
 * EhsTgtFilesystem_Init() is a no-op: the fwk_rtos POSIX layer
 * is initialised before the ERT task starts.
 */

#include "globals.h"
#include "target_file.h"
#include "targetos_init.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/* -------------------------------------------------------------------------
 * Basic filesystem operations
 * ------------------------------------------------------------------------- */

ehs_bool EhsTF_cd(const ehs_char *dir)
{
    return (chdir(dir) == 0);
}

ehs_bool EhsTF_mkdir(const char *szParameterFilePath)
{
    return (mkdir(szParameterFilePath, S_IRWXU) == 0);
}

static int remove_directory(const char *path)
{
    /* Stub — recursive directory removal not required during bring-up */
    return rmdir(path);
}

ehs_bool EhsTF_rmdir(ehs_char *szPath)
{
    ehs_sint16 ret = 0;
    if (EhsTF_exists(szPath) >= 1)
    {
        ret = remove_directory(szPath);
        if (ret < 0)
        {
            ret = remove(szPath);
        }
    }
    else
    {
        ret = -1;
    }
    return (ret == 0);
}

ehs_uint8 EhsTF_exists(const ehs_char *fname)
{
    if (fname == NULL)   return 0;
    if (fname[0] == '\0') return 0;

    struct stat xFileInfo;
    int stat_ret = stat(fname, &xFileInfo);
    if (stat_ret == -1)
    {
        return (errno == ENOENT) ? 0 : 0;
    }
    /* Return 2 for directory, 1 for file (matches other target ports) */
    return S_ISDIR(xFileInfo.st_mode) ? 2 : 1;
}

/* -------------------------------------------------------------------------
 * Directory listing — stubbed for bring-up
 * ------------------------------------------------------------------------- */

void EhsTDFiles_cleanup(EhsTDFilesClass *pFiles)
{
    (void)pFiles;
}

ehs_bool EhsTDFiles_setDir(EhsTDFilesClass *pFiles, const ehs_char *szDir)
{
    (void)pFiles;
    (void)szDir;
    return EHS_FALSE;
}

ehs_uint8 EhsTDFiles_listFirst(EhsTDFilesClass *pFiles, ehs_char *szName)
{
    (void)pFiles;
    (void)szName;
    return EHS_FALSE;
}

ehs_uint8 EhsTDFiles_listNext(EhsTDFilesClass *pFiles, ehs_char *szName)
{
    (void)pFiles;
    (void)szName;
    return EHS_FALSE;
}

void EhsTDFiles_init(struct EhsTDFilesStruct **pFiles)
{
    (void)pFiles;
}

/* -------------------------------------------------------------------------
 * Filesystem initialisation
 *
 * On xcore / fwk_rtos the POSIX layer is brought up by the RTOS before
 * the ERT task runs, so there is nothing to do here.
 * ------------------------------------------------------------------------- */

ehs_bool EhsTgtFilesystem_Init(void)
{
    printf("xcore: filesystem (POSIX via fwk_rtos) ready\n");
    return EHS_TRUE;
}
