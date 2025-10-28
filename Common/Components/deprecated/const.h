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
#define EHS_FB_NAME_Constant_Int ("Constant_Int")
#define EHS_FB_ID_Constant_Int (0x46F9)

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(ConstantInt);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(ConstantInt);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ConstantInt);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ConstantInt)

/******************************************************************************/
/* Declare string constant */
#define EHS_FB_NAME_Constant_String "Constant_String"
#define EHS_FB_ID_Constant_String 0xB704

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(ConstantString);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(ConstantString);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ConstantString);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ConstantString)


/******************************************************************************/
/* Declare boolean constant */
#define EHS_FB_NAME_Constant_Bool ("Constant_Bool")
#define EHS_FB_ID_Constant_Bool (0x7BDF)

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(ConstantBool);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(ConstantBool);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ConstantBool);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ConstantBool)


#ifdef EHS_TARGET_FP_SUPPORT
/******************************************************************************/
/* Declare float constant */
#define EHS_FB_NAME_Constant_Real ("Constant_Real")
#define EHS_FB_ID_Constant_Real (0xD9FF)

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(ConstantFloat);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(ConstantFloat);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ConstantFloat);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ConstantFloat)

#endif /* EHS_TARGET_FP_SUPPORT */

#endif
