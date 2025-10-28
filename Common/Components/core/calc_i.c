/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/* icalc.c
 *
 * Calculation function. Allows arbitrary integer calculations to be supplied
 *
 * For definition of arguments in Ncapsa functions (Identify_, Init_ and Run_)
 * please see types.h.
 *
 * @author: inx limited
 *
 */

#define EHSL_MODULE_ID EHSH_LOG_MODULE_HAL_PROCESS

#include "globals.h"
#include "ehs_fb_types.h"
#include "utils.h"

#include "messages.h"
#include "app_data.h"

#include "hal-api.h" /* Required for logging */
#include "calc_i.h"

/**
 * @page FunctionReference Lucid Function Block Reference Guide
 * @section calc_i Calculation Block (Integer)
 * @anchor calc_i
 * @subsection appliesTo Applies To:
 * Operator &gt; Calculation &gt; integer
 * @subsection properties Properties:
 * <b>Expression.</b> This is the mathematical expression you want the block to calculate.
 * Inputs are named 'A' to 'J' corresponding to the block inputs. You can use: +, -, / (divide)
 * * (multiply), % (modulus/remainder), ^ (exponent/raise to the power of).
 * Functions supported are as follows:
 * @li abs (x) return x as a positive value
 * @li square(x) return x * x
 * @li min(x,y) return the smaller of x or y
 * @li max(x,y) return the larger of x or y.
 * @li sin(mult, angle) return the sine of 'angle' multiplied by 'mult'. Angle is specified in degrees (0-360)
 * @li cos(mult, angle) return the cosine of 'angle' multiplied by 'mult'. Angle is specified in degrees (0-360)
 *
 * Examples:
 * @li A * (B + 1)
 * @li max(square(A)%17,(B^3)%17)
 *
 * @subsection description Description:
 * This function block calculates the result of an expression and sends it to the output
 * of the block.
 *
 */


/*
 * To add a new function:
 * 1. Define EhsICalcTokenInfoType for the new function type.
 * 2. Add a reference to the new instance of EhsICalcTokenInfoType to EhsICalcTokenInfo
 * 3. Add a new EhsICalcEngInstr_<function> constant (in calc_i.h)*/
/******************************************************************************/
/* Define Integer Calculation function block */

EHS_FB_FUNCTIONS_START(ICalc)

EHS_FB_FUNCTION_ENTRY("run", 0x01, ICalc)
EHS_FB_FUNCTIONS_END
EHS_FB_FUNCTIONS_START(ICalc_1)

EHS_FB_FUNCTION_ENTRY("run", 0x01, ICalc_1)
EHS_FB_FUNCTIONS_END
EHS_FB_FUNCTIONS_START(ICalc_2)

EHS_FB_FUNCTION_ENTRY("run", 0x01, ICalc_2)
EHS_FB_FUNCTIONS_END
EHS_FB_FUNCTIONS_START(ICalc_4)

EHS_FB_FUNCTION_ENTRY("run", 0x01, ICalc_4)
EHS_FB_FUNCTIONS_END
EHS_FB_FUNCTIONS_START(ICalc_6)

EHS_FB_FUNCTION_ENTRY("run", 0x01, ICalc_6)
EHS_FB_FUNCTIONS_END
EHS_FB_FUNCTIONS_START(ICalc_8)

EHS_FB_FUNCTION_ENTRY("run", 0x01, ICalc_8)
EHS_FB_FUNCTIONS_END

#define EHS_FB_ICALC_RESULT 1

#define EHS_ICALC_MAX_PAREN_DEPTH 16 /**< Maximum nesting depth of parentheses */

#define EHS_MSG_CALC_MINUSMINUS(x) "**Error: \"--\" is invalid in calculate function block. Occured before %s",x
#define EHS_MSG_CALC_INPUT(x) "**Error: Expected value (const, input id, '-'value, or function). Occurred before %s",x
#define EHS_MSG_CALC_COMMA(x) "**Error: Comma not expected here. Occurred before %s",x
#define EHS_MSG_CALC_PAREN_NEST(x) "**Error: Parenthesis nesting depth exceeded. Occurred before %s",x
#define EHS_MSG_CALC_OPERATOR(x) "**Error: Expected operator (+,-,...), close parenthesis, end of expression or comma. Occurred before %s",x
#define EHS_MSG_CALC_ARGS(x) "**Error: Arguments missing from function. Occurred before %s",x
#define EHS_MSG_CALC_OPEN_EXPECTED(x) "**Error: Expected open parenthesis after function name. Occurred before %s",x
#define EHS_MSG_CALC_CLOSE(x) "**Error: Mismatched parentheses - parens closed without an open. Occurred before %s",x
#define EHS_MSG_CALC_INTERNAL_END "**Error: Internal problem - parser called in end state"
#define EHS_MSG_CALC_PAREN_MISMATCH "**Error: Mismatched parentheses - parens left open at end of expression"
#define EHS_MSG_CALC_UNRECOGNISED(x) "**Error: Internal problem - unrecognised state. Occurred before %s",x
#define EHS_MSG_ICALC_INVALID_TOKEN(x) "**Error: Invalid token. Occurred at %s",x

#define EHS_MSG_WARNING_ICALC_ASSOCIATIVITY(x) "**Warning: Can't determine associativity of operator \"%s\"",x
#define EHS_MSG_WARNING_ICALC_CONNECTION(x) "**Warning: Input %c is used but not connected",((x)+'a'-1)

/******************************************************************************/
/* type definitions local to the integer calculation block */


#define EHS_I_CALC_TYPE_INFIX		0x0001 /**< Keyword appears between two operators */
#define EHS_I_CALC_TYPE_POSTFIX		0x0002 /**< Keyword appears after operators */
#define EHS_I_CALC_TYPE_PREFIX		0x0004 /**< Keyword appears before operators */

/**
 * Operation is associative if A o (B o C) == (A o B) o C
 * Examples: Add, multiply
 */
#define EHS_I_CALC_OP_ASSOCIATIVE 0x0100
/**
 * Operator is left associative if A o B o C == (A o B) o C
 * Examples: Subtract, divide, modulus
 */
#define EHS_I_CALC_OP_LEFT_ASSOCIATIVE 0x0200
/**
 * Operator is right associative if A o B o C = A o (B o C)
 * examples: exponentiation, open parenthesis
 */
#define EHS_I_CALC_OP_RIGHT_ASSOCIATIVE 0x0400
/**
 * Maximum precendence of an operator
 */
#define EHS_I_CALC_OP_MAX_PRECEDENCE 0x00ff
/**
 * precedence of an operator.
 */
#define EHS_I_CALC_OP_PRECEDENCE(x) ((x) & EHS_I_CALC_OP_MAX_PRECEDENCE)

/* Precedence order: highest to lowest
 * 4 function (left associative)
 * 3 negation (right associative)
 * 2 *, ^ (exp) left assoc
 * 2 / % right assoc
 * 1 + left assoc
 * 1 - left assoc
 */
/* standard tokens -- always required by the parser */
EhsICalcTokenInfoType EhsICalcToken_open[] =
{{EhsICalcEngInstr_Nop,	EhsICalcInputState_error,"(",1,0,EHS_I_CALC_OP_RIGHT_ASSOCIATIVE | EHS_I_CALC_OP_PRECEDENCE(0),NULL}};
EhsICalcTokenInfoType EhsICalcToken_close[] = {{EhsICalcEngInstr_Nop,EhsICalcInputState_error,	")",		1, 0, 0, NULL}};
EhsICalcTokenInfoType EhsICalcToken_comma[] = {{EhsICalcEngInstr_Nop,	EhsICalcInputState_error,",",		1, 0, 0, NULL}};
EhsICalcTokenInfoType EhsICalcToken_end[] = {{EhsICalcEngInstr_Nop,	EhsICalcInputState_error, NULL,		0, 0, 0, NULL}};
EhsICalcTokenInfoType EhsICalcToken_invalid[] = {{EhsICalcEngInstr_Nop,	EhsICalcInputState_error, NULL,		0, 0, 0, NULL}};
EhsICalcTokenInfoType EhsICalcToken_const[] = {{EhsICalcEngInstr_Const,	EhsICalcInputState_error, NULL,		0, 0, 0, NULL}};
EhsICalcTokenInfoType EhsICalcToken_input[] = {{EhsICalcEngInstr_Input,	EhsICalcInputState_error,NULL,		0, 0, 0, NULL}};
EhsICalcTokenInfoType EhsICalcToken_negate[] =
{{EhsICalcEngInstr_Negate,EhsICalcInputState_operator,NULL,0, 1,EHS_I_CALC_OP_RIGHT_ASSOCIATIVE | EHS_I_CALC_OP_PRECEDENCE(3),NULL}};

/* functions and operators - intended to be extended as required */
EhsICalcTokenInfoType EhsICalcToken_cos[] =
{{EhsICalcEngInstr_Cos,	EhsICalcInputState_function,"cos",3,2,EHS_I_CALC_OP_LEFT_ASSOCIATIVE | EHS_I_CALC_OP_PRECEDENCE(4), NULL}};
EhsICalcTokenInfoType EhsICalcToken_sin[] =
{{EhsICalcEngInstr_Sin,	EhsICalcInputState_function,"sin",3,2,EHS_I_CALC_OP_LEFT_ASSOCIATIVE | EHS_I_CALC_OP_PRECEDENCE(4), NULL}};
EhsICalcTokenInfoType EhsICalcToken_min[] =
{{EhsICalcEngInstr_Min,	EhsICalcInputState_function,"min",3,2,EHS_I_CALC_OP_LEFT_ASSOCIATIVE | EHS_I_CALC_OP_PRECEDENCE(4), NULL}};
EhsICalcTokenInfoType EhsICalcToken_max[] =
{{EhsICalcEngInstr_Max,	EhsICalcInputState_function,"max",3,2,EHS_I_CALC_OP_LEFT_ASSOCIATIVE | EHS_I_CALC_OP_PRECEDENCE(4), NULL}};
EhsICalcTokenInfoType EhsICalcToken_abs[] =
{{EhsICalcEngInstr_Abs,	EhsICalcInputState_function,"abs",3,1,EHS_I_CALC_OP_LEFT_ASSOCIATIVE | EHS_I_CALC_OP_PRECEDENCE(4), NULL}};
EhsICalcTokenInfoType EhsICalcToken_log10[] =
{{EhsICalcEngInstr_Log10,	EhsICalcInputState_function,"log10",5,1,EHS_I_CALC_OP_LEFT_ASSOCIATIVE | EHS_I_CALC_OP_PRECEDENCE(4), NULL}};
EhsICalcTokenInfoType EhsICalcToken_loge[] =
{{EhsICalcEngInstr_Loge,	EhsICalcInputState_function,"loge",4,1,EHS_I_CALC_OP_LEFT_ASSOCIATIVE | EHS_I_CALC_OP_PRECEDENCE(4), NULL}};
EhsICalcTokenInfoType EhsICalcToken_square[] =
{{EhsICalcEngInstr_Square,EhsICalcInputState_function,"square",6,1,EHS_I_CALC_OP_LEFT_ASSOCIATIVE | EHS_I_CALC_OP_PRECEDENCE(4), NULL}};
EhsICalcTokenInfoType EhsICalcToken_add[] =
{{EhsICalcEngInstr_Add,EhsICalcInputState_operator,"+",1,2,EHS_I_CALC_OP_ASSOCIATIVE|EHS_I_CALC_OP_PRECEDENCE(1), NULL}};
EhsICalcTokenInfoType EhsICalcToken_sub[] =
{{EhsICalcEngInstr_Sub,EhsICalcInputState_operator,"-",1,2,EHS_I_CALC_OP_LEFT_ASSOCIATIVE|EHS_I_CALC_OP_PRECEDENCE(1),NULL}};
EhsICalcTokenInfoType EhsICalcToken_mult[] =
{{EhsICalcEngInstr_Mult,EhsICalcInputState_operator,"*",1,2,EHS_I_CALC_OP_ASSOCIATIVE|EHS_I_CALC_OP_PRECEDENCE(2),NULL}};
EhsICalcTokenInfoType EhsICalcToken_div[] =
{{EhsICalcEngInstr_Div,EhsICalcInputState_operator,"/",1,2,EHS_I_CALC_OP_LEFT_ASSOCIATIVE|EHS_I_CALC_OP_PRECEDENCE(2),NULL}};
EhsICalcTokenInfoType EhsICalcToken_mod[] =
{{EhsICalcEngInstr_Mod,EhsICalcInputState_operator,"%",1,2,EHS_I_CALC_OP_LEFT_ASSOCIATIVE|EHS_I_CALC_OP_PRECEDENCE(2),NULL}};
EhsICalcTokenInfoType EhsICalcToken_exp[] =
{{EhsICalcEngInstr_Exp,EhsICalcInputState_operator,"^",1,2,EHS_I_CALC_OP_RIGHT_ASSOCIATIVE|EHS_I_CALC_OP_PRECEDENCE(2),NULL}};

/**
 * Define a table of string equivalents to keywords to support the parser
 */
EhsICalcTokenInfoType *EhsICalcTokenInfo[] =
{
    EhsICalcToken_open, /**< ( */
    EhsICalcToken_close, /**< ) */
    EhsICalcToken_add, /** < + */
    EhsICalcToken_sub, /**< - */
    EhsICalcToken_mult, /**< * */
    EhsICalcToken_div, /**<  / */
    EhsICalcToken_mod, /**< % */
    EhsICalcToken_exp, /**< ^ */
    EhsICalcToken_min, /**< min */
    EhsICalcToken_max, /**< max */
    EhsICalcToken_abs, /**< abs */
    EhsICalcToken_log10, /**< log base 10 */
    EhsICalcToken_loge, /**< naural log */
    EhsICalcToken_square, /**< square */
    EhsICalcToken_cos,
    EhsICalcToken_sin,
    EhsICalcToken_const, /**< "0" | ["1"-"9"]["0"-"9"]* */
    EhsICalcToken_input, /**< "A" | "B" | "C" | "D" | "E" | "F" | "G" | "H" | "J" | "K" */
    EhsICalcToken_comma, /**< , */
    EhsICalcToken_end, /**< End of input detected */
    EhsICalcToken_invalid, /**< invalid symbol detected */
    0
};

const char* EhsICalcGetToken(const char* szInput, EhsICalcTokenInfoType **peToken);
EhsICalcInputStateEnum EhsICalcValidateToken(EhsICalcTokenInfoType* eToken, EhsICalcInputStateEnum eState, const char* szContext);
ehs_uint16 EhsICalcCountInstr(const char* szInput);
ehs_uint32 EhsICalcWriteInstructionQueue(const char* szInput, EhsICalcEngContext* pContext);

/******************************************************************************/
/******************************************************************************/
/*
 * ICalc - DEPRECATED CODE - replaced by ICalc_1, ICalc_2, ICalc_4,ICalc_6, ICalc_8
 *
 */
/******************************************************************************/
/******************************************************************************/

/**
 * Identify the function block. Determine the memory required.
 *
 * This function provides access to:
 *  EHS_FB_IDENTIFY_PARAMETERS - string containing parameter text
 *  EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this
 *   function block's context
 *
 */
/*-------------- DEPRECATED -----------------------*/
EHS_FB_IDENTIFY_FUNCTION(ICalc)
{
    /* We need to allocate memory for the calculation engine context ...*/
    EHS_FB_IDENTIFY_MEMORY = sizeof(EhsICalcEngContext);
    return;
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function parses the parameter string to check for validity. It also
 * determines the space required to:
 * @li store the engine instructions.
 * @li hold the results stack. This value is calculated roughly, meaning that it is
 * likely to be pessimistic (too much memory assigned).
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
/*-------------- DEPRECATED -----------------------*/
EHS_FB_INIT_FUNCTION(ICalc)
{
    EhsICalcEngContext *pCtx = EHS_FB_INIT_CONTEXT;
    //int* pContents = NULL;
    ehs_bool bRet = EHS_FALSE;
    ehs_uint32 iInstr;
    ehs_sint32 nInputId; /* Which input to use - sint32 used because nConstVal holds this value for us until
						  * we're ready to fix it up correctly */
    /* determine memory required for instruction queue */
    pCtx->nInstrSize = EhsICalcCountInstr(EHS_FB_INIT_PARAMETERS);
    if (pCtx->nInstrSize > 0)
    {
        ehs_uint32 nCalcStackSize;
        pCtx->pInstrQueue = EhsHMem_writeableAlloc(pCtx->nInstrSize*sizeof(EhsICalcEngInstrType));
        nCalcStackSize = EhsICalcWriteInstructionQueue(EHS_FB_INIT_PARAMETERS, pCtx);
        if (nCalcStackSize > 0)
        {
            pCtx->pDataStack = EhsHMem_writeableAlloc(nCalcStackSize*sizeof(EhsInternalCalcData));

            /* fix up inputs */
            for (iInstr = 0; iInstr < pCtx->nInstrSize; iInstr++)
            {
                if (pCtx->pInstrQueue[iInstr].eInstr == EhsICalcEngInstr_Input)
                {
                    iInstr++;
                    nInputId = pCtx->pInstrQueue[iInstr].nConstVal;
                    if (nInputId < 1 || !EHS_FB_IN_CONNECTED(nInputId-1))
                    {
                        EHSH_LOG_WARNING(EHS_MSG_WARNING_ICALC_CONNECTION(nInputId));
                        //pCtx->pInstrQueue[iInstr].pnInputVal = 0;
                    }
                    //else
                    pCtx->pInstrQueue[iInstr].pnInputVal = (EhsDataflowIntType*)(EHS_FB_IN(nInputId-1)); /* assign to the pointer not the value */
                }
                else if (pCtx->pInstrQueue[iInstr].eInstr == EhsICalcEngInstr_Const)
                {
                    iInstr++; /* skip the constant values */
                }
            }
            bRet = EHS_TRUE;
        }
        else
            bRet = EHS_FALSE;
    }
    pCtx->bValid = bRet;
    return EHS_TRUE;
}

/**
 * Run the function calculating using the specified numerical expression.
 *
 * @todo Overflows are not detected - they simply wrap round
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 */
/*-------------- DEPRECATED -----------------------*/
EHS_FB_RUN_FUNCTION(ICalc)
{
    EhsICalcEngContext *pCtx = EHS_FB_RUN_CONTEXT;
    EhsInternalCalcData *pStack = pCtx->pDataStack;
    ehs_uint32 i;
    EhsInternalCalcData nTemp;

    if(pCtx->bValid == EHS_FALSE){
        EHSH_LOG_ERROR("Calc function is invalid.");
        return;
    }

    for (i = 0; i < pCtx->nInstrSize; i++)
    {
        switch(pCtx->pInstrQueue[i].eInstr)
        {
        case EhsICalcEngInstr_Const:
            i++;
            *(pStack++) = pCtx->pInstrQueue[i].nConstVal;
            break;
        case EhsICalcEngInstr_Input:
            i++;
            *(pStack++) = *(pCtx->pInstrQueue[i].pnInputVal);
            break;
        /* unary functions */
        case EhsICalcEngInstr_Abs:
            if (*(pStack-1) < 0) *(pStack-1) *= -1;
            break;
        case EhsICalcEngInstr_Log10:
            *(pStack-1) = EhsTgtFloat_log10(*(pStack-1));;
            break;
        case EhsICalcEngInstr_Loge:
            *(pStack-1) = EhsTgtFloat_loge(*pStack-1);
            break;
        case EhsICalcEngInstr_Negate:
            *(pStack-1) = - *(pStack-1);
            break;
        case EhsICalcEngInstr_Square:
            *(pStack-1) *= *(pStack-1);
            break;
        /* binary functions */
        case EhsICalcEngInstr_Add:
            pStack--;
            *(pStack-1) += *pStack;
            break;
        case EhsICalcEngInstr_Sub:
            pStack--;
            *(pStack-1) -= *pStack;
            break;
        case EhsICalcEngInstr_Mult:
            pStack--;
            *(pStack-1) *= *pStack;
            break;
        case EhsICalcEngInstr_Div:
            pStack--;
            if (*pStack)
                *(pStack-1) /= *pStack;
            else
            {
                *(pStack-1) = EHS_DATAFLOW_INT_MAX;
                EHSH_LOG_ERROR(EHS_MSG_ERROR_DIV0);
            }
            break;
        case EhsICalcEngInstr_Mod:
            pStack--;
            *(pStack-1) = (EhsDataflowIntType)*(pStack-1) % (EhsDataflowIntType)*pStack;
            break;
        case EhsICalcEngInstr_Cos:
            pStack--;
            pStack[-1] = EhsTgtInt_cos(pStack[-1],pStack[0]);
            break;
        case EhsICalcEngInstr_Sin:
            pStack--;
            pStack[-1] = EhsTgtInt_sin(pStack[-1],pStack[0]);
            break;
        case EhsICalcEngInstr_Exp:
            --pStack;
            if (*pStack < 0)
                *(pStack-1) = 0;
            else if (*pStack == 0)
                *(pStack-1) = 1;
            else
            {
                nTemp = *(pStack-1);
                while ((*pStack) > 1)
                {
                    *(pStack-1) *= nTemp;
                    (*pStack)--;
                }
            }
            break;
        case EhsICalcEngInstr_Min:
            pStack--;
            if (*(pStack-1) > *pStack)
                *(pStack-1) = *pStack;
            break;
        case EhsICalcEngInstr_Max:
            pStack--;
            if (*(pStack-1) < *pStack)
                *(pStack-1) = *pStack;
            break;
        default:
            break;
        }
    }

    EHS_FB_OUT_I(EHS_FB_ICALC_RESULT-1) = *(pCtx->pDataStack);
    EhsFunctionInstanceData_triggerEvent(EHS_FB_RUN_CONTEXT_REF,1);
}
/*-------------- END DEPRECATED CODE -----------------------*/

/******************************************************************************/
/*
 * ICalc_1 - 1 input version
 * @todo - code needs optimising, all versions of icalc (1,2,4,6,8 input), have same code repeated
 */
/******************************************************************************/


/**
 * Identify the function block. Determine the memory required.
 *
 * This function provides access to:
 *  EHS_FB_IDENTIFY_PARAMETERS - string containing parameter text
 *  EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this
 *   function block's context
 *
 */
EHS_FB_IDENTIFY_FUNCTION(ICalc_1)
{
    /* We need to allocate memory for the calculation engine context ...*/
    EHS_FB_IDENTIFY_MEMORY = sizeof(EhsICalcEngContext);
    return;
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function parses the parameter string to check for validity. It also
 * determines the space required to:
 * @li store the engine instructions.
 * @li hold the results stack. This value is calculated roughly, meaning that it is
 * likely to be pessimistic (too much memory assigned).
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(ICalc_1)
{
    EhsICalcEngContext *pCtx = EHS_FB_INIT_CONTEXT;
    //int* pContents = NULL;
    ehs_bool bRet = EHS_FALSE;
    ehs_uint32 iInstr;
    ehs_sint32 nInputId; /* Which input to use - sint32 used because nConstVal holds this value for us until
						  * we're ready to fix it up correctly */
    /* determine memory required for instruction queue */
    pCtx->nInstrSize = EhsICalcCountInstr(EHS_FB_INIT_PARAMETERS);
    if (pCtx->nInstrSize > 0)
    {
        ehs_uint32 nCalcStackSize;
        pCtx->pInstrQueue = EhsHMem_writeableAlloc(pCtx->nInstrSize*sizeof(EhsICalcEngInstrType));
        nCalcStackSize = EhsICalcWriteInstructionQueue(EHS_FB_INIT_PARAMETERS, pCtx);
        if (nCalcStackSize > 0)
        {
            pCtx->pDataStack = EhsHMem_writeableAlloc(nCalcStackSize*sizeof(EhsInternalCalcData));

            /* fix up inputs */
            for (iInstr = 0; iInstr < pCtx->nInstrSize; iInstr++)
            {
                if (pCtx->pInstrQueue[iInstr].eInstr == EhsICalcEngInstr_Input)
                {
                    iInstr++;
                    nInputId = pCtx->pInstrQueue[iInstr].nConstVal;
                    if (nInputId < 1 || !EHS_FB_IN_CONNECTED(nInputId-1))
                    {
                        EHSH_LOG_WARNING(EHS_MSG_WARNING_ICALC_CONNECTION(nInputId));
                        //pCtx->pInstrQueue[iInstr].pnInputVal = 0;
                    }
                    //else
                    pCtx->pInstrQueue[iInstr].pnInputVal = (EhsDataflowIntType*)EHS_FB_IN(nInputId-1); /* assign to the pointer not the value */
                }
                else if (pCtx->pInstrQueue[iInstr].eInstr == EhsICalcEngInstr_Const)
                {
                    iInstr++; /* skip the constant values */
                }
            }
            bRet = EHS_TRUE;
        }
        else
            bRet = EHS_FALSE;
    }
    pCtx->bValid = bRet;
    return EHS_TRUE;
}

/**
 * Run the function calculating using the specified numerical expression.
 *
 * @todo Overflows are not detected - they simply wrap round
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ICalc_1)
{
    EhsICalcEngContext *pCtx = EHS_FB_RUN_CONTEXT;
    EhsInternalCalcData *pStack = pCtx->pDataStack;
    ehs_uint32 i;
    EhsInternalCalcData nTemp;

    if(pCtx->bValid == EHS_FALSE){
        EHSH_LOG_ERROR("Calc function is invalid.");
        return;
    }

    for (i = 0; i < pCtx->nInstrSize; i++)
    {
        switch(pCtx->pInstrQueue[i].eInstr)
        {
        case EhsICalcEngInstr_Const:
            i++;
            *(pStack++) = pCtx->pInstrQueue[i].nConstVal;
            break;
        case EhsICalcEngInstr_Input:
            i++;
            *(pStack++) = *(pCtx->pInstrQueue[i].pnInputVal);
            break;
        /* unary functions */
        case EhsICalcEngInstr_Abs:
            if (*(pStack-1) < 0) *(pStack-1) *= -1;
            break;
        case EhsICalcEngInstr_Log10:
            if (*(pStack-1) < 0) *(pStack-1) = EhsTgtFloat_log10(pStack[-1]);;
            break;
        case EhsICalcEngInstr_Loge:
            if (*(pStack-1) < 0) *(pStack-1) = EhsTgtFloat_loge(pStack[-1]);
            break;
        case EhsICalcEngInstr_Negate:
            *(pStack-1) = - *(pStack-1);
            break;
        case EhsICalcEngInstr_Square:
            *(pStack-1) *= *(pStack-1);
            break;
        /* binary functions */
        case EhsICalcEngInstr_Add:
            pStack--;
            *(pStack-1) += *pStack;
            break;
        case EhsICalcEngInstr_Sub:
            pStack--;
            *(pStack-1) -= *pStack;
            break;
        case EhsICalcEngInstr_Mult:
            pStack--;
            *(pStack-1) *= *pStack;
            break;
        case EhsICalcEngInstr_Div:
            pStack--;
            if (*pStack)
                *(pStack-1) /= *pStack;
            else
            {
                *(pStack-1) = EHS_DATAFLOW_INT_MAX;
                EHSH_LOG_ERROR(EHS_MSG_ERROR_DIV0);
            }
            break;
        case EhsICalcEngInstr_Mod:
            pStack--;
            *(pStack-1) = (EhsDataflowIntType)*(pStack-1) % (EhsDataflowIntType)*pStack;
            break;
        case EhsICalcEngInstr_Cos:
            pStack--;
            pStack[-1] = EhsTgtInt_cos(pStack[-1],pStack[0]);
            break;
        case EhsICalcEngInstr_Sin:
            pStack--;
            pStack[-1] = EhsTgtInt_sin(pStack[-1],pStack[0]);
            break;
        case EhsICalcEngInstr_Exp:
            --pStack;
            if (*pStack < 0)
                *(pStack-1) = 0;
            else if (*pStack == 0)
                *(pStack-1) = 1;
            else
            {
                nTemp = *(pStack-1);
                while ((*pStack) > 1)
                {
                    *(pStack-1) *= nTemp;
                    (*pStack)--;
                }
            }
            break;
        case EhsICalcEngInstr_Min:
            pStack--;
            if (*(pStack-1) > *pStack)
                *(pStack-1) = *pStack;
            break;
        case EhsICalcEngInstr_Max:
            pStack--;
            if (*(pStack-1) < *pStack)
                *(pStack-1) = *pStack;
            break;
        default:
            break;
        }
    }

    EHS_FB_OUT_I(EHS_FB_ICALC_RESULT-1) = *(pCtx->pDataStack);
    EhsFunctionInstanceData_triggerEvent(EHS_FB_RUN_CONTEXT_REF,1);
}



/******************************************************************************/
/*
 * ICalc_2 - 2 input version
 * @todo - code needs optimising, all versions of icalc (1,2,4,6,8 input), have same code repeated
 */
/******************************************************************************/


/**
 * Identify the function block. Determine the memory required.
 *
 * This function provides access to:
 *  EHS_FB_IDENTIFY_PARAMETERS - string containing parameter text
 *  EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this
 *   function block's context
 *
 */
EHS_FB_IDENTIFY_FUNCTION(ICalc_2)
{
    /* We need to allocate memory for the calculation engine context ...*/
    EHS_FB_IDENTIFY_MEMORY = sizeof(EhsICalcEngContext);
    return;
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function parses the parameter string to check for validity. It also
 * determines the space required to:
 * @li store the engine instructions.
 * @li hold the results stack. This value is calculated roughly, meaning that it is
 * likely to be pessimistic (too much memory assigned).
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(ICalc_2)
{
    EhsICalcEngContext *pCtx = EHS_FB_INIT_CONTEXT;
    //int* pContents = NULL;
    ehs_bool bRet = EHS_FALSE;
    ehs_uint32 iInstr;
    ehs_sint32 nInputId; /* Which input to use - sint32 used because nConstVal holds this value for us until
						  * we're ready to fix it up correctly */
    /* determine memory required for instruction queue */
    pCtx->nInstrSize = EhsICalcCountInstr(EHS_FB_INIT_PARAMETERS);
    if (pCtx->nInstrSize > 0)
    {
        ehs_uint32 nCalcStackSize;
        pCtx->pInstrQueue = EhsHMem_writeableAlloc(pCtx->nInstrSize*sizeof(EhsICalcEngInstrType));
        nCalcStackSize = EhsICalcWriteInstructionQueue(EHS_FB_INIT_PARAMETERS, pCtx);
        if (nCalcStackSize > 0)
        {
            pCtx->pDataStack = EhsHMem_writeableAlloc(nCalcStackSize*sizeof(EhsInternalCalcData));

            /* fix up inputs */
            for (iInstr = 0; iInstr < pCtx->nInstrSize; iInstr++)
            {
                if (pCtx->pInstrQueue[iInstr].eInstr == EhsICalcEngInstr_Input)
                {
                    iInstr++;
                    nInputId = pCtx->pInstrQueue[iInstr].nConstVal;
                    if (nInputId < 1 || !EHS_FB_IN_CONNECTED(nInputId-1))
                    {
                        EHSH_LOG_WARNING(EHS_MSG_WARNING_ICALC_CONNECTION(nInputId));
                        //pCtx->pInstrQueue[iInstr].pnInputVal = 0;
                    }
                    //else
                    pCtx->pInstrQueue[iInstr].pnInputVal = (EhsDataflowIntType*)EHS_FB_IN(nInputId-1); /* assign to the pointer not the value */
                }
                else if (pCtx->pInstrQueue[iInstr].eInstr == EhsICalcEngInstr_Const)
                {
                    iInstr++; /* skip the constant values */
                }
            }
            bRet = EHS_TRUE;
        }
        else
            bRet = EHS_FALSE;
    }
    pCtx->bValid = bRet;
    return EHS_TRUE;
}

/**
 * Run the function calculating using the specified numerical expression.
 *
 * @todo Overflows are not detected - they simply wrap round
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ICalc_2)
{
    EhsICalcEngContext *pCtx = EHS_FB_RUN_CONTEXT;
    EhsInternalCalcData *pStack = pCtx->pDataStack;
    ehs_uint32 i;
    EhsInternalCalcData nTemp;

    if(pCtx->bValid == EHS_FALSE){
        EHSH_LOG_ERROR("Calc function is invalid.");
        return;
    }

    for (i = 0; i < pCtx->nInstrSize; i++)
    {
        switch(pCtx->pInstrQueue[i].eInstr)
        {
        case EhsICalcEngInstr_Const:
            i++;
            *(pStack++) = pCtx->pInstrQueue[i].nConstVal;
            break;
        case EhsICalcEngInstr_Input:
            i++;
            *(pStack++) = *(pCtx->pInstrQueue[i].pnInputVal);
            break;
        /* unary functions */
        case EhsICalcEngInstr_Abs:
            if (*(pStack-1) < 0) *(pStack-1) *= -1;
            break;
        case EhsICalcEngInstr_Log10:
            if (*(pStack-1) < 0) *(pStack-1) = EhsTgtFloat_log10(pStack[-1]);;
            break;
        case EhsICalcEngInstr_Loge:
            if (*(pStack-1) < 0) *(pStack-1) = EhsTgtFloat_loge(pStack[-1]);
            break;
        case EhsICalcEngInstr_Negate:
            *(pStack-1) = - *(pStack-1);
            break;
        case EhsICalcEngInstr_Square:
            *(pStack-1) *= *(pStack-1);
            break;
        /* binary functions */
        case EhsICalcEngInstr_Add:
            pStack--;
            *(pStack-1) += *pStack;
            break;
        case EhsICalcEngInstr_Sub:
            pStack--;
            *(pStack-1) -= *pStack;
            break;
        case EhsICalcEngInstr_Mult:
            pStack--;
            *(pStack-1) *= *pStack;
            break;
        case EhsICalcEngInstr_Div:
            pStack--;
            if (*pStack)
                *(pStack-1) /= *pStack;
            else
            {
                *(pStack-1) = EHS_DATAFLOW_INT_MAX;
                EHSH_LOG_ERROR(EHS_MSG_ERROR_DIV0);
            }
            break;
        case EhsICalcEngInstr_Mod:
            pStack--;
            *(pStack-1) = (EhsDataflowIntType)*(pStack-1) % (EhsDataflowIntType)*pStack;
            break;
        case EhsICalcEngInstr_Cos:
            pStack--;
            pStack[-1] = EhsTgtInt_cos(pStack[-1],pStack[0]);
            break;
        case EhsICalcEngInstr_Sin:
            pStack--;
            pStack[-1] = EhsTgtInt_sin(pStack[-1],pStack[0]);
            break;
        case EhsICalcEngInstr_Exp:
            --pStack;
            if (*pStack < 0)
                *(pStack-1) = 0;
            else if (*pStack == 0)
                *(pStack-1) = 1;
            else
            {
                nTemp = *(pStack-1);
                while ((*pStack) > 1)
                {
                    *(pStack-1) *= nTemp;
                    (*pStack)--;
                }
            }
            break;
        case EhsICalcEngInstr_Min:
            pStack--;
            if (*(pStack-1) > *pStack)
                *(pStack-1) = *pStack;
            break;
        case EhsICalcEngInstr_Max:
            pStack--;
            if (*(pStack-1) < *pStack)
                *(pStack-1) = *pStack;
            break;
        default:
            break;
        }
    }

    EHS_FB_OUT_I(EHS_FB_ICALC_RESULT-1) = *(pCtx->pDataStack);
    EhsFunctionInstanceData_triggerEvent(EHS_FB_RUN_CONTEXT_REF,1);
}



/******************************************************************************/
/*
 * ICalc_4 - 4 input version
 * @todo - code needs optimising, all versions of icalc (1,2,4,6,8 input), have same code repeated
 */
/******************************************************************************/


/**
 * Identify the function block. Determine the memory required.
 *
 * This function provides access to:
 *  EHS_FB_IDENTIFY_PARAMETERS - string containing parameter text
 *  EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this
 *   function block's context
 *
 */
EHS_FB_IDENTIFY_FUNCTION(ICalc_4)
{
    /* We need to allocate memory for the calculation engine context ...*/
    EHS_FB_IDENTIFY_MEMORY = sizeof(EhsICalcEngContext);
    return;
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function parses the parameter string to check for validity. It also
 * determines the space required to:
 * @li store the engine instructions.
 * @li hold the results stack. This value is calculated roughly, meaning that it is
 * likely to be pessimistic (too much memory assigned).
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(ICalc_4)
{
    EhsICalcEngContext *pCtx = EHS_FB_INIT_CONTEXT;
    //int* pContents = NULL;
    ehs_bool bRet = EHS_FALSE;
    ehs_uint32 iInstr;
    ehs_sint32 nInputId; /* Which input to use - sint32 used because nConstVal holds this value for us until
						  * we're ready to fix it up correctly */
    /* determine memory required for instruction queue */
    pCtx->nInstrSize = EhsICalcCountInstr(EHS_FB_INIT_PARAMETERS);
    if (pCtx->nInstrSize > 0)
    {
        ehs_uint32 nCalcStackSize;
        pCtx->pInstrQueue = EhsHMem_writeableAlloc(pCtx->nInstrSize*sizeof(EhsICalcEngInstrType));
        nCalcStackSize = EhsICalcWriteInstructionQueue(EHS_FB_INIT_PARAMETERS, pCtx);
        if (nCalcStackSize > 0)
        {
            pCtx->pDataStack = EhsHMem_writeableAlloc(nCalcStackSize*sizeof(EhsInternalCalcData));

            /* fix up inputs */
            for (iInstr = 0; iInstr < pCtx->nInstrSize; iInstr++)
            {
                if (pCtx->pInstrQueue[iInstr].eInstr == EhsICalcEngInstr_Input)
                {
                    iInstr++;
                    nInputId = pCtx->pInstrQueue[iInstr].nConstVal;
                    if (nInputId < 1 || !EHS_FB_IN_CONNECTED(nInputId-1))
                    {
                        EHSH_LOG_WARNING(EHS_MSG_WARNING_ICALC_CONNECTION(nInputId));
                        //pCtx->pInstrQueue[iInstr].pnInputVal = 0;
                    }
                    //else
                    pCtx->pInstrQueue[iInstr].pnInputVal = (EhsDataflowIntType*)EHS_FB_IN(nInputId-1); /* assign to the pointer not the value */
                }
                else if (pCtx->pInstrQueue[iInstr].eInstr == EhsICalcEngInstr_Const)
                {
                    iInstr++; /* skip the constant values */
                }
            }
            bRet = EHS_TRUE;
        }
        else
            bRet = EHS_FALSE;
    }
    pCtx->bValid = bRet;
    return EHS_TRUE;
}

/**
 * Run the function calculating using the specified numerical expression.
 *
 * @todo Overflows are not detected - they simply wrap round
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ICalc_4)
{
    EhsICalcEngContext *pCtx = EHS_FB_RUN_CONTEXT;
    EhsInternalCalcData *pStack = pCtx->pDataStack;
    ehs_uint32 i;
    EhsInternalCalcData nTemp;

    if(pCtx->bValid == EHS_FALSE){
        EHSH_LOG_ERROR("Calc function is invalid.");
        return;
    }

    for (i = 0; i < pCtx->nInstrSize; i++)
    {
        switch(pCtx->pInstrQueue[i].eInstr)
        {
        case EhsICalcEngInstr_Const:
            i++;
            *(pStack++) = pCtx->pInstrQueue[i].nConstVal;
            break;
        case EhsICalcEngInstr_Input:
            i++;
            *(pStack++) = *(pCtx->pInstrQueue[i].pnInputVal);
            break;
        /* unary functions */
        case EhsICalcEngInstr_Abs:
            if (*(pStack-1) < 0) *(pStack-1) *= -1;
            break;
        case EhsICalcEngInstr_Log10:
            if (*(pStack-1) < 0) *(pStack-1) = EhsTgtFloat_log10(pStack[-1]);;
            break;
        case EhsICalcEngInstr_Loge:
            if (*(pStack-1) < 0) *(pStack-1) = EhsTgtFloat_loge(pStack[-1]);
            break;
        case EhsICalcEngInstr_Negate:
            *(pStack-1) = - *(pStack-1);
            break;
        case EhsICalcEngInstr_Square:
            *(pStack-1) *= *(pStack-1);
            break;
        /* binary functions */
        case EhsICalcEngInstr_Add:
            pStack--;
            *(pStack-1) += *pStack;
            break;
        case EhsICalcEngInstr_Sub:
            pStack--;
            *(pStack-1) -= *pStack;
            break;
        case EhsICalcEngInstr_Mult:
            pStack--;
            *(pStack-1) *= *pStack;
            break;
        case EhsICalcEngInstr_Div:
            pStack--;
            if (*pStack)
                *(pStack-1) /= *pStack; /* Still seem to get a fiv 0 problem */
            else
            {
                *(pStack-1) = EHS_DATAFLOW_INT_MAX;
                EHSH_LOG_ERROR(EHS_MSG_ERROR_DIV0);
            }
            break;
        case EhsICalcEngInstr_Mod:
            pStack--;
            *(pStack-1) = (EhsDataflowIntType)*(pStack-1) % (EhsDataflowIntType)*pStack;
            break;
        case EhsICalcEngInstr_Cos:
            pStack--;
            pStack[-1] = EhsTgtInt_cos(pStack[-1],pStack[0]);
            break;
        case EhsICalcEngInstr_Sin:
            pStack--;
            pStack[-1] = EhsTgtInt_sin(pStack[-1],pStack[0]);
            break;
        case EhsICalcEngInstr_Exp:
            --pStack;
            if (*pStack < 0)
                *(pStack-1) = 0;
            else if (*pStack == 0)
                *(pStack-1) = 1;
            else
            {
                nTemp = *(pStack-1);
                while ((*pStack) > 1)
                {
                    *(pStack-1) *= nTemp;
                    (*pStack)--;
                }
            }
            break;
        case EhsICalcEngInstr_Min:
            pStack--;
            if (*(pStack-1) > *pStack)
                *(pStack-1) = *pStack;
            break;
        case EhsICalcEngInstr_Max:
            pStack--;
            if (*(pStack-1) < *pStack)
                *(pStack-1) = *pStack;
            break;
        default:
            break;
        }
    }

    EHS_FB_OUT_I(EHS_FB_ICALC_RESULT-1) = *(pCtx->pDataStack);
    EhsFunctionInstanceData_triggerEvent(EHS_FB_RUN_CONTEXT_REF,1);
}



/******************************************************************************/
/*
 * ICalc_6 - 6 input version
 * @todo - code needs optimising, all versions of icalc (1,2,4,6,8 input), have same code repeated
 */
/******************************************************************************/


/**
 * Identify the function block. Determine the memory required.
 *
 * This function provides access to:
 *  EHS_FB_IDENTIFY_PARAMETERS - string containing parameter text
 *  EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this
 *   function block's context
 *
 */
EHS_FB_IDENTIFY_FUNCTION(ICalc_6)
{
    /* We need to allocate memory for the calculation engine context ...*/
    EHS_FB_IDENTIFY_MEMORY = sizeof(EhsICalcEngContext);
    return;
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function parses the parameter string to check for validity. It also
 * determines the space required to:
 * @li store the engine instructions.
 * @li hold the results stack. This value is calculated roughly, meaning that it is
 * likely to be pessimistic (too much memory assigned).
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(ICalc_6)
{
    EhsICalcEngContext *pCtx = EHS_FB_INIT_CONTEXT;
    //int* pContents = NULL;
    ehs_bool bRet = EHS_FALSE;
    ehs_uint32 iInstr;
    ehs_sint32 nInputId; /* Which input to use - sint32 used because nConstVal holds this value for us until
						  * we're ready to fix it up correctly */
    /* determine memory required for instruction queue */
    pCtx->nInstrSize = EhsICalcCountInstr(EHS_FB_INIT_PARAMETERS);
    if (pCtx->nInstrSize > 0)
    {
        ehs_uint32 nCalcStackSize;
        pCtx->pInstrQueue = EhsHMem_writeableAlloc(pCtx->nInstrSize*sizeof(EhsICalcEngInstrType));
        nCalcStackSize = EhsICalcWriteInstructionQueue(EHS_FB_INIT_PARAMETERS, pCtx);
        if (nCalcStackSize > 0)
        {
            pCtx->pDataStack = EhsHMem_writeableAlloc(nCalcStackSize*sizeof(EhsInternalCalcData));

            /* fix up inputs */
            for (iInstr = 0; iInstr < pCtx->nInstrSize; iInstr++)
            {
                if (pCtx->pInstrQueue[iInstr].eInstr == EhsICalcEngInstr_Input)
                {
                    iInstr++;
                    nInputId = pCtx->pInstrQueue[iInstr].nConstVal;
                    if (nInputId < 1 || !EHS_FB_IN_CONNECTED(nInputId-1))
                    {
                        EHSH_LOG_WARNING(EHS_MSG_WARNING_ICALC_CONNECTION(nInputId));
                        //pCtx->pInstrQueue[iInstr].pnInputVal = 0;
                    }
                    //else
                    pCtx->pInstrQueue[iInstr].pnInputVal = (EhsDataflowIntType*)EHS_FB_IN(nInputId-1); /* assign to the pointer not the value */
                }
                else if (pCtx->pInstrQueue[iInstr].eInstr == EhsICalcEngInstr_Const)
                {
                    iInstr++; /* skip the constant values */
                }
            }
            bRet = EHS_TRUE;
        }
        else
            bRet = EHS_FALSE;
    }
    pCtx->bValid = bRet;
    return EHS_TRUE;
}

/**
 * Run the function calculating using the specified numerical expression.
 *
 * @todo Overflows are not detected - they simply wrap round
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ICalc_6)
{
    EhsICalcEngContext *pCtx = EHS_FB_RUN_CONTEXT;
    EhsInternalCalcData *pStack = pCtx->pDataStack;
    ehs_uint32 i;
    EhsInternalCalcData nTemp;

    if(pCtx->bValid == EHS_FALSE){
        EHSH_LOG_ERROR("Calc function is invalid.");
        return;
    }

    for (i = 0; i < pCtx->nInstrSize; i++)
    {
        switch(pCtx->pInstrQueue[i].eInstr)
        {
        case EhsICalcEngInstr_Const:
            i++;
            *(pStack++) = pCtx->pInstrQueue[i].nConstVal;
            break;
        case EhsICalcEngInstr_Input:
            i++;
            *(pStack++) = *(pCtx->pInstrQueue[i].pnInputVal);
            break;
        /* unary functions */
        case EhsICalcEngInstr_Abs:
            if (*(pStack-1) < 0) *(pStack-1) *= -1;
            break;
        case EhsICalcEngInstr_Log10:
            if (*(pStack-1) < 0) *(pStack-1) = EhsTgtFloat_log10(pStack[-1]);;
            break;
        case EhsICalcEngInstr_Loge:
            if (*(pStack-1) < 0) *(pStack-1) = EhsTgtFloat_loge(pStack[-1]);
            break;
        case EhsICalcEngInstr_Negate:
            *(pStack-1) = - *(pStack-1);
            break;
        case EhsICalcEngInstr_Square:
            *(pStack-1) *= *(pStack-1);
            break;
        /* binary functions */
        case EhsICalcEngInstr_Add:
            pStack--;
            *(pStack-1) += *pStack;
            break;
        case EhsICalcEngInstr_Sub:
            pStack--;
            *(pStack-1) -= *pStack;
            break;
        case EhsICalcEngInstr_Mult:
            pStack--;
            *(pStack-1) *= *pStack;
            break;
        case EhsICalcEngInstr_Div:
            pStack--;
            if (*pStack)
                *(pStack-1) /= *pStack;
            else
            {
                *(pStack-1) = EHS_DATAFLOW_INT_MAX;
                EHSH_LOG_ERROR(EHS_MSG_ERROR_DIV0);
            }
            break;
        case EhsICalcEngInstr_Mod:
            pStack--;
            *(pStack-1) = (EhsDataflowIntType)*(pStack-1) % (EhsDataflowIntType)*pStack;
            break;
        case EhsICalcEngInstr_Cos:
            pStack--;
            pStack[-1] = EhsTgtInt_cos(pStack[-1],pStack[0]);
            break;
        case EhsICalcEngInstr_Sin:
            pStack--;
            pStack[-1] = EhsTgtInt_sin(pStack[-1],pStack[0]);
            break;
        case EhsICalcEngInstr_Exp:
            --pStack;
            if (*pStack < 0)
                *(pStack-1) = 0;
            else if (*pStack == 0)
                *(pStack-1) = 1;
            else
            {
                nTemp = *(pStack-1);
                while ((*pStack) > 1)
                {
                    *(pStack-1) *= nTemp;
                    (*pStack)--;
                }
            }
            break;
        case EhsICalcEngInstr_Min:
            pStack--;
            if (*(pStack-1) > *pStack)
                *(pStack-1) = *pStack;
            break;
        case EhsICalcEngInstr_Max:
            pStack--;
            if (*(pStack-1) < *pStack)
                *(pStack-1) = *pStack;
            break;
        default:
            break;
        }
    }

    EHS_FB_OUT_I(EHS_FB_ICALC_RESULT-1) = *(pCtx->pDataStack);
    EhsFunctionInstanceData_triggerEvent(EHS_FB_RUN_CONTEXT_REF,1);
}



/******************************************************************************/
/*
 * ICalc_8 - 8 input version
 * @todo - code needs optimising, all versions of icalc (1,2,4,6,8 input), have same code repeated
 */
/******************************************************************************/


/**
 * Identify the function block. Determine the memory required.
 *
 * This function provides access to:
 *  EHS_FB_IDENTIFY_PARAMETERS - string containing parameter text
 *  EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this
 *   function block's context
 *
 */
EHS_FB_IDENTIFY_FUNCTION(ICalc_8)
{
    /* We need to allocate memory for the calculation engine context ...*/
    EHS_FB_IDENTIFY_MEMORY = sizeof(EhsICalcEngContext);
    return;
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function parses the parameter string to check for validity. It also
 * determines the space required to:
 * @li store the engine instructions.
 * @li hold the results stack. This value is calculated roughly, meaning that it is
 * likely to be pessimistic (too much memory assigned).
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(ICalc_8)
{
    EhsICalcEngContext *pCtx = EHS_FB_INIT_CONTEXT;
    //int* pContents = NULL;
    ehs_bool bRet = EHS_FALSE;
    ehs_uint32 iInstr;
    ehs_sint32 nInputId; /* Which input to use - sint32 used because nConstVal holds this value for us until
						  * we're ready to fix it up correctly */
    /* determine memory required for instruction queue */
    pCtx->nInstrSize = EhsICalcCountInstr(EHS_FB_INIT_PARAMETERS);
    if (pCtx->nInstrSize > 0)
    {
        ehs_uint32 nCalcStackSize;
        pCtx->pInstrQueue = EhsHMem_writeableAlloc(pCtx->nInstrSize*sizeof(EhsICalcEngInstrType));
        nCalcStackSize = EhsICalcWriteInstructionQueue(EHS_FB_INIT_PARAMETERS, pCtx);
        if (nCalcStackSize > 0)
        {
            pCtx->pDataStack = EhsHMem_writeableAlloc(nCalcStackSize*sizeof(EhsInternalCalcData));

            /* fix up inputs */
            for (iInstr = 0; iInstr < pCtx->nInstrSize; iInstr++)
            {
                if (pCtx->pInstrQueue[iInstr].eInstr == EhsICalcEngInstr_Input)
                {
                    iInstr++;
                    nInputId = pCtx->pInstrQueue[iInstr].nConstVal;
                    if (nInputId < 1 || !EHS_FB_IN_CONNECTED(nInputId-1))
                    {
                        EHSH_LOG_WARNING(EHS_MSG_WARNING_ICALC_CONNECTION(nInputId));
                        //pCtx->pInstrQueue[iInstr].pnInputVal = 0;
                    }
                    //else
                    pCtx->pInstrQueue[iInstr].pnInputVal = (EhsDataflowIntType*)EHS_FB_IN(nInputId-1); /* assign to the pointer not the value */
                }
                else if (pCtx->pInstrQueue[iInstr].eInstr == EhsICalcEngInstr_Const)
                {
                    iInstr++; /* skip the constant values */
                }
            }
            bRet = EHS_TRUE;
        }
        else
            bRet = EHS_FALSE;
    }
    pCtx->bValid = bRet;
    return EHS_TRUE;
}

/**
 * Run the function calculating using the specified numerical expression.
 *
 * @todo Overflows are not detected - they simply wrap round
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ICalc_8)
{
    EhsICalcEngContext *pCtx = EHS_FB_RUN_CONTEXT;
    EhsInternalCalcData *pStack = pCtx->pDataStack;
    ehs_uint32 i;
    EhsInternalCalcData nTemp;

    if(pCtx->bValid == EHS_FALSE){
        EHSH_LOG_ERROR("Calc function is invalid.");
        return;
    }

    for (i = 0; i < pCtx->nInstrSize; i++)
    {
        switch(pCtx->pInstrQueue[i].eInstr)
        {
        case EhsICalcEngInstr_Const:
            i++;
            *(pStack++) = pCtx->pInstrQueue[i].nConstVal;
            break;
        case EhsICalcEngInstr_Input:
            i++;
            *(pStack++) = *(pCtx->pInstrQueue[i].pnInputVal);
            break;
        /* unary functions */
        case EhsICalcEngInstr_Abs:
            if (*(pStack-1) < 0) *(pStack-1) *= -1;
            break;
        case EhsICalcEngInstr_Log10:
            if (*(pStack-1) < 0) *(pStack-1) = EhsTgtFloat_log10(pStack[-1]);;
            break;
        case EhsICalcEngInstr_Loge:
            if (*(pStack-1) < 0) *(pStack-1) = EhsTgtFloat_loge(pStack[-1]);
            break;
        case EhsICalcEngInstr_Negate:
            *(pStack-1) = - *(pStack-1);
            break;
        case EhsICalcEngInstr_Square:
            *(pStack-1) *= *(pStack-1);
            break;
        /* binary functions */
        case EhsICalcEngInstr_Add:
            pStack--;
            *(pStack-1) += *pStack;
            break;
        case EhsICalcEngInstr_Sub:
            pStack--;
            *(pStack-1) -= *pStack;
            break;
        case EhsICalcEngInstr_Mult:
            pStack--;
            *(pStack-1) *= *pStack;
            break;
        case EhsICalcEngInstr_Div:
            pStack--;
            if (*pStack)
                *(pStack-1) /= *pStack;
            else
            {
                *(pStack-1) = EHS_DATAFLOW_INT_MAX;
                EHSH_LOG_ERROR(EHS_MSG_ERROR_DIV0);
            }
            break;
        case EhsICalcEngInstr_Mod:
            pStack--;
            *(pStack-1) = (EhsDataflowIntType)*(pStack-1) % (EhsDataflowIntType)*pStack;
            break;
        case EhsICalcEngInstr_Cos:
            pStack--;
            pStack[-1] = EhsTgtInt_cos(pStack[-1],pStack[0]);
            break;
        case EhsICalcEngInstr_Sin:
            pStack--;
            pStack[-1] = EhsTgtInt_sin(pStack[-1],pStack[0]);
            break;
        case EhsICalcEngInstr_Exp:
            --pStack;
            if (*pStack < 0)
                *(pStack-1) = 0;
            else if (*pStack == 0)
                *(pStack-1) = 1;
            else
            {
                nTemp = *(pStack-1);
                while ((*pStack) > 1)
                {
                    *(pStack-1) *= nTemp;
                    (*pStack)--;
                }
            }
            break;
        case EhsICalcEngInstr_Min:
            pStack--;
            if (*(pStack-1) > *pStack)
                *(pStack-1) = *pStack;
            break;
        case EhsICalcEngInstr_Max:
            pStack--;
            if (*(pStack-1) < *pStack)
                *(pStack-1) = *pStack;
            break;
        default:
            break;
        }
    }

    EHS_FB_OUT_I(EHS_FB_ICALC_RESULT-1) = *(pCtx->pDataStack);
    EhsFunctionInstanceData_triggerEvent(EHS_FB_RUN_CONTEXT_REF,1);
}



/******************************************************************************/
/* Integer Calculation parsing code */

/**
 * Get the next token from the input string
 *
 * @param szInput input string to read the token from
 * @param piVal output containing constant value (if any)
 * @param pcInputId output containing input id (if any)
 * @param peToken output containing the next token
 * @return pointer to string after token has been read
 */
const char* EhsICalcGetToken(const char* szInput, EhsICalcTokenInfoType **peToken)
{
    EhsICalcTokenInfoType **pKeywordTable = EhsICalcTokenInfo;
    szInput = EhsStrTrimL(szInput); /* consume leading spaces */
    *peToken = EhsICalcToken_invalid; /* assume we don't recognise token */

    /* find keyword in table? */
    while (!EHS_TABLE_END(pKeywordTable))
    {
        if (((*pKeywordTable)->szName) &&
                (0 == EhsStrnicmp((*pKeywordTable)->szName, szInput, (*pKeywordTable)->nLen)))
        {
            *peToken = *pKeywordTable;
            szInput += (*peToken)->nLen;
            break;
        }
        pKeywordTable++;
    }
    /* keyword not found. Is it a special token (constant, variable or end of string)? */
    if (EhsICalcToken_invalid == *peToken)
    {
        if ('\0' == *szInput)
        {
            *peToken = EhsICalcToken_end;
        }
        else if (strchr("ABCDEFGHIJKabcdefghijk",*szInput))
        {

            *peToken = EhsICalcToken_input;
            /* Convert variable into index (starting at 1) and store variable index in nConst for now.
             * This gets fixed up after the parsing is complete */
            (*peToken)->nConst = 1 + (EhsDataflowIntType)EHS_TO_LOWER(*szInput) - 'a';
            szInput++;
        }
        else if (isdigit(*szInput))
        {
            *peToken = EhsICalcToken_const;
            szInput = EhsGetSint32FromString(&((*peToken)->nConst),szInput);
        }
        else
        {
            szInput--; /* rollback to show problem input during error message */
            EHSH_LOG_ERROR(EHS_MSG_ICALC_INVALID_TOKEN(szInput));
        }
    }
    return szInput;
}

/**
 * Determine the next state, given that we are currently in a state
 * that is expecting to see a value next (constant, open bracket, input id
 * or function name)
 *
 * @param eToken Type of input just read
 * @param eState Current state of the parser
 * @param szContext remainder of input string (used in error messages)
 * @return next state resulting from this
 */
EhsICalcInputStateEnum EhsICalcValidateValueNext(EhsICalcTokenInfoType* eToken, EhsICalcInputStateEnum eState, const char* szContext)
{
    EhsICalcInputStateEnum ret;

    if (EhsICalcInputState_function == eToken->eState)
    {
        ret = EhsICalcInputState_function;
    }
    else if (EhsICalcToken_open == eToken)     /* ( */
    {
        ret = EhsICalcInputState_open;
    }
    else if (EhsICalcToken_sub == eToken)    /* - This is a special case for an operator. When it appears before a value, it's a negation function */
    {
        if (eState == EhsICalcInputState_negate)
        {
            ret = EhsICalcInputState_error;
            EHSH_LOG_ERROR(EHS_MSG_CALC_MINUSMINUS(szContext));
        }
        else
        {
            ret = EhsICalcInputState_negate;
        }
    }
    else if ((EhsICalcToken_const == eToken) || (EhsICalcToken_input == eToken))
    {
        /* "0" | ["1"-"9"]["0"-"9"]*  or  "A" | "B" | "C" | "D" | "E" | "F" | "G" | "H" | "J" | "K" */
        ret = EhsICalcInputState_value;
    }
    else
    {
        ret = EhsICalcInputState_error;
        EHSH_LOG_ERROR(EHS_MSG_CALC_INPUT(szContext));
    }
    return ret;
}


/**
 * Determine the next state, given that we are currently in a value state
 * that is expecting to see a certain range of types next
 * (end, close paren, binary operator, comma)
 *
 * @param eToken Type of input just read
 * @param eState Current state of the parser
 * @param szContext remainder of input string (used in error messages)
 * @return next state resulting from this
 */
EhsICalcInputStateEnum EhsICalcValueCurrent(EhsICalcTokenInfoType* eToken, EhsICalcInputStateEnum eState, const char* szContext)
{
    if (EhsICalcInputState_operator == eToken->eState)
    {
        eState = EhsICalcInputState_operator;
    }
    else if (EhsICalcToken_end == eToken)    /* End of input detected */
    {
        eState = EhsICalcInputState_end;
    }
    else if (EhsICalcToken_close == eToken)     /**< ) */
    {
        eState = EhsICalcInputState_close;
    }
    else if (EhsICalcToken_comma == eToken)     /**< , */
    {
        eState = EhsICalcInputState_comma;
    }
    else
    {
        eState = EhsICalcInputState_error;
        EHSH_LOG_ERROR(EHS_MSG_CALC_OPERATOR(szContext));
    }
    return eState;

}
/**
 * Validate the next token by checking it against our parser state machine. The
 * state machine implemented is as follows:
 *
 *
 * @dot
 digraph d1 {
 value -> end [label="end"];
 close -> end [label="end"];
 value -> close [label=")"];
 close -> close [label=")"];
 value -> operator [label="+,-,*,%,/,^"];
 close -> operator [label="+,-,*,%,/,^"];
 value -> comma [label=","];
 close -> comma [label=","];
 init -> val;
 operator -> val;
 comma -> val;
 open -> val;
 negate -> val;
 function -> open [label="("];
 val [shape=point];
 val -> value [label="<constant>, <input>"];
 val -> open [label="("];
 val -> negate [label="-"];
 val -> function [label="min, max, abs, square"];
}
 @enddot
 *
 * @param eToken Type of input just read
 * @param eState Current state of the parser
 * @param szContext remainder of input string (used in error messages)
 * @return next state resulting from this
 */
EhsICalcInputStateEnum EhsICalcValidateToken(EhsICalcTokenInfoType* eToken, EhsICalcInputStateEnum eState, const char* szContext)
{
    static ehs_uint8 nArgCount[EHS_ICALC_MAX_PAREN_DEPTH]; /* manages a stack containing number of arguments required at each nesting level */
    static ehs_uint16 nArgLevel = 0; /* represents current nesting level of functions. 0 = not within a
									 function */
    switch(eState)
    {
    case EhsICalcInputState_init: /* Start state */
        /* initialise arg count stack with 1. This can be changed by the occurrence of
         * multi param functions */
        for (nArgLevel = 0; nArgLevel < EHS_ICALC_MAX_PAREN_DEPTH; nArgLevel++)
        {
            nArgCount[nArgLevel] = 1;
        }
        nArgLevel = 0;
    /* fall through to next case */
    case EhsICalcInputState_operator:
    case EhsICalcInputState_negate:
        /* we expect a value token (constant,variable,...) - check this */
        eState = EhsICalcValidateValueNext(eToken, eState, szContext);
        break;
    case EhsICalcInputState_comma:
        if (nArgCount[nArgLevel] > 1)
        {
            /* this is the second or subsequent argument in a function - check that we now have a value */
            eState = EhsICalcValidateValueNext(eToken, eState, szContext);
            nArgCount[nArgLevel]--;
        }
        else
        {
            EHSH_LOG_ERROR(EHS_MSG_CALC_COMMA(szContext));
            eState = EhsICalcInputState_error;
        }
        break;
    case EhsICalcInputState_open:
        nArgLevel++; /* start a new nesting level */
        if (nArgLevel < EHS_ICALC_MAX_PAREN_DEPTH)
        {
            eState = EhsICalcValidateValueNext(eToken, eState, szContext);
        }
        else
        {
            EHSH_LOG_ERROR(EHS_MSG_CALC_PAREN_NEST(szContext));
            eState = EhsICalcInputState_error;
        }
        break;
    case EhsICalcInputState_close:
        if (1 == nArgCount[nArgLevel])
        {
            if (nArgLevel > 0)
            {
                nArgLevel--;
                eState = EhsICalcValueCurrent(eToken, eState, szContext);
            }
            else
            {
                EHSH_LOG_ERROR(EHS_MSG_CALC_CLOSE(szContext));
                eState = EhsICalcInputState_error;
            }
        }
        else
        {
            eState = EhsICalcInputState_error;
            EHSH_LOG_ERROR(EHS_MSG_CALC_ARGS(szContext));
        }
        break;
    case EhsICalcInputState_value:
        eState = EhsICalcValueCurrent(eToken, eState, szContext);
        break;
    case EhsICalcInputState_function:
        if (EhsICalcToken_open == eToken)
        {
            eState = EhsICalcInputState_open;
        }
        else
        {
            eState = EhsICalcInputState_error;
            EHSH_LOG_ERROR(EHS_MSG_CALC_OPEN_EXPECTED(szContext));
        }
        break;
    case EhsICalcInputState_end:
        EHSH_LOG_ERROR(EHS_MSG_CALC_INTERNAL_END);
        eState = EhsICalcInputState_error;
        break;
    default:
        eState = EhsICalcInputState_error;
        EHSH_LOG_ERROR(EHS_MSG_CALC_UNRECOGNISED(szContext));
        break;
    }

    /* just entered state function, check to see how many parameters we require */
    if (EhsICalcInputState_function == eState)
    {
        /* we need to modify nArgLevel+1 as the next item we find should be
         * open parens, which will increase the nArgLevel */
        nArgCount[nArgLevel+1] = eToken->nParams;
    }
    else if (EhsICalcInputState_end == eState)
    {
        /* just entered end state. Check that nothing's been left open or whatever */
        if (nArgLevel > 0)
        {
            if (nArgCount[nArgLevel] > 1)
            {
                EHSH_LOG_ERROR(EHS_MSG_CALC_ARGS(szContext));
                eState = EhsICalcInputState_error;
            }
            else
            {
                EHSH_LOG_ERROR(EHS_MSG_CALC_PAREN_MISMATCH);
                eState = EhsICalcInputState_error;
            }
        }
    }
    else if (EhsICalcInputState_error == eState)
    {
        /* reset statics, so that the next calculate function block restarts correctly */
        nArgLevel = 0;
    }
    return eState;
}

/**
 * Count the number of instructions detected in the input stream.
 *
 * @return number of instruction slots required. 0 indicates that the input contains
 * a syntax error.
 */
ehs_uint16 EhsICalcCountInstr(const char* szInput)
{
    //EhsICalcInputStateEnum eState = EhsICalcInputState_init; /* current state for the parser */
    EhsICalcTokenInfoType* eToken; /* token read from the input */

    ehs_uint16 nNumEntries = 0; /* number of entries required in instruction queue */
    ehs_uint32 nCurrentStackSize = 0; /* Stack size at the current instruction */
    ehs_uint32 nMaxStackSize = 0; /* high water mark for the stack usage */
    do
    {
        szInput = EhsICalcGetToken(szInput, &eToken);
        if ((EhsICalcToken_const == eToken) ||
                (EhsICalcToken_input == eToken))
        {
            /* these require two entries because they use an instruction and data */
            nNumEntries += 2;
            nCurrentStackSize++;
        }
        else if ((EhsICalcInputState_function == eToken->eState) ||
                 (EhsICalcInputState_operator == eToken->eState) ||
                 (EhsICalcInputState_value == eToken->eState))
        {
            nNumEntries++;
            nCurrentStackSize = 1 - eToken->nParams; /* functions and operators take nParams off the stack, and put one back on */
        }
        else if (EhsICalcToken_invalid == eToken)
        {
            nNumEntries = 0; /* a syntax error means that no instructions are required */
            break;
        }

        /* check for high water mark on stack usage */
        if (nCurrentStackSize > nMaxStackSize)
        {
            nMaxStackSize = nCurrentStackSize;
        }
    }
    while ((EhsICalcToken_end != eToken) &&
            (EhsICalcToken_invalid != eToken));

    if (EhsICalcToken_invalid == eToken)
    {
        nNumEntries = 0;
    }

    return nNumEntries;
}

/**
 * In an expression A o B p C the right hand operator needs to be executed before the
 * left hand operator
 */
ehs_bool EhsICalc_executeLaterOpFirst(EhsICalcTokenInfoType* eTokenFirst, EhsICalcTokenInfoType* eTokenSecond)
{
    ehs_bool ret=EHS_FALSE;
    EhsDataflowIntType nFirstProps = eTokenFirst->nConst;
    EhsDataflowIntType nSecondProps = eTokenSecond->nConst;

    if ( ((EHS_I_CALC_OP_ASSOCIATIVE & nSecondProps) != 0) || ((EHS_I_CALC_OP_LEFT_ASSOCIATIVE & nSecondProps) != 0))
    {
        /* the second (later) operator is associative or left associative:
         * it's executed before the first operator iff its order of precedence is greater than the first operator's
         */
        ret = (EHS_I_CALC_OP_PRECEDENCE(nSecondProps) > EHS_I_CALC_OP_PRECEDENCE(nFirstProps));
    }
    else if ((EHS_I_CALC_OP_RIGHT_ASSOCIATIVE & nSecondProps) != 0)
    {
        /* the second operator is right associative:
         * it's executed before the first iff its order of precedence is the same or greater than the first operator's
         */
        ret = (EHS_I_CALC_OP_PRECEDENCE(nSecondProps) >= EHS_I_CALC_OP_PRECEDENCE(nFirstProps));
    }
    else
    {
        /* error */
        EHSH_LOG_ERROR(EHS_MSG_WARNING_ICALC_ASSOCIATIVITY(eTokenSecond->szName));
    }
    return ret;
}

/**
 * Add the latest instruction into the instruction store.
 * Update the stack size to reflect how adding this instruction will change the stack requirement.
 *
 * @param pInstrStore Where to store the latest instruction
 * @param pToken Pointer to the token to add
 * @param pStackSize Pointer to the stack size to update
 */
EhsICalcEngInstrType* EhsICalc_storeValue(EhsICalcEngInstrType* pInstrStore, EhsICalcTokenInfoType* pToken, ehs_uint32* pStackSize)
{
    if (pToken->eInstruction != EhsICalcEngInstr_Nop)
    {
        /* only store "real" instructions */
        pInstrStore->eInstr = pToken->eInstruction;
        pInstrStore++;

        if ((EhsICalcToken_const == pToken) || (EhsICalcToken_input == pToken))
        {
            /* in the case of an input token, the const val will be converted into an input reference later */
            pInstrStore->nConstVal = pToken->nConst;
            pInstrStore++;
            (*pStackSize)++; /* constants and variables require one stack entry */
        }
        else
        {
            (*pStackSize) += 1 - pToken->nParams; /* operators consume nParams, but return one item */
        }
    }

    return pInstrStore;
}

/**
 * Load the number of instructions from the input stream into the instruction store.
 *
 * @return Stack size required. 0 indicates that the input contains
 * a syntax error.
 */
ehs_uint32 EhsICalcWriteInstructionQueue(const char* szInput, EhsICalcEngContext* pContext)
{
    EhsICalcInputStateEnum eState = EhsICalcInputState_init; /* current state for the parser */
    EhsICalcTokenInfoType* eToken; /* token read from the input */
    EhsICalcTokenInfoType** aOpStack = EhsHMem_tempAlloc(pContext->nInstrSize*sizeof(EhsICalcTokenInfoType*)); /* temporary stack used to hold operators as we constuct the expression */
    ehs_uint32 iOpStack = 0; /* index into the operator stack */
    EhsICalcEngInstrType* pInstrStore = pContext->pInstrQueue;

    ehs_uint32 nCurrentStackSize = 0; /* Stack size at the current instruction */
    ehs_uint32 nMaxStackSize = 0; /* high water mark for the stack usage */

    do
    {
        /* note that GetToken() returns instance-specific data in the case of variables and constants.
         * However, this data is copied into the instruction queue before the next instruction is
         * received. No other kind of token has instance-specific data associated with it, so it
         * is safe to keep a pointer to the token in aOpStack.
         */
        szInput = EhsICalcGetToken(szInput, &eToken);
        eState = EhsICalcValidateToken(eToken, eState, szInput);
        switch (eState)
        {
        case EhsICalcInputState_value:  /* variable or constant */
            /* put value onto instruction queue */
            pInstrStore = EhsICalc_storeValue(pInstrStore, eToken, &nCurrentStackSize);
            break;
        case EhsICalcInputState_negate:
            eToken = EhsICalcToken_negate; /* convert token correctly before continuing */
        case EhsICalcInputState_operator:
        case EhsICalcInputState_function:
            while ((iOpStack > 0) && !EhsICalc_executeLaterOpFirst(aOpStack[iOpStack-1],eToken))
            {
                /* take the operator from the top of the stack and put it into the instruction queue */
                pInstrStore = EhsICalc_storeValue(pInstrStore, aOpStack[--iOpStack], &nCurrentStackSize);
            }
            /* push the new operator onto the stack */
            aOpStack[iOpStack++] = eToken;
            break;
        case EhsICalcInputState_open:
            /* put open parentheses or commas onto operator stack */
            aOpStack[iOpStack++] = eToken;
            break;
        case EhsICalcInputState_comma:
            /* move operators from operator stack onto instruction queue until:
             * open parenthesis or comma is found
             * operator stack is empty - indicates a malformed expression
             */
            while ((iOpStack > 0) &&
                    (EhsICalcToken_open != aOpStack[iOpStack-1]) &&
                    (EhsICalcToken_comma != aOpStack[iOpStack-1]))
            {
                pInstrStore = EhsICalc_storeValue(pInstrStore, aOpStack[--iOpStack], &nCurrentStackSize);
            }

            break;
        case EhsICalcInputState_close:
            /* move operators from operator stack onto instruction queue until:
             * open parenthesis is found
             * operator stack is empty - actually indicates a malformed expression
             */
            while ((iOpStack > 0) && (EhsICalcToken_open != aOpStack[iOpStack-1]))
            {
                pInstrStore = EhsICalc_storeValue(pInstrStore, aOpStack[--iOpStack], &nCurrentStackSize);
            }

            if (iOpStack > 0 && EhsICalcToken_open == aOpStack[iOpStack-1])
            {
                iOpStack--; /* pop top value (open parens) off stack */
            } /* else mismatched parentheses -- this condition shouldn't happen here, as parser should have already detected it */
            break;
        case EhsICalcInputState_end: /* end doesn't require any processing */
            break;
        case EhsICalcInputState_init:
        /* should never happen after one token is read - treat this as an error */
        case EhsICalcInputState_error:
            /* syntax error - expression not valid */
            nMaxStackSize = 0;
            nCurrentStackSize = 0;
            break;
        }

        /* check for increased stack size requirements */
        if (nCurrentStackSize > nMaxStackSize) nMaxStackSize = nCurrentStackSize;

    }
    while ((EhsICalcInputState_end != eState) &&
            (EhsICalcInputState_error != eState));

    /* pop operators from output stack onto instruction queue */
    while (iOpStack > 0)
    {
        pInstrStore = EhsICalc_storeValue(pInstrStore, aOpStack[--iOpStack], &nCurrentStackSize);
    }

    EhsHMem_tempFree(aOpStack);

    return nMaxStackSize;
}
