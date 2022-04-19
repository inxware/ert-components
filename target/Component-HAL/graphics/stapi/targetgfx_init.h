/** @file target_process.h
 * The target-specific declarations required to support the HAL for process services
 * and related OS interations are defined here. 
 * This file should only be included by hal_process.h
 * 
 * @author: inx limited
 * @version: $Revision: 1242 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 * 
 * Copyright (c) inx limited, 2006. All rights reserved.
 */

#ifndef EHS_TARGET_GFX_INIT_H
#define EHS_TARGET_GFX_INIT_H

/*****************************************************************************/
/* Included files */
#include "target.h"

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
