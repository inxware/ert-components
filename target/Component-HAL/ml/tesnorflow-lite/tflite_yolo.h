#ifndef YOLO_MODEL_H
#define YOLO_MODEL_H

#include "tflite_model.h"

/* Non-Maximum-Suppression max buffer count */
#ifndef YOLO_NMS_MAX
#define YOLO_NMS_MAX (3 * EHS_ML_OBJ_DETECTIONS_MAX)
#endif

#define YOLO_ERROR_OK               0
#define YOLO_ERROR_INVALID_OUTPUT   1 << 1
#define YOLO_ERROR_INVALID_INPUT    1 << 2
#define YOLO_ERROR_INVAL_CLASS_ID   1 << 3
#define YOLO_ERROR_OBJ_DET_MAX      1 << 4
#define YOLO_ERROR_NMS_MAX          1 << 5
//#define YOLO_ERROR_               1 << 6
//#define YOLO_ERROR_               1 << 7

#ifdef __cplusplus
extern "C" {
#endif
// supports both float32 and float16 yolo models
TfLiteErr TfLiteRunYolo_Float(struct TfLiteModelCtx* ctx, struct TfLiteBoxDetections* detections);
// supports int8 and uint8 yolo models
TfLiteErr TfLiteRunYolo_Int8(struct TfLiteModelCtx* ctx, struct TfLiteBoxDetections* detections);

// @TODO - Add quantised options for Yolo
// TfLiteErr TfLiteRunYolo_Int32(struct TfLiteModelCtx* ctx, struct TfLiteBoxDetections* detections);
// ...

#ifdef __cplusplus
}
#endif

#endif // YOLO_MODEL_H