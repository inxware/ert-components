#include "tflite_model.h"
#include "tflite_yolo.h"
#include <tensorflow/lite/delegates/xnnpack/xnnpack_delegate.h>
#include <string.h>   /* memset */
//#include <stdio.h>

TfLiteErr TfLiteModel_Create(TfLiteModelCtx* ctx, const char* model_path, int num_threads)
{
    if (!ctx || !model_path || model_path[0] == '\0') return false;

    /* 1. Load model ------------------------------------------------------ */
    ctx->model = TfLiteModelCreateFromFile(model_path);
    if (!ctx->model) goto fail;

    /* 2. Interpreter options -------------------------------------------- */
    ctx->options = TfLiteInterpreterOptionsCreate();
    if (!ctx->options) goto fail;

    TfLiteInterpreterOptionsSetNumThreads(ctx->options, num_threads);

    /* create and attach XNNPACK delegate */
    TfLiteXNNPackDelegateOptions xnn_opts = TfLiteXNNPackDelegateOptionsDefault();
    xnn_opts.num_threads = num_threads;

    ctx->xnn_delegate = TfLiteXNNPackDelegateCreate(&xnn_opts);
    TfLiteInterpreterOptionsAddDelegate(ctx->options, ctx->xnn_delegate);

     // If you know which ops you need, you can add them here:
     //  TfLiteInterpreterOptionsAddBuiltinOp(ctx->options,
     //       kTfLiteBuiltinConv2d, /* min_version = */ 1, /* max_version = */ 5);
     //  ...
     //  For most models you can skip this step – the runtime will
     //  automatically resolve built-ins it finds in the flatbuffer.       

    /* 3. Create interpreter --------------------------------------------- */
    ctx->interp = TfLiteInterpreterCreate(ctx->model, ctx->options);
    if (!ctx->interp) goto fail;

    /* 4. Allocate tensors ----------------------------------------------- */
    if (TfLiteInterpreterAllocateTensors(ctx->interp) != kTfLiteOk) goto fail;

    /* 5. Identify which model we process -------------------------------- */
    ctx->in_tensor  = TfLiteInterpreterGetInputTensor (ctx->interp, 0);
    ctx->out_tensor = TfLiteInterpreterGetOutputTensor(ctx->interp, 0);
    if(!ctx->in_tensor || !ctx->out_tensor) goto fail;

    int32_t input_dims = TfLiteTensorNumDims(ctx->in_tensor);
    int32_t output_dims = TfLiteTensorNumDims(ctx->out_tensor);
    
    // Input  (4D) Yolo style [B, H, W, C], typical = [1, H, W, 3] 
    // Output (3D) Yolo style [B, N, M], typical = [1, N, 5 + num_classes] , N = a large flat number of detections (e.g., 6300, 8400)
    if(input_dims == 4 && output_dims == 3){ // is yolo style
        // Set detector model type for yolo
        ctx->model_type = TF_LITE_MODEL_DETECTOR;
        // Check model type e.g. float16
        ctx->type = TfLiteTensorType(ctx->in_tensor);
        if(ctx->type == kTfLiteFloat32 || ctx->type == kTfLiteFloat16){
            // set float yolo type detector
            ctx->fnRunDetector = TfLiteRunYolo_Float;
        } else if(ctx->type == kTfLiteInt8 || ctx->type == kTfLiteUInt8) {
            // set int8 yolo type detector
            ctx->fnRunDetector =  TfLiteRunYolo_Int8;
        }
        // not sure if these are even needed for yolo
        //else if(ctx->type == kTfLiteInt16)  { ... }
        //else if(ctx->type == kTfLiteUInt16) { ... }
        // ...
        else{
            //printf("Unsupported model input format=%d\n",ctx->type);
            goto fail;    
        }
    }
    // else if() { ... } // add other model support
    else{
        //printf("Unsupported model loaded inputs=%d outputs=%d\n",input_dims, output_dims);
        goto fail;
    }

    return TF_LITE_OK;

fail:
    TfLiteModel_Destroy(ctx);
    return TF_LITE_FAILED;
}

void TfLiteModel_Destroy(TfLiteModelCtx* ctx)
{
    if (!ctx) return;

    if (ctx->interp ) TfLiteInterpreterDelete(ctx->interp );
    if (ctx->xnn_delegate) TfLiteXNNPackDelegateDelete(ctx->xnn_delegate);
    if (ctx->options) TfLiteInterpreterOptionsDelete(ctx->options);
    if (ctx->model  ) TfLiteModelDelete(ctx->model);

    memset(ctx, 0, sizeof(*ctx));                /* mark as empty */
}

TfLiteErr TfLiteModel_Set_Input_Data(TfLiteModelCtx* ctx, const void* data, unsigned int size)
{ 
    return ( ctx && ctx->in_tensor && data) &&
        TfLiteTensorCopyFromBuffer((TfLiteTensor*)ctx->in_tensor, data, size) == kTfLiteOk ? TF_LITE_OK : TF_LITE_FAILED;
}

TfLiteErr TfLiteModel_Run_Detector(TfLiteModelCtx* ctx, TfLiteBoxDetections* detections)
{
    if(!ctx || ctx->model_type != TF_LITE_MODEL_DETECTOR || !ctx->fnRunDetector || !detections) return TF_LITE_FAILED;

    return ctx->fnRunDetector(ctx, detections);
}