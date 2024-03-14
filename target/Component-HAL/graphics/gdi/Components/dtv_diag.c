/** @file dtv_diag.c
 * Contains the definition of the DTV diagnostic function
 *
 * @author: inx limited
 *
 * Copyright (c), inx limited, 2007. All rights reserved.
 */

#include "target.h"

#include "dtv_diag.h"
#include "globals.h"
#include "setCompletes.h"

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

