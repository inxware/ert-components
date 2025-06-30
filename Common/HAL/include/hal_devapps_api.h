/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
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

ehs_bool EhsAppSetDefaultApp(ehs_char * app); /* Sets the app2run file for next app that runs */
ehs_bool EhsAppDenyCurrentApp(); /* When an application does not initialise or has a runtime fault this is called to configure a retry sequence of other apps */
void EhsAppConfirmCurrentApp(); /*When an application initialised successfully this is called to do any tidying necessary */

#endif /* EHS_HAL_DEVAPPS_API_H */