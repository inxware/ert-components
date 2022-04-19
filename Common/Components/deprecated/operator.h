/**
 * operator.h
 *
 * function prototypes for operatorFunctions.c
 *
 * @author: SDG
 * @version: $Revision: 1248 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 * 
 * Copyright (c) inx limited, 2007. All rights reserved.
 */
#ifndef EHS_OPERATOR_FUNCTIONS_H
#define EHS_OPERATOR_FUNCTIONS_H

/******************************************************************************/
/* Declare integer > */
#define EHS_COMPARATOR_GREATER_INT_NAME "ComparatorGreaterInt"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ComparatorGreaterInt);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ComparatorGreaterInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ComparatorGreaterInt);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ComparatorGreaterInt)

/******************************************************************************/
/* Declare integer >= */
#define EHS_COMPARATOR_GREATER_EQUAL_INT_NAME "ComparatorGreaterEqualInt"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ComparatorGreaterEqualInt);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ComparatorGreaterEqualInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ComparatorGreaterEqualInt);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ComparatorGreaterEqualInt)

/******************************************************************************/
/* Declare integer = */
#define EHS_COMPARATOR_EQUAL_INT_NAME "ComparatorEqualInt"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ComparatorEqualInt);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ComparatorEqualInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ComparatorEqualInt);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ComparatorEqualInt)

/******************************************************************************/
/* Declare float > */
#define EHS_COMPARATOR_GREATER_FLOAT_NAME "ComparatorGreaterReal"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ComparatorGreaterReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ComparatorGreaterReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ComparatorGreaterReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ComparatorGreaterReal)

/******************************************************************************/
/* Declare float >= */
#define EHS_COMPARATOR_GREATER_EQUAL_FLOAT_NAME "ComparatorGreaterEqualReal"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ComparatorGreaterEqualReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ComparatorGreaterEqualReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ComparatorGreaterEqualReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ComparatorGreaterEqualReal)

/******************************************************************************/
/* Declare flaot = */
#define EHS_COMPARATOR_EQUAL_FLOAT_NAME "ComparatorEqualReal"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ComparatorEqualReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ComparatorEqualReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ComparatorEqualReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ComparatorEqualReal)


#endif /* EHS_OPERATOR_FUNCTIONS_H */
