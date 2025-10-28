/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/**
 * demux.h
 *
 * function prototypes for demultiplexerFunctions.c - Demultiplexor blocks of all types
 *
 * @author: inx limited
 *
 */

#ifndef EHS_DEMULTIPLEXER_FUNCTIONS_H
#define EHS_DEMULTIPLEXER_FUNCTIONS_H
#include "ehs_fb_types.h"

/******************************************************************************/
/* Declare demux 2 output int converter */

#define EHS_FB_NAME_DemultiplexTwoOutputInt "DemultiplexTwoOutputInt"
#define EHS_FB_ID_DemultiplexTwoOutputInt 0xAE32

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(DemultiplexTwoOutputInt);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(DemultiplexTwoOutputInt);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexTwoOutputInt1);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexTwoOutputInt2);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(DemultiplexTwoOutputInt)

/******************************************************************************/
/* Declare demux 3 output int converter */
#define EHS_FB_NAME_DemultiplexThreeOutputInt "DemultiplexThreeOutputInt"
#define EHS_FB_ID_DemultiplexThreeOutputInt 0x3E05

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(DemultiplexThreeOutputInt);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(DemultiplexThreeOutputInt);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexThreeOutputInt1);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexThreeOutputInt2);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexThreeOutputInt3);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(DemultiplexThreeOutputInt)

/******************************************************************************/
/* Declare demux 4 output int converter */
#define EHS_FB_NAME_DemultiplexFourOutputInt "DemultiplexFourOutputInt"
#define EHS_FB_ID_DemultiplexFourOutputInt 0xE2F7

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(DemultiplexFourOutputInt);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(DemultiplexFourOutputInt);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexFourOutputInt1);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexFourOutputInt2);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexFourOutputInt3);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexFourOutputInt4);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(DemultiplexFourOutputInt)

/******************************************************************************/
/* Declare demux 8 output int converter */
#define EHS_FB_NAME_DemultiplexEightOutputInt "DemultiplexEightOutputInt"
#define EHS_FB_ID_DemultiplexEightOutputInt 0xA1B0

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(DemultiplexEightOutputInt);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(DemultiplexEightOutputInt);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexEightOutputInt1);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexEightOutputInt2);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexEightOutputInt3);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexEightOutputInt4);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexEightOutputInt5);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexEightOutputInt6);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexEightOutputInt7);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexEightOutputInt8);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(DemultiplexEightOutputInt)

/******************************************************************************/
/* Declare demux 2 output bool converter */
#define EHS_FB_NAME_DemultiplexTwoOutputBool "DemultiplexTwoOutputBool"
#define EHS_FB_ID_DemultiplexTwoOutputBool 0xEC76

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(DemultiplexTwoOutputBool);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(DemultiplexTwoOutputBool);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexTwoOutputBool1);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexTwoOutputBool2);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(DemultiplexTwoOutputBool)

/******************************************************************************/
/* Declare demux 3 output int converter */
#define EHS_FB_NAME_DemultiplexThreeOutputBool "DemultiplexThreeOutputBool"
#define EHS_FB_ID_DemultiplexThreeOutputBool 0x3AA7

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(DemultiplexThreeOutputBool);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(DemultiplexThreeOutputBool);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexThreeOutputBool1);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexThreeOutputBool2);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexThreeOutputBool3);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(DemultiplexThreeOutputBool)

/******************************************************************************/
/* Declare demux 4 output int converter */
#define EHS_FB_NAME_DemultiplexFourOutputBool "DemultiplexFourOutputBool"
#define EHS_FB_ID_DemultiplexFourOutputBool 0xBFFA

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(DemultiplexFourOutputBool);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(DemultiplexFourOutputBool);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexFourOutputBool1);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexFourOutputBool2);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexFourOutputBool3);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexFourOutputBool4);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(DemultiplexFourOutputBool)

/******************************************************************************/
/* Declare demux 8 output int converter */
#define EHS_FB_NAME_DemultiplexEightOutputBool "DemultiplexEightOutputBool"
#define EHS_FB_ID_DemultiplexEightOutputBool 0x8DF9

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(DemultiplexEightOutputBool);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(DemultiplexEightOutputBool);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexEightOutputBool1);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexEightOutputBool2);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexEightOutputBool3);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexEightOutputBool4);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexEightOutputBool5);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexEightOutputBool6);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexEightOutputBool7);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexEightOutputBool8);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(DemultiplexEightOutputBool)

/******************************************************************************/
/* Declare demux 2 output string converter */
#define EHS_FB_NAME_DemultiplexTwoOutputString "DemultiplexTwoOutputString"
#define EHS_FB_ID_DemultiplexTwoOutputString 0x493B

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(DemultiplexTwoOutputString);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(DemultiplexTwoOutputString);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexTwoOutputString1);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexTwoOutputString2);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(DemultiplexTwoOutputString)

/******************************************************************************/
/* Declare demux 3 output string converter */

#define EHS_FB_NAME_DemultiplexThreeOutputString "DemultiplexThreeOutputString"
#define EHS_FB_ID_DemultiplexThreeOutputString 0x87E6

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(DemultiplexThreeOutputString);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(DemultiplexThreeOutputString);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexThreeOutputString1);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexThreeOutputString2);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexThreeOutputString3);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(DemultiplexThreeOutputString)

/******************************************************************************/
/* Declare demux 4 output string converter */
#define EHS_FB_NAME_DemultiplexFourOutputString "DemultiplexFourOutputString"
#define EHS_FB_ID_DemultiplexFourOutputString 0xB41F

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(DemultiplexFourOutputString);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(DemultiplexFourOutputString);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexFourOutputString1);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexFourOutputString2);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexFourOutputString3);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexFourOutputString4);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(DemultiplexFourOutputString)

/******************************************************************************/
/* Declare demux 8 output string converter */
#define EHS_FB_NAME_DemultiplexEightOutputString "DemultiplexEightOutputString"
#define EHS_FB_ID_DemultiplexEightOutputString 0x919E

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(DemultiplexEightOutputString);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(DemultiplexEightOutputString);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexEightOutputString1);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexEightOutputString2);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexEightOutputString3);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexEightOutputString4);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexEightOutputString5);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexEightOutputString6);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexEightOutputString7);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexEightOutputString8);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(DemultiplexEightOutputString)

/******************************************************************************/
/* Declare indexed demux event converter */

#define EHS_FB_NAME_IndexedDemultiplexer_Event "IndexedDemultiplexer_Event"
#define EHS_FB_ID_IndexedDemultiplexer_Event 0x6E2A

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(IndexedDemultiplexer);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(IndexedDemultiplexer);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(IndexedDemultiplexer_Event);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(IndexedDemultiplexer_Event)

/******************************************************************************/
/* Declare indexed demux event converter */
#define EHS_FB_NAME_TextCaseDemultiplexer_Event "TextCaseDemultiplexer_Event"
#define EHS_FB_ID_TextCaseDemultiplexer_Event 0xCF87

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(TextCaseDemultiplexer_Event);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(TextCaseDemultiplexer_Event);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(TextCaseDemultiplexer_Event);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(TextCaseDemultiplexer_Event)

/******************************************************************************/
/* Declare indexed demux bool converter */
#define EHS_FB_NAME_IndexedDemultiplexer_Bool "IndexedDemultiplexer_Bool"
#define EHS_FB_ID_IndexedDemultiplexer_Bool 0x78B5

//EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(IndexedDemultiplexer_Bool);
//EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(IndexedDemultiplexer_Bool);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(IndexedDemultiplexer_Bool);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(IndexedDemultiplexer_Bool)

/******************************************************************************/
/* Declare indexed demux float converter */
#define EHS_FB_NAME_IndexedDemultiplexer_Float "IndexedDemultiplexer_Float"
#define EHS_FB_ID_IndexedDemultiplexer_Float 0x844C

//EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(IndexedDemultiplexer_Float);
//EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(IndexedDemultiplexer_Float);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(IndexedDemultiplexer_Float);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(IndexedDemultiplexer_Float)

/******************************************************************************/
/* Declare indexed demux int converter */
#define EHS_FB_NAME_IndexedDemultiplexer_Int "IndexedDemultiplexer_Int"
#define EHS_FB_ID_IndexedDemultiplexer_Int 0xECFC

//EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(IndexedDemultiplexer_Int);
//EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(IndexedDemultiplexer_Int);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(IndexedDemultiplexer_Int);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(IndexedDemultiplexer_Int)

/******************************************************************************/
/* Declare indexed demux string converter */
#define EHS_FB_NAME_IndexedDemultiplexer_String "IndexedDemultiplexer_String"
#define EHS_FB_ID_IndexedDemultiplexer_String 0x166A

//EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(IndexedDemultiplexer_String);
//EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(IndexedDemultiplexer_String);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(IndexedDemultiplexer_String);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(IndexedDemultiplexer_String)

#ifdef EHS_TARGET_FP_SUPPORT
/******************************************************************************/
/* Declare demux 2 output float converter */
#define EHS_FB_NAME_DemultiplexTwoOutputReal "DemultiplexTwoOutputReal"
#define EHS_FB_ID_DemultiplexTwoOutputReal 0x4E56

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(DemultiplexTwoOutputFloat);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(DemultiplexTwoOutputFloat);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexTwoOutputFloat1);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexTwoOutputFloat2);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(DemultiplexTwoOutputFloat)

/******************************************************************************/
/* Declare demux 3 output float converter */
#define EHS_FB_NAME_DemultiplexThreeOutputReal "DemultiplexThreeOutputReal"
#define EHS_FB_ID_DemultiplexThreeOutputReal 0x9887

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(DemultiplexThreeOutputFloat);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(DemultiplexThreeOutputFloat);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexThreeOutputFloat1);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexThreeOutputFloat2);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexThreeOutputFloat3);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(DemultiplexThreeOutputFloat)

/******************************************************************************/
/* Declare demux 4 output float converter */
#define EHS_FB_NAME_DemultiplexFourOutputReal "DemultiplexFourOutputReal"
#define EHS_FB_ID_DemultiplexFourOutputReal 0x1DDA

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(DemultiplexFourOutputFloat);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(DemultiplexFourOutputFloat);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexFourOutputFloat1);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexFourOutputFloat2);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexFourOutputFloat3);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexFourOutputFloat4);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(DemultiplexFourOutputFloat)

/******************************************************************************/
/* Declare demux 8 output float converter */
#define EHS_FB_NAME_DemultiplexEightOutputReal "DemultiplexEightOutputReal"
#define EHS_FB_ID_DemultiplexEightOutputReal 0x2FD9

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(DemultiplexEightOutputFloat);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(DemultiplexEightOutputFloat);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexEightOutputFloat1);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexEightOutputFloat2);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexEightOutputFloat3);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexEightOutputFloat4);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexEightOutputFloat5);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexEightOutputFloat6);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexEightOutputFloat7);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DemultiplexEightOutputFloat8);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(DemultiplexEightOutputFloat)

#endif /* EHS_TARGET_FP_SUPPORT */



#endif /* EHS_DEMULTIPLEXER_FUNCTIONS_H */
