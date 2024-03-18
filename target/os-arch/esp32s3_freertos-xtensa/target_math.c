/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

/** @file target_math.c
 * In this file, target-specific math functions for EHS are defined.
 *
 * @author: inx limited
 *
 */
/**
 * @page Verification Verification report
 * @section target
 * @anchor target
 * @subsection misra MISRA compliance:
 * target_math.c demonstrated MISRA compliant on <not yet>
 * Last modified on $Date$
 *
 * This file contained the following derogations to the MISRA standard.
 */

/*****************************************************************************/
/* Included files */
#define EHS_TARGET_CODE /* Ensure header files include target-internal values  \
                         */

#include "target.h"
#include <math.h>

/*****************************************************************************/
/* Declare macros and local typedefs used by this file */
/*****************************************************************************/
/* Declare prototypes of local functions */

/*****************************************************************************/
/* Variables defined with file-scope */

/**
 * Simple conversion of degrees to sine
 */
EHS_LOCAL ehs_sint32 EhsTgtMathCosTable[91] =
{
    1000000, 999848, 999391, 998630, 997564, 996195, 994522, 992546, 990268,
    987688,  984808, 981627, 978148, 974370, 970296, 965926, 961262, 956305,
    951057,  945519, 939693, 933580, 927184, 920505, 913545, 906308, 898794,
    891007,  882948, 874620, 866025, 857167, 848048, 838671, 829038, 819152,
    809017,  798636, 788011, 777146, 766044, 754710, 743145, 731354, 719340,
    707107,  694658, 681998, 669131, 656059, 642788, 629320, 615661, 601815,
    587785,  573576, 559193, 544639, 529919, 515038, 500000, 484810, 469472,
    453990,  438371, 422618, 406737, 390731, 374607, 358368, 342020, 325568,
    309017,  292372, 275637, 258819, 241922, 224951, 207912, 190809, 173648,
    156434,  139173, 121869, 104528, 87156,  69756,  52336,  34899,  17452,
    0
};

/**
 * Performs sine function using integers.
 *
 * @param nMult multiplier for output
 * @param nAngle angle to find (0 to 360)
 * @return nMult * sin(nAngle % 360)
 */
ehs_sint32 EhsTgtInt_sin(ehs_sint32 nMult, ehs_sint32 nAngle)
{
    ehs_sint32 ret = EhsTgtInt_cos(nMult, nAngle - 90);

    return ret;
}

/**
 * Performs cosine function using integers.
 *
 * @param nMult multiplier for output
 * @param nAngle angle to find (0 to 360)
 * @return nMult * cos(nAngle % 360)
 */
ehs_sint32 EhsTgtInt_cos(ehs_sint32 nMult, ehs_sint32 nAngle)
{
    ehs_sint32 nDiv = 1000000 / nMult;
    ehs_sint32 normalisedAngle =
        (nAngle < 0) ? (-(nAngle % 360)) : (nAngle % 360);
    ehs_sint32 ret;

    if (normalisedAngle <= 90)
    {
        ret = EhsTgtMathCosTable[normalisedAngle] / nDiv;
    }
    else if (normalisedAngle <= 180)
    {
        ret = -EhsTgtMathCosTable[180 - normalisedAngle] / nDiv;
    }
    else if (normalisedAngle <= 270)
    {
        ret = -EhsTgtMathCosTable[normalisedAngle - 180] / nDiv;
    }
    else
    {
        ret = EhsTgtMathCosTable[360 - normalisedAngle] / nDiv;
    }

    return ret;
}

double EhsTgtFloat_log10(double in)
{
    return log10(in);
}

double EhsTgtFloat_loge(double in)
{
    return log(in);
}
