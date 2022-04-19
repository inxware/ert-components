//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-modbus_read.h"
#include "ehs_main.h" // we run th main from here!
//ICB HEADER MACRO END -- DO NOT ALTER

//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_modbus_read_state
{
	EhsFunctionInstanceDataType* pFIdata;
	struct inx_modbus_read_state* pNext;
	struct inx_modbus_read_state* pPrev;
} inx_modbus_read_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
static inx_modbus_read_state_type* gpFirstWidget=NULL;

static inx_modbus_read_state_type* inxModbusReadGetLastWidget(){
	inx_modbus_read_state_type* widget=gpFirstWidget;
	while(widget!=NULL && widget->pNext!=NULL){
		widget=widget->pNext;
		if(widget==widget->pNext){
			EHSH_LOG_ERROR("inxModbusReadGetLastWidget infinite loop found");
			widget->pNext=NULL;
		}
	}
	return widget;
}

static void inxModbusReadRegisterWidget(inx_modbus_read_state_type* pState){
	if(gpFirstWidget==NULL){
		gpFirstWidget=pState;
		return;
	}

	inx_modbus_read_state_type* lastWidget=inxModbusReadGetLastWidget();
	if(lastWidget==NULL){
		gpFirstWidget=pState;
	}else{
		lastWidget->pNext=pState;
		pState->pPrev=lastWidget;
	}
}

//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(modbus_read)
EHS_FB_FUNCTION_ENTRY("read", modbus_read_read)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_modbus_read_ARG_read_channel 1
#define INX_modbus_read_ARG_read_value 1
#define INX_modbus_read_ARG_read_finishread 1
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
EHS_FB_IDENTIFY_FUNCTION(modbus_read)
{
/* Uncomment the following if you need to parse the parameters to calculate memory required */
/*
	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,""); */
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_modbus_read_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(modbus_read)
{
	ehs_bool bRet = EHS_TRUE; /* assume success */
	
	//this is the reference to the object data for this instance of the function block
	inx_modbus_read_state_type* inx_modbus_read_state = (inx_modbus_read_state_type*)EHS_FB_INIT_CONTEXT;
	/* read the initialisation parameters */
	EhsSscanf(EHS_FB_INIT_PARAMETERS,"");
	inx_modbus_read_state->pFIdata=NULL;
	inx_modbus_read_state->pNext=NULL;
	inx_modbus_read_state->pPrev=NULL;

	/* Add any further intialisation code here */
	EhsTPMutex_lock(EhsTPMutex_fbIO);
	inxModbusReadRegisterWidget(inx_modbus_read_state);
	EhsTPMutex_unlock(EhsTPMutex_fbIO);
	return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(modbus_read)
{
	/*
	inx_modbus_read_state_type *inx_modbus_read_state = (inx_modbus_read_state_type*)EHS_FB_DESTROY_CONTEXT;
	*/
	//Your code below here
	gpFirstWidget=NULL;
	return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE

//ICB FUNCTION read MACRO START -- DO NOT ALTER
/**
 * Definition of modbus_read_read.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(modbus_read_read)
{
	inx_modbus_read_state_type* inx_modbus_read_state = (inx_modbus_read_state_type*)EHS_FB_RUN_CONTEXT;

	// Your code here
	inx_modbus_read_state->pFIdata = EHS_FB_RUN_CONTEXT_REF;
	/*
	if (EHS_FB_IN_CONNECTED_API2(INX_modbus_read_ARG_read_channel))
		EHS_FB_IN_I_API2(INX_modbus_read_ARG_read_channel) ;
	if (EHS_FB_OUT_CONNECTED_API2(INX_modbus_read_ARG_read_value))
		EHS_FB_OUT_I_API2(INX_modbus_read_ARG_read_value) ;
	EHS_FB_FINISH(INX_modbus_read_ARG_read_finishread);
	*/
}//ICB FUNCTION read MACRO END -- DO NOT ALTER THIS LINE

#ifdef EHS_MINGW
#define EHS_MODBUS_READ_EXPORT __declspec(dllexport)
#else
#define EHS_MODBUS_READ_EXPORT // nothing
#endif

/*
get rid of the BSS buffers that emodbus looks at
instead have it iterate our modbus blocks
if that gets a match on the channel then return it as the correct type
*/
static void inxModbusReadSetValues(const uint32_t channel,const EhsDataflowIntType value){
	inx_modbus_read_state_type* widget=gpFirstWidget;
	while(widget!=NULL){
		//create pFIData variable so we can use the APIs
		EhsFunctionInstanceDataType* pFIdata=widget->pFIdata;
		if(pFIdata==NULL){

		}else{
			if(EHS_FB_IN_CONNECTED_API2(INX_modbus_read_ARG_read_channel) && EHS_FB_IN_I_API2(INX_modbus_read_ARG_read_channel)==channel){
				EHS_FB_OUT_I_API2(INX_modbus_read_ARG_read_value)=value;
				EHS_FB_FINISH(INX_modbus_read_ARG_read_finishread);
			}
		}
		widget=widget->pNext;
		if(widget==widget->pNext){
			EHSH_LOG_ERROR("inxModbusReadSetValues infinite loop found");
			widget->pNext=NULL;
		}
	}
}

EHS_MODBUS_READ_EXPORT ehs_bool EhsModbusReadEvent(const uint32_t channel,const EhsDataflowIntType value){
	inxModbusReadSetValues(channel,value);
	return EHS_TRUE;
}
