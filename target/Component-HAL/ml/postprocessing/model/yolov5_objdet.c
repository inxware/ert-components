/***************************************************************
* Copyright (C) 2008-2025 inx limited - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/** @file yolov5_objdet.c
 * Machine Learning model implementation for YOLOv5 object detection.
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
#include "hal-api.h"
#include "yolov5_objdet.h"
#include "ml_utils/ehs_ml_nms.h"
#include "ml_utils/ehs_ml_dequant.h"    /* EHS_ML_TENSOR_DEQUANT */
#include "ehs_ml_objdet_output.h"       /* EhsML_Coco80_Label, EhsML_ObjDet_Json_AppendCentre */

/*****************************************************************************/
/* Declare macros and local typedefs used by this file */
#ifndef YOLO_NMS_MAX
#define YOLO_NMS_MAX (3 * EHS_ML_OBJ_DETECTIONS_MAX)
#endif

#define YOLO_ATTRIB_CNT 5 /* 4 box coordinates + 1 objectness */

/* Dequantisation: see ml_utils/ehs_ml_dequant.h — EHS_ML_TENSOR_DEQUANT */


/*****************************************************************************/
/* Declare prototypes of local functions */

/*****************************************************************************/
/* Variables defined with file-scope */

/*****************************************************************************/
/* Variables defined with global-scope */

/*****************************************************************************/
/* Function definitions */

EhsML_Err EhsML_Yolov5_ObjDet_RunPipeline(EhsML_Context* ctx)
{
    EhsML_Err err = EhsML_InfEngine_RunInference(ctx);
    if (err != EHS_ML_OK)
    {
        return err;
    }

    /* 2. Process the output */
    NMSBox boxes[YOLO_NMS_MAX] = {0};
    int nms_count = 0;
    int i = 0;
    int num_detections = ctx->output_tensor[0].dims[1];
    int elements_per_detection = ctx->output_tensor[0].dims[2];
    int num_classes = elements_per_detection - YOLO_ATTRIB_CNT;

    int input_width = ctx->input_tensor[0].dims[2];
    int input_height = ctx->input_tensor[0].dims[1];

    double scale = ctx->output_tensor[0].quantisation_params.scale;
    double zero_point = ctx->output_tensor[0].quantisation_params.offset;
#if 0
    printf("[YOLOV5_DBG] output tensor: dims[0]=%d dims[1]=%d dims[2]=%d "
           "num_detections=%d elements_per_detection=%d num_classes=%d\n",
           ctx->output_tensor[0].dims[0], ctx->output_tensor[0].dims[1],
           ctx->output_tensor[0].dims[2],
           num_detections, elements_per_detection, num_classes);
    printf("[YOLOV5_DBG] conf_thres=%.3f scale=%.6f zero_point=%.3f\n",
           (double)ctx->conf_thres, scale, zero_point);
#endif
    /* Track rejection reasons */
    int rej_no_class = 0, rej_zero_score = 0, rej_below_thres = 0;
    double max_score_seen = 0.0;

    for (i = 0 ; i < num_detections ; ++i)
    {
        NMSBox box = {0};

        /* Dequantize box coordinates using the shared type-dispatched macro */
        EhsML_Tensor_t* t = &ctx->output_tensor[0];
        EHS_ML_TENSOR_DEQUANT(box.x, t, i * elements_per_detection + 0, zero_point, scale);
        EHS_ML_TENSOR_DEQUANT(box.y, t, i * elements_per_detection + 1, zero_point, scale);
        EHS_ML_TENSOR_DEQUANT(box.w, t, i * elements_per_detection + 2, zero_point, scale);
        EHS_ML_TENSOR_DEQUANT(box.h, t, i * elements_per_detection + 3, zero_point, scale);

        /* Objectness score */
        double objectness;
        EHS_ML_TENSOR_DEQUANT(objectness, t, i * elements_per_detection + 4, zero_point, scale);

        /* Find max class score */
        float max_class_score = 0.0f;
        int class_id = -1;
        for (int j = 0; j < num_classes; ++j) {
            double class_score;
            EHS_ML_TENSOR_DEQUANT(class_score, t, i * elements_per_detection + YOLO_ATTRIB_CNT + j, zero_point, scale);
            if (class_score > max_class_score) {
                max_class_score = class_score;
                class_id = j;
            }
        }

        if (class_id < 0) {
            rej_no_class++;
            continue;
        }

        box.class_id = class_id;
        box.score = objectness * max_class_score;
        if (box.score > max_score_seen) max_score_seen = box.score;

        if (!box.score) { rej_zero_score++; continue; }

        if (box.score >= ctx->conf_thres) {
#if 0
            printf("[YOLOV5_DBG] det[%d]: obj=%.3f cls_score=%.3f combined=%.3f "
                   "cls=%d x=%.1f y=%.1f w=%.1f h=%.1f -> ACCEPTED\n",
                   i, objectness, (double)max_class_score, box.score,
                   class_id, box.x, box.y, box.w, box.h);
#endif
            boxes[nms_count++] = box;
            if (nms_count >= YOLO_NMS_MAX) {
                break;
            }
        } else {
            rej_below_thres++;
        }
    }
#if 0
    printf("[YOLOV5_DBG] scan done: pre_nms=%d rej_no_class=%d rej_zero=%d "
           "rej_below_thres=%d max_score_seen=%.4f conf_thres=%.3f\n",
           nms_count, rej_no_class, rej_zero_score, rej_below_thres,
           max_score_seen, (double)ctx->conf_thres);
#endif

    /* 3. (Optional) NMS (Greedy or Soft) algorithm on object detection */
    size_t keep[YOLO_NMS_MAX];
    size_t scratch_i[YOLO_NMS_MAX];
    uint8_t scratch_f[YOLO_NMS_MAX];
    size_t keep_count = EhsApply_Greedy_NMS(
        boxes, nms_count,
        ctx->conf_thres, 0.45f,
        1, /*class-aware*/
        keep, YOLO_NMS_MAX,
        scratch_i, scratch_f
    );

    /* 4. Populate canonical detection list */
    ctx->detection_count = 0;
    for (i = 0; i < (int)keep_count && ctx->detection_count < EHS_ML_OBJ_DETECTIONS_MAX; i++)
    {
        EhsML_Detection_t *d = &ctx->detections[ctx->detection_count++];
        d->conf     = (ehs_float)boxes[keep[i]].score;
        d->cls      = (ehs_uint32)boxes[keep[i]].class_id;
        d->filtered = EHS_FALSE;
        d->x        = (ehs_float)boxes[keep[i]].x;
        d->y        = (ehs_float)boxes[keep[i]].y;
        d->w        = (ehs_float)boxes[keep[i]].w;
        d->h        = (ehs_float)boxes[keep[i]].h;
    }
    return EHS_ML_OK;
}
