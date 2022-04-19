/** @file targetos_init.c
 * Initialization/reset/termination code for the OS
 *
 * @author: inx limited
 * @version: $Revision: 1242 $
 * @date: $Date$
 * 
 * Copyright (c) inx limited, 2008. All rights reserved.
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
#include "targetgfx_init.h"
#include "blockref_table.h"
#include "hal_viewport.h"

/*****************************************************************************/
/* Declare macros and local typedefs used by this file */

/*****************************************************************************/
/* Declare prototypes of local functions */

/*****************************************************************************/
/* Variables defined with file-scope */

/*****************************************************************************/
/* Variables defined with global-scope */

extern EhsBlockRefType EhsBlockRefTable_Dtv[];


/*****************************************************************************/
/* Function definitions */

/** 
 * Perform necessary Operating system setup upon system initialisation
 */
void EhsTGfxSys_init(void)
{
#ifdef EHS_GUI_SUPPORT
	EhsTV_init(&EhsTV);
#endif /* EHS_GUI_SUPPORT */
}

/**
 * Called to shutdown the target prior to shutting down EHS
 */
void EhsTGfxSys_term(void)
{
}

/**
 * Perform setup before loading in a new application
 */
void EhsTGfxApp_init(void)
{
}

/**
 * Called to clean up after the execution of an application in order to make
 * sure everything has been stopped.
 */
void EhsTGfxApp_term(void)
{
}


/**
 * Perform necessary target setup per application execution
 */
void EhsTGfxApp_reset(void)
{
}
