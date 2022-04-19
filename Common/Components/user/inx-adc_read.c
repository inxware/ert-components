//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-adc_read.h"
#include "ehs_main.h" // we run th main from here!
//ICB HEADER MACRO END -- DO NOT ALTER

//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_adc_read_state
{
	ehs_uint8 channel;
	ehs_float value;
	EhsFunctionInstanceDataType* pFIdata;
	struct inx_adc_read_state* pNext;
	struct inx_adc_read_state* pPrev;
} inx_adc_read_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
static inx_adc_read_state_type* gpFirstWidget=NULL;

static inx_adc_read_state_type* inxADCReadGetLastWidget(){
	inx_adc_read_state_type* widget=gpFirstWidget;
	while(widget!=NULL && widget->pNext!=NULL){
		widget=widget->pNext;
		if(widget==widget->pNext){
			EHSH_LOG_ERROR("inxMQTTSubscribeGetLastWidget infinite loop found");
			widget->pNext=NULL;
		}
	}
	return widget;
}

static void inxADCReadRegisterWidget(inx_adc_read_state_type* pState){
	if(gpFirstWidget==NULL){
		gpFirstWidget=pState;
		return;
	}

	inx_adc_read_state_type* lastWidget=inxADCReadGetLastWidget();
	if(lastWidget==NULL){
		gpFirstWidget=pState;
	}else{
		lastWidget->pNext=pState;
		pState->pPrev=lastWidget;
	}
}

//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(adc_read)
EHS_FB_FUNCTION_ENTRY("read", adc_read_read)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_adc_read_ARG_read_finishread 1
#define INX_adc_read_ARG_read_channel 1
#define INX_adc_read_ARG_read_value 1
#define INX_adc_read_ARG_read_finishread 1
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
EHS_FB_IDENTIFY_FUNCTION(adc_read)
{
/* Uncomment the following if you need to parse the parameters to calculate memory required */
/*
	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,""); */
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_adc_read_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(adc_read)
{
	ehs_bool bRet = EHS_TRUE; /* assume success */
	
	//this is the reference to the object data for this instance of the function block
	inx_adc_read_state_type* inx_adc_read_state = (inx_adc_read_state_type*)EHS_FB_INIT_CONTEXT;
	/* read the initialisation parameters */
	EhsSscanf(EHS_FB_INIT_PARAMETERS,"");
	inx_adc_read_state->pFIdata=NULL;
	inx_adc_read_state->channel=0;
	inx_adc_read_state->value=0;
	inx_adc_read_state->pNext=NULL;
	inx_adc_read_state->pPrev=NULL;

	/* Add any further intialisation code here */
	EhsTPMutex_lock(EhsTPMutex_fbIO);
	inxADCReadRegisterWidget(inx_adc_read_state);
	EhsTPMutex_unlock(EhsTPMutex_fbIO);
	return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(adc_read)
{
	/*
	inx_adc_read_state_type *inx_adc_read_state = (inx_adc_read_state_type*)EHS_FB_DESTROY_CONTEXT;
	*/
	//Your code below here
	gpFirstWidget=NULL;
	return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE

//ICB FUNCTION read MACRO START -- DO NOT ALTER
/**
 * Definition of adc_read_read.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(adc_read_read)
{
	inx_adc_read_state_type* inx_adc_read_state = (inx_adc_read_state_type*)EHS_FB_RUN_CONTEXT;

	// Your code here
	inx_adc_read_state->pFIdata = EHS_FB_RUN_CONTEXT_REF;
	/*
	if (EHS_FB_IN_CONNECTED_API2(INX_adc_read_ARG_read_channel))
		EHS_FB_IN_I_API2(INX_adc_read_ARG_read_channel) ;
	if (EHS_FB_OUT_CONNECTED_API2(INX_adc_read_ARG_read_value))
		EHS_FB_OUT_F_API2(INX_adc_read_ARG_read_value) ;
	EHS_FB_FINISH(INX_adc_read_ARG_read_finishread);
	*/
}//ICB FUNCTION read MACRO END -- DO NOT ALTER THIS LINE

#ifdef EHS_MINGW
#define EHS_ADC_READ_EXPORT __declspec(dllexport)
#else
#define EHS_ADC_READ_EXPORT // nothing
#endif

static void inxADCReadSetValues(const uint8_t channel,const ehs_float value){
	inx_adc_read_state_type* widget=gpFirstWidget;
	while(widget!=NULL){
		//create pFIData variable so we can use the APIs
		EhsFunctionInstanceDataType* pFIdata=widget->pFIdata;
		if(pFIdata==NULL){

		}else{
			if(EHS_FB_IN_CONNECTED_API2(INX_adc_read_ARG_read_channel)){
				widget->channel=EHS_FB_IN_I_API2(INX_adc_read_ARG_read_channel);
			}
			if(widget->channel==channel){
				widget->value=value;
				//printf("PBB adc c=%d v=%f\n",widget->channel,widget->value);
				EHS_FB_OUT_F_API2(INX_adc_read_ARG_read_value)=widget->value;
				EHS_FB_FINISH(INX_adc_read_ARG_read_finishread);
			}
		}
		widget=widget->pNext;
		if(widget==widget->pNext){
			EHSH_LOG_ERROR("inxADCReadSetValues infinite loop found");
			widget->pNext=NULL;
		}
	}
}

EHS_ADC_READ_EXPORT ehs_bool EhsADCReadEvent(const uint8_t channel,const ehs_float value){
	EHSH_LOG_INFO("EhsADCReadEvent");
	inxADCReadSetValues(channel,value);
	return EHS_TRUE;
}
