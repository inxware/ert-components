/**
 * logic.h
 *
 * function prototypes for logicFunctions.c
 *
 * @author: MDD
 * @version: $Revision: 971 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 * 
 * Copyright (c) inx limited, 2007. All rights reserved.
 */
#ifndef EHS_LOGICFUNCTIONS_H
#define EHS_LOGICFUNCTIONS_H
#include "ehs_fb_types.h"


/******************************************************************************/
/* Declare not */
#define EHS_FB_LOGIC_NOT_NAME "LogicNot"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(LogicNot);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(LogicNot);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(LogicNot);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(LogicNot)

/******************************************************************************/
/* Declare 2 input and */
#define EHS_FB_LOGIC_2INPUT_AND_NAME "LogicTwoInputAnd"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(LogicTwoInputAnd);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(LogicTwoInputAnd);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(LogicTwoInputAnd);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(LogicTwoInputAnd)

/******************************************************************************/
/* Declare 3 input and */
#define EHS_FB_LOGIC_3INPUT_AND_NAME "LogicThreeInputAnd"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(LogicThreeInputAnd);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(LogicThreeInputAnd);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(LogicThreeInputAnd);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(LogicThreeInputAnd)

/******************************************************************************/
/* Declare 4 input and */
#define EHS_FB_LOGIC_4INPUT_AND_NAME "LogicFourInputAnd"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(LogicFourInputAnd);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(LogicFourInputAnd);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(LogicFourInputAnd);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(LogicFourInputAnd)

/******************************************************************************/
/* Declare 2 input or */
#define EHS_FB_LOGIC_2INPUT_OR_NAME "LogicTwoInputOr"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(LogicTwoInputOr);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(LogicTwoInputOr);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(LogicTwoInputOr);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(LogicTwoInputOr)

/******************************************************************************/
/* Declare 3 input or */
#define EHS_FB_LOGIC_3INPUT_OR_NAME "LogicThreeInputOr"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(LogicThreeInputOr);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(LogicThreeInputOr);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(LogicThreeInputOr);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(LogicThreeInputOr)

/******************************************************************************/
/* Declare 4 input or */
#define EHS_FB_LOGIC_4INPUT_OR_NAME "LogicFourInputOr"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(LogicFourInputOr);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(LogicFourInputOr);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(LogicFourInputOr);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(LogicFourInputOr)

/******************************************************************************/
/* Declare 2 input nor */
#define EHS_FB_LOGIC_2INPUT_NOR_NAME "LogicTwoInputNor"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(LogicTwoInputNor);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(LogicTwoInputNor);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(LogicTwoInputNor);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(LogicTwoInputNor)

/******************************************************************************/
/* Declare 3 input nor */
#define EHS_FB_LOGIC_3INPUT_NOR_NAME "LogicThreeInputNor"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(LogicThreeInputNor);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(LogicThreeInputNor);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(LogicThreeInputNor);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(LogicThreeInputNor)

/******************************************************************************/
/* Declare 4 input nor */
#define EHS_FB_LOGIC_4INPUT_NOR_NAME "LogicFourInputNor"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(LogicFourInputNor);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(LogicFourInputNor);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(LogicFourInputNor);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(LogicFourInputNor)

/******************************************************************************/
/* Declare 2 input nand */
#define EHS_FB_LOGIC_2INPUT_NAND_NAME "LogicTwoInputNand"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(LogicTwoInputNand);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(LogicTwoInputNand);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(LogicTwoInputNand);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(LogicTwoInputNand)

/******************************************************************************/
/* Declare 3 input nand */
#define EHS_FB_LOGIC_3INPUT_NAND_NAME "LogicThreeInputNand"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(LogicThreeInputNand);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(LogicThreeInputNand);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(LogicThreeInputNand);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(LogicThreeInputNand)

/******************************************************************************/
/* Declare 4 input nand */
#define EHS_FB_LOGIC_4INPUT_NAND_NAME "LogicFourInputNand"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(LogicFourInputNand);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(LogicFourInputNand);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(LogicFourInputNand);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(LogicFourInputNand)

/******************************************************************************/
/* Declare 2 input xor */
#define EHS_FB_LOGIC_XOR_NAME "LogicXor"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(LogicXor);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(LogicXor);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(LogicXor);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(LogicXor)

#endif /* EHS_LOGICFUNCTIONS_H */
