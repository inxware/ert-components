/*
 * TODO LIST
 *
 * MODEL TYPE AUTO-DETECTION
 *   Model_Type == 0 (auto/generic) runs raw inference without model-specific
 *   decode or NMS, leaving ctx->detections[] empty.  A future improvement
 *   would inspect the model's input/output tensor shapes and embedded metadata
 *   to infer the architecture (e.g. YOLOv5 vs YOLOv8) automatically.
 *
 * DEBUG PRINTF CLEANUP
 *   Several printf() calls remain from development (image presentation size,
 *   load path, format check).  These should be replaced with EHSH_LOG_DEBUG
 *   calls or removed before production use.
 */

//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-ml_image_inference.h"
#include "hal_ml.h"
#include "hal_mv.h"
#include "hal_file.h"
//ICB HEADER MACRO END -- DO NOT ALTER

/* Threading via EHS HAL — do NOT include <pthread.h> directly in Common/ code.
 * All thread/mutex/condition operations must go through hal_process.h abstractions. */
#include "hal_process.h"

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
	ehs_bool Flat_Json;
	ehs_bool Normalized_Coords;
	EhsML_Context ml_ctx;

	/* Inference worker thread — persistent, woken by condition signal.
	 * All fields below are protected by worker_mutex unless noted. */
	EhsTPMutexClass     worker_mutex;
	EhsTPConditionClass worker_cond;      /* signalled when work is queued or exit requested */
	EhsTPConditionClass worker_done_cond; /* signalled by worker thread just before it exits  */
	int                 worker_started;   /* 1 = thread running (unprotected — set before use) */
	int                 worker_busy;      /* 1 = inference in progress                         */
	int                 worker_exit;      /* 1 = thread should exit                            */
	int                 worker_done;      /* 1 = thread has exited (set by thread)             */
	ehs_sint32          pending_frame_id; /* frame to process on next wakeup                  */
	void*               pFIdata;          /* saved EhsFunctionInstanceDataType* for inference  */

	/* Load worker — one-shot thread per load_model call */
	int                 load_busy;        /* 1 = load in progress (checked from run function)  */
	void*               load_pFIdata;     /* saved EhsFunctionInstanceDataType* for load       */
	ehs_char            pending_model_path[EHS_MAXPATHLENGTH]; /* canonical path for load thread */
} inx_ml_image_inference_state_type;
//ICB STATE VAR MACRO END -- DO NOT ALTER

//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
EHS_FB_FUNCTIONS_START(ml_image_inference)
EHS_FB_FUNCTION_ENTRY("load_model", 0x01, ml_image_inference_load_model)
EHS_FB_FUNCTION_ENTRY("inference", 0x02, ml_image_inference_inference)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER

//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
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
#define INX_FB_ml_image_inference_Model_File_Path
#define INX_FB_ml_image_inference_Model_Type 0
#define INX_FB_ml_image_inference_Model_Format 1
#define INX_FB_ml_image_inference_Data_Type 0
#define INX_FB_ml_image_inference_HW_Accelerate 0
#define INX_FB_ml_image_inference_Conf_Thres 0.5
#define INX_FB_ml_image_inference_Thread_Number 0
#define INX_FB_ml_image_inference_Use_Application_Dir 0
#define INX_FB_ml_image_inference_Flat_Json 0
#define INX_FB_ml_image_inference_Normalized_Coords 0
//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER

//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_IDENTIFY_FUNCTION(ml_image_inference)
{
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_ml_image_inference_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER

//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_INIT_FUNCTION(ml_image_inference)
{
	ehs_bool bRet = EHS_TRUE;
	inx_ml_image_inference_state_type* inx_ml_image_inference_state = (inx_ml_image_inference_state_type*)EHS_FB_INIT_CONTEXT;
	const char *pParams = EHS_FB_INIT_PARAMETERS;
	if (pParams) {
		ehs_uint8 hw_accel = 0;
		ehs_uint8 useAppDir = 0;
		pParams = EhsGetSint32FromString(&inx_ml_image_inference_state->Model_Type, pParams);
		pParams = EhsGetSint32FromString(&inx_ml_image_inference_state->Model_Format, pParams);
		pParams = EhsGetSint32FromString(&inx_ml_image_inference_state->Data_Type, pParams);
		pParams = EhsGetUint8FromString(&hw_accel, pParams);
		inx_ml_image_inference_state->HW_Accelerate = (hw_accel) ? EHS_TRUE : EHS_FALSE;
		pParams = EhsGetDoubleFromString(&inx_ml_image_inference_state->Conf_Thres, pParams);
		pParams = EhsGetSint32FromString(&inx_ml_image_inference_state->Thread_Number, pParams);
		pParams = EhsGetUint8FromString(&useAppDir, pParams);
		inx_ml_image_inference_state->Use_Application_Dir = (useAppDir) ? EHS_TRUE : EHS_FALSE;
		ehs_uint8 flatJson = 0;
		pParams = EhsGetUint8FromString(&flatJson, pParams);
		inx_ml_image_inference_state->Flat_Json = (flatJson) ? EHS_TRUE : EHS_FALSE;
		ehs_uint8 normalizedCoords = 0;
		pParams = EhsGetUint8FromString(&normalizedCoords, pParams);
		inx_ml_image_inference_state->Normalized_Coords = (normalizedCoords) ? EHS_TRUE : EHS_FALSE;
	}
	inx_ml_image_inference_state->ml_ctx        = (EhsML_Context){0};
	inx_ml_image_inference_state->worker_mutex  = NULL;
	inx_ml_image_inference_state->worker_cond   = NULL;
	inx_ml_image_inference_state->worker_done_cond = NULL;
	inx_ml_image_inference_state->worker_started = 0;
	inx_ml_image_inference_state->worker_busy   = 0;
	inx_ml_image_inference_state->worker_exit   = 0;
	inx_ml_image_inference_state->worker_done   = 0;
	inx_ml_image_inference_state->load_busy     = 0;
	inx_ml_image_inference_state->pFIdata       = NULL;
	inx_ml_image_inference_state->load_pFIdata  = NULL;
	return bRet;
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER

/* -----------------------------------------------------------------------
 * Inference worker thread helpers
 * ----------------------------------------------------------------------- */

/* Signal the worker thread to exit and wait until it has done so.
 * Must NOT be called with worker_mutex held. */
static void _ml_inference_stop_worker(inx_ml_image_inference_state_type* state)
{
	if (!state->worker_started) return;

	EhsTPMutex_lock(state->worker_mutex);
	state->worker_exit = 1;
	EhsTPCondition_signal(state->worker_cond);
	while (!state->worker_done)
		EhsTPCondition_wait(state->worker_done_cond, state->worker_mutex);
	EhsTPMutex_unlock(state->worker_mutex);

	state->worker_started = 0;
	state->worker_busy    = 0;
	state->worker_exit    = 0;
	state->worker_done    = 0;

	EhsHMutex_destroy(&state->worker_mutex);
	EhsHCond_destroy(&state->worker_cond);
	EhsHCond_destroy(&state->worker_done_cond);
}

/* Inference worker: loops waiting for frames, runs inference, fires finish events
 * via the saved pFIdata so all EHS_FB_* port macros resolve correctly. */
static EhsThreadFuncReturnType _ml_inference_worker(void* arg)
{
	inx_ml_image_inference_state_type* state = (inx_ml_image_inference_state_type*)arg;

	EhsTPMutex_lock(state->worker_mutex);
	while (1) {
		while (!state->worker_busy && !state->worker_exit)
			EhsTPCondition_wait(state->worker_cond, state->worker_mutex);

		if (state->worker_exit) break;

		ehs_sint32 frame_id = state->pending_frame_id;
		/* Rebind as a local named pFIdata so all EHS_FB_* macros expand correctly */
		EhsFunctionInstanceDataType* pFIdata = (EhsFunctionInstanceDataType*)state->pFIdata;
		EhsTPMutex_unlock(state->worker_mutex);

		/* ---- run inference ---- */
		EhsML_Err err = EHS_ML_OK;

		if (state->ml_ctx.ml_model_ctx == NULL) {
			err = EHS_ML_MODEL_CTX_ERR;
			EHSH_LOG_ERROR("[ml_inference_worker] Model context is NULL");
			goto worker_done;
		}

		if (EHS_FB_OUT_CONNECTED_API2(INX_ml_image_inference_ARG_inference_json)) {
			EhsCameraFrame* frame = EhsCameraFrameGetById(frame_id);
			if (!frame) {
				err = EHS_ML_INVALID_FRAME;
				EHSH_LOG_ERROR("[ml_inference_worker] Invalid frame id %d", frame_id);
				goto worker_done;
			}
			void* frame_data = NULL;
			ehs_uint32 frame_size = 0;
			if (EHS_FALSE == EhsCameraFrameGetData(frame, &frame_data, &frame_size)) {
				err = EHS_ML_INVALID_FRAME;
				EHSH_LOG_ERROR("[ml_inference_worker] Failed to get frame data");
				goto worker_done;
			}
			EhsCameraFrameEnsureCPU(frame);
			err = EhsML_SetInputData(&state->ml_ctx, frame_data, frame_size);
			if (err != EHS_ML_OK) {
				EHSH_LOG_ERROR("[ml_inference_worker] EhsML_SetInputData failed: %d", err);
				goto worker_done;
			}
			err = EhsML_RunOutputJson(&state->ml_ctx,
				EHS_FB_OUT_S_API2(INX_ml_image_inference_ARG_inference_json),
				EHS_STRING_LENGTH_MAX);
			if (err != EHS_ML_OK) {
				EHSH_LOG_ERROR("[ml_inference_worker] EhsML_RunOutputJson failed: %d", err);
				goto worker_done;
			}
		}

	worker_done:
		if (err == EHS_ML_OK) {
			if (EHS_FB_OUT_CONNECTED_API2(INX_ml_image_inference_ARG_inference_inference_errno))
				EHS_FB_OUT_I_API2(INX_ml_image_inference_ARG_inference_inference_errno) = 0;
			EHS_FB_FINISH(INX_ml_image_inference_ARG_inference_inference_done);
		} else {
			if (EHS_FB_OUT_CONNECTED_API2(INX_ml_image_inference_ARG_inference_inference_errno))
				EHS_FB_OUT_I_API2(INX_ml_image_inference_ARG_inference_inference_errno) = err;
			EHS_FB_FINISH(INX_ml_image_inference_ARG_inference_inference_error);
		}

		EhsTPMutex_lock(state->worker_mutex);
		state->worker_busy = 0;
	}
	/* Signal that we have exited before releasing the mutex */
	state->worker_done = 1;
	EhsTPCondition_signal(state->worker_done_cond);
	EhsTPMutex_unlock(state->worker_mutex);
	return 0;
}

/* -----------------------------------------------------------------------
 * Load worker thread
 * ----------------------------------------------------------------------- */

/* One-shot thread that performs the slow parts of model loading:
 * stopping the old inference worker, calling EhsML_Create, spawning
 * a fresh inference worker, then firing the load finish event. */
static EhsThreadFuncReturnType _ml_load_model_worker(void* arg)
{
	inx_ml_image_inference_state_type* state = (inx_ml_image_inference_state_type*)arg;
	/* Rebind pFIdata locally so EHS_FB_* load-port macros resolve correctly */
	EhsFunctionInstanceDataType* pFIdata = (EhsFunctionInstanceDataType*)state->load_pFIdata;
	EhsML_Err err = EHS_ML_OK;

	/* Stop any existing inference worker before swapping the model */
	_ml_inference_stop_worker(state);
	EhsML_Destroy(&state->ml_ctx);

	err = EhsML_Create(&state->ml_ctx, state->pending_model_path,
		(EhsML_Type)state->Model_Type,
		state->Conf_Thres,
		state->Thread_Number);
	if (err != EHS_ML_OK) {
		EHSH_LOG_ERROR("[ml_load_worker] EhsML_Create failed: %d", err);
		goto load_done;
	}
	state->ml_ctx.enable_flat_json        = state->Flat_Json;
	state->ml_ctx.enable_normalized_coords = state->Normalized_Coords;

	if (EHS_FB_OUT_CONNECTED_API2(INX_ml_image_inference_ARG_load_model_model_info)) {
		EhsML_GetModelInfoJson(
			&state->ml_ctx,
			state->pending_model_path,
			EHS_FB_OUT_S_API2(INX_ml_image_inference_ARG_load_model_model_info),
			EHS_STRING_LENGTH_MAX);
	}

	/* Spawn the inference worker for the newly loaded model */
	EhsHMutex_create(&state->worker_mutex);
	EhsHCond_create(&state->worker_cond);
	EhsHCond_create(&state->worker_done_cond);
	state->worker_busy = 0;
	state->worker_exit = 0;
	state->worker_done = 0;
	if (!EhsHThread_execute(_ml_inference_worker, state, 0, EHS_THREAD_USE_DEFAULT_STACK_SIZE)) {
		err = EHS_ML_INIT_ERR;
		EhsHMutex_destroy(&state->worker_mutex);
		EhsHCond_destroy(&state->worker_cond);
		EhsHCond_destroy(&state->worker_done_cond);
		EHSH_LOG_ERROR("[ml_load_worker] Failed to start inference worker thread");
		goto load_done;
	}
	state->worker_started = 1;

load_done:
	if (err == EHS_ML_OK) {
		if (EHS_FB_OUT_CONNECTED_API2(INX_ml_image_inference_ARG_load_model_load_errno))
			EHS_FB_OUT_I_API2(INX_ml_image_inference_ARG_load_model_load_errno) = 0;
		EHS_FB_FINISH(INX_ml_image_inference_ARG_load_model_load_done);
	} else {
		if (EHS_FB_OUT_CONNECTED_API2(INX_ml_image_inference_ARG_load_model_load_errno))
			EHS_FB_OUT_I_API2(INX_ml_image_inference_ARG_load_model_load_errno) = err;
		EHS_FB_FINISH(INX_ml_image_inference_ARG_load_model_load_err);
	}
	state->load_busy = 0;
	return 0;
}

//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(ml_image_inference)
{
	inx_ml_image_inference_state_type *inx_ml_image_inference_state = (inx_ml_image_inference_state_type*)EHS_FB_DESTROY_CONTEXT;
	/* Stop inference worker before destroying the ML context it may be using */
	_ml_inference_stop_worker(inx_ml_image_inference_state);
	EhsML_Destroy(&inx_ml_image_inference_state->ml_ctx);
	return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE

/* -----------------------------------------------------------------------
 * Utility helpers
 * ----------------------------------------------------------------------- */

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
				if (EhsStrnicmp(_dot + 1, expected_ext, ext_len) == 0)
					return EHS_TRUE;
			}
			_dot++;
		}
	} while (_dot != NULL);
	if (!dot || dot == file_path) return EHS_FALSE;
	return EHS_FALSE;
}

#define _EHS_ML_IMG_INFERENCE_GOTO_ON_ERROR(error, label, tag, msg)  do { \
		EhsML_Err _err = error;            \
		if (_err != EHS_ML_OK) {            \
			EHSH_LOG_ERROR("[%s] %s", tag, msg); \
			goto label;                    \
		}                                   \
	} while(0)

/* -----------------------------------------------------------------------
 * load_model run function — fast path only; dispatches _ml_load_model_worker
 * ----------------------------------------------------------------------- */
//ICB FUNCTION load_model MACRO START -- DO NOT ALTER
EHS_FB_RUN_FUNCTION(ml_image_inference_load_model)
{
	inx_ml_image_inference_state_type* inx_ml_image_inference_state = (inx_ml_image_inference_state_type*)EHS_FB_RUN_CONTEXT;
	EhsML_Err err = EHS_ML_OK;

	/* Prevent double-load */
	if (inx_ml_image_inference_state->load_busy) {
		if (EHS_FB_OUT_CONNECTED_API2(INX_ml_image_inference_ARG_load_model_load_errno))
			EHS_FB_OUT_I_API2(INX_ml_image_inference_ARG_load_model_load_errno) = EHS_ML_MODEL_IN_USE;
		EHS_FB_FINISH(INX_ml_image_inference_ARG_load_model_load_err);
		return;
	}

	if (EHS_FB_IN_CONNECTED_API2(INX_ml_image_inference_ARG_load_model_model_path))
		EhsStrcpy(inx_ml_image_inference_state->Model_File_Path,
		          EHS_FB_IN_S_API2(INX_ml_image_inference_ARG_load_model_model_path));

	/* Auto-detect format from file extension */
	if (inx_ml_image_inference_state->Model_Format == EHS_ML_FORMAT_START) {
		const ehs_char* p = inx_ml_image_inference_state->Model_File_Path;
		if      (_check_file_extension(p, "tflite") || _check_file_extension(p, "tfl"))
			inx_ml_image_inference_state->Model_Format = EHS_ML_FORMAT_TFLITE;
		else if (_check_file_extension(p, "onnx")   || _check_file_extension(p, "onn"))
			inx_ml_image_inference_state->Model_Format = EHS_ML_FORMAT_ONNX;
		else if (_check_file_extension(p, "pb"))
			inx_ml_image_inference_state->Model_Format = EHS_ML_FORMAT_PB;
		else if (_check_file_extension(p, "hef"))
			inx_ml_image_inference_state->Model_Format = EHS_ML_FORMAT_HEF;
	}

	/* Validate extension */
	switch ((EhsML_ModelFormat_t)inx_ml_image_inference_state->Model_Format) {
		case EHS_ML_FORMAT_TFLITE:
			err = (_check_file_extension(inx_ml_image_inference_state->Model_File_Path, "tflite") ||
			       _check_file_extension(inx_ml_image_inference_state->Model_File_Path, "tfl"))
			      ? EHS_ML_OK : EHS_ML_MODEL_NAME_ERR;
			break;
		case EHS_ML_FORMAT_ONNX:
			err = (_check_file_extension(inx_ml_image_inference_state->Model_File_Path, "onnx") ||
			       _check_file_extension(inx_ml_image_inference_state->Model_File_Path, "onn"))
			      ? EHS_ML_OK : EHS_ML_MODEL_NAME_ERR;
			break;
		case EHS_ML_FORMAT_PB:
			err = _check_file_extension(inx_ml_image_inference_state->Model_File_Path, "pb")
			      ? EHS_ML_OK : EHS_ML_MODEL_NAME_ERR;
			break;
		case EHS_ML_FORMAT_HEF:
			err = _check_file_extension(inx_ml_image_inference_state->Model_File_Path, "hef")
			      ? EHS_ML_OK : EHS_ML_MODEL_NAME_ERR;
			if (err == EHS_ML_OK) {
				if (inx_ml_image_inference_state->HW_Accelerate == EHS_FALSE)
					err = EHS_ML_MODEL_TYPE_ERR;
				else if (EhsML_HWAccel_supported() != EHS_ML_HWACCEL_HAILO)
					err = EHS_ML_INVALID_DEP;
			}
			break;
		default:
			err = EHS_ML_MODEL_NAME_ERR;
			break;
	}
	_EHS_ML_IMG_INFERENCE_GOTO_ON_ERROR(err, load_err, __func__, "Model file extension does not match!");

	/* Resolve canonical path (fast — local filesystem stat) */
	{
		ehs_char szCanonicalFilePath[EHS_MAXPATHLENGTH] = { '\0' };
		if (inx_ml_image_inference_state->Use_Application_Dir == EHS_TRUE) {
			if (EhsHMetagetCurrentAppDir(szCanonicalFilePath)) {
				EhsStrcat(szCanonicalFilePath, EHS_TD_FILES_SEPARATOR_STR);
				EhsStrcat(szCanonicalFilePath, inx_ml_image_inference_state->Model_File_Path);
			} else {
				err = EHS_ML_MODEL_PATH_ERR;
			}
		} else {
			if (EhsTF_tryCanonicPath(szCanonicalFilePath, EHS_RUNTIME_USERDATA_DIR,
			                         inx_ml_image_inference_state->Model_File_Path, EHS_TRUE) == EHS_FALSE)
				err = EHS_ML_MODEL_PATH_ERR;
		}
		_EHS_ML_IMG_INFERENCE_GOTO_ON_ERROR(err, load_err, __func__, "Model file does not exist!");

		/* Store canonical path and dispatch the load thread */
		EhsStrcpy(inx_ml_image_inference_state->pending_model_path, szCanonicalFilePath);
	}

	inx_ml_image_inference_state->load_pFIdata = (void*)pFIdata;
	inx_ml_image_inference_state->load_busy    = 1;
	if (!EhsHThread_execute(_ml_load_model_worker, inx_ml_image_inference_state,
	                         0, EHS_THREAD_USE_DEFAULT_STACK_SIZE)) {
		inx_ml_image_inference_state->load_busy = 0;
		err = EHS_ML_INIT_ERR;
		_EHS_ML_IMG_INFERENCE_GOTO_ON_ERROR(err, load_err, __func__, "Failed to start load thread!");
	}
	return; /* load thread fires the finish event */

load_err:
	if (EHS_FB_OUT_CONNECTED_API2(INX_ml_image_inference_ARG_load_model_load_errno))
		EHS_FB_OUT_I_API2(INX_ml_image_inference_ARG_load_model_load_errno) = err;
	EHS_FB_FINISH(INX_ml_image_inference_ARG_load_model_load_err);
}//ICB FUNCTION load_model MACRO END -- DO NOT ALTER THIS LINE

/* -----------------------------------------------------------------------
 * inference run function — queues frame for worker, returns immediately
 * ----------------------------------------------------------------------- */
//ICB FUNCTION inference MACRO START -- DO NOT ALTER
/**
 * Queues a frame for the inference worker thread and returns immediately.
 * The worker fires inference_done or inference_error when complete.
 * If the worker is still busy, fires inference_error with EHS_ML_BUSY (errno=27).
 */
EHS_FB_RUN_FUNCTION(ml_image_inference_inference)
{
	inx_ml_image_inference_state_type* inx_ml_image_inference_state = (inx_ml_image_inference_state_type*)EHS_FB_RUN_CONTEXT;
	EhsML_Err err = EHS_ML_OK;
	ehs_sint32 frame_id = -1;

	if (!inx_ml_image_inference_state->worker_started) {
		err = EHS_ML_MODEL_CTX_ERR;
		EHSH_LOG_ERROR("[%s] Inference called before model was loaded", __func__);
		goto error;
	}

	if (EHS_FB_IN_CONNECTED_API2(INX_ml_image_inference_ARG_inference_stream_id))
		frame_id = EHS_FB_IN_I_API2(INX_ml_image_inference_ARG_inference_stream_id);
	if (frame_id < 0) {
		err = EHS_ML_INVALID_FRAME_ID;
		EHSH_LOG_ERROR("[%s] Invalid stream ID", __func__);
		goto error;
	}

	EhsTPMutex_lock(inx_ml_image_inference_state->worker_mutex);
	if (inx_ml_image_inference_state->worker_busy) {
		EhsTPMutex_unlock(inx_ml_image_inference_state->worker_mutex);
		err = EHS_ML_BUSY;
		goto error;
	}
	inx_ml_image_inference_state->pending_frame_id = frame_id;
	inx_ml_image_inference_state->pFIdata          = (void*)pFIdata;
	inx_ml_image_inference_state->worker_busy      = 1;
	EhsTPCondition_signal(inx_ml_image_inference_state->worker_cond);
	EhsTPMutex_unlock(inx_ml_image_inference_state->worker_mutex);
	return; /* worker fires the finish event */

error:
	if (EHS_FB_OUT_CONNECTED_API2(INX_ml_image_inference_ARG_inference_inference_errno))
		EHS_FB_OUT_I_API2(INX_ml_image_inference_ARG_inference_inference_errno) = err;
	EHS_FB_FINISH(INX_ml_image_inference_ARG_inference_inference_error);
}//ICB FUNCTION inference MACRO END -- DO NOT ALTER THIS LINE
