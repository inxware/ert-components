/***************************************************************
* Copyright (C) 2008-2025 inx limited - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/** @file yolov8_objdet.c
 * Machine Learning model implementation for YOLOv8 object detection.
 *
 * @author: inx limited
 */


/**
 * @page Verification Verification report
 * @section hal_graphics_font
 * @anchor hal_graphics_font
 * @subsection misra MISRA compliance:
 * test.c demonstrated MISRA compliant on
 * Last modified on $Date:$
 *
 * This file contained no derogations to the MISRA standard.
 */

/*****************************************************************************/
/* Included files */
#include "yolov8_objdet.h"
#include "ehs_ml_objdet_output.h"   /* EhsML_Coco80_Label, EhsML_ObjDet_Json_AppendCorner, EhsML_ObjDet_Json_FromDetections */
#ifdef EHS_ML_HWACCEL_SUPPORT_NVIDIA
#include "ert_trt_nms_decode.h"
#endif

/*****************************************************************************/
/* Declare macros and local typedefs used by this file */


/*****************************************************************************/
/* Declare prototypes of local functions */

/*****************************************************************************/
/* Variables defined with file-scope */

/*****************************************************************************/
/* Variables defined with global-scope */

/*****************************************************************************/
/* Function definitions */

/* Label lookup: EhsML_Coco80_Label() from ehs_ml_objdet_output.h (0-indexed, 0="person") */

/**
 * @brief Run YOLOv8 object-detection inference and serialise results to JSON.
 *
 * Expected output tensor format (SSD/TFLite Object Detection API style):
 *
 *   The single output tensor is a flat FP32 array encoded as:
 *
 *     For each of the 80 COCO classes (outer loop, class_idx 0–79):
 *       [count]  — number of boxes detected for this class (float cast to size_t)
 *       For each box:
 *         [y_min, x_min, y_max, x_max]  — corner coordinates, normalised 0–1
 *         [confidence]                  — confidence score for this box
 *
 *   Coordinates are normalised to [0,1] relative to the model input dimensions
 *   and are scaled to pixel values using ctx->input_tensor[0].dims[1] (height)
 *   and ctx->input_tensor[0].dims[2] (width).
 *
 * NOTE: This decoder targets the TFLite Object Detection API output format
 * (count-prefixed, corner coordinates). It is NOT compatible with the standard
 * Ultralytics YOLOv8 TFLite export, whose output shape is [1, 4+classes, anchors].
 * Using a standard Ultralytics export will produce incorrect results.
 *
 * NOTE: Only FP32 output tensors are supported. Quantised (INT8/UINT8) models
 * are rejected at runtime with EHS_ML_MODEL_OUTPUT_ERR. To support quantised
 * models, dequantise using ctx->output_tensor[0].quantisation_params.scale and
 * .offset before interpreting the decoded values.
 *
 * JSON output schema:
 *   {"cls0":"person","cnf0":0.87,"ymin0":45.2,"xmin0":120.1,"ymax0":380.0,"xmax0":290.4,...,"det_cnt":N}
 *   Coordinates are in pixels (corner format), ordered ymin/xmin/ymax/xmax.
 *   Only detections with confidence >= ctx->conf_thres are emitted.
 *   No NMS is applied — the model output is assumed to be pre-filtered.
 *
 * @param ctx         Initialised ML context with output_tensor[0] populated.
 * @param json_output Caller-allocated buffer for JSON output.
 * @param output_size Size of json_output in bytes.
 * @return EHS_ML_OK on success, EHS_ML_MODEL_OUTPUT_ERR if data type is not FP32.
 */
EhsML_Err EhsML_Yolov8_ObjDet_RunPipeline(EhsML_Context* ctx)
{
    EhsML_Err err = EhsML_InfEngine_RunInference(ctx);
    if (err != EHS_ML_OK)
    {
        return err;
    }

#ifdef EHS_ML_HWACCEL_SUPPORT_NVIDIA
    /* TensorRT NMS-plugin path: engine delivers 4 canonical output tensors.
     * Decode into ctx->detections[] — serialisation is done by EhsML_GetOutput(). */
    if (ctx->hw_accel == EHS_ML_HWACCEL_NVIDIA)
    {
        return EhsML_TRT_NMS_Decode(ctx);
    }
#endif

    /* TFLite SSD path (count-prefixed, corner coordinates).
     *
     * Expected output tensor format:
     *   For each of the 80 COCO classes:
     *     [count]  number of boxes for this class (float cast to size_t)
     *     For each box: [y_min, x_min, y_max, x_max, confidence]
     *   Coordinates normalised [0,1], scaled to pixels below.
     *
     * Guard: this decoder only handles FP32 output. */
    if (ctx->output_tensor[0].data_type != EHS_ML_DATATYPE_FP32)
    {
        EHSH_LOG_ERROR("yolov8_objdet: output tensor data type %d is not FP32 — "
                       "this decoder only supports float32 output\n",
                       (int)ctx->output_tensor[0].data_type);
        return EHS_ML_MODEL_OUTPUT_ERR;
    }

    int input_width  = ctx->input_tensor[0].dims[2];  /* W: dims = [batch, H, W, C] */
    int input_height = ctx->input_tensor[0].dims[1];  /* H */

    size_t class_idx = 0;
    size_t index = -1;
    ctx->detection_count = 0;

    while (class_idx < 80)
    {
        size_t num_of_class_boxes = (size_t)ctx->output_tensor[0].data_ptr.f32[++index];

        for (size_t box_idx = 0 ; box_idx < num_of_class_boxes ; box_idx++)
        {
            float y_min = ctx->output_tensor[0].data_ptr.f32[++index] * input_height;
            float x_min = ctx->output_tensor[0].data_ptr.f32[++index] * input_width;
            float y_max = ctx->output_tensor[0].data_ptr.f32[++index] * input_height;
            float x_max = ctx->output_tensor[0].data_ptr.f32[++index] * input_width;
            float conf  = ctx->output_tensor[0].data_ptr.f32[++index];

            if (conf >= ctx->conf_thres && ctx->detection_count < EHS_ML_OBJ_DETECTIONS_MAX)
            {
                EhsML_Detection_t *d = &ctx->detections[ctx->detection_count++];
                d->conf     = conf;
                d->cls      = (ehs_uint32)class_idx;
                d->filtered = EHS_FALSE;
                d->x        = (x_min + x_max) * 0.5f;
                d->y        = (y_min + y_max) * 0.5f;
                d->w        = x_max - x_min;
                d->h        = y_max - y_min;
            }
        }
        class_idx++;
    }

    return EHS_ML_OK;
}
