/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/* appget.c
 *
 * functions for use in the EHS system providing POSIX compliant file access.
 *
 *
*/

//#define EHSL_MODULE_ID EHSH_LOG_MODULE_HAL_NETWORK

//#include <math.h>
//EHS- API
#include "globals.h"

#include "ehs_fb_types.h"
#include "setCompletes.h"
#include "callback_queue.h"
#include "fid.h"
#include "hal_devapps.h" // needed for devapp hal code

#include "appget.h"
//EHS Common services API
#include "hal_string.h"
//#include "hal_network.h" //for libcurl
#include "hal_file.h" // for achiving
#include "hal-api.h" //needed for  version info
//#include "hal_url.h"
//#include "hal_devapps.h" // Use the app downloader code from here
#include "app_data.h" //needed for data table - all wrong!

#include <errno.h>

#define CR '\r'
#define LF '\n'

/* Operational Description
 *Implements:
 *Get Application & Run transient
 *Get Application & install resident
 *Get Applications on the server with credentials
 *
 *
 *Listing are done with a post, and the data is returned immediately in XML format
 *
 *Application downloads are done by sending a post to get a download URL & cookie
 *The actual download is done as a get.
 */

/******************************************************************************/
/* Define app info get function block */
/* Retrieves the app and runs it - but does not permanently install*/
/* Retrieves the app and runs it - but does not permanently install*/
/* Retries app and installs it. Optionally runs it.*/
/* Gets a list of remote apps with version, dependency and user meta data filter.*/
/* Lists the installed apps with user meta data filter.*/

/* define some DEVMAN fail-over constants */

#ifndef EHS_DEVMAN_DEFAULTBASEURL
#define EHS_DEVMAN_DEFAULTBASEURL	"https://devman.inx-systems.com"
#endif


#ifndef EHS_DEVMAN_APPGETINFOURLPATH
#define EHS_DEVMAN_APPGETINFOURLPATH "/getAppInfo.php"
#endif

#ifndef EHS_DEVMAN_APPGETAPPURLPATH
#define EHS_DEVMAN_APPGETAPPURLPATH "/downloadApp.php"
#endif

#ifndef EHS_DEVMAN_APPGETLISTURLPATH
#define EHS_DEVMAN_APPGETLISTURLPATH "/getAppList.php"
#endif


EHS_FB_FUNCTIONS_START(appget)
EHS_FB_FUNCTION_ENTRY("listremote", 0x01, appget_list_remote)
EHS_FB_FUNCTION_ENTRY("getinfo", 0x02, appget_getinfo)
EHS_FB_FUNCTION_ENTRY("getinstall", 0x03, appget_getapp)
EHS_FB_FUNCTIONS_END

//define the port mappings
#define EHS_GETAPP_GETSTAT_PORT_FINISH 1

//app info ports
#define	EHS_GETAPP_GETINFO_OK_EO 1
#define	EHS_GETAPP_GETINFO_ERROR_EO 2
#define EHS_GETAPP_GETINFO_APP_DI 0
#define EHS_GETAPP_GETINFO_SERVER_DI 1
#define EHS_GETAPP_GETINFO_INFO_DO 0
#define EHS_GETAPP_GETINFO_NAME_DO 1
#define EHS_GETAPP_GETINFO_COMM_NAME_DO 2
#define EHS_GETAPP_GETINFO_VERSION_DO 3
#define EHS_GETAPP_GETINFO_DESCRIPTION_DO 4
#define EHS_GETAPP_GETINFO_ERROR_DO 5


//@todo need a busy flag

//for get app
#define EHS_GETAPP_GETAPP_FINISH_EO 1
#define EHS_GETAPP_GETAPP_ERROR_EO 2
#define	EHS_GETAPP_GETAPP_NAME_DI 0
#define EHS_GETAPP_GETAPP_SERVER_DI  1
#define EHS_GETAPP_GETAPP_RUN_DI  2
#define EHS_GETAPP_GETAPP_DEFAULT_DI  3
#define EHS_GETAPP_GETAPP_MODE_DI  4
#define EHS_GETAPP_GETAPP_ERROR_DO  0

/* Get list port mappings */
#define EHS_GETAPP_GETLIST_FINISH_EO 1
#define EHS_GETAPP_GETLIST_ERROR_EO  2
#define EHS_GETAPP_GETLIST_KEYWORDS_DI  0
#define EHS_GETAPP_GETLIST_SERVER_DI  1
#define EHS_GETAPP_GETLIST_LIST_DO  0
#define EHS_GETAPP_GETLIST_ERROR_DO  1

void nout()
{
//just to stop the syntax checker carrying forward grunge.
}

//@todo this should be dynamic





/* This thread reads data from the URL using libcurl */
EHS_FB_THREAD_FUNCTION(appget_read_data)
{

    appgetObj *ObjData = (appgetObj*) EHS_FB_RUN_CONTEXT;
    Ehs_FB_ThreadStarted();
    if (HAL_AppGetRead_data(ObjData))
    {
        EHS_FB_FINISH(EHS_GETAPP_GETAPP_FINISH_EO);
    }
    else
    {
        EHS_FB_FINISH(EHS_GETAPP_GETAPP_ERROR_EO);
    }
    Ehs_FB_ThreadComplete();
    EhsHThread_exit();
}

/* Kick off our thread to retrieve an app, setting the busy flag and tidying up */
EHS_FB_THREAD_FUNCTION(appget_getapp_thread)
{
    appgetObj *ObjData = (appgetObj*)EHS_FB_RUN_CONTEXT;
    ObjData->bBusy = EHS_FALSE;
    Ehs_FB_ThreadStarted(); //@todo all these should be moved to the caller
    HAL_appGetWaitForURLDataAndWrite(ObjData);
    /* and tidy up - we are function block thread */
    Ehs_FB_ThreadComplete();
    EhsHThread_exit();
}







/* get app - install using api */
EHS_FB_RUN_FUNCTION(appget_getapp)
{
    appgetObj *ObjData = (appgetObj*)EHS_FB_RUN_CONTEXT;
    if (ObjData->bBusy==EHS_FALSE)
    {
        ObjData->bBusy=EHS_TRUE;
        if (EHS_FB_IN_CONNECTED(EHS_GETAPP_GETAPP_NAME_DI))
        {
            EhsStrcpy(ObjData->szAppCanonicalName, EHS_FB_IN_S(EHS_GETAPP_GETAPP_NAME_DI));
        }
        if (EHS_FB_IN_CONNECTED(EHS_GETAPP_GETAPP_RUN_DI))
        {
            ObjData->bRunWhenDone = EHS_FB_IN_B(EHS_GETAPP_GETAPP_RUN_DI);
        }
        if (EHS_FB_IN_CONNECTED(EHS_GETAPP_GETAPP_DEFAULT_DI))
        {
            ObjData->bSetAsDefaultApp = EHS_FB_IN_B(EHS_GETAPP_GETAPP_DEFAULT_DI);
        }
        if (EHS_FB_IN_CONNECTED(EHS_GETAPP_GETAPP_MODE_DI))
        {
            ObjData->nInstallMode = EHS_FB_IN_I(EHS_GETAPP_GETAPP_MODE_DI);
        }
        /*
         * Install Modes:
         * 0. Install as standard app - install in canonical dir
         * 1. Install as home app - install in default dir
         * 2. Install as temp app - install in temp dir
         */
        if (ObjData->nInstallMode == 1)
        {
            //@todo Install as Home App - This should could some credentials service - or be removed from 3rd -party component profiles
            EhsAppMakeDownloadString(ObjData->szpDownloadDir, EHS_SYS_APP_DEFAULT_NAME);
            EhsStrcpy(ObjData->szAppLocalName,EHS_SYS_APP_DEFAULT_NAME);
        }
        else if (ObjData->nInstallMode == 2)
        {
            // install as temp app
            EhsAppMakeDownloadString(ObjData->szpDownloadDir,EHS_SYS_APP_TEMP_NAME);
            EhsStrcpy(ObjData->szAppLocalName,EHS_SYS_APP_TEMP_NAME);
        }
        else
        {
            // install as standard app
            EhsAppMakeDownloadString(ObjData->szpDownloadDir,ObjData->szAppCanonicalName);
            EhsStrcpy(ObjData->szAppLocalName,ObjData->szAppCanonicalName);
        }

        /* Start URL get thread to pump data into a buffer */
        EHS_FB_START_THREAD(appget_read_data, -90);
        /* Start Incremental archive decoder/writer */
        EHS_FB_START_THREAD(appget_getapp_thread, -90);
    }
    else
    {
        EHS_FB_FINISH(EHS_GETAPP_GETAPP_ERROR_EO);
        EhsStrcpy(EHS_FB_OUT_S(EHS_GETAPP_GETAPP_ERROR_DO),"busy");
    }
}




/**
 * Identify the function block. Determine the memory required.
 *
 * This function provides access to:
 *  EHS_FB_IDENTIFY_PARAMETERS - string containing parameter text
 *  EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this
 *   function block's context
 *
 */

EHS_FB_IDENTIFY_FUNCTION(appget)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(appgetObj);
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */


EHS_FB_INIT_FUNCTION(appget)
{
    appgetObj *ObjData = (appgetObj*)EHS_FB_INIT_CONTEXT;
    EhsSscanf(EHS_FB_INIT_PARAMETERS,"%s%hhd%hhd%d",ObjData->szUrl,&ObjData->bRunWhenDone,&ObjData->bSetAsDefaultApp,&ObjData->nInstallMode);
    if (EhsStrlen(ObjData->szUrl) == 0) EhsStrcpy(ObjData->szUrl,EHS_DEVMAN_DEFAULTBASEURL); /* set a default base devman URL if one isn't supplied */

    ObjData->bBusy=EHS_FALSE;
    ObjData->curl = curl_easy_init(); /* We can call this multiple times apparently, but not threaded *///@todo check that curl_global_init() is called before threads start
#warning memory leek - need to have curl close functions in tear down.
    if (!ObjData->curl)
    {
        EHSH_LOG_ERROR("Could not initialise CURL");
    }
    ObjData->server_info.http_username[0]='\0';
    ObjData->server_info.http_password[0]='\0';
    ObjData->server_info.authentication=0;
    ObjData->URL_write_data_buffer_struct=NULL;
    ObjData->bFreeWhenDone=EHS_FALSE; /* we keep this lying about for function blocks! */
    EhsStrcpy(ObjData->szID,EhsHMetaGetHWID());
    EhsStrcpy(ObjData->szpDownloadDir,""); // this is always set properly when needed.
    EhsStrcpy(ObjData->szAppLocalName,"");
    EhsStrcpy(ObjData->szUrlAppGetExtPath,EHS_DEVMAN_APPGETAPPURLPATH); /* Default appget URL appendic this is the normal path for the app downloader */
    return EHS_TRUE; /* initialisation always succeeds */
}


/* This is called for file or network gets
 * This needs a write buffer
 *
 * */
EHS_FB_THREAD_FUNCTION(appget_getinfo_thread)
{
    appgetObj *ObjData = (appgetObj*)EHS_FB_RUN_CONTEXT;
    ehs_char * szXml;
    ehs_uint32 http_no;
    ehs_char szFullURL[EHS_MAXDEVMANNAMELEN];
    ehs_char* PostString;
    ehs_char * element_start;
    ehs_char element_cropped[EHS_STRING_LENGTH_MAX];
    ehs_char cName[EHS_STRING_LENGTH_MAX];
    ehs_char cCommercialName[EHS_STRING_LENGTH_MAX];
    ehs_char cVersion[EHS_STRING_LENGTH_MAX];
    ehs_char cDescription[EHS_STRING_LENGTH_MAX];
    //EhsHwrite_data_bufferType * write_data_buffer_struct;
    Ehs_FB_ThreadStarted();
    PostString = EhsHMem_tempAlloc(EHS_POST_STRING_LENGTH_MAX); //more than enough?ehs_char * PostString; // TODO2024 why we do this here but not for the above
    if (!PostString)
    {
        EhsStrcpy(EHS_FB_OUT_S(EHS_GETAPP_GETINFO_ERROR_DO), "Insufficient memory");
        EHS_FB_FINISH(EHS_GETAPP_GETINFO_ERROR_EO);
        goto error;
    }
    PostString[0]='\0';
    /* Allocate a buffer */
    ObjData->URL_write_data_buffer_struct=EhsHDoAllGenericConfig(ObjData->curl,&ObjData->server_info, 64*1024, 50000,120);/* 2 minute timeouts and 50ms chunk gap */
    
    /* Add any ssl certificates */
    /* TODO2024 - add alterantive appp download */
    EhsHSetUpClientTlsCertificate(ObjData->curl, EHS_RUNTIME_DEVMAN_DIR, EHS_DEVMAN_CLIENT_CERTIFICATE_KEY, NULL /* combined in PEM */, NULL);
    EhsHSetUpCaTlsCertificate(ObjData->curl, EHS_RUNTIME_DEVMAN_DIR, EHS_DEVMAN_CA_CERTIFICATE);

    EhsHSetUpLocalProxy(ObjData->curl);
    if (!ObjData->URL_write_data_buffer_struct)
    {
        EhsStrcpy(EHS_FB_OUT_S(EHS_GETAPP_GETINFO_ERROR_DO),"Could not configure URL get");
        EHS_FB_FINISH(EHS_GETAPP_GETINFO_ERROR_EO);
        goto error;
    }
    if (EHS_FB_IN_CONNECTED(EHS_GETAPP_GETINFO_SERVER_DI))
    {
        EhsStrcpy(ObjData->szUrl, EHS_FB_IN_S(EHS_GETAPP_GETINFO_SERVER_DI));
    }
    EhsStrcpy(szFullURL, ObjData->szUrl);
    EhsStrcat(szFullURL, EHS_DEVMAN_APPGETINFOURLPATH);
    if (EHS_FB_IN_CONNECTED(EHS_GETAPP_GETINFO_APP_DI))
    {
        EhsStrcpy(ObjData->szAppCanonicalName, EHS_FB_IN_S(EHS_GETAPP_GETINFO_APP_DI));
    }
    EhsHCreateQueryString(ObjData->curl,PostString, "AppCanonicalName", ObjData->szAppCanonicalName,EHS_POST_STRING_LENGTH_MAX);
    EhsHCreateQueryString(ObjData->curl,PostString, "ID", ObjData->szID,EHS_POST_STRING_LENGTH_MAX);
    if (EhsHURLConfigPostGet(ObjData->curl,ObjData->URL_write_data_buffer_struct,szFullURL, PostString,EHS_TRUE))
    {
        http_no=EhsHURLdoRequest(ObjData->curl);
    }
    else http_no=0;

    if (http_no == 200)
    {
        szXml=EhsHURLget_write_data_buffer(ObjData->URL_write_data_buffer_struct);
        if (szXml != NULL)
        {
            EhsStrncpy(EHS_FB_OUT_S(EHS_GETAPP_GETINFO_INFO_DO), szXml,EHS_STRING_LENGTH_MAX);

            // very dirty bit of xml parsing - assuming tags always arrive in this order
            EhsStrcpy(cName,"No Name Found");
            EhsStrcpy(cCommercialName,"No Commercial Name Found");
            EhsStrcpy(cVersion,"No Version Found");
            EhsStrcpy(cDescription,"No Description Found");

            if (element_start = Ehs_ReadXMLTag(szXml, EHS_SYS_APP_INFO_XMLTAG_NAME))
            {
                Ehs_CopyXMLTagElement(element_cropped, element_start, EHS_STRING_LENGTH_MAX, EHS_TRUE);
                EhsStrcpy(cName,element_cropped);
            }
            if (element_start = Ehs_ReadXMLTag(szXml, EHS_SYS_APP_INFO_XMLTAG_COMM_NAME))
            {
                Ehs_CopyXMLTagElement(element_cropped, element_start, EHS_STRING_LENGTH_MAX, EHS_TRUE);
                EhsStrcpy(cCommercialName,element_cropped);
            }
            if (element_start = Ehs_ReadXMLTag(szXml, EHS_SYS_APP_INFO_XMLTAG_VERSION))
            {
                Ehs_CopyXMLTagElement(element_cropped, element_start, EHS_STRING_LENGTH_MAX, EHS_TRUE);
                EhsStrcpy(cVersion,element_cropped);
            }
            if (element_start = Ehs_ReadXMLTag(szXml, EHS_SYS_APP_INFO_XMLTAG_DESCRIPTION))
            {
                Ehs_CopyXMLTagElement(element_cropped, element_start, EHS_STRING_LENGTH_MAX, EHS_TRUE);
                EhsStrcpy(cDescription,element_cropped);
            }
            EhsStrcpy(EHS_FB_OUT_S(EHS_GETAPP_GETINFO_NAME_DO), cName);
            EhsStrcpy(EHS_FB_OUT_S(EHS_GETAPP_GETINFO_COMM_NAME_DO), cCommercialName);
            EhsStrcpy(EHS_FB_OUT_S(EHS_GETAPP_GETINFO_VERSION_DO), cVersion);
            EhsStrcpy(EHS_FB_OUT_S(EHS_GETAPP_GETINFO_DESCRIPTION_DO), cDescription);

            EhsStrcpy(EHS_FB_OUT_S(EHS_GETAPP_GETINFO_ERROR_DO), "OK");
            EHS_FB_FINISH(EHS_GETAPP_GETINFO_OK_EO);
        }
        else
        {
            EhsStrcpy(EHS_FB_OUT_S(EHS_GETAPP_GETINFO_ERROR_DO), "Can not read internal data buffer");
            EHS_FB_FINISH(EHS_GETAPP_GETINFO_ERROR_EO);
        }
    }
    else
    {
        EhsStrcpy(EHS_FB_OUT_S(EHS_GETAPP_GETINFO_ERROR_DO), "Can not access info URL");
        EHS_FB_FINISH(EHS_GETAPP_GETINFO_ERROR_EO);
    }
    EhsHURLfree_write_data_buffer(ObjData->URL_write_data_buffer_struct);
    ObjData->URL_write_data_buffer_struct = NULL;

    EhsHMem_tempFree(PostString);
    PostString = NULL;

error:
    Ehs_FB_ThreadComplete();
    EhsHThread_exit();
}

#warning curl_easy_cleanup(curl) needs to be called somwhere - need a tear down function;

/**
 * Run the function <more detail required>.
 *
 * Retrieve application meta data from a server
 */
EHS_FB_RUN_FUNCTION(appget_getinfo)
{
    appgetObj *ObjData = (appgetObj*)EHS_FB_RUN_CONTEXT;
    if (!ObjData->curl)
    {
        EhsStrcpy(EHS_FB_OUT_S(EHS_GETAPP_GETINFO_INFO_DO),"");
        EhsStrcpy(EHS_FB_OUT_S(EHS_GETAPP_GETINFO_ERROR_DO),"initialisation failed");
        EHS_FB_FINISH(EHS_GETAPP_GETINFO_ERROR_EO);
        return;
    }

    //@todo should have mutexing here for the busy flag
    if (ObjData->bBusy==EHS_FALSE)
    {
        EHS_FB_START_THREAD(appget_getinfo_thread,-90);

    }
    else
    {
        ObjData->bBusy=EHS_TRUE;
        EHS_FB_FINISH(EHS_GETAPP_GETINFO_ERROR_EO);
        EhsStrcpy(EHS_FB_OUT_S(EHS_GETAPP_GETINFO_INFO_DO),"");
        EhsStrcpy(EHS_FB_OUT_S(EHS_GETAPP_GETINFO_ERROR_DO),"busy");
    }
}


/* Retrieves a list of applications (Meta data and titles and URLs, given a set of criteria
 * This calls a thread function to do this
 * */
/*
#define 	EHS_GETAPP_GETLIST_FINISH_EO 1
#define 	EHS_GETAPP_GETLIST_ERROR_EO  2
#define 	EHS_GETAPP_GETLIST_KEYWORD_DI  0
#define 	EHS_GETAPP_GETLIST_LIST_DO  0
*/
EHS_FB_THREAD_FUNCTION(appget_list_remote)
{
    appgetObj *ObjData = (appgetObj*)EHS_FB_RUN_CONTEXT;
    /* do post here */
    ObjData->bBusy = EHS_FALSE;
    ehs_char * szXml;
    ehs_uint32 http_no;
    ehs_char szFullURL[EHS_MAXDEVMANNAMELEN];
    ehs_char* PostString;
    //EhsHwrite_data_bufferType * write_data_buffer_struct;
    Ehs_FB_ThreadStarted();
    PostString = EhsHMem_tempAlloc(EHS_POST_STRING_LENGTH_MAX); //more than enough?ehs_char * PostString;
    if (!PostString)
    {
        EhsStrcpy(EHS_FB_OUT_S(EHS_GETAPP_GETLIST_ERROR_DO), "Insufficient memory");
        EHS_FB_FINISH(EHS_GETAPP_GETLIST_ERROR_EO);
        goto memory_error;
    }
    PostString[0]='\0';
    /* Allocate a buffer */
    ObjData->URL_write_data_buffer_struct=EhsHDoAllGenericConfig(ObjData->curl,&ObjData->server_info, 64*1024, 50000,480);/* 2 minute timeouts and 50ms chunk gap */
    EhsHSetUpLocalProxy(ObjData->curl);
    if (!ObjData->URL_write_data_buffer_struct)
    {
        EhsStrcpy(EHS_FB_OUT_S(EHS_GETAPP_GETLIST_ERROR_DO), "Could not configure URL get");
        EHS_FB_FINISH(EHS_GETAPP_GETLIST_ERROR_EO);
        goto memory_error;
    }
    if (EHS_FB_IN_CONNECTED(EHS_GETAPP_GETLIST_SERVER_DI))
    {
        EhsStrcpy(ObjData->szUrl, EHS_FB_IN_S(EHS_GETAPP_GETLIST_SERVER_DI));
    }
    EhsStrcpy(szFullURL,ObjData->szUrl);
    EhsStrcat(szFullURL,EHS_DEVMAN_APPGETLISTURLPATH);
    EhsHCreateQueryString(ObjData->curl,PostString, "KeyWords", EHS_FB_IN_S(EHS_GETAPP_GETLIST_KEYWORDS_DI),EHS_POST_STRING_LENGTH_MAX);
    EhsHCreateQueryString(ObjData->curl,PostString, "ID", ObjData->szID,EHS_POST_STRING_LENGTH_MAX);
    if (EhsHURLConfigPostGet(ObjData->curl,ObjData->URL_write_data_buffer_struct,szFullURL, PostString,EHS_TRUE))
    {
        http_no=EhsHURLdoRequest(ObjData->curl);
    }
    else http_no=0;
    if (http_no == 200)
    {
        szXml=EhsHURLget_write_data_buffer(ObjData->URL_write_data_buffer_struct);
        EhsStrcpy(EHS_FB_OUT_S(EHS_GETAPP_GETLIST_LIST_DO), szXml);
        EhsStrcpy(EHS_FB_OUT_S(EHS_GETAPP_GETLIST_ERROR_DO), "OK");
        EHS_FB_FINISH(EHS_GETAPP_GETLIST_FINISH_EO);
    }
    else
    {
        EhsStrcpy(EHS_FB_OUT_S(EHS_GETAPP_GETLIST_ERROR_DO), "Can not access URL");
        EhsStrcpy(EHS_FB_OUT_S(EHS_GETAPP_GETLIST_LIST_DO), "");
        EHS_FB_FINISH(EHS_GETAPP_GETLIST_ERROR_EO);
    }

memory_error:
    if (ObjData->URL_write_data_buffer_struct)
    {
        EhsHURLfree_write_data_buffer(ObjData->URL_write_data_buffer_struct);
        ObjData->URL_write_data_buffer_struct = NULL;
    }
    if (PostString) EhsHMem_tempFree(PostString);
    Ehs_FB_ThreadComplete();
    EhsHThread_exit();
}

EHS_FB_RUN_FUNCTION(appget_list_remote)
{
    appgetObj *ObjData = (appgetObj*)EHS_FB_RUN_CONTEXT;
    if (!ObjData->curl)
    {
        EhsStrcpy(EHS_FB_OUT_S(EHS_GETAPP_GETLIST_LIST_DO),"");
        EhsStrcpy(EHS_FB_OUT_S(EHS_GETAPP_GETLIST_ERROR_DO),"initialisation failed");
        EHS_FB_FINISH(EHS_GETAPP_GETLIST_ERROR_EO);
        return;
    }
    if (ObjData->bBusy==EHS_FALSE)
    {
        EHS_FB_START_THREAD(appget_list_remote,-90);
    }
    else
    {
        ObjData->bBusy=EHS_TRUE;
        EHS_FB_FINISH(EHS_GETAPP_GETLIST_ERROR_EO);
        EhsStrcpy(EHS_FB_OUT_S(EHS_GETAPP_GETLIST_LIST_DO),"");
        EhsStrcpy(EHS_FB_OUT_S(EHS_GETAPP_GETLIST_ERROR_DO),"busy");
    }
}
