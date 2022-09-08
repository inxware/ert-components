/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

/** @file globals.h
 * Declarations of  Module Loading functions to build the module loader against.
 */


#ifndef EHS_MODULES_H
#define EHS_MODULES_H

/*****************************************************************************/
/* Included files */

#include "target_config.h" /* special include required before macro definitions */
#include "ehs_types.h"

/*****************************************************************************/
/* Define macros  */


/**
 * Initialize all global variables to the correct state for startup
 */
void EhsAddStaticModules(void);

/*
 * Implement calls to library initialisers and loaders.
 *
 * Called once by the kernel at system start.
 */
ehs_bool EhsInitStaticModules();


/*
 * Implement calls to library close down.
 *
 * Called on ehs close (if this ever happens).
 * or if the EHS kernel decides too.
 */
ehs_bool EhsShutdownStaticModules();


/*
 * Implement calls to library reset down.
 *
 * This may implement any lightweight system resetting functions.
 *
 * This maybe called when new applications are loaded.
 */
ehs_bool EhsResetStaticModules();



#endif /* EHS_GLOBALS */

