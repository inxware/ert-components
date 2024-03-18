/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/
 
/** @file targetos_init.h
 * The target-specific declarations required to support the HAL for process
 * services and related OS interations are defined here. This file should only
 * be included by hal_process.h
 *
 * @author: inx limited
 *
 */

#ifndef EHS_TARGET_OS_INIT_H
#define EHS_TARGET_OS_INIT_H

/*****************************************************************************/
/* Included files */
#include "globals.h"
#include "hal-api.h"

/*****************************************************************************/
/* Define macros  */

/*****************************************************************************/
/* Define types */

/*****************************************************************************/
/* Declare global variables */

/*****************************************************************************/
/* Declare function prototypes  */

/* updated dynamic and static data
 * The what parameter can be used 0: get everything, 1 get just static 2 just
 * dynamic
 */

EHS_GLOBAL ehs_bool EhsTOsSys_UpdateEnvironment(EhsMetaDataType *pEhsMetaData,
        ehs_uint8 what);

/**
 * Perform necessary Operating system setup upon system initialisation
 */
EHS_GLOBAL void EhsTOsSys_init(void);

/**
 * Called to shutdown the target prior to shutting down EHS
 */
EHS_GLOBAL void EhsTOsSys_term(void);

/**
 * Perform setup before loading in a new application
 */
EHS_GLOBAL void EhsTOsApp_init(void);

/**
 * Called to clean up after the execution of an application in order to make
 * sure everything has been stopped.
 */
EHS_GLOBAL void EhsTOsApp_term(void);

/**
 * Perform necessary target setup per application execution
 */
EHS_GLOBAL void EhsTOsApp_reset(void);

/* Function to add EHS's canonical bin to the search path */
void EhsBinSearchPath(void);

#endif /* EHS_TARGET_OS_INIT_H */
