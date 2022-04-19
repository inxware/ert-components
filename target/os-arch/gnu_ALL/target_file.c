/** @file target_file.c
 * Functions supporting file usage on the SH4 platform.
 *
 * @author: inx limited
 * @version: $Revision: 1986 $
 * @date: $Date$
 * 
 * Copyright (c) inx limited, 2007. All rights reserved.
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
#define _XOPEN_SOURCE 500

#include <sys/stat.h>

#ifndef EHS_MINGW
#ifndef EHS_ANDROID
#include <sys/dir.h>
#include <ftw.h>
#else //ANDROID
//#include <dir.h>
#endif
#else//MINGW
#include <dirent.h>
#include <dir.h>
#endif
#include "dirent.h" // bit naughty - this should be in a linux target HAL layer
#include "sys/types.h"

// or try : #include <direct.h> /* This is posix for directory listings */

#include <unistd.h>
#include <errno.h>

#define EHSL_MODULE_ID (EHSH_LOG_MODULE_HAL_FILE)

#include "messages.h"
#include "hal-api.h"
#include "ehs_types.h"

/*****************************************************************************/
/* Declare macros and local typedefs used by this file */


/**
 * Contains the properties held about a directory
 */
struct EhsTDFilesStruct
{
	ehs_char szPath[EHS_TD_FILES_MAX_PATH];	/* the full "current path". Always begins with EHS_TD_FILES_ROOT */
	DIR* pDirectoryInfo;
	//EhsTDFileFlagEnum eSearch;					/* search types currently in force */
};



/**
 * Contains the properties held about the filesystem
 */
//struct EhsTDFilesStruct {
//	int * pSearchHandle;						/**< Contains the search handle or EHS_TD_FILES_SEARCH_INVALID */
	//EhsTDFileFlagEnum eSearch;					/**< search types currently in force */
//	ehs_char szPath[EHS_TD_FILES_MAX_PATH]; /**< the full "current path". Always begins with EHS_TD_FILES_ROOT */
//	struct direct **files;
//	struct dirent *ent; /**< Contains the latest directory info */
//	ehs_uint32 iMaxFiles;
//	ehs_uint32 iFileIndex;
//	DIR *dir;
//};



/*****************************************************************************/
/* Declare prototypes of local functions */

/*****************************************************************************/
/* Variables defined with file-scope */

/*****************************************************************************/
/* Variables defined with global-scope */

/*****************************************************************************/
/* Function definitions */

/*****************************************************************************/
/* Declare macros and local typedefs used by this file */

/**
 * Specifies where the root of the playback media folder can be found
 * NOTE: no trailing /
 */

/* @todo This file has a very inconcistent API with the generic player function block TD / TV functiontypes are not consistent. */



/**
 * Check for any errors and report them if appropriate
 */
#define EHS_TD_REPORT_ERRORS(err,func) if ((err) != ST_NO_ERROR) { EhsError(EHS_MSG_TGT_STAPI(func,GetErrorText(err))); } else {printf("-\t%s ok\n",func);}

/**
 * Indicates that the search for files isn't currently taking place
 */
#define EHS_TD_FILES_SEARCH_INVALID -1
#ifndef EHS_MINGW //why do we need this? Not in the header?
extern int alphasort();
#endif

/* Declared in HAL_file.h
 *
void EhsTDFiles_init(struct EhsTDFilesStruct** pFiles);
ehs_bool EhsTDFiles_getDir(EhsTDFilesClass* pFiles, ehs_char* szDir);
ehs_bool EhsTDFiles_setDir(EhsTDFilesClass* pFiles, const ehs_char* szDir);
ehs_bool EhsTDFiles_listFirst(EhsTDFilesClass* pFiles, ehs_char* szName);
//int file_select(struct direct *entry) ;
ehs_bool EhsTDFiles_listNext(EhsTDFilesClass* pFiles, ehs_char* szName);
*/

/* Globals variables ! */
//EHS_LOCAL EhsTDFilesClass EhsTDFiles;
//EhsTDFilesClass* EhsTDFilesRef = &EhsTDFiles;

/**
 * Tests whether a file (or diectory) exists
 * @param fname Name of the file to test
 * @return 0,1,2 for not, file, dir.
 *
 */

ehs_uint8 EhsTF_exists(const ehs_char* fname) {
	struct stat xFileInfo;
	ehs_uint8 nRet = 0;
#ifdef EHS_MINGW
	ehs_char fname_temp[EHS_SYS_MAXPATHLENGTH];
	EhsStrcpy(fname_temp,fname);
	if (fname_temp[EhsStrlen(fname_temp)-1] == '\\') fname_temp[EhsStrlen(fname_temp)-1]='\0';
//#error "NOT MNGW"
#else
	const ehs_char *fname_temp = fname;
#endif
	int stat_ret=stat(fname_temp,&xFileInfo);
	if (( stat_ret == -1) && (errno == ENOENT)) {
		nRet = 0;
	}
	else {
		if ( stat_ret != -1) { // only do the following if stat() as successful (because ENOENT is not reliable on all platforms
			if (S_ISREG(xFileInfo.st_mode)) {
				nRet=1;
			}
			else if (S_ISDIR(xFileInfo.st_mode)) {
				nRet=2;
			}
		}else {
			nRet = 0;
		}
	}
	return nRet;
}


/** Raw OS cd function */

ehs_bool EhsTF_cd(const ehs_char * dir) {
	return (chdir(dir)==0);
}

/* return 0 if OK, 1 for error */
ehs_bool EhsTF_mkdir(const char * szParameterFilePath) {
#ifndef EHS_MINGW
	return (mkdir(szParameterFilePath,S_IRWXU) == 0);
#else
	return (mkdir(szParameterFilePath) == 0);
#endif
}




/* The following may need to be split into a common part */


/**
 * Initialise the EhsTDFilesClass structure. Sets the path to the root of the
 * media "filesystem" (i.e. EhsTDFiles_getDir returns "empty"). Resets any
 * searches.  @todo make this work for vlc
 */
void EhsTDFiles_init(struct EhsTDFilesStruct** pFiles) {
	*pFiles = EhsHMem_tempAlloc(sizeof(struct EhsTDFilesStruct )); //this is cleared at the end of the app run by the garbage collector
	if (*pFiles) {
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
	int err;
	if (pFiles->pDirectoryInfo)
	{
		err = closedir(pFiles->pDirectoryInfo);
		pFiles->pDirectoryInfo = NULL;
	}
}


/**
 * Return the directory we are curently looking at.
 * @param[in] pFiles Context for the file operation
 * @param[out] szDir Text containing the current path. Caller must pass in an array of length EHS_TD_FILES_MAX_PATH
 * @return true if successful
 */
ehs_bool EhsTDFiles_getDir(struct EhsTDFilesStruct* pFiles, ehs_char* szDir) {
	ehs_bool bRet = EHS_FALSE; /* assume failure */

	//	if (0 == EhsStrncmp(pFiles->szPath,EHS_TD_FILES_ROOT,EhsStrlen(EHS_TD_FILES_ROOT)))
	//	{
	/* if the path begins with EHS_TD_FILES_ROOT, it must be valid! */
	/* Return only the part of the path that doesn't include EHS_TD_FILES_ROOT */
	//		EhsStrcpy(szDir,&pFiles->szPath[EhsStrlen(EHS_TD_FILES_ROOT)]);
	//		bRet = EHS_TRUE;
	//	}
	//	else
	//	{
	/* invalid path - reset the pFiles structure, and return and empty string */
	//		*szDir = '\0';
	//		EhsTDFiles_init(pFiles);
	//	}

	return bRet;
}

/**
 * Change the directory we will use for file listing operations
 * @param[in] pFiles Context for the file operation
 * @param[in] szDir absolute Directory path
 * @return true if successful.
 */
ehs_bool EhsTDFiles_setDir(EhsTDFilesClass* pFiles, const ehs_char* szDir) {
	ehs_bool bRet = EHS_FALSE; /* assume this fails */
	if (EhsTF_exists(szDir)) {
		EhsStrcpy(pFiles->szPath, szDir);
		bRet=EHS_TRUE;
	}
	else {
		EhsStrcpy(pFiles->szPath, ""); /* set to default so we don't get more from last directory */
		bRet=EHS_FALSE;
	}
	return bRet;
}

/*
 * Count files in a directory.
 * Reuturns count of the files or
 * -1 for an error
 * -2 if more than 10,000 were found (so we don't block for long)
 *
 */

ehs_sint32 EhsTDFiles_countDirFiles ( const char *path ){
    size_t count = 0;
    struct dirent *res;
    struct stat sb;

    if (path != NULL) {
    	EHSH_LOG_ERROR("Null Directory given");
    	return -1;
    }
    if (stat(path, &sb) == 0 && S_ISDIR(sb.st_mode)){
        DIR *folder = opendir ( path );

        if (access ( path, F_OK ) != -1 ){
            if ( folder ){
                while ( ( res = readdir ( folder ) ) ){
                    if ( strcmp( res->d_name, "." ) && strcmp( res->d_name, ".." ) ){
                        count++;
                    }
                    if (count > 10000) {
                    	closedir ( folder ); // @todo not nice here...
                    	return -2; // @todo not nice here...
                    }
                }

                closedir ( folder );
            }else{
            	EHSH_LOG_ERROR ( "Could not open the directory" );
                return -1;
            }
        }
    }
    else{
    	EHSH_LOG_ERROR("%s is not a directory");
        return -1;
    }

    return count;
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
	int errval = 0;
	struct dirent* pCurrent;
	struct dirent xDirent;
	ehs_bool bRet = EHS_FALSE;
	if (pFiles->pDirectoryInfo) /* Tidy up if this handle was being used form something else already */
	{
		closedir(pFiles->pDirectoryInfo);
	}
	pFiles->pDirectoryInfo = opendir(pFiles->szPath);

	if (!pFiles->pDirectoryInfo)
	{
		errval = errno;
		switch (errval)
		{
			// print errno to screen here and report the error
			case EACCES:
				EHSH_LOG_ERROR("\nRead permission is denied for the directory");
				break;
			case EMFILE:
				EHSH_LOG_ERROR("\nThe process has too many files open");
				break;
			case ENFILE:
				EHSH_LOG_ERROR("\nSystem cannot support any additional open files at the moment");
				break;
			case ENOMEM:
				EHSH_LOG_ERROR("\nNot enough memory available");
				break;
			case ENOENT:
				EHSH_LOG_ERROR("\nFile not found");
				break;
			default:
				EHSH_LOG_ERROR("\nunknown error errval=[%d]",errval);
				break;
		}
	}
	return EhsTDFiles_listNext(pFiles, szName);
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
	struct dirent* pCurrent;
	struct dirent xDirent;
	ehs_uint8 bRet = 0;

	if (pFiles->pDirectoryInfo)
	{
#ifndef EHS_MINGW
		while ((!bRet) && (0 == readdir_r(pFiles->pDirectoryInfo, &xDirent, &pCurrent)) && pCurrent)
		{

			switch (xDirent.d_type)
			{
			case 0x04:
          /* ignore . and .. directory types */
				if ((EhsStrcmp(pCurrent->d_name, ".") != 0) && ((EhsStrcmp(
						pCurrent->d_name, "..") != 0))) {
					EhsStrcpy(szName, xDirent.d_name);
					bRet = 2;
				}
				/* else get next file */
				break;
			case 0x08:
					EhsStrcpy(szName,xDirent.d_name);
					bRet = 1;
				/* File found */
				break;
			default:
				break;
			}
		}
#else    // @todo doesn't yet differ files and dirs - perhaps we should use this on linux - it is more widely supported?
	while ((!bRet) && (0 != (pCurrent=readdir(pFiles->pDirectoryInfo)))) /* Iterate through till we find something that's a file */
	{
		//printf("Did Something = %s\n",pCurrent->d_name);
		if ((EhsStrcmp(pCurrent->d_name,".")!=0) &&  ((EhsStrcmp(pCurrent->d_name,"..")!=0))) {
			ehs_char temppath[EHS_SYS_MAXPATHLENGTH];
			EhsStrcpy(szName,pCurrent->d_name);/* We need to check the status afterwaords for windows to see if it is dir or file*/
			EhsStrcpy(temppath,pFiles->szPath);
			EhsStrcat(temppath,EHS_TD_FILES_SEPARATOR_STR);
			EhsStrcat(temppath,szName);
			bRet=EhsTF_exists(temppath);
			//bRet = 1; //always reprot file @todo fixme
			printf("Type %s is %d\n",temppath,bRet);//We are returning with pCurrent->d_name=%s\n",pCurrent->d_name);
			break;
		}
	}
	//EHSH_LOG_WARNING("Not reporting dirs/files properly in windows mingw\n");
#endif
	}
	return bRet;
}

/*
 * Continue retrieving files from the current directory. Keep calling this function until
 * it returns false.
 * @param[in] pFiles Context for the file operation
 * @param[out] szName Text containing the name of the retrieved object. Caller must pass in an array
 * of length EHS_TD_FILES_MAX_FILENAME
 * @return true if a file name has been obtained
 * /
ehs_bool EhsTDFiles_listNext(struct EhsTDFilesStruct* pFiles, ehs_char* szName) {
	ehs_bool bRet = EHS_FALSE;

		if (pFiles->pSearchHandle != EHS_TD_FILES_SEARCH_INVALID)
		{

			do
			{
				if ((pFiles->eSearch & EHS_TD_FILEFLAG_FOLDER) == EHS_TD_FILEFLAG_FOLDER)
				{
					if ((pFiles->xFileInfo.attrib & (_A_SUBDIR | _A_HIDDEN | _A_SYSTEM)) == _A_SUBDIR)
					{
						bRet = EHS_TRUE;
					}
				}
				else if ((pFiles->eSearch & EHS_TD_FILEFLAG_FILE) == EHS_TD_FILEFLAG_FILE)
				{
					if ((pFiles->xFileInfo.attrib & (_A_SUBDIR | _A_HIDDEN | _A_SYSTEM)) == 0)
					{
						bRet = EHS_TRUE;
					}
				}
			} while ((!bRet) && (EHS_TD_FILES_SEARCH_INVALID != _findnext(pFiles->pSearchHandle, &(pFiles->xFileInfo))));

			if (bRet)
			{
				EhsStrcpy(szName,pFiles->xFileInfo.name);
				if (EHS_TD_FILES_SEARCH_INVALID == _findnext(pFiles->pSearchHandle, &(pFiles->xFileInfo)))
					pFiles->pSearchHandle = EHS_TD_FILES_SEARCH_INVALID;
			}
			else
			{
				pFiles->pSearchHandle = EHS_TD_FILES_SEARCH_INVALID;
			}
		}
	if (pFiles->iFileIndex < pFiles->iMaxFiles) {
		return (EHS_FALSE);
		//@todo doesn't compile and this is not used anyway:/				EhsStrcpy(szName, pFiles->files[pFiles->iFileIndex]->d_name);
		pFiles->iFileIndex++;
		bRet = EHS_TRUE;
	}
	return bRet;
}
*/

ehs_bool EhsTF_stat(ehs_char* szPath,struct stat *statbuf) {
	return (stat(szPath, statbuf) == 0);
}

ehs_bool EhsTF_utime(ehs_char* szPath,struct utimbuf *new_times) {
	return (utime(szPath, new_times) == 0);
}


#ifndef EHS_MINGW

#ifdef EHS_ANDROID
static int remove_directory(const char *path)
{
   DIR *d = opendir(path);
   size_t path_len = strlen(path);
   int r = -1;

   if (d)
   {
      struct dirent *p;

      r = 0;

      while (!r && (p=readdir(d)))
      {
          int r2 = -1;
          char *buf;
          size_t len;

          /* Skip the names "." and ".." as we don't want to recurse on them. */
          if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, ".."))
          {
             continue;
          }

          len = path_len + strlen(p->d_name) + 2; 
          buf = malloc(len);

          if (buf)
          {
             struct stat statbuf;

             snprintf(buf, len, "%s/%s", path, p->d_name);

             if (!stat(buf, &statbuf))
             {
                if (S_ISDIR(statbuf.st_mode))
                {
                   r2 = remove_directory(buf);
                }
                else
                {
                   r2 = unlink(buf);
                }
             }

             free(buf);
          }

          r = r2;
      }

      closedir(d);
   }

   if (!r)
   {
      r = rmdir(path);
   }

   return r;
}

/* return true if succesful This is recursive */
ehs_bool EhsTF_rmdir(ehs_char* szPath) {
	ehs_sint16 ret= 0;
	if (EhsTF_exists(szPath) >= 1) { //@todo this will remove a file also - is this what we want?
		ret=remove_directory(szPath);
		if(ret < 0){
			ret=remove(szPath);
		}
		EHSH_LOG_INFO("EhsTF_rmdir ret=%d",ret);
	}
	else {
		ret=-1;
	}
	EHSH_LOG_INFO("EhsTF_rmdir ret=%d szPath=%s",ret,szPath);
	return (ret == 0);
}
#else //EHS_ANDROID
int unlink_cb(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
    int rv = remove(fpath);

    if (rv)
       EHSH_LOG_WARNING("Could not remove %s",fpath);
    return rv;
}

/* return true if succesful */
ehs_bool EhsTF_rmdir(ehs_char* szPath) {
	ehs_sint16 ret= 0;
	if (EhsTF_exists(szPath) >= 1) { //@todo this will remove a file also - is this what we want?
		ret=nftw(szPath, unlink_cb, 64, FTW_DEPTH | FTW_PHYS);
		EHSH_LOG_INFO("EhsTF_rmdir ret=%d",ret);
	}
	else {
		ret=-1;
	}
	EHSH_LOG_INFO("EhsTF_rmdir ret=%d szPath=%s",ret,szPath);
	return (ret == 0);
}
#endif //EHS_ANDROID
#else
ehs_bool EhsTF_rmdir(ehs_char* szPath) {
	ehs_sint16 ret= 0;
	//@todo we need a windows version - FTW_DEPTH doesn't work in mingw !
	ret=-1;
	return (ret == 0);
	EHSH_LOG_ERROR("Not implemented directory removal in mingw for windows")
}
#endif
