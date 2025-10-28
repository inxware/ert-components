//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-frame_read_file.h"
//ICB HEADER MACRO END -- DO NOT ALTER
#include "hal_mv.h"
//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_FrameReadFromFile_state
{
	ehs_bool Use_Application_Dir;
	EhsCameraFrame frame;
} inx_FrameReadFromFile_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(FrameReadFromFile)
EHS_FB_FUNCTION_ENTRY("run_read_file", 0x01, FrameReadFromFile_run_read_file)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_FrameReadFromFile_ARG_run_read_file_filepath 1
#define INX_FrameReadFromFile_ARG_run_read_file_start_errno 1
#define INX_FrameReadFromFile_ARG_run_read_file_formated_frame_id 2
#define INX_FrameReadFromFile_ARG_run_read_file_start_ok 1
#define INX_FrameReadFromFile_ARG_run_read_file_start_error 2
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER
//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
/* Parameters */
/* Create some macros for the default parameters */
#define INX_FB_FrameReadFromFile_Use_Application_Dir 0
//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(FrameReadFromFile)
{
/* Uncomment the following if you need to parse the parameters to calculate memory required */
/*
	ehs_bool Use_Application_Dir;
	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,"%d",&Use Application Dir); */
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_FrameReadFromFile_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(FrameReadFromFile)
{
	ehs_bool bRet = EHS_TRUE; /* assume success */
	//this is the reference to the object data for this instance of the function block
	inx_FrameReadFromFile_state_type* inx_FrameReadFromFile_state = (inx_FrameReadFromFile_state_type*)EHS_FB_INIT_CONTEXT;
	EhsMemset(inx_FrameReadFromFile_state, 0, sizeof(inx_FrameReadFromFile_state_type));
	EhsMemset(&inx_FrameReadFromFile_state->frame, 0, sizeof(EhsCameraFrame));
	/* read the initialisation parameters */
	const char* pParams = EHS_FB_INIT_PARAMETERS;
	if (pParams) {
		ehs_uint8 isFromAppsDir = 0;
		pParams = EhsGetUint8FromString(&isFromAppsDir, pParams);
		inx_FrameReadFromFile_state->Use_Application_Dir = (isFromAppsDir) ? EHS_TRUE : EHS_FALSE;
	}

	// create frame instace for this camera and add it to the global list
	EhsCameraFrameCreate(&inx_FrameReadFromFile_state->frame);
	EhsCameraFrameAdd(&inx_FrameReadFromFile_state->frame); // add frame to global buffer 

	/* Add any further intialisation code here */
	return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(FrameReadFromFile)
{
	inx_FrameReadFromFile_state_type *inx_FrameReadFromFile_state = (inx_FrameReadFromFile_state_type*)EHS_FB_DESTROY_CONTEXT;
	EhsCameraFrameRemove(&inx_FrameReadFromFile_state->frame); // remove frame from global buffer
	EhsCameraFrameDestroy(&inx_FrameReadFromFile_state->frame);
	return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION run_read_file MACRO START -- DO NOT ALTER
/**
 * Definition of FrameReadFromFile_run_read_file.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(FrameReadFromFile_run_read_file)
{
	inx_FrameReadFromFile_state_type* inx_FrameReadFromFile_state = (inx_FrameReadFromFile_state_type*)EHS_FB_RUN_CONTEXT;
	const ehs_char* file_path = NULL;
	// Your code here
	if (EHS_FB_IN_CONNECTED_API2(INX_FrameReadFromFile_ARG_run_read_file_filepath)){
		file_path = EHS_FB_IN_S_API2(INX_FrameReadFromFile_ARG_run_read_file_filepath);
	}
	ehs_bool ret = EHS_FALSE;
	if(file_path){
		ehs_char szCanonicalFilePath[EHS_MAXPATHLENGTH]={'\0'};
		if(inx_FrameReadFromFile_state->Use_Application_Dir==EHS_TRUE){
			if (EhsHMetagetCurrentAppDir(szCanonicalFilePath))
			{
				EhsStrcat(szCanonicalFilePath, EHS_TD_FILES_SEPARATOR_STR);
				EhsStrcat(szCanonicalFilePath, file_path);
				
			}else{
				ret = EHS_FALSE;
			}
		}else{
			if (EhsTF_tryCanonicPath(szCanonicalFilePath, EHS_RUNTIME_USERDATA_DIR, file_path, EHS_TRUE) == EHS_FALSE){
				ret = EHS_FALSE;
			}
		}
		ret = EhsCameraFrameReadFromFile(&inx_FrameReadFromFile_state->frame, szCanonicalFilePath);
	}else {
		ret = EHS_FALSE;
	}

	if (EHS_FB_OUT_CONNECTED_API2(INX_FrameReadFromFile_ARG_run_read_file_formated_frame_id)){
		EHS_FB_OUT_I_API2(INX_FrameReadFromFile_ARG_run_read_file_formated_frame_id) = inx_FrameReadFromFile_state->frame.id;
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_FrameReadFromFile_ARG_run_read_file_start_errno)){
		EHS_FB_OUT_I_API2(INX_FrameReadFromFile_ARG_run_read_file_start_errno) = (ret == EHS_TRUE) ? 0 : 1;
	}
	if(ret == EHS_TRUE){
		EHS_FB_FINISH(INX_FrameReadFromFile_ARG_run_read_file_start_ok);
	}else{
		EHS_FB_FINISH(INX_FrameReadFromFile_ARG_run_read_file_start_error);
	}
}//ICB FUNCTION run_read_file MACRO END -- DO NOT ALTER THIS LINE
