/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/**
 * @file ert_trt_nms_decode.h
 * @brief Decoder for TensorRT EfficientNMS_TRT / BatchedNMS_TRT output tensors.
 *
 * After EhsML_InfEngine_RunInference() with a TensorRT NMS-plugin engine,
 * ctx->output_tensor[0..3] hold the four standard NMS output bindings:
 *
 *   [0] num_detections  — int32  [1]              number of valid detections
 *   [1] detection_boxes — float32 [max_det, 4]    normalised [x1,y1,x2,y2]
 *   [2] detection_scores— float32 [max_det]       confidence per detection
 *   [3] detection_classes—int32  [max_det]        0-based class index
 *
 * EhsML_TRT_NMS_Decode() reads these tensors, scales box coordinates to
 * pixel space using the engine input dimensions, converts from corner to
 * centre format, and populates ctx->detections[] / ctx->detection_count.
 *
 * The result is then passed to EhsML_ObjDet_Json_FromDetections() for
 * engine-independent JSON serialisation.
 *
 * Coord-order caveat: EfficientNMS_TRT box order is [x1,y1,x2,y2] by
 * default (inputOrder=0).  If the engine was built with inputOrder=1
 * ([y1,x1,y2,x2]), swap the index assignments in ert_trt_nms_decode.c.
 */

#ifndef ERT_TRT_NMS_DECODE_H
#define ERT_TRT_NMS_DECODE_H

#include "hal_ml.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Decode TensorRT NMS plugin output tensors into ctx->detections[].
 *
 * @param ctx  Active EhsML_Context after EhsML_InfEngine_RunInference().
 *             Must have output_tensor_count >= 4 and hw_accel == EHS_ML_HWACCEL_NVIDIA.
 * @return EHS_ML_OK on success, EHS_ML_MODEL_TENSOR_DIM_ERR if < 4 output tensors.
 */
EhsML_Err EhsML_TRT_NMS_Decode(EhsML_Context *ctx);

#ifdef __cplusplus
}
#endif

#endif /* ERT_TRT_NMS_DECODE_H */
