/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file hal_bootstate.c
 * Default file-backed implementation of the persistent boot-state API.
 *
 * Each state lives in a small file in the system dir whose contents are the
 * decimal counter (in ASCII). Set increments and rewrites; Clear deletes the
 * file. IsSet / GetCount read it back.
 *
 * Targets that need NVRAM/RTC-RAM backing can replace this .c with their own
 * implementation of the prototypes in hal_bootstate.h.
 */

#define EHSL_MODULE_ID (EHSH_LOG_MODULE_UNDEFINED) /* define before hal_logger.h */

#include "globals.h"
#include "hal-api.h"
#include "hal_bootstate.h"

#ifndef EHS_APP_FAILED_BOOT_LIMIT
/* Default to 1: deny on first unconfirmed boot, mirroring crash-auto-delete.
 * The build always sets this via ert-components/Common/Ehs/ehs.mk; the guard
 * is here so the file still compiles in tooling that drops the -D. */
#define EHS_APP_FAILED_BOOT_LIMIT 1
#endif

/* Filename for each state. Keep the index aligned with EhsBootStateType.
 * One file per state keeps the on-disk format trivial and lets us add new
 * states by extending the enum + this table without touching readers. */
static const ehs_char *EhsBootStateFilename[EHS_BOOT_STATE_COUNT] = {
    [EHS_BOOT_STATE_APP_LOAD_STARTED] = "bootstate_app_load_started.flag",
};

static const ehs_char *EhsBootStateNameFor(EhsBootStateType state)
{
    if ((unsigned)state >= (unsigned)EHS_BOOT_STATE_COUNT) return NULL;
    return EhsBootStateFilename[state];
}

ehs_uint32 EhsHBootStateGetCount(EhsBootStateType state)
{
    const ehs_char *fname = EhsBootStateNameFor(state);
    if (fname == NULL) return 0;

    ehs_FILE *f = Ehs_SysFopen(fname, "r");
    if (f == NULL) return 0;

    ehs_char buf[16];
    buf[0] = '\0';
    EhsFgets(buf, sizeof(buf), f);
    EhsFclose(f);

    /* atoi-style parse — content is always ASCII decimal written by us. */
    ehs_uint32 n = 0;
    for (ehs_uint8 i = 0; buf[i] >= '0' && buf[i] <= '9'; ++i) {
        n = (n * 10u) + (ehs_uint32)(buf[i] - '0');
    }
    return n;
}

ehs_bool EhsHBootStateIsSet(EhsBootStateType state)
{
    return (EhsHBootStateGetCount(state) > 0u) ? EHS_TRUE : EHS_FALSE;
}

ehs_bool EhsHBootStateSet(EhsBootStateType state)
{
    const ehs_char *fname = EhsBootStateNameFor(state);
    if (fname == NULL) return EHS_FALSE;

    ehs_uint32 next = EhsHBootStateGetCount(state) + 1u;

    ehs_FILE *f = Ehs_SysFopen(fname, "w");
    if (f == NULL) {
        EHSH_LOG_WARNING("Could not open boot-state file '%s' for write", fname);
        return EHS_FALSE;
    }
    EhsFprintf(f, "%u", (unsigned)next);
    EhsFclose(f);
    return EHS_TRUE;
}

ehs_bool EhsHBootStateClear(EhsBootStateType state)
{
    const ehs_char *fname = EhsBootStateNameFor(state);
    if (fname == NULL) return EHS_FALSE;

    /* Best-effort: a missing file is already "cleared". Ehs_SysRm returns
     * FALSE if the file doesn't exist on some targets, so don't propagate
     * that as failure. */
    if (EhsTF_exists(fname) > 0) {
        Ehs_SysRm((ehs_char *)fname);
    }
    return EHS_TRUE;
}

ehs_bool EhsHFailedBootShouldDenyApp(void)
{
    /* Master switch EHS_APP_AUTO_DELETE forces the whole boot-time auto-deny
     * feature off (default for now — see Common/Ehs/ehs.mk). The kernel still
     * sets/clears the load-started flag and increments the counter, but the
     * deny action is suppressed when the master switch is off. */
#ifndef EHS_APP_AUTO_DELETE
    return EHS_FALSE;
#else
    return (EhsHBootStateGetCount(EHS_BOOT_STATE_APP_LOAD_STARTED)
            >= (ehs_uint32)EHS_APP_FAILED_BOOT_LIMIT)
           ? EHS_TRUE : EHS_FALSE;
#endif
}
