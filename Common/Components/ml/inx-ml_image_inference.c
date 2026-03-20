//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
//#include <sys/types.h>
#include "inx-ml_image_inference.h"
#include "hal_ml.h"
#include "hal_mv.h"
#include "hal_file.h"
//ICB HEADER MACRO END -- DO NOT ALTER
//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_ml_image_inference_state
{
	ehs_char Model_File_Path[EHS_STRING_LENGTH_MAX];
	EhsML_Type Model_Type;
	EhsML_ModelFormat_t Model_Format;
	EhsML_DataType_t Data_Type;
	EhsML_HWAccel_t HW_Accelerate;
	ehs_float Conf_Thres;
	ehs_sint32 Thread_Number;
	ehs_bool Use_Application_Dir;
	EhsML_Context ml_ctx;
} inx_ml_image_inference_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(ml_image_inference)
EHS_FB_FUNCTION_ENTRY("load_model", 0x01, ml_image_inference_load_model)
EHS_FB_FUNCTION_ENTRY("inference", 0x02, ml_image_inference_inference)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_ml_image_inference_ARG_load_model_model_path 1
#define INX_ml_image_inference_ARG_load_model_load_errno 1
#define INX_ml_image_inference_ARG_load_model_load_done 1
#define INX_ml_image_inference_ARG_load_model_load_err 2
#define INX_ml_image_inference_ARG_load_model_model_info 2
#define INX_ml_image_inference_ARG_inference_stream_id 1
#define INX_ml_image_inference_ARG_inference_inference_errno 1
#define INX_ml_image_inference_ARG_inference_json 2
#define INX_ml_image_inference_ARG_inference_inference_done 1
#define INX_ml_image_inference_ARG_inference_inference_error 2
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER
//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
/* Parameters */
/* Create some macros for the default parameters */
#define INX_FB_ml_image_inference_Model_File_Path 
#define INX_FB_ml_image_inference_Model_Type 0
#define INX_FB_ml_image_inference_Model_Format 1
#define INX_FB_ml_image_inference_Data_Type 0
#define INX_FB_ml_image_inference_HW_Accelerate 0
#define INX_FB_ml_image_inference_Conf_Thres 0.5
#define INX_FB_ml_image_inference_Thread_Number 0
#define INX_FB_ml_image_inference_Use_Application_Dir 0
//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(ml_image_inference)
{
/* Uncomment the following if you need to parse the parameters to calculate memory required */
/*
	ehs_char* Model_File_Path;
	ehs_sint32 Model_Type;
	ehs_sint32 Model_Format;
	ehs_sint32 Data_Type;
	ehs_bool HW_Accelerate;
	ehs_float Conf_Thres;
	ehs_sint32 Thread_Number;
	ehs_bool Use_Application_Dir;
	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,"%s %d %d %d %d %f %d %d",&Model File Path,&Model Type,&Model Format,&Quantisation Level,&HW Accelerate,&Conf Thres,&Thread Number,&Use Application Dir); */
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_ml_image_inference_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(ml_image_inference)
{
	ehs_sint32 Model_Type;
	ehs_sint32 Model_Format;
	ehs_sint32 Data_Type;
	ehs_bool HW_Accelerate;
	ehs_float Conf_Thres;
	ehs_sint32 Thread_Number;
	ehs_bool Use_Application_Dir;
	ehs_bool bRet = EHS_TRUE; /* assume success */
	//this is the reference to the object data for this instance of the function block
	inx_ml_image_inference_state_type* inx_ml_image_inference_state = (inx_ml_image_inference_state_type*)EHS_FB_INIT_CONTEXT;
	/* read the initialisation parameters */
	const char *pParams = EHS_FB_INIT_PARAMETERS;
	if (pParams) {
		ehs_uint8 hw_accel = 0;
		ehs_uint8 useAppDir = 0;
		ehs_char* Model_File_Path;
		pParams = EhsGetSint32FromString(&inx_ml_image_inference_state->Model_Type, pParams);
		pParams = EhsGetSint32FromString(&inx_ml_image_inference_state->Model_Format, pParams);
		pParams = EhsGetSint32FromString(&inx_ml_image_inference_state->Data_Type, pParams);
		pParams = EhsGetUint8FromString(&hw_accel, pParams);
		inx_ml_image_inference_state->HW_Accelerate = (hw_accel) ? EHS_TRUE : EHS_FALSE;
		pParams = EhsGetDoubleFromString(&inx_ml_image_inference_state->Conf_Thres, pParams);
		pParams = EhsGetSint32FromString(&inx_ml_image_inference_state->Thread_Number, pParams);
		pParams = EhsGetUint8FromString(&useAppDir, pParams);
		inx_ml_image_inference_state->Use_Application_Dir = (useAppDir) ? EHS_TRUE : EHS_FALSE;
	}
	/* Initialise your context data structure here */
	inx_ml_image_inference_state->ml_ctx = (EhsML_Context){0};

	/* Add any further intialisation code here */
	return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(ml_image_inference)
{
	inx_ml_image_inference_state_type *inx_ml_image_inference_state = (inx_ml_image_inference_state_type*)EHS_FB_DESTROY_CONTEXT;
	EhsML_Destroy(&inx_ml_image_inference_state->ml_ctx);
	return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
/**
 * @brief Case insensitive check of file extension
 * 
 * @param file_path 
 * @param expected_ext 
 * @return ehs_bool 
 */
ehs_bool _check_file_extension(const ehs_char* file_path, const ehs_char *expected_ext)
{
	if (!file_path || !expected_ext) return EHS_FALSE;
	const ehs_char *dot = NULL;
	const ehs_char *_dot = file_path;
	ehs_uint32 file_path_len = EhsStrlen(file_path);
	ehs_uint32 ext_len = EhsStrlen(expected_ext);
	do {
		dot = _dot;
		_dot = EhsStrchr(_dot, '.');
		if (_dot != NULL) {
			ehs_uint32 remaining_len = file_path_len - (ehs_uint32)(_dot - file_path) - 1;
			if (remaining_len == ext_len) {
				// compare extensions
				if (EhsStrnicmp(_dot + 1, expected_ext, ext_len) == 0) {
					return EHS_TRUE;
				}
			}
			_dot++; // move past the current dot for next search
		}
	} while (_dot != NULL);
	if (!dot || dot == file_path) return EHS_FALSE; // No extension found

	return EHS_FALSE;
}
#define _EHS_ML_IMG_INFERENCE_GOTO_ON_ERROR(error, label, tag, msg)  do { \
		EhsML_Err _err = error;            \
		if (_err != EHS_ML_OK) {            \
			EHSH_LOG_ERROR("[%s] %s", tag, msg); \
			goto label;                    \
		}                                   \
	} while(0)
//ICB FUNCTION load_model MACRO START -- DO NOT ALTER
/**
 * Definition of ml_image_inference_load_model.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ml_image_inference_load_model)
{
	inx_ml_image_inference_state_type* inx_ml_image_inference_state = (inx_ml_image_inference_state_type*)EHS_FB_RUN_CONTEXT;

	// Your code here
	EhsML_Err err = EHS_ML_OK;
	if (EHS_FB_IN_CONNECTED_API2(INX_ml_image_inference_ARG_load_model_model_path))
		EhsStrcpy(inx_ml_image_inference_state->Model_File_Path, EHS_FB_IN_S_API2(INX_ml_image_inference_ARG_load_model_model_path));
	// Determine the file extension

	printf("**** IMAGE_REF**** PATH=%s, for type %d (expected %d)",inx_ml_image_inference_state->Model_File_Path,inx_ml_image_inference_state->Model_Format,EHS_ML_FORMAT_TFLITE);
	switch ((EhsML_ModelFormat_t)inx_ml_image_inference_state->Model_Format) {
		case EHS_ML_FORMAT_TFLITE:
			/* Accept .tflite (full) and .tfl (8.3 short) */
			err = (_check_file_extension(inx_ml_image_inference_state->Model_File_Path, "tflite") ||
			       _check_file_extension(inx_ml_image_inference_state->Model_File_Path, "tfl"))
			      ? EHS_ML_OK : EHS_ML_MODEL_NAME_ERR;
			break;
		case EHS_ML_FORMAT_ONNX:
			/* Accept .onnx (full) and .onn (8.3 short) */
			err = (_check_file_extension(inx_ml_image_inference_state->Model_File_Path, "onnx") ||
			       _check_file_extension(inx_ml_image_inference_state->Model_File_Path, "onn"))
			      ? EHS_ML_OK : EHS_ML_MODEL_NAME_ERR;
			break;
		case EHS_ML_FORMAT_PB:
			err = _check_file_extension(inx_ml_image_inference_state->Model_File_Path, "pb") ? EHS_ML_OK : EHS_ML_MODEL_NAME_ERR;
			break;
		case EHS_ML_FORMAT_HEF:
			// HEF
			err = _check_file_extension(inx_ml_image_inference_state->Model_File_Path, "hef") ? EHS_ML_OK : EHS_ML_MODEL_NAME_ERR;
			if (err == EHS_ML_OK)
			{
				// Check hardware acceleration
				if (inx_ml_image_inference_state->HW_Accelerate == EHS_FALSE)
				{
					printf("^^^50!\n");
					err = EHS_ML_MODEL_TYPE_ERR; // HEF model requires hardware acceleration
					break; // exit switch
				}
				if (EhsML_HWAccel_supported() != EHS_ML_HWACCEL_HAILO)
				{
					err = EHS_ML_INVALID_DEP; // Hailo hardware acceleration not supported
					break; // exit switch
				}
			}
			break;
		default:
			err = EHS_ML_MODEL_NAME_ERR;
			break;
	}
	_EHS_ML_IMG_INFERENCE_GOTO_ON_ERROR(err, err, __func__, "Model file extension does not match!");

	{
		ehs_char szCanonicalFilePath[EHS_MAXPATHLENGTH] = { '\0' };
		if (inx_ml_image_inference_state->Use_Application_Dir == EHS_TRUE) {
			if (EhsHMetagetCurrentAppDir(szCanonicalFilePath))
			{
				EhsStrcat(szCanonicalFilePath, EHS_TD_FILES_SEPARATOR_STR);
				EhsStrcat(szCanonicalFilePath, inx_ml_image_inference_state->Model_File_Path);

			}
			else {
				err = EHS_ML_MODEL_PATH_ERR;
			}
		}
		else {
			if (EhsTF_tryCanonicPath(szCanonicalFilePath, EHS_RUNTIME_USERDATA_DIR, inx_ml_image_inference_state->Model_File_Path, EHS_TRUE) == EHS_FALSE) {
				err = EHS_ML_MODEL_PATH_ERR;
			}
		}
		_EHS_ML_IMG_INFERENCE_GOTO_ON_ERROR(err, err, __func__, "Model file does not exist!");
        printf("Which one ??? Just for a change...\n");
		err = EhsML_Create(&inx_ml_image_inference_state->ml_ctx, szCanonicalFilePath,
			(EhsML_Type)inx_ml_image_inference_state->Model_Type,
			inx_ml_image_inference_state->Conf_Thres,
			inx_ml_image_inference_state->Thread_Number);
		_EHS_ML_IMG_INFERENCE_GOTO_ON_ERROR(err, err, __func__, "Failed to create ML context!");

		if (EHS_FB_OUT_CONNECTED_API2(INX_ml_image_inference_ARG_load_model_model_info)) {
			EhsML_GetModelInfoJson(
				&inx_ml_image_inference_state->ml_ctx,
				szCanonicalFilePath,
				EHS_FB_OUT_S_API2(INX_ml_image_inference_ARG_load_model_model_info),
				EHS_STRING_LENGTH_MAX);
		}
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_ml_image_inference_ARG_load_model_load_errno))
		EHS_FB_OUT_I_API2(INX_ml_image_inference_ARG_load_model_load_errno) = 0;
	EHS_FB_FINISH(INX_ml_image_inference_ARG_load_model_load_done);
	return;
err:
	if (EHS_FB_OUT_CONNECTED_API2(INX_ml_image_inference_ARG_load_model_load_errno))
		EHS_FB_OUT_I_API2(INX_ml_image_inference_ARG_load_model_load_errno) = err;
	EHS_FB_FINISH(INX_ml_image_inference_ARG_load_model_load_err);
	return;
}//ICB FUNCTION load_model MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION inference MACRO START -- DO NOT ALTER
/**
 * Definition of ml_image_inference_inference.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ml_image_inference_inference)
{
	inx_ml_image_inference_state_type* inx_ml_image_inference_state = (inx_ml_image_inference_state_type*)EHS_FB_RUN_CONTEXT;

	// Your code here
	EhsML_Err err = EHS_ML_OK;
	ehs_sint32 frame_id = -1;

	if (inx_ml_image_inference_state->ml_ctx.ml_model_ctx == NULL) err = EHS_ML_MODEL_CTX_ERR;
	_EHS_ML_IMG_INFERENCE_GOTO_ON_ERROR(err, error, __func__, "Model Context is NULL!");

	if (EHS_FB_IN_CONNECTED_API2(INX_ml_image_inference_ARG_inference_stream_id))
		frame_id = EHS_FB_IN_I_API2(INX_ml_image_inference_ARG_inference_stream_id) ;
	if (frame_id < 0) err = EHS_ML_INVALID_FRAME_ID;
	_EHS_ML_IMG_INFERENCE_GOTO_ON_ERROR(err, error, __func__, "Invalid stream ID!");

	// Only do inference if the JSON output is connected
	if (EHS_FB_OUT_CONNECTED_API2(INX_ml_image_inference_ARG_inference_json))
	{
		EhsCameraFrame *frame = EhsCameraFrameGetById(frame_id);
		if (!frame) err = EHS_ML_INVALID_FRAME;
		_EHS_ML_IMG_INFERENCE_GOTO_ON_ERROR(err, error, __func__, "Failed to get frame by ID!");

		void *frame_data = NULL;
		ehs_uint32 frame_size = 0;
		if (EHS_FALSE == EhsCameraFrameGetData(frame, &frame_data, &frame_size)) err = EHS_ML_INVALID_FRAME;
		_EHS_ML_IMG_INFERENCE_GOTO_ON_ERROR(err, error, __func__, "Failed to get frame data!");

		/* If the frame was captured with OpenCL, download GPU→CPU before
		 * passing to TFLite or Hailo which require CPU-accessible data. */
		EhsCameraFrameEnsureCPU(frame);
		printf(">>>image presnetation is %d x %x\n",frame_size, frame_data);
		err = EhsML_SetInputData(&inx_ml_image_inference_state->ml_ctx, frame_data, frame_size);
		_EHS_ML_IMG_INFERENCE_GOTO_ON_ERROR(err, error, __func__, "Failed to set input data!");

		err = EhsML_RunOutputJson(&inx_ml_image_inference_state->ml_ctx,
			EHS_FB_OUT_S_API2(INX_ml_image_inference_ARG_inference_json),
			EHS_STRING_LENGTH_MAX);
		_EHS_ML_IMG_INFERENCE_GOTO_ON_ERROR(err, error, __func__, "Failed to run inference!");
		//EHS_FB_OUT_S_API2(INX_ml_image_inference_ARG_inference_json) ;
	}

	if (EHS_FB_OUT_CONNECTED_API2(INX_ml_image_inference_ARG_inference_inference_errno))
		EHS_FB_OUT_I_API2(INX_ml_image_inference_ARG_inference_inference_errno) = 0;
	EHS_FB_FINISH(INX_ml_image_inference_ARG_inference_inference_done);
	return;
error:
	if (EHS_FB_OUT_CONNECTED_API2(INX_ml_image_inference_ARG_inference_inference_errno))
		EHS_FB_OUT_I_API2(INX_ml_image_inference_ARG_inference_inference_errno) = err;
	EHS_FB_FINISH(INX_ml_image_inference_ARG_inference_inference_error);
	return;
}//ICB FUNCTION inference MACRO END -- DO NOT ALTER THIS LINE
