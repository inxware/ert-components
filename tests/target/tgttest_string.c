/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/



/** @file tgttest_string.c
 * Target tests for string operations
 *
 * @author: inx limited
 */

/**
 * @todo add tests for: EhsIsAlpha, EhsIsAlNum, EhsGetUint32FromString, EhsGetSint32FromString,
 * EhsGetUint16FromString, EhsGetUint8FromString, EhsGetEol
 */
#include "hal_file.h"
#include "hal_console.h"
#include "hal_string.h"
#include "target.h"
#include "widget.h"
#include "tgttest.h"

EHS_GLOBAL ehs_uint16 TestsTried;
EHS_GLOBAL ehs_uint16 TestsPassed;

const ehs_char* TgtTestFile_testSprintf(void);
const ehs_char* TgtTestFile_testStricmp(void);
const ehs_char* TgtTestFile_testStrnicmp(void);
const ehs_char* TgtTestFile_testStrlen(void);
const ehs_char* TgtTestString_testIsSpace(void);
const ehs_char* TgtTestString_testTrim(void);
const ehs_char* TgtTestString_toBeImplemented(void);
const ehs_char* TgtTestString_getUtf32(void);

struct TgtTestSuiteStruct TgtTestSetString = {"String", 12,
    {
        { "Sprintf", TgtTestFile_testSprintf},
        { "Sscanf", TgtTestString_toBeImplemented},
        { "Strcat", TgtTestString_toBeImplemented},
        { "Strchr", TgtTestString_toBeImplemented},
        { "Stricmp", TgtTestFile_testStricmp},
        { "Strnicmp", TgtTestFile_testStrnicmp},
        { "Strlen", TgtTestFile_testStrlen},
        { "IsSpace", TgtTestString_testIsSpace},
        { "Trim", TgtTestString_testTrim},
        { "Strcpy", TgtTestString_toBeImplemented},
        { "Strncpy", TgtTestString_toBeImplemented},
        { "getUtf32", TgtTestString_getUtf32}
    }
};

/**
 * Testing sprintf.
 */
const ehs_char* TgtTestFile_testSprintf(void)
{
    const ehs_char* szRet = NULL;
    ehs_char pBuff[100];
    ehs_char *pTest = "test 100 a hi";
    ehs_char *p1 = pBuff;
    ehs_char *p2 = pTest;

    EhsSprintf(pBuff, "test %d %c %s",100,'a',"hi");

    while (*p1)
    {
        if (*p1 != *p2)
        {
            EhsSprintf(pBuff,"Sprintf generated \"%s\", expected \"%s\"",pBuff,pTest);
            szRet = pBuff;
            break;
        }
        p1++;
        p2++;
    }

    return szRet;
}

/**
 * Testing sscanf. To be implemented
 */
const ehs_char* TgtTestString_toBeImplemented(void)
{
    const ehs_char* szRet = NULL;
    EhsConsolePrintf("\tTo be implemented\r\n");
    return szRet;
}
/**
 * Testing EhsStricmp.
 */
const ehs_char* TgtTestFile_testStricmp(void)
{
    const ehs_char* szRet = NULL;

    if (EhsStricmp("Hello","Hello"))
    {
        szRet = "Hello != Hello";
    }
    if (EhsStricmp("Hello","hEllo"))
    {
        szRet = "Hello != hEllo";
    }
    if (EhsStricmp("Hello","Hello1") == 0)
    {
        szRet = "Hello == Hello1";
    }
    if (EhsStricmp("Hello1","Hello") == 0)
    {
        szRet = "Hello1 == Hello";
    }
    return szRet;
}

/**
 * Testing EhsStrnicmp.
 */
const ehs_char* TgtTestFile_testStrnicmp(void)
{
    const ehs_char* szRet = NULL;

    if (EhsStrnicmp("Hello","Hello",4))
    {
        szRet = "(4) Hello != Hello";
    }
    if (EhsStrnicmp("Hello","hEllo",6))
    {
        szRet = "(6) Hello != hEllo";
    }
    if (EhsStrnicmp("Hello","Hello1",5))
    {
        szRet = "(5) Hello == Hello1";
    }
    if (EhsStrnicmp("Hello1","Hello",5))
    {
        szRet = "(5) Hello1 == Hello";
    }
    return szRet;
}

/**
 * Testing EhsStrlen.
 */
const ehs_char* TgtTestFile_testStrlen(void)
{
    const ehs_char* szRet = NULL;

    if (EhsStrlen("") != 0)
    {
        szRet = "EhsStrlen(\"\") != 0";
    }
    if (EhsStrlen("hi") != 2)
    {
        szRet = "EhsStrlen(\"hi\") != 2";
    }
    return szRet;
}

/**
 * Test that isSpace is correctly implemented
 */
const ehs_char* TgtTestString_testIsSpace(void)
{
    const ehs_char* szRet = NULL;
    ehs_char* szIsSpace = " \t\r\n";
    ehs_char* szIsNotSpace = "dQ4!\"6$%^&&*()\\";
    ehs_char szErr[100];

    while (*szIsSpace)
    {
        if (!EhsStrIsSpace(*szIsSpace))
        {
            EhsSprintf(szErr,"EhsIsSpace(0x%x) == FALSE",*szIsSpace);
            szRet = szErr;
            break;
        }
        szIsSpace++;
    }
    while (*szIsNotSpace)
    {
        if (EhsStrIsSpace(*szIsNotSpace))
        {
            EhsSprintf(szErr,"EhsIsSpace(0x%x) == TRUE",*szIsNotSpace);
            szRet = szErr;
            break;
        }
        szIsNotSpace++;
    }
    return szRet;
}

/**
 * Test string trimming (left, right, both)
 */
const ehs_char* TgtTestString_testTrim(void)
{
    const ehs_char* szRet = NULL;
    ehs_char szTestString[100];
    ehs_char szErr[100];
#define TEST_TRIM(expected,operation,actual) EhsStrcpy(szTestString, actual); if (0 != EhsStrcmp(expected,operation(szTestString))) {szRet = szErr; EhsSprintf(szErr,"%s <> %s()%s",#expected,#operation,#actual);}
    TEST_TRIM("ok",EhsStrTrimL,("ok"));
    TEST_TRIM("ok",EhsStrTrimL,(" \tok"));
    TEST_TRIM("ok \n",EhsStrTrimL,(" ok \n"));
    TEST_TRIM("",EhsStrTrimL,(" \n \n\r\t"));
    TEST_TRIM("ok",EhsStrTrimR,("ok"));
    TEST_TRIM(" \tok",EhsStrTrimR,(" \tok"));
    TEST_TRIM(" ok",EhsStrTrimR,(" ok \n"));
    TEST_TRIM("",EhsStrTrimR,(" \n \n\r\t"));
    TEST_TRIM("ok",EhsStrTrimLR,("ok"));
    TEST_TRIM("ok",EhsStrTrimLR,(" \tok"));
    TEST_TRIM("ok",EhsStrTrimLR,(" ok \n"));
    TEST_TRIM("",EhsStrTrimLR,(" \n \n\r\t"));
    if (NULL != EhsStrTrimL(NULL))
    {
        szRet = "expected Null from EhsStrTrimL(NULL)";
    }
    if (NULL != EhsStrTrimR(NULL))
    {
        szRet = "expected Null from EhsStrTrimR(NULL)";
    }
    if (NULL != EhsStrTrimLR(NULL))
    {
        szRet = "expected Null from EhsStrTrimLR(NULL)";
    }

    return szRet;
}

/**
 * @page Test_String getUtf32
 * @section getUtf32
 * @anchor getUtf32
 * @subsection purpose Purpose:
 * Tests the getUtf32 correctly converts utf8 sequences into utf32 value
 *
 * @subsection initial Initial conditions:
 * None defined.
 * @subsection data Test data:
 * No specific test data is required
 * @subsection steps Test steps:
 * -# valid single character sequence: assert utf32({0x7f}) == 0x7f
 * -# valid double character sequence: assert utf32({0xdf,0xbf}) == 0x7ff
 * -# valid triple character sequence: assert utf32({0xef,0xbf,bf}) == 0xffff
 * -# valid quad character sequence: assert utf32({0xf3,0xbf,0xbf,0xbf} = 0x10ffff
 * -# malformed sequence: assert utf32({0xdf,0x7f}) == 0xfffffff, and utf32 returns false
 * -# abruptly terminated sequence: assert utf32({0xf7,0}) == 0xffffffff, and utf32 returns false
 * @subsection results Expected results:
 * -# Overwrites are detected in each case.
 */
const ehs_char* TgtTestString_getUtf32(void)
{
    const ehs_char* pRet = NULL;
    ehs_uint32 nUtf32;
    const ehs_uint8* szString;
    ehs_uint8 szStrData[] = {0x7f, 0xdf,0xbf, 0xef,0xbf,0xbf,0xf4,0x8f,0xbf,0xbf,0xdf,0x7f,0};
    ehs_uint8 szStrErr[] = {0xf7,0};

    // * -# valid single character sequence: assert utf32({0x7f}) == 0x7f
    szString = EhsHSUtil_getUtf32(&nUtf32,szStrData);
    if (!szString)
    {
        pRet = "2. getUtf32 failed";
    }
    else if (nUtf32 != 0x7f)
    {
        pRet = "1. converted char not 7f";
    }
    // * -# valid double character sequence: assert utf32({0xdf,0xbf}) == 0x7ff
    if (!pRet)
    {
        szString = EhsHSUtil_getUtf32(&nUtf32,szString);
        if (!szString)
        {
            pRet = "3. getUtf32 failed";
        }
        else if (nUtf32 != 0x7ff)
        {
            pRet = "4. converted char not 0x7ff";
        }
    }
    // * -# valid triple character sequence: assert utf32({0xef,0xbf,bf}) == 0xffff
    if (!pRet)
    {
        szString = EhsHSUtil_getUtf32(&nUtf32,szString);
        if (!szString)
        {
            pRet = "5. getUtf32 failed";
        }
        else if (nUtf32 != 0xffff)
        {
            pRet = "6. converted char not 0xffff";
        }
    }
    // * -# valid quad character sequence: assert utf32({0xf4,0xbf,0xbf,0xbf} = 0x10ffff
    if (!pRet)
    {
        szString = EhsHSUtil_getUtf32(&nUtf32,szString);
        if (!szString)
        {
            pRet = "7. getUtf32 failed";
        }
        else if (nUtf32 != 0x10ffff)
        {
            pRet = "8. converted char not 0x10ffff";
        }
    }
    // * -# malformed sequence: assert utf32({0xdf,0x7f}) == 0xfffffff, and utf32 returns false
    if (!pRet)
    {
        szString = EhsHSUtil_getUtf32(&nUtf32,szString);
        if (szString)
        {
            pRet = "9. getUtf32 succeeded unexpectedly";
        }
        else if (nUtf32 != 0xffffffff)
        {
            pRet = "10. converted char not 0xffffffff";
        }
    }
    // * -# abruptly terminated sequence: assert utf32({0xf7,0}) == 0xffffffff, and utf32 returns false
    if (!pRet)
    {
        if (EhsHSUtil_getUtf32(&nUtf32,szStrErr))
        {
            pRet = "9. getUtf32 succeeded unexpectedly";
        }
        else if (nUtf32 != 0xffffffff)
        {
            pRet = "10. converted char not 0xffffffff";
        }
    }

    return pRet;
}
