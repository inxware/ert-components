#include "tflite_yolo.h"
#include "utils/tflite_nms.h"
#include "utils/tflite_utils.h"
#include <stdio.h>

#define YOLO_ATTRIB_CNT 5 // 4 box coordinates + 1 objectness

void TfLiteRunYolo_Apply_NMS(TfLiteBoxDetections* detections, const NMSBox* boxes, int nms_count, int input_width, int input_height, float conf_thres)
{
    size_t keep[YOLO_NMS_MAX];
    size_t scratch_i[YOLO_NMS_MAX];
    uint8_t scratch_f[YOLO_NMS_MAX];

    size_t k = TfLite_NMS(boxes, nms_count,
                          conf_thres, 0.45f,
                          1, //classAware
                          keep, YOLO_NMS_MAX,
                          scratch_i, scratch_f);

    if(k > EHS_ML_OBJ_DETECTIONS_MAX){
        k = EHS_ML_OBJ_DETECTIONS_MAX;
        // warn about finding more objects than the max allows
        detections->error_id |= YOLO_ERROR_OBJ_DET_MAX;
    }
    detections->box_detections_count = k;
    for(size_t i=0; i<k; ++i){
        size_t id = keep[i];
        detections->box_detections[i].class_id = boxes[id].class_id;
        detections->box_detections[i].confidence = boxes[id].score;
        detections->box_detections[i].x = (unsigned int)(boxes[id].x * input_width);
        detections->box_detections[i].y = (unsigned int)(boxes[id].y * input_height);
        detections->box_detections[i].w = (unsigned int)(boxes[id].w * input_width);
        detections->box_detections[i].h = (unsigned int)(boxes[id].h * input_height);
    }

}

TfLiteErr TfLiteRunYolo_Float(struct TfLiteModelCtx* ctx, struct TfLiteBoxDetections* detections)
{
    if(!ctx || !ctx->out_tensor || !detections) return TF_LITE_FAILED;

    unsigned long start = TfLiteTimeNow_ms();

    detections->error_id = YOLO_ERROR_OK;

    //[1, N, 5 + num_classes] 
    int num_detections = TfLiteTensorDim(ctx->out_tensor, 1); // N  - YOLOv5 for 320x320 input // 320×320	40×40, 20×20, 10×10	40²×3 + 20²×3 + 10²×3 = 6300
    int elements_per_detection = TfLiteTensorDim(ctx->out_tensor, 2); // 5 + num_classes
    int num_classes = elements_per_detection - YOLO_ATTRIB_CNT;

    int input_width = TfLiteTensorDim(ctx->in_tensor, 2);
    int input_height = TfLiteTensorDim(ctx->in_tensor, 1);

    float* output = (float*)TfLiteTensorData(ctx->out_tensor);

    if(!output) {
        detections->error_id |= YOLO_ERROR_INVALID_OUTPUT;
        return TF_LITE_FAILED;
    }

  //  printf("%d %d %d %d %d\n", num_detections, elements_per_detection, num_classes, input_width, input_height);

    /* 1. invoke --------------------------------------------------------- */
    if(TfLiteInterpreterInvoke(ctx->interp) != kTfLiteOk) return TF_LITE_FAILED;

    NMSBox boxes[YOLO_NMS_MAX] = {0};
    int nms_count = 0;

    // Process detections and store them in NMS buffer
    for (int i = 0; i < num_detections; ++i) {
        float* det = output + i * elements_per_detection;
        NMSBox box = {0};
        // Extract box coords
        box.x = det[0];
        box.y = det[1];
        box.w = det[2];
        box.h = det[3];
        // Extract objectness
        float objectness = det[4];
        
        // Get class with max score
        float max_class_score = 0;
        int class_id = -1;
        for (int j = 0; j < num_classes; ++j) {
            float score = det[YOLO_ATTRIB_CNT + j];
            if (score > max_class_score) {
                max_class_score = score;
                class_id = j;
            }
        }
        if(class_id < 0){
            // warn bad class found in the model
            detections->error_id |= YOLO_ERROR_INVAL_CLASS_ID;
            continue;
        }
        box.class_id = class_id;
        box.score = objectness * max_class_score;
        if(!box.score) continue; // ignore '0' score detections
        if (box.score >= ctx->conf_thres) {
           // printf("id=%d x=%.2f y=%.2f w=%.2f h=%.2f c=%.2f t=%.2f\n", box.class_id, box.x, box.y, box.w, box.h, box.score, ctx->conf_thres);
            boxes[nms_count] = box;
            nms_count++;
            if(nms_count >= YOLO_NMS_MAX){
                // warn NMS box count reached max
                detections->error_id |= YOLO_ERROR_NMS_MAX;
                break;
            }
        }
    }

    TfLiteRunYolo_Apply_NMS(detections, boxes, nms_count, input_width, input_height, ctx->conf_thres);
    
    unsigned long end = TfLiteTimeNow_ms();
    detections->processing_time_ms = (int)(end - start);

    return TF_LITE_OK;
}

TfLiteErr TfLiteRunYolo_Int8(struct TfLiteModelCtx* ctx, struct TfLiteBoxDetections* detections)
{
    if(!ctx || !ctx->out_tensor || !detections) return TF_LITE_FAILED;

    unsigned long start = TfLiteTimeNow_ms();

    detections->error_id = YOLO_ERROR_OK;

    //[1, N, 5 + num_classes] 
    int num_detections = TfLiteTensorDim(ctx->out_tensor, 1); // N  - YOLOv5 for 320x320 input // 320×320	40×40, 20×20, 10×10	40²×3 + 20²×3 + 10²×3 = 6300
    int elements_per_detection = TfLiteTensorDim(ctx->out_tensor, 2); // 5 + num_classes
    int num_classes = elements_per_detection - YOLO_ATTRIB_CNT;

    int input_width = TfLiteTensorDim(ctx->in_tensor, 2);
    int input_height = TfLiteTensorDim(ctx->in_tensor, 1);

    // Get quantization parameters
    TfLiteQuantizationParams q = TfLiteTensorQuantizationParams(ctx->out_tensor);
    float scale = q.scale;
    int zero_point = q.zero_point;

    uint8_t* output = (uint8_t*)TfLiteTensorData(ctx->out_tensor);

    if(!output) {
        detections->error_id |= YOLO_ERROR_INVALID_OUTPUT;
        return TF_LITE_FAILED;
    }

    //  printf("%d %d %d %d %d\n", num_detections, elements_per_detection, num_classes, input_width, input_height);

    /* 1. invoke --------------------------------------------------------- */
    if(TfLiteInterpreterInvoke(ctx->interp) != kTfLiteOk) return TF_LITE_FAILED;

    NMSBox boxes[YOLO_NMS_MAX] = {0};
    int nms_count = 0;

    for (int i = 0; i < num_detections; ++i) {
        NMSBox box = {0};

        // Dequantize box coordinates
        box.x = (output[i * elements_per_detection + 0] - zero_point) * scale;
        box.y = (output[i * elements_per_detection + 1] - zero_point) * scale;
        box.w = (output[i * elements_per_detection + 2] - zero_point) * scale;
        box.h = (output[i * elements_per_detection + 3] - zero_point) * scale;

        // Objectness score
        float objectness = (output[i * elements_per_detection + 4] - zero_point) * scale;

        // Find max class score
        float max_class_score = 0.0f;
        int class_id = -1;
        for (int j = 0; j < num_classes; ++j) {
            float class_score = (output[i * elements_per_detection + YOLO_ATTRIB_CNT + j] - zero_point) * scale;
            if (class_score > max_class_score) {
                max_class_score = class_score;
                class_id = j;
            }
        }

        if (class_id < 0) {
            detections->error_id |= YOLO_ERROR_INVAL_CLASS_ID;
            continue;
        }

        box.class_id = class_id;
        box.score = objectness * max_class_score;
        if (!box.score) continue;

        if (box.score >= ctx->conf_thres) {
            boxes[nms_count++] = box;
            if (nms_count >= YOLO_NMS_MAX) {
                detections->error_id |= YOLO_ERROR_NMS_MAX;
                break;
            }
        }
    }

    TfLiteRunYolo_Apply_NMS(detections, boxes, nms_count, input_width, input_height, ctx->conf_thres);

    unsigned long end = TfLiteTimeNow_ms();
    detections->processing_time_ms = (int)(end - start);

    return TF_LITE_OK;
}