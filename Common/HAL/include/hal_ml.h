/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
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
#include "ehs_types.h"

// max number of the objects per detetction
#ifndef EHS_ML_OBJ_DETECTIONS_MAX
#define EHS_ML_OBJ_DETECTIONS_MAX 20
#endif

typedef enum {
    EHS_ML_OK = 0,
    EHS_ML_FAILED,
    EHS_ML_MEMORY_ERR,
    EHS_ML_MODEL_LOAD_ERR,
    EHS_ML_MODEL_IN_USE, // model already loaded
    EHS_ML_MODEL_PATH_ERR
} EhsML_Err;

typedef enum {
    EHS_ML_OBJ_DETECTOR = 0,
    EHS_ML_CLASSIFIER
    // ...
} EhsML_Type;

typedef struct {
    void* ml_model_ctx;
    EhsML_Type type;
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
EHS_GLOBAL EhsML_Err EhsML_Create(EhsML_Context* ctx, const ehs_char* model_path, EhsML_Type model_type, ehs_float conf_thres, ehs_sint32 thread_count);

// Frees and cleans up all resources associated with the ML context.
EHS_GLOBAL void EhsML_Destroy(EhsML_Context* ctx);

// Sets the input data for inference.
// - data: raw input (e.g., image/frame/text/bin data).
// - size: size of the input data in bytes.
// Returns an error code indicating success or failure.
EHS_GLOBAL EhsML_Err EhsML_SetInputData(EhsML_Context* ctx, const void* data, ehs_uint32 size);

// [TODO] Runs inference and outputs raw data into provided buffer.
// This function is not yet implemented.
// EHS_GLOBAL EhsML_Err EhsML_RunOutputData(EhsML_Context* ctx, void* data, ehs_uint32 size);

// Runs inference and writes the results to a JSON-formatted string.
// - json: output buffer for JSON result string (must be preallocated).
// - size: maximum size of the buffer to avoid overflow.
// Output format example: {"type":0, "res":[{"cls":0, "cnf":0.92, "x":...}, ...]}
// Returns an error code indicating success or failure.
EHS_GLOBAL EhsML_Err EhsML_RunOutputJson(EhsML_Context* ctx, ehs_char* json, ehs_uint32 size);


#endif // _EHS_HAL_ML_H