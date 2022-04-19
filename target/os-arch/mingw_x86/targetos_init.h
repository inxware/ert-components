/** @file target_process.h
 * The target-specific declarations required to support the HAL for process services
 * and related OS interations are defined here.
 * This file should only be included by hal_process.h
 *
 * @author: inx limited
 * @version: $Revision: 3033 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 *
 * Copyright (c) inx limited, 2006. All rights reserved.
 */

#ifndef EHS_TARGET_OS_INIT_H
#define EHS_TARGET_OS_INIT_H

/*****************************************************************************/
/* Included files */
#include "target.h"
#include "hal-api.h"

/*****************************************************************************/
/* Define macros  */

/*****************************************************************************/
/* Define types */

/*****************************************************************************/
/* Declare global variables */

/*****************************************************************************/
/* Declare function prototypes  */

ehs_bool EhsTOsSys_UpdateEnvironment(EhsMetaDataType * pEhsMetaData,ehs_uint8 which);

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

/*
 * Update the environment structure, which = 0,1,2. 1: static only, 2 dynamic only, and 0 is both.
 */


#endif /* EHS_TARGET_OS_INIT_H */
