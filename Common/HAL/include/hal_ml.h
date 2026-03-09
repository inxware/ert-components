/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file hal_ml.h
 * In this file, all of the abstraction layer for machine learning.
 *
 * @author: inx limited
 *
 */

#ifndef _EHS_HAL_ML_H
#define _EHS_HAL_ML_H

/*****************************************************************************/
/* Included files */
#include "target_types.h"

// max number of the objects per detetction
#ifndef EHS_ML_OBJ_DETECTIONS_MAX
#define EHS_ML_OBJ_DETECTIONS_MAX 20
#endif

// Max number of dimensions for input/output tensors
#ifndef EHS_ML_TENSOR_MAX_DIMS
#define EHS_ML_TENSOR_MAX_DIMS 4
#endif//EHS_ML_TENSOR_MAX_DIMS

// Max number of tensors of input and output layers
#ifndef EHS_ML_LAYER_TENSORS_MAX
#define EHS_ML_LAYER_TENSORS_MAX 128
#endif//EHS_ML_LAYER_TENSORS_MAX


/* General inference engine errors */
typedef enum {
    EHS_ML_OK = 0,
    EHS_ML_FAILED,
    EHS_ML_MEMORY_ERR,
    EHS_ML_INIT_ERR,
    EHS_ML_INVALID_SIZE_ERR,
    EHS_ML_INVALID_QUANT_ERR,
    EHS_ML_MODEL_LOAD_ERR,
    EHS_ML_MODEL_CTX_ERR,
    EHS_ML_MODEL_TENSOR_DIM_ERR,
    EHS_ML_MODEL_IN_USE, // model already loaded
    EHS_ML_MODEL_PATH_ERR,
    EHS_ML_MODEL_NAME_ERR,
    EHS_ML_MODEL_TYPE_ERR,
    EHS_ML_MODEL_OUTPUT_ERR,
    EHS_ML_INFERENCE_ERR,
    EHS_ML_INVALID_FRAME_ID,
    EHS_ML_INVALID_FRAME,
    EHS_ML_INVALID_DEP,  // Invalid Library dependency (e.g. mismatched Hailo library version)
    EHS_ML_INVALID_TENSOR_ERR,
    EHS_ML_JSON_STRSIZE_ERR,
    EHS_ML_NOT_IMPLEMENTED,
    EHS_ML_NOT_SUPPORTED,
    EHS_ML_NULL_CTX_ERR,              // NULL EhsML_Context pointer passed to an API function
    EHS_ML_NULL_INPUT_ERR,            // NULL input data pointer passed to EhsML_SetInputData
    EHS_ML_NULL_JSON_BUF_ERR,         // NULL json output buffer passed to EhsML_RunOutputJson
    EHS_ML_INPUT_SIZE_MISMATCH_ERR,   // Input data size does not match the model's input tensor size
} EhsML_Err;

typedef enum {
    EHS_ML_FILE_EXT_START = 0,
    EHS_ML_FILE_EXT_TFLITE,
    EHS_ML_FILE_EXT_ONNX,
    EHS_ML_FILE_EXT_PB,
    EHS_ML_FILE_EXT_HEF,
    // ...
    EHS_ML_FILE_EXT_MAX
} EhsML_File_Ext_t;

typedef enum {
    EHS_ML_TYPE_TEST_RESERVED = 0, // For testing purposes
    EHS_ML_TYPE_STUBBED,
    /* Image */
    EHS_ML_TYPE_IMAGE_START = 1000,
    EHS_ML_YOLOV3_OBJ_DETECTOR,
    EHS_ML_YOLOV4_OBJ_DETECTOR,
    EHS_ML_YOLOV5_OBJ_DETECTOR,
    EHS_ML_YOLOV6_OBJ_DETECTOR,
    EHS_ML_YOLOV7_OBJ_DETECTOR,
    EHS_ML_YOLOV8_OBJ_DETECTOR,
    EHS_ML_YOLOV8_INSTANCE_SEGMENTER,
    EHS_ML_YOLOV8_POSE_ESTIMATOR,
    EHS_ML_YOLOV8_ORIENTED_BBOX_DETECTOR,
    EHS_ML_YOLOV8_CLASSIFIER,
    EHS_ML_YOLOV9_OBJ_DETECTOR,
    EHS_ML_YOLOV9_INSTANCE_SEGMENTER,
    EHS_ML_YOLOV10_OBJ_DETECTOR,
    EHS_ML_YOLOV11_OBJ_DETECTOR,
    EHS_ML_YOLOV11_INSTANCE_SEGMENTER,
    EHS_ML_YOLOV11_POSE_ESTIMATOR,
    EHS_ML_YOLOV11_ORIENTED_BBOX_DETECTOR,
    EHS_ML_YOLOV11_CLASSIFIER,
    EHS_ML_YOLOV12_OBJ_DETECTOR,
    EHS_ML_YOLOV12_INSTANCE_SEGMENTER,
    EHS_ML_YOLOV12_POSE_ESTIMATOR,
    EHS_ML_YOLOV12_ORIENTED_BBOX_DETECTOR,
    EHS_ML_YOLOV12_CLASSIFIER,
    EHS_ML_YOLOV26_OBJ_DETECTOR,
    EHS_ML_YOLOV26_INSTANCE_SEGMENTER,
    EHS_ML_YOLOV26_POSE_ESTIMATOR,
    EHS_ML_YOLOV26_ORIENTED_BBOX_DETECTOR,
    EHS_ML_YOLOV26_CLASSIFIER,
    EHS_ML_SAM_IMAGE_SEGMENTER,
    // ...
    EHS_ML_TYPE_IMAGE_MAX,  // Maximum value for image types
    /* Text */
    EHS_ML_TYPE_TEXT_START = 2000,
    EHS_ML_TRANSFORMER_TEXT_CLASSIFIER,
    EHS_ML_TRANSFORMER_SENTIMENT_ANALYZER,
    EHS_ML_TRANSFORMER_NAMED_ENTITY_RECOGNITION,
    EHS_ML_TRANSFORMER_TEXT_SUMMARIZER,
    EHS_ML_TRANSFORMER_LANGUAGE_TRANSLATOR,
    // ...
    EHS_ML_TYPE_TEXT_MAX,   // Maximum value for text types
    /* Audio */
    EHS_ML_TYPE_AUDIO_START = 3000,
    EHS_ML_AUDIO_SPEECH_RECOGNITION,
    EHS_ML_AUDIO_KEYWORD_SPOTTING,
    EHS_ML_AUDIO_SPEAKER_IDENTIFICATION,
    EHS_ML_AUDIO_NOISE_SUPPRESSION,
    EHS_ML_AUDIO_SOUND_CLASSIFICATION,
    EHS_ML_AUDIO_VOICE_ACTIVITY_DETECTION,
    // ...
    EHS_ML_TYPE_AUDIO_MAX,  // Maximum value for audio types
    EHS_ML_TYPE_MAX         // Maximum value for all types
} EhsML_Type;

typedef enum {
    EHS_ML_HWACCEL_NONE = 0,
    EHS_ML_HWACCEL_HAILO = 1,
    EHS_ML_HWACCEL_NVIDIA = 2,
    EHS_ML_HWACCEL_AMD = 3,
    EHS_ML_HWACCEL_EIQ = 4,
    EHS_ML_HWACCEL_DEEPX = 5,
    EHS_ML_HWACCEL_GEMMA = 6,
    EHS_ML_HWACCEL_CUSTOM_NPU=7,
    // ...
    EHS_ML_HWACCEL_MAX
} EhsML_HWAccel_t;

typedef enum {
    EHS_ML_DATATYPE_NONE = 0,
    EHS_ML_DATATYPE_UINT8,
    EHS_ML_DATATYPE_INT8,
    EHS_ML_DATATYPE_UINT16,
    EHS_ML_DATATYPE_INT16,
    EHS_ML_DATATYPE_UINT32,
    EHS_ML_DATATYPE_INT32,
    EHS_ML_DATATYPE_UINT64,
    EHS_ML_DATATYPE_INT64,
    EHS_ML_DATATYPE_FP16,
    EHS_ML_DATATYPE_FP32,
    EHS_ML_DATATYPE_FP64,
    EHS_ML_DATATYPE_COMPLEX64,
    EHS_ML_DATATYPE_COMPLEX128,
    EHS_ML_DATATYPE_BOOL,
    EHS_ML_DATATYPE_RAW,
    EHS_ML_DATATYPE_CONST_RAW,
    // ...
    EHS_ML_DATATYPE_MAX
} EhsML_DataType_t;

// Value type used for inputs/outputs according to the quantisation level
typedef struct {
    float real;
    float imag;
} EhsML_Complex64_t;

typedef struct {
    double real;
    double imag;
} EhsML_Complex128_t;

typedef struct {
    ehs_uint16 value;
} EhsML_Float16_t;

typedef union {
    ehs_uint8*          u8;
    ehs_sint8*          s8;
    ehs_uint16*         u16;
    ehs_sint16*         s16;
    ehs_uint32*         u32;
    ehs_sint32*         s32;
    ehs_uint64*         u64;
    ehs_sint64*         s64;
    EhsML_Float16_t*    f16;
    float*              f32;
    double*             f64;
    EhsML_Complex64_t*  c64;
    EhsML_Complex128_t* c128;
    ehs_bool*           b;
    ehs_char*           raw;
    const ehs_char*     const_raw;
    // As arbitrary pointer
    void*               ptr;
} EhsML_DataPtr_U;

typedef struct {
    double scale;
    double offset;
} EhsML_Quan_t;

typedef struct {
    EhsML_DataPtr_U data_ptr;
    size_t          size_in_bytes;
    // This includes the length of each dimension
    ehs_uint32      dims[EHS_ML_TENSOR_MAX_DIMS];
    ehs_uint32      num_dims;
    EhsML_Quan_t    quantisation_params;
    EhsML_DataType_t data_type;
    void            *handle;       /* Placeholder for any framework-specific tensor handle if needed */
    ehs_bool        handle_owned;  /* If true, EhsML_Tensor_Free() will free handle */
    ehs_bool        data_ptr_owned; /* If true, EhsML_Tensor_Free() will free data_ptr.
                                     * Set to EHS_FALSE for zero-copy tensors whose buffer
                                     * is owned by the inference framework (TFLite, Hailo). */
} EhsML_Tensor_t;

typedef struct {
    ehs_float conf; // confidence score (0.0-1.0)
    ehs_uint32 cls; // class index
    ehs_bool filtered; // whether the detection is filtered out by post-processing
    // Bounding box coordinates (x, y, w, h) - can be absolute pixel values or relative (0.0-1.0) depending on the model output
    ehs_float x;
    ehs_float y;
    ehs_float w;
    ehs_float h;
    ehs_char label[64]; // Optional: class label string (if available)
} EhsML_Detection_t;

typedef struct {
    void* ml_model_ctx;
    // It is not necessary to have the input_tensor
    //  because additional copies cost further time as the result of too much abstraction
    // This might still serve as the placeholder for potential storage of input data
    ehs_uint32 input_tensor_count;
    EhsML_Tensor_t input_tensor[EHS_ML_LAYER_TENSORS_MAX];
    // The output_tensor is a valid cost of the abstraction
    //  because the output format could be different for various ML frameworks
    ehs_uint32 output_tensor_count;
    EhsML_Tensor_t output_tensor[EHS_ML_LAYER_TENSORS_MAX];
    ehs_uint32 intermediate_tensor_count;
    EhsML_Tensor_t intermediate_tensor[EHS_ML_LAYER_TENSORS_MAX]; // Placeholder for intermediate tensors if needed for processing
    ehs_uint32 detection_count;
    EhsML_Detection_t detections[EHS_ML_LAYER_TENSORS_MAX];
    EhsML_Type type;
    EhsML_HWAccel_t hw_accel;
    EhsML_DataType_t data_type;
    ehs_float conf_thres; // confidence threshold (0.0-1.0)
    // if supported creates json without arrays e.g.
    // {"type":0,...,"cls0":0,"cnf0":0.00,"x0":0,"y0":0,"w0":0,"h0":0, ... , "clsN":0,"cnfN":0.00, ... }
    ehs_bool enable_flat_json;
    // ... other ?
} EhsML_Context;

// Creates and initializes a machine learning context for a given model.
// - ctx: pointer to an allocated EhsML_Context structure.
// - model_path: path to the ML model file.
// - model_type: type of the ML model (e.g., object detector, classifier, etc).
// - conf_thres: confidence threshold for post-processing predictions.
// - thread_count: number of threads used for processing the model
// Returns an error code indicating success or failure.
EhsML_Err EhsML_Create(EhsML_Context* ctx, const ehs_char* model_path, EhsML_Type model_type, ehs_float conf_thres, ehs_sint32 thread_count);

// Frees and cleans up all resources associated with the ML context.
void EhsML_Destroy(EhsML_Context* ctx);

// Sets the input data for inference.
// - data: raw input (e.g., image/frame/text/bin data).
// - size: size of the input data in bytes.
// Returns an error code indicating success or failure.
EhsML_Err EhsML_SetInputData(EhsML_Context* ctx, const void* data, ehs_uint32 size);

// [TODO] Runs inference and outputs raw data into provided buffer.
// This function is not yet implemented.
// EhsML_Err EhsML_RunOutputData(EhsML_Context* ctx, void* data, ehs_uint32 size);

// Runs inference and writes the results to a JSON-formatted string.
// - json: output buffer for JSON result string (must be preallocated).
// - size: maximum size of the buffer to avoid overflow.
// Output format example: {"type":0, "res":[{"cls":0, "cnf":0.92, "x":...}, ...]}
// Returns an error code indicating success or failure.
EhsML_Err EhsML_RunOutputJson(EhsML_Context* ctx, ehs_char* json, ehs_uint32 size);

/**
 * @brief Get the currently supported hardware acceleration type
 *
 * @return EhsML_HWAccel_t The hardware acceleration currently supported.
 */
EhsML_HWAccel_t EhsML_HWAccel_supported( void );

/**
 * @brief Build a JSON string describing a loaded ML model.
 *
 * Combines framework C-API tensor introspection with model file metadata
 * parsing to produce a self-contained JSON object describing the model's
 * inputs, outputs, inferred type, acceleration status, and any embedded
 * metadata.  Intended for diagnostics and for future exposure as a
 * function block output port.
 *
 * @param ctx        Initialised EhsML_Context (EhsML_Create must have succeeded).
 * @param model_path Path to the model file (used for metadata extraction).
 * @param json_buf   Caller-provided output buffer.
 * @param json_size  Size of output buffer in bytes.
 * @return EHS_ML_OK on success, error code otherwise.
 */
EhsML_Err EhsML_GetModelInfoJson(EhsML_Context* ctx, const ehs_char* model_path,
                                  ehs_char* json_buf, ehs_uint32 json_size);

/**
 * @brief Create and allocate a tensor
 * 
 * @param tensor The tensor to create
 * @param data_type Quantisation level of the tensor
 * @param dims List of dimensions
 * @param num_dims The number of dimensions
 * @return EhsML_Err EHS_ML_OK if successful, error code otherwise
 * 
 * @code {.C}
 * EhsML_Tensor_t tensor;
 * ehs_uint32 dims[3] = {5, 10, 32};
 * EhsML_Err err = EhsML_Tensor_Alloc(&tensor, EHS_ML_DATATYPE_FP16, dims, 3);
 * @endcode
 * 
 */
EhsML_Err EhsML_Tensor_Alloc(EhsML_Tensor_t* tensor, EhsML_DataType_t data_type, const ehs_uint32* dims, ehs_uint32 num_dims);

/**
 * @brief Free the allocated tensor. If tensor is NULL, does nothing.
 * 
 * @param tensor The allocated tensor to free
 */
void EhsML_Tensor_Free(EhsML_Tensor_t* tensor);

/**
 * @brief Fill the tensor with raw data. The size of the data must match the tensor size.
 * 
 * @param tensor The tensor to fill
 * @param value Pointer to the raw data
 * @param size_in_bytes Size of the raw data in bytes
 * @return EhsML_Err EHS_ML_OK if successful, EHS_ML_MEMORY_ERR if size does not match, EHS_ML_INIT_ERR if the destination tensor is not allocated, error code otherwise
 * @code {.C}
 * EhsML_Tensor_t tensor;
 * // Assume tensor is already allocated
 * ehs_char* data = ...; // Pointer to raw data
 * size_t data_size = ...; // Size of raw data in bytes
 * EhsML_Err err = EhsML_Tensor_FillRaw(&tensor, data, data_size * sizeof(ehs_char));
 * @endcode
 */
EhsML_Err EhsML_Tensor_FillRaw(EhsML_Tensor_t* tensor, ehs_char *value, size_t size_in_bytes);


#endif // _EHS_HAL_ML_H
