/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

/**
 * operator1.h
 *
 * function prototypes for operator1.c
 *
 * @author: inx limited
 *
 */
#ifndef EHS_OPERATOR_FUNCTIONS1_H
#define EHS_OPERATOR_FUNCTIONS1_H

#include "ehs_fb_types.h"

/******************************************************************************/
/* Declare 2-input integer addition */

#define EHS_FB_NAME_AdditionTwoInputInt "AdditionTwoInputInt"
#define EHS_FB_ID_AdditionTwoInputInt 0x3F77

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(AdditionTwoInputInt);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(AdditionTwoInputInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(AdditionTwoInputInt);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(AdditionTwoInputInt)

/******************************************************************************/
/* Declare 3-input integer addition */

#define EHS_FB_NAME_AdditionThreeInputInt "AdditionThreeInputInt"
#define EHS_FB_ID_AdditionThreeInputInt 0x6FEC

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(AdditionThreeInputInt);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(AdditionThreeInputInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(AdditionThreeInputInt);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(AdditionThreeInputInt)

/******************************************************************************/
/* Declare 4-input integer addition */

#define EHS_FB_NAME_AdditionFourInputInt "AdditionFourInputInt"
#define EHS_FB_ID_AdditionFourInputInt 0xB009

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(AdditionFourInputInt);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(AdditionFourInputInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(AdditionFourInputInt);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(AdditionFourInputInt)

/******************************************************************************/
/* Declare integer subtraction */

#define EHS_FB_NAME_SubtractionInt "SubtractionInt"
#define EHS_FB_ID_SubtractionInt 0xD06E

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(SubtractionInt);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(SubtractionInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(SubtractionInt);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(SubtractionInt)

/******************************************************************************/
/* Declare 2 integer multiply */

#define EHS_FB_NAME_MultiplyTwoInputInt "MultiplyTwoInputInt"
#define EHS_FB_ID_MultiplyTwoInputInt 0xEB8B

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(MultiplyTwoInputInt);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(MultiplyTwoInputInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplyTwoInputInt);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(MultiplyTwoInputInt)

/******************************************************************************/
/* Declare 3 integer multiply */

#define EHS_FB_NAME_MultiplyThreeInputInt "MultiplyThreeInputInt"
#define EHS_FB_ID_MultiplyThreeInputInt 0x30AD

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(MultiplyThreeInputInt);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(MultiplyThreeInputInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplyThreeInputInt);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(MultiplyThreeInputInt)

/******************************************************************************/
/* Declare 4 integer multiply */

#define EHS_FB_NAME_MultiplyFourInputInt "MultiplyFourInputInt"
#define EHS_FB_ID_MultiplyFourInputInt 0xF1DD

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(MultiplyFourInputInt);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(MultiplyFourInputInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplyFourInputInt);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(MultiplyFourInputInt)

/******************************************************************************/
/* Declare integer division */

#define EHS_FB_NAME_DivisionInt "DivisionInt"
#define EHS_FB_ID_DivisionInt 0xCE65

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(DivisionInt);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(DivisionInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DivisionInt);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(DivisionInt)

/******************************************************************************/
/* Declare integer modulus */

#define EHS_FB_NAME_ModulusInt "ModulusInt"
#define EHS_FB_ID_ModulusInt 0x723A

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ModulusInt);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ModulusInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ModulusInt);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ModulusInt)

/******************************************************************************/
/* Declare integer power */

#define EHS_FB_NAME_PowerInt "PowerInt"
#define EHS_FB_ID_PowerInt 0x5C3A

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(PowerInt);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(PowerInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(PowerInt);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(PowerInt)

/******************************************************************************/
/* Declare integer square */

#define EHS_FB_NAME_SquareInt "SquareInt"
#define EHS_FB_ID_SquareInt 0xFBD9

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(SquareInt);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(SquareInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(SquareInt);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(SquareInt)

/******************************************************************************/
/* Declare integer add-accumulate */

#define EHS_FB_NAME_AddAccumulateInt "AddAccumulateInt"
#define EHS_FB_ID_AddAccumulateInt 0xFCAA

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(AddAccumulateInt);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(AddAccumulateInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(AddAccumulateInt);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(AddAccumulateInt)

/******************************************************************************/
/* Declare integer subtract-accumulate */

#define EHS_FB_NAME_SubAccumulateInt "SubAccumulateInt"
#define EHS_FB_ID_SubAccumulateInt 0x5BF1

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(SubAccumulateInt);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(SubAccumulateInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(SubAccumulateInt);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(SubAccumulateInt)

/******************************************************************************/
/* Declare integer abs */

#define EHS_FB_NAME_AbsInt "AbsInt"
#define EHS_FB_ID_AbsInt 0xCC11

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(AbsInt);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(AbsInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(AbsInt);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(AbsInt)

/******************************************************************************/
/* Declare integer > */

#define EHS_FB_NAME_ComparatorGreaterInt1 "ComparatorGreaterInt1"
#define EHS_FB_ID_ComparatorGreaterInt1 0xD107

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ComparatorGreaterInt1);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ComparatorGreaterInt1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ComparatorGreaterInt1);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ComparatorGreaterInt1)

/******************************************************************************/
/* Declare integer >= */

#define EHS_FB_NAME_ComparatorGreaterEqualInt1 "ComparatorGreaterEqualInt1"
#define EHS_FB_ID_ComparatorGreaterEqualInt1 0x7C85

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ComparatorGreaterEqualInt1);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ComparatorGreaterEqualInt1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ComparatorGreaterEqualInt1);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ComparatorGreaterEqualInt1)

/******************************************************************************/
/* Declare integer = */

#define EHS_FB_NAME_ComparatorEqualInt1 "ComparatorEqualInt1"
#define EHS_FB_ID_ComparatorEqualInt1 0x458D

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ComparatorEqualInt1);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ComparatorEqualInt1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ComparatorEqualInt1);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ComparatorEqualInt1)

/******************************************************************************/
/* Declare integer integrator */

#define EHS_FB_NAME_Integrator_Int "Integrator_Int"
#define EHS_FB_ID_Integrator_Int 0x2E63

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(IntegratorInt);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(IntegratorInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(IntegratorInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(IntegratorInt_Reset);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(IntegratorInt)

/******************************************************************************/
/* Declare integer max */
#define EHS_FB_NAME_Max_Int "Max_Int"
#define EHS_FB_ID_Max_Int 0xFADA

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(MaxInt);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(MaxInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MaxInt);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(MaxInt)

/******************************************************************************/
/* Declare integer min */

#define EHS_FB_NAME_Min_Int "Min_Int"
#define EHS_FB_ID_Min_Int 0x7192

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(MinInt);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(MinInt);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MinInt);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(MinInt)

#ifdef EHS_TARGET_FP_SUPPORT
/******************************************************************************/
/* Declare 2 input float addition */

#define EHS_FB_NAME_AdditionTwoInputReal "AdditionTwoInputReal"
#define EHS_FB_ID_AdditionTwoInputReal 0xBD06

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(AdditionTwoInputReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(AdditionTwoInputReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(AdditionTwoInputReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(AdditionTwoInputReal)

/******************************************************************************/
/* Declare 3 input float addition */
#define EHS_FB_NAME_AdditionThreeInputReal "AdditionThreeInputReal"
#define EHS_FB_ID_AdditionThreeInputReal 0x1617

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(AdditionThreeInputReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(AdditionThreeInputReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(AdditionThreeInputReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(AdditionThreeInputReal)

/******************************************************************************/
/* Declare 4  input float addition */
#define EHS_FB_NAME_AdditionFourInputReal "AdditionFourInputReal"
#define EHS_FB_ID_AdditionFourInputReal 0x9D09

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(AdditionFourInputReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(AdditionFourInputReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(AdditionFourInputReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(AdditionFourInputReal)

/******************************************************************************/
/* Declare float subtract */

#define EHS_FB_NAME_SubtractionReal "SubtractionReal"
#define EHS_FB_ID_SubtractionReal 0x7728

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(SubtractionReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(SubtractionReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(SubtractionReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(SubtractionReal)

/******************************************************************************/
/* Declare 2 input multiply float */

#define EHS_FB_NAME_MultiplyTwoInputReal "MultiplyTwoInputReal"
#define EHS_FB_ID_MultiplyTwoInputReal 0xFCD2

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(MultiplyTwoInputReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(MultiplyTwoInputReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplyTwoInputReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(MultiplyTwoInputReal)

/******************************************************************************/
/* Declare 3 input multiply float */

#define EHS_FB_NAME_MultiplyThreeInputReal "MultiplyThreeInputReal"
#define EHS_FB_ID_MultiplyThreeInputReal 0x2688

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(MultiplyThreeInputReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(MultiplyThreeInputReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplyThreeInputReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(MultiplyThreeInputReal)

/******************************************************************************/
/* Declare 4 input multiply float */

#define EHS_FB_NAME_MultiplyFourInputReal "MultiplyFourInputReal"
#define EHS_FB_ID_MultiplyFourInputReal 0xC248

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(MultiplyFourInputReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(MultiplyFourInputReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MultiplyFourInputReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(MultiplyFourInputReal)

/******************************************************************************/
/* Declare float divide */

#define EHS_FB_NAME_DivisionReal "DivisionReal"
#define EHS_FB_ID_DivisionReal 0xB077

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(DivisionReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(DivisionReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DivisionReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(DivisionReal)

/******************************************************************************/
/* Declare float modulus */

#define EHS_FB_NAME_ModulusReal "ModulusReal"
#define EHS_FB_ID_ModulusReal 0x888B

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ModulusReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ModulusReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ModulusReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ModulusReal)

/******************************************************************************/
/* Declare float power */

#define EHS_FB_NAME_PowerReal "PowerReal"
#define EHS_FB_ID_PowerReal 0x88A5

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(PowerReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(PowerReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(PowerReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(PowerReal)

/******************************************************************************/
/* Declare float square */

#define EHS_FB_NAME_SquareReal "SquareReal"
#define EHS_FB_ID_SquareReal 0x0143

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(SquareReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(SquareReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(SquareReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(SquareReal)

/******************************************************************************/
/* Declare AddAccumulateReal */

#define EHS_FB_NAME_AddAccumulateReal "AddAccumulateReal"
#define EHS_FB_ID_AddAccumulateReal 0xE405

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(AddAccumulateReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(AddAccumulateReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(AddAccumulateReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(AddAccumulateReal)

/******************************************************************************/
/* Declare SubAccumulateReal */

#define EHS_FB_NAME_SUB_ACCUMULATE_FLOAT_NAME "SubAccumulateReal"
#define EHS_FB_ID_SUB_ACCUMULATE_FLOAT_NAME 0x1FE3

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(SubAccumulateReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(SubAccumulateReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(SubAccumulateReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(SubAccumulateReal)

/******************************************************************************/
/* Declare SquareRootReal */

#define EHS_FB_NAME_SquareRootReal "SquareRootReal"
#define EHS_FB_ID_SquareRootReal 0x72E6

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(SquareRootReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(SquareRootReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(SquareRootReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(SquareRootReal)

/******************************************************************************/
/* Declare integer abs */

#define EHS_FB_NAME_LogBaseTenReal "LogBaseTenReal"
#define EHS_FB_ID_LogBaseTenReal 0xACCA

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(LogBaseTenReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(LogBaseTenReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(LogBaseTenReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(LogBaseTenReal)

/******************************************************************************/
/* Declare LogReal */

#define EHS_FB_NAME_LogReal "LogReal"
#define EHS_FB_ID_LogReal 0x4998

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(LogReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(LogReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(LogReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(LogReal)

/******************************************************************************/
/* Declare SineReal */

#define EHS_FB_NAME_SineReal "SineReal"
#define EHS_FB_ID_SineReal 0xB3D1

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(SineReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(SineReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(SineReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(SineReal)

/******************************************************************************/
/* Declare CosineReal */

#define EHS_FB_NAME_CosineReal "CosineReal"
#define EHS_FB_ID_CosineReal 0x82C8

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(CosineReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(CosineReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(CosineReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(CosineReal)

/******************************************************************************/
/* Declare TanReal */

#define EHS_FB_NAME_TanReal "TanReal"
#define EHS_FB_ID_TanReal 0x67DD

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(TanReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(TanReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(TanReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(TanReal)

/******************************************************************************/
/* Declare ArcSineReal */

#define EHS_FB_NAME_ArcSineReal "ArcSineReal"
#define EHS_FB_ID_ArcSineReal 0x9D9F

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ArcSineReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ArcSineReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ArcSineReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ArcSineReal)

/******************************************************************************/
/* Declare ArcCosineReal */

#define EHS_FB_NAME_ArcCosineReal "ArcCosineReal"
#define EHS_FB_ID_ArcCosineReal 0x3E7D

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ArcCosineReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ArcCosineReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ArcCosineReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ArcCosineReal)

/******************************************************************************/
/* Declare ArcTanReal */

#define EHS_FB_NAME_ArcTanReal "ArcTanReal"
#define EHS_FB_ID_ArcTanReal 0xE9B4

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ArcTanReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ArcTanReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ArcTanReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ArcTanReal)

/******************************************************************************/
/* Declare HyperbolicSineReal */

#define EHS_FB_NAME_HyperbolicSineReal "HyperbolicSineReal"
#define EHS_FB_ID_HyperbolicSineReal 0x75FB

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(HyperbolicSineReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(HyperbolicSineReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(HyperbolicSineReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(HyperbolicSineReal)

/******************************************************************************/
/* Declare HyperbolicCosineReal */

#define EHS_FB_NAME_HyperbolicCosineReal "HyperbolicCosineReal"
#define EHS_FB_ID_HyperbolicCosineReal 0xB057

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(HyperbolicCosineReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(HyperbolicCosineReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(HyperbolicCosineReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(HyperbolicCosineReal)

/******************************************************************************/
/* Declare HyperbolicTanReal */

#define EHS_FB_NAME_HyperbolicTanReal "HyperbolicTanReal"
#define EHS_FB_ID_HyperbolicTanReal 0x4CD5

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(HyperbolicTanReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(HyperbolicTanReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(HyperbolicTanReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(HyperbolicTanReal)

/******************************************************************************/
/* Declare ArcHyperbolicSineReal */

#define EHS_FB_NAME_ArcHyperbolicSineReal "ArcHyperbolicSineReal"
#define EHS_FB_ID_ArcHyperbolicSineReal 0xF54D

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ArcHyperbolicSineReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ArcHyperbolicSineReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ArcHyperbolicSineReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ArcHyperbolicSineReal)

/******************************************************************************/
/* Declare ArcHyperbolicCosineReal */

#define EHS_FB_NAME_ArcHyperbolicCosineReal "ArcHyperbolicCosineReal"
#define EHS_FB_ID_ArcHyperbolicCosineReal 0x7020

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ArcHyperbolicCosineReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ArcHyperbolicCosineReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ArcHyperbolicCosineReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ArcHyperbolicCosineReal)

/******************************************************************************/
/* Declare ArcHyperbolicTanReal */

#define EHS_FB_NAME_ArcHyperbolicTanReal "ArcHyperbolicTanReal"
#define EHS_FB_ID_ArcHyperbolicTanReal 0x7B2B

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ArcHyperbolicTanReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ArcHyperbolicTanReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ArcHyperbolicTanReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ArcHyperbolicTanReal)

/******************************************************************************/
/* Declare float > */

#define EHS_FB_NAME_ComparatorGreaterReal1 "ComparatorGreaterReal1"
#define EHS_FB_ID_ComparatorGreaterReal1 0x641E

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ComparatorGreaterReal1);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ComparatorGreaterReal1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ComparatorGreaterReal1);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ComparatorGreaterReal1)

/******************************************************************************/
/* Declare float >= */

#define EHS_FB_NAME_ComparatorGreaterEqualReal1 "ComparatorGreaterEqualReal1"
#define EHS_FB_ID_ComparatorGreaterEqualReal1 0x0533

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ComparatorGreaterEqualReal1);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ComparatorGreaterEqualReal1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ComparatorGreaterEqualReal1);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ComparatorGreaterEqualReal1)

/******************************************************************************/
/* Declare flaot = */

#define EHS_FB_NAME_ComparatorEqualReal1 "ComparatorEqualReal1"
#define EHS_FB_ID_ComparatorEqualReal1 0xC30B

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ComparatorEqualReal1);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ComparatorEqualReal1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ComparatorEqualReal1);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ComparatorEqualReal1)

/******************************************************************************/
/* Declare Integrator_Real */

#define EHS_FB_NAME_Integrator_Real "Integrator_Real"
#define EHS_FB_ID_Integrator_Real 0xB217

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(IntegratorReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(IntegratorReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(IntegratorReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(IntegratorReal_Reset);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(IntegratorReal)

/******************************************************************************/
/* Declare ExponentialReal */

#define EHS_FB_NAME_ExponentialReal "ExponentialReal"
#define EHS_FB_ID_ExponentialReal 0x79A5

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ExponentialReal);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ExponentialReal);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ExponentialReal);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ExponentialReal)
#endif /* EHS_TARGET_FP_SUPPORT */



#endif /* EHS_OPERATOR_FUNCTIONS_H */
