/**
 * mux.h
 *
 * function prototypes for multiplexerFunctions.c
 *
 * @author: SDG
 * @version: $Revision: 4502 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 * 
 * Copyright (c) inx limited, 2007. All rights reserved.
 */
#ifndef EHS_MULTIPLEXER_FUNCTIONS_H
#define EHS_MULTIPLEXER_FUNCTIONS_H
#include "target_config.h"
#include "ehs_fb_types.h"

/******************************************************************************/
/* Declare 2-input integer multiplexor */
#define EHS_FB_MULTIPLEX_TWO_INPUT_INT_NAME "MultiplexTwoInputInt"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(MultiplexTwoInputInt);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(MultiplexTwoInputInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplexTwoInputInt1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplexTwoInputInt2);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(MultiplexTwoInputInt)

/******************************************************************************/
/* Declare 3-input integer multiplexor */
#define EHS_FB_MULTIPLEX_THREE_INPUT_INT_NAME "MultiplexThreeInputInt"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(MultiplexThreeInputInt);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(MultiplexThreeInputInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplexThreeInputInt1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplexThreeInputInt2);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplexThreeInputInt3);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(MultiplexThreeInputInt)

/******************************************************************************/
/* Declare 4-input integer multiplexor */
#define EHS_FB_MULTIPLEX_FOUR_INPUT_INT_NAME "MultiplexFourInputInt"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(MultiplexFourInputInt);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(MultiplexFourInputInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplexFourInputInt1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplexFourInputInt2);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplexFourInputInt3);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplexFourInputInt4);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(MultiplexFourInputInt)

/******************************************************************************/
/* Declare 2-input boolean multiplexor */
#define EHS_FB_MULTIPLEX_TWO_INPUT_BOOL_NAME "MultiplexTwoInputBool"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(MultiplexTwoInputBool);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(MultiplexTwoInputBool);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplexTwoInputBool1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplexTwoInputBool2);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(MultiplexTwoInputBool)

/******************************************************************************/
/* Declare 3-input boolean multiplexor */
#define EHS_FB_MULTIPLEX_THREE_INPUT_BOOL_NAME "MultiplexThreeInputBool"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(MultiplexThreeInputBool);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(MultiplexThreeInputBool);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplexThreeInputBool1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplexThreeInputBool2);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplexThreeInputBool3);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(MultiplexThreeInputBool)

/******************************************************************************/
/* Declare 4-input boolean multiplexor */
#define EHS_FB_MULTIPLEX_FOUR_INPUT_BOOL_NAME "MultiplexFourInputBool"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(MultiplexFourInputBool);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(MultiplexFourInputBool);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplexFourInputBool1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplexFourInputBool2);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplexFourInputBool3);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplexFourInputBool4);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(MultiplexFourInputBool)

/******************************************************************************/
/* Declare 2-input string multiplexor */
#define EHS_FB_MULTIPLEX_TWO_INPUT_STRING_NAME "MultiplexTwoInputString"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(MultiplexTwoInputString);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(MultiplexTwoInputString);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplexTwoInputString1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplexTwoInputString2);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(MultiplexTwoInputString)

/******************************************************************************/
/* Declare 3-input string multiplexor */
#define EHS_FB_MULTIPLEX_THREE_INPUT_STRING_NAME "MultiplexThreeInputString"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(MultiplexThreeInputString);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(MultiplexThreeInputString);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplexThreeInputString1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplexThreeInputString2);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplexThreeInputString3);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(MultiplexThreeInputString)

/******************************************************************************/
/* Declare 4-input string multiplexor */
#define EHS_FB_MULTIPLEX_FOUR_INPUT_STRING_NAME "MultiplexFourInputString"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(MultiplexFourInputString);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(MultiplexFourInputString);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplexFourInputString1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplexFourInputString2);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplexFourInputString3);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplexFourInputString4);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(MultiplexFourInputString)

#ifdef EHS_TARGET_FP_SUPPORT
/******************************************************************************/
/* Declare 2-input boolean multiplexor */
#define EHS_FB_MULTIPLEX_TWO_INPUT_FLOAT_NAME "MultiplexTwoInputReal"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(MultiplexTwoInputFloat);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(MultiplexTwoInputFloat);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplexTwoInputFloat1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplexTwoInputFloat2);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(MultiplexTwoInputFloat)

/******************************************************************************/
/* Declare 3-input boolean multiplexor */
#define EHS_FB_MULTIPLEX_THREE_INPUT_FLOAT_NAME "MultiplexThreeInputReal"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(MultiplexThreeInputFloat);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(MultiplexThreeInputFloat);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplexThreeInputFloat1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplexThreeInputFloat2);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplexThreeInputFloat3);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(MultiplexThreeInputFloat)

/******************************************************************************/
/* Declare 4-input boolean multiplexor */
#define EHS_FB_MULTIPLEX_FOUR_INPUT_FLOAT_NAME "MultiplexFourInputReal"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(MultiplexFourInputFloat);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(MultiplexFourInputFloat);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplexFourInputFloat1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplexFourInputFloat2);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplexFourInputFloat3);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplexFourInputFloat4);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(MultiplexFourInputFloat)

/******************************************************************************/
/* Declare IndexedMultiplexer_Event */
#define EHS_FB_INDEXED_MULTIPLEXER_EVENT_NAME "IndexedMultiplexer_Event"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(IndexedMultiplexer_Event);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(IndexedMultiplexer_Event);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(IndexedMultiplexer_Event_1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(IndexedMultiplexer_Event_2);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(IndexedMultiplexer_Event_3);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(IndexedMultiplexer_Event_4);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(IndexedMultiplexer_Event_5);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(IndexedMultiplexer_Event_6);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(IndexedMultiplexer_Event_7);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(IndexedMultiplexer_Event_8);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(IndexedMultiplexer_Event_Reset);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(IndexedMultiplexer_Event)

#endif /* EHS_TARGET_FP_SUPPORT */

#endif /* EHS_MULTIPLEXER_FUNCTIONS_H */
