/** @file dtv_diag.c
 * Contains the definition of the DTV diagnostic function
 * 
 * @author: inx limited
 * @version: $Revision: 1238 $
 * @date: $Date: 2006-10-30 05:05:44 +0000 (Mon, 30 Oct 2006), $
 * 
 * Copyright (c), inx limited, 2007. All rights reserved.
 */

#include "target.h"

#include "dtv_diag.h"
#include "globals.h"
#include "setCompletes.h"

EHS_FB_FUNCTIONS_START(DtvDiagStr)
EHS_FB_FUNCTION_ENTRY(EHS_FB_DTV_DIAGS_RUN_NAME, DtvDiagStr)
EHS_FB_FUNCTIONS_END

/**
 * Define the identify function. 
 */
EHS_FB_IDENTIFY_FUNCTION(DtvDiagStr)
{
}

/**
 * Initialise play back (nothing to do here).
 */
EHS_FB_INIT_FUNCTION(DtvDiagStr)
{
	return EHS_TRUE; /* initialise always succeeds */
}



/**
 * Play a specified programme
 */
EHS_FB_RUN_FUNCTION(DtvDiagStr)
{
	ehs_FILE* fPtr = Ehs_AppFopen("DTVDIAG.TXT","a");
	if (fPtr)
	{
		EhsFprintf(fPtr,"%s\n",NCAPSA_szIn(1));
		EhsFclose(fPtr);
	}
	return;
}

