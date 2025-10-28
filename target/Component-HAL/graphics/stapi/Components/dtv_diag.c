/** @file dtv_diag.c
 * Contains the definition of the DTV diagnostic function
 *
 * @author: inx limited
 *
 * Copyright (c), inx limited, 2007. All rights reserved.
 */


#include "globals.h"
#include "dtv_diag.h"
#include "fid.h"

EHS_FB_FUNCTIONS_START(DtvDiagStr)
EHS_FB_FUNCTION_ENTRY("run", 0x01, DtvDiagStr)
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
}

