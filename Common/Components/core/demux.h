/**
 * demux.h
 *
 * function prototypes for demultiplexerFunctions.c - Demultiplexor blocks of all types
 *
 * @author: SDG
 * @version: $Revision: 1248 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 * 
 * Copyright (c) inx limited, 2007. All rights reserved.
 */

#ifndef EHS_DEMULTIPLEXER_FUNCTIONS_H
#define EHS_DEMULTIPLEXER_FUNCTIONS_H
#include "ehs_fb_types.h"

/******************************************************************************/
/* Declare demux 2 output int converter */
#define EHS_FB_DEMULTIPLEX_TWO_OUTPUT_INT_NAME "DemultiplexTwoOutputInt"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(DemultiplexTwoOutputInt);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(DemultiplexTwoOutputInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DemultiplexTwoOutputInt1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DemultiplexTwoOutputInt2);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(DemultiplexTwoOutputInt)

/******************************************************************************/
/* Declare demux 3 output int converter */
#define EHS_FB_DEMULTIPLEX_THREE_OUTPUT_INT_NAME "DemultiplexThreeOutputInt"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(DemultiplexThreeOutputInt);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(DemultiplexThreeOutputInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DemultiplexThreeOutputInt1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DemultiplexThreeOutputInt2);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DemultiplexThreeOutputInt3);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(DemultiplexThreeOutputInt)

/******************************************************************************/
/* Declare demux 4 output int converter */
#define EHS_FB_DEMULTIPLEX_FOUR_OUTPUT_INT_NAME "DemultiplexFourOutputInt"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(DemultiplexFourOutputInt);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(DemultiplexFourOutputInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DemultiplexFourOutputInt1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DemultiplexFourOutputInt2);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DemultiplexFourOutputInt3);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DemultiplexFourOutputInt4);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(DemultiplexFourOutputInt)

/******************************************************************************/
/* Declare demux 2 output bool converter */
#define EHS_FB_DEMULTIPLEX_TWO_OUTPUT_BOOL_NAME "DemultiplexTwoOutputBool"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(DemultiplexTwoOutputBool);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(DemultiplexTwoOutputBool);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DemultiplexTwoOutputBool1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DemultiplexTwoOutputBool2);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(DemultiplexTwoOutputBool)

/******************************************************************************/
/* Declare demux 3 output int converter */
#define EHS_FB_DEMULTIPLEX_THREE_OUTPUT_BOOL_NAME "DemultiplexThreeOutputBool"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(DemultiplexThreeOutputBool);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(DemultiplexThreeOutputBool);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DemultiplexThreeOutputBool1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DemultiplexThreeOutputBool2);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DemultiplexThreeOutputBool3);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(DemultiplexThreeOutputBool)

/******************************************************************************/
/* Declare demux 4 output int converter */
#define EHS_FB_DEMULTIPLEX_FOUR_OUTPUT_BOOL_NAME "DemultiplexFourOutputBool"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(DemultiplexFourOutputBool);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(DemultiplexFourOutputBool);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DemultiplexFourOutputBool1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DemultiplexFourOutputBool2);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DemultiplexFourOutputBool3);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DemultiplexFourOutputBool4);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(DemultiplexFourOutputBool)

/******************************************************************************/
/* Declare demux 2 output string converter */
#define EHS_FB_DEMULTIPLEX_TWO_OUTPUT_STRING_NAME "DemultiplexTwoOutputString"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(DemultiplexTwoOutputString);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(DemultiplexTwoOutputString);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DemultiplexTwoOutputString1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DemultiplexTwoOutputString2);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(DemultiplexTwoOutputString)

/******************************************************************************/
/* Declare demux 3 output string converter */
#define EHS_FB_DEMULTIPLEX_THREE_OUTPUT_STRING_NAME "DemultiplexThreeOutputString"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(DemultiplexThreeOutputString);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(DemultiplexThreeOutputString);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DemultiplexThreeOutputString1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DemultiplexThreeOutputString2);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DemultiplexThreeOutputString3);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(DemultiplexThreeOutputString)

/******************************************************************************/
/* Declare demux 4 output string converter */
#define EHS_FB_DEMULTIPLEX_FOUR_OUTPUT_STRING_NAME "DemultiplexFourOutputString"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(DemultiplexFourOutputString);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(DemultiplexFourOutputString);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DemultiplexFourOutputString1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DemultiplexFourOutputString2);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DemultiplexFourOutputString3);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DemultiplexFourOutputString4);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(DemultiplexFourOutputString)

/******************************************************************************/
/* Declare indexed demux event converter */
#define EHS_FB_INDEXED_DEMULTIPLEX_EVENT_NAME "IndexedDemultiplexer_Event"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(IndexedDemultiplexer);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(IndexedDemultiplexer);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(IndexedDemultiplexer_Event);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(IndexedDemultiplexer_Event)

/******************************************************************************/
/* Declare indexed demux event converter */
#define EHS_FB_TEXTCASE_DEMULTIPLEX_EVENT_NAME "TextCaseDemultiplexer_Event"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(TextCaseDemultiplexer_Event);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(TextCaseDemultiplexer_Event);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(TextCaseDemultiplexer_Event);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(TextCaseDemultiplexer_Event)

/******************************************************************************/
/* Declare indexed demux bool converter */
#define EHS_FB_INDEXED_DEMULTIPLEX_BOOL_NAME "IndexedDemultiplexer_Bool"
//EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(IndexedDemultiplexer_Bool);
//EHS_GLOBAL EHS_FB_INIT_FUNCTION(IndexedDemultiplexer_Bool);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(IndexedDemultiplexer_Bool);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(IndexedDemultiplexer_Bool)

/******************************************************************************/
/* Declare indexed demux float converter */
#define EHS_FB_INDEXED_DEMULTIPLEX_FLOAT_NAME "IndexedDemultiplexer_Float"
//EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(IndexedDemultiplexer_Float);
//EHS_GLOBAL EHS_FB_INIT_FUNCTION(IndexedDemultiplexer_Float);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(IndexedDemultiplexer_Float);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(IndexedDemultiplexer_Float)

/******************************************************************************/
/* Declare indexed demux int converter */
#define EHS_FB_INDEXED_DEMULTIPLEX_INT_NAME "IndexedDemultiplexer_Int"
//EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(IndexedDemultiplexer_Int);
//EHS_GLOBAL EHS_FB_INIT_FUNCTION(IndexedDemultiplexer_Int);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(IndexedDemultiplexer_Int);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(IndexedDemultiplexer_Int)

/******************************************************************************/
/* Declare indexed demux string converter */
#define EHS_FB_INDEXED_DEMULTIPLEX_STRING_NAME "IndexedDemultiplexer_String"
//EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(IndexedDemultiplexer_String);
//EHS_GLOBAL EHS_FB_INIT_FUNCTION(IndexedDemultiplexer_String);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(IndexedDemultiplexer_String);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(IndexedDemultiplexer_String)

#ifdef EHS_TARGET_FP_SUPPORT
/******************************************************************************/
/* Declare demux 2 output float converter */
#define EHS_FB_DEMULTIPLEX_TWO_OUTPUT_FLOAT_NAME "DemultiplexTwoOutputReal"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(DemultiplexTwoOutputFloat);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(DemultiplexTwoOutputFloat);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DemultiplexTwoOutputFloat1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DemultiplexTwoOutputFloat2);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(DemultiplexTwoOutputFloat)

/******************************************************************************/
/* Declare demux 3 output float converter */
#define EHS_FB_DEMULTIPLEX_THREE_OUTPUT_FLOAT_NAME "DemultiplexThreeOutputReal"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(DemultiplexThreeOutputFloat);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(DemultiplexThreeOutputFloat);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DemultiplexThreeOutputFloat1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DemultiplexThreeOutputFloat2);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DemultiplexThreeOutputFloat3);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(DemultiplexThreeOutputFloat)

/******************************************************************************/
/* Declare demux 4 output float converter */
#define EHS_FB_DEMULTIPLEX_FOUR_OUTPUT_FLOAT_NAME "DemultiplexFourOutputReal"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(DemultiplexFourOutputFloat);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(DemultiplexFourOutputFloat);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DemultiplexFourOutputFloat1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DemultiplexFourOutputFloat2);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DemultiplexFourOutputFloat3);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DemultiplexFourOutputFloat4);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(DemultiplexFourOutputFloat)

#endif /* EHS_TARGET_FP_SUPPORT */



#endif /* EHS_DEMULTIPLEXER_FUNCTIONS_H */
