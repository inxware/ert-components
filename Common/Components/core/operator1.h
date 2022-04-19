/**
 * operator1.h
 *
 * function prototypes for operator1.c
 *
 * @author: SDG
 * @version: $Revision: 1248 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 * 
 * Copyright (c) inx limited, 2007. All rights reserved.
 */
#ifndef EHS_OPERATOR_FUNCTIONS1_H
#define EHS_OPERATOR_FUNCTIONS1_H

/******************************************************************************/
/* Declare 2-input integer addition */
#define EHS_FB_ADDITION_TWO_INPUT_INT_NAME "AdditionTwoInputInt"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(AdditionTwoInputInt);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(AdditionTwoInputInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(AdditionTwoInputInt);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(AdditionTwoInputInt)

/******************************************************************************/
/* Declare 3-input integer addition */
#define EHS_FB_ADDITION_THREE_INPUT_INT_NAME "AdditionThreeInputInt"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(AdditionThreeInputInt);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(AdditionThreeInputInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(AdditionThreeInputInt);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(AdditionThreeInputInt)

/******************************************************************************/
/* Declare 4-input integer addition */
#define EHS_FB_ADDITION_FOUR_INPUT_INT_NAME "AdditionFourInputInt"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(AdditionFourInputInt);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(AdditionFourInputInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(AdditionFourInputInt);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(AdditionFourInputInt)

/******************************************************************************/
/* Declare integer subtraction */
#define EHS_FB_SUBTRACTION_INT_NAME "SubtractionInt"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(SubtractionInt);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(SubtractionInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(SubtractionInt);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(SubtractionInt)

/******************************************************************************/
/* Declare 2 integer multiply */
#define EHS_FB_MULTIPLY_TWO_INPUT_INT_NAME "MultiplyTwoInputInt"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(MultiplyTwoInputInt);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(MultiplyTwoInputInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplyTwoInputInt);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(MultiplyTwoInputInt)

/******************************************************************************/
/* Declare 3 integer multiply */
#define EHS_FB_MULTIPLY_THREE_INPUT_INT_NAME "MultiplyThreeInputInt"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(MultiplyThreeInputInt);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(MultiplyThreeInputInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplyThreeInputInt);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(MultiplyThreeInputInt)

/******************************************************************************/
/* Declare 4 integer multiply */
#define EHS_FB_MULTIPLY_FOUR_INPUT_INT_NAME "MultiplyFourInputInt"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(MultiplyFourInputInt);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(MultiplyFourInputInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplyFourInputInt);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(MultiplyFourInputInt)

/******************************************************************************/
/* Declare integer division */
#define EHS_FB_DIVISION_INT_NAME "DivisionInt"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(DivisionInt);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(DivisionInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DivisionInt);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(DivisionInt)

/******************************************************************************/
/* Declare integer modulus */
#define EHS_FB_MODULUS_INT_NAME "ModulusInt"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ModulusInt);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ModulusInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ModulusInt);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ModulusInt)

/******************************************************************************/
/* Declare integer power */
#define EHS_FB_POWER_INT_NAME "PowerInt"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(PowerInt);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(PowerInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(PowerInt);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(PowerInt)

/******************************************************************************/
/* Declare integer square */
#define EHS_FB_SQUARE_INT_NAME "SquareInt"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(SquareInt);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(SquareInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(SquareInt);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(SquareInt)

/******************************************************************************/
/* Declare integer add-accumulate */
#define EHS_FB_ADD_ACCUMULATE_INT_NAME "AddAccumulateInt"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(AddAccumulateInt);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(AddAccumulateInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(AddAccumulateInt);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(AddAccumulateInt)

/******************************************************************************/
/* Declare integer subtract-accumulate */
#define EHS_FB_SUB_ACCUMULATE_INT_NAME "SubAccumulateInt"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(SubAccumulateInt);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(SubAccumulateInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(SubAccumulateInt);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(SubAccumulateInt)

/******************************************************************************/
/* Declare integer abs */
#define EHS_FB_ABS_INT_NAME "AbsInt"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(AbsInt);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(AbsInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(AbsInt);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(AbsInt)

/******************************************************************************/
/* Declare integer > */
#define EHS_COMPARATOR_GREATER_INT_NAME "ComparatorGreaterInt1"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ComparatorGreaterInt1);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ComparatorGreaterInt1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ComparatorGreaterInt1);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ComparatorGreaterInt1)

/******************************************************************************/
/* Declare integer >= */
#define EHS_COMPARATOR_GREATER_EQUAL_INT_NAME "ComparatorGreaterEqualInt1"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ComparatorGreaterEqualInt1);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ComparatorGreaterEqualInt1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ComparatorGreaterEqualInt1);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ComparatorGreaterEqualInt1)

/******************************************************************************/
/* Declare integer = */
#define EHS_COMPARATOR_EQUAL_INT_NAME "ComparatorEqualInt1"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ComparatorEqualInt1);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ComparatorEqualInt1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ComparatorEqualInt1);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ComparatorEqualInt1)

/******************************************************************************/
/* Declare integer integrator */
#define EHS_INTEGRATOR_INT_NAME "Integrator_Int"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(IntegratorInt);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(IntegratorInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(IntegratorInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(IntegratorInt_Reset);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(IntegratorInt)

/******************************************************************************/
/* Declare integer max */
#define EHS_MAXINT_INT_NAME "Max_Int"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(MaxInt);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(MaxInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MaxInt);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(MaxInt)

/******************************************************************************/
/* Declare integer min */
#define EHS_MININT_INT_NAME "Min_Int"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(MinInt);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(MinInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MinInt);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(MinInt)

#ifdef EHS_TARGET_FP_SUPPORT
/******************************************************************************/
/* Declare 2 input float addition */
#define EHS_ADDITION_TWO_INPUT_REAL_NAME "AdditionTwoInputReal"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(AdditionTwoInputReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(AdditionTwoInputReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(AdditionTwoInputReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(AdditionTwoInputReal)

/******************************************************************************/
/* Declare 3 input float addition */
#define EHS_ADDITION_THREE_INPUT_FLOAT_NAME "AdditionThreeInputReal"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(AdditionThreeInputReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(AdditionThreeInputReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(AdditionThreeInputReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(AdditionThreeInputReal)

/******************************************************************************/
/* Declare 4  input float addition */
#define EHS_ADDITION_FOUR_INPUT_FLOAT_NAME "AdditionFourInputReal"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(AdditionFourInputReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(AdditionFourInputReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(AdditionFourInputReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(AdditionFourInputReal)

/******************************************************************************/
/* Declare float subtract */
#define EHS_SUBTRACTION_FLOAT_NAME "SubtractionReal"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(SubtractionReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(SubtractionReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(SubtractionReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(SubtractionReal)

/******************************************************************************/
/* Declare 2 input multiply float */
#define EHS_MULTIPLY_TWO_INPUT_FLOAT_NAME "MultiplyTwoInputReal"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(MultiplyTwoInputReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(MultiplyTwoInputReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplyTwoInputReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(MultiplyTwoInputReal)

/******************************************************************************/
/* Declare 3 input multiply float */
#define EHS_MULTIPLY_THREE_INPUT_FLOAT_NAME "MultiplyThreeInputReal"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(MultiplyThreeInputReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(MultiplyThreeInputReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplyThreeInputReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(MultiplyThreeInputReal)

/******************************************************************************/
/* Declare 4 input multiply float */
#define EHS_MULTIPLY_FOUR_INPUT_FLOAT_NAME "MultiplyFourInputReal"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(MultiplyFourInputReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(MultiplyFourInputReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplyFourInputReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(MultiplyFourInputReal)

/******************************************************************************/
/* Declare float divide */
#define EHS_DIVISION_FLOAT_NAME "DivisionReal"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(DivisionReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(DivisionReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DivisionReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(DivisionReal)

/******************************************************************************/
/* Declare float modulus */
#define EHS_MODULUS_FLOAT_NAME "ModulusReal"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ModulusReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ModulusReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ModulusReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ModulusReal)

/******************************************************************************/
/* Declare float power */
#define EHS_POWER_FLOAT_NAME "PowerReal"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(PowerReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(PowerReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(PowerReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(PowerReal)

/******************************************************************************/
/* Declare float square */
#define EHS_SQUARE_FLOAT_NAME "SquareReal"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(SquareReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(SquareReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(SquareReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(SquareReal)

/******************************************************************************/
/* Declare AddAccumulateReal */
#define EHS_ADD_ACCUMULATE_FLOAT_NAME "AddAccumulateReal"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(AddAccumulateReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(AddAccumulateReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(AddAccumulateReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(AddAccumulateReal)

/******************************************************************************/
/* Declare SubAccumulateReal */
#define EHS_SUB_ACCUMULATE_FLOAT_NAME "SubAccumulateReal"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(SubAccumulateReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(SubAccumulateReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(SubAccumulateReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(SubAccumulateReal)

/******************************************************************************/
/* Declare SquareRootReal */
#define EHS_SQUARE_ROOT_FLOAT_NAME "SquareRootReal"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(SquareRootReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(SquareRootReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(SquareRootReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(SquareRootReal)

/******************************************************************************/
/* Declare integer abs */
#define EHS_LOG_BASE_TEN_FLOAT_NAME "LogBaseTenReal"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(LogBaseTenReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(LogBaseTenReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(LogBaseTenReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(LogBaseTenReal)

/******************************************************************************/
/* Declare LogReal */
#define EHS_LOG_FLOAT_NAME "LogReal"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(LogReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(LogReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(LogReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(LogReal)

/******************************************************************************/
/* Declare SineReal */
#define EHS_SINE_FLOAT_NAME "SineReal"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(SineReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(SineReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(SineReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(SineReal)

/******************************************************************************/
/* Declare CosineReal */
#define EHS_COSINE_FLOAT_NAME "CosineReal"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(CosineReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(CosineReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(CosineReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(CosineReal)

/******************************************************************************/
/* Declare TanReal */
#define EHS_TAN_FLOAT_NAME "TanReal"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(TanReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(TanReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(TanReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(TanReal)

/******************************************************************************/
/* Declare ArcSineReal */
#define EHS_ARC_SINE_FLOAT_NAME "ArcSineReal"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ArcSineReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ArcSineReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ArcSineReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ArcSineReal)

/******************************************************************************/
/* Declare ArcCosineReal */
#define EHS_ARC_COSINE_FLOAT_NAME "ArcCosineReal"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ArcCosineReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ArcCosineReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ArcCosineReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ArcCosineReal)

/******************************************************************************/
/* Declare ArcTanReal */
#define EHS_ARC_TAN_FLOAT_NAME "ArcTanReal"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ArcTanReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ArcTanReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ArcTanReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ArcTanReal)

/******************************************************************************/
/* Declare HyperbolicSineReal */
#define EHS_HYPERBOLIC_SINE_FLOAT_NAME "HyperbolicSineReal"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(HyperbolicSineReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(HyperbolicSineReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(HyperbolicSineReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(HyperbolicSineReal)

/******************************************************************************/
/* Declare HyperbolicCosineReal */
#define EHS_HYPERBOLIC_COSINE_FLOAT_NAME "HyperbolicCosineReal"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(HyperbolicCosineReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(HyperbolicCosineReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(HyperbolicCosineReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(HyperbolicCosineReal)

/******************************************************************************/
/* Declare HyperbolicTanReal */
#define EHS_HYPERBOLIC_TAN_FLOAT_NAME "HyperbolicTanReal"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(HyperbolicTanReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(HyperbolicTanReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(HyperbolicTanReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(HyperbolicTanReal)

/******************************************************************************/
/* Declare ArcHyperbolicSineReal */
#define EHS_ARC_HYPERBOLIC_SINE_FLOAT_NAME "ArcHyperbolicSineReal"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ArcHyperbolicSineReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ArcHyperbolicSineReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ArcHyperbolicSineReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ArcHyperbolicSineReal)

/******************************************************************************/
/* Declare ArcHyperbolicCosineReal */
#define EHS_ARC_HYPERBOLIC_COSINE_FLOAT_NAME "ArcHyperbolicCosineReal"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ArcHyperbolicCosineReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ArcHyperbolicCosineReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ArcHyperbolicCosineReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ArcHyperbolicCosineReal)

/******************************************************************************/
/* Declare ArcHyperbolicTanReal */
#define EHS_ARC_HYPERBOLIC_TAN_FLOAT_NAME "ArcHyperbolicTanReal"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ArcHyperbolicTanReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ArcHyperbolicTanReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ArcHyperbolicTanReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ArcHyperbolicTanReal)

/******************************************************************************/
/* Declare float > */
#define EHS_COMPARATOR_GREATER_FLOAT_NAME "ComparatorGreaterReal1"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ComparatorGreaterReal1);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ComparatorGreaterReal1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ComparatorGreaterReal1);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ComparatorGreaterReal1)

/******************************************************************************/
/* Declare float >= */
#define EHS_COMPARATOR_GREATER_EQUAL_FLOAT_NAME "ComparatorGreaterEqualReal1"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ComparatorGreaterEqualReal1);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ComparatorGreaterEqualReal1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ComparatorGreaterEqualReal1);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ComparatorGreaterEqualReal1)

/******************************************************************************/
/* Declare flaot = */
#define EHS_COMPARATOR_EQUAL_FLOAT_NAME "ComparatorEqualReal1"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ComparatorEqualReal1);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ComparatorEqualReal1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ComparatorEqualReal1);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ComparatorEqualReal1)

/******************************************************************************/
/* Declare Integrator_Real */
#define EHS_INTEGRATOR_FLOAT_NAME "Integrator_Real"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(IntegratorReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(IntegratorReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(IntegratorReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(IntegratorReal_Reset);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(IntegratorReal)

/******************************************************************************/
/* Declare ExponentialReal */
#define EHS_EXPONENTIAL_FLOAT_NAME "ExponentialReal"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ExponentialReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ExponentialReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ExponentialReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ExponentialReal)
#endif /* EHS_TARGET_FP_SUPPORT */



#endif /* EHS_OPERATOR_FUNCTIONS_H */
