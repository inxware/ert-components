/***************************************************************
* Copyright (C) 2008-2025 inx limited - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/** @file ml_inf_engine.h
 * Inference engine dispatch layer.
 *
 * This layer sits between the model post-processing layer and the individual
 * inference engine backends (TFLite, HailoRT, TensorRT, ...).
 *
 * It selects the correct engine backend at runtime based on ctx->hw_accel
 * (which is set from EhsML_HWAccel_supported() during Create).  All model
 * implementations share these four functions for lifecycle and I/O — only the
 * RunOutputJson stage is model-specific, because that is where the raw engine
 * tensor output is decoded into application data.
 *
 * @author: inx limited
 */

#ifndef _C_HEADER_EHS_TARGET_HAL_ML_ENGINE_ML_INF_ENGINE_H_
#define _C_HEADER_EHS_TARGET_HAL_ML_ENGINE_ML_INF_ENGINE_H_

#include "hal_ml.h"
#include "hal-api.h"

/**
 * @brief Create and initialise the inference engine for this context.
 *
 * Detects which hardware acceleration is available, selects the matching
 * backend (TFLite / HailoRT / TensorRT / …) and delegates creation to it.
 * Falls back to TFLite if the preferred hardware backend is absent at build
 * time (and TFLite is built in).  Returns EHS_ML_NOT_SUPPORTED if no usable
 * backend is compiled in.
 */
EhsML_Err EhsML_InfEngine_Create(EhsML_Context* ctx, const ehs_char* model_path, EhsML_Type model_type, ehs_float conf_thres, ehs_sint32 thread_count);

/**
 * @brief Destroy the inference engine and free its resources.
 */
void EhsML_InfEngine_Destroy(EhsML_Context* ctx);

/**
 * @brief Feed an input buffer into the inference engine.
 *
 * Routes the call to the active backend's SetInputData function.
 */
EhsML_Err EhsML_InfEngine_SetInputData(EhsML_Context* ctx, const void* input_data, ehs_uint32 data_size);

/**
 * @brief Execute inference and populate ctx->output_tensor[].
 *
 * Runs one inference pass on the previously set input data and places the
 * raw engine output into ctx->output_tensor[].  Does NOT perform
 * model-specific post-processing or output serialisation — those are the
 * responsibility of the model layer (RunPipeline) and EhsML_GetOutput().
 */
EhsML_Err EhsML_InfEngine_RunInference(EhsML_Context* ctx);


#endif /* _C_HEADER_EHS_TARGET_HAL_ML_ENGINE_ML_INF_ENGINE_H_ */
