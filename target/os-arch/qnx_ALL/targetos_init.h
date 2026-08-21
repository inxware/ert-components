/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file targetos_init.h
 * Target-specific declarations for OS initialisation on QNX Neutrino AArch64.
 * This file should only be included by hal_process.h.
 *
 * @author: inx limited
 */

#ifndef EHS_TARGET_OS_INIT_H
#define EHS_TARGET_OS_INIT_H

#include "globals.h"
#include "hal-api.h"

/*****************************************************************************/
/* Declare function prototypes  */

/** Updated dynamic and static data.
 *  @param what  0: everything, 1: static only, 2: dynamic only
 */
ehs_bool EhsTOsSys_UpdateEnvironment(EhsMetaDataType * pEhsMetaData, ehs_uint8 what);

/** Perform necessary OS setup upon system initialisation */
void EhsTOsSys_init(void);

/** Called to shutdown the target prior to shutting down EHS */
void EhsTOsSys_term(void);

/** Perform setup before loading a new application */
void EhsTOsApp_init(void);

/** Called to clean up after application execution */
void EhsTOsApp_term(void);

/** Perform necessary target setup per application execution */
void EhsTOsApp_reset(void);

/** Add EHS's canonical bin directory to the search path */
void EhsBinSearchPath(void);

#endif /* EHS_TARGET_OS_INIT_H */
