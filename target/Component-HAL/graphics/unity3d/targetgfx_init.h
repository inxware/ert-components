/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

/** @file target_process.h
 * The target-specific declarations required to support the HAL for process services
 * and related OS interations are defined here.
 * This file should only be included by hal_process.h
 *
 * @author: inx limited
 *
 */

#ifndef EHS_TARGET_GFX_INIT_H
#define EHS_TARGET_GFX_INIT_H

/*****************************************************************************/
/* Included files */
#include "globals.h"

/*****************************************************************************/
/* Define macros  */

/*****************************************************************************/
/* Define types */

/*****************************************************************************/
/* Declare global variables */

/*****************************************************************************/
/* Declare function prototypes  */

/**
 * Perform necessary Operating system setup upon system initialisation
 */
EHS_GLOBAL void EhsTGfxSys_init(void);

/**
 * Called to shutdown the target prior to shutting down EHS
 */
EHS_GLOBAL void EhsTGfxSys_term(void);

/**
 * Perform setup before loading in a new application
 */
EHS_GLOBAL void EhsTGfxApp_init(void);

/**
 * Called to clean up after the execution of an application in order to make
 * sure everything has been stopped.
 */
EHS_GLOBAL void EhsTGfxApp_term(void);

/**
 * Perform necessary target setup per application execution
 */
EHS_GLOBAL void EhsTGfxApp_reset(void);

#endif /* EHS_TARGET_GFX_INIT_H */