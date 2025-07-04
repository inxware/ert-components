/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/


/** @file tgttest_main.c
 * This is the Main EHS entry point for target testing
 *
 * @author: inx limited
 */

#include "ehs_main.h"
#include "target.h"
#include "tgttest.h"

extern struct TgtTestStruct TgtTestSetConsole, TgtTestSetFile, TgtTestSetString,
           TgtTestSetTime, TgtTestSetViewport, TgtTestSetGraphics, TgtTestSetWidget, TgtTestSetMemory;
/**
 *  set of tests to execute
 */
struct TgtTestSuiteStruct *TgtTestSet[] =
{
    &TgtTestSetConsole, &TgtTestSetFile, &TgtTestSetString, &TgtTestSetTime,
    &TgtTestSetViewport, &TgtTestSetGraphics, &TgtTestSetWidget,
    &TgtTestSetMemory, NULL
};

ehs_uint16 TestsTried;
ehs_uint16 TestsPassed;
ehs_uint16 nNumTestSuites; /**< Records the number of test suites in TgtTestStruct */

ehs_bool bContinueFlag; /* while true, we keep calling EhsMainLoop */

/**
 * General purpose string to contain error messages returned by individual tests
 */
ehs_char szErrorMessage[1000];

/**
 * Each call of this function executes a single test. This function also operates a state machine
 * [init] / ask the user "test (A)ll suites, test specific suite (1-n), list test suites (?) or (E)xit", results in
 * 		a transition to [test-all-suites], [test-suite-init], remain in [init], return with false
 * [test-all-suites] / for each test suite, execute [test-suite-all], then return to [init]
 * [test-suite-init] / ask the user "test (A)ll cases in this suite, test specific case (1-n), list test cases (?), (R)eturn, or (E)xit
 * 		results in [test-suite-all], [test-suite-single], remain in this state, [test-all-suites], return with false
 * [test-suite-all] / for each test case, execute [test-suite-single], then return to previous state
 * [test-suite-single] / execute a single test, then return to the previous state
 */
void EhsMainLoop(void)
{
    /**
     * Indicates the current test suite we are running. When -1, need to prompt the
     * tester whether we test all, or specific suites */
    static ehs_sint16 nCurrentSuite = -1;
    /*
     * When nCurrentSuite > -1, this indicates whether to run all test suites (or just one)
     */
    static ehs_bool bAllSuites;
    /**
     * Indicates that testing should continue
    /**
     * Indicates which test case within the current suite we are running. When -1, need to
     * prompt the tester whether to test all or specific cases. When nCurrentSuite is -1, this
     * is meaningless */
    static ehs_sint16 nCurrentCase = -1;
    /**
     * When nCurrentCase > -1, this indicates whether to run all cases within the current suite
     */
    static ehs_bool bAllCases;
    ehs_char cResp[100];

    if (nCurrentSuite == -1)
    {
        ehs_uint16 i;
        EhsConsolePrintf("Test (A)ll suites, test specific suite (0-%d), (L)ist suites, (E)xit\n\r> ", nNumTestSuites-1);
        while (!EhsConsoleLineReady()) {}
        EhsConsoleGetLine(cResp,100);
        EhsConsolePrintf("\n\r");
        switch(cResp[0])
        {
        case 'A':
        case 'a':
            /* run all test suites */
            nCurrentSuite = 0;
            nCurrentCase = 0;
            bAllSuites = EHS_TRUE;
            bAllCases = EHS_TRUE;
            TestsPassed = 0;
            TestsTried = 0;
            break;
        case '9':
            nCurrentSuite++;
        case '8':
            nCurrentSuite++;
        case '7':
            nCurrentSuite++;
        case '6':
            nCurrentSuite++;
        case '5':
            nCurrentSuite++;
        case '4':
            nCurrentSuite++;
        case '3':
            nCurrentSuite++;
        case '2':
            nCurrentSuite++;
        case '1':
            nCurrentSuite++;
        case '0':
            nCurrentSuite++;
            if (nCurrentSuite >= nNumTestSuites)
            {
                EhsConsolePrintf("**Out of range\n\r");
            }
            bAllSuites = EHS_FALSE;
            break;
        case 'l':
        case 'L':
            for (i = 0; i < nNumTestSuites; i++)
                EhsConsolePrintf("%d\t%s\n\r", i, TgtTestSet[i]->szName);
            break;
        case 'e':
        case 'E':
            bContinueFlag = EHS_FALSE;
            break;
        }
    }
    else
    {
        /* we're running tests within a specific suite */
        ehs_uint16 nCases = TgtTestSet[nCurrentSuite]->nCases;

        if (nCurrentCase == -1)
        {
            ehs_uint16 i;

            if (nCases > 9)
                EhsConsolePrintf("Warning - we can only handle up to 9 test cases with the menu system implemented here. Update tgttest_main.c\n");

            EhsConsolePrintf("Test suite %s: (A)ll cases, test specific case (0-%d), (L)ist cases, (R)eturn to suite selection, (E)xit\n\r> ",
                             TgtTestSet[nCurrentSuite]->szName, nCases-1);
            while (!EhsConsoleLineReady()) {}
            EhsConsoleGetLine(cResp,100);
            EhsConsolePrintf("\n\r");
            switch(cResp[0])
            {
            case 'A':
            case 'a':
                /* run all test cases */
                nCurrentCase = 0;
                bAllCases = EHS_TRUE;
                break;
            case '9':
                nCurrentCase++;
            case '8':
                nCurrentCase++;
            case '7':
                nCurrentCase++;
            case '6':
                nCurrentCase++;
            case '5':
                nCurrentCase++;
            case '4':
                nCurrentCase++;
            case '3':
                nCurrentCase++;
            case '2':
                nCurrentCase++;
            case '1':
                nCurrentCase++;
            case '0':
                nCurrentCase++;
                if (nCurrentCase >= nCases)
                {
                    EhsConsolePrintf("**Out of range\n\r");
                }
                bAllCases = EHS_FALSE;
                break;
            case 'l':
            case 'L':
                for (i = 0; i < nCases; i++)
                    EhsConsolePrintf("%d\t%s:%s\n\r", i, TgtTestSet[nCurrentSuite]->szName, TgtTestSet[nCurrentSuite]->pCases[i].szName);
                break;
            case 'e':
            case 'E':
                bContinueFlag = EHS_FALSE;
                break;
            case 'r':
            case 'R':
                nCurrentSuite = -1;
                break;
            }
        }
        else
        {
            /* we need to run a specific test now */
            if (nCurrentCase < nCases)
            {
                const ehs_char* pRet = TgtTestSet[nCurrentSuite]->pCases[nCurrentCase].pfFunc();
                TestsTried++;
                if (pRet)
                {
                    EhsConsolePrintf("--Failed %s:%s (%s)\n\r",
                                     TgtTestSet[nCurrentSuite]->szName,
                                     TgtTestSet[nCurrentSuite]->pCases[nCurrentCase].szName,
                                     pRet);
                }
                else
                {
                    EhsConsolePrintf("++Passed %s:%s\n\r",
                                     TgtTestSet[nCurrentSuite]->szName,
                                     TgtTestSet[nCurrentSuite]->pCases[nCurrentCase].szName);
                    TestsPassed++;
                }
                if (bAllCases)
                    nCurrentCase++; /* test the next case on the next execution */
                else
                    nCurrentCase = -1; /* return the the menu for the current suite */
            }
            else
            {
                if (bAllSuites)
                {
                    nCurrentSuite++;
                    if (nCurrentSuite < nNumTestSuites)
                    {
                        /* start testing the first test case of the next suite */
                        nCurrentCase = 0;
                    }
                    else
                    {
                        /* just finished a complete test of everything - return to top-level menu */
                        EhsConsolePrintf("Passed %d out of %d\n\r",TestsPassed, TestsTried);
                        nCurrentSuite = -1;
                    }
                }
                else
                {
                    /* just finished a test within the current suite - return to suite level menu */
                    nCurrentCase = -1;
                }
            }
        }
    }
}

/**
 * EHS Entry point. Initialise EHS, then start running EHS & handling console
 * input.
 */
void EhsMain(void)
{
    TestsTried = 0;
    TestsPassed = 0;
    EhsHSys_init();
    EhsHApp_init();
    EhsHApp_reset();

    for (nNumTestSuites = 0; TgtTestSet[nNumTestSuites]; nNumTestSuites++)
    {}

    if (nNumTestSuites > 9)
        EhsConsolePrintf("Warning - we can only handle up to 9 test suites with the menu system implemented here. Update tgttest_main.c\n");

    bContinueFlag = EHS_TRUE;

#ifndef EHS_TARGET_EVENT_MODEL
    while(bContinueFlag)
    {
        EhsMainLoop();
    }
#endif

    EhsExit(0);
}
