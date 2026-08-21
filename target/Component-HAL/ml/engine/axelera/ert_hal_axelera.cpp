/***************************************************************
* Copyright (C) 2008-2025 inx limited - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/** @file ert_hal_axelera.cpp
 * Axelera Voyager SDK (axruntime) inference engine backend.
 *
 * libaxruntime.so is loaded at runtime via dlopen so the binary starts on
 * machines that do not have the Axelera SDK installed.  If the library or
 * device is absent, Create returns EHS_ML_INIT_ERR and the function block
 * fires its load_err event — everything else on the host continues running.
 *
 * API reference: /opt/axelera/runtime-1.6.0-1/include/axruntime/axruntime.h
 * SDK version  : 1.6.0
 *
 * @author: inx limited
 */

/* C headers must be wrapped so C++ does not mangle their symbols */
extern "C" {
#include "ert_hal_axelera.h"
#include "hal_ml.h"
#include "hal-api.h"
}

/* Axelera SDK — C++-native header, included for struct/enum type definitions.
 * The library itself is NOT linked at build time; it is dlopen'd at runtime. */
#include "axruntime/axruntime.h"
#include <dlfcn.h>
#include <cstdlib>
#include <cstring>

/* ── Runtime-loaded function pointers ──────────────────────────────────────── */

typedef axrContext*       (*pfn_axr_create_context)(void);
typedef size_t            (*pfn_axr_list_devices)(axrContext*, axrDeviceInfo**);
typedef axrConnection*    (*pfn_axr_device_connect)(axrContext*, const axrDeviceInfo*, size_t, const axrProperties*);
typedef axrModel*         (*pfn_axr_load_model)(axrContext*, const char*);
typedef size_t            (*pfn_axr_num_model_inputs)(axrModel*);
typedef axrTensorInfo     (*pfn_axr_get_model_input)(axrModel*, size_t);
typedef size_t            (*pfn_axr_num_model_outputs)(axrModel*);
typedef axrTensorInfo     (*pfn_axr_get_model_output)(axrModel*, size_t);
typedef size_t            (*pfn_axr_tensor_size)(const axrTensorInfo*);
typedef axrModelInstance* (*pfn_axr_load_model_instance)(axrConnection*, axrModel*, axrProperties*);
typedef axrResult         (*pfn_axr_run_model_instance)(axrModelInstance*, axrArgument*, size_t, axrArgument*, size_t);
typedef void              (*pfn_axr_destroy)(const axrObject*);
typedef const char*       (*pfn_axr_last_error_string)(const axrObject*);
typedef const char*       (*pfn_axr_error_string)(axrResult);

typedef struct {
    pfn_axr_create_context       axr_create_context;
    pfn_axr_list_devices         axr_list_devices;
    pfn_axr_device_connect       axr_device_connect;
    pfn_axr_load_model           axr_load_model;
    pfn_axr_num_model_inputs     axr_num_model_inputs;
    pfn_axr_get_model_input      axr_get_model_input;
    pfn_axr_num_model_outputs    axr_num_model_outputs;
    pfn_axr_get_model_output     axr_get_model_output;
    pfn_axr_tensor_size          axr_tensor_size;
    pfn_axr_load_model_instance  axr_load_model_instance;
    pfn_axr_run_model_instance   axr_run_model_instance;
    pfn_axr_destroy              axr_destroy;
    pfn_axr_last_error_string    axr_last_error_string;
    pfn_axr_error_string         axr_error_string;
} AxeleraVtable;

/* The SDK installs to a fixed prefix; fall back to that if the system ldconfig
 * path ("libaxruntime.so") does not resolve (no ldconfig run yet, non-standard
 * install, etc.). */
#define AXRUNTIME_LIB_NAME    "libaxruntime.so"
#define AXRUNTIME_LIB_FALLBACK "/opt/axelera/runtime-1.6.0-1/lib/libaxruntime.so"

#define AXR_SYM(vtable, handle, name) \
    (vtable).name = (pfn_##name) dlsym((handle), #name); \
    if (!(vtable).name) { \
        EHSH_LOG_ERROR("dlsym " #name " failed: %s\n", dlerror()); \
        dlclose(handle); \
        return EHS_ML_INIT_ERR; \
    }

static EhsML_Err axr_load_vtable(void **out_handle, AxeleraVtable *vt)
{
    void *h = dlopen(AXRUNTIME_LIB_NAME, RTLD_LAZY | RTLD_LOCAL);
    if (!h) {
        EHSH_LOG_ERROR("dlopen(%s) failed: %s — trying fallback path\n",
                       AXRUNTIME_LIB_NAME, dlerror());
        h = dlopen(AXRUNTIME_LIB_FALLBACK, RTLD_LAZY | RTLD_LOCAL);
    }
    if (!h) {
        EHSH_LOG_ERROR("Axelera SDK not found (%s). "
                       "Install axelera-runtime or set LD_LIBRARY_PATH.\n",
                       dlerror());
        return EHS_ML_INIT_ERR;
    }

    AXR_SYM(*vt, h, axr_create_context)
    AXR_SYM(*vt, h, axr_list_devices)
    AXR_SYM(*vt, h, axr_device_connect)
    AXR_SYM(*vt, h, axr_load_model)
    AXR_SYM(*vt, h, axr_num_model_inputs)
    AXR_SYM(*vt, h, axr_get_model_input)
    AXR_SYM(*vt, h, axr_num_model_outputs)
    AXR_SYM(*vt, h, axr_get_model_output)
    AXR_SYM(*vt, h, axr_tensor_size)
    AXR_SYM(*vt, h, axr_load_model_instance)
    AXR_SYM(*vt, h, axr_run_model_instance)
    AXR_SYM(*vt, h, axr_destroy)
    AXR_SYM(*vt, h, axr_last_error_string)
    AXR_SYM(*vt, h, axr_error_string)

    *out_handle = h;
    return EHS_ML_OK;
}

/* ── Per-context state ──────────────────────────────────────────────────────── */

typedef struct {
    void            *dl_handle;
    AxeleraVtable    fn;
    axrContext      *context;
    axrConnection   *connection;
    axrModel        *model;
    axrModelInstance *instance;
    size_t           num_inputs;
    size_t           num_outputs;
    axrArgument      input_args[EHS_ML_LAYER_TENSORS_MAX];
    axrArgument      output_args[EHS_ML_LAYER_TENSORS_MAX];
} AxeleraCtx;

/* ── Data type mapping ──────────────────────────────────────────────────────── */

static EhsML_DataType_t axr_type_to_ehs(size_t type, size_t bits)
{
    if (type == AXR_FLOAT) {
        if (bits == 16) return EHS_ML_DATATYPE_FP16;
        if (bits == 32) return EHS_ML_DATATYPE_FP32;
        if (bits == 64) return EHS_ML_DATATYPE_FP64;
    } else if (type == AXR_SIGNED) {
        if (bits == 8)  return EHS_ML_DATATYPE_INT8;
        if (bits == 16) return EHS_ML_DATATYPE_INT16;
        if (bits == 32) return EHS_ML_DATATYPE_INT32;
        if (bits == 64) return EHS_ML_DATATYPE_INT64;
    } else { /* AXR_UNSIGNED */
        if (bits == 8)  return EHS_ML_DATATYPE_UINT8;
        if (bits == 16) return EHS_ML_DATATYPE_UINT16;
        if (bits == 32) return EHS_ML_DATATYPE_UINT32;
        if (bits == 64) return EHS_ML_DATATYPE_UINT64;
    }
    return EHS_ML_DATATYPE_NONE;
}

/* ── Public API ─────────────────────────────────────────────────────────────── */

extern "C" EhsML_Err EhsML_FW_Axelera_Create(EhsML_Context *ctx, const ehs_char *model_path,
                                              ehs_float conf_thres, ehs_sint32 thread_count)
{
    (void)conf_thres;
    (void)thread_count;

    if (ctx == NULL || model_path == NULL) {
        EHSH_LOG_ERROR("NULL ctx or model_path\n");
        return EHS_ML_INIT_ERR;
    }

    AxeleraCtx *axctx = (AxeleraCtx *)calloc(1, sizeof(AxeleraCtx));
    if (axctx == NULL) return EHS_ML_MEMORY_ERR;

    EhsML_Err err = axr_load_vtable(&axctx->dl_handle, &axctx->fn);
    if (err != EHS_ML_OK) {
        free(axctx);
        return err;
    }

    /* — Context — */
    axctx->context = axctx->fn.axr_create_context();

    /* — Device discovery — */
    axrDeviceInfo *devices = NULL;
    size_t num_devices = axctx->fn.axr_list_devices(axctx->context, &devices);
    if (num_devices == 0) {
        EHSH_LOG_ERROR("No Axelera Metis device found (SDK: %s)\n",
                       axctx->fn.axr_last_error_string(AXR_OBJECT(axctx->context)));
        err = EHS_ML_INIT_ERR;
        goto l_destroy_context;
    }

    /* — Connect to first available device (all sub-devices) — */
    axctx->connection = axctx->fn.axr_device_connect(
        axctx->context, &devices[0], devices[0].subdevice_count, NULL);
    if (axctx->connection == NULL) {
        EHSH_LOG_ERROR("axr_device_connect failed: %s\n",
                       axctx->fn.axr_last_error_string(AXR_OBJECT(axctx->context)));
        err = EHS_ML_INIT_ERR;
        goto l_destroy_context;
    }

    /* — Load model — */
    axctx->model = axctx->fn.axr_load_model(axctx->context, model_path);
    if (axctx->model == NULL) {
        EHSH_LOG_ERROR("axr_load_model failed for '%s': %s\n",
                       model_path, axctx->fn.axr_last_error_string(AXR_OBJECT(axctx->context)));
        err = EHS_ML_MODEL_LOAD_ERR;
        goto l_destroy_context;
    }

    /* — Input tensors — */
    axctx->num_inputs = axctx->fn.axr_num_model_inputs(axctx->model);
    if (axctx->num_inputs == 0 || axctx->num_inputs > EHS_ML_LAYER_TENSORS_MAX) {
        EHSH_LOG_ERROR("Unsupported input count %zu (max %d)\n",
                       axctx->num_inputs, EHS_ML_LAYER_TENSORS_MAX);
        err = EHS_ML_INIT_ERR;
        goto l_destroy_context;
    }
    ctx->input_tensor_count = (ehs_uint32)axctx->num_inputs;

    for (size_t n = 0; n < axctx->num_inputs; n++) {
        axrTensorInfo info = axctx->fn.axr_get_model_input(axctx->model, n);
        size_t byte_size   = axctx->fn.axr_tensor_size(&info);
        ehs_uint32 dims_bytes[1] = { (ehs_uint32)byte_size };

        ctx->input_tensor[n].data_type = axr_type_to_ehs(info.type, info.bits);
        ctx->input_tensor[n].quantisation_params.scale  = info.scale;
        ctx->input_tensor[n].quantisation_params.offset = (double)info.zero_point;
        ctx->input_tensor[n].num_dims = (info.ndims < EHS_ML_TENSOR_MAX_DIMS) ?
                                         (ehs_uint32)info.ndims : EHS_ML_TENSOR_MAX_DIMS;
        for (ehs_uint32 d = 0; d < ctx->input_tensor[n].num_dims; d++)
            ctx->input_tensor[n].dims[d] = (ehs_uint32)info.dims[d];

        err = EhsML_Tensor_Alloc(&ctx->input_tensor[n], EHS_ML_DATATYPE_RAW, dims_bytes, 1);
        if (err != EHS_ML_OK) {
            EHSH_LOG_ERROR("Failed to allocate input tensor %zu\n", n);
            goto l_free_input_tensors;
        }

        axctx->input_args[n].ptr    = ctx->input_tensor[n].data_ptr.ptr;
        axctx->input_args[n].fd     = -1;
        axctx->input_args[n].offset = 0;
        axctx->input_args[n].size   = 0;
    }

    /* — Output tensors — */
    axctx->num_outputs = axctx->fn.axr_num_model_outputs(axctx->model);
    if (axctx->num_outputs == 0 || axctx->num_outputs > EHS_ML_LAYER_TENSORS_MAX) {
        EHSH_LOG_ERROR("Unsupported output count %zu (max %d)\n",
                       axctx->num_outputs, EHS_ML_LAYER_TENSORS_MAX);
        err = EHS_ML_INIT_ERR;
        goto l_free_input_tensors;
    }
    ctx->output_tensor_count = (ehs_uint32)axctx->num_outputs;

    for (size_t n = 0; n < axctx->num_outputs; n++) {
        axrTensorInfo info = axctx->fn.axr_get_model_output(axctx->model, n);
        size_t byte_size   = axctx->fn.axr_tensor_size(&info);
        ehs_uint32 dims_bytes[1] = { (ehs_uint32)byte_size };

        ctx->output_tensor[n].data_type = axr_type_to_ehs(info.type, info.bits);
        ctx->output_tensor[n].quantisation_params.scale  = info.scale;
        ctx->output_tensor[n].quantisation_params.offset = (double)info.zero_point;
        ctx->output_tensor[n].num_dims = (info.ndims < EHS_ML_TENSOR_MAX_DIMS) ?
                                          (ehs_uint32)info.ndims : EHS_ML_TENSOR_MAX_DIMS;
        for (ehs_uint32 d = 0; d < ctx->output_tensor[n].num_dims; d++)
            ctx->output_tensor[n].dims[d] = (ehs_uint32)info.dims[d];

        err = EhsML_Tensor_Alloc(&ctx->output_tensor[n], EHS_ML_DATATYPE_RAW, dims_bytes, 1);
        if (err != EHS_ML_OK) {
            EHSH_LOG_ERROR("Failed to allocate output tensor %zu\n", n);
            goto l_free_output_tensors;
        }

        axctx->output_args[n].ptr    = ctx->output_tensor[n].data_ptr.ptr;
        axctx->output_args[n].fd     = -1;
        axctx->output_args[n].offset = 0;
        axctx->output_args[n].size   = 0;
    }

    /* — Load model instance onto the NPU — */
    axctx->instance = axctx->fn.axr_load_model_instance(
        axctx->connection, axctx->model, NULL);
    if (axctx->instance == NULL) {
        EHSH_LOG_ERROR("axr_load_model_instance failed: %s\n",
                       axctx->fn.axr_last_error_string(AXR_OBJECT(axctx->context)));
        err = EHS_ML_INIT_ERR;
        goto l_free_output_tensors;
    }

    ctx->ml_model_ctx = (void *)axctx;
    ctx->hw_accel     = EHS_ML_HWACCEL_AXELERA;

    EHS_ML_STAGE_SET(ctx->pipeline, EHS_ML_STAGE_INFER,  EHS_ML_TECH_AXELERA);
    EHS_ML_STAGE_SET(ctx->pipeline, EHS_ML_STAGE_UNPACK, EHS_ML_TECH_AXELERA);

    return EHS_ML_OK;

l_free_output_tensors:
    for (size_t i = 0; i < axctx->num_outputs; i++)
        EhsML_Tensor_Free(&ctx->output_tensor[i]);
l_free_input_tensors:
    for (size_t i = 0; i < axctx->num_inputs; i++)
        EhsML_Tensor_Free(&ctx->input_tensor[i]);
l_destroy_context:
    axctx->fn.axr_destroy(AXR_OBJECT(axctx->context));
    dlclose(axctx->dl_handle);
    free(axctx);
    return err;
}

extern "C" void EhsML_FW_Axelera_Destroy(EhsML_Context *ctx)
{
    if (ctx == NULL || ctx->ml_model_ctx == NULL) return;

    AxeleraCtx *axctx = (AxeleraCtx *)ctx->ml_model_ctx;

    for (size_t i = 0; i < axctx->num_inputs; i++)
        EhsML_Tensor_Free(&ctx->input_tensor[i]);
    for (size_t i = 0; i < axctx->num_outputs; i++)
        EhsML_Tensor_Free(&ctx->output_tensor[i]);

    /* Destroying the context tears down connection, model, and instance */
    axctx->fn.axr_destroy(AXR_OBJECT(axctx->context));
    dlclose(axctx->dl_handle);

    free(axctx);
    ctx->ml_model_ctx = NULL;
}

extern "C" EhsML_Err EhsML_FW_Axelera_SetInputData(EhsML_Context *ctx,
                                                    const void *input_data,
                                                    ehs_uint32 data_size)
{
    if (ctx == NULL || ctx->ml_model_ctx == NULL) return EHS_ML_NULL_CTX_ERR;
    if (input_data == NULL) return EHS_ML_NULL_INPUT_ERR;
    if (data_size == 0)     return EHS_ML_INVALID_SIZE_ERR;

    if (ctx->input_tensor[0].data_ptr.ptr == NULL) {
        EHSH_LOG_ERROR("Input tensor not allocated\n");
        return EHS_ML_MEMORY_ERR;
    }
    if (data_size > ctx->input_tensor[0].size_in_bytes) {
        EHSH_LOG_ERROR("Input size %u exceeds tensor buffer %zu\n",
                       data_size, ctx->input_tensor[0].size_in_bytes);
        return EHS_ML_INPUT_SIZE_MISMATCH_ERR;
    }

    EhsML_Tensor_FillRaw(&ctx->input_tensor[0], (ehs_char *)input_data, data_size);
    return EHS_ML_OK;
}

extern "C" EhsML_Err EhsML_FW_Axelera_GetOutputData(EhsML_Context *ctx)
{
    if (ctx == NULL || ctx->ml_model_ctx == NULL) return EHS_ML_NULL_CTX_ERR;

    AxeleraCtx *axctx = (AxeleraCtx *)ctx->ml_model_ctx;

    axrResult result = axctx->fn.axr_run_model_instance(
        axctx->instance,
        axctx->input_args,  axctx->num_inputs,
        axctx->output_args, axctx->num_outputs);

    if (result != AXR_SUCCESS) {
        EHSH_LOG_ERROR("axr_run_model_instance failed: %s\n",
                       axctx->fn.axr_error_string(result));
        return EHS_ML_INFERENCE_ERR;
    }

    return EHS_ML_OK;
}
