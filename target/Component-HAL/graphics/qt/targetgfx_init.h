/***************************************************************
 * Copyright (C) 2025 inx limited, UK - All Rights Reserved.
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license
 * with this file. If not, please visit:
 *  <https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/*
 * Graphics subsystem initialisation hooks
 */

#ifndef EHS_TARGET_GFX_INIT_H
#define EHS_TARGET_GFX_INIT_H

#include "globals.h"

// #define CAIRO_TARGET

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
