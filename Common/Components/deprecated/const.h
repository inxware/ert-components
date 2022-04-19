/**
 * const.h
 *
 * function prototypes for the constFunctions.c - Constant blocks of all types
 *
 * @author: Dr A Coombes
 * @version: $Revision: 333 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 * 
 * Copyright (c) inx limited, 2007. All rights reserved.
 */

#ifndef EHS_CONST_H
#define EHS_CONST_H
#include "ehs_fb_types.h"

/******************************************************************************/
/* Declare integer constant */
#define EHS_FB_CONSTANT_INT_NAME ("Constant_Int")
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ConstantInt);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ConstantInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ConstantInt);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ConstantInt)

/******************************************************************************/
/* Declare string constant */
#define EHS_FB_CONSTANT_STRING_NAME "Constant_String"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ConstantString);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ConstantString);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ConstantString);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ConstantString)


/******************************************************************************/
/* Declare boolean constant */
#define EHS_FB_CONSTANT_BOOL_NAME ("Constant_Bool")
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ConstantBool);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ConstantBool);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ConstantBool);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ConstantBool)


#ifdef EHS_TARGET_FP_SUPPORT
/******************************************************************************/
/* Declare float constant */
#define EHS_FB_CONSTANT_FLOAT_NAME ("Constant_Real")
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ConstantFloat);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ConstantFloat);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ConstantFloat);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ConstantFloat)

#endif /* EHS_TARGET_FP_SUPPORT */

#endif
