//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-frame_fmt.h"
//ICB HEADER MACRO END -- DO NOT ALTER
#include "hal_mv.h"
//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_FrameFormatter_state
{
	ehs_sint32 format;
	EhsCameraFrame frame;
} inx_FrameFormatter_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
typedef enum {
	FRAME_FMT_OK = 0,
	FRAME_FMT_FAILED,
	FRAME_FMT_UNUSED
} FrameFormatterErr;
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(FrameFormatter)
EHS_FB_FUNCTION_ENTRY("format", 0x01, FrameFormatter_format)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_FrameFormatter_ARG_format_frame_id 1
#define INX_FrameFormatter_ARG_format_data_format 2
#define INX_FrameFormatter_ARG_format_start_errno 1
#define INX_FrameFormatter_ARG_format_formated_frame_id 2
#define INX_FrameFormatter_ARG_format_start_ok 1
#define INX_FrameFormatter_ARG_format_start_error 2
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER
//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
/* Parameters */
/* Create some macros for the default parameters */
#define INX_FB_FrameFormatter_format 0
//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(FrameFormatter)
{
/* Uncomment the following if you need to parse the parameters to calculate memory required */
/*
	ehs_sint32 format;
	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,"%d",&format); */
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_FrameFormatter_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(FrameFormatter)
{
	ehs_bool bRet = EHS_TRUE; /* assume success */
	//this is the reference to the object data for this instance of the function block
	inx_FrameFormatter_state_type* inx_FrameFormatter_state = (inx_FrameFormatter_state_type*)EHS_FB_INIT_CONTEXT;
	
	EhsMemset(&inx_FrameFormatter_state->frame, 0, sizeof(EhsCameraFrame));
	/* read the initialisation parameters */
	const char* pParams = EHS_FB_INIT_PARAMETERS;
	if (pParams) {
		pParams = EhsGetSint32FromString(&inx_FrameFormatter_state->format, pParams);
	}

	// create frame instace for this camera and add it to the global list
	EhsCameraFrameCreate(&inx_FrameFormatter_state->frame);
	EhsCameraFrameAdd(&inx_FrameFormatter_state->frame); // add frame to global buffer 

	/* Add any further intialisation code here */
	return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(FrameFormatter)
{
	inx_FrameFormatter_state_type *inx_FrameFormatter_state = (inx_FrameFormatter_state_type*)EHS_FB_DESTROY_CONTEXT;
	EhsCameraFrameRemove(&inx_FrameFormatter_state->frame); // remove frame from global buffer
	EhsCameraFrameDestroy(&inx_FrameFormatter_state->frame);
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION format MACRO START -- DO NOT ALTER
/**
 * Definition of FrameFormatter_format.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(FrameFormatter_format)
{
	inx_FrameFormatter_state_type* inx_FrameFormatter_state = (inx_FrameFormatter_state_type*)EHS_FB_RUN_CONTEXT;

	EhsCameraDataFormat fmt = inx_FrameFormatter_state->format;
	
	ehs_sint32 frame_id = -1;
	FrameFormatterErr err = FRAME_FMT_OK;

	if(inx_FrameFormatter_state->frame.id < 0) {err = FRAME_FMT_FAILED; goto done; }

	if (EHS_FB_IN_CONNECTED_API2(INX_FrameFormatter_ARG_format_frame_id)){
		frame_id = EHS_FB_IN_I_API2(INX_FrameFormatter_ARG_format_frame_id) ;
	}
	if(frame_id < 0) {err = FRAME_FMT_FAILED; goto done; }
	EhsCameraFrame* src_frame = EhsCameraFrameGetById(frame_id);
	if(!src_frame) {err = FRAME_FMT_FAILED; goto done; }

	if (EHS_FB_IN_CONNECTED_API2(INX_FrameFormatter_ARG_format_data_format)){
		fmt = EHS_FB_IN_I_API2(INX_FrameFormatter_ARG_format_data_format) ;
	}

	if(fmt != EHS_CAM_FMT_DEF){
		if(EHS_FALSE == EhsCameraFrameFormat(src_frame, &inx_FrameFormatter_state->frame, fmt)) { err = FRAME_FMT_FAILED; goto done; }
	}else{
		err = FRAME_FMT_UNUSED; goto done;
	}

	if (EHS_FB_OUT_CONNECTED_API2(INX_FrameFormatter_ARG_format_formated_frame_id)){
		EHS_FB_OUT_I_API2(INX_FrameFormatter_ARG_format_formated_frame_id) = inx_FrameFormatter_state->frame.id;
	}
done:
	if (EHS_FB_OUT_CONNECTED_API2(INX_FrameFormatter_ARG_format_start_errno)){
		EHS_FB_OUT_I_API2(INX_FrameFormatter_ARG_format_start_errno) = err;
	}
	if(err == FRAME_FMT_OK){
		EHS_FB_FINISH(INX_FrameFormatter_ARG_format_start_ok);
	}else{
		EHS_FB_FINISH(INX_FrameFormatter_ARG_format_start_error);
	}

}//ICB FUNCTION format MACRO END -- DO NOT ALTER THIS LINE
