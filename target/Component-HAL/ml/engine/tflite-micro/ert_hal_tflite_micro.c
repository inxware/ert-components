#include "ert_hal_tflite_micro.h"

EhsML_Err EhsML_FW_TFLiteMicro_Create(EhsML_Context* ctx, const ehs_char* model_path, ehs_float conf_thres, ehs_sint32 thread_count)
{
    return EHS_ML_OK;
}

void EhsML_FW_TFLiteMicro_Destroy(EhsML_Context* ctx)
{
    return;
}

EhsML_Err EhsML_FW_TFLiteMicro_SetInputData(EhsML_Context* ctx, const void* input_data, ehs_uint32 data_size)
{
    return EHS_ML_OK;
}

EhsML_Err EhsML_FW_TFLiteMicro_GetOutputData(EhsML_Context* ctx)
{
    return EHS_ML_OK;
}