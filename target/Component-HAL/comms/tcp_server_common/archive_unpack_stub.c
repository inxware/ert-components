/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/** @file archive_unpack_stub.c
 *
 * No-op stub implementation of EhsConsoleUnpackArchive() for platforms built
 * without libarchive. The real (libarchive) implementation lives in
 * archive_unpack.c and is selected by the build when EHS_LIBARCHIVE_SUPPORT=yes
 * in the os-arch target.mk.
 *
 * @author: inx limited
 */

#include "globals.h"

#include "hal-api.h"        /* logging */
#include "hal_console.h"    /* EhsConsoleUnpackArchive prototype */

ehs_bool EhsConsoleUnpackArchive(const ehs_char *name, ehs_bool removeArchive)
{
    (void)removeArchive;
    EHSH_LOG_WARNING("Archive unpack of [%s] requested, but this build has no libarchive support - ignoring",
                     (name != NULL) ? name : "(null)");
    return EHS_FALSE;
}
