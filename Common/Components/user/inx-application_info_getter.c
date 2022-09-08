/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-application_info_getter.h"
#include "hal_devapps.h" // Use the app downloader code from here
//todo - the following includes should n't be used. Use the HAL methods instead.
#include <sys/types.h>
#include <dirent.h>
//ICB HEADER MACRO END -- DO NOT ALTER
//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct
{
    ehs_char appdataPath[EHS_MAXPATHLENGTH];
} inx_application_info_getter_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(application_info_getter)

EHS_FB_FUNCTION_ENTRY("getList", 0x00, application_info_getter_getList)

EHS_FB_FUNCTION_ENTRY("getInfo", 0x01, application_info_getter_getInfo)

EHS_FB_FUNCTION_ENTRY("setDefault", 0x02, application_info_getter_setDefault)

EHS_FB_FUNCTION_ENTRY("deleteApp", 0x03, application_info_getter_deleteApp)

EHS_FB_FUNCTION_ENTRY("getDefault", 0x04, application_info_getter_getDefault)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */

/* getList */
#define INX_application_info_getter_ARG_getList_keywords_DI 1
#define INX_application_info_getter_ARG_getList_amount_DO 1
#define INX_application_info_getter_ARG_getList_list_DO 2
#define INX_application_info_getter_ARG_getList_errors_DO 3
#define INX_application_info_getter_ARG_getList_xxxxxlist_EO 1
#define INX_application_info_getter_ARG_getList_errorlist_EO 2

/* getInfo */
#define INX_application_info_getter_ARG_getInfo_app_nameinfo_DI 1
#define INX_application_info_getter_ARG_getInfo_errors_DO 1
#define INX_application_info_getter_ARG_getInfo_name_DO 2
#define INX_application_info_getter_ARG_getInfo_commercial_Name_DO 3
#define INX_application_info_getter_ARG_getInfo_version_DO 4
#define INX_application_info_getter_ARG_getInfo_description_DO 5
#define INX_application_info_getter_ARG_getInfo_info_DO 6
#define INX_application_info_getter_ARG_getInfo_xxxxxinfo_EO 1
#define INX_application_info_getter_ARG_getInfo_errorinfo_EO 2

/* setDefault */
#define INX_application_info_getter_ARG_setDefault_app_namesetDefault_DI 1
#define INX_application_info_getter_ARG_setDefault_errors_DO 1
#define INX_application_info_getter_ARG_setDefault_xxxxxsetDefault_EO 1
#define INX_application_info_getter_ARG_setDefault_errorsetDefault_EO 2

/* deleteApp */
#define INX_application_info_getter_ARG_deleteApp_app_namedelete_DI 1
#define INX_application_info_getter_ARG_deleteApp_errors_DO 1
#define INX_application_info_getter_ARG_deleteApp_xxxxxdelete_EO 1
#define INX_application_info_getter_ARG_deleteApp_errordelete_EO 2

/* getDefault */
#define INX_application_info_getter_ARG_getDefault_errors_DO 1
#define INX_application_info_getter_ARG_getDefault_app_namegetDefault_DO 2
#define INX_application_info_getter_ARG_getDefault_xxxxxgetDefault_EO 1
#define INX_application_info_getter_ARG_getDefault_errorgetDefault_EO 2

//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER
//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
/* Parameters */
/* Create some macros for the default parameters */
//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(application_info_getter)
{
    /* Uncomment the following if you need to parse the parameters to calculate memory required */
    /*
    	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,""); */
    EHS_FB_IDENTIFY_MEMORY = sizeof(inx_application_info_getter_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(application_info_getter)
{
    ehs_bool bRet = EHS_TRUE; /* assume success */
    //this is the reference to the object data for this instance of the function block
    inx_application_info_getter_state_type* inx_application_info_getter_state = (inx_application_info_getter_state_type*)EHS_FB_INIT_CONTEXT;
    /* read the initialisation parameters */
    //EhsSscanf(EHS_FB_INIT_PARAMETERS,"");

    /* Add any further intialisation code here */
    //get the canonical path to the appdata directory
    inx_application_info_getter_state_type *state=inx_application_info_getter_state;
    if(!EhsTF_tryCanonicPath(state->appdataPath,EHS_RUNTIME_APPDATA_DIR,"",EHS_FALSE))
    {
        EHSH_LOG_ERROR("Could not get appdata directory\n");
    }
    /* Add any further intialisation code here */
    return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB FUNCTION getList MACRO START -- DO NOT ALTER
/**
 * Definition of application_info_getter_getList.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 *  @todo Should use the HAL version of the readdir
 */
EHS_FB_RUN_FUNCTION(application_info_getter_getList)
{
    inx_application_info_getter_state_type* inx_application_info_getter_state = (inx_application_info_getter_state_type*)EHS_FB_RUN_CONTEXT;

    // Your code here
    inx_application_info_getter_state_type *state=inx_application_info_getter_state;
    ehs_char *appdatapath=state->appdataPath;
    ehs_char *filename;
    ehs_uint32 count=0;
    ehs_uint32 csvlen=0;
    ehs_char csv[EHS_STRING_LENGTH_MAX];
    csv[0]='\0';
    DIR *appdir;
    struct dirent *entry;

    //@todo - implement keyword search
    if (EHS_FB_IN_CONNECTED_API2(INX_application_info_getter_ARG_getList_keywords_DI))
    {
        EHS_FB_IN_S_API2(INX_application_info_getter_ARG_getList_keywords_DI) ;
    }

    //try to open the appdata directory and get the list of directories in there
    appdir=opendir(appdatapath);
    //test for errors
    if(appdir==NULL)
    {
        EHSH_LOG_ERROR("Failed to open appdata directory with path:%s",appdatapath);
        EhsStrcpy(EHS_FB_OUT_S_API2(INX_application_info_getter_ARG_getList_errors_DO), "Failed to open app data dir.");
        EHS_FB_FINISH(INX_application_info_getter_ARG_getList_errorlist_EO);
        return;
    }
    //read all the entries from the directory, entry will equal NULL when we reach the end
    while((entry=readdir(appdir))!=NULL)
    {
        //current filename is entry->d_name
        filename=(ehs_char*)entry->d_name;
        //need to test filename, is it . or .. (in which case ignore), then check is it a directory
        // also ignore canonical names that include _dl, _prev as these are the temp download and backup install folders respectively
        if((EhsStrcmp(filename,".")!=0) && (EhsStrcmp(filename,"..")!=0) && (EhsStrcmp(filename,"fallbacks")!=0) && (EhsStrstr(filename, "_dl") == NULL) && (EhsStrstr(filename, "_prev") == NULL))
        {
            //each one of these is a real app so increment the count and append to the CSV string
            count++;
            csvlen=EhsStrlen(csv);
            if(csvlen!=0)
            {
                //not first directory so prepend comma
                EhsStrcpy(&csv[csvlen],",");
                csvlen++;
            }
            EhsStrcpy(&csv[csvlen],filename);
        }
    }
    //close the directory assuming we finished with it
    if(closedir(appdir)!=0)
    {
        EHSH_LOG_ERROR("Failed to close appdata directory with path:%s\n",appdatapath);
    }
    if (EHS_FB_OUT_CONNECTED_API2(INX_application_info_getter_ARG_getList_list_DO))
    {
        EhsStrcpy(EHS_FB_OUT_S_API2(INX_application_info_getter_ARG_getList_list_DO),csv);
    }
    if (EHS_FB_OUT_CONNECTED_API2(INX_application_info_getter_ARG_getList_amount_DO))
    {
        EHS_FB_OUT_I_API2(INX_application_info_getter_ARG_getList_amount_DO)=count;
    }
    EhsStrcpy(EHS_FB_OUT_S_API2(INX_application_info_getter_ARG_getList_errors_DO), "OK.");
    EHS_FB_FINISH(INX_application_info_getter_ARG_getList_xxxxxlist_EO);
}//ICB FUNCTION getList MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION getInfo MACRO START -- DO NOT ALTER
/**
 * Definition of application_info_getter_getInfo.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(application_info_getter_getInfo)
{
    inx_application_info_getter_state_type* inx_application_info_getter_state = (inx_application_info_getter_state_type*)EHS_FB_RUN_CONTEXT;

    // Your code here
    inx_application_info_getter_state_type *state=inx_application_info_getter_state;
    ehs_char* appdatapath=state->appdataPath;
    ehs_char appname[EHS_STRING_LENGTH_MAX];
    ehs_char infopath[EHS_STRING_LENGTH_MAX];

    ehs_char returndata[EHS_STRING_LENGTH_MAX];
    char * element_start;
    char element_cropped[EHS_STRING_LENGTH_MAX];

    ehs_char cName[EHS_STRING_LENGTH_MAX];
    ehs_char cCommercialName[EHS_STRING_LENGTH_MAX];
    ehs_char cVersion[EHS_STRING_LENGTH_MAX];
    ehs_char cDescription[EHS_STRING_LENGTH_MAX];

    cName[0] = '\0';
    cCommercialName[0] = '\0';
    cVersion[0] = '\0';
    cDescription[0] = '\0';
    infopath[0]='\0';

    EhsStrcpy(cName,"No Name Found");
    EhsStrcpy(cCommercialName,"No Commercial Name Found");
    EhsStrcpy(cVersion,"No Version Found");
    EhsStrcpy(cDescription,"No Description Found");

    //infopath should be a path to the info xml. We should now read it into our XML parser and get out the data we want
    FILE *infofile;
    if(EHS_FB_IN_CONNECTED_API2(INX_application_info_getter_ARG_getInfo_app_nameinfo_DI))
    {
        EhsStrcpy(appname,EHS_FB_IN_S_API2(INX_application_info_getter_ARG_getInfo_app_nameinfo_DI));
        //create the infopath
        EhsStrcat(infopath,appdatapath);
        EhsStrcat(infopath,appname);
        //EhsStrcat(infopath,"/info.xml");
        EhsStrcat(infopath,"/");
        EhsStrcat(infopath,EHS_SYS_APP_INFO_FILENAME);

        //now try to open the file
        infofile=EhsFopen(infopath,"r");
        if(infofile==NULL)
        {
            EHSH_LOG_WARNING("Could not open:%s\n",infopath);
            EhsStrcpy(EHS_FB_OUT_S_API2(INX_application_info_getter_ARG_getInfo_errors_DO), "Failed to open info.xml.");
            EHS_FB_FINISH(INX_application_info_getter_ARG_getInfo_errorinfo_EO);
            return;
        }

        //todo 2022 don't do this for ESP32
#ifndef EHS_ESP32_SUPPORT
        // read to end of file
        while (EhsFgets(returndata,EHS_STRING_LENGTH_MAX,infofile) != NULL)
        {

            if (element_start = Ehs_ReadXMLTag(returndata, EHS_SYS_APP_INFO_XMLTAG_NAME))
            {
                Ehs_CopyXMLTagElement(element_cropped, element_start, EHS_STRING_LENGTH_MAX, EHS_TRUE);
                EhsStrcpy(cName,element_cropped);
            }
            if (element_start = Ehs_ReadXMLTag(returndata, EHS_SYS_APP_INFO_XMLTAG_COMM_NAME))
            {
                Ehs_CopyXMLTagElement(element_cropped, element_start, EHS_STRING_LENGTH_MAX, EHS_TRUE);
                EhsStrcpy(cCommercialName,element_cropped);
            }
            if (element_start = Ehs_ReadXMLTag(returndata, EHS_SYS_APP_INFO_XMLTAG_VERSION))
            {
                Ehs_CopyXMLTagElement(element_cropped, element_start, EHS_STRING_LENGTH_MAX, EHS_TRUE);
                EhsStrcpy(cVersion,element_cropped);
            }
            if (element_start = Ehs_ReadXMLTag(returndata, EHS_SYS_APP_INFO_XMLTAG_DESCRIPTION))
            {
                Ehs_CopyXMLTagElement(element_cropped, element_start, EHS_STRING_LENGTH_MAX, EHS_TRUE);
                EhsStrcpy(cDescription,element_cropped);
            }
        }
        EhsFclose(infofile);
#endif
    }
    else
    {
        EHSH_LOG_WARNING("Trying to get application info with no name given.");
        EhsStrcpy(EHS_FB_OUT_S_API2(INX_application_info_getter_ARG_getInfo_errors_DO), "No app name supplied.");
        EHS_FB_FINISH(INX_application_info_getter_ARG_getInfo_errorinfo_EO);
        return;
    }

    //@todo - output whole xml chunk to another port for future extensibility
    if (EHS_FB_OUT_CONNECTED_API2(INX_application_info_getter_ARG_getInfo_name_DO))
        EhsStrcpy(EHS_FB_OUT_S_API2(INX_application_info_getter_ARG_getInfo_name_DO),cName);
    if (EHS_FB_OUT_CONNECTED_API2(INX_application_info_getter_ARG_getInfo_commercial_Name_DO))
        EhsStrcpy(EHS_FB_OUT_S_API2(INX_application_info_getter_ARG_getInfo_commercial_Name_DO),cCommercialName);
    if (EHS_FB_OUT_CONNECTED_API2(INX_application_info_getter_ARG_getInfo_version_DO))
        EhsStrcpy(EHS_FB_OUT_S_API2(INX_application_info_getter_ARG_getInfo_version_DO),cVersion);
    if (EHS_FB_OUT_CONNECTED_API2(INX_application_info_getter_ARG_getInfo_description_DO))
        EhsStrcpy(EHS_FB_OUT_S_API2(INX_application_info_getter_ARG_getInfo_description_DO),cDescription);

    EhsStrcpy(EHS_FB_OUT_S_API2(INX_application_info_getter_ARG_getInfo_errors_DO), "OK.");
    EHS_FB_FINISH(INX_application_info_getter_ARG_getInfo_xxxxxinfo_EO);
}//ICB FUNCTION getInfo MACRO END -- DO NOT ALTER THIS LINE
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(application_info_getter)
{
    inx_application_info_getter_state_type *inx_application_info_getter_state = (inx_application_info_getter_state_type*)EHS_FB_DESTROY_CONTEXT;
    //Your code below here
}//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION getList MACRO END -- DO NOT ALTER
//ICB FUNCTION setDefault MACRO START -- DO NOT ALTER
/**
 * Definition of application_info_getter_setDefault.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(application_info_getter_setDefault)
{
    ehs_bool succ = EHS_FALSE;
    inx_application_info_getter_state_type* inx_application_info_getter_state = (inx_application_info_getter_state_type*)EHS_FB_RUN_CONTEXT;

    // Your code here
    if (EHS_FB_IN_CONNECTED_API2(INX_application_info_getter_ARG_setDefault_app_namesetDefault_DI))
    {
        succ = EhsAppSetDefaultApp(EHS_FB_IN_S_API2(INX_application_info_getter_ARG_setDefault_app_namesetDefault_DI));
    }
    if (succ == EHS_TRUE)
    {
        EhsStrcpy(EHS_FB_OUT_S_API2(INX_application_info_getter_ARG_setDefault_errors_DO), "OK.");
        EHS_FB_FINISH(INX_application_info_getter_ARG_setDefault_xxxxxsetDefault_EO);
    }
    else
    {
        EhsStrcpy(EHS_FB_OUT_S_API2(INX_application_info_getter_ARG_setDefault_errors_DO), "Failed to set app as default.");
        EHS_FB_FINISH(INX_application_info_getter_ARG_setDefault_errorsetDefault_EO);
    }
    return;
}
//ICB FUNCTION setDefault MACRO END -- DO NOT ALTER
//ICB FUNCTION deleteApp MACRO START -- DO NOT ALTER
/**
 * Definition of application_info_getter_deleteApp.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(application_info_getter_deleteApp)
{
    ehs_bool succ = EHS_FALSE;
    inx_application_info_getter_state_type* inx_application_info_getter_state = (inx_application_info_getter_state_type*)EHS_FB_RUN_CONTEXT;

    // Your code here

    if (EHS_FB_IN_CONNECTED_API2(INX_application_info_getter_ARG_deleteApp_app_namedelete_DI))
    {
        //@todo - complete
        // delete app folder if exists

        EHS_FB_IN_S_API2(INX_application_info_getter_ARG_deleteApp_app_namedelete_DI) ;
    }

    if (succ == EHS_TRUE)
    {
        EhsStrcpy(EHS_FB_OUT_S_API2(INX_application_info_getter_ARG_deleteApp_errors_DO), "OK.");
        EHS_FB_FINISH(INX_application_info_getter_ARG_deleteApp_xxxxxdelete_EO);
    }
    else
    {
        EhsStrcpy(EHS_FB_OUT_S_API2(INX_application_info_getter_ARG_deleteApp_errors_DO), "Failed to uninstall app.");
        EHS_FB_FINISH(INX_application_info_getter_ARG_deleteApp_errordelete_EO);
    }
    return;
}
//ICB FUNCTION deleteApp MACRO END -- DO NOT ALTER
//ICB FUNCTION getDefault MACRO START -- DO NOT ALTER
/**
 * Definition of application_info_getter_getDefault.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(application_info_getter_getDefault)
{
    inx_application_info_getter_state_type* inx_application_info_getter_state = (inx_application_info_getter_state_type*)EHS_FB_RUN_CONTEXT;

    // Your code here
    if (EHS_FB_OUT_CONNECTED_API2(INX_application_info_getter_ARG_getDefault_app_namegetDefault_DO))
    {
        EhsAppGetDefaultApp(EHS_FB_OUT_S_API2(INX_application_info_getter_ARG_getDefault_app_namegetDefault_DO));
    }
    //always succeeds
    EhsStrcpy(EHS_FB_OUT_S_API2(INX_application_info_getter_ARG_getDefault_errors_DO), "OK.");
    EHS_FB_FINISH(INX_application_info_getter_ARG_getDefault_xxxxxgetDefault_EO);

    return;
}
//ICB FUNCTION getDefault MACRO END -- DO NOT ALTER
