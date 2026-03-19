/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/**
 * @file ert_trt_nms_decode.c
 * @brief Decode EfficientNMS_TRT / BatchedNMS_TRT output into ctx->detections[].
 */

#include "ert_trt_nms_decode.h"
#include "hal-api.h"

EhsML_Err EhsML_TRT_NMS_Decode(EhsML_Context *ctx)
{
    if (!ctx) return EHS_ML_NULL_CTX_ERR;

    if (ctx->output_tensor_count < 4)
    {
        EHSH_LOG_ERROR("TRT NMS decode: expected 4 output tensors, got %u",
                       ctx->output_tensor_count);
        return EHS_ML_MODEL_TENSOR_DIM_ERR;
    }

    /* EfficientNMS_TRT / BatchedNMS_TRT standard output binding layout (batch=1):
     *
     *   output_tensor[0]  num_detections  — int32  [1]
     *   output_tensor[1]  detection_boxes — float32 [max_det, 4]  normalised [x1,y1,x2,y2]
     *   output_tensor[2]  detection_scores— float32 [max_det]
     *   output_tensor[3]  detection_classes—int32  [max_det]
     *
     * Box coordinates are normalised [0,1] relative to the model input dimensions.
     * TensorRT engines use NCHW layout: input dims = [N, C, H, W].
     */
    const ehs_sint32 num_det = ctx->output_tensor[0].data_ptr.s32[0];
    const float     *boxes   = ctx->output_tensor[1].data_ptr.f32;
    const float     *scores  = ctx->output_tensor[2].data_ptr.f32;
    const ehs_sint32*classes = ctx->output_tensor[3].data_ptr.s32;

    /* Input dims: NCHW — [N=0, C=1, H=2, W=3] */
    const float input_w = (float)ctx->input_tensor[0].dims[3];
    const float input_h = (float)ctx->input_tensor[0].dims[2];

    /* Pipeline: TRT NMS plugin absorbs dequant, decode, and logical (NMS) stages.
     * They were all performed by the TensorRT engine during INFER. */
    EHS_ML_STAGE_SET(ctx->pipeline, EHS_ML_STAGE_DEQUANT,  EHS_ML_TECH_COVERED_BY_PREV);
    EHS_ML_STAGE_SET(ctx->pipeline, EHS_ML_STAGE_DECODE,   EHS_ML_TECH_COVERED_BY_PREV);
    EHS_ML_STAGE_SET(ctx->pipeline, EHS_ML_STAGE_LOGICAL,  EHS_ML_TECH_COVERED_BY_PREV);

    ctx->detection_count = 0;

    for (ehs_sint32 i = 0; i < num_det && ctx->detection_count < EHS_ML_OBJ_DETECTIONS_MAX; i++)
    {
        /* Scale normalised corners to pixel space */
        float x1 = boxes[i * 4 + 0] * input_w;
        float y1 = boxes[i * 4 + 1] * input_h;
        float x2 = boxes[i * 4 + 2] * input_w;
        float y2 = boxes[i * 4 + 3] * input_h;

        EhsML_Detection_t *d = &ctx->detections[ctx->detection_count++];
        d->conf     = scores[i];
        d->cls      = (ehs_uint32)classes[i];
        d->filtered = EHS_FALSE;
        /* Convert corners to centre format used by EhsML_Detection_t */
        d->x = (x1 + x2) * 0.5f;
        d->y = (y1 + y2) * 0.5f;
        d->w = x2 - x1;
        d->h = y2 - y1;
    }

    return EHS_ML_OK;
}
