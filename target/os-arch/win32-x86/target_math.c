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

#define EHS_TARGET_CODE /* Ensure header files include target-internal values */

#include "target.h"
#include <math.h>

/**
 * Implementation of asinh as VisualStudio doesn't provide one.
 */
double asinh(double x)
{
    double ret = log(x + sqrt(x*x+1));

    return ret;
}

/**
 * Implementation of acosh as VisualStudio doesn't provide one.
 */
double acosh(double x)
{
    double ret = log(x + sqrt(x*x-1));

    return ret;
}

/**
 * Implementation of atanh as VisualStudio doesn't provide one.
 */
double atanh(double x)
{
    double ret = 0.5 * log((1+x)/(1-x));

    return ret;
}

/**
 * Performs sine function using integers.
 *
 * @param nMult multiplier for output
 * @param nAngle angle to find (0 to 360)
 * @return nMult * sin(nAngle % 360)
 */
EHS_GLOBAL ehs_sint32 EhsTgtInt_sin(ehs_sint32 nMult, ehs_sint32 nAngle)
{
    double dAngle = (nAngle / 360.0) * 2 * 3.1415926535897932384626433832795;
    double dRet = sin(dAngle) * nMult;

    return (ehs_sint32)dRet;
}

/**
 * Performs cosine function using integers.
 *
 * @param nMult multiplier for output
 * @param nAngle angle to find (0 to 360)
 * @return nMult * cos(nAngle % 360)
 */
EHS_GLOBAL ehs_sint32 EhsTgtInt_cos(ehs_sint32 nMult, ehs_sint32 nAngle)
{
    double dAngle = (nAngle / 360.0) * 2 * 3.1415926535897932384626433832795;
    double dRet = cos(dAngle) * nMult;

    return (ehs_sint32)dRet;
}
