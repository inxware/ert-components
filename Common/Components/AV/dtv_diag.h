/** 
 * @file .h
 * This declares the Diagnostic functions for the DTV toolkit. See
 * target-specific components directory for the implementation of this
 *
 * @author: inx limited
 * @version: $Revision: 1253 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 * 
 * Copyright (c) inx limited, 2007. All rights reserved.
 */

#ifndef _EHS_DTV_DIAG_H
#define _EHS_DTV_DIAG_H

#include "ehs_fb_types.h"

#define EHS_FB_DTV_DIAGS_NAME "DtvDiagnosticString"
#define EHS_FB_DTV_DIAGS_RUN_NAME "run"

/* declare functions provided by this function block */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(DtvDiagStr);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(DtvDiagStr);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DtvDiagStr);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(DtvDiagStr)

#endif /* _EHS_DTV_DIAG_H */

