/** @file target_math.c
 * In this file, target-specific math functions for EHS are defined.
 * 
 * @author: inx limited
 * @version: $Revision: 1236 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 * 
 * Copyright (c) inx limited, 2006. All rights reserved.
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
