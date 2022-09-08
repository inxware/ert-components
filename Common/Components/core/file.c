/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

/* file.c
 *
 * functions for use in the EHS system providing POSIX compliant file access.
 *
 * For definition of arguments in Ncapsa functions (Identify_, Init_ and Run_)
 * please see types.h.
 *
*/

#include <math.h>
#include "target.h"
#include "file.h"
#include "setCompletes.h"

#include "hal-api.h"

#include "app_data.h"


#define CR '\r'
#define LF '\n'

/******************************************************************************/
/* Define FILE_ReadOnly_Bool function block */
//@todo These function blocks should include a method set the filename.

EHS_FB_FUNCTIONS_START(FILE_ReadOnly_Bool)

EHS_FB_FUNCTION_ENTRY("Run_OpenFILE_ReadOnly_Bool", 0x00, FILE_ReadOnly_Bool_Open)

EHS_FB_FUNCTION_ENTRY("Run_CloseFILE_ReadOnly_Bool", 0x01, FILE_ReadOnly_Bool_Close)

EHS_FB_FUNCTION_ENTRY("Run_ReadFILE_ReadOnly_Bool", 0x02, FILE_ReadOnly_Bool_Read)
EHS_FB_FUNCTIONS_END


/**
 * file object type structure...
 * @todo data type should be converted to compact ehs_ types
 */
typedef struct
{
    ehs_char szFilename[EHS_FILESTRING_SIZE];
    ehs_FILE *sFile;
    int nWidth;
    ehs_char cSeparator;
    int nPrecision;
    int bAppend;     // append data to file if EHS_TRUE else overwrite existing data.
    ehs_bool app_user;/* flag to look into app directory rather than user directory (read only)*/
    ehs_char szFileExtensionWhenOpen[32]; //This is a string name that is used during writing to a file (truncate mode only */
} structFileObj;


/**
 * Identify the function block. Determine the memory required.
 *
 * This function provides access to:
 *  EHS_FB_IDENTIFY_PARAMETERS - string containing parameter text
 *  EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this
 *   function block's context
 *
 */
EHS_FB_IDENTIFY_FUNCTION(FILE_ReadOnly_Bool)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(structFileObj);
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(FILE_ReadOnly_Bool)
{
    int nIndex = 0;
    char cSeparator;
    structFileObj* pFileObj = (structFileObj*)EHS_FB_INIT_CONTEXT;
    /* parse parameter string */

    nIndex = GetFilename(EHS_FB_INIT_PARAMETERS,pFileObj->szFilename,pFileObj->szFileExtensionWhenOpen);

    //strcpy( pFileObj->szFilename, szFilename ); // copy filename.
    nIndex = GetSeparator(EHS_FB_INIT_PARAMETERS, nIndex, &cSeparator);
    pFileObj->cSeparator = cSeparator;
    EhsSscanf(&((ehs_char*)EHS_FB_INIT_PARAMETERS)[nIndex],"%hhd",&pFileObj->app_user);
    pFileObj->sFile=NULL;
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(FILE_ReadOnly_Bool_Open)
{
    char szFilename[EHS_FILESTRING_SIZE] = {'\0'};
    structFileObj* pFileObj = NULL;
    ehs_FILE * sFile = NULL;

    NCAPSA_nOut(0)=ERR_NO_ERROR;
    pFileObj = (structFileObj*)EHS_FB_RUN_CONTEXT;

    // first close file if one is currently open
    sFile = pFileObj->sFile;
    if(sFile)
    {
        EhsFclose(sFile);
        pFileObj->sFile = NULL;
    }

    // if path port connected, use this instead of file parameter
    if (EHS_FB_IN_CONNECTED(0))
    {
        EhsStrcpy(szFilename,EHS_FB_IN_S(0));
    }
    else
    {
        /* get the filename from the state data...*/
        strcpy(szFilename, pFileObj->szFilename);
    }

    if (EhsStrlen(szFilename)>10 && EhsStrncmp(szFilename,EHS_FILE_LOCALHOST_PREFIX,EhsStrlen(EHS_FILE_LOCALHOST_PREFIX))==0)   // allow absolute path to local host
    {
        sFile = EhsFopen( &szFilename[EhsStrlen(EHS_FILE_LOCALHOST_PREFIX)], "r" );
    }
    else
    {
        if (pFileObj->app_user)
            sFile = Ehs_AppFopen( szFilename,"r");
        else
            sFile = Ehs_UserFopen( szFilename, "r" );
    }
    if( sFile == NULL )
    {
        /* write error flag specified by SODL */
        NCAPSA_nOut(0)=ERR_FAILED_TO_OPEN_FILE;
        SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }
    else
    {
        SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }
    /* copy the file descriptor to the state data...*/
    pFileObj->sFile = sFile;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(FILE_ReadOnly_Bool_Close)
{
    structFileObj* pFileObj = NULL;
    ehs_FILE * sFile = NULL;

    NCAPSA_nOut(0)=ERR_NO_ERROR;
    pFileObj = (structFileObj*)EHS_FB_RUN_CONTEXT;
    /* get the file descriptor from the state data...*/
    sFile = pFileObj->sFile;
    pFileObj->sFile = NULL;  // set file descriptor to invalid value.
    if( !sFile )   // check for invalid file descriptor.
    {
        NCAPSA_nOut(0)=ERR_INVALID_DESCRIPTOR;
        SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }
    else
    {
        if( EhsFclose(sFile) )
        {
            /* failure to close file */
            NCAPSA_nOut(0)=ERR_CLOSE_FILE_FAILED;
            SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
        }
        else
        {
            SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
        }
    }
//	SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(FILE_ReadOnly_Bool_Read)
{
    int cSeparator;
    ehs_sint16 cFlag;
    ehs_FILE *sFile = NULL;
    structFileObj* pFileObj = (structFileObj*)EHS_FB_RUN_CONTEXT;
    sFile = pFileObj->sFile;
    cSeparator = pFileObj->cSeparator;
    NCAPSA_nOut(1)=ERR_NO_ERROR;
    if( !sFile )  // invalid file descriptor.
    {
        NCAPSA_nOut(1)=ERR_INVALID_DESCRIPTOR;
        SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }
    else
    {
        cFlag = EhsFgetc(sFile);  // get boolean character ( 1 or 0 ).
        if( cSeparator )   // if separator is used.
        {
            EhsFgetc(sFile);  // get separator then discard.
        }
        if( cFlag == EHS_EOF )
        {
            NCAPSA_nOut(1)=ERR_END_OF_FILE;
            SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
//			return;
        }
        else if( cFlag == '0' )
        {
            NCAPSA_bOut(0) = EHS_FALSE;  // valid data output.
            SetCompletes1((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
        }
        else if( cFlag == '1' )
        {
            NCAPSA_bOut(0) = EHS_TRUE;  // valid data output.
            SetCompletes1((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
        }
        else
        {
            NCAPSA_nOut(1)=ERR_INVALID_FORMAT;
            SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
        }
    }
//	SetCompletes1((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
}

/******************************************************************************/
/* Define FILE_WriteOnly_Bool function block */

EHS_FB_FUNCTIONS_START(FILE_WriteOnly_Bool)

EHS_FB_FUNCTION_ENTRY("Run_OpenFILE_WriteOnly_Bool", 0x00, FILE_WriteOnly_Bool_Open)

EHS_FB_FUNCTION_ENTRY("Run_CloseFILE_WriteOnly_Bool", 0x01, FILE_WriteOnly_Bool_Close)

EHS_FB_FUNCTION_ENTRY("Run_WriteFILE_WriteOnly_Bool", 0x02, FILE_WriteOnly_Bool_Write)
EHS_FB_FUNCTIONS_END

/**
 * Identify the function block. Determine the memory required.
 *
 * This function provides access to:
 *  EHS_FB_IDENTIFY_PARAMETERS - string containing parameter text
 *  EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this
 *   function block's context
 *
 */
EHS_FB_IDENTIFY_FUNCTION(FILE_WriteOnly_Bool)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(structFileObj);
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(FILE_WriteOnly_Bool)
{
    int bAppend;
    char cSeparator;
    int nIndex = 0;
    //char szFilename[EHS_FILESTRING_SIZE] = {'\0'};
    structFileObj* pFileObj = (structFileObj*)EHS_FB_INIT_CONTEXT;

    /* parse parameter string */
    nIndex = GetFilename(EHS_FB_INIT_PARAMETERS,pFileObj->szFilename,pFileObj->szFileExtensionWhenOpen);
    //strcpy( pFileObj->szFilename, szFilename ); // copy filename.
    nIndex = GetSeparator(EHS_FB_INIT_PARAMETERS, nIndex, &cSeparator);
    pFileObj->cSeparator = cSeparator;
    nIndex = GetAppend(EHS_FB_INIT_PARAMETERS, nIndex, &bAppend);
    pFileObj->bAppend = bAppend;
    pFileObj->sFile=NULL;
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(FILE_WriteOnly_Bool_Open)
{
    char szFilename[EHS_FILESTRING_SIZE] = {'\0'};
    char *szOpenMode;
    char *szOpenModeApp="a";
    char *szOpenModeTrunc="w";
    structFileObj* pFileObj = NULL;
    ehs_FILE * sFile = NULL;
    int bAppend = EHS_FALSE;

    NCAPSA_nOut(0)=ERR_NO_ERROR;
    pFileObj = (structFileObj*)EHS_FB_RUN_CONTEXT;
    // first close file if one is currently open
    sFile = pFileObj->sFile;
    if(sFile)
    {
        EhsFclose(sFile);
        pFileObj->sFile = NULL;
    }
    // if path port connected, use this instead of file parameter
    bAppend = pFileObj->bAppend;
    if (EHS_FB_IN_CONNECTED(0))
    {
        GetFilenameSplit_allowSpaces(EHS_FB_IN_S(0), szFilename, pFileObj->szFileExtensionWhenOpen);
        EhsStrcpy(pFileObj->szFilename,szFilename); // we need to know original when we close the file if we are renaming
        //EhsStrcpy(szFilename,EHS_FB_IN_S(0));
    }
    else
    {
        /* get the filename from the state data...*/
        strcpy(szFilename, pFileObj->szFilename);
    }
    /* Rebuild path with a dot for the temporary extension */
    if (!bAppend && pFileObj->szFileExtensionWhenOpen[0] != '\0')
    {
        strcat(szFilename, ".");
        strcat(szFilename, pFileObj->szFileExtensionWhenOpen);
    }

    if( bAppend )
        szOpenMode=szOpenModeApp;
    else
        szOpenMode=szOpenModeTrunc;

    if (EhsStrlen(szFilename)>10 && EhsStrncmp(szFilename,EHS_FILE_LOCALHOST_PREFIX,EhsStrlen(EHS_FILE_LOCALHOST_PREFIX))==0)   // allow abolute path to local host
    {
        sFile = EhsFopen( &szFilename[EhsStrlen(EHS_FILE_LOCALHOST_PREFIX)], szOpenMode );
    }
    else
    {
        sFile = Ehs_UserFopen( szFilename, szOpenMode );
    }

    if( sFile == NULL )
    {
        /* write error flag specified by SODL */
        NCAPSA_nOut(0)=ERR_FAILED_TO_OPEN_FILE;
        SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }
    else
    {
        SetCompletes1((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }
    /* copy the file descriptor to the state data...*/
    pFileObj->sFile = sFile;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(FILE_WriteOnly_Bool_Close)
{
    structFileObj* pFileObj = NULL;
    ehs_FILE * sFile = NULL;

    NCAPSA_nOut(0)=ERR_NO_ERROR;
    pFileObj = (structFileObj*)EHS_FB_RUN_CONTEXT;
    /* get the file descriptor from the state data...*/
    sFile = pFileObj->sFile;
    if( !sFile )   // check for invalid file descriptor.
    {
        NCAPSA_nOut(0)=ERR_INVALID_DESCRIPTOR;
        SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }
    else if( EhsFclose(sFile) )
    {
        /* failure to close file */
        NCAPSA_nOut(0)=ERR_CLOSE_FILE_FAILED;
        SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }
    else
    {
        if (!pFileObj->bAppend && pFileObj->szFileExtensionWhenOpen[0] != '\0')
        {
            char szFilename[EHS_FILESTRING_SIZE] = {'\0'};
            strcpy(szFilename, pFileObj->szFilename);
            strcat(szFilename, ".");
            strcat(szFilename, pFileObj->szFileExtensionWhenOpen);
            Ehs_UserRm(pFileObj->szFilename);// force clobber the original file in this case
            if (Ehs_UserRename(szFilename,pFileObj->szFilename) == EHS_FALSE)
            {
                NCAPSA_nOut(0)=ERR_CLOSE_FILE_FAILED;
                SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
            }
            else SetCompletes1((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
        }
        else SetCompletes1((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }
    pFileObj->sFile = NULL;  // set file descriptor to invalid value.
    //	SetCompletes1((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(FILE_WriteOnly_Bool_Write)
{
    int cSeparator;
    int nIn;
    ehs_FILE *sFile = NULL;
    structFileObj* pFileObj = (structFileObj*)EHS_FB_RUN_CONTEXT;
    sFile = pFileObj->sFile;
    cSeparator = pFileObj->cSeparator;
    NCAPSA_nOut(0)=ERR_NO_ERROR;
    if( sFile == NULL )
    {
        /* write error flag specified by SODL */
        NCAPSA_nOut(0)=ERR_INVALID_DESCRIPTOR;
        SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }
    else
    {
        if( NCAPSA_bIn(0) )
            nIn = 1;
        else
            nIn = 0;

        if( cSeparator )
            EhsFprintf(sFile,"%d%c", nIn, cSeparator );  // write bool digit + separator.
        else
            EhsFprintf(sFile,"%d", nIn );  // write bool digit only.

        SetCompletes1((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }
//	SetCompletes1((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
}


/**
 * Run the function. Seems to be used to change teh filename. Not currently in use.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(FILE_WriteOnly_Bool_SetName)
{
    structFileObj* pFileObj = (structFileObj*)EHS_FB_RUN_CONTEXT;

    strcpy( pFileObj->szFilename, NCAPSA_szIn(0)); // copy filename.
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
}


#ifdef EHS_TARGET_FP_SUPPORT

/******************************************************************************/
/* Define FILE_ReadOnly_Float function block */

EHS_FB_FUNCTIONS_START(FILE_ReadOnly_Float)

EHS_FB_FUNCTION_ENTRY("Run_OpenFILE_ReadOnly_Real", 0x00, FILE_ReadOnly_Float_Open)

EHS_FB_FUNCTION_ENTRY("Run_CloseFILE_ReadOnly_Real", 0x01, FILE_ReadOnly_Float_Close)

EHS_FB_FUNCTION_ENTRY("Run_ReadFILE_ReadOnly_Real", 0x02, FILE_ReadOnly_Float_Read)
EHS_FB_FUNCTIONS_END

/**
 * Identify the function block. Determine the memory required.
 *
 * This function provides access to:
 *  EHS_FB_IDENTIFY_PARAMETERS - string containing parameter text
 *  EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this
 *   function block's context
 *
 */
EHS_FB_IDENTIFY_FUNCTION(FILE_ReadOnly_Float)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(structFileObj);
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(FILE_ReadOnly_Float)
{
    int nWidth;
    char cSeparator;
    int nIndex = 0;
    structFileObj* pFileObj = (structFileObj*)EHS_FB_INIT_CONTEXT;

    nIndex = GetFilename(EHS_FB_INIT_PARAMETERS,pFileObj->szFilename,pFileObj->szFileExtensionWhenOpen);

    nIndex = GetWidth(EHS_FB_INIT_PARAMETERS, nIndex, &nWidth);
    pFileObj->nWidth = nWidth;
    nIndex = GetSeparator(EHS_FB_INIT_PARAMETERS, nIndex, &cSeparator);
    pFileObj->cSeparator = cSeparator;
    EhsSscanf(&((ehs_char*)EHS_FB_INIT_PARAMETERS)[nIndex],"%hhd",&pFileObj->app_user);
    pFileObj->sFile=NULL;
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(FILE_ReadOnly_Float_Open)
{
    char szFilename[EHS_FILESTRING_SIZE] = {'\0'};
    structFileObj* pFileObj = NULL;
    ehs_FILE * sFile = NULL;

    NCAPSA_nOut(0)=ERR_NO_ERROR;
    pFileObj = (structFileObj*)EHS_FB_RUN_CONTEXT;

    // first close file if one is currently open
    sFile = pFileObj->sFile;
    if(sFile)
    {
        EhsFclose(sFile);
        pFileObj->sFile = NULL;
    }

    // if path port connected, use this instead of file parameter
    if (EHS_FB_IN_CONNECTED(0))
    {
        EhsStrcpy(szFilename,EHS_FB_IN_S(0));
    }
    else
    {
        /* get the filename from the state data...*/
        strcpy(szFilename, pFileObj->szFilename);
    }

    if (EhsStrlen(szFilename)>10 && EhsStrncmp(szFilename,EHS_FILE_LOCALHOST_PREFIX,EhsStrlen(EHS_FILE_LOCALHOST_PREFIX))==0)   // allow abolute path to local host
    {
        sFile = EhsFopen( &szFilename[EhsStrlen(EHS_FILE_LOCALHOST_PREFIX)], "r" );
    }
    else
    {
        if (pFileObj->app_user)
            sFile = Ehs_AppFopen(szFilename, "r");
        else
            sFile = Ehs_UserFopen(szFilename, "r");
    }
    if( sFile == NULL )
    {
        /* write error flag specified by SODL */
        NCAPSA_nOut(0)=ERR_FAILED_TO_OPEN_FILE;
        SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }
    else
    {
        SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }
    /* copy the file descriptor to the state data...*/
    pFileObj->sFile = sFile;
//	SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(FILE_ReadOnly_Float_Close)
{
    structFileObj* pFileObj = NULL;
    ehs_FILE * sFile = NULL;

    NCAPSA_nOut(0)=ERR_NO_ERROR;
    pFileObj = (structFileObj*)EHS_FB_RUN_CONTEXT;
    /* get the file descriptor from the state data...*/
    sFile = pFileObj->sFile;
    if( !sFile )   // check for invalid file descriptor.
    {
        NCAPSA_nOut(0)=ERR_INVALID_DESCRIPTOR;
        SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }
    else
    {
        if( EhsFclose(sFile) )
        {
            /* failure to close file */
            NCAPSA_nOut(0)=ERR_CLOSE_FILE_FAILED;
            SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
        }
        else
        {
            SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
        }
        pFileObj->sFile = NULL;  // set file descriptor to invalid value.
    }
//	SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(FILE_ReadOnly_Float_Read)
{
    int nRet;
    int nError,nChar;
    int nWidth,cSeparator;
    char szBuffer[MAX_READ_CHARS] = {'\0'};
    double dReadReal;
    ehs_FILE *sFile = NULL;
    structFileObj* pFileObj = (structFileObj*)EHS_FB_RUN_CONTEXT;
    sFile = pFileObj->sFile;
    nWidth = pFileObj->nWidth;  // reintroduced field width and scrapped precision for floats
    cSeparator = pFileObj->cSeparator;
    NCAPSA_nOut(1)=ERR_NO_ERROR;
    if( !sFile )  // invalid file descriptor.
    {
        nError = READ_ERROR_INVALID_FILE_DESCRIPTOR;
        NCAPSA_nOut(1)=ERR_INVALID_DESCRIPTOR;
        SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }
    else
    {
        if((!cSeparator) && (nWidth))  // no separator specified, just field width.
        {
            // reintroduced field width and scrapped precision for floats
            //nLen = GetFixedWidthFieldReal( szBuffer, &nWidth, sFile, &nError );
            GetFixedWidthField( szBuffer, &nWidth, sFile, &nError );
        }
        else  // no field width specified.
        {
            // reintroduced field width and scrapped precision for floats, so just read float and take any non numeric character as a delimiter
            // added %c to capture separating char
            nRet = EhsFscanf(sFile,"%lf%c",&dReadReal,(char*)&nChar);

            // output a warning if it is not an expected separator
            if (nChar != cSeparator && nChar != 0x0a && nChar != EHS_EOF)
            {
                EHSH_LOG_WARNING("Unexpected delimiter %c", nChar);
            }


            if( nRet != EHS_EOF )
            {
                NCAPSA_dOut(0) = dReadReal;
                SetCompletes1((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
                return;
            }
            else
            {
                nError = READ_ERROR_EOF;
            }
        }

        if( nError == READ_ERROR_EOF )
        {
            NCAPSA_nOut(1)=ERR_END_OF_FILE;
            SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
        }
        else if( nError > 0 )   // read errors other than EOF.
        {
            NCAPSA_nOut(1)=ERR_FILE_READ_ERROR;
            SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
        }
        else   // no read errors.
        {
            dReadReal = atof(szBuffer);  // convert string to float.
            if( dReadReal == 0.0 )    // either an error or actually 0.0.
            {
                NCAPSA_nOut(1)=ERR_INVALID_FORMAT;
                NCAPSA_dOut(0) = 0.0;
//				SetCompletes1((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
                SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
            }
            else
            {
                NCAPSA_dOut(0) = dReadReal;
                SetCompletes1((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
            }
        }
    }
}

/******************************************************************************/
/* Define FILE_WriteOnly_Float function block */

EHS_FB_FUNCTIONS_START(FILE_WriteOnly_Float)

EHS_FB_FUNCTION_ENTRY("Run_OpenFILE_WriteOnly_Real", 0x00, FILE_WriteOnly_Float_Open)

EHS_FB_FUNCTION_ENTRY("Run_CloseFILE_WriteOnly_Real", 0x01, FILE_WriteOnly_Float_Close)

EHS_FB_FUNCTION_ENTRY("Run_WriteFILE_WriteOnly_Real", 0x02, FILE_WriteOnly_Float_Write)
EHS_FB_FUNCTIONS_END

/**
 * Identify the function block. Determine the memory required.
 *
 * This function provides access to:
 *  EHS_FB_IDENTIFY_PARAMETERS - string containing parameter text
 *  EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this
 *   function block's context
 *
 */
EHS_FB_IDENTIFY_FUNCTION(FILE_WriteOnly_Float)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(structFileObj);
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(FILE_WriteOnly_Float)
{
    int nWidth, nPrecision;
    int bAppend;
    char cSeparator;
    int nIndex = 0;
    structFileObj* pFileObj = (structFileObj*)EHS_FB_INIT_CONTEXT;

    /* parse parameter string */
    nIndex = GetFilename(EHS_FB_INIT_PARAMETERS,pFileObj->szFilename,pFileObj->szFileExtensionWhenOpen);

    nIndex = GetWidth(EHS_FB_INIT_PARAMETERS, nIndex, &nWidth);
    pFileObj->nWidth = nWidth;
    nIndex = GetSeparator(EHS_FB_INIT_PARAMETERS, nIndex, &cSeparator);
    pFileObj->cSeparator = cSeparator;
    nIndex = GetPrecision(EHS_FB_INIT_PARAMETERS, nIndex, &nPrecision);
    pFileObj->nPrecision = nPrecision;
    nIndex = GetAppend(EHS_FB_INIT_PARAMETERS, nIndex, &bAppend);
    pFileObj->bAppend = bAppend;
    pFileObj->sFile=NULL;

    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(FILE_WriteOnly_Float_Open)
{
    char szFilename[EHS_FILESTRING_SIZE] = {'\0'};
    char * szOpenMode;
    char * szOpenModeApp="a";
    char * szOpenModeTrunc="w";
    structFileObj* pFileObj = NULL;
    ehs_FILE * sFile = NULL;
    int bAppend = EHS_FALSE;

    NCAPSA_nOut(0)=ERR_NO_ERROR;
    pFileObj = (structFileObj*)EHS_FB_RUN_CONTEXT;

    // first close file if one is currently open
    sFile = pFileObj->sFile;
    if(sFile)
    {
        EhsFclose(sFile);
        pFileObj->sFile = NULL;
    }
    bAppend = pFileObj->bAppend;
    // if path port connected, use this instead of file parameter
    if (EHS_FB_IN_CONNECTED(0))
    {
        GetFilenameSplit_allowSpaces(EHS_FB_IN_S(0), szFilename, pFileObj->szFileExtensionWhenOpen);
        EhsStrcpy(pFileObj->szFilename,szFilename); // we need to know original when we close the file if we are renaming
        //EhsStrcpy(szFilename,EHS_FB_IN_S(0));
    }
    else
    {
        /* get the filename from the state data...*/
        strcpy(szFilename, pFileObj->szFilename);
    }
    /* Rebuild path with a dot for the temporary extension */
    if (!bAppend && pFileObj->szFileExtensionWhenOpen[0] != '\0')
    {
        strcat(szFilename, ".");
        strcat(szFilename, pFileObj->szFileExtensionWhenOpen);
    }




    if( bAppend )
        szOpenMode=szOpenModeApp;
    else
        szOpenMode=szOpenModeTrunc;

    if (EhsStrlen(szFilename)>10 && EhsStrncmp(szFilename,EHS_FILE_LOCALHOST_PREFIX,EhsStrlen(EHS_FILE_LOCALHOST_PREFIX))==0)   // allow abolute path to local host
    {
        sFile = EhsFopen( &szFilename[EhsStrlen(EHS_FILE_LOCALHOST_PREFIX)], szOpenMode );
    }
    else
    {
        sFile = Ehs_UserFopen( szFilename, szOpenMode );
    }
    if( sFile == NULL )
    {
        /* write error flag specified by SODL */
        NCAPSA_nOut(0)=ERR_FAILED_TO_OPEN_FILE;
        SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }
    else
    {
        SetCompletes1((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }
    /* copy the file descriptor to the state data...*/
    pFileObj->sFile = sFile;
//	SetCompletes1((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(FILE_WriteOnly_Float_Close)
{
    structFileObj* pFileObj = NULL;
    ehs_FILE * sFile = NULL;

    NCAPSA_nOut(0)=ERR_NO_ERROR;
    pFileObj = (structFileObj*)EHS_FB_RUN_CONTEXT;
    /* get the file descriptor from the state data...*/
    sFile = pFileObj->sFile;
    if( !sFile )   // check for invalid file descriptor.
    {
        NCAPSA_nOut(0)=ERR_INVALID_DESCRIPTOR;
        SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }
    else if( EhsFclose(sFile) )
    {
        /* failure to close file */
        NCAPSA_nOut(0)=ERR_CLOSE_FILE_FAILED;
        SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }
    else
    {

        if (!pFileObj->bAppend && pFileObj->szFileExtensionWhenOpen[0] != '\0')
        {
            char szFilename[EHS_FILESTRING_SIZE] = {'\0'};
            strcpy(szFilename, pFileObj->szFilename);
            strcat(szFilename, ".");
            strcat(szFilename, pFileObj->szFileExtensionWhenOpen);
            Ehs_UserRm(pFileObj->szFilename);// force clobber the original file in this case
            if (Ehs_UserRename(szFilename,pFileObj->szFilename) == EHS_FALSE)
            {
                NCAPSA_nOut(0)=ERR_CLOSE_FILE_FAILED;
                SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
            }
            else SetCompletes1((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
        }
        else SetCompletes1((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }
    pFileObj->sFile = NULL;  // set file descriptor to invalid value.
//	SetCompletes1((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(FILE_WriteOnly_Float_Write)
{
    int nWidth,nSeparator,nPrecision;
    double dWriteReal;
    char cSign;
    ehs_FILE *sFile = NULL;
    structFileObj* pFileObj = (structFileObj*)EHS_FB_RUN_CONTEXT;
    sFile = pFileObj->sFile;
    nWidth = pFileObj->nWidth;
    nSeparator = pFileObj->cSeparator;
    nPrecision = pFileObj->nPrecision; // for reals, width specifies precision. i.e. nr of digits after decimal point.
    dWriteReal = NCAPSA_dIn(0);
    NCAPSA_nOut(0)=ERR_NO_ERROR;
    if( !sFile )  // invalid file descriptor.
    {
        NCAPSA_nOut(0)=ERR_INVALID_DESCRIPTOR;
        SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }
    else if( nPrecision > MAX_REAL_PRECISION )
    {
        NCAPSA_nOut(0)=ERR_INVALID_PRECISION;
        SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }
    else   // valid file descriptor.
    {

        if( nWidth )   // if width specified (> 0)
        {
            nWidth--;  	// width -1, to account for extra space that sign takes
            if( dWriteReal < 0.0 )    // force preceding sign for fixed width reasons.
            {
                dWriteReal = fabs(dWriteReal);
                cSign = '-';
            }
            else
            {
                cSign = '+';
            }
            if( nSeparator )  // if separator specified (> 0)
            {
                //EhsFprintf(sFile,"%c%010.0f%c",cSign,dWriteReal,nSeparator);  // fixed width write to file preceded by separator.
                EhsFprintf(sFile,"%c%0*.*f%c",cSign,nWidth,nPrecision,dWriteReal,nSeparator);  // fixed width write to file preceded by separator.
            }
            else
            {
                EhsFprintf(sFile,"%c%0*.*f",cSign,nWidth,nPrecision,dWriteReal);  // fixed width write to file without separator.
            }

        }
        else   // no width specified.
        {
            if( nSeparator )  // if separator specified (> 0)
            {
                EhsFprintf(sFile,"%0.*f%c",nPrecision,dWriteReal,nSeparator);  // variable width write to file preceded by separator.
            }
            else
            {
                EhsFprintf(sFile,"%0.*f",nPrecision,dWriteReal);  // variable width write to file without separator.
            }
        }
        SetCompletes1((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }
//	SetCompletes1((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
}

/**
 * Run the function. Seems to be used to change teh filename. Not currently in use.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(FILE_WriteOnly_Float_SetName)
{
    structFileObj* pFileObj = (structFileObj*)EHS_FB_RUN_CONTEXT;

    strcpy( pFileObj->szFilename, NCAPSA_szIn(0)); // copy filename.
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
}

#endif /*  EHS_TARGET_FP_SUPPORT */

/******************************************************************************/
/* Define FILE_ReadOnly_Int function block */

EHS_FB_FUNCTIONS_START(FILE_ReadOnly_Int)

EHS_FB_FUNCTION_ENTRY("Run_OpenFILE_ReadOnly_Int", 0x00, FILE_ReadOnly_Int_Open)

EHS_FB_FUNCTION_ENTRY("Run_CloseFILE_ReadOnly_Int", 0x01, FILE_ReadOnly_Int_Close)

EHS_FB_FUNCTION_ENTRY("Run_ReadFILE_ReadOnly_Int", 0x02, FILE_ReadOnly_Int_Read)
EHS_FB_FUNCTIONS_END

/**
 * Identify the function block. Determine the memory required.
 *
 * This function provides access to:
 *  EHS_FB_IDENTIFY_PARAMETERS - string containing parameter text
 *  EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this
 *   function block's context
 *
 */
EHS_FB_IDENTIFY_FUNCTION(FILE_ReadOnly_Int)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(structFileObj);
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(FILE_ReadOnly_Int)
{
    int nWidth;
    int nIndex = 0;
    char cSeparator;
    //char szFilename[EHS_FILESTRING_SIZE] = {'\0'};
    structFileObj* pFileObj = (structFileObj*)EHS_FB_INIT_CONTEXT;

    /* parse parameter string */
    //nRet = sscanf(pParams, "%s%d%s%d", szFilename, &nWidth, szSeparator, &nAppend );
    nIndex = GetFilename(EHS_FB_INIT_PARAMETERS,pFileObj->szFilename,pFileObj->szFileExtensionWhenOpen);
    nIndex = GetWidth(EHS_FB_INIT_PARAMETERS, nIndex, &nWidth);
    pFileObj->nWidth = nWidth;
    nIndex = GetSeparator(EHS_FB_INIT_PARAMETERS, nIndex, &cSeparator);
    pFileObj->cSeparator = cSeparator;
    EhsSscanf(&((ehs_char*)EHS_FB_INIT_PARAMETERS)[nIndex],"%hhd",&pFileObj->app_user);
    pFileObj->sFile = NULL;
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(FILE_ReadOnly_Int_Open)
{
    char szFilename[EHS_FILESTRING_SIZE] = {'\0'};
    structFileObj* pFileObj = NULL;
    ehs_FILE * sFile = NULL;

    NCAPSA_nOut(0)=ERR_NO_ERROR;
    pFileObj = (structFileObj*)EHS_FB_RUN_CONTEXT;


    /* first close file if one is currently open */
    sFile = pFileObj->sFile;
    if(sFile)
    {
        EhsFclose(sFile);
        pFileObj->sFile = NULL;
    }

    /* if path port connected, use this instead of file parameter */
    if (EHS_FB_IN_CONNECTED(0))
    {
        EhsStrcpy(szFilename,EHS_FB_IN_S(0));
    }
    else
    {
        /* get the filename from the state data...*/
        strcpy(szFilename, pFileObj->szFilename);
    }

    if (EhsStrlen(szFilename)>10 && EhsStrncmp(szFilename,EHS_FILE_LOCALHOST_PREFIX,EhsStrlen(EHS_FILE_LOCALHOST_PREFIX))==0)   // allow abolute path to local host
    {
        sFile = EhsFopen( &szFilename[EhsStrlen(EHS_FILE_LOCALHOST_PREFIX)], "r" );
    }
    else
    {
        if (pFileObj->app_user)
            sFile = Ehs_AppFopen(szFilename, "r");
        else
            sFile = Ehs_UserFopen(szFilename, "r");
    }
    if( sFile == NULL )
    {
        /* write error flag specified by SODL */
        NCAPSA_nOut(0)=ERR_FAILED_TO_OPEN_FILE;
        SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }
    else
    {
        SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }
    /* copy the file descriptor to the state data...*/
    pFileObj->sFile = sFile;
//	SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(FILE_ReadOnly_Int_Close)
{
    structFileObj* pFileObj = NULL;
    ehs_FILE * sFile = NULL;

    NCAPSA_nOut(0)=ERR_NO_ERROR;
    pFileObj = (structFileObj*)EHS_FB_RUN_CONTEXT;
    /* get the file descriptor from the state data...*/
    sFile = pFileObj->sFile;
    if( !sFile )   // check for invalid file descriptor.
    {
        NCAPSA_nOut(0)=ERR_INVALID_DESCRIPTOR;
        SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }
    else
    {
        if( EhsFclose(sFile) )
        {
            /* failure to close file */
            NCAPSA_nOut(0)=ERR_CLOSE_FILE_FAILED;
            SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
        }
        else
        {
            SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
        }
        pFileObj->sFile = NULL;  // set file descriptor to invalid value.
    }
//	SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(FILE_ReadOnly_Int_Read)
{
    int nRet;
    int nError = NO_READ_ERRORS;
    int nWidth,cSeparator;
    char szBuffer[MAX_READ_CHARS] = {'\0'};
    int nReadInt;
    char cSep;
    ehs_FILE *sFile = NULL;
    structFileObj* pFileObj = (structFileObj*)EHS_FB_RUN_CONTEXT;
    sFile = pFileObj->sFile;
    nWidth = pFileObj->nWidth;
    cSeparator = pFileObj->cSeparator;
    NCAPSA_nOut(1)=ERR_NO_ERROR;
    if( !sFile )  // invalid file descriptor.
    {
        nError = READ_ERROR_INVALID_FILE_DESCRIPTOR;
        NCAPSA_nOut(1)=ERR_INVALID_DESCRIPTOR;
        SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }
    else
    {
        if((!cSeparator) && (nWidth))  // no separator specified, just field width.
        {
            GetFixedWidthField( szBuffer, &nWidth, sFile, &nError );
        }
        else  // no field width specified.
        {
            nRet = EhsFscanf(sFile,"%d%c",&nReadInt,&cSep);
            if( nRet != EHS_EOF )
            {
                NCAPSA_nOut(0) = nReadInt;
                SetCompletes1((structFuncArg*)(structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
                return;
            }
            else
                nError = READ_ERROR_EOF;
        }

        if( nError == READ_ERROR_EOF )
        {
            NCAPSA_nOut(1)=ERR_END_OF_FILE;
            SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
        }
        else if( nError > 0 )   // read errors other than EOF.
        {
            NCAPSA_nOut(1)=ERR_FILE_READ_ERROR;
            SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
        }
        else   // no read errors.
        {
            nReadInt = atoi(szBuffer);  // convert string to int.
            if( !nReadInt )
            {
                NCAPSA_nOut(1)=ERR_INVALID_FORMAT;
                SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
            }
            else
            {
                NCAPSA_nOut(0) = nReadInt;
                SetCompletes1((structFuncArg*)(structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
            }
        }
    }
}

/******************************************************************************/
/* Define FILE_WriteOnly_Int function block */

EHS_FB_FUNCTIONS_START(FILE_WriteOnly_Int)

EHS_FB_FUNCTION_ENTRY("Run_OpenFILE_WriteOnly_Int", 0x00, FILE_WriteOnly_Int_Open)

EHS_FB_FUNCTION_ENTRY("Run_CloseFILE_WriteOnly_Int", 0x01, FILE_WriteOnly_Int_Close)

EHS_FB_FUNCTION_ENTRY("Run_WriteFILE_WriteOnly_Int", 0x02, FILE_WriteOnly_Int_Write)
EHS_FB_FUNCTIONS_END

/**
 * Identify the function block. Determine the memory required.
 *
 * This function provides access to:
 *  EHS_FB_IDENTIFY_PARAMETERS - string containing parameter text
 *  EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this
 *   function block's context
 *
 */
EHS_FB_IDENTIFY_FUNCTION(FILE_WriteOnly_Int)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(structFileObj);
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(FILE_WriteOnly_Int)
{
    int nWidth;
    int bAppend;
    char cSeparator;
    int nIndex = 0;
    //char szFilename[EHS_FILESTRING_SIZE] = {'\0'};
    structFileObj* pFileObj = (structFileObj*)EHS_FB_INIT_CONTEXT;

    /* parse parameter string */
    nIndex = GetFilename(EHS_FB_INIT_PARAMETERS,pFileObj->szFilename,pFileObj->szFileExtensionWhenOpen);
    nIndex = GetWidth(EHS_FB_INIT_PARAMETERS, nIndex, &nWidth);
    pFileObj->nWidth = nWidth;
    nIndex = GetSeparator(EHS_FB_INIT_PARAMETERS, nIndex, &cSeparator);
    pFileObj->cSeparator = cSeparator;
    nIndex = GetAppend(EHS_FB_INIT_PARAMETERS, nIndex, &bAppend);
    pFileObj->bAppend = bAppend;
    pFileObj->sFile=NULL;
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(FILE_WriteOnly_Int_Open)
{
    char szFilename[EHS_FILESTRING_SIZE] = {'\0'};
    char * szOpenMode;
    char * szOpenModeApp="a";
    char * szOpenModeTrunc="w";
    structFileObj* pFileObj = NULL;
    ehs_FILE * sFile = NULL;
    int bAppend = EHS_FALSE;

    NCAPSA_nOut(0)=ERR_NO_ERROR;
    pFileObj = (structFileObj*)EHS_FB_RUN_CONTEXT;


    // first close file if one is currently open
    sFile = pFileObj->sFile;
    if(sFile)
    {
        EhsFclose(sFile);
        pFileObj->sFile = NULL;
    }
    bAppend = pFileObj->bAppend;
    // if path port connected, use this instead of file parameter
    if (EHS_FB_IN_CONNECTED(0))
    {
        GetFilenameSplit_allowSpaces(EHS_FB_IN_S(0), szFilename, pFileObj->szFileExtensionWhenOpen);
        EhsStrcpy(pFileObj->szFilename,szFilename); // we need to know original when we close the file if we are renaming

        //EhsStrcpy(szFilename,EHS_FB_IN_S(0));
    }
    else
    {
        /* get the filename from the state data...*/
        strcpy(szFilename, pFileObj->szFilename);
    }
    /* Rebuild path with a dot for the temporary extension */

    if (!bAppend && pFileObj->szFileExtensionWhenOpen[0] != '\0')
    {
        strcat(szFilename, ".");
        strcat(szFilename, pFileObj->szFileExtensionWhenOpen);
    }



    if( bAppend )
        szOpenMode=szOpenModeApp;
    else
        szOpenMode=szOpenModeTrunc;

    if (EhsStrlen(szFilename)>10 && EhsStrncmp(szFilename,EHS_FILE_LOCALHOST_PREFIX,EhsStrlen(EHS_FILE_LOCALHOST_PREFIX))==0)   // allow abolute path to local host
    {
        sFile = EhsFopen( &szFilename[EhsStrlen(EHS_FILE_LOCALHOST_PREFIX)], szOpenMode );
    }
    else
    {
        sFile = Ehs_UserFopen( szFilename, szOpenMode );
    }
    if( sFile == NULL )
    {
        /* write error flag specified by SODL */
        NCAPSA_nOut(0)=ERR_FAILED_TO_OPEN_FILE;
        SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }
    else
    {
        SetCompletes1((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }
    /* copy the file descriptor to the state data...*/
    pFileObj->sFile = sFile;
//	SetCompletes1((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(FILE_WriteOnly_Int_Close)
{
    structFileObj* pFileObj = NULL;
    ehs_FILE * sFile = NULL;

    NCAPSA_nOut(0)=ERR_NO_ERROR;
    pFileObj = (structFileObj*)EHS_FB_RUN_CONTEXT;
    /* get the file descriptor from the state data...*/
    sFile = pFileObj->sFile;
    pFileObj->sFile = NULL;  // set file descriptor to invalid value.
    if( !sFile )   // check for invalid file descriptor.
    {
        NCAPSA_nOut(0)=ERR_INVALID_DESCRIPTOR;
        SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }
    else if( EhsFclose(sFile) )
    {
        /* failure to close file */
        NCAPSA_nOut(0)=ERR_CLOSE_FILE_FAILED;
        SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }
    else
    {
        if (!pFileObj->bAppend && pFileObj->szFileExtensionWhenOpen[0] != '\0')
        {
            char szFilename[EHS_FILESTRING_SIZE] = {'\0'};
            strcpy(szFilename, pFileObj->szFilename);
            strcat(szFilename, ".");
            strcat(szFilename, pFileObj->szFileExtensionWhenOpen);
            Ehs_UserRm(pFileObj->szFilename);// force clobber the original file in this case
            if (Ehs_UserRename(szFilename,pFileObj->szFilename) == EHS_FALSE)
            {
                NCAPSA_nOut(0)=ERR_CLOSE_FILE_FAILED;
                SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
            }
            else SetCompletes1((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
        }
        else SetCompletes1((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }
//	SetCompletes1((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(FILE_WriteOnly_Int_Write)
{
    int nWidth,nSeparator;
    int nWriteInt;
    char cSign;
    ehs_FILE *sFile = NULL;
    structFileObj* pFileObj = (structFileObj*)EHS_FB_RUN_CONTEXT;
    sFile = pFileObj->sFile;
    nWidth = pFileObj->nWidth;
    nSeparator = pFileObj->cSeparator;
    nWriteInt = NCAPSA_nIn(0);
    NCAPSA_nOut(0)=ERR_NO_ERROR;
    if( !sFile )  // invalid file descriptor.
    {
        NCAPSA_nOut(0)=ERR_INVALID_DESCRIPTOR;
        SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }
    else   // valid file descriptor.
    {


        if( nWidth )   // if width specified (> 0)
        {
            nWidth--;   // subtract 1 from width to allow space for the sign of the integer value
            if( nWriteInt < 0 )    // force preceding sign for fixed width reasons.
            {
                nWriteInt = fabs(nWriteInt);
                cSign = '-';
            }
            else
            {
                cSign = '+';
            }
            if( nSeparator )  // if separator specified (> 0)
            {
                EhsFprintf(sFile,"%c%0*d%c",cSign,nWidth,nWriteInt,nSeparator);  // fixed width write to file preceded by separator.
            }
            else
            {
                EhsFprintf(sFile,"%c%0*d",cSign,nWidth,nWriteInt);  // fixed width write to file without separator.
            }
        }
        else   // no width specified.
        {
            if( nSeparator )  // if separator specified (> 0)
            {
                EhsFprintf(sFile,"%d%c",nWriteInt,nSeparator);  // variable width write to file preceded by separator.
            }
            else
            {
                EhsFprintf(sFile,"%d",nWriteInt);  // variable width write to file without separator.
            }
        }
        SetCompletes1((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }
//	SetCompletes1((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
}


/**
 * Run the function. Seems to be used to change teh filename. Not currently in use.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(FILE_WriteOnly_Int_SetName)
{
    structFileObj* pFileObj = (structFileObj*)EHS_FB_RUN_CONTEXT;

    strcpy( pFileObj->szFilename, NCAPSA_szIn(0)); // copy filename.
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
}

/******************************************************************************/
/* Define FILE_ReadOnly_String function block */

EHS_FB_FUNCTIONS_START(FILE_ReadOnly_String)

EHS_FB_FUNCTION_ENTRY("Run_OpenFILE_ReadOnly_String", 0x00, FILE_ReadOnly_String_Open)

EHS_FB_FUNCTION_ENTRY("Run_CloseFILE_ReadOnly_String", 0x01, FILE_ReadOnly_String_Close)

EHS_FB_FUNCTION_ENTRY("Run_ReadFILE_ReadOnly_String", 0x02, FILE_ReadOnly_String_Read)
EHS_FB_FUNCTIONS_END

/**
 * Identify the function block. Determine the memory required.
 *
 * This function provides access to:
 *  EHS_FB_IDENTIFY_PARAMETERS - string containing parameter text
 *  EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this
 *   function block's context
 *
 */
EHS_FB_IDENTIFY_FUNCTION(FILE_ReadOnly_String)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(structFileObj);
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(FILE_ReadOnly_String)
{
    int nWidth;
    int nIndex = 0;
    char cSeparator;
    //char szFilename[EHS_FILESTRING_SIZE] = {'\0'};
    structFileObj* pFileObj = (structFileObj*)EHS_FB_INIT_CONTEXT;

    /* parse parameter string */
    nIndex = GetFilename(EHS_FB_INIT_PARAMETERS,pFileObj->szFilename,pFileObj->szFileExtensionWhenOpen);
    nIndex = GetWidth(EHS_FB_INIT_PARAMETERS, nIndex, &nWidth);
    pFileObj->nWidth = nWidth;
    nIndex = GetSeparator(EHS_FB_INIT_PARAMETERS, nIndex, &cSeparator);
    pFileObj->cSeparator = cSeparator;
    EhsSscanf(&((ehs_char*)EHS_FB_INIT_PARAMETERS)[nIndex],"%hhd",&pFileObj->app_user);
    pFileObj->sFile = NULL;
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(FILE_ReadOnly_String_Open)
{
    char szFilename[EHS_FILESTRING_SIZE] = {'\0'};
    structFileObj* pFileObj = NULL;
    ehs_FILE * sFile = NULL;

    NCAPSA_nOut(0)=ERR_NO_ERROR;
    pFileObj = (structFileObj*)EHS_FB_RUN_CONTEXT;

    /* first close file if one is currently open */
    sFile = pFileObj->sFile;
    if(sFile)
    {
        EhsFclose(sFile);
        pFileObj->sFile = NULL;
    }

    /* if path port connected, use this instead of file parameter */
    if (EHS_FB_IN_CONNECTED(0))
    {
        EhsStrcpy(szFilename,EHS_FB_IN_S(0));
    }
    else
    {
        /* get the filename from the state data...*/
        strcpy(szFilename, pFileObj->szFilename);
    }

    if (EhsStrlen(szFilename) > 10 && EhsStrncmp(szFilename, EHS_FILE_LOCALHOST_PREFIX,EhsStrlen(EHS_FILE_LOCALHOST_PREFIX))== 0)   // allow abolute path to local host
    {
        sFile = EhsFopen( &szFilename[EhsStrlen(EHS_FILE_LOCALHOST_PREFIX)], "r" );
    }
    else
    {
        if (pFileObj->app_user)
            sFile = Ehs_AppFopen(szFilename, "r");
        else
            sFile = Ehs_UserFopen(szFilename, "r");
    }

    /* copy the file descriptor to the state data...*/
    pFileObj->sFile = sFile;
    if( sFile == NULL )
    {
        /* write error flag specified by SODL */
        NCAPSA_nOut(0)=ERR_FAILED_TO_OPEN_FILE;
        SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }
    else
    {
        SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(FILE_ReadOnly_String_Close)
{
    structFileObj* pFileObj = NULL;
    ehs_FILE * sFile = NULL;

    NCAPSA_nOut(0) = ERR_NO_ERROR;
    pFileObj = (structFileObj*) EHS_FB_RUN_CONTEXT;
    /* get the file descriptor from the state data...*/
    sFile = pFileObj->sFile;
    pFileObj->sFile = NULL; // set file descriptor to invalid value.
    if (!sFile)
    {
        NCAPSA_nOut(0) = ERR_INVALID_DESCRIPTOR;
        SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }
    else
    {
        if (EhsFclose(sFile))
        {
            /* failure to close file */NCAPSA_nOut(0) = ERR_CLOSE_FILE_FAILED;
            SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
        }
        else
        {
            SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
        }
    }
//	SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(FILE_ReadOnly_String_Read)
{
    int nChar, i = 0;
    int nError = NO_READ_ERRORS;
    char szBuffer[EHS_STRING_LENGTH_MAX] = {'\0'};
    int nWidth,cSeparator;
    ehs_FILE *sFile = NULL;
    structFileObj* pFileObj = (structFileObj*)EHS_FB_RUN_CONTEXT;
    sFile = pFileObj->sFile;
    nWidth = pFileObj->nWidth;
    cSeparator = pFileObj->cSeparator;
    NCAPSA_nOut(1)=ERR_NO_ERROR;

    /* initialise szBuffer as empty to ensure that no errors result in
     * last time's output remaining
     */
    szBuffer[0] = '\0';

    if(!sFile)
    {
        nError = READ_ERROR_INVALID_FILE_DESCRIPTOR;
        NCAPSA_nOut(1)=ERR_INVALID_DESCRIPTOR;
        SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }
    else
    {
        if (nWidth) // width overrides any delimeters
        {
            /* no separator specified, just field width. */
            GetFixedWidthField( szBuffer, &nWidth, sFile, &nError );
            EhsStrcpy(NCAPSA_szOut(0), "Hello there ........................");
            EhsStrcpy(NCAPSA_szOut(0), szBuffer);
            SetCompletes1((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
        }
        /*
        //else if((cSeparator) && (nWidth))
        //{ / * both separator & field width specified. * /
        	GetMaxWidthFieldDelim( szBuffer, &nWidth, sFile, &nError );

        // loop thru chars until find the separator or reach EOF

        	nChar = EhsFgetc( sFile );
        	if (nChar == EHS_EOF) {
        		nError = READ_ERROR_EOF;
        	} else {
        		 while (nChar != cSeparator / *&& nChar != 0x0a* / && nChar != EHS_EOF) {
        			nChar = EhsFgetc( sFile );
        }
        }
        //			EhsFgetc( sFile );

        EhsStrcpy(NCAPSA_szOut(0), szBuffer);
        SetCompletes1((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);

        }
         */
        else
        {
            /* got a delimeter nut no fixed width */
            nChar = EhsFgetc( sFile );

            if (nChar == EHS_EOF)
            {
                nError = READ_ERROR_EOF;
            }
            else
            {
                while (nChar != cSeparator && nChar != EHS_EOF) /* note: used to include && nChar != 0x0a */
                {
                    szBuffer[i] = nChar;
                    i = i + 1;
                    if(i < EHS_STRING_LENGTH_MAX)
                    {
                        nChar = EhsFgetc( sFile );
                    }
                    else
                    {
                        //ran out of space so break;
                        break;
                    }
                }
                szBuffer[i] = '\0';
                EhsStrcpy(NCAPSA_szOut(0), szBuffer);
                SetCompletes1((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
            }
        }
        if( nError == READ_ERROR_EOF )
        {
            NCAPSA_nOut(1)=ERR_END_OF_FILE;
            SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
        }
        else if( nError > 0 )
        {
            NCAPSA_nOut(1)=ERR_FILE_READ_ERROR;
            SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
        }
    }
    /* in the meantime, SetCompletes1 is always called when read finishes */
//	EhsStrcpy(NCAPSA_szOut(0), szBuffer);
//	SetCompletes1((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
}

/******************************************************************************/
/* Define FILE_WriteOnly_String function block */

EHS_FB_FUNCTIONS_START(FILE_WriteOnly_String)

EHS_FB_FUNCTION_ENTRY("Run_OpenFILE_WriteOnly_String", 0x00, FILE_WriteOnly_String_Open)

EHS_FB_FUNCTION_ENTRY("Run_CloseFILE_WriteOnly_String", 0x01, FILE_WriteOnly_String_Close)

EHS_FB_FUNCTION_ENTRY("Run_WriteFILE_WriteOnly_String", 0x02, FILE_WriteOnly_String_Write)
EHS_FB_FUNCTIONS_END

/**
 * Identify the function block. Determine the memory required.
 *
 * This function provides access to:
 *  EHS_FB_IDENTIFY_PARAMETERS - string containing parameter text
 *  EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this
 *   function block's context
 *
 */
EHS_FB_IDENTIFY_FUNCTION(FILE_WriteOnly_String)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(structFileObj);
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(FILE_WriteOnly_String)
{
    int nWidth;
    char cSeparator;
    int bAppend;
    int nIndex = 0;
    structFileObj* pFileObj = (structFileObj*)EHS_FB_INIT_CONTEXT;

    /* parse parameter string */
    nIndex = GetFilename(EHS_FB_INIT_PARAMETERS,pFileObj->szFilename,pFileObj->szFileExtensionWhenOpen);
    nIndex = GetWidth(EHS_FB_INIT_PARAMETERS, nIndex, &nWidth);
    pFileObj->nWidth = nWidth;
    nIndex = GetSeparator(EHS_FB_INIT_PARAMETERS, nIndex, &cSeparator);
    pFileObj->cSeparator = cSeparator;
    nIndex = GetAppend(EHS_FB_INIT_PARAMETERS, nIndex, &bAppend);
    pFileObj->bAppend = bAppend;
    pFileObj->sFile=NULL;
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 *  @todo all these open functions should be the same function
 *  */
EHS_FB_RUN_FUNCTION(FILE_WriteOnly_String_Open)
{
    char szFilename[EHS_FILESTRING_SIZE] = {'\0'};
    char * szOpenMode;
    char * szOpenModeApp="a";
    char * szOpenModeTrunc="w";
    structFileObj* pFileObj = NULL;
    ehs_FILE * sFile = NULL;
    int bAppend = EHS_FALSE;
    NCAPSA_nOut(0)=ERR_NO_ERROR;
    pFileObj = (structFileObj*)EHS_FB_RUN_CONTEXT;

    // first close file if one is currently open
    sFile = pFileObj->sFile;
    if(sFile)
    {
        EhsFclose(sFile);
        pFileObj->sFile = NULL;
    }
    bAppend = pFileObj->bAppend;
    // if path port connected, use this instead of file parameter
    if (EHS_FB_IN_CONNECTED(0))
    {
        GetFilenameSplit_allowSpaces(EHS_FB_IN_S(0), szFilename, pFileObj->szFileExtensionWhenOpen);
        EhsStrcpy(pFileObj->szFilename,szFilename); // we need to know original when we close the file if we are renaming
    }
    else
    {
        /* get the filename from the state data...*/
        strcpy(szFilename, pFileObj->szFilename);
    }
    /* Rebuild path with a dot for the temporary extension */

    if (!bAppend && pFileObj->szFileExtensionWhenOpen[0] != '\0')
    {
        strcat(szFilename, ".");
        strcat(szFilename, pFileObj->szFileExtensionWhenOpen);
    }

    if( bAppend )
        szOpenMode=szOpenModeApp;
    else
        szOpenMode=szOpenModeTrunc;

    if (EhsStrlen(szFilename)>10 && EhsStrncmp(szFilename,EHS_FILE_LOCALHOST_PREFIX,EhsStrlen(EHS_FILE_LOCALHOST_PREFIX))==0)   // allow abolute path to local host
    {
        sFile = EhsFopen( &szFilename[EhsStrlen(EHS_FILE_LOCALHOST_PREFIX)], szOpenMode );
    }
    else
    {
        sFile = Ehs_UserFopen( szFilename, szOpenMode );
    }
    pFileObj->sFile = sFile;
    if( sFile == NULL )
    {
        /* write error flag specified by SODL */
        NCAPSA_nOut(0)=ERR_FAILED_TO_OPEN_FILE;
        SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }
    else
    {
        SetCompletes1((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }
    /* copy the file descriptor to the state data...*/

//	SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(FILE_WriteOnly_String_Close)
{
    structFileObj* pFileObj = NULL;
    ehs_FILE * sFile = NULL;

    NCAPSA_nOut(0)=ERR_NO_ERROR;
    pFileObj = (structFileObj*)EHS_FB_RUN_CONTEXT;
    /* get the file descriptor from the state data...*/
    sFile = pFileObj->sFile;
    pFileObj->sFile = NULL;  // set file descriptor to invalid value.
    if( !sFile )   // check for invalid file descriptor.
    {
        NCAPSA_nOut(0)=ERR_INVALID_DESCRIPTOR;
        SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }
    else if( EhsFclose(sFile) != 0 )
    {
        /* failure to close file */
        NCAPSA_nOut(0)=ERR_CLOSE_FILE_FAILED;
        SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }
    else
    {
        if (!pFileObj->bAppend && pFileObj->szFileExtensionWhenOpen[0] != '\0')
        {
            char szFilename[EHS_FILESTRING_SIZE] = {'\0'};
            strcpy(szFilename, pFileObj->szFilename);
            strcat(szFilename, ".");
            strcat(szFilename, pFileObj->szFileExtensionWhenOpen);
            Ehs_UserRm(pFileObj->szFilename);// force clobber the original file in this case
            if (Ehs_UserRename(szFilename,pFileObj->szFilename) == EHS_FALSE)
            {
                NCAPSA_nOut(0)=ERR_CLOSE_FILE_FAILED;
                SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);

            }
            else
            {

                SetCompletes1((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
            }
        }
        else
        {

            SetCompletes1((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
        }
    }
//	SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(FILE_WriteOnly_String_Write)
{
    int nWidth,nSeparator,i;
    char szWriteString[EHS_STRING_LENGTH_MAX+1] = {'\0'};
    ehs_FILE *sFile = NULL;
    ehs_bool bEndFound=0;
    char *pRet = NULL;
    structFileObj* pFileObj = (structFileObj*)EHS_FB_RUN_CONTEXT;
    sFile = pFileObj->sFile;
    NCAPSA_nOut(0)=ERR_NO_ERROR;
    nWidth = pFileObj->nWidth;
    nSeparator = pFileObj->cSeparator;
    if( sFile == NULL )
    {
        EHSH_LOG_WARNING("Trying to Writing to unopened file\n" );
        /* write error flag specified by SODL */
        NCAPSA_nOut(0)=ERR_INVALID_DESCRIPTOR;
        SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }
    else
    {
        pRet = EhsStrcpy ( szWriteString, NCAPSA_szIn(0) );  // copy string input.
        if( !pRet )   // string copy failed.
        {
            NCAPSA_nOut(0)=ERR_STRING_COPY_FAILED;
            SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
        }
        else if( !sFile )  // invalid file descriptor.
        {
            NCAPSA_nOut(0)=ERR_INVALID_DESCRIPTOR;
            SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
        }
        else // valid file descriptor.
        {
            if (nWidth) // if width specified (> 0)
            {
                for (i = 0; i < nWidth; i++)
                {
                    if (szWriteString[i] == '\0')   //@todo this assumes there's nulls after the entire string?
                    {
                        szWriteString[i] = ' ';
                        bEndFound = EHS_TRUE;
                    }
                    if (bEndFound)   // we need to pad the rest as well
                    {
                        szWriteString[i] = ' ';
                    }
                }
                szWriteString[nWidth] = '\0';
            }
            if (nSeparator) // if separator specified (> 0)
            {
                EhsFprintf(sFile, "%s%c", szWriteString, nSeparator); // fixed width write to file preceded by separator.
            }
            else
            {
                EhsFprintf(sFile, "%s", szWriteString); // fixed width write to file without separator.
            }
        }
        SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }
}

/**
 * Run the function. Seems to be used to change teh filename. Not currently in use.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(Run_SetName_String)
{
    structFileObj* pFileObj = (structFileObj*)EHS_FB_RUN_CONTEXT;

    strcpy( pFileObj->szFilename, NCAPSA_szIn(0)); // copy filename.
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
}

/** Destroy functions
 *  @todo we could join all these */


EHS_FB_DESTROY_FUNCTION(FILE_WriteOnly_Int)
{
    structFileObj* pFileObj = (structFileObj*)EHS_FB_RUN_CONTEXT;
    if (pFileObj->sFile)
    {
        EhsFclose(pFileObj->sFile);
    }
    return EHS_TRUE;
}

EHS_FB_DESTROY_FUNCTION(FILE_WriteOnly_Bool)
{
    structFileObj* pFileObj = (structFileObj*)EHS_FB_RUN_CONTEXT;
    if (pFileObj->sFile)
    {
        EhsFclose(pFileObj->sFile);
    }
    return EHS_TRUE;
}



EHS_FB_DESTROY_FUNCTION(FILE_WriteOnly_String)
{
    structFileObj* pFileObj = (structFileObj*)EHS_FB_RUN_CONTEXT;
    if (pFileObj->sFile)
    {
        EhsFclose(pFileObj->sFile);
    }
    return EHS_TRUE;
}


EHS_FB_DESTROY_FUNCTION(FILE_ReadOnly_Int)
{
    structFileObj* pFileObj = (structFileObj*)EHS_FB_RUN_CONTEXT;
    if (pFileObj->sFile)
    {
        EhsFclose(pFileObj->sFile);
    }
    return EHS_TRUE;
}

EHS_FB_DESTROY_FUNCTION(FILE_ReadOnly_Bool)
{
    structFileObj* pFileObj = (structFileObj*)EHS_FB_RUN_CONTEXT;
    if (pFileObj->sFile)
    {
        EhsFclose(pFileObj->sFile);
    }
    return EHS_TRUE;
}



EHS_FB_DESTROY_FUNCTION(FILE_ReadOnly_String)
{
    structFileObj* pFileObj = (structFileObj*)EHS_FB_RUN_CONTEXT;
    if (pFileObj->sFile)
    {
        EhsFclose(pFileObj->sFile);
    }
    return EHS_TRUE;
}

#ifdef EHS_TARGET_FP_SUPPORT
EHS_FB_DESTROY_FUNCTION(FILE_WriteOnly_Float)
{
    structFileObj* pFileObj = (structFileObj*)EHS_FB_RUN_CONTEXT;
    if (pFileObj->sFile)
    {
        EhsFclose(pFileObj->sFile);
    }
    return EHS_TRUE;
}

EHS_FB_DESTROY_FUNCTION(FILE_ReadOnly_Float)
{
    structFileObj* pFileObj = (structFileObj*)EHS_FB_RUN_CONTEXT;
    if (pFileObj->sFile)
    {
        EhsFclose(pFileObj->sFile);
    }
    return EHS_TRUE;
}
#endif
