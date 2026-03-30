//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-frame_show.h"

#include "hal_mv.h"
#include "hal_process.h"

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
	ehs_char            window_title[INX_FRAME_SHOW_WINDOW_TITLE_SIZE + 1];
	EhsTPMutexClass     disp_mutex;
	EhsTPConditionClass disp_cond;
	EhsTPConditionClass disp_done_cond;
	volatile ehs_sint32 disp_frame_id;  /* -1 = idle, >=0 = frame pending */
	volatile int        disp_running;
	volatile int        disp_done;
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

/**
 * Persistent display thread — owns all cv_mat_imshow calls so that the
 * show run function returns (fires "shown") without blocking on rendering.
 *
 * cv_window_start_thread() is called in init so that the GTK event loop
 * runs in OpenCV's own background thread, making cv::imshow() safe to call
 * from here on all supported backends.
 *
 * The thread takes the state pointer as its context (not pFIdata) because
 * it never needs to fire EHS finish events — that is done by the run function.
 */
static EhsThreadFuncReturnType _frame_show_display_worker(void* arg)
{
	inx_frame_show_state_type* state = (inx_frame_show_state_type*)arg;
	const char* title = state->window_title[0]
		? state->window_title
		: INX_FB_frame_show_window_title;

	while (state->disp_running)
	{
		EhsTPMutex_lock(state->disp_mutex);
		while (state->disp_frame_id < 0 && state->disp_running)
			EhsTPCondition_wait(state->disp_cond, state->disp_mutex);
		ehs_sint32 frame_id = state->disp_frame_id;
		state->disp_frame_id = -1;
		EhsTPMutex_unlock(state->disp_mutex);

		if (!state->disp_running) break;

#ifdef EHS_MV_SUPPORT__opencv
		EhsCameraFrame* src = EhsCameraFrameGetById(frame_id);
		if (src) {
			cv_mat_imshow(title, (cv_mat*)src->frameObj);
			/* Pump Qt's event loop so the window paints.
			 * cv::startWindowThread() is a no-op for the Qt highgui backend,
			 * so this is the sole event-loop driver.  Must stay in THIS thread —
			 * Qt associates windows with the thread that first called imshow(). */
			cv_mat_waitkey(1);
		}
#endif
	}

#ifdef EHS_MV_SUPPORT__opencv
	/* Destroy the window from this thread before exiting.  Qt ties window
	 * ownership to the thread that created it, so destroying here keeps the
	 * association clean — the next SODL load's fresh worker thread can create
	 * a new window without inheriting stale Qt thread state. */
	cv_mat_destroy_all_windows();
#endif

	EhsTPMutex_lock(state->disp_mutex);
	state->disp_done = 1;
	EhsTPCondition_signal(state->disp_done_cond);
	EhsTPMutex_unlock(state->disp_mutex);
	return 0;
}

//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(frame_show)
{
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
	inx_frame_show_state_type* state = (inx_frame_show_state_type*)EHS_FB_INIT_CONTEXT;

	EhsMemset(state->window_title, 0, INX_FRAME_SHOW_WINDOW_TITLE_SIZE + 1);
	EhsSscanf(EHS_FB_INIT_PARAMETERS,
	          "%" INX_FRAME_SHOW_STR(INX_FRAME_SHOW_WINDOW_TITLE_SIZE) "s",
	          state->window_title);

	state->disp_frame_id  = -1;
	state->disp_running   = 0;
	state->disp_done      = 0;
	state->disp_mutex     = NULL;
	state->disp_cond      = NULL;
	state->disp_done_cond = NULL;

	if (!EhsHMutex_create(&state->disp_mutex)    ||
	    !EhsHCond_create(&state->disp_cond)       ||
	    !EhsHCond_create(&state->disp_done_cond))
	{
		bRet = EHS_FALSE;
	}
	else
	{
#ifdef EHS_MV_SUPPORT__opencv
		/* Allow cv::imshow() to be called from the display thread on all backends */
		cv_window_start_thread();
#endif
		state->disp_running = 1;
		if (!EhsHThread_execute(_frame_show_display_worker, (void*)state,
		                        0, EHS_THREAD_USE_DEFAULT_STACK_SIZE))
		{
			state->disp_running = 0;
			bRet = EHS_FALSE;
		}
	}
	return bRet;
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(frame_show)
{
	inx_frame_show_state_type* state = (inx_frame_show_state_type*)EHS_FB_DESTROY_CONTEXT;

	if (state->disp_mutex && state->disp_running)
	{
		EhsTPMutex_lock(state->disp_mutex);
		state->disp_running = 0;
		EhsTPCondition_signal(state->disp_cond);
		while (!state->disp_done)
			EhsTPCondition_wait(state->disp_done_cond, state->disp_mutex);
		EhsTPMutex_unlock(state->disp_mutex);
	}

	EhsHMutex_destroy(&state->disp_mutex);
	EhsHCond_destroy(&state->disp_cond);
	EhsHCond_destroy(&state->disp_done_cond);
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
	inx_frame_show_state_type* state = (inx_frame_show_state_type*)EHS_FB_RUN_CONTEXT;
	ehs_uint8 err = 0;

	ehs_sint32 frame_id = -1;
	if (EHS_FB_IN_CONNECTED_API2(INX_frame_show_ARG_show_frame_id))
		frame_id = EHS_FB_IN_I_API2(INX_frame_show_ARG_show_frame_id);
	if (frame_id < 0) { err = 1; goto done; }
	if (!EhsCameraFrameGetById(frame_id)) { err = 2; goto done; }

	if (state->disp_mutex)
	{
		EhsTPMutex_lock(state->disp_mutex);
		state->disp_frame_id = frame_id;  /* latest frame wins; previous is dropped */
		EhsTPCondition_signal(state->disp_cond);
		EhsTPMutex_unlock(state->disp_mutex);
	}

done:
	if (err == 0) EHS_FB_FINISH(INX_frame_show_ARG_show_shown);
	else EHS_FB_FINISH(INX_frame_show_ARG_show_error);
}//ICB FUNCTION show MACRO END -- DO NOT ALTER THIS LINE
