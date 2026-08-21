//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-ota.h"
#include "hal_ota.h"
//ICB HEADER MACRO END -- DO NOT ALTER
//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_OTA_state
{
	EhsCallbackQueueEntryType xEntry;
	ehs_bool ota_partition_alt;
	ehs_sint32 partition_number;
	ehs_char SHA256[65];
} inx_OTA_state_type; //Reference this, maybe store your config parameters in here too.
static EhsCallbackQueueType xOTACallbackQueue;
static ehs_uint8 write_errno = 0;
//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(OTA)
EHS_FB_FUNCTION_ENTRY("begin", 0x01, OTA_begin)
EHS_FB_FUNCTION_ENTRY("write", 0x02, OTA_write)
EHS_FB_FUNCTION_ENTRY("end", 0x03, OTA_end)
EHS_FB_FUNCTION_ENTRY("abort", 0x04, OTA_abort)
EHS_FB_FUNCTION_ENTRY("write_cb", 0x05, OTA_write_cb)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_OTA_ARG_begin_SHA256 1
#define INX_OTA_ARG_begin_begin_errno 1
#define INX_OTA_ARG_begin_begin_OK 1
#define INX_OTA_ARG_begin_begin_error 2
#define INX_OTA_ARG_write_data 1
#define INX_OTA_ARG_write_size 2
#define INX_OTA_ARG_write_offset 3
#define INX_OTA_ARG_write_write_OK 1
#define INX_OTA_ARG_write_write_pass_error 2
#define INX_OTA_ARG_end_ended 1
#define INX_OTA_ARG_end_end_error 2
#define INX_OTA_ARG_abort_aborted 1
#define INX_OTA_ARG_write_cb_write_errno 1
#define INX_OTA_ARG_write_cb_written 1
#define INX_OTA_ARG_write_cb_write_error 2
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER
//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
/* Parameters */
/* Create some macros for the default parameters */
#define INX_FB_OTA_ota_partition_alt 0
#define INX_FB_OTA_partition_number 0
//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(OTA)
{
/* Uncomment the following if you need to parse the parameters to calculate memory required */
/*
	ehs_bool ota_partition_alt;
	ehs_sint32 partition_number;
	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,"%d %d",&ota_partition_alt,&partition_number); */
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_OTA_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(OTA)
{
	ehs_bool bRet = EHS_TRUE; /* assume success */
	//this is the reference to the object data for this instance of the function block
	inx_OTA_state_type* inx_OTA_state = (inx_OTA_state_type*)EHS_FB_INIT_CONTEXT;
	EhsCallbackQueue_register(&xOTACallbackQueue,
			   EHS_FB_RUN_NAME(OTA_write_cb),
			   EHS_FB_INIT_CALLBACK_FUNCTION_INSTANCE(-1), //TODO
			   &(inx_OTA_state->xEntry)
		);
	/* read the initialisation parameters */
	EhsSscanf(EHS_FB_INIT_PARAMETERS,"%hhu %d",&(inx_OTA_state->ota_partition_alt),&(inx_OTA_state->partition_number));

	/* Add any further intialisation code here */
	return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(OTA)
{
	//inx_OTA_state_type *inx_OTA_state = (inx_OTA_state_type*)EHS_FB_DESTROY_CONTEXT;
	//Your code below here
	return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION begin MACRO START -- DO NOT ALTER
/**
 * Definition of OTA_begin.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(OTA_begin)
{
	inx_OTA_state_type* inx_OTA_state = (inx_OTA_state_type*)EHS_FB_RUN_CONTEXT;

	// Your code here
	if (EHS_FB_IN_CONNECTED_API2(INX_OTA_ARG_begin_SHA256))
		EhsStrcpy(inx_OTA_state->SHA256, EHS_FB_IN_S_API2(INX_OTA_ARG_begin_SHA256)) ;
	else goto err;
	ehs_sint32 errono = thOTA_begin(inx_OTA_state->ota_partition_alt, inx_OTA_state->partition_number);
	if (EHS_FB_OUT_CONNECTED_API2(INX_OTA_ARG_begin_begin_errno))
		EHS_FB_OUT_I_API2(INX_OTA_ARG_begin_begin_errno) = errono;
	if (errono == 0) EHS_FB_FINISH(INX_OTA_ARG_begin_begin_OK);
	else EHS_FB_FINISH(INX_OTA_ARG_begin_begin_error);
	goto function_end;
err:
	EHS_FB_FINISH(INX_OTA_ARG_begin_begin_error);
	EHS_FB_OUT_I_API2(INX_OTA_ARG_begin_begin_errno) = -1;
function_end:
	while (0); // NOP
}//ICB FUNCTION begin MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION write MACRO START -- DO NOT ALTER
/**
 * Definition of OTA_write.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(OTA_write)
{
	//inx_OTA_state_type* inx_OTA_state = (inx_OTA_state_type*)EHS_FB_RUN_CONTEXT;

	// Your code here
	if (!(EHS_FB_IN_CONNECTED_API2(INX_OTA_ARG_write_data) && EHS_FB_IN_CONNECTED_API2(INX_OTA_ARG_write_size) && EHS_FB_IN_CONNECTED_API2(INX_OTA_ARG_write_offset)))
		goto err;
	ehs_uint8 ret = thOTA_write_passthrough(EHS_FB_IN_S_API2(INX_OTA_ARG_write_data), EHS_FB_IN_I_API2(INX_OTA_ARG_write_size), EHS_FB_IN_I_API2(INX_OTA_ARG_write_offset));
	if (ret != 0) goto err;
	EHS_FB_FINISH(INX_OTA_ARG_write_write_OK);
	goto function_end;
err:
	EHS_FB_FINISH(INX_OTA_ARG_write_write_pass_error);
function_end:
	while (0); // NOP
}//ICB FUNCTION write MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION end MACRO START -- DO NOT ALTER
/**
 * Definition of OTA_end.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(OTA_end)
{
	inx_OTA_state_type* inx_OTA_state = (inx_OTA_state_type*)EHS_FB_RUN_CONTEXT;

	// Your code here
	thOTA_end();
	// @TODO - this needs a callback with waiting for 'TARGET_OTA_ENDED' state !!!
	ehs_bool validated;
	validated = thOTA_checkChecksum(inx_OTA_state->ota_partition_alt, inx_OTA_state->partition_number, inx_OTA_state->SHA256, EhsStrlen(inx_OTA_state->SHA256));
	if (validated == EHS_FALSE) goto err;
	validated = thOTA_checkValid(inx_OTA_state->ota_partition_alt, inx_OTA_state->partition_number);
	if (validated == EHS_FALSE) goto err;
	ehs_bool switched = thOTA_switch(inx_OTA_state->ota_partition_alt, inx_OTA_state->partition_number);
	if (switched == EHS_FALSE) goto err;
	EHS_FB_FINISH(INX_OTA_ARG_end_ended);
	goto function_end;
err:
	EHS_FB_FINISH(INX_OTA_ARG_end_end_error);
function_end:
	thOTA_idle();
}//ICB FUNCTION end MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION abort MACRO START -- DO NOT ALTER
/**
 * Definition of OTA_abort.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(OTA_abort)
{
	//inx_OTA_state_type* inx_OTA_state = (inx_OTA_state_type*)EHS_FB_RUN_CONTEXT;

	// Your code here
	thOTA_abort();
	EHS_FB_FINISH(INX_OTA_ARG_abort_aborted);
}//ICB FUNCTION abort MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION write_cb MACRO START -- DO NOT ALTER
/**
 * Definition of OTA_write_cb.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(OTA_write_cb)
{
	//inx_OTA_state_type* inx_OTA_state = (inx_OTA_state_type*)EHS_FB_RUN_CONTEXT;

	// Your code here
	if (EHS_FB_OUT_CONNECTED_API2(INX_OTA_ARG_write_cb_write_errno))
		EHS_FB_OUT_I_API2(INX_OTA_ARG_write_cb_write_errno) = write_errno;
	if (write_errno == 0) EHS_FB_FINISH(INX_OTA_ARG_write_cb_written);
	else EHS_FB_FINISH(INX_OTA_ARG_write_cb_write_error);
}//ICB FUNCTION write_cb MACRO END -- DO NOT ALTER THIS LINE

/* What is this for and why does the #if logix seem reversed to what you might expect? */
void Common_OTA_Write_ACK(ehs_uint8 errno)
{
	write_errno = errno;
#if !defined(EHS_DEVMAN_SUPPORT) || (EHS_DEVMAN_SUPPORT != EHS_DEVMAN_MQTT) // @TODO - we may want to exclude this in a better way
	EhsCallbackQueue_execute(&xOTACallbackQueue);
#endif
}