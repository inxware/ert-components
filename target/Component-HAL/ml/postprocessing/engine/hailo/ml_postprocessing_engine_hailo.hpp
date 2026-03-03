#ifndef _CHEADER_TARGET_COMPONENT_HAL_ML_POSTPROCESSING_ENGINE_HAILO_H_
#define _CHEADER_TARGET_COMPONENT_HAL_ML_POSTPROCESSING_ENGINE_HAILO_H_

#include "hal_ml.h"
#include "hal-api.h"


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Post-processing engine implementation for Hailo platform.
 * This is the middle layer between the engine raw output and the general dequantisation logic.
 * It takes the output tensor from the engine and processes it to the expected format for the general dequantisation logic to work with.
 * This is where the model-specific logic for processing the output tensors should be implemented.
 * 
 */
ehs_uint32 EhsML_Postprocessing_Engine_Hailo_ProcessOutput(EhsML_Context* ctx);

#ifdef __cplusplus
}
#endif

#endif // _CHEADER_TARGET_COMPONENT_HAL_ML_POSTPROCESSING_ENGINE_HAILO_H_