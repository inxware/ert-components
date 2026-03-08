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

/**
 * @brief Decode a COCO class index to its human-readable label.
 *
 * The switch is 1-indexed: case 0 is reserved for "__background__" so that
 * the caller can pass (model_class_idx + 1) when iterating over an output
 * tensor whose classes are 0-based but the background class is implicit
 * (i.e. the model produces 80 actual-object classes, 0–79, and the caller
 * adds 1 to skip the background slot in this table).
 *
 * Returns "N/A" for any index outside the range 0–80.
 */
static const char* get_coco_name_from_int(int cls)
{
    char* result = "N/A";
    switch(cls) {
		case 0: result = "__background__";break;
		case 1: result = "person";break;
		case 2: result = "bicycle";break;
		case 3: result = "car";break;
		case 4: result = "motorcycle";break;
		case 5: result = "airplane";break;
		case 6: result = "bus";break;
		case 7: result = "train";break;
		case 8: result = "truck";break;
		case 9: result = "boat";break;
		case 10: result = "traffic light";break;
		case 11: result = "fire hydrant";break;
		case 12: result = "stop sign";break;
		case 13: result = "parking meter";break;
		case 14: result = "bench";break;
		case 15: result = "bird";break;
		case 16: result = "cat";break;
		case 17: result = "dog";break;
		case 18: result = "horse";break;
		case 19: result = "sheep";break;
		case 20: result = "cow";break;
		case 21: result = "elephant";break;
		case 22: result = "bear";break;
		case 23: result = "zebra";break;
		case 24: result = "giraffe";break;
		case 25: result = "backpack";break;
		case 26: result = "umbrella";break;
		case 27: result = "handbag";break;
		case 28: result = "tie";break;
		case 29: result = "suitcase";break;
		case 30: result = "frisbee";break;
		case 31: result = "skis";break;
		case 32: result = "snowboard";break;
		case 33: result = "sports ball";break;
		case 34: result = "kite";break;
		case 35: result = "baseball bat";break;
		case 36: result = "baseball glove";break;;
		case 37: result = "skateboard";break;
		case 38: result = "surfboard";break;
		case 39: result = "tennis racket";break;
		case 40: result = "bottle";break;
		case 41: result = "wine glass";break;
		case 42: result = "cup";break;
		case 43: result = "fork";break;
		case 44: result = "knife";break;
		case 45: result = "spoon";break;
		case 46: result = "bowl";break;
		case 47: result = "banana";break;
		case 48: result = "apple";break;
		case 49: result = "sandwich";break;
		case 50: result = "orange";break;
		case 51: result = "broccoli";break;
		case 52: result = "carrot";break;
		case 53: result = "hot dog";break;
		case 54: result = "pizza";break;
		case 55: result = "donut";break;
		case 56: result = "cake";break;
		case 57: result = "chair";break;
		case 58: result = "couch";break;
		case 59: result = "potted plant";break;
		case 60: result = "bed";break;
		case 61: result = "dining table";break;
		case 62: result = "toilet";break;
		case 63: result = "tv";break;
		case 64: result = "laptop";break;
		case 65: result = "mouse";break;
		case 66: result = "remote";break;
		case 67: result = "keyboard";break;
		case 68: result = "cell phone";break;
		case 69: result = "microwave";break;
		case 70: result = "oven";break;
		case 71: result = "toaster";break;
		case 72: result = "sink";break;
		case 73: result = "refrigerator";break;
		case 74: result = "book";break;
		case 75: result = "clock";break;
		case 76: result = "vase";break;
		case 77: result = "scissors";break;
		case 78: result = "teddy bear";break;
		case 79: result = "hair drier";break;
		case 80: result = "toothbrush";break;
    }
	return result;
}

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
EhsML_Err EhsML_Yolov8_ObjDet_RunOutputJson(EhsML_Context* ctx, ehs_char* json_output, ehs_uint32 output_size)
{
    EhsML_Err err = EhsML_Model_Boilerplate_RunOutputJson(ctx, json_output, output_size);
    if (err != EHS_ML_OK)
    {
        return err;
    }

    /* Guard: this decoder only handles FP32 output. Quantised models must be
     * dequantised before use — see quantisation_params in EhsML_Tensor_t. */
    if (ctx->output_tensor[0].data_type != EHS_ML_DATATYPE_FP32)
    {
        EHSH_LOG_ERROR("yolov8_objdet: output tensor data type %d is not FP32 — "
                       "this decoder only supports float32 output\n",
                       (int)ctx->output_tensor[0].data_type);
        return EHS_ML_MODEL_OUTPUT_ERR;
    }

    /* Scale factors: tensor coordinates are normalised 0–1 relative to
     * model input dimensions. Read from the input tensor metadata so that
     * any model input resolution works without hardcoding. */
    int input_width  = ctx->input_tensor[0].dims[2];  /* W: dims = [batch, H, W, C] */
    int input_height = ctx->input_tensor[0].dims[1];  /* H */

    size_t class_idx = 0;
    size_t index = -1;         /* flat index into the output tensor; pre-incremented on every read */
    size_t printed_count = 0;
    size_t string_index = 0;
    string_index += EhsSprintf(&(json_output[string_index]), "{");

    /* Outer loop: one iteration per COCO class (0–79).
     * class_idx is the 0-based model class index; +1 is passed to
     * get_coco_name_from_int() because that function reserves case 0 for
     * "__background__", so case 1 == "person" == model class 0. */
    while (class_idx < 80)
    {
        /* First element for this class: number of boxes the model detected. */
        size_t num_of_class_boxes = (size_t)ctx->output_tensor[0].data_ptr.f32[++index];

        for (size_t box_idx = 0 ; box_idx < num_of_class_boxes ; box_idx++)
        {
            /* Coordinates are normalised [0,1]; scale to pixels. */
            float y_min = ctx->output_tensor[0].data_ptr.f32[++index] * input_height;
            float x_min = ctx->output_tensor[0].data_ptr.f32[++index] * input_width;
            float y_max = ctx->output_tensor[0].data_ptr.f32[++index] * input_height;
            float x_max = ctx->output_tensor[0].data_ptr.f32[++index] * input_width;
            float conf  = ctx->output_tensor[0].data_ptr.f32[++index];

            if (conf >= ctx->conf_thres) {
                string_index += EhsSprintf(&(json_output[string_index]),
                    "\"cls%d\":\"%s\",\"cnf%d\":%.2f,"
                    "\"ymin%d\":%.2f,\"xmin%d\":%.2f,\"ymax%d\":%.2f,\"xmax%d\":%.2f,",
                    (int)printed_count, get_coco_name_from_int((int)class_idx + 1),
                    (int)printed_count, conf,
                    (int)printed_count, y_min,
                    (int)printed_count, x_min,
                    (int)printed_count, y_max,
                    (int)printed_count, x_max
                );
                printed_count++;
            }
        }
        class_idx++;
    }

    string_index += EhsSprintf(&(json_output[string_index]), "\"det_cnt\":%d", (int)printed_count);
    string_index += EhsSprintf(&(json_output[string_index]), "}");

    return EHS_ML_OK;
}
