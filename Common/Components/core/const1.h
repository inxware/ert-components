/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/**
 * const.h
 *
 * function prototypes for the constFunctions.c - Constant blocks of all types
 *
 * @author: inx limited
 *
 */

#ifndef EHS_CONST_H
#define EHS_CONST_H
#include "ehs_fb_types.h"

/******************************************************************************/
/* Declare integer constant */
#define EHS_FB_NAME_Constant_Int1 ("Constant_Int1")
#define EHS_FB_ID_Constant_Int1 (0x9647)
EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(ConstantInt1);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(ConstantInt1);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ConstantInt1);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ConstantInt1)

/******************************************************************************/
/* Declare string constant */
#define EHS_FB_NAME_Constant_String1 "Constant_String1"
#define EHS_FB_ID_Constant_String1 0x1777
EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(ConstantString1);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(ConstantString1);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ConstantString1);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ConstantString1)


/******************************************************************************/
/* Declare boolean constant */
#define EHS_FB_NAME_Constant_Bool1 ("Constant_Bool1")
#define EHS_FB_ID_Constant_Bool1 (0x4CFB)

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(ConstantBool1);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(ConstantBool1);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ConstantBool1);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ConstantBool1)


#ifdef EHS_TARGET_FP_SUPPORT
/******************************************************************************/
/* Declare float constant */
#define EHS_FB_NAME_Constant_Real1 ("Constant_Real1")
#define EHS_FB_ID_Constant_Real1 (0x9458)

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(ConstantFloat1);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(ConstantFloat1);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ConstantFloat1);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ConstantFloat1)

#endif /* EHS_TARGET_FP_SUPPORT */

#endif
