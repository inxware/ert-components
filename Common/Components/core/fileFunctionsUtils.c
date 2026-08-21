/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/* fileFunctionsUtils.c
 *
 * Utility functions for use in the EHS system providing POSIX compliant file access.
 *
 * For definition of arguments in Ncapsa functions (Identify_, Init_ and Run_)
 * please see types.h.
 *
*/


#include "globals.h"
#include "ehs_fb_types.h"
#include "file.h"

#define CR '\r'
#define LF '\n'

/* This function will read a filename from a file block parameter string and split it into file name and optional extension
It will stop when it gets to an escaped space - which is a bit of a limiations
TODO we really need to allow this to use filename with spaces in! FOr now will duplicate this function for parsing dyanmic filenames which can have filenames with spaces in
this has an extension size of 64 bytes //TODO:STRINGLENGTH! trusted client??
*/

int GetFilename(const char *pParams, char *szFileName, char *tempExtension)
{
    int nChar;
    int i, nCount = 0;
    do
    {
        nChar = pParams[nCount];
        if (nChar == 0x1F)
           szFileName[nCount] = ' '; //lucid may send these instead of spaces and we would want to do this for files anyway
        else
           szFileName[nCount] = nChar;
        nCount++;
    }
    while(
           (nChar != 0x1E )    // accept endof recordalso
        && (nChar != '\0')
        && (nChar != ' ' ) //PBB 20201221 removed due to bad escape char || (nCount > 2 && pParams[nCount-2] == '\ ' )) // stop at a space or an escaped spaces - TODO - do we really want to stop at spaces is this the parm delimeter?
        && (nChar != '?' ) // this means we have a temp extension to read while writing truncated files.
        && (nCount < EHS_STRING_LENGTH_MAX) ); //TODO:STRINGLENGTH! Shoud this be max file path or string length? We prolly need a max value argument for this function or declare it trusted client.
    if ( nCount > 0 )szFileName[nCount-1] = '\0';

    if (nChar == '?')
    {
        for (i=0; i < 64 ; i++)
        {
            tempExtension[i] = pParams[nCount];
            if(pParams[nCount] == '\0' || pParams[nCount] == ' ') break;
            nCount++;
        }
        nCount++;
        tempExtension[i] = '\0';
    }
    else tempExtension[0] = '\0'; //sinals there isn't an extension

    return( nCount );  // return index of next field.
}


/* 
    Duplication fo above without space limitation - used for dynamic filename parsing, extensions are maxum 32 bytes wide. //TODO:STRINGLENGTH! trusted client
*/

int GetFilenameSplit_allowSpaces(const char *FullFilename, char *szFileName, char *tempExtension)
{
    int nChar;
    int i, nCount = 0;

    do
    {
        nChar = FullFilename[nCount];
        if (nChar == 0x1F)
           szFileName[nCount] = ' '; //lucid may send these instead of spaces and we would want to do this for files anyway
        else
           szFileName[nCount] = nChar;
        nCount++;
    }
    while(
           (nChar != 0x1E )    // accept endof recordalso
        && (nChar != '\0')
        && nChar != '?'  // this means we have a temp extension to read while writing truncated files.
        && (nCount < EHS_STRING_LENGTH_MAX) ); //TODO:STRINGLENGTH!
    if (nCount > 0 && nChar == '?') szFileName[nCount-1] = '\0';
    else szFileName[nCount] = '\0';

    if (nChar == '?')
    {
        for (i=0; i < 31 ; i++)
        {
            tempExtension[i] = FullFilename[nCount];
            if(FullFilename[nCount] == '\0') break;
            nCount++;
        }
        nCount++;
        tempExtension[i] = '\0';
    }
    else tempExtension[0] = '\0'; //sinals there isn't an extension

    return( nCount );  // return index of next field.
}


int GetWidth(const char *pParams, int nIndex, int *pWidth)
{
    int nChar;
    int nCount = 0;
    char sBuf[MAX_INTEGER_LEN+1];
    do
    {
        nChar = pParams[nIndex+nCount];
        sBuf[nCount] = nChar;
        nCount++;
    }
    while( (nChar != ' ') && (nChar != '\0') && (nCount < MAX_INTEGER_LEN) );
    sBuf[nCount-1] = '\0';   // need some error checking !
    *pWidth = atoi(sBuf);
    return( nIndex + nCount );  // return index of next field.
}

int GetPrecision(const char *pParams, int nIndex, int *nPrecision)
{
    int nChar;
    int nCount = 0;
    char sBuf[MAX_INTEGER_LEN+1];
    do
    {
        nChar = pParams[nIndex+nCount];
        sBuf[nCount] = nChar;
        nCount++;
    }
    while( (nChar != ' ') && (nChar != '\0') && (nCount < MAX_INTEGER_LEN) );
    sBuf[nCount-1] = '\0';   // need some error checking !
    *nPrecision = atoi(sBuf);
    return( nIndex + nCount );  // return index of next field.
}

int GetSeparator(const char *pParams, int nIndex, char *cSeparator)
{
    int nChar;
    int nCount=0;

    nChar = pParams[nIndex+nCount];

    if( nChar == '(' )  // could be "( )" or "()"
    {
        nCount++;
        nChar = pParams[nIndex+nCount];
        if( nChar == ')' )  // no separator specified.
        {
            *cSeparator = 0;
        }
        // check for carriage return and new line
        else if (nChar == '\\')
        {
            nCount++;
            nChar = pParams[nIndex+nCount];
            if (nChar == 'r')
            {
                *cSeparator = 10;
            }
            else if (nChar == 'n')
            {
                *cSeparator = 10;
            }
            else
            {
                *cSeparator = '\\';
            }
            //*cSeparator = nChar;
            nCount++;
        }
        else
        {
            *cSeparator = nChar;  // separator set to next character after '('.
            nCount++;
            nChar = pParams[nIndex+nCount];  // don't bother to check closing ')'.
        }
    }
    else                   // invalid separator
    {
        *cSeparator = 0;
    }

    return( nIndex + nCount + 2);  // return index of next field (after space).
}

int GetAppend(const char *pParams, int nIndex, int *pAppend)
{
    int nChar;
    int nCount = 0;
    char sBuf[MAX_APPEND_LEN+1];
    do
    {
        nChar = pParams[nIndex+nCount];
        sBuf[nCount] = nChar;
        nCount++;
    }
    while( (nChar != ' ') && (nChar != '\0') && (nCount < MAX_APPEND_LEN) );
    sBuf[nCount] = '\0';   // need some error checking !
    *pAppend = atoi(sBuf);
    return( nIndex + nCount );  // return index of next field.
}


/*
 * GetFixedWidthField
 *
 * Reads a fixed width data field from file. This is used by core EHS which is not alwats supported by proper string library - so we will re-write it!
 *
 */
int GetFixedWidthField( char *szBuffer, int *nWidth, ehs_FILE* sFile, int *nError )
{
    int i,nChar;
    int nMax = *nWidth>EHS_STRING_LENGTH_MAX?EHS_STRING_LENGTH_MAX:*nWidth; //TODO:STRINGLENGTH!
    for( i=0; i<nMax ; i++ )  // copy nr of chars specified by field width.
    {
        nChar = EhsFgetc( sFile );  // get next char.
        if(nChar == EHS_EOF)
        {
            szBuffer[i] = '\0';
            *nError = READ_ERROR_EOF;
            return i;
        }
        szBuffer[i] = nChar;       // add character to buffer.
    }
    szBuffer[nMax] = '\0';  // zero terminate string.
    *nError = NO_READ_ERRORS;
    return nMax;
}

int GetFixedWidthFieldReal(char *szBuffer, int *nWidth, ehs_FILE* sFile, int *nError )
{
    int i,nChar;
    int nMax = *nWidth + MIN_REAL_WRITE_WIDTH;  // width = min value + precision specified in nWidth.
    for( i=0; i<nMax ; i++ )  // copy nr of chars specified by field width.
    {
        nChar = EhsFgetc( sFile );  // get next char.
        if(nChar == EHS_EOF)
        {
            *nError = READ_ERROR_EOF;
            return i+1;
        }
        szBuffer[i] = nChar;       // add character to buffer.
    }
    szBuffer[nMax] = '\0';  // zero terminate string.
    *nError = NO_READ_ERRORS;
    return nMax;
}
