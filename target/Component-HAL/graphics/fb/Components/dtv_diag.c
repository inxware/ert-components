/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/
/** @file dtv_diag.c
 * Contains the definition of the DTV diagnostic function
 *
 * @author: inx limited
 */

#include "target.h"

#include "dtv_diag.h"
#include "globals.h"
#include "fid.h"

EHS_FB_FUNCTIONS_START(DtvDiagStr)
EHS_FB_FUNCTION_ENTRY(EHS_FB_NAME_DTV_DIAGS_RUN_NAME, 0x01, DtvDiagStr)
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

