//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-frame_crop.h"
//ICB HEADER MACRO END -- DO NOT ALTER
#include "hal_mv.h"
//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_FrameCrop_state
{
	ehs_sint32 crop_x1;
	ehs_sint32 crop_y1;
	ehs_sint32 crop_x2;
	ehs_sint32 crop_y2;

	EhsCameraFrame frame;

} inx_FrameCrop_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER

typedef enum {
	FRAME_CROP_OK = 0,
	FRAME_CROP_FAILED,
	FRAME_CROP_UNUSED
} EhsFrameCropErr;

//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(FrameCrop)
EHS_FB_FUNCTION_ENTRY("run_cropping", 0x01, FrameCrop_run_cropping)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_FrameCrop_ARG_run_cropping_frame_id 1
#define INX_FrameCrop_ARG_run_cropping_crop_x1 2
#define INX_FrameCrop_ARG_run_cropping_crop_y1 3
#define INX_FrameCrop_ARG_run_cropping_crop_x2 4
#define INX_FrameCrop_ARG_run_cropping_crop_y2 5
#define INX_FrameCrop_ARG_run_cropping_start_errno 1
#define INX_FrameCrop_ARG_run_cropping_formated_frame_id 2
#define INX_FrameCrop_ARG_run_cropping_start_ok 1
#define INX_FrameCrop_ARG_run_cropping_start_error 2
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER
//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
/* Parameters */
/* Create some macros for the default parameters */
#define INX_FB_FrameCrop_crop_x1 0
#define INX_FB_FrameCrop_crop_y1 0
#define INX_FB_FrameCrop_crop_x2 0
#define INX_FB_FrameCrop_crop_y2 0
//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(FrameCrop)
{
/* Uncomment the following if you need to parse the parameters to calculate memory required */
/*
	ehs_sint32 crop_x1;
	ehs_sint32 crop_y1;
	ehs_sint32 crop_x2;
	ehs_sint32 crop_y2;
	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,"%d %d %d %d",&crop_x1,&crop_y1,&crop_x2,&crop_y2); */
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_FrameCrop_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(FrameCrop)
{
	ehs_bool bRet = EHS_TRUE; /* assume success */
	//this is the reference to the object data for this instance of the function block
	inx_FrameCrop_state_type* inx_FrameCrop_state = (inx_FrameCrop_state_type*)EHS_FB_INIT_CONTEXT;

	EhsMemset(&inx_FrameCrop_state->frame, 0, sizeof(EhsCameraFrame));
	/* read the initialisation parameters */
	const char* pParams = EHS_FB_INIT_PARAMETERS;
	if (pParams) {
		pParams = EhsGetSint32FromString(&inx_FrameCrop_state->crop_x1, pParams);
		pParams = EhsGetSint32FromString(&inx_FrameCrop_state->crop_y1, pParams);
		pParams = EhsGetSint32FromString(&inx_FrameCrop_state->crop_x2, pParams);
		pParams = EhsGetSint32FromString(&inx_FrameCrop_state->crop_y2, pParams);
	}

	// create frame instace for this camera and add it to the global list
	EhsCameraFrameCreate(&inx_FrameCrop_state->frame);
	EhsCameraFrameAdd(&inx_FrameCrop_state->frame); // add frame to global buffer 

	/* Add any further intialisation code here */
	return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(FrameCrop)
{
	inx_FrameCrop_state_type *inx_FrameCrop_state = (inx_FrameCrop_state_type*)EHS_FB_DESTROY_CONTEXT;
	EhsCameraFrameRemove(&inx_FrameCrop_state->frame); // remove frame from global buffer
	EhsCameraFrameDestroy(&inx_FrameCrop_state->frame);
	return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION run_cropping MACRO START -- DO NOT ALTER
/**
 * Definition of FrameCrop_run_cropping.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(FrameCrop_run_cropping)
{
	inx_FrameCrop_state_type* inx_FrameCrop_state = (inx_FrameCrop_state_type*)EHS_FB_RUN_CONTEXT;

	ehs_uint32 crop_x1 = inx_FrameCrop_state->crop_x1, crop_y1 = inx_FrameCrop_state->crop_y1, 
	           crop_x2 = inx_FrameCrop_state->crop_x2, crop_y2 = inx_FrameCrop_state->crop_y2;

	ehs_sint32 frame_id = -1;
	EhsFrameCropErr err = FRAME_CROP_OK;

	if(inx_FrameCrop_state->frame.id < 0) {err = FRAME_CROP_FAILED; goto done; }
	
	if (EHS_FB_IN_CONNECTED_API2(INX_FrameCrop_ARG_run_cropping_frame_id)){
		frame_id = EHS_FB_IN_I_API2(INX_FrameCrop_ARG_run_cropping_frame_id);
	}
	if(frame_id < 0) { err = FRAME_CROP_FAILED; goto done; }
	EhsCameraFrame* src_frame = EhsCameraFrameGetById(frame_id);
	if(!src_frame) { err = FRAME_CROP_FAILED; goto done; }

	if (EHS_FB_IN_CONNECTED_API2(INX_FrameCrop_ARG_run_cropping_crop_x1)){
		crop_x1 = EHS_FB_IN_I_API2(INX_FrameCrop_ARG_run_cropping_crop_x1);
	}
	if (EHS_FB_IN_CONNECTED_API2(INX_FrameCrop_ARG_run_cropping_crop_y1)){
		crop_y1 = EHS_FB_IN_I_API2(INX_FrameCrop_ARG_run_cropping_crop_y1);
	}
	if (EHS_FB_IN_CONNECTED_API2(INX_FrameCrop_ARG_run_cropping_crop_x2)){
		crop_x2 = EHS_FB_IN_I_API2(INX_FrameCrop_ARG_run_cropping_crop_x2);
	}
	if (EHS_FB_IN_CONNECTED_API2(INX_FrameCrop_ARG_run_cropping_crop_y2)){
		crop_y2 = EHS_FB_IN_I_API2(INX_FrameCrop_ARG_run_cropping_crop_y2);
	}
	
	// crop if any cropping was requested (crop_* > 0)
	if(crop_x1 > 0 || crop_y1 > 0 || crop_x2 > 0 || crop_y2 > 0){
		if(EHS_FALSE == EhsCameraFrameCrop(src_frame, &inx_FrameCrop_state->frame, crop_x1, crop_y1, crop_x2, crop_y2)) { err = FRAME_CROP_FAILED; goto done; }
	}else{
		err = FRAME_CROP_UNUSED; goto done;
	}
	
	if (EHS_FB_OUT_CONNECTED_API2(INX_FrameCrop_ARG_run_cropping_formated_frame_id)){
		EHS_FB_OUT_I_API2(INX_FrameCrop_ARG_run_cropping_formated_frame_id) = inx_FrameCrop_state->frame.id;
	}
done:
	if (EHS_FB_OUT_CONNECTED_API2(INX_FrameCrop_ARG_run_cropping_start_errno)){
		EHS_FB_OUT_I_API2(INX_FrameCrop_ARG_run_cropping_start_errno) = err;
	}
	if(err == FRAME_CROP_OK){
		EHS_FB_FINISH(INX_FrameCrop_ARG_run_cropping_start_ok);
	}else{
		EHS_FB_FINISH(INX_FrameCrop_ARG_run_cropping_start_error);
	}

}//ICB FUNCTION run_cropping MACRO END -- DO NOT ALTER THIS LINE
