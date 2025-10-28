/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/**
 * @file system_exec.h
 *
 * function prototypes for system_exec.c (general purpose executable block)
 *
 * @author: inx limited
 *
 */

#ifndef _EHS_SYSTEM_EXEC_H
#define _EHS_SYSTEM_EXEC_H

/******************************************************************************/
/* Declare State */

#define EHS_FB_NAME_Exec "Exec"
#define EHS_FB_ID_Exec 0x6CFE

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(Exec);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(Exec);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(Exec);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(Exec)

#endif /* EHS_FB_CALC_INT_H */


