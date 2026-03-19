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
    EHS_ML_NULL_JSON_BUF_ERR,         // NULL output buffer passed to EhsML_GetOutput / EhsML_RunAndGetOutput
    EHS_ML_INPUT_SIZE_MISMATCH_ERR,   // Input data size does not match the model's input tensor size
    EHS_ML_BUSY,                      // Inference already in progress; request dropped
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

/* ************************************************************************************************
 * Pipeline capability map
 *
 * Each EhsML_Context carries a per-stage descriptor that records which
 * technology handles each canonical pipeline stage and whether the stage
 * is implemented, absorbed by the preceding stage, or absent.
 *
 * Intended uses:
 *   1. Runtime gap detection — EHS_ML_STAGE_NOT_IMPLEMENTED stages can be
 *      reported back to the function block as EHS_ML_NOT_IMPLEMENTED.
 *   2. Diagnostics / model-info JSON — expose the pipeline map to the
 *      application so it knows which stages are hardware-accelerated.
 *   3. (Roadmap) Pipeline builder — a future builder can inspect the map
 *      after engine Create and fill NOT_IMPLEMENTED stages by inserting
 *      generic eRT implementations.  The fn pointer is reserved for this
 *      purpose; it is always NULL in current monolithic call chains.
 *
 * Population contract:
 *   - Engine Create (ert_hal_tflite.c, ert_hal_hailo.c, etc.) writes the
 *     stages it handles (INFER, UNPACK, and any SDK-covered post-processing).
 *   - Model RunOutputJson (yolov5_objdet.c, etc.) writes the remaining
 *     model/generic stages (DEQUANT, DECODE, LOGICAL, FORMAT).
 *   - Stages not touched by either are left zeroed (NOT_IMPLEMENTED).
 * *********************************************************************************************** */

/** Canonical pipeline stages, in execution order. */
typedef enum {
    EHS_ML_STAGE_INFER   = 0,  /**< Engine runs model arithmetic */
    EHS_ML_STAGE_UNPACK  = 1,  /**< Engine-specific tensor unpacking into ctx->output_tensor[] */
    EHS_ML_STAGE_DEQUANT = 2,  /**< Dequantisation (scale × (raw − zero_point) → float) */
    EHS_ML_STAGE_DECODE  = 3,  /**< Model-architecture decode (anchor, DFL, grid offsets) */
    EHS_ML_STAGE_LOGICAL = 4,  /**< Logical post-processing (NMS, confidence threshold, …) */
    EHS_ML_STAGE_FORMAT  = 5,  /**< Output serialisation (JSON, binary, Protobuf, …) */
    EHS_ML_STAGE_COUNT   = 6
} EhsML_PipelineStage_t;

/**
 * Which technology is responsible for a stage.
 *
 * This enum serves double duty as both a technology identifier and a
 * stage-status indicator, eliminating the need for a separate status enum:
 *
 *   EHS_ML_TECH_NONE (0)          — stage not implemented (zero-initialised default)
 *   EHS_ML_TECH_COVERED_BY_PREV   — stage absorbed by the preceding stage's technology
 *                                    (e.g. TensorRT NMS plugin handling decode+logical
 *                                    as part of inference; HailoRT dequantising internally)
 *   Any other value               — stage implemented by the named technology
 *
 * Because NONE is zero, a fully zeroed EhsML_Pipeline_t correctly represents
 * an uninitialised pipeline where all stages are not implemented.
 */
typedef enum {
    EHS_ML_TECH_NONE            = 0,  /**< Not implemented — gap in pipeline */
    EHS_ML_TECH_COVERED_BY_PREV = 1,  /**< Absorbed by preceding stage's technology */
    EHS_ML_TECH_TFLITE          = 2,  /**< TFLite runtime */
    EHS_ML_TECH_TFLITE_MICRO    = 3,  /**< TFLite Micro runtime */
    EHS_ML_TECH_HAILORT         = 4,  /**< HailoRT SDK (Hailo NPU) */
    EHS_ML_TECH_TENSORRT        = 5,  /**< TensorRT SDK (NVIDIA Jetson GPU/DLA) */
    EHS_ML_TECH_ERT_MODEL       = 6,  /**< eRT model-specific code (postprocessing/model/) */
    EHS_ML_TECH_ERT_GENERIC     = 7,  /**< eRT shared/generic code (postprocessing/logical/ etc.) */
    /* Future: EHS_ML_TECH_RKNN, EHS_ML_TECH_COREML, EHS_ML_TECH_EIQ, … */
} EhsML_StageTech_t;

/**
 * Roadmap hook: pointer to a discrete single-stage pipeline function.
 *
 * All stages except FORMAT share the signature (void *ctx) → EhsML_Err,
 * where ctx is cast to EhsML_Context*.  FORMAT additionally needs a JSON
 * output buffer and size; to fit the uniform signature those would be stored
 * as transient fields in EhsML_Context before the pipeline runner executes
 * (a future addition — not present today).
 *
 * NULL = stage is embedded in a larger monolithic RunOutputJson call (all
 * current flows).  The pipeline runner is a future feature; fn exists as a
 * defined hook so the ABI and struct layout are stable when it is added.
 */
typedef EhsML_Err (*EhsML_StageFn_t)(void *ctx);

/**
 * Descriptor for one canonical pipeline stage.
 *
 * tech encodes both status and identity:
 *   NONE             → not implemented
 *   COVERED_BY_PREV  → absorbed by preceding stage
 *   anything else    → implemented by that technology
 */
typedef struct {
    EhsML_StageTech_t  tech;  /**< Technology (or NONE / COVERED_BY_PREV) */
    EhsML_StageFn_t    fn;    /**< Roadmap: discrete stage fn; always NULL in current flows */
} EhsML_StageInfo_t;

/** Complete pipeline capability map — one EhsML_StageInfo_t per canonical stage. */
typedef struct {
    EhsML_StageInfo_t stages[EHS_ML_STAGE_COUNT];
} EhsML_Pipeline_t;

/**
 * Convenience macro: set one stage entry.
 * fn is always NULL (monolithic flows; roadmap hook only).
 *
 * Usage:
 *   EHS_ML_STAGE_SET(ctx->pipeline, EHS_ML_STAGE_INFER,  EHS_ML_TECH_TFLITE);
 *   EHS_ML_STAGE_SET(ctx->pipeline, EHS_ML_STAGE_DEQUANT, EHS_ML_TECH_COVERED_BY_PREV);
 */
#define EHS_ML_STAGE_SET(pipeline, stage, _tech)        \
    do { (pipeline).stages[(stage)].tech = (_tech);     \
         (pipeline).stages[(stage)].fn   = NULL; } while (0)

/* ************************************************************************************************
   This is the main ML engine and post processing handle that is passed down the inference pipeline
   within a function block, but could also be passed between function blocks for graphical methods
    of post-processing if that becomes useful to application developers .
   ********************************************************************************************** */

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

    /**
     * Pipeline capability map.
     *
     * Populated during EhsML_Create (engine layer fills INFER/UNPACK and any
     * SDK-covered stages) and during EhsML_Run (model layer fills DEQUANT/
     * DECODE/LOGICAL/FORMAT).  Stages left zeroed after Create are NOT_IMPLEMENTED
     * gaps detectable at runtime.
     *
     * Use EHS_ML_STAGE_SET(pipeline, stage, tech) to populate individual stages.
     */
    EhsML_Pipeline_t pipeline;

    /**
     * Busy flag — set to EHS_TRUE for the duration of EhsML_Run() through
     * EhsML_GetOutput().  Any call to EhsML_Run() or EhsML_SetInputData()
     * while this is EHS_TRUE returns EHS_ML_BUSY immediately, dropping the
     * request without corrupting the in-progress inference.
     *
     * Declared volatile so that the check is not optimised away by the
     * compiler when the flag is set/read from different call contexts
     * (e.g. a timer callback vs. the inference call).
     */
    volatile ehs_bool inferring;
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

/**
 * @brief Run the full inference pipeline for the loaded model.
 *
 * Executes: inference → unpack → dequant → decode → logical (NMS etc.).
 * Results are written into ctx->detections[] / ctx->detection_count.
 * No output buffer is involved; call EhsML_GetOutput() afterwards to
 * serialise the results into the desired format.
 *
 * @param ctx  Initialised EhsML_Context (EhsML_Create must have succeeded).
 * @return EHS_ML_OK on success, error code otherwise.
 */
EhsML_Err EhsML_Run(EhsML_Context* ctx);

/**
 * @brief Serialise the pipeline results into an output buffer.
 *
 * Dispatches to the formatter appropriate for ctx->type.  Currently all
 * supported model types produce a JSON string.
 * Output format example: {"type":0,"det_cnt":N,"res":[{"cls":0,"cnf":0.92,...}]}
 *
 * Must be called after a successful EhsML_Run().
 *
 * @param ctx   Initialised EhsML_Context after EhsML_Run() has completed.
 * @param buf   Caller-provided output buffer (must be preallocated, non-NULL).
 * @param size  Size of the buffer in bytes.
 * @return EHS_ML_OK on success, EHS_ML_NULL_JSON_BUF_ERR if buf is NULL,
 *         error code otherwise.
 */
EhsML_Err EhsML_GetOutput(EhsML_Context* ctx, ehs_char* buf, ehs_uint32 size);

/**
 * @brief Convenience wrapper: EhsML_Run() followed by EhsML_GetOutput().
 *
 * Equivalent to:
 *   err = EhsML_Run(ctx);
 *   if (err == EHS_ML_OK) err = EhsML_GetOutput(ctx, buf, size);
 *   return err;
 *
 * Replaces EhsML_RunOutputJson() at call sites.  The old name is kept as a
 * backwards-compatible alias until all callers have been updated.
 *
 * @param ctx   Initialised EhsML_Context.
 * @param buf   Caller-provided output buffer (must be preallocated, non-NULL).
 * @param size  Size of the buffer in bytes.
 * @return EHS_ML_OK on success, error code otherwise.
 */
EhsML_Err EhsML_RunAndGetOutput(EhsML_Context* ctx, ehs_char* buf, ehs_uint32 size);

/** @deprecated Use EhsML_RunAndGetOutput() instead. */
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
