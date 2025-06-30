#ifndef TFLITE_MODEL_H
#define TFLITE_MODEL_H

#include <tensorflow/lite/c/c_api.h>
#include "hal_ml.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    TF_LITE_OK = 0,
    TF_LITE_FAILED
} TfLiteErr;

typedef enum {
    TF_LITE_MODEL_UNKNWON = 0,
    TF_LITE_MODEL_DETECTOR,
    TF_LITE_MODEL_CLASSIFIER,     // @TODO 
    TF_LITE_MODEL_POSE_ESTIMATOR  // @TODO
    // ... Other?
} TfLiteModelType;

// Forward declaration
struct TfLiteModelCtx;
struct TfLiteBoxDetection;
struct TfLiteBoxDetections;
// ...

////////////////////////////////////////////////////////////////////////
//* Object Detector *  e.g YOLOv5, YOLOv8, SSD MobileNet, CenterNet
// Output:
// Bounding boxes + class + confidence (e.g., x, y, w, h, class, score/confidence)
typedef struct TfLiteBoxDetection {
    int   class_id;
    float confidence;
    unsigned int x, y, w, h; /* pixel units, relative to network input size */
} TfLiteBoxDetection;

typedef struct TfLiteBoxDetections {
    TfLiteBoxDetection box_detections[EHS_ML_OBJ_DETECTIONS_MAX]; // box detections buffer
    int box_detections_count;    // number of detections found in model run
    int processing_time_ms;     // time taken to obtain detections from the model
    int error_id;               // error id
} TfLiteBoxDetections;

// Define the detector function pointer type
typedef TfLiteErr (*TfLiteDetectorRunFn)(struct TfLiteModelCtx* ctx, struct TfLiteBoxDetections* detections);
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////
//* Classifiers *  e.g MobileNet V1/V2, EfficientNet, ResNet
// Output:
// 1D array of class probabilities (e.g. [0.01, 0.99, 0.0, ...])
// Top-1 or Top-N class predictions
// @TODO
// Define the classifier function pointer type
// typedef TfLiteErr (*TfLiteClassifierRunFn)(struct TfLiteModelCtx* ctx, struct TfLiteClassifier* classifier);
////////////////////////////////////////////

////////////////////////////////////////////
//* Pose Estimators *  e.g BlazePose, MoveNet
// Output:
// List of keypoints (x, y, confidence) for each body part
// @TODO
// Define the pose estimator function pointer type
// typedef TfLiteErr (*TfLitePoseEstRunFn)(struct TfLiteModelCtx* ctx, struct TfLitePoseEst* pose_est);
////////////////////////////////////////////

////////////////////////////////////////////
//* Other *
// Output:
// @TODO
////////////////////////////////////////////

/* Tensorflow-lite model context */
typedef struct TfLiteModelCtx {
    TfLiteModel*              model;            /* owns the flat-buffer */
    TfLiteInterpreterOptions* options;          /* holds thread count etc. */
    TfLiteInterpreter*        interp;           /* ready-to-run interpreter */
    TfLiteDelegate*           xnn_delegate;     /* used for enabling XNNPACK */
    TfLiteType                type;             /* model data type e.g. float32, float16, int8 etc. */
    TfLiteModelType           model_type;       /* model type detector, classifier, pose ... , etc. */
    const TfLiteTensor*       in_tensor;
    const TfLiteTensor*       out_tensor;

    float                     conf_thres;       /* confidence threshold */

    TfLiteDetectorRunFn       fnRunDetector;    /* detector callback e.g. yolo_float16, yolo_int8 it gets assiged in TfLiteModel_Create */
    //TfLiteClassifierRunFn   fnRunClassifier; //@TODO
} TfLiteModelCtx;

/* Create - returns TF_LITE_OK on success. */
TfLiteErr TfLiteModel_Create(TfLiteModelCtx* ctx, const char* model_path, int num_threads);

/* Destroy – safe to call on a zero-initialised or already-destroyed ctx. */
void TfLiteModel_Destroy(TfLiteModelCtx* ctx);

/* Sets input data */
TfLiteErr TfLiteModel_Set_Input_Data(TfLiteModelCtx* ctx, const void* data, unsigned int size);

/* Run Detector - returns TF_LITE_OK on success. */
TfLiteErr TfLiteModel_Run_Detector(TfLiteModelCtx* ctx, TfLiteBoxDetections* detections);

/* Run Classifier - returns TF_LITE_OK on success. */
//TfLiteErr TfLiteModel_Run_Classifier(TfLiteModelCtx* ctx, TfLiteClassifier* classifier);

// ...

#ifdef __cplusplus
}
#endif
#endif /* TFLITE_MODEL_H */
