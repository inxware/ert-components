/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/*
 * These functions provide HAL services for application downloaders & launchers
 * Function Implementations for managing Application storage on disk, flash, etc.
 * */
#define EHSL_MODULE_ID (EHSH_LOG_MODULE_UNDEFINED) /* define before hal_logger.h */

#include "globals.h"
#include "hal-api.h" //include general HAL services
#include "hal_devman.h"

/* Constants */
#define EHS_SYS_APP2RUN_FILENAME "app2run.nfo" /* This is the filename of a data file containing the name of the app to run at boot*/
#define EHS_SYS_DEFAULT_APP2RUN "default" /* This is the name of the default app to run if EHS_SYS_APP2RUN_FILENAME does not exist or a reset is requested */
/* This function looks for a file in ./sys/ identifying the requested application to run (e.g. if the system was rebooted).
 * If the file exists the app name is read from it and an the working directory moved to it - if it exists.
 * If the directory does no exist the app directory is left as is and an error is returned.
 * if there is no file then the directory is chaged to default to run the default app.
 * Note - any directory when code is downloaded - if there is a duplicate then a download version directroy is created for incoming code.
 * When complete the live version directory is renamed to last and the new code is updated to actual live name.
 * Should an application fail to initialise or set a falg that is not working a function call is called to remove the new one and default to
 * the origiinal working one.
 */
/* hal facing */

/* This creates a full download path string for the application name - client trusting! */
/* Private */
ehs_bool EhsAppMakeDirString(ehs_char * dest, const ehs_char * dirname,const ehs_char * postfix)
{
#ifndef INX_SODL_IN_FLASH
    if (dest && dirname)
    {
#ifdef EHS_ESP32_SUPPORT
        // on esp32 target '/appdata' is a partition and not located within inst path
        EhsStrcpy(dest,"");
#else
        EhsStrcpy(dest,EhsHMetaGetInstPath());
#endif
        EhsStrcat(dest, EHS_TD_FILES_SEPARATOR_STR EHS_SYS_APP_BASE_DIR EHS_TD_FILES_SEPARATOR_STR );
        //EhsdirnameStrcat(dest,"\\" EHS_SYS_APP_BASE_DIR "\\");

        EhsStrcat(dest,dirname);
        if (postfix) EhsStrcat(dest,postfix);
        return EHS_TRUE;
    }
    else
    {
        return EHS_FALSE;
    }
#else
    EhsStrcpy(dest,dirname);
    return EHS_TRUE;
#endif
}

/* Public HAL */
ehs_bool EhsAppMakeDownloadString(ehs_char * dest, const  ehs_char * dirname)
{
    return EhsAppMakeDirString( dest, dirname,EHS_SYS_APP2RUN_DOWNLOAD_FILENAME_POSTFIX);
}
ehs_bool EhsAppMakeLiveDirString(ehs_char * dest, const ehs_char * dirname)
{
    return EhsAppMakeDirString(dest,  dirname,NULL);
}
ehs_bool EhsAppMakePreviousDirString(ehs_char * dest, const ehs_char * dirname)
{
    return EhsAppMakeDirString(dest, dirname,EHS_SYS_APP2RUN_PREVIOUS_FILENAME_POSTFIX);
}

/* Create the downloaded OK token in the download directory (full path must be provided)
 *
 * Run me at the end of the download
 * */
ehs_bool EhsAppSetDownloadOKToken(ehs_char * canonicalName)
{
    #ifndef INX_SODL_IN_FLASH
    ehs_char appdir[EHS_SYS_MAXPATHLENGTH];
    //EhsAppMakeDownloadString(appdir, canonicalName);
    EhsStrcpy(appdir,canonicalName);
    EhsStrcat(appdir, EHS_TD_FILES_SEPARATOR_STR);
    EhsStrcat(appdir, EHS_SYS_APP_DOWNLOAD_OK_TOKEN);
    return Ehs_Touch(appdir); /* create token to mark it runnable */
    #else 
    return EHS_TRUE;
    #endif
}

/* read in the active application and set up run environment for it
 * Run default if this doesn't exist.
 *
 * If a validated download directory exists this is switched with any current version.
 *
 * THis function will leave the requested app directory in a runnable state if atall possible.
 * kernel facing:
 *
 * returns 0 for not runnable, 1 for runnable, but no back up, 2: runnable - all OK
 *
 * */

ehs_uint8 EhsAppCheckAndSwitchDownloadDir(ehs_char * canonicalName)
{
#ifndef INX_SODL_IN_FLASH
    ehs_uint8 ret = 2;
    ehs_char appdir[EHS_SYS_MAXPATHLENGTH];
    ehs_char appdir_x[EHS_SYS_MAXPATHLENGTH];
#if defined(EHS_ANDROID) || defined(EHS_ALWAYS_START_DEFAULT_APP)
    if (EhsStrcmp(canonicalName, EHS_SYS_DEFAULT_APP2RUN) == 0)
    {
        // Android devices always use 'default' app
        EHSH_LOG_INFO("Always use 'default' app, provided it exists.");
        EhsAppMakeLiveDirString(appdir, canonicalName);
        if(EhsTF_exists(appdir))
        {
            return 0;
        }
    }
#endif
    /* this checks if download token exists in *_dl directory, then moves it to 
     * a live direcory (removes _dl postfix)
     */
    EhsAppMakeDownloadString(appdir_x, canonicalName); /* First check if the download is valid */
    EhsStrcat(appdir_x, EHS_TD_FILES_SEPARATOR_STR );
    EhsStrcat(appdir_x, EHS_SYS_APP_DOWNLOAD_OK_TOKEN);
    if (EhsTF_exists(appdir_x) > 0)   /* We only do this if there is valid version or the debugger's temp apps*/
    {
        EHSH_LOG_INFO("Found valid download for %s",canonicalName);
        EhsAppMakePreviousDirString(appdir_x, canonicalName); /* Shift Current to previous */
        //EHSH_LOG_INFO("PBB Removing %s",appdir_x);
        if (EhsTF_rmdir(appdir_x) == EHS_FALSE)
        {
            EHSH_LOG_INFO("Can't remove old app version for %s",canonicalName);
            ret = 1; /* can't remove old version */
        }
        /* Demote current to previous */
        //EhsAppMakeLiveDirString(appdir, canonicalName);
        //if (EhsHRename(appdir, appdir_x) == EHS_FALSE) {
        //	EHSH_LOG_WARNING("Can'tbackup current app %s", canonicalName);
        //	ret = 1;/* Can't backup up current app */
        //}
        /*Switch download app to live */
        EhsAppMakeLiveDirString(appdir, canonicalName);

        if (EhsHRename(appdir, appdir_x) == EHS_FALSE)
        {
            EHSH_LOG_WARNING("Can't backup current app %s\n to \n%s",appdir,appdir_x);
            ret = 1;/* Can't backup up current app */
        }

        //} else { /* We can't make a back up so we delete the current live version*/
        //	if (EhsTF_rmdir(appdir) == EHS_FALSE) {
        //		EHSH_LOG_WARNING("Can't remove current application %s",canonicalName);
        //		ret = 1;
        //	}
        //}
        EhsAppMakeDownloadString(appdir_x, canonicalName);

        if (EhsHRename(appdir_x, appdir) == EHS_FALSE)
        {
            //EHSH_LOG_INFO
            ret = 0; /* default to default make download live */
        }
        else
        {
            ret = 2; /* made directory OK - don't need this... */
            EHSH_LOG_INFO("Set app %s to live\n%s\n->%s",canonicalName,appdir_x,appdir);
        }
    }
    // make sure that we have a valid app, and if not request download of default app
    if(ret > 0){
        EhsAppMakeLiveDirString(appdir, canonicalName);
        EhsStrcat(appdir, EHS_TD_FILES_SEPARATOR_STR );
        EhsStrcat(appdir, EHS_DEFAULT_SODL_NAME );
        if (EhsTF_exists(appdir) == EHS_FALSE) { // check if the app exists
            ret = 0; // try default
        }
    }
    if(ret == 0){
        EhsAppMakeLiveDirString(appdir, EHS_SYS_DEFAULT_APP2RUN);
        EhsStrcat(appdir, EHS_TD_FILES_SEPARATOR_STR );
        EhsStrcat(appdir, EHS_DEFAULT_SODL_NAME );
        if (EhsTF_exists(appdir) == EHS_FALSE) { // check if the app exists
            ret = 10; // no app even in the default directy, return download default request if not installed
        }
    }
    
    return ret;
#else
    return 2;
#endif
}

ehs_bool EhsAppSetDefaultApp(ehs_char * app)
{
#ifndef INX_SODL_IN_FLASH
    ehs_bool ret=EHS_FALSE;
    ehs_FILE* app2run_FILE;
    if (EhsStrlen(app)==0)
    {
        EHSH_LOG_ERROR("Tried so set the default app to something that doesn't exist");
        ret=EHS_FALSE;
    }
    else  /* @todo we could test for existence here */
    {
        if ((app2run_FILE = Ehs_SysFopen(EHS_SYS_APP2RUN_FILENAME, "w")))
        {
            EhsFprintf(app2run_FILE, "%s", app);
            EhsFclose(app2run_FILE);
            ret=EHS_TRUE;
        }
    }
    return ret;
#else
    return EHS_TRUE;
#endif
}

/**
 * Retrieve the default app specified in app2run file
 * @param cDefaultApp return param, name of default app
 *
 * If no file found or file is empty will return 'Default' as the default app
 * Will always return something ensible or exit;
 */
ehs_bool EhsAppGetDefaultApp(ehs_char * cDefaultApp)
{
#ifndef INX_SODL_IN_FLASH
    ehs_FILE* app2run_FILE;
    ehs_bool ret = EHS_TRUE; // for when we remove the exit
    if (cDefaultApp  == NULL )
    {
        EHSH_LOG_ERROR("NULL pointer provided to get Default Application - exiting!");
        EhsExit(0);
    }
    if ((app2run_FILE = Ehs_SysFopen(EHS_SYS_APP2RUN_FILENAME, "r")))
    {
        EhsFgets(cDefaultApp,EHS_SYS_MAXPATHLENGTH,app2run_FILE); // read one line from file
        EhsFclose(app2run_FILE);
        //trim - remove new line chr or spaces
        ehs_char * s;
        s = cDefaultApp;
        int end = strlen(s) - 1;
        while (isspace(s[end]) && (end >= 0))
        {
            end--;
        }
        s[end+1] = '\0';

        if (EhsStrlen(cDefaultApp) == 0)   /* If we haven't got something sensible then revert to default @todo - could check for path here */
        {
            EhsStrcpy(cDefaultApp,EHS_SYS_DEFAULT_APP2RUN);
        }
        ret = EHS_TRUE;

    }
    else
    {
        EhsStrcpy(cDefaultApp,EHS_SYS_DEFAULT_APP2RUN);
        EHSH_LOG_INFO("File App2Run not found, reverting to default");
    }
    return ret;
#else
    EhsStrcpy(cDefaultApp,"FLASH");
    return EHS_TRUE;
#endif

}


/* Finds out what app should be run &
 * Makes sure the app that is run is the latest downloaded version
 *
 * Amended to check a global var first 'app to run next', if no app set, then checks app2run file
 * global var 'app to run next' allows an app to be set to run when EHS restarts
 * app2run file allows an app to be set as the default app when EHS is first started up
 */

ehs_bool EhsAppInitLiveAppDir()
{
#ifndef INX_SODL_IN_FLASH
    ehs_char appdir[EHS_SYS_MAXPATHLENGTH];
    ehs_char AppCanonical[EHS_SYS_MAXPATHLENGTH];
    ehs_FILE* app2run_FILE;
    ehs_uint8 dirstat;
    //ehs_uint32 len;
    ehs_bool app_installed;
    app_installed=EHS_FALSE;

    // if we have EhsHMetaGetNextAppToRun, then run this app, else check the app2run file for a default app
    EhsStrcpy(AppCanonical,EhsHMetaGetNextAppToRun());
    EhsAppMakeDownloadString(appdir, EHS_SYS_DEFAULT_APP2RUN); // Default to this in case here is no app2run set
    EHSH_LOG_ERROR("app-to-run-from memory is [%s]",EhsHMetaGetNextAppToRun());
    if (EhsStrlen(AppCanonical) == 0)   // if we haven't got one set then lets handle this by checking the file system or setting the default
    {
        ehs_bool bAlwaysStartDefault = EHS_FALSE;
#ifdef EHS_ALWAYS_START_DEFAULT_APP
        bAlwaysStartDefault = EHS_TRUE;
#endif
        EHSH_LOG_ERROR(" Looking for app2runfile...");
        if ((app2run_FILE = Ehs_SysFopen(EHS_SYS_APP2RUN_FILENAME, "r")) && !bAlwaysStartDefault)
        {
            EhsFgets(AppCanonical,EHS_SYS_MAXPATHLENGTH,app2run_FILE); // read one line from file
            EhsFclose(app2run_FILE);
            if (EhsStrlen(AppCanonical)>0 )
            {
                //trim - remove new line chr or spaces
                ehs_char * s;
                s = AppCanonical;
                int end = strlen(s) - 1;
                while (isspace(s[end]) && (end >= 0))
                {
                    end--;
                }
                s[end+1] = '\0';
                EHSH_LOG_ERROR(" app2runfile good ...");
                EhsHMetaSetNextAppToRun(AppCanonical);
                EhsHMetaAppSetCurrent(AppCanonical);
            }
            else
            {
                EHSH_LOG_ERROR("No app2runfile. setting to default");
                EhsHMetaSetNextAppToRun(EHS_SYS_DEFAULT_APP2RUN);
                EhsHMetaAppSetCurrent(EHS_SYS_DEFAULT_APP2RUN);
            }
        }
        else
        {

            EHSH_LOG_ERROR("No file specified Opening default");
            EhsHMetaSetNextAppToRun(EHS_SYS_DEFAULT_APP2RUN);
            goto do_default;
        }

        EhsAppGetDefaultApp(AppCanonical);
    }


    if (EhsStrlen(AppCanonical) == 0)   /* If we haven't got something sensible then revert to default */
    {
        /* first we'll check the default app directory exists - in case something strange happened */
        EHSH_LOG_INFO("Requested App %s not found - trying default",AppCanonical);
        goto do_default;
    }
    else     /* OK we have a valid name - but does it exist as a runnable or download? */
    {
        EHSH_LOG_INFO("Found app %s trying...", AppCanonical);
        EhsAppMakeLiveDirString(appdir, AppCanonical);
        if (EhsTF_exists(appdir) > 0)
        {
            app_installed = EHS_TRUE;
            goto do_rest;
        }
        else
        {
            EhsAppMakeDownloadString(appdir, AppCanonical); // There is a DL directory so carry on!
            if ((dirstat = EhsTF_exists(appdir)) == 2)   /* If the live directory does not exist then create an empty one. This is to simply for the launcher testing for a valid directory before doing a swtch */
            {
                app_installed = EHS_TRUE;
                goto do_rest;
            }
            else
            {
                //EhsAppMakeDownloadString(appdir,AppCanonical);
                EHSH_LOG_WARNING("Could not find DL either at %s", appdir);
                goto do_default;
            }
        }
    }

do_default: /* Set up the default directory stuff */

    EhsAppMakeLiveDirString(appdir,EHS_SYS_DEFAULT_APP2RUN);
    EhsStrcpy(AppCanonical,EHS_SYS_DEFAULT_APP2RUN); // Set just the canonical string for Meta data set at the end!
    //EHSH_LOG_INFO("default path for %s = %s",AppCanonical,appdir);
    if (EhsTF_exists(appdir) == 0)   //if we have nothing make a default with an empty application
    {
        EhsTF_mkdir(appdir);
    }
do_rest: /* Finally check of there is a downloaded version  and do switch if so */
    switch(EhsAppCheckAndSwitchDownloadDir(AppCanonical))
    {
    case 0:
        EhsHMetaAppSetCurrent(EHS_SYS_APP_DEFAULT_NAME);/* Revert to default App - this is enough! */
        EHSH_LOG_INFO("No download available Loading default app instead (%s)",appdir);
        break;
    case 1:
    case 2:
        if (EhsStrlen(AppCanonical) > 0 )
        {
            EhsHMetaAppSetCurrent(AppCanonical);/* Update internal structure */
        }
        else
        {
            EHSH_LOG_WARNING("Didn't get a app path - not changed the current live app");
        }
        break;
    case 10: // Apparantly there is no download app or defalt in this case so download th default.
        if (app_installed != EHS_TRUE)
        {
            EHSH_LOG_ERROR("No default app found!");
#if (EHS_DEVMAN_SUPPORT == EHS_DEVMAN_HTTP)
            ehs_char szUrl[EHS_MAXDEVMANNAMELEN];// App URL/
            GetDevmanBASEURL(szUrl);
            EHSH_LOG_INFO("Downloading App!");
            EhsStrcat(szUrl, EHS_DEVMAN_APPGETDEFAULTURLPATHONLY);
            if (start_appget_getapp("default_devman","default_devman", szUrl, EHS_TRUE, EHS_FALSE, EHS_FALSE))   // don#tuse the run fags we just copy this into default.
            {
                EHSH_LOG_INFO("Downloaded - Making the Default app!");
                Ehs_AppRm(EHS_SYS_DEFAULT_APP2RUN);
                Ehs_AppRename("default_devman",EHS_SYS_DEFAULT_APP2RUN);
                EhsHMetaSetNextAppToRun(EHS_SYS_DEFAULT_APP2RUN); // not sure thi is needed?
                EhsHMetaAppSetCurrent(EHS_SYS_DEFAULT_APP2RUN); // this sets the app to default if it wasn't the default already for some reason

            }
#endif //EHS_DEVMAN_SUPPORT=CURL
        }
        break;
    default:
        break;

    }
    return EHS_TRUE; // don't need this - EhsTF_cd(appdir); /* we don't return an error if we ran the default rather than requested app */
#else // else SODL in flash.
    return EHS_TRUE;
#endif
}

/* remove any temp data ! */
void EhsAppConfirmCurrentApp()
{
#ifndef INX_SODL_IN_FLASH
    ehs_char appdir[EHS_SYS_MAXPATHLENGTH];
    // ehs_FILE* app2run_FILE;
    EhsStrcpy(appdir,EhsHMetaAppGetCurrent());
    EhsStrcat(appdir,EHS_SYS_APP2RUN_PREVIOUS_FILENAME_POSTFIX);
    Ehs_SysRmdir(appdir); /* todo we may not want to do this so that revert can be used for each app*/
    EhsTF_cd(appdir); /* we don't return an error if we ran the default rather than requested app */
#endif
}

/* This will try and revive any old versions and remove the failed one.
 * If there is no old one then the default app is configured to run next
 * Returns true if an old version was found, false if it has switched to the default app.
 * It will also repair the default directory path if it is not there.
 * */
ehs_bool EhsAppDenyCurrentApp()
{
#ifdef INX_SODL_IN_FLASH
    return EHS_TRUE;
#else
    if (EhsHMetaAppGetCurrent()>0)   // rally souldn't happn unless w have trampled over a structure
    {
        if (EhsStrlen(EhsHMetaAppGetCurrent()) > 0 )
        {
            ehs_char appdir[EHS_SYS_MAXPATHLENGTH];
            ehs_char old_appdir[EHS_SYS_MAXPATHLENGTH];
            EhsAppMakePreviousDirString( old_appdir,EhsHMetaAppGetCurrent());
            EhsAppMakeLiveDirString( appdir,EhsHMetaAppGetCurrent());
            EhsTF_rmdir(appdir); /* Dump this app unconditionally */
            if (EhsHRename(old_appdir,appdir) )   /* reinstate previous version (if it existed).*/
            {
                // todo we may want to remove the default application file here?
                EHSH_LOG_INFO("Reverted Denied App");
                return EHS_TRUE;
            }
            else return EHS_FALSE;
        }
        else
        {
            return EHS_FALSE;

        }
    }
    //else EHSH_LOG_ERROR("EHS APP DENY TRAMPLED MTATA DATA");
    /* Nothing to revert to - so set to default app*/
    //EHSH_LOG_INFO("Reverted to Default app. No version of %s is runnable",EhsHMetaAppGetCurrent());
    //Ehs_SysRm(EHS_SYS_APP2RUN_FILENAME); /* remove the app2run file to avoid at next start-up*/
    //EhsTF_rmdir(old_appdir); /* just in case to be tidy */
    //EhsAppMakeLiveDirString( ,EHS_SYS_APP_DEFAULT_NAME); /* Check we do have a default app dir and create empty if not*/
    //if (EhsTF_exists(appdir) != 2) EhsTF_mkdir(appdir);
    EhsHMetaAppSetCurrent(EHS_SYS_APP_DEFAULT_NAME);/* Update internal structure */
    return EHS_FALSE;
#endif
}
