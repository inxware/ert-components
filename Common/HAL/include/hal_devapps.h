/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file hal_console.h
 * In this file, all of the hardware abstraction layer declarations provided for EHS console I/O are given.
 *
 * @author: inx limited
 *
 */

#ifndef EHS_HAL_DEVAPPS_H
#define EHS_HAL_DEVAPPS_H

#include "globals.h" // this includes the macros, and ehs types
#if defined(EHS_COMPONENT_NETWORKING_SUPPORT) && !defined(EHS_COMPONENTS_NETWORK_URL_GET__NONE)
#include "hal_url.h" // need thearchive and libcurl structs
#endif //EHS_COMPONENT_NETWORKING_SUPPORT
#include "hal_devman.h"


/* macros used for applications */

#include "hal_appstorage.h"

#include "hal_devapps_api.h"

/* internal (private) structures */

/* Internal structure used to store archive data that is being retrieved */

typedef struct AppGet_URL_data_buffer
{
    ehs_char * buffer;
    ehs_uint32 size_read;
    ehs_uint32 buffer_size;
    ehs_bool data_ready;
    struct archive *ar; // this is the write archive handle that is passed to the writer.
} AppGet_URL_data_buffer_Type;

//*todo share this between the component and HAL version...
// but do we want the PFI data in the HAL??? probably not so need to do something about the firing functions

typedef struct
{
    ehs_bool bBusy; 				/* flag not to use multiple functions simultaneously */
    ehs_bool bRunWhenDone; /* Flag to run the app when it has been downloaded */
    ehs_bool bSetAsDefaultApp; /* Flag to set this as the default app */
    ehs_sint32 nInstallMode; /* 0 install standard app, 1 install as home app, 2 as temp app */
#if defined(EHS_COMPONENT_NETWORKING_SUPPORT) && !defined(EHS_COMPONENTS_NETWORK_URL_GET__NONE)
    AppGet_URL_data_buffer_Type AppGet_write_data_buffer_struct; // this is for the standard URL get/post e.g. for info gets
    EhsH_URLwrite_data_bufferType * URL_write_data_buffer_struct; // this is the special buffer struct for the un archiver into which data is pumped from URL read to libarchive
    EhsNetworkServerInfo_t server_info; //@todo this should be updated from a central repository for the interface. Need a new function block to populate it.
#endif //EHS_COMPONENT_NETWORKING_SUPPORT
    ehs_bool bFreeWhenDone; //Set this to True if you want the downloader thread to remove this struct when it's finished (not used for function blocks
#if defined(EHS_COMPONENT_NETWORKING_SUPPORT) && !defined(EHS_COMPONENTS_NETWORK_URL_GET__NONE)
    CURL * curl; //use a global handle
#endif //EHS_COMPONENT_NETWORKING_SUPPORT
    ehs_char szID[EHS_STRING_LENGTH_MAX];  /* Key the server may require to make the download*/
    ehs_char szAppCanonicalName[EHS_MAXDEVMANNAMELEN]; /* App name to retrieve */
    ehs_char szAppLocalName[EHS_MAXDEVMANNAMELEN]; /* App name to retrieve */
    ehs_char szUrl[EHS_MAXDEVMANNAMELEN];	  /* url from where to retrieve the app - must be a devman server */
    ehs_char szUrlAppGetExtPath[EHS_MAXPATHLENGTH]; /* This can be populated with the path so that szURL acts as a base for other stuff */
    ehs_char szKeywords[EHS_STRING_LENGTH_MAX];     /* This is used to query the devman server for matching apps - this is XML format */
    /* <feature> [string] <version> [major.minor.vminor.vvminor]> <\version><\feature> */
    ehs_char szpDownloadDir[EHS_MAXPATHLENGTH];
} appgetObj;




/* High Level System environment for application directory / storage management */

/* Function to call to download apps. */

ehs_bool start_appget_getapp(const ehs_char * appname, const ehs_char * localname, ehs_char * server_base, ehs_bool block, ehs_bool runFlag, ehs_bool defaultAppFlag);

ehs_bool EhsAppSetDownloadOKToken(ehs_char * canonicalName);

ehs_uint8 EhsAppCheckAndSwitchDownloadDir(ehs_char * canonicalName); /* If a validated download directory exists this is switched with any current version. */
ehs_bool EhsAppGetDefaultApp(ehs_char * cDefaultApp); /* Gets the canonical name of the default app specified in the app2run file */
ehs_bool EhsAppInitLiveAppDefaultDir(); /* This sets the environment for the default application that is to be started. read from file */
ehs_bool EhsAppMakeDownloadString(ehs_char * dest, const ehs_char * dirname);
ehs_bool EhsAppMakeLiveDirString(ehs_char * dest, const ehs_char * dirname);
ehs_bool EhsAppMakePreviousDirString(ehs_char * dest, const ehs_char * dirname);

/* Public functions used by the downloader */

EhsThreadFuncReturnType HAL_appGetWaitForURLDataAndWrite(void *XObjData);
EhsThreadFuncReturnType HAL_AppGetRead_data(void *XObjData);

#endif
