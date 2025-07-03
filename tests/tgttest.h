/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/


/** @file tgttest.h
 * This file defines the test structures used to automate tests.
 *
 * @author: inx limited
 */

#ifndef TGTTEST_H_
#define TGTTEST_H_

#include "ehs_types.h"

#define MAX_CASES_PER_SUITE 20 /**< Maximum number of test cases per suite */

/**
 * Defines a pointer to a standard test function .
 *
 * @return NULL if successful, error message if the test failed
 */
typedef const char* (*TgtTestFunc)(void);

/**
 * Defines a single test
 */
struct TgtTestCaseStruct
{
    const char* szName;	/**< Name of the test */
    TgtTestFunc pfFunc;	/**< Test function to execute */
};

/**
 * Defines a collection of tests
 */
struct TgtTestSuiteStruct
{
    const char* szName;		/**< Name of the test set */
    int nCases;		/**< Number of cases in this test suite */
    struct TgtTestCaseStruct pCases[MAX_CASES_PER_SUITE];	/**< Pointer to null terminated collection of test cases */
};

/**
 * General purpose string to contain error messages returned by individual tests
 */
EHS_GLOBAL ehs_char szErrorMessage[1000];

#endif /*TGTTEST_H_*/
