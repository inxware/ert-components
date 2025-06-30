/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

/** @file target_file.c
 * Functions supporting file usage on the SH4 platform.
 *
 * @author: inx limited
 *
 */


/**
 * @page Verification Verification report
 * @section hal_graphics_font
 * @anchor hal_graphics_font
 * @subsection misra MISRA compliance:
 * test.c demonstrated MISRA compliant on
 * Last modified on $Date:$
 *
 * This file contained no derogations to the MISRA standard.
 */

/*****************************************************************************/
/* Included files */

#include <mbed_config.h>
#include <LittleFileSystem2.h>
#include <BlockDevice.h>

//#define EHSL_MODULE_ID (EHSH_LOG_MODULE_HAL_FILE)
#include "target_file.h"
#include "messages.h"
#include "hal-api.h"

/*****************************************************************************/
/* Declare macros and local typedefs used by this file */


/**
 * Contains the properties held about a directory
 */
struct EhsTDFilesStruct
{
    ehs_char szPath[EHS_TD_FILES_MAX_PATH];	/* the full "current path". Always begins with EHS_TD_FILES_ROOT */
    void* pDirectoryInfo;
    //EhsTDFileFlagEnum eSearch;					/* search types currently in force */
};


/*****************************************************************************/
/* Declare prototypes of local functions */

/*****************************************************************************/
/* Variables defined with file-scope */

/*****************************************************************************/
/* Variables defined with global-scope */

/*****************************************************************************/
/* Function definitions */
void EhsPrintPermissions(const struct stat *st);

/*****************************************************************************/
/* Declare macros and local typedefs used by this file */

#define EHS_FS_ARDUINO_DEBUG 1
#if EHS_FS_ARDUINO_DEBUG == 1
#define EHS_ARDUINO_FS_LOG(...) EhsStdioPrintf(__VA_ARGS__)
#else
#define EHS_ARDUINO_FS_LOG(...)
#endif

/**
 * Indicates that the search for files isn't currently taking place
 */
#define EHS_TD_FILES_SEARCH_INVALID -1

#ifndef EHS_BASE_DIR_NAME
#define EHS_BASE_DIR_NAME "ehs"
#endif
#define EHS_BASE_DIR "/" EHS_BASE_DIR_NAME

#ifndef EHS_LFS_READ_SIZE
#define EHS_LFS_READ_SIZE MBED_LFS_READ_SIZE
#endif
#ifndef EHS_LFS_PROG_SIZE
#define EHS_LFS_PROG_SIZE MBED_LFS_PROG_SIZE
#endif
#ifndef EHS_LFS_BLOCK_SIZE
#define EHS_LFS_BLOCK_SIZE MBED_LFS_BLOCK_SIZE
#endif
#ifndef EHS_LFS_LOOKAHEAD
#define EHS_LFS_LOOKAHEAD MBED_LFS_LOOKAHEAD
#endif
////////////////////////////////////////////////////////////////////
// @TODO - temporary solution
// ! using sodl bin converted to buffer as a default app !
#include "sodl_bin.c"
const ehs_char* default_sodl = sodl_binary_data;
////////////////////////////////////////////////////////////////////

/* Globals variables ! */
mbed::BlockDevice *bd = mbed::BlockDevice::get_default_instance();  // Use default block device (flash)
mbed::LittleFileSystem2 fs(EHS_BASE_DIR_NAME, bd, EHS_LFS_READ_SIZE, EHS_LFS_PROG_SIZE, 
                           EHS_LFS_BLOCK_SIZE, EHS_LFS_LOOKAHEAD);  // Adjusted parameters for performance
ehs_bool fsMounted = EHS_FALSE;

/* Initalise and mount the arduino file system */
ehs_bool EhsArdurinoFsInit()
{
    if(fsMounted == EHS_TRUE){
        return EHS_TRUE;
    }
    EHS_ARDUINO_FS_LOG("Initializing LittleFS\n");
    // mount the file system
    int err = fs.mount(bd);
    if (err != 0) {
        EHS_ARDUINO_FS_LOG("Mount failed err=%d. Formatting...\n", err);
        err = fs.reformat(bd);
        if (err != 0) {
            EHS_ARDUINO_FS_LOG("Formatting failed err=%d!\n", err);
            return EHS_FALSE;
        }
        EHS_ARDUINO_FS_LOG("Formatting successful!\n");
    }
    fsMounted = EHS_TRUE;
    return EHS_TRUE;
}

void EhsArdurinoFsUnmount()
{
    fsMounted = EHS_FALSE;
    fs.unmount();  // Unmount the filesystem after use
    EHS_ARDUINO_FS_LOG("LittleFS Unmounted\n");
}

ehs_bool EhsCreateDir(const char *path){
    if(path == NULL || path[0] == '\0'){
        return EHS_FALSE;
    }
    if( EhsTF_exists( path ) != 2 ) {
        EHS_ARDUINO_FS_LOG("Create '%s'\n", path );
        if( EhsTF_mkdir( path ) == EHS_FALSE ) { 
            return EHS_FALSE;
        }
        if( EhsTF_exists( path ) != 2 ){
            return EHS_FALSE;
        }
    }
    EHS_ARDUINO_FS_LOG("'%s' ready !\n", path);
    return EHS_TRUE;
}

void EhsWriteFile(const char *path, void *buffer, size_t size) 
{
    EHS_ARDUINO_FS_LOG("write '%s'\n", path);
    FILE *file = fopen(path, "wb");  // Open file in binary write mode
    if (!file) {
        EHS_ARDUINO_FS_LOG("Failed to open file for writing!\n");
        return;
    }

    fwrite(buffer, 1, size, file);  // Write binary data
    fclose(file);  // Close file
    EHS_ARDUINO_FS_LOG("Binary file saved successfully: %s\n", path);
}

ehs_bool EhsTgtFilesystem_Get_FS_Sizes(ehs_uint64* Size, ehs_uint64* Used, ehs_uint64* Free)
{
    ehs_bool success = EHS_FALSE;
    *Size = 0;
    *Used = 0;
    *Free = 0;
    if(fsMounted == EHS_TRUE && bd){
        uint64_t total_size = bd->size();
        struct statvfs stat;
        if (statvfs(EHS_BASE_DIR, &stat) == 0) {
            uint64_t free_space = stat.f_bfree * stat.f_bsize;
            uint64_t used_space = total_size - free_space;
            *Size = (ehs_uint64)total_size;
            *Used = (ehs_uint64)used_space;
            *Free = (ehs_uint64)free_space;
            success = EHS_TRUE;
        }
    }
    return success;
}

ehs_bool EhsTgtFilesystem_Init(void)
{
    // initalise and mount the fs
    if(EhsArdurinoFsInit() == EHS_FALSE){
        goto failed_fs_init;
    }
    // create a root directory
    if( EhsTF_exists(EHS_BASE_DIR) == 2 ){
        EHS_ARDUINO_FS_LOG("Base directory found (%s) !\n", EHS_BASE_DIR);
    }else{
        EHS_ARDUINO_FS_LOG("Create base directory (%s).\n", EHS_BASE_DIR);
        if( EhsTF_mkdir(EHS_BASE_DIR) == EHS_TRUE ) {
            EHS_ARDUINO_FS_LOG("Base directory OK.\n");
        } else {
            EHS_ARDUINO_FS_LOG("Base directory failed !\n");
            goto failed_fs_init;
        }
    }
    EhsHMetaSetInstPath(EHS_BASE_DIR);

    // create eRT directory structure if it doesn't exists
    if( EhsCreateDir( EHS_BASE_DIR "/appdata" ) == EHS_FALSE ) goto failed_fs_init;
    if( EhsCreateDir( EHS_BASE_DIR "/appdata/temp" ) == EHS_FALSE ) goto failed_fs_init;
    if( EhsCreateDir( EHS_BASE_DIR "/appdata/default" ) == EHS_FALSE ) goto failed_fs_init;
    if( EhsCreateDir( EHS_BASE_DIR "/sysdata" ) == EHS_FALSE ) goto failed_fs_init;
    //if( EhsCreateDir( EHS_BASE_DIR "/devman" ) == EHS_FALSE ) goto failed_fs_init; // @TODO
    
    // copy default sodl to the flash
    // @TODO - check if the flash already has an app, and compare sodl hash to prevent writing the same app
    EhsWriteFile( EHS_BASE_DIR "/appdata/default/t.sdl", default_sodl, sodl_binary_data_size );
    if( EhsTF_exists( EHS_BASE_DIR "/appdata/default/t.sdl") != 1 ) EHS_ARDUINO_FS_LOG("No default sodl found !\n");
    EhsHMetaSetAppsPath( EHS_BASE_DIR );

    EHS_ARDUINO_FS_LOG("eRT fs ready !\n");
    return EHS_TRUE;

failed_fs_init:
    EHS_ARDUINO_FS_LOG("eRT fs init failed !\n");
    return EHS_FALSE;
}

ehs_uint8 EhsTF_exists(const ehs_char* fname)
{
    if(fname == NULL || fname[0] == '\0'){
        return 0;
    }

    struct stat xFileInfo = { 0 };

    // Attempt to get file status
    int stat_ret = stat(fname, &xFileInfo);
    if (stat_ret == 0) {
        // Successfully retrieved file status, now check type
        if (S_ISREG(xFileInfo.st_mode)) {
            return 1;  // Regular file
        }
        if (S_ISDIR(xFileInfo.st_mode)) {
            return 2;  // Directory
        }
    }

    return 0;  // Default case for other errors;
}


/** Raw OS cd function */

ehs_bool EhsTF_cd(const ehs_char * dir)
{
    // @TODO - not supported create a fake root
    return EHS_FALSE;
}

ehs_bool EhsTF_mkdir(const char * szParameterFilePath)
{
    if(szParameterFilePath == NULL || szParameterFilePath[0] == '\0'){
        return EHS_FALSE;
    }
    return (mkdir(szParameterFilePath, 0777) == 0) ? EHS_TRUE : EHS_FALSE;
}

/* return true if succesful */
ehs_bool EhsTF_rmdir(ehs_char* szPath)
{
    if(szPath == NULL || szPath[0] == '\0'){
        return EHS_FALSE;
    }
    return (remove(szPath) == 0) ? EHS_TRUE : EHS_FALSE;
}

/**
 * Initialise the EhsTDFilesClass structure. Sets the path to the root of the
 * media "filesystem" (i.e. EhsTDFiles_getDir returns "empty"). Resets any
 * searches.  @todo make this work for vlc
 */
void EhsTDFiles_init(struct EhsTDFilesStruct** pFiles)
{
    *pFiles = EhsHMem_tempAlloc(sizeof(struct EhsTDFilesStruct )); //this is cleared at the end of the app run by the garbage collector
    if (*pFiles)
    {
        (*pFiles)->pDirectoryInfo = NULL;
        //(*pFiles)->eSearch = EHS_TD_FILES_SEARCH_INVALID;
    }
}

/**
 * Cleanup - close the current directory
 * @param[in] pFiles Context for the file operation
 */
void EhsTDFiles_cleanup(EhsTDFilesClass* pFiles)
{
    // todo
}

/**
 * Return the directory we are curently looking at.
 * @param[in] pFiles Context for the file operation
 * @param[out] szDir Text containing the current path. Caller must pass in an array of length EHS_TD_FILES_MAX_PATH
 * @return true if successful
 */
ehs_bool EhsTDFiles_getDir(struct EhsTDFilesStruct* pFiles, ehs_char* szDir)
{
    return EHS_FALSE; // todo
}

/**
 * Change the directory we will use for file listing operations
 * @param[in] pFiles Context for the file operation
 * @param[in] szDir absolute Directory path
 * @return true if successful.
 */
ehs_bool EhsTDFiles_setDir(EhsTDFilesClass* pFiles, const ehs_char* szDir)
{
    return EHS_FALSE; // todo
}

/**
 * Start retrieving files from the current directory
 * @param[in] pFiles Context for the file operation
 * @param[out] szName Text containing the name of the retrieved object. Caller must pass in an array
 * of length EHS_TD_FILES_MAX_FILENAME
 * @return true if a file name has been obtained
 */
ehs_uint8 EhsTDFiles_listFirst(EhsTDFilesClass* pFiles, ehs_char* szName)
{
    return EHS_FALSE; // todo
}

/**
 * Continue retrieving files from the current directory. Keep calling this function until
 * it returns false.
 * @param[in] pFiles Context for the file operation
 * @param[out] szName Text containing the name of the retrieved object. Caller must pass in an array
 * of length EHS_TD_FILES_MAX_FILENAME
 * @return 0 for nothing, 1 for a file and 2 for a directory
 */
ehs_uint8 EhsTDFiles_listNext(EhsTDFilesClass* pFiles, ehs_char* szName)
{
    return EHS_FALSE; // todo
}


ehs_bool EhsTF_stat(ehs_char* szPath,struct stat *statbuf)
{
    if(szPath == NULL || szPath[0] == '\0'){
        return EHS_FALSE;
    }
    return (stat(szPath, statbuf) == 0);
}

ehs_bool EhsTF_utime(ehs_char* szPath,struct utimbuf *new_times)
{
    //return (utime(szPath, new_times) == 0);
    return EHS_TRUE;
}

void EhsPrintPermissions(const struct stat *st) {
    // User permissions
    char perms[10] = "----------";  // Default: no permissions

    if (st->st_mode & S_IRUSR) perms[0] = 'r';
    if (st->st_mode & S_IWUSR) perms[1] = 'w';
    if (st->st_mode & S_IXUSR) perms[2] = 'x';

    // Group permissions
    if (st->st_mode & S_IRGRP) perms[3] = 'r';
    if (st->st_mode & S_IWGRP) perms[4] = 'w';
    if (st->st_mode & S_IXGRP) perms[5] = 'x';

    // Other permissions
    if (st->st_mode & S_IROTH) perms[6] = 'r';
    if (st->st_mode & S_IWOTH) perms[7] = 'w';
    if (st->st_mode & S_IXOTH) perms[8] = 'x';

    // Print in rwxr-xr-x format
    EhsStdioPrintf(" %s  |  %o\n", perms, st->st_mode & 0777); // Mask to 0777 to get user, group, and other
}
