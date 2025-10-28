/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/**
 * calc_i.h
 *
 * function prototypes for calc_i.c (general purpose integer calculation block)
 *
 * @author: inx limited
 *
 */

#ifndef EHS_FB_CALC_INT_H
#define EHS_FB_CALC_INT_H

// set to use integer only calculations - not some functions wont be very useful such as log...
#ifdef INX_INTEGER_INTERNALS
#define EhsInternalCalcData EhsDataflowIntType
#define EhsGetInternalTypeFromString(a,b) EhsGetSint32FromString(a,b)
#else
#define EhsInternalCalcData EhsDataflowFloatType
#define EhsGetInternalTypeFromString(a,b) EhsGetDoubleFromString(a,b)
#endif


/******************************************************************************/
/* Declare State */
#define EHS_FB_NAME_ICalc "ICalc"
#define EHS_FB_ID_ICalc 0xBDF4

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(ICalc);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(ICalc);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ICalc);

#define EHS_FB_NAME_ICalc_1 "ICalc_1"
#define EHS_FB_ID_ICalc_1 0x957E

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(ICalc_1);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(ICalc_1);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ICalc_1);
#define EHS_FB_NAME_ICalc_2 "ICalc_2"
#define EHS_FB_ID_ICalc_2 0x943E

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(ICalc_2);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(ICalc_2);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ICalc_2);

#define EHS_FB_NAME_ICalc_4 "ICalc_4"
#define EHS_FB_ID_ICalc_4 0x96BE

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(ICalc_4);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(ICalc_4);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ICalc_4);

#define EHS_FB_NAME_ICalc_6 "ICalc_6"
#define EHS_FB_ID_ICalc_6 0x573F

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(ICalc_6);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(ICalc_6);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ICalc_6);

#define EHS_FB_NAME_ICalc_8 "ICalc_8"
#define EHS_FB_ID_ICalc_8 0x93BE

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(ICalc_8);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(ICalc_8);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ICalc_8);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ICalc)
EHS_FB_FUNCTIONS(ICalc_1)
EHS_FB_FUNCTIONS(ICalc_2)
EHS_FB_FUNCTIONS(ICalc_4)
EHS_FB_FUNCTIONS(ICalc_6)
EHS_FB_FUNCTIONS(ICalc_8)

/******************************************************************************/
/* type definitions (normally) local to the integer calculation block, but also
 * required for unit testing purposes */

/**
 * kind of operations that the calculation engine can execute
 */
typedef enum
{
    EhsICalcEngInstr_Nop,	/**< No-operation - not an instruction */
    EhsICalcEngInstr_Negate,	/**< Negate the top number on the stack */
    EhsICalcEngInstr_Add,	/**< Add top two numbers on stack */
    EhsICalcEngInstr_Sub,	/**< Subtract 2nd number from top number on stack */
    EhsICalcEngInstr_Mult,	/**< Multiple top two numbers on stack */
    EhsICalcEngInstr_Div,	/**< Divide 2nd number on stack by top number on stack */
    EhsICalcEngInstr_Mod,	/**< Modulus(2nd number, top number) */
    EhsICalcEngInstr_Exp,	/**< Raise 2nd number on stack to power of top number  */
    EhsICalcEngInstr_Min,	/**< Get smallest of top two numbers on stack */
    EhsICalcEngInstr_Max,	/**< Get largest of top two numbers on stack */
    EhsICalcEngInstr_Abs,	/**< remove sign of number at top of stack */
    EhsICalcEngInstr_Log10,	/**< log base 10*/
    EhsICalcEngInstr_Loge,	/**< rnatural log */
    EhsICalcEngInstr_Cos,	/**< convert angle into cosine */
    EhsICalcEngInstr_Sin,	/**< convert angle into sine */
    EhsICalcEngInstr_Square,/**< multiply top number on stack by itself */
    EhsICalcEngInstr_Const,	/**< put constant value on stack */
    EhsICalcEngInstr_Input	/**< get input value, put it on stack */
} EhsICalcEngInstrEnum;

/**
 * State managed by the validate token function, which is responsible for correct parsing
 * of input math expressions. See EhsICalcValidateToken for definition of states
 */
typedef enum
{
    EhsICalcInputState_init, /* Start state */
    EhsICalcInputState_value,
    EhsICalcInputState_close,
    EhsICalcInputState_comma,
    EhsICalcInputState_open,
    EhsICalcInputState_function,
    EhsICalcInputState_operator,
    EhsICalcInputState_negate,
    EhsICalcInputState_end,
    EhsICalcInputState_error
} EhsICalcInputStateEnum;


/**
 * Defines the type giving token properties. @todo separate out readonly and read-writeable values.
 */
typedef struct
{
    EhsICalcEngInstrEnum eInstruction;	/**< Equivalent calc engine instruction, if any */
    EhsICalcInputStateEnum eState;		/**< Is this token a function (e.g. "f(a,b)"), an operator (e.g "a � b") or something else?
										 (EhsICalcInputState_error used as a special value to indicate that the state is determined in the code) */
    const char* szName;			/**< String corresponding to this token. NULL means that special parsing needs to
								be implemented in EhsICalcGetToken */
    ehs_uint8 nLen;				/**< Length of string. Zero if szName is NULL. */
    ehs_uint8 nParams;			/**< Number of parameters of a function or operator. Zero if this isn't a function or operator */
    EhsDataflowIntType nConst;	/**< Constant value for a specific instance of EhsICalcToken_const. Used by operators to store properties of the operation */
    EhsInternalCalcData* pVariable; /**< Pointer to a variable for a specific instance of EhsICalcToken_input */
} EhsICalcTokenInfoType;

/**
 * Item that appears in calculation engine instruction queue
 */
typedef union
{
    EhsICalcEngInstrEnum eInstr; /**< Instruction to use */
    EhsDataflowIntType nConstVal; /**< Value of the constant (always follows EhsICalcEngInstr_Const instruction) */
    EhsDataflowIntType *pnInputVal; /**< Pointer to location of input value (always follows EhsICalcEngInstr_Input) */
} EhsICalcEngInstrType;

/**
 * Contains the calculation engine context: the instruction queue + size and the stack
 */
typedef struct
{
    EhsICalcEngInstrType* pInstrQueue;
    EhsInternalCalcData* pDataStack;
    ehs_uint16 nInstrSize;
    ehs_bool bValid;
} EhsICalcEngContext;


#endif /* EHS_FB_CALC_INT_H */


