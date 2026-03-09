//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-frame_resize.h"
//ICB HEADER MACRO END -- DO NOT ALTER
#include "hal_mv.h"
//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_FrameResize_state
{
	ehs_sint32 size_x;
	ehs_sint32 size_y;
	ehs_sint32 interp; /* interpolation method: 0=NEAREST, 1=LINEAR, 2=CUBIC, 3=AREA */

	EhsCameraFrame frame;

} inx_FrameResize_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
typedef enum {
	FRAME_RESIZE_OK = 0,
	FRAME_RESIZE_FAILED,
	FRAME_RESIZE_UNUSED
} EhsFrameResizeErr;
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(FrameResize)
EHS_FB_FUNCTION_ENTRY("resize", 0x01, FrameResize_resize)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_FrameResize_ARG_resize_frame_id 1
#define INX_FrameResize_ARG_resize_size_x 2
#define INX_FrameResize_ARG_resize_size_y 3
#define INX_FrameResize_ARG_resize_start_errno 1
#define INX_FrameResize_ARG_resize_formated_frame_id 2
#define INX_FrameResize_ARG_resize_start_ok 1
#define INX_FrameResize_ARG_resize_start_error 2
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER
//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
/* Parameters */
/* Create some macros for the default parameters */
#define INX_FB_FrameResize_size_x 0
#define INX_FB_FrameResize_size_y 0
//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(FrameResize)
{
/* Uncomment the following if you need to parse the parameters to calculate memory required */
/*
	ehs_sint32 size_x;
	ehs_sint32 size_y;
	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,"%d %d",&size_x,&size_y); */
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_FrameResize_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(FrameResize)
{
	ehs_bool bRet = EHS_TRUE; /* assume success */
	//this is the reference to the object data for this instance of the function block
	inx_FrameResize_state_type* inx_FrameResize_state = (inx_FrameResize_state_type*)EHS_FB_INIT_CONTEXT;
	
	EhsMemset(&inx_FrameResize_state->frame, 0, sizeof(EhsCameraFrame));
	/* read the initialisation parameters */
	inx_FrameResize_state->interp = 1; /* default: INTER_LINEAR */
	const char* pParams = EHS_FB_INIT_PARAMETERS;
	if (pParams) {
		pParams = EhsGetSint32FromString(&inx_FrameResize_state->size_x, pParams);
		pParams = EhsGetSint32FromString(&inx_FrameResize_state->size_y, pParams);
		pParams = EhsGetSint32FromString(&inx_FrameResize_state->interp, pParams);
	}

	// create frame instace for this camera and add it to the global list
	EhsCameraFrameCreate(&inx_FrameResize_state->frame);
	EhsCameraFrameAdd(&inx_FrameResize_state->frame); // add frame to global buffer 

	/* Add any further intialisation code here */
	return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(FrameResize)
{
	inx_FrameResize_state_type *inx_FrameResize_state = (inx_FrameResize_state_type*)EHS_FB_DESTROY_CONTEXT;
	
	EhsCameraFrameRemove(&inx_FrameResize_state->frame); // remove frame from global buffer
	EhsCameraFrameDestroy(&inx_FrameResize_state->frame);
	return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION resize MACRO START -- DO NOT ALTER
/**
 * Definition of FrameResize_resize.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(FrameResize_resize)
{
	inx_FrameResize_state_type* inx_FrameResize_state = (inx_FrameResize_state_type*)EHS_FB_RUN_CONTEXT;

	ehs_uint32 size_x = inx_FrameResize_state->size_x, size_y = inx_FrameResize_state->size_y;

	ehs_sint32 frame_id = -1;
	EhsFrameResizeErr err = FRAME_RESIZE_OK;

	if(inx_FrameResize_state->frame.id < 0) {err = FRAME_RESIZE_FAILED; goto done; }
	
	if (EHS_FB_IN_CONNECTED_API2(INX_FrameResize_ARG_resize_frame_id)){
		frame_id = EHS_FB_IN_I_API2(INX_FrameResize_ARG_resize_frame_id) ;
	}
	if(frame_id < 0) { err = FRAME_RESIZE_FAILED; goto done; }
	EhsCameraFrame* src_frame = EhsCameraFrameGetById(frame_id);
	if(!src_frame) { err = FRAME_RESIZE_FAILED; goto done; }

	if (EHS_FB_IN_CONNECTED_API2(INX_FrameResize_ARG_resize_size_x)){
		size_x = EHS_FB_IN_I_API2(INX_FrameResize_ARG_resize_size_x) ;
	}
	if (EHS_FB_IN_CONNECTED_API2(INX_FrameResize_ARG_resize_size_y)){
		size_y = EHS_FB_IN_I_API2(INX_FrameResize_ARG_resize_size_y) ;
	}

	if(size_x > 0 || size_y > 0){
		if(EHS_FALSE == EhsCameraFrameResize(src_frame, &inx_FrameResize_state->frame, size_x, size_y, inx_FrameResize_state->interp)) { err = FRAME_RESIZE_FAILED; goto done; }
	}else{
		err = FRAME_RESIZE_UNUSED; goto done;
	}

	if (EHS_FB_OUT_CONNECTED_API2(INX_FrameResize_ARG_resize_formated_frame_id)){
		EHS_FB_OUT_I_API2(INX_FrameResize_ARG_resize_formated_frame_id) = inx_FrameResize_state->frame.id;
	}
done:
	if (EHS_FB_OUT_CONNECTED_API2(INX_FrameResize_ARG_resize_start_errno)){
		EHS_FB_OUT_I_API2(INX_FrameResize_ARG_resize_start_errno) = err ;
	}
	if(err == FRAME_RESIZE_OK){
		EHS_FB_FINISH(INX_FrameResize_ARG_resize_start_ok);
	}else{
		EHS_FB_FINISH(INX_FrameResize_ARG_resize_start_error);
	}

}//ICB FUNCTION resize MACRO END -- DO NOT ALTER THIS LINE
