/***************************************************************
* Copyright (C) 2008-2025 inx limited - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/** @file ml_model_template.c
 * Model implementation template file.
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
#include "yolov8_pose.h"
#include "ml_utils/ehs_ml_dequant.h"    /* EHS_ML_TENSOR_DEQUANT */

//TODO check Hailo support
#include "ml_postprocessing_engine_hailo.hpp"

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
/***** The Create, Destroy and SetInputData functions can be implemented here 
 ***** if they are unique from the boilerplate code
 *****/

/* Dequantisation: use EHS_ML_TENSOR_DEQUANT from ml_utils/ehs_ml_dequant.h.
 * That macro handles all EHS_ML_DATATYPE_* variants correctly.
 * The former local dequantise_box_values() only handled UINT8 and was never
 * called — it has been removed. */

EhsML_Err EhsML_Yolov8_Pose_RunOutputJson(EhsML_Context* ctx, ehs_char* json_output, ehs_uint32 output_size)
{
    EhsML_Err err = EhsML_Model_Boilerplate_RunOutputJson(ctx, json_output, output_size);
    if (err != EHS_ML_OK)
    {
        return err;
    }

    // Your code here
    if (ctx == NULL)
    {
        EHSH_LOG_ERROR("Invalid argument: ctx is NULL\n");
        return EHS_ML_MODEL_CTX_ERR;
    }

    if (json_output == NULL || output_size == 0)
    {
        EHSH_LOG_ERROR("Invalid argument: json_output is NULL or output_size is 0\n");
        return EHS_ML_INVALID_SIZE_ERR;
    }

    switch (ctx->hw_accel)
    {
        case EHS_ML_HWACCEL_HAILO:
        {
            ehs_uint32 output_size_written = 0;
            ehs_char* json_output_ptr = json_output;
            ehs_uint32 valid_detections = 0;
            // Process Hailo-specific output tensors and populate ctx->detections
            EhsML_Postprocessing_Engine_Hailo_ProcessOutput(ctx);
            output_size_written += EhsSnprintf(json_output_ptr, output_size - output_size_written, "{\"type\":%d,", ctx->type);
            for (size_t i = 0 ; i < ctx->detection_count; i++)
            {
                if (ctx->detections[i].filtered) continue; // Skip filtered detections if needed
                EHSH_LOG_INFO("Detection %zu: x=%f, y=%f, w=%f, h=%f, label=%s\n",
                    i,
                    ctx->detections[i].x,
                    ctx->detections[i].y,
                    ctx->detections[i].w,
                    ctx->detections[i].h,
                    ctx->detections[i].label
                );
                output_size_written += EhsSnprintf(
                    json_output_ptr + output_size_written,
                    output_size - output_size_written,
                    i == ctx->detection_count - 1 ? 
                        "\"x%03d\":%f,\"y%03d\":%f,\"w%03d\":%f,\"h%03d\":%f,\"label%03d\":\"%s\"" : 
                        "\"x%03d\":%f,\"y%03d\":%f,\"w%03d\":%f,\"h%03d\":%f,\"label%03d\":\"%s\",",
                    valid_detections,
                    ctx->detections[i].x,
                    valid_detections,
                    ctx->detections[i].y,
                    valid_detections,
                    ctx->detections[i].w,
                    valid_detections,
                    ctx->detections[i].h,
                    valid_detections,
                    ctx->detections[i].label
                );
                valid_detections++;
            }
            output_size_written += EhsSnprintf(json_output_ptr + output_size_written, output_size - output_size_written, "}");
            if (output_size_written >= output_size)            {
                EHSH_LOG_ERROR("Output size %u exceeds buffer size %u\n", output_size_written, output_size);
                return EHS_ML_JSON_STRSIZE_ERR;
            }
            break;
        }
        case EHS_ML_HWACCEL_NONE:
        default:
        {
            //TODO Are we sure we can just fall back to tflite for any model type?
            return EHS_ML_NOT_IMPLEMENTED;
            break;
        }
    }
    
    /* 1. Get the Boxes, Scores and Keypoints arrays from the output tensors */
    EhsML_Tensor_t raw_boxes_tensor[ctx->output_tensor_count / 3];
    EhsML_Tensor_t scores_tensor[ctx->output_tensor_count / 3];
    EhsML_Tensor_t raw_keypoints_tensor[ctx->output_tensor_count / 3];

    ehs_uint32 box_idx = 0;
    ehs_uint32 score_idx = 0;
    ehs_uint32 keypoint_idx = 0;

    for (ehs_uint32 i = 0 ; i < ctx->output_tensor_count ; i += 3)
    {
        if (ctx->output_tensor[i].dims[0] == 51)
        {
            // Boxes tensors have the feature dimension of 51
            raw_boxes_tensor[box_idx] = ctx->output_tensor[i];
            // Rearrange dimensions for correct processing
            int width = raw_boxes_tensor[box_idx].dims[2];
            int height = raw_boxes_tensor[box_idx].dims[1];
            int features = raw_boxes_tensor[box_idx].dims[0];
            raw_boxes_tensor[box_idx].dims[0] = width;
            raw_boxes_tensor[box_idx].dims[1] = height;
            raw_boxes_tensor[box_idx].dims[2] = features;
            raw_boxes_tensor[box_idx].num_dims = 3;
            // Now dims[0] = width, dims[1] = height, dims[2] = features
            box_idx++;
        }
        else if (ctx->output_tensor[i].dims[0] == 1)
        {
            // Scores tensors have the feature dimension of 1
            scores_tensor[score_idx] = ctx->output_tensor[i];
            scores_tensor[score_idx].dims[0] = 1; // Adjust dimension for easier processing
            scores_tensor[score_idx].dims[1] *= scores_tensor[score_idx].dims[2]; // Combine other dimensions
            scores_tensor[score_idx].num_dims = 2;
            score_idx++;
        }
        else if (ctx->output_tensor[i].dims[0] == 64)
        {
            // Keypoints tensors have the feature dimension of 64
            raw_keypoints_tensor[keypoint_idx] = ctx->output_tensor[i];
            // Rearrange dimensions for correct processing
            int width = raw_keypoints_tensor[keypoint_idx].dims[2];
            int height = raw_keypoints_tensor[keypoint_idx].dims[1];
            int features = raw_keypoints_tensor[keypoint_idx].dims[0];
            raw_keypoints_tensor[keypoint_idx].dims[0] = width;
            raw_keypoints_tensor[keypoint_idx].dims[1] = height;
            raw_keypoints_tensor[keypoint_idx].dims[2] = features;
            raw_keypoints_tensor[keypoint_idx].num_dims = 3;
            // Now dims[0] = width, dims[1] = height, dims[2] = features
            keypoint_idx++;
        }
        else
        {
            EHSH_LOG_ERROR("Unexpected output tensor dimension: %d\n", ctx->output_tensor[i].dims[0]);
            return EHS_ML_MODEL_OUTPUT_ERR;
        }
    }

    /* 2. Process the tensors to extract boxes, scores, and keypoints */
    // Note that the regression length is 15
    // TODO Get the centers
    ehs_float confidence = 0.0f;
    int regression_length = 15;
    int regression_distance[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    int instance_index = 0;

    for (ehs_uint32 i = 0 ; i < box_idx ; i++)
    {
        // Boxes Setup
        ehs_float qp_scale = raw_boxes_tensor[i].quantisation_params.scale;
        ehs_float qp_zp = raw_boxes_tensor[i].quantisation_params.offset;
        EhsML_Tensor_t dequant_boxes_tensor = raw_boxes_tensor[i];
        // Change the dimensions with regression length
        dequant_boxes_tensor.dims[0] *= dequant_boxes_tensor.dims[1];
        dequant_boxes_tensor.dims[1] = 4;
        dequant_boxes_tensor.dims[2] = regression_length + 1;
        int shape[2] = {dequant_boxes_tensor.dims[1], dequant_boxes_tensor.dims[2]};

        // Keypoints Setup
        ehs_float kp_qp_scale = raw_keypoints_tensor[i].quantisation_params.scale;
        ehs_float kp_qp_zp = raw_keypoints_tensor[i].quantisation_params.offset;
        EhsML_Tensor_t dequant_keypoints_tensor = raw_keypoints_tensor[i];
        // Change the dimensions with regression length
        dequant_keypoints_tensor.dims[0] *= dequant_keypoints_tensor.dims[1];
        dequant_keypoints_tensor.dims[1] = 17;
        dequant_keypoints_tensor.dims[2] = 3;
        int kp_shape[2] = {dequant_keypoints_tensor.dims[1], dequant_keypoints_tensor.dims[2]};

        // Bbox decoding
        for (ehs_uint32 j = 0 ; j < dequant_boxes_tensor.dims[0] ; j++)
        {
            confidence = scores_tensor[i].data_ptr.f32[j * scores_tensor[i].dims[0] + 0];
            instance_index++;
            if (confidence < ctx->conf_thres) continue;

            // TODO... Dequantise box values
            /// See https://github.com/hailo-ai/Hailo-Application-Code-Examples/blob/main/runtime/hailo-8/cpp/pose_estimation/yolov8_pose/yolov8pose_postprocess.cpp#L213
            /// See also https://github.com/ultralytics/ultralytics/blob/main/ultralytics/cfg/models/v8/yolov8-pose.yaml
            /// See also https://github.com/ultralytics/ultralytics/blob/main/ultralytics/cfg/datasets/coco-pose.yaml
            /// See also https://github.com/ultralytics/ultralytics/blob/v8.0.103/ultralytics/nn/modules/head.py#L100
            /// See also https://xtensor.readthedocs.io/en/latest/index.html
            /// See also https://docs.ultralytics.com/datasets/pose/
            /// See also https://community.hailo.ai/t/inquiry-regarding-yolov8s-pose-model-outputs-on-hailo-platform/17854
        }
    }

    // size_t used_size = 0;
    // json_output[0] = '\0'; // Initialize the output string
    // used_size += EhsSprintf(json_output + used_size, output_size - used_size, "{");
    // for (int i = 0 ; i < ctx->output_tensor_count ; i++)
    // {
    //     if (!ctx->output_tensor[i].data_ptr.f32)
    //     {
    //         EHSH_LOG_ERROR("Output tensor %d data pointer is NULL\n", i);
    //         return EHS_ML_MODEL_OUTPUT_ERR;
    //     }
    //     // Process each output tensor as needed
    //     /// If there are 53 features per tensor, for example
    //     // int features = ctx->output_tensor[i].dims[0];
    //     // int num_elements = ctx->output_tensor[i].num_elements;
    //     // float* data = ctx->output_tensor[i].data_ptr.f32;
    //     if (ctx->output_tensor[i].dims[0] != 53) continue;
    //     // Process the tensor data
    //     for (int j = 0; j < ctx->output_tensor[i].dims[0]; j++) {
    //         used_size += EhsSprintf(json_output + used_size, output_size - used_size, "\"\":%.6f", ctx->output_tensor[i].data_ptr.f32[j]);
    //         if (j < ctx->output_tensor[i].num_elements - 1) {
    //             used_size += EhsSprintf(json_output + used_size, output_size - used_size, ",");
    //         }
    //     }
    //     // Example: Log the first 10 values of each output tensor
    //     printf("Output tensor %d data (first 10 values): ", i);
    //     for (int j = 0; j < 10 && j < ctx->output_tensor[i].num_elements; j++) {
    //         printf("%.2f ", ctx->output_tensor[i].data_ptr.f32[j]);
    //     }
    //     printf("\n");
    // }
    // used_size += EhsSprintf(json_output + used_size, output_size - used_size, "}");
    return EHS_ML_OK;
}
