/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/



/** @file tgttest_time.c
 * Target tests for timer
 *
 * @author: inx limited
 */

#include "hal_time.h"
#include "hal_console.h"
#include "hal_string.h"
#include "target.h"
#include "tgttest.h"

EHS_GLOBAL ehs_uint16 TestsTried;
EHS_GLOBAL ehs_uint16 TestsPassed;

const ehs_char* TgtTestTimeNow(void);
const ehs_char* TgtTestTimeSleep(void);
const ehs_char* TgtTestTimeTimer(void);
const ehs_char* TgtTestTimeReset(void);

struct TgtTestSuiteStruct TgtTestSetTime = {"Time", 4,
    {
        { "Now", TgtTestTimeNow},
        { "Sleep", TgtTestTimeSleep},
        { "Timer", TgtTestTimeTimer},
        { "Reset", TgtTestTimeReset}
    }
};

/**
 * Test the EhsTgtTimer_now function. Show time passing until 10 seconds has occurred.
 */
const ehs_char* TgtTestTimeNow(void)
{
    ehs_char buff[1000];
    EhsTickType start;
    EhsTickType finish;

    EhsConsolePrintf("\tPress enter to start 10 second count\r\n");
    while (!EhsConsoleLineReady())
        ;
    EhsConsoleGetLine(buff,1000);

    /* get start and end time */
    start = EhsTgtTimer_now();
    finish = EHS_CURRENT_TIME_OFFSET(EHS_TIME_s(10));

    /* run until end time */
    while (EHS_TARGET_TIME_IS_EARLIER(EhsTgtTimer_now(),finish))
    {
        if (EHS_TGT_TIME_DIFFERENCE(start,EhsTgtTimer_now()) > (EHS_TIME_s(1)))
        {
            EhsConsolePrintf("\t.\r\n");
            start = EhsTgtTimer_now();
        }
    }
    EhsConsolePrintf("Done. Was that 10 seconds? Press enter\r\n");
    while (!EhsConsoleLineReady())
        ;
    EhsConsoleGetLine(buff,1000);
    return NULL;
}

/**
 * Test the Sleep function
 *
 * - Sleep for a period of time. Check that the clock agrees with the period we've
 * slept for
 */
const ehs_char* TgtTestTimeSleep(void)
{
    EhsTickType start, finish;
    ehs_uint16 delta = 3; /* allowable margin of error on sleep test in ms */
    ehs_uint16 nSleptFor;
    const ehs_char* bRet = NULL;

    start = EhsTgtTimer_now();
    EhsSleep(EHS_TIME_ms(100));
    finish = EhsTgtTimer_now();

    nSleptFor = EhsTgtTimer_tickTous(EHS_TGT_TIME_DIFFERENCE(start,finish)) / 1000;

    if (((100 - nSleptFor) > delta) || (nSleptFor - 100) > delta)
    {
        EhsSprintf(szErrorMessage, "Called EhsSleep(100ms), delay was %-dms", nSleptFor);
        bRet = szErrorMessage;
    }

    return bRet;
}

ehs_uint16 TgtTestTime_tickCalled;

/**
 * Test the timer functionality
 *
 * -# Set the timer to expire in 10 seconds time
 * -# Check the expiry time is correct
 * -# Sleep for one second
 * -# Report the time remaining remaining until expiry
 * -# Check if the timer has expired and if EhsTimer_tick was called
 * -# If not repeat the sleep-report-check again
 */
const ehs_char* TgtTestTimeTimer(void)
{
    EhsTickType tExpiry;
    EhsTickType tRemains;
    const ehs_char* bRet = NULL;

    TgtTestTime_tickCalled = 0;

    tExpiry = EHS_CURRENT_TIME_OFFSET(EHS_TIME_s(10)); /* 10 secs from now */
    EhsTgtTimer_set(tExpiry);
    while (TgtTestTime_tickCalled == 0)
    {
        /* there's a slight chance that the timer expires between the
         * guard on this loop and the point this check takes place */
        if (EhsTgtTimer_expiry() != tExpiry)
        {
            EhsSprintf(szErrorMessage, "EhsTgtTimer_expiry() = %d", EhsTgtTimer_expiry());
            bRet = szErrorMessage;
            break;
        }
        if (!EHS_TARGET_TIME_IS_EARLIER(EhsTgtTimer_now(),tExpiry))
        {
            EhsSprintf(szErrorMessage, "Timer hasn't expired yet. Expiry time is %d, Current time is %d",tExpiry,EhsTgtTimer_now());
            bRet = szErrorMessage;
            break;
        }
        tRemains = EHS_TGT_TIME_DIFFERENCE(EhsTgtTimer_now(), tExpiry);
        EhsConsolePrintf("\tTime remaining = %d.%03ds\r\n",EhsTgtTimer_tickTous(tRemains)/1000000u,(EhsTgtTimer_tickTous(tRemains)/1000)%1000);
        EhsSleep(EHS_TIME_ms(800));
        EhsTgtTimer_tick();
    }

    if (!bRet && (EhsTgtTimer_expiry() != EHS_TICKTYPE_INVALID))
    {
        EhsSprintf(szErrorMessage, "Timer not reset following timer expiry");
        bRet = szErrorMessage;
    }

    return bRet;

}

/**
 * Test the EhsTgtTimer_reset()/EhsTgtTimer_clear() functions
 *
 * -# Set a timer running
 * -# Check expiry time
 * -# Reset timer
 * -# Check expiry time
 * -# Sleep until after expiry
 * -# Call Tick
 * -# Check that TgtTestTime_tickCalled hasn't been incremented
 */
const ehs_char* TgtTestTimeReset()
{
    EhsTickType tExpiry;
    EhsTickType tRemains;
    const ehs_char* bRet = NULL;
    ehs_char szErr[100];

    TgtTestTime_tickCalled = 0;

    EhsTgtTimer_reset();

    tExpiry = EHS_CURRENT_TIME_OFFSET(EHS_TIME_ms(100)); /* 100 ms from now */
    EhsTgtTimer_set(tExpiry);
    if (EhsTgtTimer_expiry() == tExpiry)
    {
        EhsSleep(EHS_TIME_ms(50));
        EhsTgtTimer_tick();
        if ((EhsTgtTimer_expiry() == tExpiry) && (TgtTestTime_tickCalled == 0))
        {
            EhsTgtTimer_clear();
            if (EhsTgtTimer_expiry() == EHS_TICKTYPE_INVALID)
            {
                EhsSleep(EHS_TIME_ms(60));
                EhsTgtTimer_tick();
                if (TgtTestTime_tickCalled != 0)
                {
                    EhsSprintf(szErrorMessage, "EhsTimer_tick called after expiry");
                    bRet = szErrorMessage;
                }
            }
            else
            {
                EhsSprintf(szErrorMessage, "EhsTgtTimer_clear() didn't reset expiry time");
                bRet = szErrorMessage;
            }
        }
        else
        {
            EhsSprintf(szErrorMessage, "Timer inconsistent. Test abandoned");
            bRet = szErrorMessage;
        }
    }
    else
    {
        EhsSprintf(szErrorMessage, "Timer not set.");
        bRet = szErrorMessage;
    }
    return bRet;
}

/*****************************************************************************/
/* From timer.c */
/**
 * Called by the timer hardware - either on a specific timer expiry, or on every tick
 */
void EhsTimer_tick(void)
{
    TgtTestTime_tickCalled++;
}

