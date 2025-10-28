
#include "hal_ml.h"


EhsML_Err EhsML_Create(EhsML_Context* ctx, const ehs_char* model_path, EhsML_Type model_type, ehs_float conf_thres, ehs_sint32 thread_count)
{
    return EHS_ML_OK;
}

void EhsML_Destroy(EhsML_Context* ctx)
{
}

EhsML_Err EhsML_SetInputData(EhsML_Context* ctx, const void* data, ehs_uint32 size)
{
    return EHS_ML_OK;
}

EhsML_Err EhsML_RunOutputJson(EhsML_Context* ctx, ehs_char* json, ehs_uint32 size)
{
    return EHS_ML_OK;
}