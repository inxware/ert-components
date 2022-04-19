/** @file tgttest_file.c
 * Target tests for file operations
 * 
 * @author: inx limited
 * @version: $Revision: 1120 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 * 
 * Copyright (c) inx limited, 2006. All rights reserved.
 */

#include "hal_file.h"
#include "hal_console.h"
#include "hal_string.h"
#include "target.h"
#include "widget.h"
#include "tgttest.h"

const char* TgtTestFile_testOpen(void);
const char* TgtTestFile_testWrite(void);
const char* TgtTestFile_testRead(void);
const char* TgtTestFile_testEof(void);
const char* TgtTestFile_testConsumeLine(void);
const char* TgtTestFile_testGetWord(void);
const char* TgtTestFile_testFgets(void);

struct TgtTestSuiteStruct TgtTestSetFile = {"File", 7,
	{
		{ "Open", TgtTestFile_testOpen},
		{ "Write", TgtTestFile_testWrite},
		{ "Read", TgtTestFile_testRead},
		{ "Eof", TgtTestFile_testEof},
		{ "ConsumeLine", TgtTestFile_testConsumeLine},
		{ "GetWord", TgtTestFile_testGetWord},
		{ "fgets", TgtTestFile_testFgets}
	}
};

/**
 * Testing the file open operation. Try to open a file for reading that doesn't
 * exist.
 */
const char* TgtTestFile_testOpen(void)
{
	const char* szRet = NULL;
	ehs_FILE* pFile;
  	
	/* test opening a file for reading that doesn't exist */
	if (!szRet)
	{
		pFile = EhsFopen("no-exist.txt", "r");
		if (pFile)
		{
			EhsFclose(pFile);
			szRet = "Opened non-existent file for reading";
		}	
	}
	return szRet;
}

/**
 *  Test the fopen(*,"w"), write, fprintf operations on a file
 */
const char* TgtTestFile_testWrite(void)
{
	const char* szRet = NULL;
	ehs_FILE* pFile;
	
	pFile = EhsFopen("filetest.txt", "w");
	if (pFile)
	{
		char* pBuff = "Test EhsFwrite\n";
		ehs_uint16 i;
		
		EhsFwrite(pBuff, EhsStrlen(pBuff), 1, pFile);
		EhsFprintf(pFile,"Test EhsFprintf %d %c %s\n",5,'a',"everything");
		for (i = 0; i < 100; i++)
		{
			EhsFwrite(pBuff, 1, 1, pFile);
		}
		EhsFclose(pFile);
	}
	else
	{
		szRet = "Couldn't open filetest.txt for writing";
	}
	return szRet;
}

/**
 * Testing the file open(*,"r"), read, fgets, fscanf, and fgetc operations. 
 */
const char* TgtTestFile_testRead(void)
{
	const char* szRet = NULL;
	ehs_FILE* pFile;
	ehs_char errorMessage[100];
 	
	/* test opening a file for reading that doesn't exist */
	if (!szRet)
	{
		pFile = EhsFopen("filetest.txt", "r");
		if (pFile)
		{
			/* test fgets */
			ehs_char pBuff[500];
			EhsFgets(pBuff, 500, pFile);
			if (EhsStrnicmp("Test EhsFwrite",pBuff,14) != 0)
			{
				szRet = errorMessage;
				EhsSprintf(errorMessage,"EhsFgets returned %s, not \"Test EhsFwrite\"", pBuff);
			}
			
			/* test fscanf */
			if (!szRet)
			{
				ehs_sint32 num;
				ehs_char cRead;
				EhsFscanf(pFile,"Test EhsFprintf %d %c %s\n", &num, &cRead, &pBuff);
				if (num != 5)
				{
					szRet = errorMessage;
					EhsSprintf(errorMessage,"EhsFscanf returned num = %d, not 5",num);
				}
				if (cRead != 'a')
				{
					szRet = errorMessage;
					EhsSprintf(errorMessage,"EhsFscanf returned cRead = %c, not 'a'",cRead);
				}
				if (EhsStricmp(pBuff,"everything"))
				{
					szRet = errorMessage;
					EhsSprintf(errorMessage,"EhsFscanf returned pBuff = \"%s\", not \"everything\"", pBuff);
				}
			}
			
			/* test fgetc */
			if (!szRet)
			{
				ehs_char cRead = EhsFgetc(pFile);
				
				if (cRead != 'T')
				{
					szRet = errorMessage;
					EhsSprintf(errorMessage,"EhsFgetc returned '%c', not 'T'",cRead);
				}
			}
			
			/* test fread */
			if (!szRet)
			{
				ehs_sint16 nCount = EhsFread(pBuff,50,1,pFile);
				
				if (nCount != 1)
				{
					szRet = errorMessage;
					EhsSprintf(errorMessage,"EhsFread returned %d items, not 1", nCount);
				}
				while (nCount > 0)
				{
					nCount--;
					if (pBuff[nCount] != 'T')
					{
						szRet = errorMessage;
						EhsSprintf(errorMessage,"EhsFread item %d was '%c', not 'T'", nCount, pBuff[nCount]);
						break;
					}
				}
				
				nCount = EhsFread(pBuff, 1, 50, pFile);
				if (nCount != 49)
				{
					szRet = errorMessage;
					EhsSprintf(errorMessage,"EhsFread returned %d items, not 49", nCount);
				}
				while (nCount > 0)
				{
					nCount--;
					if (pBuff[nCount] != 'T')
					{
						szRet = errorMessage;
						EhsSprintf(errorMessage,"EhsFread item %d was '%c', not 'T'", nCount, pBuff[nCount]);
						break;
					}
				}
				
			}
			EhsFclose(pFile);
		}
		else
		{
			szRet = errorMessage;
			EhsSprintf(errorMessage,"Couldn't open filetest.txt for reading");
		}	
	}
	return szRet;
}

/**
 * Testing the file EOF detection capabilities. 
 */
const char* TgtTestFile_testEof(void)
{
	const char* szRet = NULL;
	ehs_char szError[100];
	ehs_FILE* f = EhsFopen("filetest.txt","w");
	if (f)
	{
		/* generate test data */
		ehs_char* cData = "ABCDEFGHIJ";
		EhsFwrite(cData,1,10,f);
		EhsFclose(f);
		
		f = EhsFopen("filetest.txt","r");
		if (f)
		{
			ehs_uint16 i;
			
			for (i = 0u; i < 10; i++)
			{
				if (EHS_EOF == EhsFgetc(f))
				{
					EhsSprintf(szError,"fgetc returned EHS_EOF at character %d",i);
					szRet = szError;
					break;
				}				
			}
			
			if (EHS_EOF != EhsFgetc(f))
			{
				EhsSprintf(szError,"fgetc didn't return EHS_EOF at end of file");
				szRet = szError;
			}
			
		}
		else
		{
			EhsSprintf(szError,"Couldn't open filetest.txt for reading");
			szRet = szError;
		}
	}
	else
	{
		szRet = szError;
		EhsSprintf(szError,"Couldn't open filetest.txt");
	}
	
	return szRet;
}


/**
 * Testing the EhsFconsumeLine oepration. Creates a file with line breaks in
 * to consume
 */
const char* TgtTestFile_testConsumeLine(void)
{
	const ehs_char* szRet = NULL;
	ehs_FILE* f = EhsFopen("filecons.txt","w");
	if (f)
	{
		EhsFprintf(f, "1234\n2222\r\n3\n456");
		EhsFclose(f);
	} else {
		szRet = "Couldn't open filecons for writing";
	}
	
	if (!szRet)
	{
		f = EhsFopen("filecons.txt","r");
		if (f)
		{
			if (EhsFgetc(f) != '1')
				szRet = "Expected to read '1'";
			else if (!EhsFconsumeLine(f))
				szRet = "Premature end of line after 1";
			else if (EhsFgetc(f) != '2')
				szRet = "Expected to read '2'";
			else if (!EhsFconsumeLine(f))
				szRet = "Premature end of line after 2";
			else if (EhsFgetc(f) != '3')
				szRet = "Expected to read '3'";
			else if (!EhsFconsumeLine(f))
				szRet = "Premature end of line after 3";
			else if (EhsFgetc(f) != '4')
				szRet = "Expected to read '4'";
			else if (EhsFconsumeLine(f))
				szRet = "Unexpected end of line after 4";
			EhsFclose(f);
		} else {
			szRet = "Couldn't open filecons for reading";
		}
	}
	return szRet;
}

/**
 * Testing the EhsFgetWord function
 */
const char* TgtTestFile_testGetWord(void)
{
	const ehs_char* szRet = NULL;
	ehs_char szInput[10];
	ehs_FILE* f = EhsFopen("filegetw.txt","w");
	if (f)
	{
		EhsFprintf(f, "hello \t  world\ntoo_long_to_fit_in_my_string\n numbers 1,22 ;3");
		EhsFclose(f);
	} else {
		szRet = "Couldn't open filegetw for writing";
	}
	
	if (!szRet)
	{
		f = EhsFopen("filegetw.txt","r");
		if (f)
		{
			if (!EhsFgetWord(f,szInput,10,NULL))
				szRet = "EhsFgetWord returned false for hello";
			else if (0 != EhsStrcmp("hello",szInput))
				szRet = "expected hello";
			else if (EhsFgetWord(f,szInput,10,NULL))
				szRet = "EhsFgetWord returned true for world";
			else if (0 != EhsStrcmp("world",szInput))
				szRet = "expected world";
			else if (EhsFgetWord(f,szInput,10,NULL))
				szRet = "EhsFgetWord returned true for too_long_ ...";
			else if (0 != EhsStrcmp("too_long_",szInput))
				szRet = "expected too_long_";
			else if (!EhsFgetWord(f,szInput,10,NULL))
				szRet = "EhsFgetWord returned false for numbers";
			else if (0 != EhsStrcmp("numbers",szInput))
				szRet = "expected numbers";
			else if (!EhsFgetWord(f,szInput,10,",;"))
				szRet = "EhsFgetWord returned false for 1";
			else if (0 != EhsStrcmp("1",szInput))
				szRet = "expected 1";
			else if (!EhsFgetWord(f,szInput,10,",;"))
				szRet = "EhsFgetWord returned false for 22";
			else if (0 != EhsStrcmp("22 ",szInput))
				szRet = szInput;
			else if (EhsFgetWord(f,szInput,10,NULL))
				szRet = "EhsFgetWord returned true for 3";
			else if (0 != EhsStrcmp("3",szInput))
				szRet = "expected 3";
			EhsFclose(f);
		} else {
			szRet = "Couldn't open filegetw for reading";
		}
	}
	return szRet;
}

/**
 * Check that fgets handling of end of file is as we expect it to be
 */
const char* TgtTestFile_testFgets(void)
{
	const ehs_char* szRet = NULL;
	ehs_char szInput[12];
	ehs_FILE* f = EhsFopen("filegets.txt","w");
	if (f)
	{
		EhsFprintf(f, "\nreally_long_string\nhello world");
		EhsFclose(f);
	} else {
		szRet = "Couldn't open filegets for writing";
	}
	
	if (!szRet)
	{
		f = EhsFopen("filegets.txt","r");
		if (f)
		{
			/* test reading a string with no content on it */
			if (!EhsFgets(szInput,12,f))
				szRet = "fgets failed before end of file";
			else if (0 != EhsStrcmp("\n",szInput))
				szRet = "Expected \"\\n\"";
			/* test fgets with a line longer than the buffer we specified */
			else if (!EhsFgets(szInput,12,f))
				szRet = "Expected fgets to return value after reading long string";
			else if (0 != EhsStrcmp("really_long",szInput))
				szRet = "fgets didn't get \"really_long\"";
			else if (!EhsFgets(szInput,12,f))	/* consume remainder of long string */
				szRet = "Expected fgets to return value after reading second part of long string";
			/* test fgets with a line that doesn't terminate in EOL (CRLF/LF) */
			else if (!EhsFgets(szInput,12,f))
				szRet = "Expected fgets to return value after reading \"hello world\"";
			else if (0 != EhsStrcmp("hello world",szInput))
				szRet = "fgets didn't get \"hello world\"";
			/* test that fgets detects end of file correctly */
			else if (EhsFgets(szInput,12,f))
				szRet = "Expected fgets to return null at end of file";
			EhsFclose(f);
		} else {
			szRet = "Couldn't open filegets for reading";
		}
	}
	return szRet;
	
}
