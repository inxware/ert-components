/** @file targetos_init.c
 * Initialization/reset/termination code for the OS
 *
 * @author: inx limited
 * @version: $Revision: 3946 $
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
#include "messages.h"

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
 * Perform necessary Operating system setup upon system initialisation, early call - too early to call EhsTV_init
 */
void EhsTGfxSys_init()
{
	EHSH_LOG_INFO("early GFX init");
	if (0) {
		if (!EhsTV_init(&EhsTV))
		{
			printf("Target viewport initialisation failed\n");
			exit(1);
		}
	}
}

/**
 * Perform necessary Operating system setup upon system initialisation, late call - ok to call EhsTV_init now
 * OS graphics handles data passed in here to be used by rendering functions
 */
void EhsTGfxSys_initLate(void * userdata)
{
	EHSH_LOG_INFO("late GFX init");
	struct EhsTVStruct* pViewport = &EhsTV;
	pViewport->engine = userdata;
	if (!EhsTV_init(&EhsTV))
	{
		EHSH_LOG_ERROR("Target viewport initialisation failed");
		exit(1);
	}
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
	//EhsTV_reset(&EhsTV); //not generic - moved to common code
}


/**
 * Perform necessary target setup per application execution
 */
void EhsTGfxApp_reset(void)
{
}
