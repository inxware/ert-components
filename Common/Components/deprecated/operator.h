/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

/**
 * operator.h
 *
 * function prototypes for operatorFunctions.c
 *
 * @author: inx limited
 *
 */
#ifndef EHS_OPERATOR_FUNCTIONS_H
#define EHS_OPERATOR_FUNCTIONS_H

#include "globals.h"
#include "ehs_fb_types.h"

/******************************************************************************/
/* Declare integer > */
#define EHS_FB_NAME_ComparatorGreaterInt "ComparatorGreaterInt"
#define EHS_FB_ID_ComparatorGreaterInt 0xC60C

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ComparatorGreaterInt);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ComparatorGreaterInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ComparatorGreaterInt);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ComparatorGreaterInt)

/******************************************************************************/
/* Declare integer >= */
#define EHS_FB_NAME_ComparatorGreaterEqualInt "ComparatorGreaterEqualInt"
#define EHS_FB_ID_ComparatorGreaterEqualInt 0xC59E

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ComparatorGreaterEqualInt);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ComparatorGreaterEqualInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ComparatorGreaterEqualInt);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ComparatorGreaterEqualInt)

/******************************************************************************/
/* Declare integer = */
#define EHS_FB_NAME_ComparatorEqualInt "ComparatorEqualInt"
#define EHS_FB_ID_ComparatorEqualInt 0xCDC2

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ComparatorEqualInt);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ComparatorEqualInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ComparatorEqualInt);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ComparatorEqualInt)

/******************************************************************************/
/* Declare float > */
#define EHS_FB_NAME_CComparatorGreaterReal "ComparatorGreaterReal"
#define EHS_FB_ID_ComparatorGreaterReal 0x9EBF

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ComparatorGreaterReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ComparatorGreaterReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ComparatorGreaterReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ComparatorGreaterReal)

/******************************************************************************/
/* Declare float >= */
#define EHS_FB_NAME_ComparatorGreaterEqualReal "ComparatorGreaterEqualReal"
#define EHS_FB_ID_ComparatorGreaterEqualReal 0x333D

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ComparatorGreaterEqualReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ComparatorGreaterEqualReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ComparatorGreaterEqualReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ComparatorGreaterEqualReal)

/******************************************************************************/
/* Declare flaot = */
#define EHS_FB_NAME_ComparatorEqualReal "ComparatorEqualReal"
#define EHS_FB_ID_ComparatorEqualReal 0x0A35

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ComparatorEqualReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ComparatorEqualReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ComparatorEqualReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ComparatorEqualReal)


#endif /* EHS_OPERATOR_FUNCTIONS_H */
