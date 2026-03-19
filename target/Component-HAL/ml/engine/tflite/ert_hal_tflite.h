#ifndef _C_HEADER_EHS_TARGET_HAL_ML_FRAMEWORK_TENSORFLOW_LITE_ERT_HAL_TFLITE_H_
#define _C_HEADER_EHS_TARGET_HAL_ML_FRAMEWORK_TENSORFLOW_LITE_ERT_HAL_TFLITE_H_

#include "hal_ml.h"
#include <tensorflow/lite/c/c_api.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Tensorflow-lite model context */
typedef struct TfLiteModelCtx {
    TfLiteModel*              model;            /* owns the flat-buffer */
    TfLiteInterpreterOptions* options;          /* holds thread count etc. */
    TfLiteInterpreter*        interp;           /* ready-to-run interpreter */
    TfLiteDelegate*           xnn_delegate;     /* used for enabling XNNPACK */
    TfLiteType                type;             /* model data type e.g. float32, float16, int8 etc. */
    const TfLiteTensor*       in_tensor;
    const TfLiteTensor*       out_tensor;

    float                     conf_thres;       /* confidence threshold */
} TfLiteModelCtx;

EhsML_Err EhsML_FW_TFLite_Create(EhsML_Context* ctx, const ehs_char* model_path, ehs_float conf_thres, ehs_sint32 thread_count);

void EhsML_FW_TFLite_Destroy(EhsML_Context* ctx);

EhsML_Err EhsML_FW_TFLite_SetInputData(EhsML_Context* ctx, const void* input_data, ehs_uint32 data_size);

EhsML_Err EhsML_FW_TFLite_GetOutputData(EhsML_Context* ctx);

#ifdef __cplusplus
}
#endif

#endif//_C_HEADER_EHS_TARGET_HAL_ML_FRAMEWORK_TENSORFLOW_LITE_ERT_HAL_TFLITE_H_