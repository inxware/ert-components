/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

/** @file core_components.c
 * Contains the static definition of the function block reference table for EHS
 *
 * @author: inx limited
 *
 */


#include "blockref_table.h"
#include "hal.h" /* Required for logging */
#include "fid.h"  /* for the Function Instance data types */

/* Components */
#include "arraystring1.h"
#include "buffer.h"
#include "calc_i.h"
#include "const1.h"
#include "convertor.h"
#include "demux.h"
#include "event_counter1.h"
#include "file.h"
#include "ehs_if.h"
#include "logic.h"
#include "mux.h"
#include "operator1.h"
#include "sfc.h"
#include "state_condition.h"
#include "state_debug.h"
#include "state_manager.h"
#include "stringfn.h"
#include "time_clock.h"
#include "wall_clock.h"
#include "trigger.h"
#include "fs_dir_list1.h"
#include "fs_dir_create_remove.h"
#include "system_exec.h"

#include "rtinfo.h"
#include "appinfo.h"
#include "appget.h"
#include "inx-rng.h"



const EhsBlockRefType EhsBlockRefTable_core[] =
{
#ifdef EHS_TARGET_FP_SUPPORT
	/* from buffer.h */
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_FIFO_Real, EHS_FB_ID_FIFO_Real, FIFOReal),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_LIFO_Real, EHS_FB_ID_LIFO_Real, LIFOReal),
	/* from const.h */
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_Constant_Real1, EHS_FB_ID_Constant_Real1, ConstantFloat1),
	/* convertor.h */
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_ConvertorRealToBool,EHS_FB_ID_ConvertorRealToBool, ConvertorRealToBool),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_ConvertorRealToInt,EHS_FB_ID_ConvertorRealToInt ,ConvertorRealToInt),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_ConvertorRealToString,EHS_FB_ID_ConvertorRealToString ,ConvertorRealToString),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_ConvertorBoolToReal,EHS_FB_ID_ConvertorBoolToReal, ConvertorBoolToReal),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_ConvertorIntToReal,EHS_FB_ID_ConvertorIntToReal ,ConvertorIntToReal),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_ConvertorStringToReal,EHS_FB_ID_ConvertorStringToReal ,ConvertorStringToReal),
	/* demux.h */
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_DemultiplexTwoOutputReal,EHS_FB_ID_DemultiplexTwoOutputReal, DemultiplexTwoOutputFloat),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_DemultiplexThreeOutputReal,EHS_FB_ID_DemultiplexThreeOutputReal, DemultiplexThreeOutputFloat),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_DemultiplexFourOutputReal, EHS_FB_ID_DemultiplexFourOutputReal,DemultiplexFourOutputFloat),
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_NAME_IndexedDemultiplexer_Float,EHS_FB_ID_IndexedDemultiplexer_Float,IndexedDemultiplexer_Float, EHS_FB_INIT_NAME(IndexedDemultiplexer),EHS_FB_IDENTIFY_NAME(IndexedDemultiplexer)),
	/* file.h */
	EHS_BLOCKREF_ENTRY_WITH_DESTROY(EHS_FB_NAME_FILE_ReadOnly_Real,EHS_FB_ID_FILE_ReadOnly_Real, FILE_ReadOnly_Float),
	EHS_BLOCKREF_ENTRY_WITH_DESTROY(EHS_FB_NAME_FILE_WriteOnly_Real,EHS_FB_ID_FILE_WriteOnly_Real, FILE_WriteOnly_Float),
	/* multiplexerFunctions.h */
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_IndexedDemultiplexer_Float,EHS_FB_ID_IndexedDemultiplexer_Float, MultiplexTwoInputFloat),
	/* Fixed width - todo2024 we only really need to implement one of these and use different CDFs */
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_MultiplexOneInputReal,EHS_FB_ID_MultiplexOneInputReal, MultiplexOneInputFloat),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_MultiplexTwoInputReal,EHS_FB_ID_MultiplexTwoInputReal, MultiplexTwoInputFloat),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_MultiplexThreeInputReal,EHS_FB_ID_MultiplexThreeInputReal, MultiplexThreeInputFloat),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_MultiplexFourInputReal,EHS_FB_ID_MultiplexFourInputReal, MultiplexFourInputFloat),
	/* operator1.h */
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_AdditionTwoInputReal,EHS_FB_ID_AdditionTwoInputReal, AdditionTwoInputReal),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_AdditionThreeInputReal,EHS_FB_ID_AdditionThreeInputReal, AdditionThreeInputReal),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_AdditionFourInputReal,EHS_FB_ID_AdditionFourInputReal, AdditionFourInputReal),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_SubtractionReal,EHS_FB_ID_SubtractionReal ,SubtractionReal),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_MultiplyTwoInputReal,EHS_FB_ID_MultiplyTwoInputReal, MultiplyTwoInputReal),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_MultiplyThreeInputReal,EHS_FB_ID_MultiplyThreeInputReal, MultiplyThreeInputReal),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_MultiplyFourInputReal,EHS_FB_ID_MultiplyFourInputReal, MultiplyFourInputReal),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_DivisionReal, EHS_FB_ID_DivisionReal,DivisionReal),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_ModulusReal,EHS_FB_ID_ModulusReal, ModulusReal),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_PowerReal,EHS_FB_ID_PowerReal, PowerReal),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_SquareReal,EHS_FB_ID_SquareReal, SquareReal),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_AddAccumulateReal,EHS_FB_ID_AddAccumulateReal, AddAccumulateReal),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_SquareRootReal,EHS_FB_ID_SquareRootReal, SquareRootReal),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_LogBaseTenReal,EHS_FB_ID_LogBaseTenReal, LogBaseTenReal),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_LogReal,EHS_FB_ID_LogReal, LogReal),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_SineReal,EHS_FB_ID_SineReal ,SineReal),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_CosineReal,EHS_FB_ID_CosineReal, CosineReal),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_TanReal,EHS_FB_ID_TanReal, TanReal),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_ArcSineReal,EHS_FB_ID_ArcSineReal, ArcSineReal),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_ArcCosineReal,EHS_FB_ID_ArcCosineReal, ArcCosineReal),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_ArcTanReal,EHS_FB_ID_ArcTanReal, ArcTanReal),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_HyperbolicSineReal,EHS_FB_ID_HyperbolicSineReal, HyperbolicSineReal),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_HyperbolicCosineReal,EHS_FB_ID_HyperbolicCosineReal, HyperbolicCosineReal),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_HyperbolicTanReal,EHS_FB_ID_HyperbolicTanReal, HyperbolicTanReal),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_ArcHyperbolicSineReal,EHS_FB_ID_ArcHyperbolicSineReal, ArcHyperbolicSineReal),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_ArcHyperbolicCosineReal,EHS_FB_ID_ArcHyperbolicCosineReal, ArcHyperbolicCosineReal),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_ArcHyperbolicTanReal,EHS_FB_ID_ArcHyperbolicTanReal, ArcHyperbolicTanReal),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_ComparatorGreaterReal1,EHS_FB_ID_ComparatorGreaterReal1, ComparatorGreaterReal1),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_ComparatorGreaterEqualReal1,EHS_FB_ID_ComparatorGreaterEqualReal1, ComparatorGreaterEqualReal1),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_ComparatorEqualReal1,EHS_FB_ID_ComparatorEqualReal1, ComparatorEqualReal1),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_Integrator_Real,EHS_FB_ID_Integrator_Real, IntegratorReal),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_ExponentialReal,EHS_FB_ID_ExponentialReal, ExponentialReal),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_ArrayReal1,EHS_FB_ID_ArrayReal1,ArrayReal1),

#endif /* EHS_TARGET_FP_SUPPORT */

	/* from arraystring1.h */
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_ArrayString1,EHS_FB_ID_ArrayString1,ArrayString1),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_ArrayInt1,EHS_FB_ID_ArrayInt1,ArrayInt1),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_ArrayBool1,EHS_FB_ID_ArrayBool1,ArrayBool1),
	/* from bufferFunctions.h */
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_FIFO_Int,EHS_FB_ID_FIFO_Int, FIFOInt),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_LIFO_Int,EHS_FB_ID_LIFO_Int, LIFOInt),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_FIFO_Bool,EHS_FB_ID_FIFO_Bool, FIFOBool),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_LIFO_Bool,EHS_FB_ID_LIFO_Bool, LIFOBool),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_FIFO_String,EHS_FB_ID_FIFO_String, FIFOString),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_LIFO_String,EHS_FB_ID_LIFO_String, LIFOString),
	/* calc_i.h */
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_ICalc_1,EHS_FB_ID_ICalc_1,ICalc_1),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_ICalc_2,EHS_FB_ID_ICalc_2,ICalc_2),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_ICalc_4,EHS_FB_ID_ICalc_4,ICalc_4),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_ICalc_6,EHS_FB_ID_ICalc_6,ICalc_6),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_ICalc_8,EHS_FB_ID_ICalc_8,ICalc_8),
	/* from constFunctions.h */
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_Constant_Int1,EHS_FB_ID_Constant_Int1, ConstantInt1),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_Constant_String1,EHS_FB_ID_Constant_String1, ConstantString1),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_Constant_Bool1,EHS_FB_ID_Constant_Bool1, ConstantBool1),
	/* convertorFunctions.h */
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_ConvertorBoolToTrigger,EHS_FB_ID_ConvertorBoolToTrigger, ConvertorBoolToTrigger),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_ConvertorBoolToInt,EHS_FB_ID_ConvertorBoolToInt, ConvertorBoolToInt),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_ConvertorBoolToString,EHS_FB_ID_ConvertorBoolToString, ConvertorBoolToString),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_ConvertorIntToBool,EHS_FB_ID_ConvertorIntToBool, ConvertorIntToBool),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_ConvertorIntToString,EHS_FB_ID_ConvertorIntToString, ConvertorIntToString),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_ConvertorStringToBool,EHS_FB_ID_ConvertorStringToBool, ConvertorStringToBool),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_ConvertorStringToInt,EHS_FB_ID_ConvertorStringToInt, ConvertorStringToInt),
	/* demux.h */
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_DemultiplexTwoOutputInt,EHS_FB_ID_DemultiplexTwoOutputInt, DemultiplexTwoOutputInt),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_DemultiplexThreeOutputInt,EHS_FB_ID_DemultiplexThreeOutputInt, DemultiplexThreeOutputInt),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_DemultiplexFourOutputInt, EHS_FB_ID_DemultiplexFourOutputInt,DemultiplexFourOutputInt),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_DemultiplexTwoOutputBool,EHS_FB_ID_DemultiplexTwoOutputBool, DemultiplexTwoOutputBool),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_DemultiplexThreeOutputBool,EHS_FB_ID_DemultiplexThreeOutputBool, DemultiplexThreeOutputBool),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_DemultiplexFourOutputBool,EHS_FB_ID_DemultiplexFourOutputBool ,DemultiplexFourOutputBool),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_DemultiplexTwoOutputString,EHS_FB_ID_DemultiplexTwoOutputString, DemultiplexTwoOutputString),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_DemultiplexThreeOutputString,EHS_FB_ID_DemultiplexThreeOutputString, DemultiplexThreeOutputString),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_DemultiplexFourOutputString,EHS_FB_ID_DemultiplexFourOutputString, DemultiplexFourOutputString),
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_NAME_IndexedDemultiplexer_Bool,EHS_FB_ID_IndexedDemultiplexer_Bool,IndexedDemultiplexer_Bool,EHS_FB_INIT_NAME(IndexedDemultiplexer),EHS_FB_IDENTIFY_NAME(IndexedDemultiplexer)),
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_NAME_IndexedDemultiplexer_Int,EHS_FB_ID_IndexedDemultiplexer_Int,IndexedDemultiplexer_Int,EHS_FB_INIT_NAME(IndexedDemultiplexer),EHS_FB_IDENTIFY_NAME(IndexedDemultiplexer)),
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_NAME_IndexedDemultiplexer_String,EHS_FB_ID_IndexedDemultiplexer_String,IndexedDemultiplexer_String,EHS_FB_INIT_NAME(IndexedDemultiplexer),EHS_FB_IDENTIFY_NAME(IndexedDemultiplexer)),
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_NAME_IndexedDemultiplexer_Event,EHS_FB_ID_IndexedDemultiplexer_Event,IndexedDemultiplexer_Event,EHS_FB_INIT_NAME(IndexedDemultiplexer),EHS_FB_IDENTIFY_NAME(IndexedDemultiplexer)),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_TextCaseDemultiplexer_Event,EHS_FB_ID_TextCaseDemultiplexer_Event, TextCaseDemultiplexer_Event),
	/* eventCounter1.h */
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_EventCounterVersatile1,EHS_FB_ID_EventCounterVersatile1,EventCounterVersatile1),
	/* fileFunctions.h */
	EHS_BLOCKREF_ENTRY_WITH_DESTROY(EHS_FB_NAME_FILE_ReadOnly_Int,EHS_FB_ID_FILE_ReadOnly_Int, FILE_ReadOnly_Int),
	EHS_BLOCKREF_ENTRY_WITH_DESTROY(EHS_FB_NAME_FILE_WriteOnly_Int,EHS_FB_ID_FILE_WriteOnly_Int, FILE_WriteOnly_Int),
	EHS_BLOCKREF_ENTRY_WITH_DESTROY(EHS_FB_NAME_FILE_ReadOnly_Bool,EHS_FB_ID_FILE_ReadOnly_Bool, FILE_ReadOnly_Bool),
	EHS_BLOCKREF_ENTRY_WITH_DESTROY(EHS_FB_NAME_FILE_WriteOnly_Bool,EHS_FB_ID_FILE_WriteOnly_Bool, FILE_WriteOnly_Bool),
	EHS_BLOCKREF_ENTRY_WITH_DESTROY(EHS_FB_NAME_FILE_ReadOnly_String,EHS_FB_ID_FILE_ReadOnly_String, FILE_ReadOnly_String),
	EHS_BLOCKREF_ENTRY_WITH_DESTROY(EHS_FB_NAME_FILE_WriteOnly_String,EHS_FB_ID_FILE_WriteOnly_String, FILE_WriteOnly_String),
	/* ehs_if.h */
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_if,EHS_FB_ID_if,If),
	/* logic.h */
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_LogicNot,EHS_FB_ID_LogicNot, LogicNot),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_LogicTwoInputAnd,EHS_FB_ID_LogicTwoInputAnd, LogicTwoInputAnd),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_LogicThreeInputAnd,EHS_FB_ID_LogicThreeInputAnd, LogicThreeInputAnd),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_LogicFourInputAnd,EHS_FB_ID_LogicFourInputAnd, LogicFourInputAnd),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_LogicTwoInputOr,EHS_FB_ID_LogicTwoInputOr, LogicTwoInputOr),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_LogicThreeInputOr,EHS_FB_ID_LogicThreeInputOr, LogicThreeInputOr),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_LogicFourInputOr,EHS_FB_ID_LogicFourInputOr, LogicFourInputOr),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_LogicTwoInputNor,EHS_FB_ID_LogicTwoInputNor, LogicTwoInputNor),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_LogicThreeInputNor,EHS_FB_ID_LogicThreeInputNor, LogicThreeInputNor),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_LogicFourInputNor,EHS_FB_ID_LogicFourInputNor, LogicFourInputNor),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_LogicTwoInputNand,EHS_FB_ID_LogicTwoInputNand, LogicTwoInputNand),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_LogicThreeInputNand,EHS_FB_ID_LogicThreeInputNand, LogicThreeInputNand),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_LogicFourInputNand,EHS_FB_ID_LogicFourInputNand, LogicFourInputNand),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_LogicXor,EHS_FB_ID_LogicXor, LogicXor),
	/* mux.h */
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_MultiplexOneInputInt,EHS_FB_ID_MultiplexOneInputInt, MultiplexOneInputInt),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_MultiplexTwoInputInt,EHS_FB_ID_MultiplexTwoInputInt, MultiplexTwoInputInt),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_MultiplexThreeInputInt,EHS_FB_ID_MultiplexThreeInputInt, MultiplexThreeInputInt),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_MultiplexFourInputInt,EHS_FB_ID_MultiplexFourInputInt, MultiplexFourInputInt),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_MultiplexOneInputBool,EHS_FB_ID_MultiplexOneInputBool, MultiplexOneInputBool),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_MultiplexTwoInputBool,EHS_FB_ID_MultiplexTwoInputBool, MultiplexTwoInputBool),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_MultiplexThreeInputBool,EHS_FB_ID_MultiplexThreeInputBool, MultiplexThreeInputBool),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_MultiplexFourInputBool,EHS_FB_ID_MultiplexFourInputBool, MultiplexFourInputBool),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_MultiplexOneInputString,EHS_FB_ID_MultiplexOneInputString, MultiplexOneInputString),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_MultiplexTwoInputString,EHS_FB_ID_MultiplexTwoInputString, MultiplexTwoInputString),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_MultiplexThreeInputString,EHS_FB_ID_MultiplexThreeInputString, MultiplexThreeInputString),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_MultiplexFourInputString,EHS_FB_ID_MultiplexFourInputString, MultiplexFourInputString),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_IndexedMultiplexer_Event,EHS_FB_ID_IndexedMultiplexer_Event, IndexedMultiplexer_Event),
	/* operator1.h */
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_AdditionTwoInputInt,EHS_FB_ID_AdditionTwoInputInt, AdditionTwoInputInt),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_AdditionThreeInputInt,EHS_FB_ID_AdditionThreeInputInt, AdditionThreeInputInt),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_AdditionFourInputInt,EHS_FB_ID_AdditionFourInputInt, AdditionFourInputInt),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_SubtractionInt,EHS_FB_ID_SubtractionInt, SubtractionInt),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_MultiplyTwoInputInt,EHS_FB_ID_MultiplyTwoInputInt, MultiplyTwoInputInt),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_MultiplyThreeInputInt,EHS_FB_ID_MultiplyThreeInputInt, MultiplyThreeInputInt),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_MultiplyFourInputInt,EHS_FB_ID_MultiplyFourInputInt, MultiplyFourInputInt),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_DivisionInt,EHS_FB_ID_DivisionInt, DivisionInt),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_ModulusInt,EHS_FB_ID_ModulusInt, ModulusInt),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_PowerInt,EHS_FB_ID_PowerInt, PowerInt),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_SquareInt,EHS_FB_ID_SquareInt, SquareInt),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_AddAccumulateInt,EHS_FB_ID_AddAccumulateInt, AddAccumulateInt),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_SubAccumulateInt,EHS_FB_ID_SubAccumulateInt, SubAccumulateInt),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_AbsInt,EHS_FB_ID_AbsInt, AbsInt),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_ComparatorGreaterInt1,EHS_FB_ID_ComparatorGreaterInt1, ComparatorGreaterInt1),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_ComparatorGreaterEqualInt1,EHS_FB_ID_ComparatorGreaterEqualInt1, ComparatorGreaterEqualInt1),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_ComparatorEqualInt1,EHS_FB_ID_ComparatorEqualInt1, ComparatorEqualInt1),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_Integrator_Int,EHS_FB_ID_Integrator_Int, IntegratorInt),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_Max_Int,EHS_FB_ID_Max_Int, MaxInt),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_Min_Int,EHS_FB_ID_Min_Int, MinInt),
	/* inx-rng.h */
	EHS_BLOCKREF_ENTRY(INXWARE_FB_NAME_rng, INXWARE_FB_ID_rng, rng),
#ifdef EHS_LEGACY_STATEMACHINE
	/* sfc.h */
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_SFCBar,EHS_FB_ID_SFCBar, SFCBar),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_SFCBarGroup1,EHS_FB_ID_SFCBarGroup1, SFCBarGroup1),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_SFCBarGroup2,EHS_FB_ID_SFCBarGroup2, SFCBarGroup2),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_SFCBarGroup3,EHS_FB_ID_SFCBarGroup3, SFCBarGroup3),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_SFCBarGroup4,EHS_FB_ID_SFCBarGroup4, SFCBarGroup4),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_SFCBarGroup8,EHS_FB_ID_SFCBarGroup8, SFCBarGroup8),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_SFCBarGroup12,EHS_FB_ID_SFCBarGroup12, SFCBarGroup12),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_SFCBarGroup1Event,EHS_FB_ID_SFCBarGroup1Event, SFCBarGroup1Event),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_SFCBarGroup2Event,EHS_FB_ID_SFCBarGroup2Event, SFCBarGroup2Event),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_SFCBarGroup3Event,EHS_FB_ID_SFCBarGroup3Event, SFCBarGroup3Event),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_SFCBarGroup4Event,EHS_FB_ID_SFCBarGroup4Event, SFCBarGroup4Event),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_SFCBarGroup8Event,EHS_FB_ID_SFCBarGroup8Event, SFCBarGroup8Event),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_SFCBarGroup12Event,EHS_FB_ID_SFCBarGroup12Event, SFCBarGroup12Event),
#endif
	/* state_condition.h state_debug.h  state_manager.h */
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_state_condition,EHS_FB_ID_state_condition, state_condition),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_state_debug,EHS_FB_ID_state_debug, state_debug),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_state_manager,EHS_FB_ID_state_manager, state_manager),
	//EHS_BLOCKREF_ENTRY(EHS_FB_NAME_state,EHS_FB_ID_state, state),
	/* stringfn.h */
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_CatString,EHS_FB_ID_CatString, string_cat),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_CmpString,EHS_FB_ID_CmpString, string_cmp),
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_NAME_LenString,EHS_FB_ID_LenString, string_len,NULL,NULL),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_string_format,EHS_FB_ID_string_format, string_format),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_string_format8,EHS_FB_ID_string_format8, string_format8),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_string_format8_int,EHS_FB_ID_string_format8_int, string_format8_int),
#ifdef EHS_TARGET_FP_SUPPORT
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_string_format8_real,EHS_FB_ID_string_format8_real, string_format8_real),
#endif
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_stringfn_scan8,EHS_FB_ID_stringfn_scan8, stringfn_scanf8),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_FindString,EHS_FB_ID_FindString, string_find),
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_NAME_ToUpperString,EHS_FB_ID_ToUpperString, string_toUpper,NULL,NULL),
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_NAME_ToLowerString,EHS_FB_ID_ToLowerString, string_toLower,NULL,NULL),
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_NAME_CharAtString,EHS_FB_ID_CharAtString, string_charAt,NULL,NULL),
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_NAME_StrAtString,EHS_FB_ID_StrAtString, string_strAt,NULL,NULL),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_InsertString,EHS_FB_ID_InsertString, string_insert),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_SubString,EHS_FB_ID_SubString, string_sub),
	/* time_clock.h */
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_time_clock,EHS_FB_ID_time_clock, time_clock),
	/* trigger.h */
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_NAME_Start,EHS_FB_ID_Start, Start, NULL, NULL),
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_NAME_EventBooleanControl,EHS_FB_ID_EventBooleanControl, EventBooleanControl, NULL, NULL),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_EventTwoInputLatchingAnd,EHS_FB_ID_EventTwoInputLatchingAnd, EventTwoInputLatchingAnd),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_EventThreeInputLatchingAnd,EHS_FB_ID_EventThreeInputLatchingAnd, EventThreeInputLatchingAnd),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_EventFourInputLatchingAnd,EHS_FB_ID_EventFourInputLatchingAnd, EventFourInputLatchingAnd),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_EventOneInputManRstAnd,EHS_FB_ID_EventOneInputManRstAnd, EventOneInputManRstAnd),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_EventTwoInputManRstAnd,EHS_FB_ID_EventTwoInputManRstAnd, EventTwoInputManRstAnd),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_EventThreeInputManRstAnd,EHS_FB_ID_EventThreeInputManRstAnd, EventThreeInputManRstAnd),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_EventFourInputManRstAnd,EHS_FB_ID_EventFourInputManRstAnd, EventFourInputManRstAnd),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_EventSelfResetTwoInputLatchingAnd,EHS_FB_ID_EventSelfResetTwoInputLatchingAnd, EventSelfResetTwoInputLatchingAnd),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_EventSelfResetThreeInputLatchingAnd,EHS_FB_ID_EventSelfResetThreeInputLatchingAnd, EventSelfResetThreeInputLatchingAnd),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_EventSelfResetFourInputLatchingAnd,EHS_FB_ID_EventSelfResetFourInputLatchingAnd, EventSelfResetFourInputLatchingAnd),
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_NAME_EventTwoInputOr,EHS_FB_ID_EventTwoInputOr, EventTwoInputOr, NULL, NULL),
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_NAME_EventThreeInputOr,EHS_FB_ID_EventThreeInputOr, EventThreeInputOr, NULL, NULL),
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_NAME_EventFourInputOr,EHS_FB_ID_EventFourInputOr, EventFourInputOr, NULL, NULL),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_EventToggleBoolean,EHS_FB_ID_EventToggleBoolean, EventToggleBoolean),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_EventPositiveEdgeDetect,EHS_FB_ID_EventPositiveEdgeDetect, EventPositiveEdgeDetect),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_EventNegativeEdgeDetect,EHS_FB_ID_EventNegativeEdgeDetect, EventNegativeEdgeDetect),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_EventEdgeDetect,EHS_FB_ID_EventEdgeDetect, EventEdgeDetect),
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_NAME_EventSetReset,EHS_FB_ID_EventSetReset, EventSetReset, NULL, NULL),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_EventCounter,EHS_FB_ID_EventCounter, EventCounter),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_RuntimeInfo,EHS_FB_ID_RuntimeInfo, runtimeinfo),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_AppInfo,EHS_FB_ID_AppInfo, appinfo),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_wall_clock,EHS_FB_ID_wall_clock, wall_clock),
	EHS_BLOCKREF_ENTRY_WITH_DESTROY(EHS_FB_NAME_FileSystemDirList1,EHS_FB_ID_FileSystemDirList1, FileSystemDirList1),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_FileSystemDirCreateRemove,EHS_FB_ID_FileSystemDirCreateRemove, FileSystemDirCreateRemove),
	/* system_exec.h */
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_Exec,EHS_FB_ID_Exec, Exec),
	#ifdef EHS_DEVMAN_SUPPORT
		EHS_BLOCKREF_ENTRY(EHS_FB_NAME_AppGet,EHS_FB_ID_AppGet, appget),
	#endif

    {0}
};

