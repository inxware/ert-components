/**
 * buffer.h
 *
 * function prototypes for the bufferFunctions.c - FIFO and LIFO buffers
 *
 * @author: SDG
 * @version: $Revision: 3561 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 * 
 * Copyright (c) inx limited, 2007. All rights reserved.
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
#define EHS_FB_FIFOREAL_NAME "FIFO_Real"
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
#define EHS_FB_LIFOREAL_NAME "LIFO_Real"
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
#define EHS_FB_FIFOINT_NAME "FIFO_Int"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(FIFOInt);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(FIFOInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(PushFIFOInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(PopFIFOInt);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(FIFOInt)

/******************************************************************************/
/* declare LIFO Integer function block */
#define EHS_FB_LIFOINT_NAME "LIFO_Int"
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
#define EHS_FB_FIFOBOOL_NAME "FIFO_Bool"
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
#define EHS_FB_LIFOBOOL_NAME "LIFO_Bool"
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
#define EHS_FB_FIFOSTRING_NAME "FIFO_String"
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

#define EHS_FB_LIFOSTRING_NAME "LIFO_String"
#define RUN_PUSH_LIFO_STRING "Run_PushLIFOString"
#define RUN_POP_LIFO_STRING "Run_PopLIFOString"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(LIFOString);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(LIFOString);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(PushLIFOString);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(PopLIFOString);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(LIFOString)


#endif

