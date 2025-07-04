/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/
/**
 * buffer.h
 *
 * function prototypes for the bufferFunctions.c - FIFO and LIFO buffers
 *
 * @author: inx limited
 *
 */
#ifndef _BUFFER_FUNCTIONS_H
#define _BUFFER_FUNCTIONS_H

#include "ehs_fb_types.h"


#define ERR_NO_ERROR 0
#define ERR_PUSH_ERROR 1
#define ERR_POP_ERROR 2

EHS_GLOBAL EHS_FB_RUN_FUNCTION(FlushAnyBuffer);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(FlushAnyLIFOBuffer);

#ifdef EHS_TARGET_FP_SUPPORT
/******************************************************************************/
/* declare FIFOReal Function block */

#define EHS_FB_NAME_FIFO_Real "FIFO_Real"
#define EHS_FB_ID_FIFO_Real 0xD54A

#define RUN_PUSH_FIFO_REAL "Run_PushFIFOReal"
#define RUN_POP_FIFO_REAL "Run_PopFIFOReal"

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(FIFOReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(FIFOReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(PushFIFOReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(PopFIFOReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(FIFOReal)

/******************************************************************************/
/* declare LIFOReal Function block */
#define EHS_FB_NAME_LIFO_Real "LIFO_Real"
#define EHS_FB_ID_LIFO_Real 0x7534

#define RUN_PUSH_LIFO_REAL "Run_PushLIFOReal"
#define RUN_POP_LIFO_REAL "Run_PopLIFOReal"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(LIFOReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(LIFOReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(PushLIFOReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(PopLIFOReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(LIFOReal)

#endif /* EHS_TARGET_FP_SUPPORT */

/******************************************************************************/
/* declare FIFO Integer function block */

#define EHS_FB_NAME_FIFO_Int "FIFO_Int"
#define EHS_FB_ID_FIFO_Int 0x33E8

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(FIFOInt);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(FIFOInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(PushFIFOInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(PopFIFOInt);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(FIFOInt)

/******************************************************************************/
/* declare LIFO Integer function block */

#define EHS_FB_NAME_LIFO_Int "LIFO_Int"
#define EHS_FB_ID_LIFO_Int 0x4C68

#define RUN_PUSH_LIFO_INT "Run_PushLIFOInt"
#define RUN_POP_LIFO_INT "Run_PopLIFOInt"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(LIFOInt);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(LIFOInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(PushLIFOInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(PopLIFOInt);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(LIFOInt)

/******************************************************************************/
/* declare FIFO Boolean function block */

#define EHS_FB_NAME_FIFO_Bool "FIFO_Bool"
#define EHS_FB_ID_FIFO_Bool 0x776A

#define RUN_PUSH_FIFO_BOOL "Run_PushFIFOBool"
#define RUN_POP_FIFO_BOOL "Run_PopFIFOBool"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(FIFOBool);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(FIFOBool);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(PushFIFOBool);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(PopFIFOBool);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(FIFOBool)

/******************************************************************************/
/* declare LIFO Boolean function block */

#define EHS_FB_NAME_LIFO_Bool "LIFO_Bool"
#define EHS_FB_ID_LIFO_Bool 0xD714

#define RUN_PUSH_LIFO_BOOL "Run_PushLIFOBool"
#define RUN_POP_LIFO_BOOL "Run_PopLIFOBool"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(LIFOBool);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(LIFOBool);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(PushLIFOBool);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(PopLIFOBool);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(LIFOBool)

/******************************************************************************/
/* declare FIFO String function block */

#define EHS_FB_NAME_FIFO_String "FIFO_String"
#define EHS_FB_ID_FIFO_String 0x2272

#define RUN_PUSH_FIFO_STRING "Run_PushFIFOString"
#define RUN_POP_FIFO_STRING "Run_PopFIFOString"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(FIFOString);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(FIFOString);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(PushFIFOString);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(PopFIFOString);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(FIFOString)

/******************************************************************************/
/* declare FIFO String function block */


#define EHS_FB_NAME_LIFO_String "LIFO_String"
#define EHS_FB_ID_LIFO_String 0xFA53

#define RUN_PUSH_LIFO_STRING "Run_PushLIFOString"
#define RUN_POP_LIFO_STRING "Run_PopLIFOString"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(LIFOString);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(LIFOString);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(PushLIFOString);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(PopLIFOString);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(LIFOString)


#endif

