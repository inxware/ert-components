/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

/**
 * logic.h
 *
 * function prototypes for logicFunctions.c
 *
 * @author: inx limited
 *
 */
#ifndef EHS_LOGICFUNCTIONS_H
#define EHS_LOGICFUNCTIONS_H
#include "ehs_fb_types.h"


/******************************************************************************/
/* Declare not */

#define EHS_FB_NAME_LogicNot "LogicNot"
#define EHS_FB_ID_LogicNot 0xF99C

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(LogicNot);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(LogicNot);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(LogicNot);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(LogicNot)

/******************************************************************************/
/* Declare 2 input and */
#define EHS_FB_NAME_LogicTwoInputAnd "LogicTwoInputAnd"
#define EHS_FB_ID_LogicTwoInputAnd 0xA980

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(LogicTwoInputAnd);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(LogicTwoInputAnd);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(LogicTwoInputAnd);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(LogicTwoInputAnd)

/******************************************************************************/
/* Declare 3 input and */

#define EHS_FB_NAME_LogicThreeInputAnd "LogicThreeInputAnd"
#define EHS_FB_ID_LogicThreeInputAnd 0x3B4A

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(LogicThreeInputAnd);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(LogicThreeInputAnd);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(LogicThreeInputAnd);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(LogicThreeInputAnd)

/******************************************************************************/
/* Declare 4 input and */

#define EHS_FB_NAME_LogicFourInputAnd "LogicFourInputAnd"
#define EHS_FB_ID_LogicFourInputAnd 0x9851

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(LogicFourInputAnd);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(LogicFourInputAnd);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(LogicFourInputAnd);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(LogicFourInputAnd)

/******************************************************************************/
/* Declare 2 input or */

#define EHS_FB_NAME_LogicTwoInputOr "LogicTwoInputOr"
#define EHS_FB_ID_LogicTwoInputOr 0xE8FC

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(LogicTwoInputOr);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(LogicTwoInputOr);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(LogicTwoInputOr);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(LogicTwoInputOr)

/******************************************************************************/
/* Declare 3 input or */

#define EHS_FB_NAME_LogicThreeInputOr "LogicThreeInputOr"
#define EHS_FB_ID_LogicThreeInputOr 0x633B

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(LogicThreeInputOr);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(LogicThreeInputOr);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(LogicThreeInputOr);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(LogicThreeInputOr)

/******************************************************************************/
/* Declare 4 input or */

#define EHS_FB_NAME_LogicFourInputOr "LogicFourInputOr"
#define EHS_FB_ID_LogicFourInputOr 0xB9BE

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(LogicFourInputOr);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(LogicFourInputOr);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(LogicFourInputOr);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(LogicFourInputOr)

/******************************************************************************/
/* Declare 2 input nor */

#define EHS_FB_NAME_LogicTwoInputNor "LogicTwoInputNor"
#define EHS_FB_ID_LogicTwoInputNor 0xF430

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(LogicTwoInputNor);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(LogicTwoInputNor);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(LogicTwoInputNor);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(LogicTwoInputNor)

/******************************************************************************/
/* Declare 3 input nor */
#define EHS_FB_NAME_LogicThreeInputNor "LogicThreeInputNor"
#define EHS_FB_ID_LogicThreeInputNor 0x66FA

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(LogicThreeInputNor);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(LogicThreeInputNor);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(LogicThreeInputNor);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(LogicThreeInputNor)

/******************************************************************************/
/* Declare 4 input nor */

#define EHS_FB_NAME_LogicFourInputNor "LogicFourInputNor"
#define EHS_FB_ID_LogicFourInputNor 0xC5E1

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(LogicFourInputNor);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(LogicFourInputNor);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(LogicFourInputNor);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(LogicFourInputNor)

/******************************************************************************/
/* Declare 2 input nand */

#define EHS_FB_NAME_LogicTwoInputNand "LogicTwoInputNand"
#define EHS_FB_ID_LogicTwoInputNand 0xFC9C

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(LogicTwoInputNand);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(LogicTwoInputNand);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(LogicTwoInputNand);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(LogicTwoInputNand)

/******************************************************************************/
/* Declare 3 input nand */
#define EHS_FB_NAME_LogicThreeInputNand "LogicThreeInputNand"
#define EHS_FB_ID_LogicThreeInputNand 0xAB8E

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(LogicThreeInputNand);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(LogicThreeInputNand);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(LogicThreeInputNand);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(LogicThreeInputNand)

/******************************************************************************/
/* Declare 4 input nand */

#define EHS_FB_NAME_LogicFourInputNand "LogicFourInputNand"
#define EHS_FB_ID_LogicFourInputNand 0xA06D

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(LogicFourInputNand);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(LogicFourInputNand);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(LogicFourInputNand);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(LogicFourInputNand)

/******************************************************************************/
/* Declare 2 input xor */

#define EHS_FB_NAME_LogicXor "LogicXor"
#define EHS_FB_ID_LogicXor 0x3FFD

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(LogicXor);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(LogicXor);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(LogicXor);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(LogicXor)

#endif /* EHS_LOGICFUNCTIONS_H */
