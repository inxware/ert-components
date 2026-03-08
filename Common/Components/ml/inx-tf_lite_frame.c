//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-tf_lite_frame.h"
//ICB HEADER MACRO END -- DO NOT ALTER

#include "hal_mv.h"
#include "hal_file.h"
#include "hal_ml.h"

//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_TF_Lite_from_frame_state
{
	ehs_sint32 model_type;
	ehs_float conf_thres;
	ehs_sint32 thread_count;
	ehs_bool use_app_dir;
	EhsML_Context ml_ctx;
} inx_TF_Lite_from_frame_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(TF_Lite_from_frame)
EHS_FB_FUNCTION_ENTRY("load_model", 0x01, TF_Lite_from_frame_load_model)
EHS_FB_FUNCTION_ENTRY("do_inference", 0x02, TF_Lite_from_frame_do_inference)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_TF_Lite_from_frame_ARG_load_model_model_file_path 1
#define INX_TF_Lite_from_frame_ARG_load_model_load_errno 1
#define INX_TF_Lite_from_frame_ARG_load_model_load_ok 1
#define INX_TF_Lite_from_frame_ARG_load_model_load_error 2
#define INX_TF_Lite_from_frame_ARG_load_model_model_info 2
#define INX_TF_Lite_from_frame_ARG_do_inference_frame_id 1
#define INX_TF_Lite_from_frame_ARG_do_inference_output 2
#define INX_TF_Lite_from_frame_ARG_do_inference_inference_errno 1
#define INX_TF_Lite_from_frame_ARG_do_inference_done_inference 1
#define INX_TF_Lite_from_frame_ARG_do_inference_inference_error 2
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER
//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
/* Parameters */
/* Create some macros for the default parameters */
#define INX_FB_TF_Lite_from_frame_model_type 0
//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(TF_Lite_from_frame)
{
/* Uncomment the following if you need to parse the parameters to calculate memory required */
/*
	ehs_sint32 model_type;
	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,"%d",&model_type); */
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_TF_Lite_from_frame_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(TF_Lite_from_frame)
{
	ehs_sint32 model_type;
	ehs_bool bRet = EHS_TRUE; /* assume success */
	//this is the reference to the object data for this instance of the function block
	inx_TF_Lite_from_frame_state_type* inx_TF_Lite_from_frame_state = (inx_TF_Lite_from_frame_state_type*)EHS_FB_INIT_CONTEXT;
	EhsMemset(inx_TF_Lite_from_frame_state, 0, sizeof(inx_TF_Lite_from_frame_state_type));
	EhsMemset(&inx_TF_Lite_from_frame_state->ml_ctx, 0, sizeof(EhsML_Context));
	inx_TF_Lite_from_frame_state->thread_count = 2; // default number of threads to two
	/* read the initialisation parameters */
	const char* pParams = EHS_FB_INIT_PARAMETERS;
	if (pParams) {
		pParams = EhsGetSint32FromString(&inx_TF_Lite_from_frame_state->model_type, pParams);
		pParams = EhsGetDoubleFromString(&inx_TF_Lite_from_frame_state->conf_thres, pParams);
		inx_TF_Lite_from_frame_state->ml_ctx.conf_thres = inx_TF_Lite_from_frame_state->conf_thres;
		pParams = EhsGetSint32FromString(&inx_TF_Lite_from_frame_state->thread_count, pParams);
		ehs_uint8 useFlatJsonFormat = 0;
		pParams = EhsGetUint8FromString(&useFlatJsonFormat, pParams);
		inx_TF_Lite_from_frame_state->ml_ctx.enable_flat_json = (useFlatJsonFormat) ? EHS_TRUE : EHS_FALSE;
		ehs_uint8 useAppsDir = 0;
		pParams = EhsGetUint8FromString(&useAppsDir, pParams);
		inx_TF_Lite_from_frame_state->use_app_dir = (useAppsDir) ? EHS_TRUE : EHS_FALSE;
	}

	/* Add any further intialisation code here */
	return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(TF_Lite_from_frame)
{
	inx_TF_Lite_from_frame_state_type *inx_TF_Lite_from_frame_state = (inx_TF_Lite_from_frame_state_type*)EHS_FB_DESTROY_CONTEXT;
	EhsML_Destroy(&inx_TF_Lite_from_frame_state->ml_ctx);
	return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION load_model MACRO START -- DO NOT ALTER
/**
 * Definition of TF_Lite_from_frame_load_model.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(TF_Lite_from_frame_load_model)
{
	inx_TF_Lite_from_frame_state_type* inx_TF_Lite_from_frame_state = (inx_TF_Lite_from_frame_state_type*)EHS_FB_RUN_CONTEXT;

	EhsML_Err err = EHS_ML_OK;
	const ehs_char* model_path = NULL;
	if (EHS_FB_IN_CONNECTED_API2(INX_TF_Lite_from_frame_ARG_load_model_model_file_path)){
		model_path = EHS_FB_IN_S_API2(INX_TF_Lite_from_frame_ARG_load_model_model_file_path) ;
	}else{
		// @TODO - read path from param
	}
	if(model_path){
		ehs_char szCanonicalFilePath[EHS_MAXPATHLENGTH]={'\0'};
		if(inx_TF_Lite_from_frame_state->use_app_dir==EHS_TRUE){
			if (EhsHMetagetCurrentAppDir(szCanonicalFilePath))
			{
				EhsStrcat(szCanonicalFilePath, EHS_TD_FILES_SEPARATOR_STR);
				EhsStrcat(szCanonicalFilePath, model_path);

			}else{
				err = EHS_ML_MODEL_PATH_ERR;
			}
		}else{
			if (EhsTF_tryCanonicPath(szCanonicalFilePath, EHS_RUNTIME_USERDATA_DIR, model_path, EHS_TRUE) == EHS_FALSE){
				err = EHS_ML_MODEL_PATH_ERR;
			}
		}
		err = (err == EHS_ML_OK) ? EhsML_Create(&inx_TF_Lite_from_frame_state->ml_ctx, szCanonicalFilePath,
												 inx_TF_Lite_from_frame_state->model_type,
												 inx_TF_Lite_from_frame_state->conf_thres,
												 inx_TF_Lite_from_frame_state->thread_count) : err;
		if (err == EHS_ML_OK && EHS_FB_OUT_CONNECTED_API2(INX_TF_Lite_from_frame_ARG_load_model_model_info)) {
			EhsML_GetModelInfoJson(
				&inx_TF_Lite_from_frame_state->ml_ctx,
				szCanonicalFilePath,
				EHS_FB_OUT_S_API2(INX_TF_Lite_from_frame_ARG_load_model_model_info),
				EHS_STRING_LENGTH_MAX);
		}
	}else {
		err = EHS_ML_MODEL_PATH_ERR;
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_TF_Lite_from_frame_ARG_load_model_load_errno)){
		EHS_FB_OUT_I_API2(INX_TF_Lite_from_frame_ARG_load_model_load_errno) = err;
	}
	if(err == EHS_ML_OK){
		EHS_FB_FINISH(INX_TF_Lite_from_frame_ARG_load_model_load_ok);
	}else{
		EHS_FB_FINISH(INX_TF_Lite_from_frame_ARG_load_model_load_error);
	}
}//ICB FUNCTION load_model MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION do_inference MACRO START -- DO NOT ALTER
/**
 * Definition of TF_Lite_from_frame_do_inference.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(TF_Lite_from_frame_do_inference)
{
	EhsML_Err err = EHS_ML_OK;
	ehs_sint32 frame_id = -1;

	inx_TF_Lite_from_frame_state_type* inx_TF_Lite_from_frame_state = (inx_TF_Lite_from_frame_state_type*)EHS_FB_RUN_CONTEXT;

	if(!inx_TF_Lite_from_frame_state->ml_ctx.ml_model_ctx){
		err = EHS_ML_INIT_ERR;
		goto inference_fail;
	}

	if (EHS_FB_IN_CONNECTED_API2(INX_TF_Lite_from_frame_ARG_do_inference_frame_id)){
		frame_id = EHS_FB_IN_I_API2(INX_TF_Lite_from_frame_ARG_do_inference_frame_id);
	}

	if(frame_id < -1){
		err = EHS_ML_INVALID_FRAME_ID;
		goto inference_fail;
	}

	if (!EHS_FB_OUT_CONNECTED_API2(INX_TF_Lite_from_frame_ARG_do_inference_output)){
		return; /* no consumer for result - not an error, just nothing to do */
	}

	{
		EhsCameraFrame* frame = EhsCameraFrameGetById(frame_id);
		if(!frame){
			err = EHS_ML_INVALID_FRAME;
			goto inference_fail;
		}

		void* frame_data = NULL;
		ehs_uint32 frame_size = 0;
		if(EHS_FALSE == EhsCameraFrameGetData(frame, &frame_data, &frame_size)){
			err = EHS_ML_INVALID_FRAME;
			goto inference_fail;
		}

		err = EhsML_SetInputData(&inx_TF_Lite_from_frame_state->ml_ctx, frame_data, frame_size);
		if(err != EHS_ML_OK) goto inference_fail;

		err = EhsML_RunOutputJson(&inx_TF_Lite_from_frame_state->ml_ctx,
								   EHS_FB_OUT_S_API2(INX_TF_Lite_from_frame_ARG_do_inference_output),
								   EHS_STRING_LENGTH_MAX);
		if(err != EHS_ML_OK) goto inference_fail;
	}

	if (EHS_FB_OUT_CONNECTED_API2(INX_TF_Lite_from_frame_ARG_do_inference_inference_errno)){
		EHS_FB_OUT_I_API2(INX_TF_Lite_from_frame_ARG_do_inference_inference_errno) = 0;
	}
	EHS_FB_FINISH(INX_TF_Lite_from_frame_ARG_do_inference_done_inference);
	return;

inference_fail:
	if (EHS_FB_OUT_CONNECTED_API2(INX_TF_Lite_from_frame_ARG_do_inference_inference_errno)){
		EHS_FB_OUT_I_API2(INX_TF_Lite_from_frame_ARG_do_inference_inference_errno) = err;
	}
	EHS_FB_FINISH(INX_TF_Lite_from_frame_ARG_do_inference_inference_error);
}//ICB FUNCTION do_inference MACRO END -- DO NOT ALTER THIS LINE
