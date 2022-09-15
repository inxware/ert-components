/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

/** @file hal_file.c
 * Definitions for Common utilities for supporting file access
 *
 * @author: inx limited
 *
 */

/**
 * @page Verification Verification report
 * @section target_timer
 * @anchor target_timer
 * @subsection misra MISRA compliance:
 * test.c demonstrated MISRA compliant on
 * Last modified on $Date:$
 *
 * This file contained no derogations to the MISRA standard.
 */

/*****************************************************************************/
/* Included files */

//#define EHSHL_MODULE_ID EHSH_LOG_MODULE_HAL_FILE

#include "globals.h"
#include "hal-api.h" /* required for active application directory */
/*****************************************************************************/
/* Declare macros and local typedefs used by this file */

/* Default path names */
#define EHS_RUNTIME_BIN_DIR_STRING       	"bin"
#define EHS_RUNTIME_APPDATA_DIR_STRING   	"appdata"
#define EHS_RUNTIME_SYSDATA_DIR_STRING   	"sysdata"
#define EHS_RUNTIME_USERDATA_DIR_STRING  	"userdata"
#define EHS_RUNTIME_USERDATA_POSTFIX_DIR_STRING  "brix" // We add this to $HOME to contain user data - only this will be wiped by a devman command then!
#define EHS_RUNTIME_DEVMAN_DIR_STRING  		"devman"
#define EHS_RUNTIME_APPDATAFALLBACKS_DIR_STRING  "fallbacks"
#define EHS_RUNTIME_OS_ROOT_STRING					 "/"

/*****************************************************************************/
/* Declare prototypes of local functions */

/*****************************************************************************/
/* Variables defined with file-scope */

/*****************************************************************************/
/* Variables defined with global-scope */

/*****************************************************************************/
/* Function definitions */

/**
 * Consume the current line up until the end of line character
 * @param[in] pFile Pointer to the file to use
 * @return True if the end of line character was found, false otherwise, we've
 * probably hit the end of the file
 */


ehs_bool EhsFconsumeLine(ehs_FILE* pFile)
{
    ehs_bool bRet = EHS_FALSE;

    if (pFile)
    {
        ehs_sint16 nChar;
        do
        {
            nChar = EhsFgetc(pFile);
            if (nChar == EHS_EOL)
                bRet = EHS_TRUE;
        }
        while ((nChar != EHS_EOF) && (nChar != EHS_EOL));
    }
    return bRet;
}


/* Initialise the fil system. some target installers won#t dothese */
ehs_bool EhsFInitFileSystem()
{
    ehs_bool ret = EHS_FALSE;
    ret = EhsTgtFilesystem_Init();
    ehs_char szCanonicalFilePath[EHS_MAXPATHLENGTH];
    EhsTF_tryCanonicPath(szCanonicalFilePath, EHS_RUNTIME_SYSDATA_DIR,"init.nfo", EHS_TRUE);
    Ehs_MakePath(szCanonicalFilePath,EHS_TRUE);

#ifdef EHS_ANDROID // rmove the 'defery when we make the following function available for all platforms
    //EhsTInitFileSystem(); do thi from the target main code as this has the ndk app context
#endif
    return EHS_TRUE;
}


/**
 * Get a single word from the current file. Word is terminated by whitespace unless
 * the szSeparators parameter is set. Doesn't trim leading whitespace/separators
 * @param[in] pFile File to get the word from
 * @param[in,out] szWord Pointer to the area of memory to hold the word we've just read
 * @param[in] nLen Maximum Length of the word we're going to read (including terminator)
 * @param[in] szSeparators Characters that separate one word from another. NULL means
 * white space. In the NULL case, leading white space is stripped from strings
 * @return Pointer to the characters we've read unless we encounter end of line or end of file.
 */
ehs_char* EhsFgetWord(ehs_FILE* pFile, ehs_char* szWord, ehs_uint16 nLen,
                      const ehs_char* szSeparators)
{
    ehs_sint16 nChar;
    ehs_uint16 nIdx = 0;
    ehs_bool bTerm = EHS_FALSE; /* have we hit a terminator - end of line or end of file? */
    ehs_bool bSeparator = EHS_FALSE; /* have we hit a separator? */
    ehs_char* szRet = NULL;

    /* adjust nLen so that we have room for the terminator. Put the terminator on the end
     * of the string in case we need it */
    nLen--;
    szWord[nLen] = '\0';

    /* loop until: we hit a separator, end of file */
    do
    {
        nChar = EhsFgetc(pFile);

        if (nChar == EHS_EOF || nChar == EHS_EOL) /*@todo this is used only for font reading so it does an EOL test here but shouldn't in the general case - this would be a delimitor \r or \n ...*/
            bTerm = EHS_TRUE;
        else
        {
            /* normal character */
            if (szSeparators)
                bSeparator = (ehs_bool)(0 != EhsStrchr(szSeparators, nChar));
            else
            {
                bSeparator = EhsStrIsSpace(nChar) != 0;
                if (bSeparator && (nIdx == 0))
                {
                    /* it isn't a separator, it's leading whitespace - ignore it */
                    bSeparator = EHS_FALSE;
                    continue;
                }
            }

            if (!bSeparator)
            {
                /* only copy characters as long as we can fit them into szWord.
                 * Continue to consume them however */
                if (nIdx < nLen)
                    szWord[nIdx++] = (ehs_char) nChar;
            }
        }
    }
    while (!bTerm && !bSeparator);
    szWord[nIdx] = '\0'; /* make sure that a string terminator is provided */

    if (!bTerm)
        szRet = szWord;

    return szRet;
}


/**
 * \brief Populates OS/install meta data for the EHS instance.
 *
 */
ehs_bool EhsHUpdateFilePathEnvironment(EhsMetaDataType * pEhsMetaData)
{
//	ehs_char szTemp[EHS_STRING_LENGTH_MAX];
    ehs_char * directory = NULL;
    ehs_bool bUseEhsPath = EHS_TRUE; //this is TRUE if we are using the EHS install path
    ehs_bool add_postfix_dir = EHS_FALSE; // set this if we want to add a directory onto out our ehs user path.

    /* check if we have an OS preferred user directory */
    bUseEhsPath = EHS_FALSE; // default is we use a user relative path if one is found
    directory = getenv("HOME");
    if (!(directory))   /* linux */
    {
        directory = getenv("USERPROFILE"); /* windows */
        if (!directory)
        {
            directory = getenv("APPDATA"); /* windows other option */
            if (!directory)
            {
                directory = getenv("HOMEPATH"); /* windows XP other option */
                if (!directory)
                {
                    directory = EHS_RUNTIME_USERDATA_DIR_STRING;
                    bUseEhsPath = EHS_TRUE; // can't find any environment vars, use the EHS install directory
                }
            }
            else add_postfix_dir = EHS_TRUE;
        }
        else add_postfix_dir = EHS_TRUE;
    }
    else add_postfix_dir = EHS_TRUE;
    // if home dir is root (or catchall error, zero len dir), change to use ehs dir instead as we don't want to use root dir
    if (EhsStrcmp(directory, "/root") == 0 || EhsStrcmp(directory, "/") == 0 || EhsStrlen(directory) == 0)
    {
        directory = EHS_RUNTIME_USERDATA_DIR_STRING;
        bUseEhsPath = EHS_TRUE; // use the EHS install directory
    }

    if (EhsStrlen(pEhsMetaData->zInstallRootDirectory))
    {
        if (bUseEhsPath == EHS_TRUE)
        {
            // build absolute path using ehs install path and directory
            EhsStrcpy(pEhsMetaData->zUserDirectory, EhsHMetaGetInstPath());
            EhsStrcat(pEhsMetaData->zUserDirectory, EHS_TD_FILES_SEPARATOR_STR);
            EhsStrcat(pEhsMetaData->zUserDirectory, directory);

        }
        else
        {
            // use directory as absolute path
            EhsStrcpy(pEhsMetaData->zUserDirectory, directory);
            if (add_postfix_dir) EhsStrcat(pEhsMetaData->zUserDirectory, EHS_TD_FILES_SEPARATOR_STR EHS_RUNTIME_USERDATA_POSTFIX_DIR_STRING); // add sub dire to $HOME locations.
        }
    }
    else
    {
        EhsStrcpy(pEhsMetaData->zUserDirectory, "/opt/ehs/userdata"); /* as good as guess as any... */
        EHSH_LOG_WARNING("EHS User Directory not set, Ehs Install Directory needs to be set first.");
    }
    return EHS_TRUE;
}




/**
 * Return the current application working directory as an absolute path
 * */

ehs_bool EhsHMetagetCurrentAppDir(ehs_char * szCanonicalFilePath)
{
    /* Add the current application directory */
    ehs_bool ret;
    if (EhsStrlen(EhsHMetaAppGetCurrent())>0)   // we neve want o default to nothing dfalt is always last senible place fpr apps if we've arsed up the state
    {
        ret = EhsTF_tryCanonicPath(szCanonicalFilePath, EHS_RUNTIME_APPDATA_DIR,EhsHMetaAppGetCurrent(), EHS_TRUE);
    }
    else
    {
        ret = EhsTF_tryCanonicPath(szCanonicalFilePath, EHS_RUNTIME_APPDATA_DIR,"default", EHS_TRUE);
    }
    //EhsStrcat(szCanonicalFilePath,EHS_TD_FILES_SEPARATOR);
    //EHSH_LOG_INFO("PBB EHSMetagetCurrentAppDir %s", szCanonicalFilePath);

    return ret;
}

/* This function should be called before application start to ensure that all
 * shellexecs, and scripts etc run in the application directory. This assumes the current path is adjascent to
 * the app working directory
 */

ehs_bool Ehs_CDtoApp()
{
    ehs_char szCanonicalFilePath[EHS_MAXPATHLENGTH];
    if (EhsHMetagetCurrentAppDir(szCanonicalFilePath))
    {
        return EhsTF_cd(szCanonicalFilePath);
    }
    else     /* directory doesn't exist */
    {
        return EHS_FALSE;
    }
}

/* This function fixes all directory seperators are consistent with the OS
 * THis is useful/necessary because paths maybe defined in application strings
 */
void EhscorrectSeperators(ehs_char * path)
{
    int i, len;
    len = EhsStrlen(path);
    for (i = 0; i < len; i++)   /* check and replace anything that could be either normal seperator with the corrected version */
    {
        if (path[i] == '\\' || path[i] == '/')
            path[i] = EHS_TD_FILES_SEPARATOR;/* Use the macro! */
    }

}

/***
 * @brief Tests to see if a directory is present
 * If so the filename is appended to form a path
 * If not the file name on its own is used.
 * This function is used to access files within the canonical EHS filesystem, but allows default
 * file IO to current directory if the required directory is not present
 * @param[out] derived canonical absolute path
 * @param[in] directory relevant to install base to try
 * @param[in] file/path to try
 * @param[in] force means produce a path even if directories in path do not exist
 */

ehs_bool EhsTF_tryCanonicPath(ehs_char * szCanonicalFilePath,RuntimePathType directory_type, const char * file, ehs_bool force)
{

    ehs_char * directory = NULL;
    ehs_bool bUseEhsPath = EHS_TRUE; //this is TRUE if we are using the EHS install path
    ehs_bool returnval = EHS_FALSE;
    /* Default location is in the install directory -we might overwright this! */

    /* Usually we use the install pat as the base - but we might override this for specific cases below */
    EhsStrcpy(szCanonicalFilePath, EhsHMetaGetInstPath());
    EhsStrcat(szCanonicalFilePath, EHS_TD_FILES_SEPARATOR_STR);

    switch (directory_type)
    {
    case EHS_RUNTIME_BIN_DIR:
        directory = EHS_RUNTIME_BIN_DIR_STRING;
        break;
    case EHS_RUNTIME_APPDATA_DIR:
        directory = EHS_RUNTIME_APPDATA_DIR_STRING;
        break;
    case EHS_RUNTIME_SYSDATA_DIR:
        directory = EHS_RUNTIME_SYSDATA_DIR_STRING;
        break;
    case EHS_RUNTIME_USERDATA_DIR:
        bUseEhsPath = EHS_FALSE; // EhsHMetaGetUserPath() returns a full path, may or may not be in EHS install path
        EhsStrcpy(szCanonicalFilePath, EhsHMetaGetUserPath()); // replace with full path
        //if (directory_type == EHS_RUNTIME_USERDATA_DIR) EhsFprintf(bob,"file-2=%s, szCanonicalFilePath=%s\n",file,szCanonicalFilePath);
        break;
    case EHS_RUNTIME_DEVMAN_DIR:
        directory = EHS_RUNTIME_DEVMAN_DIR_STRING;
        break;
    case EHS_RUNTIME_APPDATAFALLBACKS_DIR:
        directory = EHS_RUNTIME_APPDATAFALLBACKS_DIR_STRING;
        break;
    case EHS_RUNTIME_OS_ROOT: /* Priveleged Only! */
        EhsStrcpy(szCanonicalFilePath, EHS_TD_FILES_SEPARATOR_STR); // replace with full path
        bUseEhsPath = EHS_FALSE;
        directory = EHS_RUNTIME_OS_ROOT_STRING;
        break;
    default:
        directory = "."; /* default we just stay where we are.. */
        break;
    }
    if (bUseEhsPath == EHS_TRUE)
    {
        EhsStrcat(szCanonicalFilePath, directory); // @todo this is messy !
        //if (directory_type == EHS_RUNTIME_USERDATA_DIR) EhsFprintf(bob,"file-3=%s, szCanonicalFilePath=%s\n",file,szCanonicalFilePath);

    }
    //if (directory[EhsStrLen(directory)-1] != EHS_TD_FILES_SEPARATOR_STR ) EhsStrcat(szCanonicalFilePath, EHS_TD_FILES_SEPARATOR_STR); //Just in case
    if (EhsTF_exists(szCanonicalFilePath) > 0  )   //  1 or 2 is file or directory
    {
        EhsStrcat(szCanonicalFilePath, EHS_TD_FILES_SEPARATOR_STR);
        EhsStrcat(szCanonicalFilePath, file);
        returnval = EHS_TRUE;

    }
    else if (force)    // same action as above, but we set the return value to false.
    {
        EhsStrcat(szCanonicalFilePath, EHS_TD_FILES_SEPARATOR_STR);
        EhsStrcat(szCanonicalFilePath, file);
        returnval = EHS_FALSE;

    }
    else     /* else just return the file and leave it to CWD */
    {
        EhsStrcpy(szCanonicalFilePath, file);
        //if (directory_type == EHS_RUNTIME_USERDATA_DIR) EhsFprintf(bob,"file-6=%s, szCanonicalFilePath=%s\n",file,szCanonicalFilePath);
        returnval = EHS_FALSE;

    }
    EhscorrectSeperators(szCanonicalFilePath);
    return returnval;
}

/*
 * Creates a Path recursively add any parents needed.
 * This will work for app, user, or any other file location
 * If last_is_file is true We don't care if there is a file name at the end we wont make this a directory
 */

ehs_bool Ehs_MakePath(const ehs_char * path, ehs_bool last_is_file)
{
    ehs_char szDir[EHS_MAXPATHLENGTH];
    //	ehs_char *dir=path
    ehs_sint16 i, j=0;
    ehs_bool ret = EHS_FALSE;

    for (i = 0; i < EhsStrlen(path); i++)
    {
        if ((path[i] == '/') || (path[i] == '\\'))
        {
            EhsStrncpy(szDir, path, i);
            szDir[i] = 0; //null terminate
            if (EhsTF_exists(szDir) == 0)
                EhsTF_mkdir(szDir);
            j = i;
        }
        //dir++; // keep the base pointer for each directory
    }
    if ((!last_is_file) && (j < i))   //there is a directoryname at the end
    {
        if (EhsTF_exists(&path[j + 1]) == 0)
        {
            if (EhsTF_mkdir(&path[j + 1]))
            {
                ret = EHS_TRUE;
            }
        }
    }
    return ret;
}

/*
 * This function is used for user file open functions - it creates the required path for the file rather than returning an error.
 *
 */

ehs_FILE* Ehs_PathOpen(ehs_char * path, const ehs_char * permissions)
{

    Ehs_MakePath(path, EHS_TRUE); // this will make any necessary paths
    return EhsFopen(path, permissions);
}

ehs_bool Ehs_Touch(ehs_char * path)
{
    ehs_FILE *filething;
    Ehs_MakePath(path, EHS_TRUE); // this will make any necessary paths
    filething = EhsFopen(path, "w");
    if (filething)
    {
        fprintf(filething, "X");
        EhsFclose(filething);
        //exit(0);
        return EHS_TRUE;
        //exit(0);
    }
    else
        return EHS_FALSE;
}

/* this function identifies any directories after the Domain specifier
 * if there are none then the pointer points at the null at the end of the URL string
 * URL should not contain a final file - just the Base URL including directories if any.
 */

/* trusted client */
/* @todo this should include a paramter to strip the first n directories */
ehs_char* EhsGetPathFromURL(ehs_char* URL, ehs_sint32 nIgnoreDirs)   //@todo - naming convention
{
    ehs_uint32 i;
#define EHSGETPATH_URL_HEADER "://" //@todo this should work for all identifiers - possibly just use the ://?
    ehs_char* temp, *temp2;
    temp = EhsStrstr(URL, EHSGETPATH_URL_HEADER);
    if (temp)
    {
        temp = &temp[EhsStrlen(EHSGETPATH_URL_HEADER)]; //remove any URI type string
    }
    else
    {
        temp = URL;
    }
    if (nIgnoreDirs == -1)
    {
        for (i = EhsStrlen(URL); i >= 0; i--)
        {
            if (URL[i] == '/')
            {
                i++; /* increment to the beginning of the next word */
                break;
            }
            else
            {

            }
        }
        return &URL[i];

    }
    else
    {
        temp2 = EhsStrstr(temp, "/"); // find the beginning of the path
        if (temp2)   // yes there is at least one more
        {
            for (i = 0; i < nIgnoreDirs; i++)
            {
                temp = EhsStrstr(&temp2[1], "/");
                if (temp)
                    temp2 = temp;
                else
                    return temp2; //No more so return the last element
            }
            return &temp2[1]; // don't like pointer rithmatic so use &x[1]
        }
        else   // this was a plain Domain URL e.g. http://www.inx-systems.com
        {
            return (&URL[EhsStrlen(URL)]); /*return the null at the end of the pointer*/
        }
    }
}

/* We won't use macros here because these files may be implemented using different filesystem/flash methods in the future
 * These functions are deigned to allow files to opened in the canonical runtime structure if it exists, otherwise accessing
 * all files in the current directory
 */

/* File name utilities - ansi-C*/

ehs_bool EhsHRename(const ehs_char * szOrigFilename,
                    const ehs_char * szToFilename)
{
    //EHSH_LOG_ERROR("Renaming [%s] to [%s]",szOrigFilename,szToFilename);
    if (rename(szOrigFilename, szToFilename) == 0)
    {
        return EHS_TRUE;
    }
    else
    {
        return EHS_FALSE;
    }
}

ehs_bool EhsHRemove(const ehs_char * szOrigFilename)
{
    //EHSH_LOG_ERROR("Removing [%s]",szOrigFilename);
    if (remove(szOrigFilename) == 0)
    {
        return EHS_TRUE;
    }
    else
    {
        return EHS_FALSE;
    }
}

/** Tries to open a file in the system directory */
ehs_FILE* Ehs_SysFopen(const ehs_char * szFilename, const ehs_char * access)
{
    ehs_char szCanonicalFilePath[EHS_MAXPATHLENGTH];
    EhsTF_tryCanonicPath(szCanonicalFilePath, EHS_RUNTIME_SYSDATA_DIR,szFilename, EHS_TRUE);
    return EhsFopen(szCanonicalFilePath, access);
}

/* opens a file relative to base dir */
ehs_FILE* Ehs_AppBaseFopen(const ehs_char * szFilename, const ehs_char * access)
{
    ehs_char szCanonicalFilePath[EHS_MAXPATHLENGTH];
    EhsTF_tryCanonicPath(szCanonicalFilePath, EHS_RUNTIME_APPDATA_DIR,
                         szFilename, EHS_TRUE);
    /*FILE * f=EhsFopen("../SODLLoadLog.txt","w");
     fprintf (f,"ZZZZZZZZZZZZZZZZZZZZZZZZZZZ\nTrting to open %s\n",szCanonicalFilePath);
     EhsFclose(f);
     */
    return Ehs_PathOpen(szCanonicalFilePath, access); //*force creation if it doesn't exist */
}

/** Tries to open a file in the active application directory */
ehs_FILE* Ehs_AppFopen(const ehs_char * szFilename, const ehs_char * access)
{
    ehs_char szCanonicalFilePath[EHS_MAXPATHLENGTH];
    if (EhsHMetagetCurrentAppDir(szCanonicalFilePath))
    {
        EhsStrcat(szCanonicalFilePath, EHS_TD_FILES_SEPARATOR_STR);
        EhsStrcat(szCanonicalFilePath, szFilename);
        return EhsFopen(szCanonicalFilePath, access);
    }
    else
    {
        return EHS_FALSE;
    }
}

ehs_bool Ehs_DevmanMkdir(char * szPathname)
{
    ehs_char szCanonicalFilePath[EHS_MAXPATHLENGTH];
    EhsTF_tryCanonicPath(szCanonicalFilePath, EHS_RUNTIME_DEVMAN_DIR,
                         szPathname, EHS_TRUE);
    return (EhsTF_mkdir(szCanonicalFilePath));
}

/** Tries to open a file in the devman directories */
ehs_FILE* Ehs_DevmanFopen(const ehs_char * szFilename, const ehs_char * access)
{
    ehs_char szCanonicalFilePath[EHS_MAXPATHLENGTH];
    EhsTF_tryCanonicPath(szCanonicalFilePath, EHS_RUNTIME_DEVMAN_DIR,
                         szFilename, EHS_TRUE);
    //if ( EhsStrstr("w",access) || EhsStrstr("a",access) || EhsStrstr("+",access)) return Ehs_PathOpen(szCanonicalFilePath,access);
    return EhsFopen(szCanonicalFilePath, access); //Don't try to create if we are reading - return the error
}

/** This function tries to open a file in the user directory */
ehs_FILE* Ehs_UserFopen(const ehs_char * szFilename, const ehs_char * access)
{
    ehs_char szCanonicalFilePath[EHS_MAXPATHLENGTH];
    EhsTF_tryCanonicPath(szCanonicalFilePath, EHS_RUNTIME_USERDATA_DIR,szFilename, EHS_TRUE);
    if (EhsStrstr("w", access) || EhsStrstr("a", access) || EhsStrstr("+",access))
        return Ehs_PathOpen(szCanonicalFilePath, access);
    else
        return EhsFopen(szCanonicalFilePath, access); //Don't try to create if we are reading - return the error
}

/** Make a directory in the user directory
 *  @todo need a mkdir -p option here to create upper dirs not present*/
ehs_bool Ehs_UserMkdir(char * szPathname)
{
    ehs_char szCanonicalFilePath[EHS_MAXPATHLENGTH];
    EhsTF_tryCanonicPath(szCanonicalFilePath, EHS_RUNTIME_USERDATA_DIR,
                         szPathname, EHS_TRUE);
    return (EhsTF_mkdir(szCanonicalFilePath));
}

ehs_bool Ehs_UserRmdir(char * szPathname)
{
    ehs_char szCanonicalFilePath[EHS_MAXPATHLENGTH];
    if (EhsTF_tryCanonicPath(szCanonicalFilePath, EHS_RUNTIME_USERDATA_DIR,
                             szPathname, EHS_TRUE))
    {
        return (EhsTF_rmdir(szCanonicalFilePath));
    }
    else     //Don't remove anything - it might be the RFS now!
    {
        return EHS_FALSE;
    }
}

#ifndef EHS_TARGET_FILE_SKIP_STAT
ehs_bool Ehs_UserStat(char * szPathname, struct stat *statbuf)
{
    ehs_char szCanonicalFilePath[EHS_MAXPATHLENGTH];
    if (EhsTF_tryCanonicPath(szCanonicalFilePath, EHS_RUNTIME_USERDATA_DIR,szPathname, EHS_TRUE))
    {
        return EhsTF_stat(szCanonicalFilePath,statbuf);
    }
    else     //Don't remove anything - it might be the RFS now!
    {
        return EHS_FALSE;
    }
}


ehs_bool Ehs_UserUtime(char * szPathname, struct utimbuf *new_times)
{
    ehs_char szCanonicalFilePath[EHS_MAXPATHLENGTH];
    if (EhsTF_tryCanonicPath(szCanonicalFilePath, EHS_RUNTIME_USERDATA_DIR,szPathname, EHS_TRUE))
    {
        return EhsTF_utime(szCanonicalFilePath, new_times );
    }
    else     //Don't remove anything - it might be the RFS now!
    {
        return EHS_FALSE;
    }
}
#else
#warning using stubbed stat and utime functions!
ehs_bool Ehs_UserStat(char * szPathname, void *statbuf)
{
    return EHS_FALSE;
}

ehs_bool Ehs_UserUtime(char * szPathname, void *new_times)
{
    return EHS_FALSE;
}
#endif

/* remove a file */
ehs_bool Ehs_UserRm(char * szPathname)
{
    ehs_char szCanonicalFilePath[EHS_MAXPATHLENGTH];
    if (EhsTF_tryCanonicPath(szCanonicalFilePath, EHS_RUNTIME_USERDATA_DIR,
                             szPathname, EHS_TRUE))
    {
        return (EhsHRemove(szCanonicalFilePath));
    }
    else
        return EHS_FALSE;
}

ehs_bool Ehs_UserRename(const ehs_char * szOrigFilename,
                        const ehs_char * szToFilename)
{
    ehs_char szCanonicalFilePath[EHS_MAXPATHLENGTH];
    ehs_char szCanonicalFilePathTo[EHS_MAXPATHLENGTH];
    if (EhsTF_tryCanonicPath(szCanonicalFilePath, EHS_RUNTIME_USERDATA_DIR,szOrigFilename, EHS_TRUE))
    {
        EhsTF_tryCanonicPath(szCanonicalFilePathTo, EHS_RUNTIME_USERDATA_DIR,szToFilename, EHS_FALSE); // todo do we want to make this forced too? we didn't used to
        return EhsHRename(szCanonicalFilePath, szCanonicalFilePathTo);
    }
    else
    {
        return EHS_FALSE;
    }
}

ehs_uint8 Ehs_User_PathExists(char * szPathname)
{
    ehs_char szCanonicalFilePath[EHS_MAXPATHLENGTH];
    EhsTF_tryCanonicPath(szCanonicalFilePath, EHS_RUNTIME_USERDATA_DIR,
                         szPathname, EHS_TRUE);
    return (EhsTF_exists(szCanonicalFilePath) != 0);
}

ehs_uint8 Ehs_App_PathExists(char * szPathname)
{
    ehs_char szCanonicalFilePath[EHS_MAXPATHLENGTH];
    //EhsTF_tryCanonicPath(szCanonicalFilePath, EHS_RUNTIME_APPDATA_DIR ,szPathname,EHS_TRUE );
    EhsHMetagetCurrentAppDir(szCanonicalFilePath);
    EhsStrcat(szCanonicalFilePath, EHS_TD_FILES_SEPARATOR_STR);
    EhsStrcat(szCanonicalFilePath, szPathname);
    return (EhsTF_exists(szCanonicalFilePath) != 0);
}

/* return 0 if OK, 1 for error */
/* Duplicate with diferrent constants to keep component and kernel only version separate for future API refactoring */
ehs_bool Ehs_SysMkdir(char * szPathname)
{
    ehs_char szCanonicalFilePath[EHS_MAXPATHLENGTH];
    EhsTF_tryCanonicPath(szCanonicalFilePath, EHS_RUNTIME_SYSDATA_DIR,
                         szPathname, EHS_TRUE);
    return (EhsTF_mkdir(szCanonicalFilePath));
}

/* return 0 if OK, 1 for error */
/* Duplicate with diferrent constants to keep component and kernel only version separate for future API refactoring */
ehs_bool Ehs_AppMkdir(char * szPathname)
{
    ehs_char szCanonicalFilePath[EHS_MAXPATHLENGTH];
    EhsTF_tryCanonicPath(szCanonicalFilePath, EHS_RUNTIME_APPDATA_DIR,
                         szPathname, EHS_TRUE);
    return (EhsTF_mkdir(szCanonicalFilePath));
}


//


ehs_bool Ehs_AppRmdir(char * szPathname)
{
    ehs_char szCanonicalFilePath[EHS_MAXPATHLENGTH];
    if (EhsTF_tryCanonicPath(szCanonicalFilePath, EHS_RUNTIME_APPDATA_DIR,
                             szPathname, EHS_TRUE))
    {
        return (EhsTF_rmdir(szCanonicalFilePath));
    }
    else
        return EHS_FALSE;
}

ehs_bool Ehs_AppRm(char * szPathname)
{
    ehs_char szCanonicalFilePath[EHS_MAXPATHLENGTH];
    if (EhsTF_tryCanonicPath(szCanonicalFilePath, EHS_RUNTIME_APPDATA_DIR,
                             szPathname, EHS_TRUE))
    {
        return (EhsHRemove(szCanonicalFilePath));
    }
    else
        return EHS_FALSE;
}

ehs_bool Ehs_AppRename(const ehs_char * szOrigFilename,const ehs_char * szToFilename)
{
    ehs_char szCanonicalFilePath[EHS_MAXPATHLENGTH];
    ehs_char szCanonicalFilePathTo[EHS_MAXPATHLENGTH];
    if (EhsTF_tryCanonicPath(szCanonicalFilePath, EHS_RUNTIME_APPDATA_DIR,szOrigFilename, EHS_TRUE))
    {
        EhsTF_tryCanonicPath(szCanonicalFilePathTo, EHS_RUNTIME_APPDATA_DIR,szToFilename, EHS_TRUE);
        return EhsHRename(szCanonicalFilePath, szCanonicalFilePathTo);
    }
    else
        return EHS_FALSE;
}


ehs_bool Ehs_SysRmdir(char * szPathname)
{
    ehs_char szCanonicalFilePath[EHS_MAXPATHLENGTH];
    if (EhsTF_tryCanonicPath(szCanonicalFilePath, EHS_RUNTIME_SYSDATA_DIR,
                             szPathname, EHS_TRUE))
    {
        return (EhsTF_rmdir(szCanonicalFilePath));
    }
    else
        return EHS_FALSE;
}

ehs_bool Ehs_SysRm(char * szPathname)
{
    ehs_char szCanonicalFilePath[EHS_MAXPATHLENGTH];
    if (EhsTF_tryCanonicPath(szCanonicalFilePath, EHS_RUNTIME_SYSDATA_DIR,
                             szPathname, EHS_TRUE))
    {
        return (EhsHRemove(szCanonicalFilePath));
    }
    else
        return EHS_FALSE;
}

ehs_uint8 Ehs_Sys_PathExists(char * szPathname)
{
    ehs_char szCanonicalFilePath[EHS_MAXPATHLENGTH];
    EhsTF_tryCanonicPath(szCanonicalFilePath, EHS_RUNTIME_SYSDATA_DIR,
                         szPathname, EHS_TRUE);
    return (EhsTF_exists(szCanonicalFilePath) != 0);
}

ehs_bool Ehs_SysRename(const ehs_char * szOrigFilename,
                       const ehs_char * szToFilename)
{
    ehs_char szCanonicalFilePath[EHS_MAXPATHLENGTH];
    ehs_char szCanonicalFilePathTo[EHS_MAXPATHLENGTH];
    if (EhsTF_tryCanonicPath(szCanonicalFilePath, EHS_RUNTIME_SYSDATA_DIR,szOrigFilename, EHS_TRUE))
    {
        EhsTF_tryCanonicPath(szCanonicalFilePathTo, EHS_RUNTIME_SYSDATA_DIR,szToFilename, EHS_TRUE);
        return EhsHRename(szCanonicalFilePath, szCanonicalFilePathTo);
    }
    else
        return EHS_FALSE;
}

