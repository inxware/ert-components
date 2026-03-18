/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/**
 * @file ert_hal_tensorrt.h
 * @brief TensorRT ML framework HAL — C-compatible interface.
 *
 * Mirrors the interface of ert_hal_hailo.h / ert_hal_tflite.h so that
 * ml_model_common.c can dispatch to TensorRT with the same call pattern.
 *
 * Model loading
 * -------------
 * EhsML_FW_TensorRT_Create() accepts a path to a serialized TensorRT engine
 * file (.engine).  A TensorRT engine is platform- and GPU-specific and must
 * be built with trtexec on the target Jetson before deployment:
 *
 *   trtexec --onnx=model.onnx --saveEngine=model.engine --fp16
 *
 * The function reads, deserializes, and activates the engine.  Input and
 * output binding sizes are extracted from the engine and stored in the
 * EhsML_Context tensor descriptors so that the post-processing layer can
 * work without knowing implementation details.
 *
 * Inference flow
 * --------------
 *   EhsML_FW_TensorRT_SetInputData()  — copy host input into device buffer
 *   EhsML_FW_TensorRT_GetOutputData() — run inference; copy device output
 *                                        to host; fill ctx->output_tensor[0]
 *
 * The device buffers are allocated once at Create() time and reused across
 * inference calls.
 */

#ifndef ERT_HAL_TENSORRT_H
#define ERT_HAL_TENSORRT_H

#include "hal_ml.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Load and activate a TensorRT engine.
 *
 * @param ctx         Initialised (zeroed) EhsML_Context.
 * @param model_path  Path to the serialised .engine file.
 * @param conf_thres  Confidence threshold stored in ctx (used by post-processing).
 * @param thread_count Unused for TensorRT (inference runs on GPU).
 * @return EHS_ML_OK on success, or an EhsML_Err error code.
 */
EhsML_Err EhsML_FW_TensorRT_Create(EhsML_Context *ctx,
                                    const ehs_char *model_path,
                                    ehs_float conf_thres,
                                    ehs_sint32 thread_count);

/**
 * Destroy the TensorRT engine and free all CUDA resources.
 * Safe to call with a partially-initialised context.
 */
void EhsML_FW_TensorRT_Destroy(EhsML_Context *ctx);

/**
 * Copy host input data to the CUDA device input buffer.
 *
 * @param ctx        Active EhsML_Context (EhsML_FW_TensorRT_Create succeeded).
 * @param input_data Pointer to the host-side input buffer (e.g. float* NCHW).
 * @param data_size  Size of the input buffer in bytes.  Must match the engine
 *                   input binding size exactly.
 * @return EHS_ML_OK, or EHS_ML_INPUT_SIZE_MISMATCH_ERR / EHS_ML_NULL_INPUT_ERR.
 */
EhsML_Err EhsML_FW_TensorRT_SetInputData(EhsML_Context *ctx,
                                          const void *input_data,
                                          ehs_uint32 data_size);

/**
 * Execute inference and copy the output tensor to host memory.
 *
 * On success ctx->output_tensor[0].data_ptr.ptr points to a float buffer
 * holding the raw engine output.  The buffer is owned by the TensorRT
 * model context (data_ptr_owned = EHS_FALSE); callers must not free it.
 *
 * @param ctx  Active EhsML_Context with input data already set.
 * @return EHS_ML_OK on success, EHS_ML_INFERENCE_ERR on engine failure.
 */
EhsML_Err EhsML_FW_TensorRT_GetOutputData(EhsML_Context *ctx);

/**
 * Write a JSON string describing the loaded TensorRT engine to json_buf.
 *
 * Output matches the structure produced by EhsML_TFLite_GetModelInfoJson:
 *   {"runtime":"TensorRT","input_count":1,"output_count":1,
 *    "inputs":[{"name":"...","dtype":"float32","shape":[1,3,640,640],"bytes":N}],
 *    "outputs":[{"name":"...","dtype":"float32","shape":[1,25200,85],"bytes":N}]}
 *
 * @param ctx        Active EhsML_Context (EhsML_FW_TensorRT_Create succeeded).
 * @param model_path Unused (kept for API symmetry with TFLite variant).
 * @param json_buf   Output buffer.
 * @param json_size  Size of output buffer in bytes.
 * @return EHS_ML_OK on success, EHS_ML_FAILED if buffer too small.
 */
EhsML_Err EhsML_FW_TensorRT_GetModelInfoJson(EhsML_Context *ctx,
                                              const ehs_char *model_path,
                                              ehs_char *json_buf,
                                              ehs_uint32 json_size);

#ifdef __cplusplus
}
#endif

#endif /* ERT_HAL_TENSORRT_H */
