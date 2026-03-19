/***************************************************************
* Copyright (C) 2008-2025 inx limited - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/** @file ml_inf_engine.c
 * Inference engine dispatch layer.
 *
 * Routes Create / Destroy / SetInputData / RunInference calls to the correct
 * hardware-specific backend based on ctx->hw_accel.  This layer is shared by
 * all model implementations — model-specific post-processing happens in the
 * caller (postprocessing/model/) AFTER RunInference returns.
 *
 * Backend selection:
 *   EHS_ML_HWACCEL_HAILO   → HailoRT  (requires EHS_ML_HWACCEL_SUPPORT_HAILO)
 *   EHS_ML_HWACCEL_NVIDIA  → TensorRT (requires EHS_ML_HWACCEL_SUPPORT_NVIDIA)
 *   EHS_ML_HWACCEL_AMD     → TODO (falls back to TFLite)
 *   EHS_ML_HWACCEL_NONE    → TFLite   (requires EHS_ML_HWACCEL_SUPPORT_TFLITE)
 *
 * @author: inx limited
 */

#include "ml_inf_engine.h"

#ifdef EHS_ML_HWACCEL_SUPPORT_HAILO
#include "ert_hal_hailo.h"
#endif
#ifdef EHS_ML_HWACCEL_SUPPORT_NVIDIA
#include "ert_hal_tensorrt.h"
#endif
#ifdef EHS_ML_HWACCEL_SUPPORT_TFLITE
#include "ert_hal_tflite.h"
#endif

/* Returns EHS_ML_NOT_SUPPORTED if no usable backend is compiled in for the
 * detected hardware.  The fallback to TFLite is valid only if the model was
 * exported in .tflite format — caller is responsible for model/backend
 * compatibility. */

EhsML_Err EhsML_InfEngine_Create(EhsML_Context* ctx, const ehs_char* model_path, EhsML_Type model_type, ehs_float conf_thres, ehs_sint32 thread_count)
{
    EhsML_Err err = EHS_ML_FAILED;

    if (!ctx) goto ml_fail;

    if (ctx->ml_model_ctx) {
        err = EHS_ML_MODEL_IN_USE;
        goto ml_fail;
    }

    if (!model_path || *model_path == '\0') {
        err = EHS_ML_MODEL_PATH_ERR;
        goto ml_fail;
    }

    ctx->hw_accel = EhsML_HWAccel_supported();

    switch (ctx->hw_accel)
    {
        case EHS_ML_HWACCEL_HAILO:
        {
        #ifdef EHS_ML_HWACCEL_SUPPORT_HAILO
            err = EhsML_FW_Hailo_Create(ctx, model_path, conf_thres, thread_count);
            break;
        #else
            return EHS_ML_NOT_SUPPORTED;
        #endif
        }
        case EHS_ML_HWACCEL_NVIDIA:
        {
        #ifdef EHS_ML_HWACCEL_SUPPORT_NVIDIA
            err = EhsML_FW_TensorRT_Create(ctx, model_path, conf_thres, thread_count);
            break;
        #else
            goto ml_hw_fallback;
        #endif
        }
        case EHS_ML_HWACCEL_AMD:
        {
            /* TODO: add AMD ROCm/MIGraphX support.  Fall back to TFLite for now. */
            goto ml_hw_fallback;
        }
        case EHS_ML_HWACCEL_CUSTOM_NPU:
        {
            /* TODO: add custom NPU support.  Fall back to TFLite for now. */
            goto ml_hw_fallback;
        }
        case EHS_ML_HWACCEL_NONE:
        default:
        {
            goto ml_hw_fallback;
        }
    }

    /* FORMAT is always handled by the eRT model post-processing layer
     * (EhsML_ObjDet_Json_FromDetections or equivalent), regardless of engine. */
    if (err == EHS_ML_OK)
        EHS_ML_STAGE_SET(ctx->pipeline, EHS_ML_STAGE_FORMAT, EHS_ML_TECH_ERT_MODEL);

    return err;

ml_fail:
    return err;

/* Fallback: use TFLite CPU runtime.  Only valid if the model is a .tflite
 * file — hardware-compiled models (HEF, TensorRT engine) cannot fall back. */
ml_hw_fallback:
    ctx->hw_accel = EHS_ML_HWACCEL_NONE;
#ifdef EHS_ML_HWACCEL_SUPPORT_TFLITE
    err = EhsML_FW_TFLite_Create(ctx, model_path, conf_thres, thread_count);
    if (err == EHS_ML_OK)
        EHS_ML_STAGE_SET(ctx->pipeline, EHS_ML_STAGE_FORMAT, EHS_ML_TECH_ERT_MODEL);
    return err;
#else
    return EHS_ML_NOT_SUPPORTED;
#endif
}

void EhsML_InfEngine_Destroy(EhsML_Context* ctx)
{
    if (ctx == NULL) return;
    switch (ctx->hw_accel) {
        case EHS_ML_HWACCEL_HAILO:
        {
        #ifdef EHS_ML_HWACCEL_SUPPORT_HAILO
            EhsML_FW_Hailo_Destroy(ctx);
        #endif
            break;
        }
        case EHS_ML_HWACCEL_NVIDIA:
        {
        #ifdef EHS_ML_HWACCEL_SUPPORT_NVIDIA
            EhsML_FW_TensorRT_Destroy(ctx);
        #endif
            break;
        }
        case EHS_ML_HWACCEL_AMD:
        {
            /* TODO */
            break;
        }
        case EHS_ML_HWACCEL_CUSTOM_NPU:
        {
            /* TODO */
            break;
        }
        case EHS_ML_HWACCEL_NONE:
        default:
        {
        #ifdef EHS_ML_HWACCEL_SUPPORT_TFLITE
            EhsML_FW_TFLite_Destroy(ctx);
        #endif
            break;
        }
    }
}

EhsML_Err EhsML_InfEngine_SetInputData(EhsML_Context* ctx, const void* input_data, ehs_uint32 data_size)
{
    if (ctx == NULL) return EHS_ML_NULL_CTX_ERR;
    if (input_data == NULL) return EHS_ML_NULL_INPUT_ERR;
    if (data_size == 0) return EHS_ML_INVALID_SIZE_ERR;
    switch (ctx->hw_accel)
    {
        case EHS_ML_HWACCEL_HAILO:
        {
        #ifdef EHS_ML_HWACCEL_SUPPORT_HAILO
            return EhsML_FW_Hailo_SetInputData(ctx, input_data, data_size);
        #else
            return EHS_ML_NOT_SUPPORTED;
        #endif
            break;
        }
        case EHS_ML_HWACCEL_NVIDIA:
        {
        #ifdef EHS_ML_HWACCEL_SUPPORT_NVIDIA
            return EhsML_FW_TensorRT_SetInputData(ctx, input_data, data_size);
        #else
            return EHS_ML_NOT_SUPPORTED;
        #endif
            break;
        }
        case EHS_ML_HWACCEL_AMD:
        {
            /* TODO */
            return EHS_ML_NOT_SUPPORTED;
            break;
        }
        case EHS_ML_HWACCEL_CUSTOM_NPU:
        {
            /* TODO */
            return EHS_ML_NOT_SUPPORTED;
            break;
        }
        case EHS_ML_HWACCEL_NONE:
        default:
        {
        #ifdef EHS_ML_HWACCEL_SUPPORT_TFLITE
            return EhsML_FW_TFLite_SetInputData(ctx, input_data, data_size);
        #else
            return EHS_ML_NOT_SUPPORTED;
        #endif
        }
    }
}

EhsML_Err EhsML_InfEngine_RunInference(EhsML_Context* ctx)
{
    if (ctx == NULL) return EHS_ML_NULL_CTX_ERR;
    EhsML_Err err = EHS_ML_FAILED;

    switch (ctx->hw_accel)
    {
        case EHS_ML_HWACCEL_HAILO:
        {
        #ifdef EHS_ML_HWACCEL_SUPPORT_HAILO
            err = EhsML_FW_Hailo_GetOutputData(ctx);
        #else
            err = EHS_ML_NOT_SUPPORTED;
        #endif
            break;
        }
        case EHS_ML_HWACCEL_NVIDIA:
        {
        #ifdef EHS_ML_HWACCEL_SUPPORT_NVIDIA
            err = EhsML_FW_TensorRT_GetOutputData(ctx);
        #else
            err = EHS_ML_NOT_SUPPORTED;
        #endif
            break;
        }
        case EHS_ML_HWACCEL_AMD:
        {
            /* TODO */
            err = EHS_ML_NOT_SUPPORTED;
            break;
        }
        case EHS_ML_HWACCEL_CUSTOM_NPU:
        {
            /* TODO */
            err = EHS_ML_NOT_SUPPORTED;
            break;
        }
        case EHS_ML_HWACCEL_NONE:
        default:
        {
        #ifdef EHS_ML_HWACCEL_SUPPORT_TFLITE
            err = EhsML_FW_TFLite_GetOutputData(ctx);
        #else
            err = EHS_ML_NOT_SUPPORTED;
        #endif
            break;
        }
    }
    return err;
}
