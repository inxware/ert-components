/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/**
 * @file .h
 * This declares the Diagnostic functions for the DTV toolkit. See
 * target-specific components directory for the implementation of this
 *
 * @author: inx limited
 */

#ifndef _EHS_DTV_DIAG_H
#define _EHS_DTV_DIAG_H

#include "ehs_fb_types.h"

#define EHS_FB_NAME_DTV_DIAGS_NAME "DtvDiagnosticString"
#define EHS_FB_ID_DTV_DIAGS_NAME 0x9284




/* declare functions provided by this function block */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(DtvDiagStr);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(DtvDiagStr);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DtvDiagStr);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(DtvDiagStr)

#endif /* _EHS_DTV_DIAG_H */

