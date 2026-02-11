#ifndef _C_HEADER_EHS_TARGET_HAL_ML_FRAMEWORK_TENSORFLOW_LITE_ERT_HAL_HAILO_H_
#define _C_HEADER_EHS_TARGET_HAL_ML_FRAMEWORK_TENSORFLOW_LITE_ERT_HAL_HAILO_H_

#include "hal_ml.h"

#ifdef __cplusplus
extern "C" {
#endif

EhsML_Err EhsML_FW_Hailo_Create(EhsML_Context * ctx, const ehs_char * model_path, ehs_float conf_thres, ehs_sint32 thread_count);

void EhsML_FW_Hailo_Destroy(EhsML_Context * ctx);

EhsML_Err EhsML_FW_Hailo_SetInputData(EhsML_Context * ctx, const void * input_data, ehs_uint32 data_size);

EhsML_Err EhsML_FW_Hailo_GetOutputData(EhsML_Context * ctx);

#ifdef __cplusplus
}
#endif

#endif//_C_HEADER_EHS_TARGET_HAL_ML_FRAMEWORK_TENSORFLOW_LITE_ERT_HAL_HAILO_H_