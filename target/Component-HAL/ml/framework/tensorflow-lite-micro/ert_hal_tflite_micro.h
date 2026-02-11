#ifndef _C_HEADER_EHS_TARGET_HAL_ML_FRAMEWORK_TENSORFLOW_LITE_MICRO_ERT_HAL_TFLITE_MICRO_H_
#define _C_HEADER_EHS_TARGET_HAL_ML_FRAMEWORK_TENSORFLOW_LITE_MICRO_ERT_HAL_TFLITE_MICRO_H_
#error "TFLite Micro is not implemented yet!"

#include "hal_ml.h"

#ifdef __cplusplus
extern "C" {
#endif

EhsML_Err EhsML_FW_TFLiteMicro_Create(EhsML_Context* ctx, const ehs_char* model_path, ehs_float conf_thres, ehs_sint32 thread_count);

void EhsML_FW_TFLiteMicro_Destroy(EhsML_Context* ctx);

EhsML_Err EhsML_FW_TFLiteMicro_SetInputData(EhsML_Context* ctx, const void* input_data, ehs_uint32 data_size);

EhsML_Err EhsML_FW_TFLiteMicro_GetOutputData(EhsML_Context* ctx);

#ifdef __cplusplus
}
#endif

#endif//_C_HEADER_EHS_TARGET_HAL_ML_FRAMEWORK_TENSORFLOW_LITE_MICRO_ERT_HAL_TFLITE_MICRO_H_