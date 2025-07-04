/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file targetos_init.c
 * Initialization/reset/termination code for the OS
 *
 * @author: inx limited
 *
 */

/**
 * @page Verification Verification report
 * @section hal_graphics_font
 * @anchor hal_graphics_font
 * @subsection misra MISRA compliance:
 * test.c demonstrated MISRA compliant on
 * Last modified on $Date:$
 *
 * This file contained no derogations to the MISRA standard.
 */

/*****************************************************************************/
/* Included files */
#define EHS_TARGET_CODE /* Ensure header files include target-internal values */
#include "../gst10_common/targetvideo_init.h"
#include "blockref_table.h"
#include "hal_viewport.h"
#include "graphics.h"

/*****************************************************************************/
/* Declare macros and local typedefs used by this file */

/*****************************************************************************/
/* Declare prototypes of local functions */

/*****************************************************************************/
/* Variables defined with file-scope */

/*****************************************************************************/
/* Variables defined with global-scope */



/*****************************************************************************/
/* Function definitions */

/**
 * Perform necessary Operating system setup upon system initialisation
 */
void EhsTVideoSys_init(void)
{
}


void EhsTV_ReInitVideo(const EhsGraphicsRectangleClass*  pBounds)
{
}


/**
 * Called to shutdown the target prior to shutting down EHS
 */
void EhsTVideoSys_term(void)
{
}

/**
 * Perform setup before loading in a new application
 */
void EhsTVideoApp_init(void)
{
}

/**
 * Called to clean up after the execution of an application in order to make
 * sure everything has been stopped.
 */
void EhsTVideoApp_term(void)
{
}


/**
 * Perform necessary target setup per application execution
 */
void EhsTVideoApp_reset(void)
{
}
