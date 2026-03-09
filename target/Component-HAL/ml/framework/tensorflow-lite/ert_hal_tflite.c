#include "ert_hal_tflite.h"
#include "ert_hal_tflite_meta.h"
#include <tensorflow/lite/delegates/xnnpack/xnnpack_delegate.h>
#include "hal-api.h"

static void TfLiteModel_Destroy(TfLiteModelCtx* ctx)
{
    if (!ctx) return;

    if (ctx->interp ) TfLiteInterpreterDelete(ctx->interp );
    if (ctx->xnn_delegate) TfLiteXNNPackDelegateDelete(ctx->xnn_delegate);
    if (ctx->options) TfLiteInterpreterOptionsDelete(ctx->options);
    if (ctx->model  ) TfLiteModelDelete(ctx->model);

    memset(ctx, 0, sizeof(*ctx));                /* mark as empty */
}

EhsML_Err EhsML_FW_TFLite_Create(EhsML_Context* ctx, const ehs_char* model_path, ehs_float conf_thres, ehs_sint32 thread_count)
{
    EhsML_Err err = EHS_ML_FAILED;

    if (!ctx) goto ml_fail;

    if(ctx->ml_model_ctx){
        err = EHS_ML_MODEL_IN_USE;
        goto ml_fail;
    }

    if(!model_path || *model_path == '\0'){
        err = EHS_ML_MODEL_PATH_ERR;
        goto ml_fail;
    }

    TfLiteModelCtx* tfl_model_ctx = (TfLiteModelCtx*)EhsHMem_permAlloc(sizeof(TfLiteModelCtx));;
    if(!tfl_model_ctx){
        err = EHS_ML_MEMORY_ERR;
        goto ml_fail;
    }
    EhsMemset(tfl_model_ctx, 0, sizeof(TfLiteModelCtx));
    tfl_model_ctx->conf_thres = (float)conf_thres; 

    /* TensorFlow Lite Instance creation based on model */
    //TfLiteErr tfErr = TfLiteModel_Create(tfl_model_ctx, model_path, thread_count);
    ehs_bool tfErr = EHS_FALSE;
    ehs_bool tflite_err_override = EHS_FALSE;
    /* 1. Load Model*/
    tfl_model_ctx->model = TfLiteModelCreateFromFile(model_path);
    if (!tfl_model_ctx->model) goto tflite_error;

    /* 2. Interpreter options */
    tfl_model_ctx->options = TfLiteInterpreterOptionsCreate();
    if (!tfl_model_ctx->options) goto tflite_error;

    TfLiteInterpreterOptionsSetNumThreads(tfl_model_ctx->options, thread_count);

    /* Create and attach XNNPACK delegate */
    TfLiteXNNPackDelegateOptions xnn_opts = TfLiteXNNPackDelegateOptionsDefault();
    xnn_opts.num_threads = thread_count;
    tfl_model_ctx->xnn_delegate = TfLiteXNNPackDelegateCreate(&xnn_opts);
    TfLiteInterpreterOptionsAddDelegate(tfl_model_ctx->options, tfl_model_ctx->xnn_delegate);
    // If you know which ops you need, you can add them here:
    //  TfLiteInterpreterOptionsAddBuiltinOp(ctx->options,
    //       kTfLiteBuiltinConv2d, /* min_version = */ 1, /* max_version = */ 5);
    //  ...
    //  For most models you can skip this step – the runtime will
    //  automatically resolve built-ins it finds in the flatbuffer.       

    /* 3. Create Interpreter */
    tfl_model_ctx->interp = TfLiteInterpreterCreate(tfl_model_ctx->model, tfl_model_ctx->options);
    if (!tfl_model_ctx->interp) goto tflite_error;

    /* 4. Allocate Tensorflow Lite tensors */
    if (TfLiteInterpreterAllocateTensors(tfl_model_ctx->interp) != kTfLiteOk) goto tflite_error;

    /* 5. Get Input and Output tensors for the interpreter */
    tfl_model_ctx->in_tensor = TfLiteInterpreterGetInputTensor(tfl_model_ctx->interp, 0);
    tfl_model_ctx->out_tensor = TfLiteInterpreterGetOutputTensor(tfl_model_ctx->interp, 0);
    if (!tfl_model_ctx->in_tensor || !tfl_model_ctx->out_tensor) goto tflite_error;

    /* 6. Create general tensor objects */
    // TODO Support multiple input and output tensors
    ctx->input_tensor_count = 1;
    ctx->output_tensor_count = 1;
    int32_t output_dims = TfLiteTensorNumDims(tfl_model_ctx->out_tensor);
    if (output_dims > EHS_ML_TENSOR_MAX_DIMS)
    {
        tflite_err_override = EHS_TRUE;
        err = EHS_ML_MODEL_TENSOR_DIM_ERR;
        goto tflite_error;
    }
    int i = 0;
    for (i = 0 ; i < output_dims ; i++)
    {
        ctx->output_tensor[0].dims[i] = TfLiteTensorDim(tfl_model_ctx->out_tensor, i);
    }
    switch (TfLiteTensorType(tfl_model_ctx->out_tensor))
    {
        case kTfLiteNoType:
        {
            ctx->data_type = EHS_ML_DATATYPE_NONE;
            ctx->output_tensor[0].data_type = EHS_ML_DATATYPE_NONE;
            break;
        }
        case kTfLiteFloat32:
        {
            ctx->data_type = EHS_ML_DATATYPE_FP32;
            ctx->output_tensor[0].data_type = EHS_ML_DATATYPE_FP32;
            break;
        }
        case kTfLiteInt32:
        {
            ctx->data_type = EHS_ML_DATATYPE_INT32;
            ctx->output_tensor[0].data_type = EHS_ML_DATATYPE_INT32;
            break;
        }
        case kTfLiteUInt8:
        {
            ctx->data_type = EHS_ML_DATATYPE_UINT8;
            ctx->output_tensor[0].data_type = EHS_ML_DATATYPE_UINT8;
            break;
        }
        case kTfLiteInt64:
        {
            ctx->data_type = EHS_ML_DATATYPE_INT64;
            ctx->output_tensor[0].data_type = EHS_ML_DATATYPE_INT64;
            break;
        }
        case kTfLiteString:
        {
            ctx->data_type = EHS_ML_DATATYPE_RAW;
            ctx->output_tensor[0].data_type = EHS_ML_DATATYPE_RAW;
            break;
        }
        case kTfLiteBool:
        {
            ctx->data_type = EHS_ML_DATATYPE_BOOL;
            ctx->output_tensor[0].data_type = EHS_ML_DATATYPE_BOOL;
            break;
        }
        case kTfLiteInt16:
        {
            ctx->data_type = EHS_ML_DATATYPE_INT16;
            ctx->output_tensor[0].data_type = EHS_ML_DATATYPE_INT16;
            break;
        }
        case kTfLiteComplex64:
        {
            ctx->data_type = EHS_ML_DATATYPE_COMPLEX64;
            ctx->output_tensor[0].data_type = EHS_ML_DATATYPE_COMPLEX64;
            break;
        }
        case kTfLiteInt8:
        {
            ctx->data_type = EHS_ML_DATATYPE_INT8;
            ctx->output_tensor[0].data_type = EHS_ML_DATATYPE_INT8;
            break;
        }
        case kTfLiteFloat16:
        {
            ctx->data_type = EHS_ML_DATATYPE_FP16;
            ctx->output_tensor[0].data_type = EHS_ML_DATATYPE_FP16;
            break;
        }
        case kTfLiteFloat64:
        {
            ctx->data_type = EHS_ML_DATATYPE_FP64;
            ctx->output_tensor[0].data_type = EHS_ML_DATATYPE_FP64;
            break;
        }
        case kTfLiteComplex128:
        {
            ctx->data_type = EHS_ML_DATATYPE_COMPLEX128;
            ctx->output_tensor[0].data_type = EHS_ML_DATATYPE_COMPLEX128;
            break;
        }
        case kTfLiteUInt64:
        {
            ctx->data_type = EHS_ML_DATATYPE_UINT64;
            ctx->output_tensor[0].data_type = EHS_ML_DATATYPE_UINT64;
            break;
        }
        case kTfLiteResource:
        {
            ctx->data_type = EHS_ML_DATATYPE_RAW;
            ctx->output_tensor[0].data_type = EHS_ML_DATATYPE_RAW;
            break;
        }
        case kTfLiteVariant:
        {
            ctx->data_type = EHS_ML_DATATYPE_RAW;
            ctx->output_tensor[0].data_type = EHS_ML_DATATYPE_RAW;
            break;
        }
        case kTfLiteUInt32:
        {
            ctx->data_type = EHS_ML_DATATYPE_UINT32;
            ctx->output_tensor[0].data_type = EHS_ML_DATATYPE_UINT32;
            break;
        }
        case kTfLiteUInt16:
        {
            ctx->data_type = EHS_ML_DATATYPE_UINT16;
            ctx->output_tensor[0].data_type = EHS_ML_DATATYPE_UINT16;
            break;
        }
        case kTfLiteInt4:
        {
            // TODO How to deal with 4-bit signed integer
            ctx->data_type = EHS_ML_DATATYPE_NONE;
            ctx->output_tensor[0].data_type = EHS_ML_DATATYPE_NONE;
            break;
        }
        default:
        {
            ctx->data_type = EHS_ML_DATATYPE_NONE;
            break;
        }
    }
    
    /* Zero-copy: point directly at TFLite's output buffer.
     * TfLiteInterpreterAllocateTensors() (step 4) has already allocated this
     * buffer; the pointer is stable until TfLiteInterpreterDelete() is called.
     * After each TfLiteInterpreterInvoke() call TFLite writes results in-place —
     * the model decoder reads them directly with no intermediate copy.
     *
     * data_ptr_owned = EHS_FALSE tells EhsML_Tensor_Free() not to call free()
     * on this pointer — it is freed by TfLiteModel_Destroy().
     *
     * The destroy sequence in EhsML_FW_TFLite_Destroy() nulls data_ptr BEFORE
     * calling TfLiteModel_Destroy(), so the pointer is never accessed after
     * TFLite frees it. */
    ctx->output_tensor[0].data_ptr.ptr   = TfLiteTensorData(tfl_model_ctx->out_tensor);
    ctx->output_tensor[0].size_in_bytes  = (ehs_uint32)TfLiteTensorByteSize(tfl_model_ctx->out_tensor);
    ctx->output_tensor[0].num_dims       = (ehs_uint32)output_dims;
    ctx->output_tensor[0].data_ptr_owned = EHS_FALSE;

    /* 7. Get quantisation parameters from the model.
     * TFLite returns scale=0.0 for unquantised float models (meaning "no quantisation").
     * Treat scale==0 as unconfigured and default to 1.0 so the dequantisation formula
     * (value - zero_point) * scale is a no-op for raw float output. */
    TfLiteQuantizationParams q = TfLiteTensorQuantizationParams(tfl_model_ctx->out_tensor);
    ctx->output_tensor[0].quantisation_params.scale  = (q.scale == 0.0f) ? 1.0 : (double)q.scale;
    ctx->output_tensor[0].quantisation_params.offset = (double)q.zero_point * -1.0;

    /* 8. Identify which model to process */
    //TODO should be this retrieved from the yolo_model?

    /* Last. There is no error for TfLite instance creation*/
    tfErr = EHS_TRUE;
    
tflite_error:
    if(tfErr == EHS_FALSE){
        TfLiteModel_Destroy(tfl_model_ctx);
        if (tflite_err_override == EHS_FALSE) err = EHS_ML_MODEL_LOAD_ERR;
        EhsHMem_permFree(tfl_model_ctx);
        EhsMemset(ctx, 0, sizeof(EhsML_Context));
        goto ml_fail;
    }
    // store model context
    ctx->ml_model_ctx = (void*)tfl_model_ctx;
    ctx->conf_thres = conf_thres;

    return EHS_ML_OK;

ml_fail:
    return err;
}

void EhsML_FW_TFLite_Destroy(EhsML_Context* ctx)
{
    if (ctx == NULL) return;

    /* Free owned output tensor buffers FIRST, while data_ptr is still valid.
     * EhsMemset() below would zero data_ptr before EhsML_Tensor_Free() could
     * reach it, causing the malloc'd buffer to leak on every model unload. */
    ehs_uint32 _t;
    for (_t = 0; _t < ctx->output_tensor_count; _t++)
    {
        EhsML_Tensor_Free(&ctx->output_tensor[_t]);
    }

    if (ctx->ml_model_ctx)
    {
        TfLiteModel_Destroy((TfLiteModelCtx*) ctx->ml_model_ctx);
        EhsHMem_permFree((TfLiteModelCtx*) ctx->ml_model_ctx);
    }
    EhsMemset(ctx, 0, sizeof(EhsML_Context));
}

EhsML_Err EhsML_FW_TFLite_SetInputData(EhsML_Context* ctx, const void* input_data, ehs_uint32 data_size)
{
    if (!ctx) return EHS_ML_NULL_CTX_ERR;
    if (!ctx->ml_model_ctx) return EHS_ML_INIT_ERR;

    TfLiteModelCtx* tfl = (TfLiteModelCtx*)ctx->ml_model_ctx;
    if (!tfl->in_tensor) return EHS_ML_INIT_ERR;
    if (!input_data) return EHS_ML_NULL_INPUT_ERR;

    size_t tensor_bytes = TfLiteTensorByteSize((TfLiteTensor*)tfl->in_tensor);
    if (data_size != tensor_bytes) return EHS_ML_INPUT_SIZE_MISMATCH_ERR;

    ehs_bool ok = TfLiteTensorCopyFromBuffer((TfLiteTensor*)tfl->in_tensor, input_data, data_size) == kTfLiteOk;
    if (!ok) return EHS_ML_FAILED;
    return EHS_ML_OK;
}

EhsML_Err EhsML_FW_TFLite_GetOutputData(EhsML_Context* ctx)
{
    if (!ctx) return EHS_ML_FAILED;
    if (!ctx->ml_model_ctx) return EHS_ML_INIT_ERR;
    if (!ctx->output_tensor[0].data_ptr.ptr) return EHS_ML_INVALID_TENSOR_ERR;
    TfLiteModelCtx *tfl_model_ctx = (TfLiteModelCtx *) ctx->ml_model_ctx;
    if (!tfl_model_ctx->out_tensor) return EHS_ML_INIT_ERR;

    /* Run inference. TFLite writes results directly into the buffer that
     * ctx->output_tensor[0].data_ptr already points to (set at Create time).
     * No memcpy required — the model decoder reads from data_ptr directly. */
    if (TfLiteInterpreterInvoke(tfl_model_ctx->interp) != kTfLiteOk) return EHS_ML_INFERENCE_ERR;

    return EHS_ML_OK;
}