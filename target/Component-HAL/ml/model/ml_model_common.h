#ifndef _C_HEADER_EHS_TARGET_HAL_ML_MODEL_ML_MODEL_COMMON_H_
#define _C_HEADER_EHS_TARGET_HAL_ML_MODEL_ML_MODEL_COMMON_H_

#include "hal_ml.h"
#include "hal-api.h"

EhsML_Err EhsML_Model_Boilerplate_Create(EhsML_Context* ctx, const ehs_char* model_path, EhsML_Type model_type, ehs_float conf_thres, ehs_sint32 thread_count);

void EhsML_Model_Boilerplate_Destroy(EhsML_Context* ctx);

EhsML_Err EhsML_Model_Boilerplate_SetInputData(EhsML_Context* ctx, const void* input_data, ehs_uint32 data_size);

EhsML_Err EhsML_Model_Boilerplate_RunOutputJson(EhsML_Context* ctx, ehs_char* json_output, ehs_uint32 output_size);


#endif//_C_HEADER_EHS_TARGET_HAL_ML_MODEL_ML_MODEL_COMMON_H_