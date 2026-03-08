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

/*****************************************************************************/
/* Declare macros and local typedefs used by this file */
#ifndef YOLO_NMS_MAX
#define YOLO_NMS_MAX (3 * EHS_ML_OBJ_DETECTIONS_MAX)
#endif

#define YOLO_ATTRIB_CNT 5 // 4 box coordinates + 1 objectness

/*****************************************************************************/
/* COCO-80 class label table (standard YOLOv5 class ordering, 0-indexed) */
#define YOLOV5_COCO_LABEL_COUNT 80
static const char* const kYolov5CocoLabels[YOLOV5_COCO_LABEL_COUNT] = {
    "person", "bicycle", "car", "motorcycle", "airplane",
    "bus", "train", "truck", "boat", "traffic light",
    "fire hydrant", "stop sign", "parking meter", "bench", "bird",
    "cat", "dog", "horse", "sheep", "cow",
    "elephant", "bear", "zebra", "giraffe", "backpack",
    "umbrella", "handbag", "tie", "suitcase", "frisbee",
    "skis", "snowboard", "sports ball", "kite", "baseball bat",
    "baseball glove", "skateboard", "surfboard", "tennis racket", "bottle",
    "wine glass", "cup", "fork", "knife", "spoon",
    "bowl", "banana", "apple", "sandwich", "orange",
    "broccoli", "carrot", "hot dog", "pizza", "donut",
    "cake", "chair", "couch", "potted plant", "bed",
    "dining table", "toilet", "tv", "laptop", "mouse",
    "remote", "keyboard", "cell phone", "microwave", "oven",
    "toaster", "sink", "refrigerator", "book", "clock",
    "vase", "scissors", "teddy bear", "hair drier", "toothbrush"
};

static const char* yolov5_class_label(int class_id)
{
    if (class_id >= 0 && class_id < YOLOV5_COCO_LABEL_COUNT)
    {
        return kYolov5CocoLabels[class_id];
    }
    return "";
}

#define _EHS_ML_TYPED_DATA_ASSIGN(assignee, ctx, tensor_index, index, zero_point, scale) do { \
    switch (ctx->output_tensor[tensor_index].data_type) \
    {   \
        case EHS_ML_DATATYPE_NONE:  \
        {   \
            assignee = (double)((ctx->output_tensor[tensor_index].data_ptr.raw[index] - zero_point) * scale);    \
            break;  \
        }   \
        case EHS_ML_DATATYPE_UINT8: \
        {   \
            assignee = (double)((ctx->output_tensor[tensor_index].data_ptr.u8[index] - zero_point) * scale); \
            break;  \
        }   \
        case EHS_ML_DATATYPE_INT8:  \
        {   \
            assignee = (double)((ctx->output_tensor[tensor_index].data_ptr.s8[index] - zero_point) * scale); \
            break;  \
        }   \
        case EHS_ML_DATATYPE_UINT16:    \
        {   \
            assignee = (double)((ctx->output_tensor[tensor_index].data_ptr.u16[index] - zero_point) * scale);    \
            break;  \
        }   \
        case EHS_ML_DATATYPE_INT16: \
        {   \
            assignee = (double)((ctx->output_tensor[tensor_index].data_ptr.s16[index] - zero_point) * scale);    \
            break;  \
        }   \
        case EHS_ML_DATATYPE_UINT32:    \
        {   \
            assignee = (double)((ctx->output_tensor[tensor_index].data_ptr.u32[index] - zero_point) * scale);    \
            break;  \
        }   \
        case EHS_ML_DATATYPE_INT32: \
        {   \
            assignee = (double)((ctx->output_tensor[tensor_index].data_ptr.s32[index] - zero_point) * scale);    \
            break;  \
        }   \
        case EHS_ML_DATATYPE_UINT64:    \
        {   \
            assignee = (double)((ctx->output_tensor[tensor_index].data_ptr.u64[index] - zero_point) * scale);    \
            break;  \
        }   \
        case EHS_ML_DATATYPE_INT64: \
        {   \
            assignee = (double)((ctx->output_tensor[tensor_index].data_ptr.s64[index] - zero_point) * scale);    \
            break;  \
        }   \
        case EHS_ML_DATATYPE_FP16:  \
        {   \
            /* TODO convert and calculate to valid type */  \
            /*assignee = (double)((ctx->output_tensor.data_ptr.f16[index] - zero_point) * scale);*/    \
            break;  \
        }   \
        case EHS_ML_DATATYPE_FP32:  \
        {   \
            assignee = (double)(ctx->output_tensor[tensor_index].data_ptr.f32[index]);    \
            break;  \
        }   \
        case EHS_ML_DATATYPE_FP64:  \
        {   \
            assignee = (double)(ctx->output_tensor[tensor_index].data_ptr.f64[index]);    \
            break;  \
        }   \
        case EHS_ML_DATATYPE_COMPLEX64: \
        {   \
            /* TODO convert and calculate to valid type */  \
            /*assignee = (double)((ctx->output_tensor.data_ptr.c64[index] - zero_point) * scale);*/    \
            break;  \
        }   \
        case EHS_ML_DATATYPE_COMPLEX128:    \
        {   \
            /* TODO convert and calculate to valid type */  \
            /*assignee = (double)((ctx->output_tensor.data_ptr.c128[index] - zero_point) * scale);*/   \
            break;  \
        }   \
        case EHS_ML_DATATYPE_BOOL:  \
        {   \
            assignee = (double)((ctx->output_tensor[tensor_index].data_ptr.b[index] - zero_point) * scale);  \
            break;  \
        }   \
        case EHS_ML_DATATYPE_RAW:   \
        {   \
            assignee = (double)((ctx->output_tensor[tensor_index].data_ptr.raw[index] - zero_point) * scale);    \
            break;  \
        }   \
        case EHS_ML_DATATYPE_CONST_RAW: \
        {   \
            assignee = (double)((ctx->output_tensor[tensor_index].data_ptr.const_raw[index] - zero_point) * scale);  \
            break;  \
        }   \
        default:    \
        {   \
            assignee = (double)((ctx->output_tensor[tensor_index].data_ptr.raw[index] - zero_point) * scale);    \
            break;  \
        }   \
    }   \
} while (0)

/*****************************************************************************/
/* Declare prototypes of local functions */

/*****************************************************************************/
/* Variables defined with file-scope */

/*****************************************************************************/
/* Variables defined with global-scope */

/*****************************************************************************/
/* Function definitions */

EhsML_Err EhsML_Yolov5_ObjDet_RunOutputJson(EhsML_Context* ctx, ehs_char* json_output, ehs_uint32 output_size)
{
    // Your code here
    EhsML_Err err = EhsML_Model_Boilerplate_RunOutputJson(ctx, json_output, output_size);
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

    printf("[YOLOV5_DBG] output tensor: dims[0]=%d dims[1]=%d dims[2]=%d "
           "num_detections=%d elements_per_detection=%d num_classes=%d\n",
           ctx->output_tensor[0].dims[0], ctx->output_tensor[0].dims[1],
           ctx->output_tensor[0].dims[2],
           num_detections, elements_per_detection, num_classes);
    printf("[YOLOV5_DBG] conf_thres=%.3f scale=%.6f zero_point=%.3f\n",
           (double)ctx->conf_thres, scale, zero_point);

    /* Track rejection reasons */
    int rej_no_class = 0, rej_zero_score = 0, rej_below_thres = 0;
    double max_score_seen = 0.0;

    for (i = 0 ; i < num_detections ; ++i)
    {
        NMSBox box = {0};

        // Dequantize box coordinates
        _EHS_ML_TYPED_DATA_ASSIGN(box.x, ctx, 0, i * elements_per_detection + 0, zero_point, scale);
        _EHS_ML_TYPED_DATA_ASSIGN(box.y, ctx, 0, i * elements_per_detection + 1, zero_point, scale);
        _EHS_ML_TYPED_DATA_ASSIGN(box.w, ctx, 0, i * elements_per_detection + 2, zero_point, scale);
        _EHS_ML_TYPED_DATA_ASSIGN(box.h, ctx, 0, i * elements_per_detection + 3, zero_point, scale);

        // Objectness score
        double objectness;
        _EHS_ML_TYPED_DATA_ASSIGN(objectness, ctx, 0, i * elements_per_detection + 4, zero_point, scale);

        // Find max class score
        float max_class_score = 0.0f;
        int class_id = -1;
        for (int j = 0; j < num_classes; ++j) {
            double class_score;
            _EHS_ML_TYPED_DATA_ASSIGN(class_score, ctx, 0, i * elements_per_detection + YOLO_ATTRIB_CNT + j, zero_point, scale);
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
            printf("[YOLOV5_DBG] det[%d]: obj=%.3f cls_score=%.3f combined=%.3f "
                   "cls=%d x=%.1f y=%.1f w=%.1f h=%.1f -> ACCEPTED\n",
                   i, objectness, (double)max_class_score, box.score,
                   class_id, box.x, box.y, box.w, box.h);
            boxes[nms_count++] = box;
            if (nms_count >= YOLO_NMS_MAX) {
                break;
            }
        } else {
            rej_below_thres++;
        }
    }

    printf("[YOLOV5_DBG] scan done: pre_nms=%d rej_no_class=%d rej_zero=%d "
           "rej_below_thres=%d max_score_seen=%.4f conf_thres=%.3f\n",
           nms_count, rej_no_class, rej_zero_score, rej_below_thres,
           max_score_seen, (double)ctx->conf_thres);

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

    /* 4. Parse the output into JSON output with given maximum output length */
    ehs_uint32 used = EhsSnprintf(
        json_output, output_size,
        "{\"type\":%d,\"det_cnt\":%d",
        (int)ctx->type, (int)keep_count
    );
    printf("[YOLOV5_DBG] after NMS: keep_count=%d\n", (int)keep_count);
    if (used >= output_size) return EHS_ML_JSON_STRSIZE_ERR;
    for (i = 0 ; i < keep_count ; ++i)
    {
        int remaining = output_size - used - 1; // reserve space for closing '}' and NULL terminator
        if (remaining <= 0) break; //TODO this should return an error because JSON output length is too short
        int len = EhsSnprintf(json_output + used, remaining,
            ",\"cls%d\":%d,\"lbl%d\":\"%s\",\"cnf%d\":%.2f,\"x%d\":%.1f,\"y%d\":%.1f,\"w%d\":%.1f,\"h%d\":%.1f",
            i, boxes[keep[i]].class_id,
            i, yolov5_class_label(boxes[keep[i]].class_id),
            i, boxes[keep[i]].score,
            i, boxes[keep[i]].x,
            i, boxes[keep[i]].y,
            i, boxes[keep[i]].w,
            i, boxes[keep[i]].h
        );
        if (len > 0) used += (ehs_uint32)len;
    }
    EhsSnprintf(json_output + used, output_size - used - 1, "}");
    return EHS_ML_OK;
}
