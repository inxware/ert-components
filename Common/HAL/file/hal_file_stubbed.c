/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/* TODO2024 where did the compact file system for devices that can't run littlefs?*/

/** @file hal_file_stubbed.c
 * This does absolutely no file system stuff at any level. It implements all file HAL is NOP functions
 * Definitions for Common utilities for supporting file access
 *
 * @author: inx limited
 *
 */

#include "hal_file.h"
#include "ehs_types.h"

/* If these are not defined as a direct libc implementation then we use the inx 16 bit definitions (e.g. stubs or emulated)*/
 ehs_FILE *EhsFopen(const ehs_char* fname, const ehs_char* fmode) {
    return NULL;
 }

ehs_sint16 EhsFclose(ehs_FILE* f) {
    return 0;
}

ehs_sint16 EhsFread(void* pBuff, ehs_uint16 nSize, ehs_uint16 nCount, ehs_FILE* f) {
    return 0;
}

ehs_sint16 EhsFwrite(void* pBuff, ehs_uint16 nSize, ehs_uint16 nCount, ehs_FILE* f){
    return 0;
}

ehs_sint16 EhsFgetc(ehs_FILE *f) {
    return 0;
}

ehs_char* EhsFgets(ehs_char* pBuff, ehs_uint16 nSize, ehs_FILE* f) {
    return NULL;
}

ehs_sint16 EhsFprintf(ehs_FILE *f, const ehs_char* fmt, ...){
    return 0;
}

ehs_sint16 EhsFscanf(ehs_FILE *f, const ehs_char* fmt, ...){
    return 0;
}

ehs_sint16 EhsSscanf(ehs_char *pBuff, const ehs_char* fmt, ...) {
    return 0;
}

ehs_sint16 EhsFflush(ehs_FILE* f) {
    return 0;
}

ehs_sint8 EhsFseek(ehs_FILE f,ehs_sint8 x,ehs_sint8 y) {
    return 0;
} 
ehs_sint8 EhsFtell(ehs_FILE f) {
    return 0;
}

ehs_sint8 EhsFrewind(ehs_FILE) {
    return 0;
}


/* This is used for selecting what kind of directory HAL file system commands will operate with */

ehs_char *EhsFreadDynamic(ehs_FILE *fp, ehs_uint8 *ret_code)
{
	
	return NULL;
}

/**
 * Consume the current line up until the end of line character
 * @param[in] pFile Pointer to the file to use
 * @return True if the end of line character was found, false otherwise, we've
 * probably hit the end of the file
 */


ehs_bool EhsFconsumeLine(ehs_FILE* pFile)
{
    return EHS_FALSE;
}


/* Initialise the fil system. some target installers won#t dothese */
ehs_bool EhsFInitFileSystem()
{

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
    return NULL;
}


/**
 * \brief Populates OS/install meta data for the EHS instance.
 *
 */
ehs_bool EhsHUpdateFilePathEnvironment(EhsMetaDataType * pEhsMetaData)
{

    return EHS_TRUE;
}


/**
 * Return the current application working directory as an absolute path
 * */

ehs_bool EhsHMetagetCurrentAppDir(ehs_char * szCanonicalFilePath)
{
    return EHS_FALSE;
}

/* This function should be called before application start to ensure that all
 * shellexecs, and scripts etc run in the application directory. This assumes the current path is adjascent to
 * the app working directory
 */

ehs_bool Ehs_CDtoApp()
{
   return EHS_FALSE;

}

/* This function fixes all directory seperators are consistent with the OS
 * THis is useful/necessary because paths maybe defined in application strings
 */
void EhscorrectSeperators(ehs_char * path)
{
 
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

    return EHS_FALSE;
}

/*
 * Creates a Path recursively add any parents needed.
 * This will work for app, user, or any other file location
 * If last_is_file is true We don't care if there is a file name at the end we wont make this a directory
 */

ehs_bool Ehs_MakePath(const ehs_char * path, ehs_bool last_is_file)
{
     return EHS_FALSE;
}

/*
 * This function is used for user file open functions - it creates the required path for the file rather than returning an error.
 *
 */

ehs_FILE* Ehs_PathOpen(ehs_char * path, const ehs_char * permissions)
{
    return NULL;
}

ehs_bool Ehs_Touch(ehs_char * path)
{

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
 return NULL;
}

/* We won't use macros here because these files may be implemented using different filesystem/flash methods in the future
 * These functions are deigned to allow files to opened in the canonical runtime structure if it exists, otherwise accessing
 * all files in the current directory
 */

/* File name utilities - ansi-C*/

ehs_bool EhsHRename(const ehs_char * szOrigFilename,
                    const ehs_char * szToFilename)
{
        return EHS_FALSE;
}

ehs_bool EhsHRemove(const ehs_char * szOrigFilename)
{

        return EHS_FALSE;
}

ehs_bool EhsHCopy(const ehs_char* szSrcFilename, const ehs_char* szDstFilename)
{
    return EHS_TRUE;
}

/** Tries to open a file in the system directory */
ehs_FILE* Ehs_SysFopen(const ehs_char * szFilename, const ehs_char * access)
{
return NULL;
}

/* opens a file relative to base dir */
ehs_FILE* Ehs_AppBaseFopen(const ehs_char * szFilename, const ehs_char * access)
{
  
        return NULL;
}

/** Tries to open a file in the active application directory */
ehs_FILE* Ehs_AppFopen(const ehs_char * szFilename, const ehs_char * access)
{
    return NULL;
}

/** This function tries to open a file in the user directory */
ehs_FILE* Ehs_UserFopen(const ehs_char * szFilename, const ehs_char * access)
{
    return NULL;
}

/** Make a directory in the user directory
 *  @todo need a mkdir -p option here to create upper dirs not present*/
ehs_bool Ehs_UserMkdir(char * szPathname)
{
    return EHS_FALSE;
}

ehs_bool Ehs_UserRmdir(char * szPathname)
{
    return EHS_FALSE;
}

#ifndef EHS_TARGET_FILE_SKIP_STAT
ehs_bool Ehs_UserStat(char * szPathname, struct stat *statbuf)
{
    return EHS_FALSE;
}


ehs_bool Ehs_UserUtime(char * szPathname, struct utimbuf *new_times)
{
    
        return EHS_FALSE;
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
        return EHS_FALSE;
}

ehs_bool Ehs_UserRename(const ehs_char * szOrigFilename,
                        const ehs_char * szToFilename)
{
   return EHS_FALSE;
}

ehs_uint8 Ehs_User_PathExists(char * szPathname)
{
    return 1;
}

ehs_uint8 Ehs_App_PathExists(char * szPathname)
{
    return 1;
}

/* return 0 if OK, 1 for error */
/* Duplicate with diferrent constants to keep component and kernel only version separate for future API refactoring */
ehs_bool Ehs_SysMkdir(char * szPathname)
{
    return 1;
}

/* return 0 if OK, 1 for error */
/* Duplicate with diferrent constants to keep component and kernel only version separate for future API refactoring */
ehs_bool Ehs_AppMkdir(char * szPathname)
{
    return 1;
}



ehs_bool Ehs_AppRmdir(char * szPathname)
{
        return EHS_FALSE;
}

ehs_bool Ehs_AppRm(char * szPathname)
{
    return EHS_FALSE;
}

ehs_bool Ehs_AppRename(const ehs_char * szOrigFilename,const ehs_char * szToFilename)
{
    return EHS_FALSE;
}


ehs_bool Ehs_SysRmdir(char * szPathname)
{
    return EHS_FALSE;
}

ehs_bool Ehs_SysRm(char * szPathname)
{
    return EHS_FALSE;
}

ehs_uint8 Ehs_Sys_PathExists(char * szPathname)
{
    return 1;
}

ehs_bool Ehs_SysRename(const ehs_char * szOrigFilename,
                       const ehs_char * szToFilename)
{
    return EHS_FALSE;
}

