/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file hal_console.h
 * In this file, all of the hardware abstraction layer declarations provided for EHS console I/O are given.
 *
 * @author: inx limited
 *
 */

#ifndef EHS_HAL_DEVAPPS_API_H
#define EHS_HAL_DEVAPPS_API_H

#include "globals.h"

ehs_bool EhsAppGetDefaultApp(ehs_char * cDefaultApp); /* Gets the canonical name of the default app specified in the app2run file */
ehs_bool EhsAppSetDefaultApp(ehs_char * app); /* Sets the app2run file for next app that runs */
ehs_bool EhsAppDenyCurrentApp(); /* When an application does not initialise or has a runtime fault this is called to configure a retry sequence of other apps */
void EhsAppConfirmCurrentApp(); /*When an application initialised successfully this is called to do any tidying necessary */

/* By-name generalisation of EhsAppDenyCurrentApp: remove the named app's live
 * directory and, if a _prev version exists, reinstate it (return TRUE); otherwise
 * return FALSE. If the denied app was the live-meta current, metadata is rolled
 * to the default app. Refuses EHS_SYS_APP_DEFAULT_NAME.
 * Backs the kernel console 'X' command, the deleteApp FB port, and the boot-time
 * crash-auto-delete path — same semantic as SetupApplication's parse-failure recovery. */
ehs_bool EhsAppDenyApp(const ehs_char *app);

#endif /* EHS_HAL_DEVAPPS_API_H */