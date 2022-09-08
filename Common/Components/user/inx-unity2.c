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
#include "inx-unity2.h"
#include "ehs_main.h" // we run th main from here!
//ICB HEADER MACRO END -- DO NOT ALTER

#define INX_UNITY2_FIFO_SIZE 10
typedef struct			/* state data for string FIFO buffer...*/
{
    int nPushIndex;   	/* index of next push point*/
    int nPopIndex;    	/* index of next pop point */
    int nCurrentUsage;	/* current usage / size    */
    char* pContents[INX_UNITY2_FIFO_SIZE];   /* pointer to contents     */
} structFIFO_String;
static structFIFO_String* gpFifo=NULL;

static ehs_bool inxUnityFifoInit()
{
    //set up our global fifo buffer
    if(gpFifo==NULL)
    {
        //allocate mem for our struct
        gpFifo=EhsHMem_writeableAlloc(sizeof(structFIFO_String));
        if(gpFifo==NULL)
        {
            EHSH_LOG_ERROR("unity could not allocate global fifo");
            return EHS_FALSE;
        }
        //init the struct
        gpFifo->nPushIndex=0;
        gpFifo->nPopIndex=0;
        gpFifo->nCurrentUsage=0;
        for (ehs_uint8 i=0; i<INX_UNITY2_FIFO_SIZE; i++)
        {
            gpFifo->pContents[i] = (char*)EhsHMem_writeableAlloc(EHS_STRING_LENGTH_MAX);
            if( !(gpFifo->pContents[i]) )
            {
                return EHS_FALSE;
            }
        }
    }
    return EHS_TRUE;
}

static char* inxUnityFifoPush()
{
    structFIFO_String* pFIFO_String = gpFifo;
    if(pFIFO_String==NULL)
    {
        return EHS_FALSE;
    }
    char* pString = NULL;
    int nPushIndex;
    int nCurrentUsage;
    int nSize;
    char** pContents = NULL;

    /* copy the required parts to local storage...*/
    nPushIndex = pFIFO_String->nPushIndex;
    nCurrentUsage = pFIFO_String->nCurrentUsage;
    nSize = INX_UNITY2_FIFO_SIZE;
    pContents = (char**)pFIFO_String->pContents;

    /* check we haven't walked over the top of the buffer...*/
    if (nCurrentUsage < nSize)
    {
        /* push input on to the buffer...*/
        pString=pContents[nPushIndex];
        /* increment the push and current indicies...*/
        nPushIndex++;
        if (nPushIndex == nSize)
        {
            nPushIndex = 0;
        }
        nCurrentUsage++;
    }
    else
    {
        return NULL;
    }
    /* copy state data back over...*/
    pFIFO_String->nCurrentUsage = nCurrentUsage;
    pFIFO_String->nPushIndex = nPushIndex;
    return pString;
}

static ehs_bool inxUnityFifoPop(void** buffer)
{
    structFIFO_String* pFIFO_String = gpFifo;
    if(pFIFO_String==NULL)
    {
        return EHS_FALSE;
    }

    int nPopIndex;
    int nCurrentUsage;
    int nSize;
    char** pContents = NULL;

    nPopIndex = pFIFO_String->nPopIndex;
    nCurrentUsage = pFIFO_String->nCurrentUsage;
    nSize = INX_UNITY2_FIFO_SIZE;
    pContents = (char**)pFIFO_String->pContents;

    if (nCurrentUsage == 0)
    {
        return EHS_FALSE;
    }
    else
    {
        //point the unity buffer at this string
        *buffer = pContents[nPopIndex];
        /* increment pop index for next pop...*/
        nPopIndex++;
        if (nPopIndex == nSize)
        {
            /* if we've gone over the edge - restart at the beginning...*/
            nPopIndex = 0;
        }
        /* decrement usage indicator...*/
        nCurrentUsage--;
        /* copy the data back into the object state memory...*/
        pFIFO_String->nPopIndex = nPopIndex;
        pFIFO_String->nCurrentUsage = nCurrentUsage;
    }
    return EHS_TRUE;
}

//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_unity2_state
{
    ehs_char id[EHS_STRING_LENGTH_MAX];
    EhsFunctionInstanceDataType* pFIdata;
    struct inx_unity2_state* pNext;
    struct inx_unity2_state* pPrev;
} inx_unity2_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
static inx_unity2_state_type* gpFirstWidget=NULL;

static inx_unity2_state_type* inxUnityGetLastWidget()
{
    inx_unity2_state_type* widget=gpFirstWidget;
    while(widget!=NULL && widget->pNext!=NULL)
    {
        widget=widget->pNext;
        if(widget==widget->pNext)
        {
            EHSH_LOG_ERROR("inxUnityGetLastWidget infinite loop found");
            widget->pNext=NULL;
        }
    }
    return widget;
}

static inx_unity2_state_type* inxUnityGetWidgetById(const char* id)
{
    inx_unity2_state_type* widget=gpFirstWidget;
    while(widget!=NULL)
    {
        if(EhsStrcmp(widget->id,id)==0)
        {
            break;
        }
        widget=widget->pNext;
        if(widget==widget->pNext)
        {
            EHSH_LOG_ERROR("inxUnityGetWidgetById infinite loop found");
            widget->pNext=NULL;
        }
    }
    return widget;
}

static void inxUnityRegisterWidget(inx_unity2_state_type* pState)
{
    if(gpFirstWidget==NULL)
    {
        gpFirstWidget=pState;
        return;
    }

    inx_unity2_state_type* lastWidget=inxUnityGetLastWidget();
    if(lastWidget==NULL)
    {
        gpFirstWidget=pState;
    }
    else
    {
        lastWidget->pNext=pState;
        pState->pPrev=lastWidget;
    }
}

static void inxUnityRemoveWidget(inx_unity2_state_type* pState)
{
    if(pState->pPrev!=NULL)
    {
        pState->pPrev->pNext=pState->pNext;
    }
    if(pState->pNext!=NULL)
    {
        pState->pNext->pPrev=pState->pPrev;
    }
    //do we need to update the first entry pointer?
    if(gpFirstWidget==pState)
    {
        gpFirstWidget=pState->pNext;
    }
}


//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(unity2)

EHS_FB_FUNCTION_ENTRY("create", 0x00, unity2_create)

EHS_FB_FUNCTION_ENTRY("destroy", 0x01, unity2_destroy)

EHS_FB_FUNCTION_ENTRY("setmedia", 0x02, unity2_setmedia)

EHS_FB_FUNCTION_ENTRY("stop", 0x03, unity2_stop)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_unity2_ARG_create_finishcreate 1
#define INX_unity2_ARG_create_finishevent 2
#define INX_unity2_ARG_create_id 1
#define INX_unity2_ARG_create_type 2
#define INX_unity2_ARG_create_params 3
#define INX_unity2_ARG_create_event 1
#define INX_unity2_ARG_destroy_finishdestroy 1
#define INX_unity2_ARG_setmedia_finishsetmedia 1
#define INX_unity2_ARG_setmedia_mediatype 1
#define INX_unity2_ARG_setmedia_path 2
#define INX_unity2_ARG_setmedia_text 3
#define INX_unity2_ARG_setmedia_params 4
#define INX_unity2_ARG_stop_finishstop 1
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
EHS_FB_IDENTIFY_FUNCTION(unity2)
{
    /* Uncomment the following if you need to parse the parameters to calculate memory required */
    /*
    	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,""); */
    EHS_FB_IDENTIFY_MEMORY = sizeof(inx_unity2_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(unity2)
{
    ehs_bool bRet = EHS_TRUE; /* assume success */

    bRet = inxUnityFifoInit();
    if(bRet==EHS_FALSE)
    {
        return bRet;
    }

    //this is the reference to the object data for this instance of the function block
    inx_unity2_state_type* inx_unity2_state = (inx_unity2_state_type*)EHS_FB_INIT_CONTEXT;
    /* read the initialisation parameters */
    EhsSscanf(EHS_FB_INIT_PARAMETERS,"");
    inx_unity2_state->id[0]='\0';
    inx_unity2_state->pNext=NULL;
    inx_unity2_state->pPrev=NULL;
    /* Add any further intialisation code here */
    EhsTPMutex_lock(EhsTPMutex_fbIO);
    inxUnityRegisterWidget(inx_unity2_state);
    EhsTPMutex_unlock(EhsTPMutex_fbIO);
    return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(unity2)
{
    inx_unity2_state_type *inx_unity2_state = (inx_unity2_state_type*)EHS_FB_DESTROY_CONTEXT;
    //Your code below here
    gpFifo=NULL;
    gpFirstWidget=NULL;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE

//ICB FUNCTION create MACRO START -- DO NOT ALTER
/**
 * Definition of unity2_create.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(unity2_create)
{
    inx_unity2_state_type* inx_unity2_state = (inx_unity2_state_type*)EHS_FB_RUN_CONTEXT;

    // Your code here
    //create a pointer to our run data so that we can process events from unity later
    inx_unity2_state->pFIdata = EHS_FB_RUN_CONTEXT_REF;
    EhsTPMutex_lock(EhsTPMutex_fbIO);

    if (EHS_FB_IN_CONNECTED_API2(INX_unity2_ARG_create_id))
    {
        EhsStrcpy(inx_unity2_state->id,EHS_FB_IN_S_API2(INX_unity2_ARG_create_id));
    }

    char type[EHS_STRING_LENGTH_MAX];
    type[0]='\0';
    if (EHS_FB_IN_CONNECTED_API2(INX_unity2_ARG_create_type))
    {
        EhsStrcpy(type,EHS_FB_IN_S_API2(INX_unity2_ARG_create_type));
    }

    char params[EHS_STRING_LENGTH_MAX];
    params[0]='\0';
    if (EHS_FB_IN_CONNECTED_API2(INX_unity2_ARG_create_params))
    {
        EhsStrcpy(params,EHS_FB_IN_S_API2(INX_unity2_ARG_create_params));
    }
    //enforce valid json
    if(EhsStrlen(params)<2)
    {
        EhsStrcpy(params,"{}");
    }

    char* slot=inxUnityFifoPush();
    if(slot != NULL)
    {
        EhsSprintf(slot,"{\"id\":\"%s\",\"cmd\":\"create\",\"type\":\"%s\",\"params\":%s}",
                   inx_unity2_state->id,
                   type,
                   params);
    }
    else
    {
        EHSH_LOG_ERROR("unity buffer full");
    }
    EhsTPMutex_unlock(EhsTPMutex_fbIO);
    EHS_FB_FINISH(INX_unity2_ARG_create_finishcreate);
}//ICB FUNCTION create MACRO END -- DO NOT ALTER THIS LINE

//ICB FUNCTION destroy MACRO START -- DO NOT ALTER
/**
 * Definition of unity2_destroy.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(unity2_destroy)
{
    inx_unity2_state_type* inx_unity2_state = (inx_unity2_state_type*)EHS_FB_RUN_CONTEXT;

    // Your code here
    inxUnityRemoveWidget(inx_unity2_state);
    EhsTPMutex_lock(EhsTPMutex_fbIO);
    char* slot=inxUnityFifoPush();
    if(slot != NULL)
    {
        EhsSprintf(slot,"{\"id\":\"%s\",\"cmd\":\"destroy\"}",
                   inx_unity2_state->id);
    }
    else
    {
        EHSH_LOG_ERROR("unity buffer full");
    }
    EhsTPMutex_unlock(EhsTPMutex_fbIO);

    EHS_FB_FINISH(INX_unity2_ARG_destroy_finishdestroy);
}//ICB FUNCTION destroy MACRO END -- DO NOT ALTER THIS LINE

//ICB FUNCTION setmedia MACRO START -- DO NOT ALTER
/**
 * Definition of unity2_setmedia.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(unity2_setmedia)
{
    inx_unity2_state_type* inx_unity2_state = (inx_unity2_state_type*)EHS_FB_RUN_CONTEXT;

    // Your code here
    EhsTPMutex_lock(EhsTPMutex_fbIO);

    char mediatype[EHS_STRING_LENGTH_MAX];
    mediatype[0]='\0';
    if (EHS_FB_IN_CONNECTED_API2(INX_unity2_ARG_setmedia_mediatype))
    {
        EhsStrcpy(mediatype,EHS_FB_IN_S_API2(INX_unity2_ARG_setmedia_mediatype));
    }

    char path[EHS_STRING_LENGTH_MAX];
    path[0]='\0';
    if (EHS_FB_IN_CONNECTED_API2(INX_unity2_ARG_setmedia_path))
    {
        EhsStrcat(path,EhsHMetaGetUserPath());
        EhsStrcat(path,EHS_TD_FILES_SEPARATOR_STR);
        EhsStrcat(path,EHS_FB_IN_S_API2(INX_unity2_ARG_setmedia_path));
    }

    char text[EHS_STRING_LENGTH_MAX];
    text[0]='\0';
    if (EHS_FB_IN_CONNECTED_API2(INX_unity2_ARG_setmedia_text))
    {
        EhsStrcpy(text,EHS_FB_IN_S_API2(INX_unity2_ARG_setmedia_text));
    }

    char params[EHS_STRING_LENGTH_MAX];
    params[0]='\0';
    if (EHS_FB_IN_CONNECTED_API2(INX_unity2_ARG_setmedia_params))
    {
        EhsStrcpy(params,EHS_FB_IN_S_API2(INX_unity2_ARG_setmedia_params));
    }
    //enforce valid json
    if(EhsStrlen(params)<2)
    {
        EhsStrcpy(params,"{}");
    }
    char* slot=inxUnityFifoPush();
    if(slot != NULL)
    {
        EhsSprintf(slot,"{\"id\":\"%s\",\"cmd\":\"setmedia\",\"type\":\"%s\",\"path\":\"%s\",\"text\":\"%s\",\"parameters\":%s}",
                   inx_unity2_state->id,
                   mediatype,
                   path,
                   text,
                   params);
    }
    else
    {
        EHSH_LOG_ERROR("unity buffer full");
    }
    EhsTPMutex_unlock(EhsTPMutex_fbIO);
    EHS_FB_FINISH(INX_unity2_ARG_setmedia_finishsetmedia);
}//ICB FUNCTION setmedia MACRO END -- DO NOT ALTER THIS LINE

//ICB FUNCTION stop MACRO START -- DO NOT ALTER
/**
 * Definition of unity2_stop.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(unity2_stop)
{
    inx_unity2_state_type* inx_unity2_state = (inx_unity2_state_type*)EHS_FB_RUN_CONTEXT;

    // Your code here
    EhsTPMutex_lock(EhsTPMutex_fbIO);
    char* slot=inxUnityFifoPush();
    if(slot != NULL)
    {
        EhsSprintf(slot,"{\"id\":\"%s\",\"cmd\":\"stop\"}",
                   inx_unity2_state->id);
    }
    else
    {
        EHSH_LOG_ERROR("unity buffer full");
    }
    EhsTPMutex_unlock(EhsTPMutex_fbIO);

    EHS_FB_FINISH(INX_unity2_ARG_stop_finishstop);
}//ICB FUNCTION destroy MACRO END -- DO NOT ALTER THIS LINE

#ifdef EHS_MINGW
#define EHS_UNITY_EXPORT __declspec(dllexport)
#else
#define EHS_UNITY_EXPORT // nothing
#endif

EhsThreadFuncReturnType EhsUnityStartBlocking()
{
#ifdef EHS_MINGW
    ehs_char* argv[] = { "ehs_data\\dummy\\dummy","" }; /* @todo this make the user app data in the root of the sdcard -might want something else here... */
#else
    ehs_char* argv[] = { "/sdcard/bin/ehs.exe","" }; /* @todo this make the user app data in the root of the sdcard -might want something else here... */
#endif
    ehs_char buf[EHS_MAXPATHLENGTH];
    getcwd(buf, EHS_MAXPATHLENGTH); // Note this must be implemented for mingw
    EhsHStoreArgInfo(1, argv, buf);  //@todo argv this is a bit clumsy - should extract the userdata path etc..
    //assume this is set by JNI on activity create
    //EhsHMetaSetHWID("PBB_Android_Unity");
    EhsMain(NULL, NULL);
    return EHS_TRUE;
}

// These are plug-in functions, which may be called before EHS is fully initalised.
// Watchout for racing conditions and null pointer access!

EHS_UNITY_EXPORT ehs_bool EhsUnityWritePoll(void **buffer, size_t *length)
{
    *length=0;
    if(!EhsTPMutex_fbIO) return EHS_FALSE;
    EhsTPMutex_lock(EhsTPMutex_fbIO);
    ehs_bool success=inxUnityFifoPop(buffer);
    if(success!=EHS_FALSE)
    {
        *length = strlen(*buffer);
        EHSH_LOG_INFO("EhsUnityWritePoll %d %s",*length,*buffer);
    }
    EhsTPMutex_unlock(EhsTPMutex_fbIO);
    return success;
}

EHS_UNITY_EXPORT ehs_bool EhsUnityEvent(char* id,char* event)
{
    EHSH_LOG_INFO("EhsUnityEvent");
    inx_unity2_state_type* pState=inxUnityGetWidgetById(id);
    if(pState==NULL)
    {
        return EHS_FALSE;
    }
    //create pFIData variable so we can use the APIs
    EhsFunctionInstanceDataType* pFIdata=pState->pFIdata;

    EhsStrcpy(EHS_FB_OUT_S_API2(INX_unity2_ARG_create_event),event);
    EHS_FB_FINISH(INX_unity2_ARG_create_finishevent);
}

EHS_UNITY_EXPORT void EhsUnityStartAsync()
{
#ifdef EHS_MINGW

    freopen("debug.txt", "a", stdout);
    freopen("debug_err.txt", "a", stderr);
#endif
    EhsHThread_execute(EhsUnityStartBlocking, NULL, 0);
}
