//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-camera.h"
//ICB HEADER MACRO END -- DO NOT ALTER

#include "hal_mv.h"

//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_Camera_state
{
	ehs_char camera_id[EHS_STRING_LENGTH_MAX];
	EhsCamera camera;
	EhsCameraFrame frame;
	ehs_bool im_show;
} inx_Camera_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(Camera)
EHS_FB_FUNCTION_ENTRY("startCamera", 0x01, Camera_startCamera)
EHS_FB_FUNCTION_ENTRY("grabFrame", 0x03, Camera_grabFrame)
EHS_FB_FUNCTION_ENTRY("stopCamera", 0x02, Camera_stopCamera)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_Camera_ARG_startCamera_device_id 1
#define INX_Camera_ARG_startCamera_start_errno 1
#define INX_Camera_ARG_startCamera_start_ok 1
#define INX_Camera_ARG_startCamera_start_error 2
#define INX_Camera_ARG_grabFrame_im_show 1
#define INX_Camera_ARG_grabFrame_frame_width 1
#define INX_Camera_ARG_grabFrame_frame_height 2
#define INX_Camera_ARG_grabFrame_frame_id 3
#define INX_Camera_ARG_grabFrame_captur_ok 1
#define INX_Camera_ARG_stopCamera_stopped 1
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER
//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
/* Parameters */
/* Create some macros for the default parameters */
#define INX_FB_Camera_camera_id 
//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(Camera)
{
/* Uncomment the following if you need to parse the parameters to calculate memory required */
/*
	ehs_char* camera_id;
	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,"%s",&camera_id); */
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_Camera_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(Camera)
{
	ehs_bool bRet = EHS_TRUE; /* assume success */
	//this is the reference to the object data for this instance of the function block
	inx_Camera_state_type* inx_Camera_state = (inx_Camera_state_type*)EHS_FB_INIT_CONTEXT;
	EhsMemset(inx_Camera_state->camera_id, '\0', EHS_STRING_LENGTH_MAX);
	EhsMemset(&inx_Camera_state->camera, 0, sizeof(EhsCamera));
	EhsMemset(&inx_Camera_state->frame, 0, sizeof(EhsCameraFrame));
	/* read the initialisation parameters */
	const char* pParams = EHS_FB_INIT_PARAMETERS;
		if (pParams) {
		pParams = EhsGetWordFromString(inx_Camera_state->camera_id, pParams);
		pParams = EhsGetUint16FromString(&inx_Camera_state->camera.fps, pParams);
		pParams = EhsGetUint32FromString(&inx_Camera_state->camera.width, pParams);
		pParams = EhsGetUint32FromString(&inx_Camera_state->camera.height, pParams);
		ehs_uint8 isAsync = 0; // @TODO - implement this method (not doint anything atm)
		pParams = EhsGetUint8FromString(&isAsync, pParams);
		pParams = EhsGetUint8FromString(&(inx_Camera_state->im_show), pParams);
		pParams = EhsGetUint8FromString(&(inx_Camera_state->camera.greyscale), pParams);
		inx_Camera_state->camera.async = (isAsync) ? EHS_TRUE : EHS_FALSE;
		if(inx_Camera_state->camera_id && EhsStrcmp(inx_Camera_state->camera_id, "NULL") == 0)
		{
			EhsMemset(inx_Camera_state->camera_id, '\0', EHS_STRING_LENGTH_MAX);
		}

	}

	// create frame instace for this camera and add it to the global list
	EhsCameraFrameCreate(&inx_Camera_state->frame);
	EhsCameraFrameAdd(&inx_Camera_state->frame); // add frame to global buffer 

	/* Add any further intialisation code here */
	return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(Camera)
{
	inx_Camera_state_type *inx_Camera_state = (inx_Camera_state_type*)EHS_FB_DESTROY_CONTEXT;
	
	EhsCameraDestroy(&inx_Camera_state->camera);
	EhsCameraFrameRemove(&inx_Camera_state->frame); // remove frame from global buffer
	EhsCameraFrameDestroy(&inx_Camera_state->frame);
	return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION startCamera MACRO START -- DO NOT ALTER
/**
 * Definition of Camera_startCamera.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(Camera_startCamera)
{
	inx_Camera_state_type* inx_Camera_state = (inx_Camera_state_type*)EHS_FB_RUN_CONTEXT;

	if (EHS_FB_IN_CONNECTED_API2(INX_Camera_ARG_startCamera_device_id)){
		EhsStrcpy(inx_Camera_state->camera_id, EHS_FB_IN_S_API2(INX_Camera_ARG_startCamera_device_id));
	}

	EhsCameraError err = EhsCameraStart(&inx_Camera_state->camera, inx_Camera_state->camera_id);
	
	if (EHS_FB_OUT_CONNECTED_API2(INX_Camera_ARG_startCamera_start_errno)){
		EHS_FB_OUT_I_API2(INX_Camera_ARG_startCamera_start_errno) = err;
	}
	if(err == EHS_CAM_OK){
		EHS_FB_FINISH(INX_Camera_ARG_startCamera_start_ok);
	}else{
		EHS_FB_FINISH(INX_Camera_ARG_startCamera_start_error);
	}

}//ICB FUNCTION startCamera MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION grabFrame MACRO START -- DO NOT ALTER
/**
 * Definition of Camera_grabFrame.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(Camera_grabFrame)
{
	inx_Camera_state_type* inx_Camera_state = (inx_Camera_state_type*)EHS_FB_RUN_CONTEXT;

	if (EHS_FB_IN_CONNECTED_API2(INX_Camera_ARG_grabFrame_im_show))
		inx_Camera_state->im_show = EHS_FB_IN_B_API2(INX_Camera_ARG_grabFrame_im_show) ;

	ehs_bool ret = EhsCameraGrabFrame(&inx_Camera_state->camera, &inx_Camera_state->frame, inx_Camera_state->im_show);

	if (EHS_FB_OUT_CONNECTED_API2(INX_Camera_ARG_grabFrame_frame_width)){
		EHS_FB_OUT_I_API2(INX_Camera_ARG_grabFrame_frame_width) = inx_Camera_state->frame.width;
	}
	
	if (EHS_FB_OUT_CONNECTED_API2(INX_Camera_ARG_grabFrame_frame_height)){
		EHS_FB_OUT_I_API2(INX_Camera_ARG_grabFrame_frame_height) = inx_Camera_state->frame.height;
	}
	
	if (EHS_FB_OUT_CONNECTED_API2(INX_Camera_ARG_grabFrame_frame_id)){
		EHS_FB_OUT_I_API2(INX_Camera_ARG_grabFrame_frame_id) = inx_Camera_state->frame.id;
	}
	if(ret == EHS_TRUE){
		EHS_FB_FINISH(INX_Camera_ARG_grabFrame_captur_ok);
	}
}//ICB FUNCTION grabFrame MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION stopCamera MACRO START -- DO NOT ALTER
/**
 * Definition of Camera_stopCamera.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(Camera_stopCamera)
{
	inx_Camera_state_type* inx_Camera_state = (inx_Camera_state_type*)EHS_FB_RUN_CONTEXT;
	EhsCameraStop(&inx_Camera_state->camera);
	EHS_FB_FINISH(INX_Camera_ARG_stopCamera_stopped);
}//ICB FUNCTION stopCamera MACRO END -- DO NOT ALTER THIS LINE
