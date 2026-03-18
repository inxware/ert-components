/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/**
 * @file ert_hal_tensorrt.cpp
 * @brief TensorRT ML framework HAL implementation.
 *
 * Implements EhsML_FW_TensorRT_Create/Destroy/SetInputData/GetOutputData
 * using the TensorRT C++ API (NvInfer).
 *
 * Only single-input, single-output engines are currently supported.
 * Multi-binding support can be added by iterating getNbIOTensors() / getIOTensorName().
 *
 * Error handling follows the same pattern as ert_hal_tflite.c:
 *   - goto cleanup on error
 *   - EHSH_LOG_ERROR / EHSH_LOG_WARNING for diagnostics
 *   - Return a specific EhsML_Err code
 */

#include "ert_hal_tensorrt.h"
#include "hal-api.h"

#include <NvInfer.h>
#include <cuda_runtime_api.h>

#include <fstream>
#include <memory>
#include <cstring>
#include <cstdio>
#include <cstdlib>

/* -------------------------------------------------------------------------
 * TensorRT logger — routes TRT diagnostics to the EHS log
 * ------------------------------------------------------------------------- */

namespace {

class EhsTRTLogger : public nvinfer1::ILogger
{
public:
    void log(Severity severity, const char *msg) noexcept override
    {
        switch (severity) {
        case Severity::kERROR:
        case Severity::kINTERNAL_ERROR:
            EHSH_LOG_ERROR("TensorRT: %s", msg);
            break;
        case Severity::kWARNING:
            EHSH_LOG_WARNING("TensorRT: %s", msg);
            break;
        default:
            /* kINFO, kVERBOSE: suppress unless debugging */
            break;
        }
    }
};

static EhsTRTLogger s_trt_logger;

/* -------------------------------------------------------------------------
 * Per-model context (stored in ctx->ml_model_ctx)
 * ------------------------------------------------------------------------- */

struct TRTModelCtx {
    nvinfer1::IRuntime          *runtime       = nullptr;
    nvinfer1::ICudaEngine        *engine        = nullptr;
    nvinfer1::IExecutionContext  *exec_ctx      = nullptr;
    cudaStream_t                  stream        = nullptr;

    void  *d_input   = nullptr;  /* CUDA device input  buffer */
    void  *d_output  = nullptr;  /* CUDA device output buffer */
    void  *h_output  = nullptr;  /* Host output buffer (malloc'd, owned here) */
    size_t input_bytes  = 0;
    size_t output_bytes = 0;

    float  conf_thres = 0.0f;

    /* TRT 8.x binding names */
    char   input_name[64];
    char   output_name[64];
};

/* -------------------------------------------------------------------------
 * Helpers
 * ------------------------------------------------------------------------- */

static size_t volume_of(nvinfer1::Dims const &d)
{
    size_t v = 1;
    for (int i = 0; i < d.nbDims; i++) v *= (size_t)d.d[i];
    return v;
}

static EhsML_DataType_t trt_dtype_to_ehs(nvinfer1::DataType dt)
{
    switch (dt) {
    case nvinfer1::DataType::kFLOAT: return EHS_ML_DATATYPE_FP32;
    case nvinfer1::DataType::kHALF:  return EHS_ML_DATATYPE_FP16;
    case nvinfer1::DataType::kINT8:  return EHS_ML_DATATYPE_INT8;
    case nvinfer1::DataType::kINT32: return EHS_ML_DATATYPE_INT32;
    case nvinfer1::DataType::kBOOL:  return EHS_ML_DATATYPE_BOOL;
    default:                          return EHS_ML_DATATYPE_NONE;
    }
}

static size_t trt_dtype_bytes(nvinfer1::DataType dt)
{
    switch (dt) {
    case nvinfer1::DataType::kFLOAT: return 4;
    case nvinfer1::DataType::kHALF:  return 2;
    case nvinfer1::DataType::kINT8:  return 1;
    case nvinfer1::DataType::kINT32: return 4;
    case nvinfer1::DataType::kBOOL:  return 1;
    default:                          return 1;
    }
}

static const char *ehs_dtype_to_str(EhsML_DataType_t dt)
{
    switch (dt) {
    case EHS_ML_DATATYPE_FP32:  return "float32";
    case EHS_ML_DATATYPE_FP16:  return "float16";
    case EHS_ML_DATATYPE_INT8:  return "int8";
    case EHS_ML_DATATYPE_INT32: return "int32";
    case EHS_ML_DATATYPE_BOOL:  return "bool";
    default:                     return "unknown";
    }
}

} /* anonymous namespace */

/* -------------------------------------------------------------------------
 * EhsML_FW_TensorRT_Create
 * ------------------------------------------------------------------------- */

extern "C" EhsML_Err EhsML_FW_TensorRT_Create(EhsML_Context *ctx,
                                               const ehs_char *model_path,
                                               ehs_float conf_thres,
                                               ehs_sint32 /*thread_count*/)
{
    if (!ctx)                              return EHS_ML_NULL_CTX_ERR;
    if (ctx->ml_model_ctx)                 return EHS_ML_MODEL_IN_USE;
    if (!model_path || *model_path == '\0') return EHS_ML_MODEL_PATH_ERR;

    EhsML_Err err = EHS_ML_FAILED;

    /* 1. Read engine file into a byte buffer */
    std::ifstream file(model_path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        EHSH_LOG_ERROR("TensorRT: cannot open engine file: %s", model_path);
        return EHS_ML_MODEL_PATH_ERR;
    }
    std::streamsize engine_size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::unique_ptr<char[]> engine_data(new (std::nothrow) char[engine_size]);
    if (!engine_data) {
        EHSH_LOG_ERROR("TensorRT: out of memory reading engine");
        return EHS_ML_MEMORY_ERR;
    }
    if (!file.read(engine_data.get(), engine_size)) {
        EHSH_LOG_ERROR("TensorRT: error reading engine file: %s", model_path);
        return EHS_ML_MODEL_LOAD_ERR;
    }
    file.close();

    /* 2. Allocate private context */
    TRTModelCtx *trt = new (std::nothrow) TRTModelCtx();
    if (!trt) return EHS_ML_MEMORY_ERR;

    /* 3. Deserialize engine */
    trt->runtime = nvinfer1::createInferRuntime(s_trt_logger);
    if (!trt->runtime) {
        EHSH_LOG_ERROR("TensorRT: createInferRuntime failed");
        err = EHS_ML_INIT_ERR;
        goto trt_fail;
    }

    trt->engine = trt->runtime->deserializeCudaEngine(
        engine_data.get(), static_cast<std::size_t>(engine_size));
    if (!trt->engine) {
        EHSH_LOG_ERROR("TensorRT: deserializeCudaEngine failed for %s", model_path);
        err = EHS_ML_MODEL_LOAD_ERR;
        goto trt_fail;
    }

    /* 4. Create execution context */
    trt->exec_ctx = trt->engine->createExecutionContext();
    if (!trt->exec_ctx) {
        EHSH_LOG_ERROR("TensorRT: createExecutionContext failed");
        err = EHS_ML_MODEL_CTX_ERR;
        goto trt_fail;
    }

    /* 5. Discover bindings (TRT 8.x API) */
    {
        int nb = trt->engine->getNbIOTensors();
        if (nb < 2) {
            EHSH_LOG_ERROR("TensorRT: engine has %d IO tensors, expected >=2", nb);
            err = EHS_ML_MODEL_TENSOR_DIM_ERR;
            goto trt_fail;
        }

        /* Locate first input and first output bindings */
        int in_idx  = -1;
        int out_idx = -1;
        for (int i = 0; i < nb && (in_idx < 0 || out_idx < 0); i++) {
            const char *name = trt->engine->getIOTensorName(i);
            auto mode = trt->engine->getTensorIOMode(name);
            if (mode == nvinfer1::TensorIOMode::kINPUT  && in_idx  < 0) {
                in_idx = i;
                snprintf(trt->input_name,  sizeof(trt->input_name),  "%s", name);
            }
            if (mode == nvinfer1::TensorIOMode::kOUTPUT && out_idx < 0) {
                out_idx = i;
                snprintf(trt->output_name, sizeof(trt->output_name), "%s", name);
            }
        }

        if (in_idx < 0 || out_idx < 0) {
            EHSH_LOG_ERROR("TensorRT: could not identify input/output bindings");
            err = EHS_ML_MODEL_TENSOR_DIM_ERR;
            goto trt_fail;
        }

        /* Input binding */
        nvinfer1::Dims in_dims  = trt->engine->getTensorShape(trt->input_name);
        nvinfer1::DataType in_dt = trt->engine->getTensorDataType(trt->input_name);
        trt->input_bytes  = volume_of(in_dims) * trt_dtype_bytes(in_dt);

        /* Output binding */
        nvinfer1::Dims out_dims  = trt->engine->getTensorShape(trt->output_name);
        nvinfer1::DataType out_dt = trt->engine->getTensorDataType(trt->output_name);
        trt->output_bytes = volume_of(out_dims) * trt_dtype_bytes(out_dt);

        /* Populate EhsML_Context tensor descriptors */
        ctx->input_tensor_count = 1;
        ctx->input_tensor[0].size_in_bytes = (ehs_uint32)trt->input_bytes;
        ctx->input_tensor[0].num_dims      = (ehs_uint32)in_dims.nbDims;
        ctx->input_tensor[0].data_type     = trt_dtype_to_ehs(in_dt);
        for (int d = 0; d < in_dims.nbDims && d < EHS_ML_TENSOR_MAX_DIMS; d++)
            ctx->input_tensor[0].dims[d] = (ehs_uint32)in_dims.d[d];

        ctx->output_tensor_count = 1;
        ctx->output_tensor[0].size_in_bytes = (ehs_uint32)trt->output_bytes;
        ctx->output_tensor[0].num_dims      = (ehs_uint32)out_dims.nbDims;
        ctx->output_tensor[0].data_type     = trt_dtype_to_ehs(out_dt);
        for (int d = 0; d < out_dims.nbDims && d < EHS_ML_TENSOR_MAX_DIMS; d++)
            ctx->output_tensor[0].dims[d] = (ehs_uint32)out_dims.d[d];
        ctx->output_tensor[0].data_ptr_owned = EHS_FALSE; /* owned by TRTModelCtx */
    }

    /* 6. CUDA stream */
    if (cudaStreamCreate(&trt->stream) != cudaSuccess) {
        EHSH_LOG_ERROR("TensorRT: cudaStreamCreate failed");
        err = EHS_ML_INIT_ERR;
        goto trt_fail;
    }

    /* 7. Device buffers */
    if (cudaMalloc(&trt->d_input,  trt->input_bytes)  != cudaSuccess ||
        cudaMalloc(&trt->d_output, trt->output_bytes) != cudaSuccess) {
        EHSH_LOG_ERROR("TensorRT: cudaMalloc failed (in=%zu out=%zu bytes)",
                       trt->input_bytes, trt->output_bytes);
        err = EHS_ML_MEMORY_ERR;
        goto trt_fail;
    }

    /* 8. Host output buffer */
    trt->h_output = malloc(trt->output_bytes);
    if (!trt->h_output) {
        EHSH_LOG_ERROR("TensorRT: host output buffer malloc failed");
        err = EHS_ML_MEMORY_ERR;
        goto trt_fail;
    }

    /* 9. Bind device buffers to execution context (TRT 8.6+ API) */
    if (!trt->exec_ctx->setTensorAddress(trt->input_name,  trt->d_input)  ||
        !trt->exec_ctx->setTensorAddress(trt->output_name, trt->d_output)) {
        EHSH_LOG_ERROR("TensorRT: setTensorAddress failed");
        err = EHS_ML_MODEL_CTX_ERR;
        goto trt_fail;
    }

    trt->conf_thres   = (float)conf_thres;
    ctx->ml_model_ctx = (void *)trt;
    ctx->hw_accel     = EHS_ML_HWACCEL_NVIDIA;
    ctx->conf_thres   = conf_thres;

    EHSH_LOG_INFO("TensorRT: engine loaded: %s  in=%zu out=%zu bytes",
                  model_path, trt->input_bytes, trt->output_bytes);
    return EHS_ML_OK;

trt_fail:
    /* Clean up in reverse order — some pointers may be null */
    if (trt->h_output)  { free(trt->h_output); trt->h_output = nullptr; }
    if (trt->d_output)  { cudaFree(trt->d_output); trt->d_output = nullptr; }
    if (trt->d_input)   { cudaFree(trt->d_input);  trt->d_input  = nullptr; }
    if (trt->stream)    { cudaStreamDestroy(trt->stream); trt->stream = nullptr; }
    if (trt->exec_ctx)  { delete trt->exec_ctx;  trt->exec_ctx = nullptr; }
    if (trt->engine)    { delete trt->engine;    trt->engine   = nullptr; }
    if (trt->runtime)   { delete trt->runtime;   trt->runtime  = nullptr; }
    delete trt;
    return err;
}

/* -------------------------------------------------------------------------
 * EhsML_FW_TensorRT_Destroy
 * ------------------------------------------------------------------------- */

extern "C" void EhsML_FW_TensorRT_Destroy(EhsML_Context *ctx)
{
    if (!ctx || !ctx->ml_model_ctx) return;
    TRTModelCtx *trt = static_cast<TRTModelCtx *>(ctx->ml_model_ctx);

    if (trt->stream)  cudaStreamSynchronize(trt->stream);
    if (trt->h_output) { free(trt->h_output); trt->h_output = nullptr; }
    if (trt->d_output) { cudaFree(trt->d_output); trt->d_output = nullptr; }
    if (trt->d_input)  { cudaFree(trt->d_input);  trt->d_input  = nullptr; }
    if (trt->stream)   { cudaStreamDestroy(trt->stream); trt->stream = nullptr; }
    if (trt->exec_ctx) { delete trt->exec_ctx;  trt->exec_ctx = nullptr; }
    if (trt->engine)   { delete trt->engine;    trt->engine   = nullptr; }
    if (trt->runtime)  { delete trt->runtime;   trt->runtime  = nullptr; }

    delete trt;
    ctx->ml_model_ctx = nullptr;

    /* Clear output tensor data pointer — it pointed into h_output which is freed */
    ctx->output_tensor[0].data_ptr.ptr = nullptr;
}

/* -------------------------------------------------------------------------
 * EhsML_FW_TensorRT_SetInputData
 * ------------------------------------------------------------------------- */

extern "C" EhsML_Err EhsML_FW_TensorRT_SetInputData(EhsML_Context *ctx,
                                                     const void *input_data,
                                                     ehs_uint32 data_size)
{
    if (!ctx || !ctx->ml_model_ctx) return EHS_ML_NULL_CTX_ERR;
    if (!input_data)                return EHS_ML_NULL_INPUT_ERR;

    TRTModelCtx *trt = static_cast<TRTModelCtx *>(ctx->ml_model_ctx);

    if ((size_t)data_size != trt->input_bytes) {
        EHSH_LOG_ERROR("TensorRT: SetInputData size mismatch: got %u, expected %zu",
                       data_size, trt->input_bytes);
        return EHS_ML_INPUT_SIZE_MISMATCH_ERR;
    }

    if (cudaMemcpyAsync(trt->d_input, input_data, trt->input_bytes,
                        cudaMemcpyHostToDevice, trt->stream) != cudaSuccess) {
        EHSH_LOG_ERROR("TensorRT: cudaMemcpyAsync H→D failed");
        return EHS_ML_INFERENCE_ERR;
    }

    return EHS_ML_OK;
}

/* -------------------------------------------------------------------------
 * EhsML_FW_TensorRT_GetOutputData
 * ------------------------------------------------------------------------- */

extern "C" EhsML_Err EhsML_FW_TensorRT_GetOutputData(EhsML_Context *ctx)
{
    if (!ctx || !ctx->ml_model_ctx) return EHS_ML_NULL_CTX_ERR;
    TRTModelCtx *trt = static_cast<TRTModelCtx *>(ctx->ml_model_ctx);

    /* Run inference asynchronously on the CUDA stream */
    if (!trt->exec_ctx->enqueueV3(trt->stream)) {
        EHSH_LOG_ERROR("TensorRT: enqueueV3 failed");
        return EHS_ML_INFERENCE_ERR;
    }

    /* Copy device output to host */
    if (cudaMemcpyAsync(trt->h_output, trt->d_output, trt->output_bytes,
                        cudaMemcpyDeviceToHost, trt->stream) != cudaSuccess) {
        EHSH_LOG_ERROR("TensorRT: cudaMemcpyAsync D→H failed");
        return EHS_ML_INFERENCE_ERR;
    }

    /* Synchronise so the host buffer is ready for post-processing */
    if (cudaStreamSynchronize(trt->stream) != cudaSuccess) {
        EHSH_LOG_ERROR("TensorRT: cudaStreamSynchronize failed");
        return EHS_ML_INFERENCE_ERR;
    }

    /* Expose result through EhsML_Context — pointer into our h_output buffer */
    ctx->output_tensor[0].data_ptr.ptr   = trt->h_output;
    ctx->output_tensor[0].size_in_bytes  = (ehs_uint32)trt->output_bytes;
    ctx->output_tensor[0].data_ptr_owned = EHS_FALSE; /* owned by TRTModelCtx */

    return EHS_ML_OK;
}

/* -------------------------------------------------------------------------
 * EhsML_FW_TensorRT_GetModelInfoJson
 * ------------------------------------------------------------------------- */

extern "C" EhsML_Err EhsML_FW_TensorRT_GetModelInfoJson(EhsML_Context *ctx,
                                                         const ehs_char * /*model_path*/,
                                                         ehs_char *json_buf,
                                                         ehs_uint32 json_size)
{
    if (!ctx || !ctx->ml_model_ctx || !json_buf || json_size == 0)
        return EHS_ML_FAILED;

    TRTModelCtx              *trt = static_cast<TRTModelCtx *>(ctx->ml_model_ctx);
    const EhsML_Tensor_t     *in  = &ctx->input_tensor[0];
    const EhsML_Tensor_t     *out = &ctx->output_tensor[0];

    /* Build shape arrays as JSON strings */
    char in_shape[128]  = "[";
    char out_shape[128] = "[";
    for (ehs_uint32 i = 0; i < in->num_dims; i++) {
        char tmp[16];
        snprintf(tmp, sizeof(tmp), "%s%u", (i ? "," : ""), in->dims[i]);
        strncat(in_shape,  tmp, sizeof(in_shape)  - strlen(in_shape)  - 1);
    }
    strncat(in_shape, "]", sizeof(in_shape) - strlen(in_shape) - 1);

    for (ehs_uint32 i = 0; i < out->num_dims; i++) {
        char tmp[16];
        snprintf(tmp, sizeof(tmp), "%s%u", (i ? "," : ""), out->dims[i]);
        strncat(out_shape, tmp, sizeof(out_shape) - strlen(out_shape) - 1);
    }
    strncat(out_shape, "]", sizeof(out_shape) - strlen(out_shape) - 1);

    int written = snprintf(json_buf, (size_t)json_size,
        "{"
        "\"runtime\":\"TensorRT\","
        "\"input_count\":1,\"output_count\":1,"
        "\"inputs\":[{\"name\":\"%s\",\"dtype\":\"%s\",\"shape\":%s,\"bytes\":%u}],"
        "\"outputs\":[{\"name\":\"%s\",\"dtype\":\"%s\",\"shape\":%s,\"bytes\":%u}]"
        "}",
        trt->input_name,  ehs_dtype_to_str(in->data_type),  in_shape,  in->size_in_bytes,
        trt->output_name, ehs_dtype_to_str(out->data_type), out_shape, out->size_in_bytes);

    if (written < 0 || (ehs_uint32)written >= json_size) {
        EHSH_LOG_ERROR("TensorRT: GetModelInfoJson buffer too small (%u bytes)", json_size);
        return EHS_ML_FAILED;
    }
    return EHS_ML_OK;
}
