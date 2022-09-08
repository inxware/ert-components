/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/



/** @file tgttest_console.c
 * Target tests for console I/O
 *
 * @author: inx limited
 */

#include "tgttest.h"
#include "hal_console.h"
#include "hal_string.h"
#include "target.h"

const char* TgtTestConsolePrintf(void);
const char* TgtTestConsoleInputHit(void);
const char* TgtTestConsoleLoadFile(void);
const char* TgtTestConsoleGetLine(void);

struct TgtTestSuiteStruct TgtTestSetConsole = { "Console", 4 /* test cases */,
    {
        { "Printf", TgtTestConsolePrintf },
        { "InputHit", TgtTestConsoleInputHit },
        { "LoadFile", TgtTestConsoleLoadFile },
        { "GetLine", TgtTestConsoleGetLine }
    }
} ;


/**
 * Test the Printf function
 *
 * - test variable numbers of arguments
 * - test argument formatting
 */
const char* TgtTestConsolePrintf(void)
{
    EhsConsolePrintf("Test EhsConsolePrintf\r\n");

    return NULL;
}

/**
 * Test the EhsConsoleInputHit() function. Also checks EhsConsoleLineReady()
 *
 */
const char* TgtTestConsoleInputHit(void)
{
    ehs_char buff[1000];
    const char* szRet = NULL;

    /* empty the input buffer */
    if (EhsConsoleInputHit())
    {
        EhsConsolePrintf("\tPress enter\r\n");
        while (!EhsConsoleLineReady())
            ;
        EhsConsoleGetLine(buff,1000);
    }

    /* -ve check for input hit */
    if (EhsConsoleInputHit())
    {
        szRet = "EhsConsoleInputHit returned true unexpectedly";
    }

    /* +ve check for input hit */
    EhsConsolePrintf("\tPress space\r\n");
    while (!EhsConsoleInputHit())
        ;

    /* empty the buffer */
    EhsConsolePrintf("\tPress enter\r\n");
    while (!EhsConsoleLineReady())
        ;
    EhsConsoleGetLine(buff,1000);


    return szRet;
}

/**
 * Test the EhsConsoleToFile() function.
 *
 */
const char* TgtTestConsoleLoadFile(void)
{
    ehs_char buff[1000];
    const char* szRet = NULL;


    /* load a file */
    EhsConsolePrintf("\tType \"12345\" (without quotes)\r\n");
    if (EhsConsoleToFile(5,"test.txt"))
    {
        ehs_uint16 i;
        ehs_FILE* f;
        f = EhsFopen("test.txt","r");
        if (f)
        {
            for (i =0; i < 5; i++)
            {
                buff[i] = EhsFgetc(f);
            }
            buff[i] = '\0';
            if (EhsStricmp(buff,"12345"))
            {
                szRet = "Expected string not found in file";
            }
            EhsFclose(f);
        }
        else
        {
            szRet = "File not found";
        }
    }
    else
    {
        szRet = "EhsConsoleToFile returns false";
    }

    /* check for timeout on file load */
    EhsConsolePrintf("\tWait for 10 seconds (ignore error message at timeout)\r\n");
    if (EhsConsoleToFile(5,"test.txt"))
    {
        szRet = "Expected EhsConsoleToFile to return false";
    }

    return szRet;
}

/**
 * Test the EhsConsoleGetLine()
 *
 */
const char* TgtTestConsoleGetLine(void)
{
    ehs_char buff[1000];
    const char* szRet = NULL;

    /* empty the input buffer */
    EhsConsolePrintf("\tPress enter\r\n");
    while (!EhsConsoleLineReady())
        ;
    EhsConsoleGetLine(buff,1000);

    /* check for reading test from GetLine */
    EhsConsolePrintf("\ttype \"hi\", without quotes followed by <enter>\r\n");
    while (!EhsConsoleLineReady())
        ;
    EhsConsoleGetLine(buff,1000);
    if (EhsStrnicmp(buff,"hi",2))
    {
        ehs_char* pChar = buff;
        EhsConsolePrintf("\tExpected {'h','i',\\r\n'}, received {");
        if (*pChar)
        {
            EhsConsolePrintf("%c",*pChar);
            pChar++;
        }
        while (*pChar)
        {
            EhsConsolePrintf(",%c",*pChar);
            pChar++;
        }
        EhsConsolePrintf("}\r\n");
        szRet = "Didn't get \"hi\"";
    }

    return szRet;
}
