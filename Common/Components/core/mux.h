/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

/**
 * mux.h
 *
 * function prototypes for multiplexerFunctions.c
 *
 * @author: inx limited
 *
 */
#ifndef EHS_MULTIPLEXER_FUNCTIONS_H
#define EHS_MULTIPLEXER_FUNCTIONS_H
#include "target_config.h"
#include "ehs_fb_types.h"

/******************************************************************************/
/* Declare 2-input integer multiplexor */

#define EHS_FB_NAME_MultiplexTwoInputInt "MultiplexTwoInputInt"
#define EHS_FB_ID_MultiplexTwoInputInt 0x84B5

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(MultiplexTwoInputInt);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(MultiplexTwoInputInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplexTwoInputInt1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplexTwoInputInt2);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(MultiplexTwoInputInt)

/******************************************************************************/
/* Declare 3-input integer multiplexor */
#define EHS_FB_NAME_MultiplexThreeInputInt "MultiplexThreeInputInt"
#define EHS_FB_ID_MultiplexThreeInputInt 0x7CFD

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(MultiplexThreeInputInt);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(MultiplexThreeInputInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplexThreeInputInt1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplexThreeInputInt2);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplexThreeInputInt3);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(MultiplexThreeInputInt)

/******************************************************************************/
/* Declare 4-input integer multiplexor */

#define EHS_FB_NAME_MultiplexFourInputInt "MultiplexFourInputInt"
#define EHS_FB_ID_MultiplexFourInputInt 0x2133

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

#define EHS_FB_NAME_MultiplexTwoInputBool "MultiplexTwoInputBool"
#define EHS_FB_ID_MultiplexTwoInputBool 0x8E1C

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(MultiplexTwoInputBool);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(MultiplexTwoInputBool);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplexTwoInputBool1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplexTwoInputBool2);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(MultiplexTwoInputBool)

/******************************************************************************/
/* Declare 3-input boolean multiplexor */
#define EHS_FB_NAME_MultiplexThreeInputBool "MultiplexThreeInputBool"
#define EHS_FB_ID_MultiplexThreeInputBool 0xB8E4

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(MultiplexThreeInputBool);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(MultiplexThreeInputBool);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplexThreeInputBool1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplexThreeInputBool2);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplexThreeInputBool3);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(MultiplexThreeInputBool)

/******************************************************************************/
/* Declare 4-input boolean multiplexor */

#define EHS_FB_NAME_MultiplexFourInputBool "MultiplexFourInputBool"
#define EHS_FB_ID_MultiplexFourInputBool 0x2C38

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

#define EHS_FB_NAME_MultiplexTwoInputString "MultiplexTwoInputString"
#define EHS_FB_ID_MultiplexTwoInputString 0x0094

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(MultiplexTwoInputString);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(MultiplexTwoInputString);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplexTwoInputString1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplexTwoInputString2);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(MultiplexTwoInputString)

/******************************************************************************/
/* Declare 3-input string multiplexor */

#define EHS_FB_NAME_MultiplexThreeInputString "MultiplexThreeInputString"
#define EHS_FB_ID_MultiplexThreeInputString 0xD657

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(MultiplexThreeInputString);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(MultiplexThreeInputString);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplexThreeInputString1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplexThreeInputString2);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplexThreeInputString3);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(MultiplexThreeInputString)

/******************************************************************************/
/* Declare 4-input string multiplexor */

#define EHS_FB_NAME_MultiplexFourInputString "MultiplexFourInputString"
#define EHS_FB_ID_MultiplexFourInputString 0xB90E

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
#define EHS_FB_NAME_MULTIPLEX_TWO_INPUT_FLOAT_NAME "MultiplexTwoInputReal"
#define EHS_FB_ID_MULTIPLEX_TWO_INPUT_FLOAT_NAME 0x2C3C

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(MultiplexTwoInputFloat);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(MultiplexTwoInputFloat);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplexTwoInputFloat1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplexTwoInputFloat2);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(MultiplexTwoInputFloat)

/******************************************************************************/
/* Declare 3-input boolean multiplexor */
#define EHS_FB_NAME_MultiplexThreeInputReal "MultiplexThreeInputReal"
#define EHS_FB_ID_MultiplexThreeInputReal 0x1AC4

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(MultiplexThreeInputFloat);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(MultiplexThreeInputFloat);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplexThreeInputFloat1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplexThreeInputFloat2);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplexThreeInputFloat3);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(MultiplexThreeInputFloat)

/******************************************************************************/
/* Declare 4-input boolean multiplexor */

#define EHS_FB_NAME_MultiplexFourInputReal "MultiplexFourInputReal"
#define EHS_FB_ID_MultiplexFourInputReal 0x8E18

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
#define EHS_FB_NAME_IndexedMultiplexer_Event "IndexedMultiplexer_Event"
#define EHS_FB_ID_IndexedMultiplexer_Event 0x3B77

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
