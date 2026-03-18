#include "ml_model_common.h"

#ifdef EHS_ML_HWACCEL_SUPPORT_HAILO
#include "ert_hal_hailo.h"
#endif
#ifdef EHS_ML_HWACCEL_SUPPORT_NVIDIA
#include "ert_hal_tensorrt.h"
#endif
#ifdef EHS_ML_HWACCEL_SUPPORT_TFLITE
#include "ert_hal_tflite.h"
#endif

/* Returns with EHS_ML_NOT_SUPPORTED if th ctx model passed in is not supported in the hardware */

EhsML_Err EhsML_Model_Boilerplate_Create(EhsML_Context* ctx, const ehs_char* model_path, EhsML_Type model_type, ehs_float conf_thres, ehs_sint32 thread_count)
{
    // Your code here
    EhsML_Err err = EHS_ML_FAILED;

    if (!ctx) goto ml_fail;

    if(ctx->ml_model_ctx){
        err = EHS_ML_MODEL_IN_USE;
        goto ml_fail;
    }

    if(!model_path || *model_path == '\0'){
        err = EHS_ML_MODEL_PATH_ERR;
        goto ml_fail;
    }
    // Get whether hardware is enabled
    // Check whether hardware support is enabled
    ctx->hw_accel = EhsML_HWAccel_supported();

    switch (ctx->hw_accel)
    {
        case EHS_ML_HWACCEL_HAILO:
        {
        #ifdef EHS_ML_HWACCEL_SUPPORT_HAILO
            return EhsML_FW_Hailo_Create(ctx, model_path, conf_thres, thread_count);
        #else
            return EHS_ML_NOT_SUPPORTED;
        #endif//EHS_ML_HWACCEL_SUPPORT_HAILO
            break;
        }
        case EHS_ML_HWACCEL_NVIDIA:
        {
        #ifdef EHS_ML_HWACCEL_SUPPORT_NVIDIA
            return EhsML_FW_TensorRT_Create(ctx, model_path, conf_thres, thread_count);
        #else
            goto ml_hw_fallback;
        #endif
            break;
        }
        case EHS_ML_HWACCEL_AMD:
        {
            //TODO add support later. Now just fallback to tensorflow lite
            goto ml_hw_fallback;
            break;
        }
        case EHS_ML_HWACCEL_CUSTOM_NPU:
        {
            //TODO add support later. Now just fallback to tensorflow lite
            goto ml_hw_fallback;
            break;
        }
        case EHS_ML_HWACCEL_NONE:
        default:
        {
            // Fall back to the softwareimplementation if no valid hardware acceleration is declared
            goto ml_hw_fallback;
        }
    }
    // Get what hardware support is available for current platform
    // Run the correct creation function and return its error code
    //  If the hardware acceleration is not initialised properly, the software one will be created instead.
    return EHS_ML_OK;
ml_fail:
    return err;

//TODO  I don't think we can fall back to tflite for any model we are parsed  I think this needs to return an error? 
      // Or can we detect if the model is tflit compatible and do this connditionally 
ml_hw_fallback:
    // Fallback to software only framework (i.e. Tensorflow Lite)  really? just like that?
    ctx->hw_accel = EHS_ML_HWACCEL_NONE;
    return EhsML_FW_TFLite_Create(ctx, model_path, conf_thres, thread_count);
}

void EhsML_Model_Boilerplate_Destroy(EhsML_Context* ctx)
{
    // Your code here
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
            //TODO add support later. Now just fallback to tensorflow lite
            break;
        }
        case EHS_ML_HWACCEL_CUSTOM_NPU:
        {
            //TODO add support later. Now just fallback to tensorflow lite
            break;
        }
        case EHS_ML_HWACCEL_NONE:
        default:
        {
            EhsML_FW_TFLite_Destroy(ctx);
            break;
        }
    }
}

EhsML_Err EhsML_Model_Boilerplate_SetInputData(EhsML_Context* ctx, const void* input_data, ehs_uint32 data_size)
{
    // Your code here
    if (ctx == NULL) return EHS_ML_NULL_CTX_ERR;
    if (input_data == NULL) return EHS_ML_NULL_INPUT_ERR;
    if (data_size == 0) return EHS_ML_INVALID_SIZE_ERR;
    printf("[BOILER_DBG] SetInputData: hw_accel=%d, data_size=%u\n", (int)ctx->hw_accel, (unsigned)data_size);
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
            //TODO add support later. Now just fallback to tensorflow lite
            return EHS_ML_NOT_SUPPORTED;
            break;
        }
        case EHS_ML_HWACCEL_CUSTOM_NPU:
        {
            //TODO add support later. Now just fallback to tensorflow lite
            return EHS_ML_NOT_SUPPORTED;
            break;
        }
        case EHS_ML_HWACCEL_NONE:
        default:
        {
            /* Are we sure we can just fall back to tflite for any model type?*/
            return EhsML_FW_TFLite_SetInputData(ctx, input_data, data_size);
        }
    }
}

EhsML_Err EhsML_Model_Boilerplate_RunOutputJson(EhsML_Context* ctx, ehs_char* json_output, ehs_uint32 output_size)
{
    // Your code here
    /*TODO The original Hailo code is Yolov8*/
    if (ctx == NULL || json_output == NULL || output_size == 0) return EHS_ML_FAILED;
    EhsML_Err err = EHS_ML_FAILED;
    /* 1. Get the output into the general tensor in the context object */
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
            //TODO add support later. Now just fallback to tensorflow lite
            break;
        }
        case EHS_ML_HWACCEL_CUSTOM_NPU:
        {
            //TODO add support later. Now just fallback to tensorflow lite
            break;
        }
        case EHS_ML_HWACCEL_NONE:
        default:
        {
            err = EhsML_FW_TFLite_GetOutputData(ctx);
            break;
        }
    }
    if (err != EHS_ML_OK) return err;
    return EHS_ML_OK;
}
