/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file hal_file.h
 * In this file, all of the hardware abstraction layer declarations relating to file handling are given.
 *
 * @author: inx limited
 *
 */

/**
 * @page Verification Verification report
 * @section hal_file_h
 * @anchor hal_file_h
 * @subsection misra MISRA compliance:
 * Demonstrated MISRA compliant on
 * Last modified on $Date$
 *
 * This file contained the following derogations to the MISRA standard.
 * - Rule 16.1 Functions shall not have variable number of arguments. Required for target library functions.
 */

#ifndef EHS_HAL_FILE_H
#define EHS_HAL_FILE_H

/*****************************************************************************/
/* Included files */
#include "globals.h"

#ifndef EHS_FILESYSTEM_SUPPORT__STUBBED
    #include "target_file.h"
#endif

#include "hal.h" // needed for EhsMetaDataType

/*****************************************************************************/
/* Define macros  */

/*****************************************************************************/
/* Define types */

/* This is used for selecting what kind of directory HAL file system commands will operate with */
typedef enum
{
    EHS_RUNTIME_BIN_DIR,
    EHS_RUNTIME_APPDATA_DIR,
    EHS_RUNTIME_SYSDATA_DIR,
    EHS_RUNTIME_USERDATA_DIR,
    EHS_RUNTIME_DEVMAN_DIR,
    EHS_RUNTIME_APPDATAFALLBACKS_DIR,
    EHS_RUNTIME_OS_ROOT // Privileged!!
} RuntimePathType;

/* If we are not including a target_file.h we need ot define a few defaults to */
#ifdef EHS_FILESYSTEM_SUPPORT__STUBBED
#ifndef FILE
typedef int ehs_FILE;
#else
typedef FILE ehs_FILE;
#endif
/**
 * Longest possible path name (excluding filename, but including separators and
 * terminating zero).
 */
#ifndef EHS_KERNEL_BUILD
#ifndef EHS_TD_FILES_MAX_PATH
#define EHS_TD_FILES_MAX_PATH 1
#warning "EHS_TD_FILES_MAX_PATH is not defined setting in target_file.h"
#endif
#endif

/**
 * Longest possible filename
 */
#ifndef EHS_KERNEL_BUILD
#ifndef EHS_TD_FILES_MAX_FILENAME
#define EHS_TD_FILES_MAX_FILENAME 1
#warning "EHS_TD_FILES_MAX_FILENAME is not defined setting in target_file.h"
#endif
#endif

/**
 * Path separator for this target
 */
#ifndef EHS_MINGW
#define EHS_TD_FILES_SEPARATOR '/'
#define EHS_TD_FILES_SEPARATOR_STR "/"
#else
#define EHS_TD_FILES_SEPARATOR '\\'
#define EHS_TD_FILES_SEPARATOR_STR "\\"
#endif


#define EHS_EOF EOF

#endif
/* ehs_FILE is defined in target_file.h */

/*****************************************************************************/
/* Declare global variables */

/*****************************************************************************/
/* Declare function prototypes  */

/* within target_file.h, file functions are normally #def'd to their stdio equivalents.
 * If the target cannot use the stdio version, the #define is removed from target_file,
 * and the below function is used. The declarations below also provide a secondary
 * purpose - they show the prototype of the function in question.
 */

#define EHS_COMMON_DIRECTORY_SEPARATOR '/'
#define EHS_FILE_LOCALHOST_PREFIX "localhost:" /* This is use to allow non-secure access to arbitrary host OS files */

/* If these are not defined as a direct libc implementation then we use the inx 16 bit definitions (e.g. stubs or emulated)*/
#ifndef EhsFopen
 ehs_FILE *EhsFopen(const ehs_char* fname, const ehs_char* fmode);
#endif

#ifndef EhsFclose
ehs_sint16 EhsFclose(ehs_FILE* f);
#endif

#ifndef EhsFread
ehs_sint16 EhsFread(void* pBuff, ehs_uint16 nSize, ehs_uint16 nCount, ehs_FILE* f);
#endif

#ifndef EhsFwrite
ehs_sint16 EhsFwrite(void* pBuff, ehs_uint16 nSize, ehs_uint16 nCount, ehs_FILE* f);
#endif



#ifndef EhsFgetc
ehs_sint16 EhsFgetc(ehs_FILE *f);
#endif

#ifndef EhsFgets
ehs_char* EhsFgets(ehs_char* pBuff, ehs_uint16 nSize, ehs_FILE* f);
#endif

#ifndef EhsFprintf
ehs_sint16 EhsFprintf(ehs_FILE *f, const ehs_char* fmt, ...); /*lint !e960 Variable arguments required to support fprintf */
#endif

#ifndef EhsFscanf
ehs_sint16 EhsFscanf(ehs_FILE *f, const ehs_char* fmt, ...); /*lint !e960 Variable arguments required to support fscanf */
#endif

#ifndef EhsSscanf
ehs_sint16 EhsSscanf(ehs_char *pBuff, const ehs_char* fmt, ...); /*lint !e960 Variable arguments required to support fscanf */
#endif

#ifndef EhsFflush
ehs_sint16 EhsFflush(ehs_FILE* f);
#endif

/* End-of-file test. Defaults to libc feof() (identical to the prior direct
 * feof() call); targets whose ehs_FILE is not a stdio FILE* (e.g. the Zephyr
 * fd-backed HAL) override EhsFeof in their target_file.h. */
#ifndef EhsFeof
#define EhsFeof(f) feof(f)
#endif

#ifndef EhsFseek
ehs_sint8 EhsFseek(ehs_FILE f,ehs_sint8 x,ehs_sint8 y); 
#endif

#ifndef EhsFtell
ehs_sint8 EhsFtell(ehs_FILE f);
#endif

#ifndef EhsFrewind
ehs_sint8 EhsFrewind(ehs_FILE f);
#endif

/*
These are inx abstractions for normal libc file operations
*/

/* This is for creating any directories that may or may not be crated by the installer */
ehs_bool EhsFInitFileSystem();

ehs_bool Ehs_AppMkdir(char * szPathname);
ehs_FILE* Ehs_AppBaseFopen(const ehs_char * szFilename, const ehs_char * access);


/* Create an empty file and/or update the date to current */
ehs_bool Ehs_Touch(ehs_char * path);

/* Some directory utilities - not hugely standard but are ANSI-C*/
ehs_bool Ehs_MakePath(const ehs_char * path,ehs_bool last_is_file );
ehs_bool EhsHRename(const ehs_char * szOrigFilename,const ehs_char * szToFilename);
ehs_bool EhsHRemove(const ehs_char * szOrigFilename);
ehs_bool EhsHCopy(const ehs_char* szSrcFilename, const ehs_char* szDstFilename);

/* helper functions that are probably found in hal_file.c */

/**
 * @brief Read entire file with unknown size. User needs to free the string content and close the file pointer afterwards.
 * 
 * @param fp (Input) The file pointer to the file
 * @param ret_code (Output) The return code. 0: OK. 1: No memory left. 2: Read size is less than determined size.
 * @return ehs_char* Read content with allocted memory. Need to be freed with EhsHMem_tempFree
 *
 * @code {.C}
 * ehs_FILE *fp = EhsFopen("test.txt", 'r');
 * ehs_uint8 ret_code = 0;
 * ehs_char *content = EhsFreadDynamic(fp, &ret_code);
 * // Do something with the content
 * if (ret_code == 0) printf("%s\n", content);
 * if (fp != NULL)
 * {
 *      EhsFclose(fp);
 *      fp = NULL;
 * }
 * if (content != NULL)
 * {
 *      EhsHMem_tempFree(content);
 *      content = NULL;
 * }
 * @endcode
 */
ehs_char *EhsFreadDynamic(ehs_FILE *fp, ehs_uint8 *ret_code);

/**
 * Consume the current line up until the end of line character
 * @param[in] pFile Pointer to the file to use
 * @return True if the end of line character was found, false otherwise, we've
 * probably hit the end of the file
 */
ehs_bool EhsFconsumeLine(ehs_FILE* pFile);

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
ehs_char* EhsFgetWord(ehs_FILE* pFile, ehs_char* szWord, ehs_uint16 nLen, const ehs_char* szSeparators);

/* for accessing files that should be in directories but aren't always, force will return a canonical address even if it doesn't exist*/
ehs_bool EhsTF_tryCanonicPath(ehs_char * szParameterFilePath, RuntimePathType directory,const char * file, ehs_bool force);
/* this function tries to open a file in the user directory */
ehs_FILE* Ehs_UserFopen(const ehs_char * szFilename,const ehs_char * access);
/* this function tries to open a file in the system directory */
ehs_FILE* Ehs_SysFopen(const ehs_char * szFilename,const ehs_char * access);
/* this function tries to open a file in the app directory */
ehs_FILE* Ehs_AppFopen(const ehs_char * szFilename,const ehs_char * access) ;

/** \brief Component & system facing */
/* return 0 if OK, 1 for error */
ehs_bool Ehs_Mkdir(char * szParameterFilePath) ;

/** \brief Application Files API */
ehs_bool EhsHMetagetCurrentAppDir(ehs_char * szParameterFilePath);

/** \brief set up the file path environment */
ehs_bool EhsHUpdateFilePathEnvironment(EhsMetaDataType * pEhsMetaData);
/** \brief this function tries to open a file in the user directory */
ehs_bool Ehs_CDtoApp();
/** \brief check if a file or directory exists (note directres supported here for consistency rather than usefulness as we don't currently have subdirectories in applications) */
ehs_uint8 Ehs_App_PathExists(char * szPathname);
ehs_bool Ehs_AppRmdir(char * szPathname);
ehs_bool Ehs_AppRm(char * szPathname) ;
ehs_bool Ehs_AppRename(const ehs_char * szOrigFilename,const ehs_char * szToFilename);



/** \brief User Files API */
ehs_bool Ehs_UserMkdir(char * szPathname) ; /* return true on success */
ehs_bool Ehs_UserRmdir(char * szPathname) ;
ehs_uint8 Ehs_User_PathExists(char * szPathname);
ehs_bool Ehs_UserRm(char * szPathname);


#ifndef EHS_TARGET_FILE_SKIP_STAT
ehs_bool EhsTF_stat(ehs_char* szPath,struct stat *statbuf);
ehs_bool EhsTF_utime(ehs_char* szPath,struct utimbuf *new_times);
ehs_bool Ehs_UserStat(char * szPathname, struct stat *statbuf);
ehs_bool Ehs_UserUtime(char * szPathname, struct utimbuf *new_times);
#else
//stubbed versions - we should probably define an ehs_stat type that is void * for clients...
ehs_bool EhsTF_stat(ehs_char* szPath,void *statbuf);
ehs_bool EhsTF_utime(ehs_char* szPath,void *new_times);
ehs_bool Ehs_UserStat(char * szPathname, void *statbuf);
ehs_bool Ehs_UserUtime(char * szPathname, void *new_times);
#endif

ehs_bool Ehs_UserRename(const ehs_char * szOrigFilename,
                        const ehs_char * szToFilename);

/* These shouldn't be component facing */
/* return 0 if OK, 1 for error */
ehs_bool Ehs_SysMkdir(char * szPathname) ; /* return true on success */
ehs_bool Ehs_SysRmdir(char * szPathname) ;
ehs_uint8 Ehs_Sys_PathExists(char * szPathname);
ehs_bool Ehs_SysRm(char * szPathname);
ehs_bool Ehs_SysRename(const ehs_char * szOrigFilename,const ehs_char * szToFilename);
/* Declare target specifically implemented function prototypes  */

/**
 * Tests whether a file exists
 * @param fname Name of the file to test
 * @return true if the file exists
 */
ehs_bool EhsTF_mkdir(const char * szParameterFilePath); //@todo  get rif of all this TF prefix crap and call make one prefix for all HAL calls
ehs_uint8 EhsTF_exists(const ehs_char* fname);

/* Some URL string manipulation functions
 *
 */

/* this function identifies any directories after the Domain specifier
 * if there are none then the pointer points at the null at the end of the URL string
 * URL should not contain a final file - just the Base URL including directories if any.
 */

ehs_char* EhsGetPathFromURL(ehs_char* URL,ehs_sint32 nIgnoreDirs );


/**
 * Indicates the properties contained within the target viewport type
 */



/**
 * Contains the properties held about the video playback filesystem
 */
typedef struct EhsTDFilesStruct EhsTDFilesClass;



typedef enum { EHS_TD_FILEFLAG_NONE = 0,
               EHS_TD_FILEFLAG_FILE = 1,
               EHS_TD_FILEFLAG_FOLDER = 2
             } EhsTDFileFlagEnum;



/*****************************************************************************/
/* Declare global variables */

/**
 * Currently we only allow for one, global file context
 */

extern EhsTDFilesClass* EhsTDFilesRef;


#ifndef EhsTDFiles_init
/**
 * Set up the files strucutre
 * @param[in] pFiles Context for the file operation
 */
void EhsTDFiles_init(EhsTDFilesClass** pFiles);
#endif

#ifndef EhsTDFiles_cleanup
/**
 * Cleanup - close the current directory
 * @param[in] pFiles Context for the file operation
 */
void EhsTDFiles_cleanup(EhsTDFilesClass* pFiles);
#endif

#ifndef EhsTDFiles_getDir
/**
 * Return the directory we are curently looking at.
 * @param[in] pFiles Context for the file operation
 * @param[out] szDir Text containing the current path. Caller must pass in an array of length EHS_TD_FILES_MAX_PATH
 * @return true if successful
 */
ehs_bool EhsTDFiles_getDir(EhsTDFilesClass* pFiles, ehs_char* szDir);
#endif

#ifndef EhsTDFiles_setDir
/**
 * Change the directory we are currently looking at
 * @param[in] pFiles Context for the file operation
 * @param[in] szDir Directory to change to. "/" represents the root of the DTV file system (which might
 * not be the root of the underlying OS file system). Absolute and relative paths are supported.
 * @return true if successful.
 */
ehs_bool EhsTDFiles_setDir(EhsTDFilesClass* pFiles, const ehs_char* szDir);
#endif

#ifndef EhsTDFiles_listFirst
/**
 * Start retrieving files from the current directory
 * @param[in] pFiles Context for the file operation
 * @param[in] eFlags Indicates which types of things to report files, directories or both
 * @param[out] szName Text containing the name of the retrieved object. Caller must pass in an array
 * of length EHS_TD_FILES_MAX_FILENAME
 * @return true if a file name has been obtained
 */
ehs_uint8 EhsTDFiles_listFirst(EhsTDFilesClass* pFiles, ehs_char* szName);
#endif


#ifndef EhsTDFiles_listFirst
/* Count files in a directory
 *
 */
ehs_sint32 EhsTDFiles_countDirFiles ( const char *path );
#endif

#ifndef EhsTDFiles_listNext
/**
 * Continue retrieving files from the current directory. Keep calling this function until
 * it returns false.
 * @param[in] pFiles Context for the file operation
 * @param[out] szName Text containing the name of the retrieved object. Caller must pass in an array
 * of length EHS_TD_FILES_MAX_FILENAME
 * @return true if a file name has been obtained
 */
ehs_uint8 EhsTDFiles_listNext(EhsTDFilesClass* pFiles, ehs_char* szName);
#endif


/* return true if succesful */
#ifndef EhsTF_rmdir
ehs_bool EhsTF_rmdir(ehs_char* szPath);
#endif

#ifndef EhsTF_cd
ehs_bool EhsTF_cd(const ehs_char * dir);
#endif

/* Special file accessors */
/* FOr opening devman files */
ehs_FILE* Ehs_DevmanFopen(const ehs_char * szFilename,const ehs_char * access);
ehs_bool Ehs_DevmanMkdir(char * szPathname);

#endif /* EHS_HAL_FILE_H */
