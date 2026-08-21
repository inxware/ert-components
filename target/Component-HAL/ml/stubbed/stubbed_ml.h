#ifndef _C_HEADER_EHS_TARGET_HAL_ML_STUBBED_STUBBED_ML_H_
#define _C_HEADER_EHS_TARGET_HAL_ML_STUBBED_STUBBED_ML_H_

#include "hal_ml.h"

EhsML_Err EhsML_Stubbed_Create(EhsML_Context* ctx, const ehs_char* model_path, EhsML_Type model_type, ehs_float conf_thres, ehs_sint32 thread_count);
void EhsML_Stubbed_Destroy(EhsML_Context* ctx);
EhsML_Err EhsML_Stubbed_SetInputData(EhsML_Context* ctx, const void* data, ehs_uint32 size);
EhsML_Err EhsML_Stubbed_RunPipeline(EhsML_Context* ctx);
EhsML_Err EhsML_Stubbed_GetOutput(EhsML_Context* ctx, ehs_char* buf, ehs_uint32 size);
/** @deprecated Use EhsML_Stubbed_RunPipeline / EhsML_Stubbed_GetOutput */
EhsML_Err EhsML_Stubbed_RunOutputJson(EhsML_Context* ctx, ehs_char* json, ehs_uint32 size);

#endif//_C_HEADER_EHS_TARGET_HAL_ML_STUBBED_STUBBED_ML_H_
