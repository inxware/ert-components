/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-webkit.h"
//ICB HEADER MACRO END -- DO NOT ALTER

#include "hal_webkit.h"
#include "ehs_fb_types.h"
#include "guiparams.h" /* for EhsGuiParamsType */

//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct
{
    ehs_char ScriptURL[EHS_STRING_LENGTH_MAX];
    ehs_bool Data;
    ehs_char Function[EHS_STRING_LENGTH_MAX];
    ehs_bool visible;
    ehs_bool created;
    ehs_char BaseURL[EHS_STRING_LENGTH_MAX];
    EhsGuiParamsType xParams;
    ehs_sint32 timeout;
} inx_html_java_state_type; //Reference this, maybe store your config parameters in here too.

//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(html_java)

EHS_FB_FUNCTION_ENTRY("LoadScript", 0x01, html_java_LoadScript)

EHS_FB_FUNCTION_ENTRY("RunFunction", 0x02, html_java_RunFunction)

EHS_FB_FUNCTION_ENTRY("UnLoadAll", 0x03, html_java_UnLoadAll)

EHS_FB_FUNCTION_ENTRY("setVisibile", 0x04, html_java_setVisibile)

EHS_FB_FUNCTION_ENTRY("setInvisible", 0x05, html_java_setInvisible)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_html_java_ARG_LoadScript_ScriptURL 1
#define INX_html_java_ARG_LoadScript_BaseURL 2
#define INX_html_java_ARG_LoadScript_Loaded 1
#define INX_html_java_ARG_RunFunction_Function 1
#define INX_html_java_ARG_RunFunction_jsonIN 2
#define INX_html_java_ARG_RunFunction_jsonOUT 1
#define INX_html_java_ARG_RunFunction_Complete 1
#define INX_html_java_ARG_UnLoadAll_Unloaded 1
#define INX_html_java_ARG_setVisibile_visSet 1
#define INX_html_java_ARG_setInvisible_set 1
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER
//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
/* Parameters */
/* Create some macros for the default parameters */
#define INX_FB_html_java_ScriptURL
#define INX_FB_html_java_Data 1
#define INX_FB_html_java_Function JSmain
#define INX_FB_html_java_BaseURL
//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(html_java)
{
    /* Uncomment the following if you need to parse the parameters to calculate memory required */
    /*
    	ehs_char* ScriptURL;
    	ehs_bool Data;
    	ehs_char* Function;
    	ehs_char* BaseURL;
    	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,"%s %d %s %s",&ScriptURL,&Data,&Function,&BaseURL); */
    EHS_FB_IDENTIFY_MEMORY = sizeof(inx_html_java_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */



EHS_FB_INIT_FUNCTION(html_java)
{
    /*
     *
    ehs_char* ScriptURL;
    ehs_bool Data;
    ehs_char* Function;
    ehs_char* BaseURL;*/
    ehs_bool bRet = EHS_TRUE;
    /* assume success */
    //this is the reference to the object data for this instance of the function block
    inx_html_java_state_type* inx_html_java_state = (inx_html_java_state_type*)EHS_FB_INIT_CONTEXT;
    /* read the initialisation parameters */


    char guiParams[MAX_PARAM_STR_LEN];
    const char* pParams;
    //EhsWidgetClass *pWidget;
    /* Parse the GUI parameters */
    pParams = ReadParmFile(&EHS_FB_INIT_PARAMETERS[4], guiParams);
    if (guiParams) {
        EhsParseGuiParameters(guiParams,&inx_html_java_state->xParams);

        /* we probably don't need to add the widget to the viewport blitter - but if we do :*/
        //*(EhsWidgetClass**)EHS_FB_RUN_CONTEXT =	EhsWidgetViewport_init(&xParams.xRect, xParams.nZorder, xParams.uClass.xPatch);//@todo this should be params

        EhsSscanf(pParams,"%s %c %s %s",inx_html_java_state->ScriptURL,&inx_html_java_state->Data,inx_html_java_state->Function,inx_html_java_state->BaseURL);
        inx_html_java_state->timeout = -1; // todo add a FB parameter
        /* Add any further intialisation code here */
    }
    else {
            (*(EhsWidgetClass**)EHS_FB_INIT_CONTEXT = NULL);
            // don't flag error we may still want the app to run without a UI
    }

    return bRet; /* initialisation always succeeds */
}

//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(html_java)
{
    inx_html_java_state_type *inx_html_java_state = (inx_html_java_state_type*)EHS_FB_DESTROY_CONTEXT;
    //Your code below here
    EhsHWebkitDestroy();
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION LoadScript MACRO START -- DO NOT ALTER
/**
 * Definition of html_java_LoadScript.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(html_java_LoadScript)
{
    inx_html_java_state_type* inx_html_java_state = (inx_html_java_state_type*)EHS_FB_RUN_CONTEXT;
    ehs_char * baseURL = inx_html_java_state->BaseURL;
    ehs_char * scriptURL = inx_html_java_state->ScriptURL;
    // Your code here

    LOGI("x = %d, y= %d, w= %d, h=%d",inx_html_java_state->xParams.xRect.nLeft,inx_html_java_state->xParams.xRect.nTop,inx_html_java_state->xParams.xRect.nWidth,inx_html_java_state->xParams.xRect.nHeight);
    if (EHS_FB_IN_CONNECTED_API2(INX_html_java_ARG_LoadScript_BaseURL))
    {
        //LOGI("html_java_LoadScript 1.1");
        baseURL = EHS_FB_IN_S_API2(INX_html_java_ARG_LoadScript_BaseURL) ;

    }
    else
    {
        baseURL = inx_html_java_state->BaseURL;
    }
    if (EHS_FB_IN_CONNECTED_API2(INX_html_java_ARG_LoadScript_ScriptURL))
    {
        //LOGI("html_java_LoadScript 3");
        scriptURL =  EHS_FB_IN_S_API2(INX_html_java_ARG_LoadScript_ScriptURL);

    }
    else
    {
        scriptURL = inx_html_java_state->ScriptURL;
    }
    inx_html_java_state->created = EhsHWebkitLoad(baseURL,scriptURL,inx_html_java_state->Data,inx_html_java_state->xParams.xRect.nLeft,inx_html_java_state->xParams.xRect.nTop,inx_html_java_state->xParams.xRect.nWidth,inx_html_java_state->xParams.xRect.nHeight,inx_html_java_state->xParams.uClass.xPatch.sComp.nAlpha,inx_html_java_state->timeout);
    /* todo - we probably want an error event here!*/
    //LOGI("html_java_LoadScript 6");
    EHS_FB_FINISH(INX_html_java_ARG_LoadScript_Loaded);
}//ICB FUNCTION LoadScript MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION RunFunction MACRO START -- DO NOT ALTER
/**
 * Definition of html_java_RunFunction.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(html_java_RunFunction)
{
    inx_html_java_state_type* inx_html_java_state = (inx_html_java_state_type*)EHS_FB_RUN_CONTEXT;
    ehs_char * run_func = NULL;
    ehs_char * arg = NULL;
    ehs_char * default_run_func = "main";

    // Your code here
    if (EHS_FB_IN_CONNECTED_API2(INX_html_java_ARG_RunFunction_Function))
        run_func = EHS_FB_IN_S_API2(INX_html_java_ARG_RunFunction_Function) ;
    else
    {
        if (inx_html_java_state->Function != NULL)
            run_func = inx_html_java_state->Function;
        else
            run_func = default_run_func;
    }

    if (EHS_FB_IN_CONNECTED_API2(INX_html_java_ARG_RunFunction_jsonIN))
        arg = EHS_FB_IN_S_API2(INX_html_java_ARG_RunFunction_jsonIN) ;
    else
        arg = inx_html_java_state->Data;
    /* todo the following will need a thread and callbacks .... */
    if (EHS_FB_OUT_CONNECTED_API2(INX_html_java_ARG_RunFunction_jsonOUT))
    {
        EhsHWebkitRunFunction(inx_html_java_state->BaseURL,	run_func,arg);
        EhsStrcpy(EHS_FB_OUT_S_API2(INX_html_java_ARG_RunFunction_jsonOUT),"Not Yet Implemented"); /* todo */
    }
    else
    {
        EHSH_LOG_ERROR("Calling  EhsHWebkitRunFunction(%s,%s,%s)",inx_html_java_state->BaseURL,	run_func,arg)
        EhsHWebkitRunFunction(inx_html_java_state->BaseURL,	run_func,arg);
    }
    EHS_FB_FINISH(INX_html_java_ARG_RunFunction_Complete);
}//ICB FUNCTION RunFunction MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION UnLoadAll MACRO START -- DO NOT ALTER
/**
 * Definition of html_java_UnLoadAll.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(html_java_UnLoadAll)
{
    inx_html_java_state_type* inx_html_java_state = (inx_html_java_state_type*)EHS_FB_RUN_CONTEXT;
    EhsHWebkitUnloadAll();
    inx_html_java_state->visible=EHS_FALSE;
    inx_html_java_state->created=EHS_FALSE;

    // Your code here
    EHS_FB_FINISH(INX_html_java_ARG_UnLoadAll_Unloaded);
}//ICB FUNCTION UnLoadAll MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION setVisibile MACRO START -- DO NOT ALTER
/**
 * Definition of html_java_setVisibile.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(html_java_setVisibile)
{
    inx_html_java_state_type* inx_html_java_state = (inx_html_java_state_type*)EHS_FB_RUN_CONTEXT;

    EhsHWebkitShow(EHS_TRUE) ;
    inx_html_java_state->visible=EHS_TRUE;
    // Your code here
    EHS_FB_FINISH(INX_html_java_ARG_setVisibile_visSet);
}//ICB FUNCTION setVisibile MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION setInvisible MACRO START -- DO NOT ALTER
/**
 * Definition of html_java_setInvisible.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(html_java_setInvisible)
{
    inx_html_java_state_type* inx_html_java_state = (inx_html_java_state_type*)EHS_FB_RUN_CONTEXT;

    // Your code here
    EhsHWebkitShow(EHS_FALSE) ;
    inx_html_java_state->visible=EHS_FALSE;
    EHS_FB_FINISH(INX_html_java_ARG_setInvisible_set);
}//ICB FUNCTION setInvisible MACRO END -- DO NOT ALTER THIS LINE
