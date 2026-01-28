#include "ml_model_common.h"

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
            return EhsML_FW_Hailo_Create(ctx, model_path, conf_thres, thread_count);
            break;
        }
        case EHS_ML_HWACCEL_NVIDIA:
        {
            //TODO add support later. Now just fallback to tensorflow lite
            goto ml_hw_fallback;
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

ml_hw_fallback:
    // Fallback to software only framework (i.e. Tensorflow Lite)
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
            EhsML_FW_Hailo_Destroy(ctx);
            break;
        }
        case EHS_ML_HWACCEL_NVIDIA:
        {
            //TODO add support later. Now just fallback to tensorflow lite
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
    if (ctx == NULL || input_data == NULL || data_size == 0) return EHS_ML_FAILED;
    switch (ctx->hw_accel)
    {
        case EHS_ML_HWACCEL_HAILO:
        {
            return EhsML_FW_Hailo_SetInputData(ctx, input_data, data_size);
        }
        case EHS_ML_HWACCEL_NVIDIA:
        {
            //TODO add support later. Now just fallback to tensorflow lite
            return EHS_ML_NOT_SUPPORTED;
        }
        case EHS_ML_HWACCEL_AMD:
        {
            //TODO add support later. Now just fallback to tensorflow lite
            return EHS_ML_NOT_SUPPORTED;
        }
        case EHS_ML_HWACCEL_CUSTOM_NPU:
        {
            //TODO add support later. Now just fallback to tensorflow lite
        }
        case EHS_ML_HWACCEL_NONE:
        default:
        {
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
            err = EhsML_FW_Hailo_GetOutputData(ctx);
        }
        case EHS_ML_HWACCEL_NVIDIA:
        {
            //TODO add support later. Now just fallback to tensorflow lite
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