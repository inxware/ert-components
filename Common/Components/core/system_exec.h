/**
 * @file system_exec.h
 *
 * function prototypes for system_exec.c (general purpose executable block)
 *
 * @author: Sam Gardner
 * @version: $Revision: 1248 $
 * @date: $Date: $
 * 
 * Copyright (c) inx limited, 2008. All rights reserved.
 */

#ifndef _EHS_SYSTEM_EXEC_H
#define _EHS_SYSTEM_EXEC_H

/******************************************************************************/
/* Declare State */
#define EHS_FB_EXEC_NAME "Exec"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(Exec);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(Exec);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(Exec);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(Exec)

#endif /* EHS_FB_CALC_INT_H */


