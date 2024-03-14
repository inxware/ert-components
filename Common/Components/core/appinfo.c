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
#include "globals.h"
#include "appinfo.h"
#include "app_data.h" // IS_CONNECTED_IN()

#include "setCompletes.h"


#include "hal-api.h" //needed for  version info
#include "hal_devman.h"
#define CR '\r'
#define LF '\n'


/**
 * file object type structure...
 */
typedef struct
{
    ehs_char szAppName[EHS_MAXDEVMANNAMELEN];
} appinfoObj;


/******************************************************************************/
/* Define app info get function block */

//EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(appinfo);
//EHS_GLOBAL EHS_FB_INIT_FUNCTION(appinfo);
//EHS_GLOBAL EHS_FB_RUN_FUNCTION(appinfo_getinfo);

EHS_FB_FUNCTIONS_START(appinfo)

EHS_FB_FUNCTION_ENTRY("getinfo", 0x01, appinfo_getinfo)
EHS_FB_FUNCTIONS_END

//define the port mappings
#define EHS_APPINFO_GETSTAT_PORT_FINISH 1

#define EHS_APPINFO_GETINFO_VERSION_OUT 0
#define EHS_APPINFO_GETINFO_ITERATION_OUT 1
#define EHS_APPINFO_GETINFO_DATE_OUT 2
#define EHS_APPINFO_GETINFO_MIN_EHS_OUT 3
#define EHS_APPINFO_GETINFO_DEPENDENCIES_OUT 4
#define EHS_APPINFO_GETINFO_PROC_GRPS_OUT 5
#define EHS_APPINFO_GETINFO_APP_NAME 6

#define EHS_APPINFO_GETINFO_NAME_IN 0

/**
 * Identify the function block. Determine the memory required.
 *
 * This function provides access to:
 *  EHS_FB_IDENTIFY_PARAMETERS - string containing parameter text
 *  EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this
 *   function block's context
 *
 */
EHS_FB_IDENTIFY_FUNCTION(appinfo)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(appinfoObj);
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(appinfo)
{
    appinfoObj *ObjData = (appinfoObj*)EHS_FB_INIT_CONTEXT;
    EhsSscanf(EHS_FB_INIT_PARAMETERS,"%s",ObjData->szAppName);
    return EHS_TRUE; /* initialisation always succeeds */
}

/* This is called for file gets - we don't do network gets here. Leave that to the appget FB */
EHS_FB_THREAD_FUNCTION(appinfo_getinfo_thread)
{
    EHSH_LOG_ERROR("Not implemented non resident apps yet");
    return EHS_FALSE;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION( appinfo_getinfo)
{
    appinfoObj *ObjData = (appinfoObj*)EHS_FB_RUN_CONTEXT;
    if (EHS_FB_IN_CONNECTED(EHS_APPINFO_GETINFO_NAME_IN))
    {
        EhsStrcpy(ObjData->szAppName, EHS_FB_IN_S(EHS_APPINFO_GETINFO_NAME_IN));
    }
    if (EhsStrlen(ObjData->szAppName) == 0 || EhsStrcmp(ObjData->szAppName,"*") || EhsStrcmp(ObjData->szAppName,"default"))   /* If nothing then assume the current application */
    {
        EhsStrcpy(EHS_FB_OUT_S(EHS_APPINFO_GETINFO_VERSION_OUT),EhsHAppMetaGetVersionNumber());
        EHS_FB_OUT_I(EHS_APPINFO_GETINFO_ITERATION_OUT)= EhsHAppMetaGetBuildNumber();
        EhsStrcpy(EHS_FB_OUT_S(EHS_APPINFO_GETINFO_DATE_OUT),EhsHAppMetaGetAppDate());
        EhsStrcpy(EHS_FB_OUT_S(EHS_APPINFO_GETINFO_MIN_EHS_OUT), "n/a");//@todo
        EhsStrcpy(EHS_FB_OUT_S(EHS_APPINFO_GETINFO_DEPENDENCIES_OUT), "n/a"); //@todo
        EhsStrcpy(EHS_FB_OUT_S(EHS_APPINFO_GETINFO_PROC_GRPS_OUT), "n/a");//@todo
        EhsStrcpy(EHS_FB_OUT_S(EHS_APPINFO_GETINFO_APP_NAME),EhsHAppMetaGetAppName());
        EHS_FB_FINISH(EHS_APPINFO_GETSTAT_PORT_FINISH);
    }
    else     /* This might be slow so create a thread*/
    {
        EHS_FB_START_THREAD(appinfo_getinfo_thread, -90);
    }
}


