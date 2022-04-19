/** @file dtv_diag.c
 * Contains the definition of the DTV diagnostic function
 * 
 * @author: inx limited
 * @version: $Revision: 2189 $
 * @date: $Date: 2006-10-30 05:05:44 +0000 (Mon, 30 Oct 2006), $
 * 
 * Copyright (c), inx limited, 2007. All rights reserved.
 */
//#@todo this file must be made a common code function

#include "target.h"

#include "dtv_diag.h"
#include "globals.h"
#include "fid.h"

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
}



/**
 * Play a specified programme
 */
EHS_FB_RUN_FUNCTION(DtvDiagStr)
{
	printf("%s\n",EHS_FB_OUT_S(1));
}

