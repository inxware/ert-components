//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-frame_show.h"

#include "hal_mv.h"

#ifdef EHS_MV_SUPPORT__opencv
#include <opencv/opencv_wrapper.h>
#endif

//ICB HEADER MACRO END -- DO NOT ALTER
//ICB STATE VAR MACRO START -- DO NOT ALTER
#define INX_FRAME_SHOW_WINDOW_TITLE_SIZE 255
#define INX_FRAME_SHOW_STR_INDIR(x) #x
#define INX_FRAME_SHOW_STR(x) INX_FRAME_SHOW_STR_INDIR(x)
/* My Component state data structure. - Use this in your code! */
typedef struct inx_frame_show_state
{
	ehs_char window_title[INX_FRAME_SHOW_WINDOW_TITLE_SIZE + 1];
} inx_frame_show_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(frame_show)
EHS_FB_FUNCTION_ENTRY("show", 0x01, frame_show_show)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_frame_show_ARG_show_frame_id 1
#define INX_frame_show_ARG_show_shown 1
#define INX_frame_show_ARG_show_error 2
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER
//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
/* Parameters */
/* Create some macros for the default parameters */
//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER
#define INX_FB_frame_show_window_title "frame"
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(frame_show)
{
/* Uncomment the following if you need to parse the parameters to calculate memory required */
/*
	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,""); */
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_frame_show_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(frame_show)
{
	ehs_bool bRet = EHS_TRUE; /* assume success */
	//this is the reference to the object data for this instance of the function block
	inx_frame_show_state_type* inx_frame_show_state = (inx_frame_show_state_type*)EHS_FB_INIT_CONTEXT;
	EhsMemset(inx_frame_show_state->window_title, 0, INX_FRAME_SHOW_WINDOW_TITLE_SIZE + 1);
	/* read the initialisation parameters */
	EhsSscanf(EHS_FB_INIT_PARAMETERS,"%" INX_FRAME_SHOW_STR(INX_FRAME_SHOW_WINDOW_TITLE_SIZE) "s", inx_frame_show_state->window_title);

	/* Add any further intialisation code here */
	return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(frame_show)
{
	inx_frame_show_state_type *inx_frame_show_state = (inx_frame_show_state_type*)EHS_FB_DESTROY_CONTEXT;
	//Your code below here
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION show MACRO START -- DO NOT ALTER
/**
 * Definition of frame_show_show.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(frame_show_show)
{
	inx_frame_show_state_type* inx_frame_show_state = (inx_frame_show_state_type*)EHS_FB_RUN_CONTEXT;
	ehs_uint8 err = 0;

	// Your code here
	ehs_sint32 frame_id = -1;
	if (EHS_FB_IN_CONNECTED_API2(INX_frame_show_ARG_show_frame_id))
		frame_id = EHS_FB_IN_I_API2(INX_frame_show_ARG_show_frame_id) ;
	if (frame_id < 0) { err = 1; goto done; }
	EhsCameraFrame* src_frame = EhsCameraFrameGetById(frame_id);
	if (src_frame == NULL) { err = 2; goto done; }
#ifdef EHS_MV_SUPPORT__opencv
	cv_mat_show(inx_frame_show_state->window_title[0] == '\0' ? INX_FB_frame_show_window_title : inx_frame_show_state->window_title, (cv_mat*)src_frame->frameObj, 1);
#else
#warning "EHS_MV_SUPPORT is not defined to a format that can show images"
#endif
done:
	if (err == 0) EHS_FB_FINISH(INX_frame_show_ARG_show_shown);
	else EHS_FB_FINISH(INX_frame_show_ARG_show_error);
}//ICB FUNCTION show MACRO END -- DO NOT ALTER THIS LINE
