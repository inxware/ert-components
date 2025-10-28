
#include "hal_ml.h"
#include "hal_logger.h"
#include "hal_mem.h"
#include "hailo/hailort.h"
#include "hailo_thread.h"
#include <threads.h>

#include "tflite_model.h"
#include "tflite_nms.h"
#include "tflite_utils.h"
#include "tflite_yolo.h"

#define DELETEME_HAILO 0
#if DELETEME_HAILO
/********* DELETEME */
#include <time.h>
/********* DELETEME */
#endif

#define EHS_HAILO_LIB_VERSION_MAJOR 4
#define EHS_HAILO_LIB_VERSION_MINOR 22
#define EHS_HAILO_LIB_VERSION_REV 0

static hailo_version_t gHailoVersion = {0, 0, 0};

static ehs_bool check_hailo_version() {
    if (gHailoVersion.major == 0) hailo_get_library_version(&gHailoVersion);
    if (gHailoVersion.major != EHS_HAILO_LIB_VERSION_MAJOR ||
        gHailoVersion.minor != EHS_HAILO_LIB_VERSION_MINOR)
        return EHS_FALSE;
    return EHS_TRUE;
}

#define HAILO_MAX_EDGE_LAYERS (16)
#define HAILO_MAX_BATCH (64)
// This is the product of frame count and frame size. The frame size is extracted from gHailoInputVStreamInfo
//#define HAILO_INFER_FRAME_SIZE (6400)

static hailo_status gHailoStatus = HAILO_UNINITIALIZED;

//static hailo_vdevice gHailoVDevice = NULL;
static hailo_device gHailoDevice = NULL;
static hailo_hef gHailoHef = NULL;
static hailo_configured_network_group gHailoConfiguredNNG = NULL;

static hailo_input_vstream_params_by_name_t gHailoInputVStreamParams[HAILO_MAX_EDGE_LAYERS] = {0};
static hailo_output_vstream_params_by_name_t gHailoOutputVStreamParams[HAILO_MAX_EDGE_LAYERS] = {0};

static ehs_uint8 *gInputBuffer = NULL;
//static ehs_uint8 *gOutputBuffer[HAILO_MAX_EDGE_LAYERS][HAILO_MAX_BATCH] = {NULL};
static void *gOutputBuffer = NULL; // This will be determined after getting the output vstream info.

static size_t gInputBufferSize[HAILO_MAX_EDGE_LAYERS] = {0};
static size_t gOutputBufferSize[HAILO_MAX_EDGE_LAYERS] = {0};

static hailo_input_vstream gInputVStream[HAILO_MAX_EDGE_LAYERS] = {NULL};
static hailo_output_vstream gOutputVStream[HAILO_MAX_EDGE_LAYERS] = {NULL};

static size_t gInputVStreamsSize = HAILO_MAX_EDGE_LAYERS;
static size_t gOutputVStreamsSize = HAILO_MAX_EDGE_LAYERS;

static hailo_activated_network_group gActivatedNetworkGroup = NULL;

// Refer to https://github.com/hailo-ai/hailort/blob/v4.22.0/hailort/libhailort/examples/c/infer_pipeline_example/infer_pipeline_example.c
//  for example code with pipline inference.
// Refer to https://hailo.ai/developer-zone/documentation/hailort-v4-20-0/?sp_referrer=inference/inference.html
//  for documentation of inference.
// Refer to https://hailo.ai/developer-zone/documentation/hailort-v4-20-0/?sp_referrer=tutorials/c_tutorial.html#inference-using-virtual-streams-vstreams-example
//  for tutorial to use pipeline inference with virtual streams. There are more tutorials on the same page.
//  And you can run and compare the performance of different methods.
static thread_return_type write_to_device(void *args)
{
    hailo_status status = HAILO_UNINITIALIZED;
    {
        status = hailo_vstream_write_raw_buffer(gInputVStream[(int)args], gInputBuffer, gInputBufferSize[(int)args]);
        if (status != HAILO_SUCCESS)
        {
            EHSH_LOG_ERROR("hailo_vstream_write_raw_buffer failed: %d\n", status);
            return (thread_return_type)status;
        }
    }

    // Optional
    // status = hailo_flush_input_vstream(gInputVStream);
    // if (status != HAILO_SUCCESS)
    // {
    //     EHSH_LOG_ERROR("hailo_flush_input_vstreams failed: %d\n", status);
    //     return (thread_return_type)status;
    // }

    return (thread_return_type)HAILO_SUCCESS;
}

/******* DELETEME */
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
static clock_t stored_time = 0;
/******* DELETEME */

static EhsML_Err EhsInternal_PostProcess_TFLite_YoloV5(TfLiteBoxDetections* detections, const float* data, size_t data_size, ehs_uint32 input_width, ehs_uint32 input_height, ehs_float conf_thres)
{
    if (data == NULL || data_size == 0) return EHS_ML_FAILED;
    // The output data format of YoloV5 is not documented. The following is inferred from the output data.
    // TODO
    int num_detections = 123; // TODO
    int elements_per_detection = 6; // TODO
    int num_classes = elements_per_detection - 5; // TODO

    NMSBox boxes[YOLO_NMS_MAX] = {0};
    int nms_count = 0;

    for (int i = 0; i < num_detections; ++i) {
        NMSBox box = {0};

        // Box coordinates are normalized to [0, 1]
        box.x = data[i * elements_per_detection + 0];
        box.y = data[i * elements_per_detection + 1];
        box.w = data[i * elements_per_detection + 2];
        box.h = data[i * elements_per_detection + 3];

        // Objectness score
        float objectness = data[i * elements_per_detection + 4];

        // Find max class score
        float max_class_score = 0.0f;
        int class_id = -1;
        for (int j = 0; j < num_classes; ++j) {
            float class_score = data[i * elements_per_detection + 5 + j];
            if (class_score > max_class_score) {
                max_class_score = class_score;
                class_id = j;
            }
        }

        if (class_id < 0) {
            // warn bad class found in the model
            //detections->error_id |= YOLO_ERROR_INVAL_CLASS_ID;
            continue;
        }

        box.class_id = class_id;
        box.score = objectness * max_class_score;
        if (!box.score) continue;
        if (box.score >= conf_thres) {
            boxes[nms_count++] = box;
            if (nms_count >= YOLO_NMS_MAX) {
                // warn NMS box count reached max
                //detections->error_id |= YOLO_ERROR_NMS_MAX;
                break;
            }
        }
    }

    // Apply Non-Maximum Suppression (NMS)
    TfLiteRunYolo_Apply_NMS(detections, boxes, nms_count, input_width, input_height, conf_thres);

    return EHS_ML_OK;
}

typedef struct {
    size_t index;
    ehs_char *json;
    ehs_uint32 json_size;
    ehs_float conf_thres;
} HailoOutputThreadArg;

static ehs_bool TfLiteFlatJsonObjectDetection(EhsML_Type type, TfLiteBoxDetections* detections, ehs_char* json, ehs_uint32 size){
    printf("TfLiteFlatJsonObjectDetection called\n");
    if (!detections || !json || size == 0) return EHS_FALSE;
    printf("Reached here 1\n");
    ehs_uint32 used = snprintf(json, size, "{\"type\":%d,\"proc_time\":%d,\"err_no\":%d,\"det_cnt\":%d", 
                               type, detections->processing_time_ms, detections->error_id, detections->box_detections_count);
    if (used >= size) return EHS_FALSE;
    printf("Reached here 2\n");

    for (int i = 0; i < detections->box_detections_count; i++) {
        TfLiteBoxDetection* d = &(detections->box_detections[i]);

        int remaining = size - used - 1; // reserve space for closing } and null terminator
        if (remaining <= 0) break;

        int len = snprintf(json + used, remaining,
            ",\"cls%d\":%d,\"cnf%d\":%.2f,\"x%d\":%u,\"y%d\":%u,\"w%d\":%u,\"h%d\":%u",
            i, d->class_id,
            i, d->confidence,
            i, d->x,
            i, d->y,
            i, d->w,
            i, d->h);

        if (len <= 0 || len >= remaining) break;

        used += (ehs_uint32)len;
    }

    // Close the JSON object
    if (used + 1 >= size) return EHS_FALSE;
    printf("Reached here 3\n");
    json[used++] = '}';
    json[used] = '\0'; // null-terminate the string
    printf("Reached here 4\n");
    printf("JSON: %s\n", json);

    return EHS_TRUE;
}

static thread_return_type read_from_device(void *args)
{
    hailo_status status = HAILO_UNINITIALIZED;
    double time_spent = 0;
    HailoOutputThreadArg* thread_args = (HailoOutputThreadArg*)args;
    TfLiteBoxDetections detections = {0};
    printf("read_from_device called for index %zu\n", thread_args->index);

    status = hailo_vstream_read_raw_buffer(gOutputVStream[thread_args->index], gOutputBuffer, gOutputBufferSize[thread_args->index]);
    if (status != HAILO_SUCCESS)
    {
        EHSH_LOG_ERROR("hailo_vstream_read_raw_buffer failed: %d\n", status);
        printf("hailo_vstream_read_raw_buffer failed: %d\n", status);
        return (thread_return_type)status;
    }

    // EhsML_Err err = EhsInternal_PostProcess_TFLite_YoloV5(&detections, (float32_t*)gOutputBuffer, gOutputBufferSize[thread_args->index], 640, 480, 0.25f);
    // if (err != EHS_ML_OK)
    // {
    //     EHSH_LOG_ERROR("EhsInternal_PostProcess_TFLite_YoloV5 failed: %d\n", err);
    //     printf("EhsInternal_PostProcess_TFLite_YoloV5 failed: %d\n", err);
    //     return (thread_return_type)EHS_ML_FAILED;
    // }

    // if (TfLiteFlatJsonObjectDetection(EHS_ML_ACCELERATOR_HAILO, &detections, thread_args->json, thread_args->json_size) == EHS_FALSE)
    // {
    //     EHSH_LOG_ERROR("TfLiteFlatJsonObjectDetection failed\n");
    //     printf("TfLiteFlatJsonObjectDetection failed\n");
    //     return (thread_return_type)EHS_ML_FAILED;
    // }

    /******* DELETEME */
    if (stored_time != 0) {
        clock_t current_time = clock();
        time_spent = (double)(current_time - stored_time) / CLOCKS_PER_SEC * 1000;
        stored_time = current_time;
    } else {
        stored_time = clock();
    }
    size_t class_idx = 0;
    size_t index = -1;
    bool printed = false;
    size_t printed_count = 0;
    ehs_char *_string = thread_args->json;
    size_t string_index = 0;
    string_index += EhsSprintf(&(thread_args->json[string_index]), "{");
    while (class_idx < 80)
    {
        size_t num_of_class_boxes = (size_t)((float32_t*)gOutputBuffer)[++index];
        for (size_t box_idx = 0 ; box_idx < num_of_class_boxes ; box_idx++)
        {
            float32_t y_min = ((float32_t*)gOutputBuffer)[++index] *480;
            float32_t x_min = ((float32_t*)gOutputBuffer)[++index] *640;
            float32_t y_max = ((float32_t*)gOutputBuffer)[++index] *480;
            float32_t x_max = ((float32_t*)gOutputBuffer)[++index] *640;
            float32_t conf = ((float32_t*)gOutputBuffer)[++index];
            if (conf >= thread_args->conf_thres) {
                //printf("Class %zu (%s): %.2f%% at [y_min=%.2f, x_min=%.2f, y_max=%.2f, x_max=%.2f]\n",
                string_index += EhsSprintf(&(thread_args->json[string_index]), "\"cls%d\":\"%s\",\"cnf%d\":%.2f,\"ymin%d\":%.2f,\"xmin%d\":%.2f,\"ymax%d\":%.2f,\"xmax%d\":%.2f,",
                    printed_count, get_coco_name_from_int((int)class_idx + 1), 
                    printed_count, conf, 
                    printed_count, y_min, 
                    printed_count, x_min, 
                    printed_count, y_max, 
                    printed_count, x_max
                );
                printed = true;
                printed_count++;
            }
        }
        class_idx++;
    }
    string_index += EhsSprintf(&(thread_args->json[string_index]), "\"det_cnt\":%d", printed_count);
    if (printed)
    {
        if (time_spent != 0) string_index += EhsSprintf(&(thread_args->json[string_index]), ",\"time\":%.3f", time_spent);
    }
    string_index += EhsSprintf(&(thread_args->json[string_index]), "}");
    /******* DELETEME */

    printf("Thread for index %zu finished successfully\n", thread_args->index);
    return (thread_return_type)HAILO_SUCCESS;
}

static hailo_status infer(ehs_char *json_output_buffer, ehs_uint32 json_output_buffer_size, ehs_float conf_thres)
{
    hailo_status status = HAILO_UNINITIALIZED;
    hailo_thread write_threads[HAILO_MAX_EDGE_LAYERS] = {0};
    hailo_thread read_threads[HAILO_MAX_EDGE_LAYERS] = {0};
    hailo_status write_thread_status = HAILO_UNINITIALIZED;
    hailo_status read_thread_status = HAILO_UNINITIALIZED;
    ehs_uint32 input_threads_index = 0;
    ehs_uint32 output_threads_index = 0;
    ehs_uint32 i = 0;

    HailoOutputThreadArg read_thread_args = {0};
    read_thread_args.json = json_output_buffer;
    read_thread_args.json_size = json_output_buffer_size;
    read_thread_args.conf_thres = conf_thres;

    for (output_threads_index = 0; output_threads_index < gOutputVStreamsSize; output_threads_index++)
    {
        read_thread_args.index = output_threads_index;
        status = hailo_create_thread(read_from_device, &read_thread_args, &read_threads[output_threads_index]);
        if (status != HAILO_SUCCESS)
        {
            EHSH_LOG_ERROR("hailo_create_thread failed: %d\n", status);
            goto l_cleanup;
        }
    }
    for (input_threads_index = 0; input_threads_index < gInputVStreamsSize; input_threads_index++)
    {
        status = hailo_create_thread(write_to_device, input_threads_index, &write_threads[input_threads_index]);
        if (status != HAILO_SUCCESS)
        {
            EHSH_LOG_ERROR("hailo_create_thread failed: %d\n", status);
            goto l_cleanup;
        }
    }

l_cleanup:
    for (i = 0 ; i < input_threads_index ; i++)
    {
        write_thread_status = hailo_join_thread(&write_threads[i]);
        if (write_thread_status != HAILO_SUCCESS)
        {
            EHSH_LOG_ERROR("hailo_join_thread failed: %d\n", write_thread_status);
            status = write_thread_status;
        }
    }

    for (i = 0 ; i < output_threads_index ; i++)
    {
        read_thread_status = hailo_join_thread(&read_threads[i]);
        if (read_thread_status != HAILO_SUCCESS)
        {
            EHSH_LOG_ERROR("hailo_join_thread failed: %d\n", read_thread_status);
            status = read_thread_status;
        }
    }

    return status;
}

EhsML_Err EhsML_Create(EhsML_Context* ctx, const ehs_char* model_path, EhsML_Type model_type, ehs_float conf_thres, ehs_sint32 thread_count)
{
    if (!check_hailo_version()) return EHS_ML_INVALID_DEP;

    if (ctx == NULL || model_path == NULL)
    {
        EHSH_LOG_ERROR("Invalid argument: ctx or model_path is NULL\n");
        return EHS_ML_INIT_ERR;
    }

    EhsML_Err _error = EHS_ML_OK;
    hailo_configure_params_t config_params = {0};
    size_t network_group_size = 1;
    bool unused = {0};
    ehs_uint32 i = 0;

    size_t number_of_devices = 0;
    hailo_pcie_device_info_t pcie_device_info[8];

    gHailoStatus = hailo_scan_pcie_devices(pcie_device_info, 8, &number_of_devices);
    gHailoStatus = hailo_create_pcie_device(&pcie_device_info[0], &gHailoDevice);
    if (gHailoStatus != HAILO_SUCCESS)
    {
        EHSH_LOG_ERROR("hailo_create_pcie_device failed: %d\n", gHailoStatus);
        _error = EHS_ML_INIT_ERR;
        goto l_error_exit;
    }

    gHailoStatus = hailo_create_hef_file(&gHailoHef, model_path);
    if (gHailoStatus != HAILO_SUCCESS)
    {
        EHSH_LOG_ERROR("hailo_read_hef_from_file failed: %d\n", gHailoStatus);
        _error = EHS_ML_MODEL_LOAD_ERR;
        goto l_release_vdevice;
    }

    gHailoStatus = hailo_init_configure_params(gHailoHef, HAILO_STREAM_INTERFACE_PCIE, &config_params);
    if (gHailoStatus != HAILO_SUCCESS)
    {
        EHSH_LOG_ERROR("hailo_init_configure_params failed: %d\n", gHailoStatus);
        _error = EHS_ML_INIT_ERR;
        goto l_release_hef;
    }

    gHailoStatus = hailo_configure_device(gHailoDevice, gHailoHef, &config_params, &gHailoConfiguredNNG, &network_group_size);
    if (gHailoStatus != HAILO_SUCCESS)
    {
        EHSH_LOG_ERROR("hailo_configure_device failed: %d\n", gHailoStatus);
        _error = EHS_ML_INIT_ERR;
        goto l_release_hef;
    }
    if (network_group_size != 1)
    {
        EHSH_LOG_ERROR("Invalid network group size %zu\n", network_group_size);
        gHailoStatus = HAILO_INVALID_ARGUMENT;
        _error = EHS_ML_INIT_ERR;
        goto l_release_hef;
    }

    // When the type is auto, there is no quantisation applied to the data
    gHailoStatus = hailo_make_input_vstream_params(gHailoConfiguredNNG, unused, HAILO_FORMAT_TYPE_AUTO, gHailoInputVStreamParams, &gInputVStreamsSize);
    if (gHailoStatus != HAILO_SUCCESS)
    {
        EHSH_LOG_ERROR("hailo_make_input_vstream_params failed: %d\n", gHailoStatus);
        _error = EHS_ML_INIT_ERR;
        goto l_release_hef;
    }

    // Override the input format order
    for (i = 0; i < gInputVStreamsSize; i++)
    {
        gHailoInputVStreamParams[i].params.user_buffer_format.order = HAILO_FORMAT_ORDER_NHWC; // HAILO_FORMAT_ORDER_AUTO;
    }

    // When the type is auto, there is no de-quantisation applied to the data
    gHailoStatus = hailo_make_output_vstream_params(gHailoConfiguredNNG, unused, HAILO_FORMAT_TYPE_AUTO, gHailoOutputVStreamParams, &gOutputVStreamsSize);
    if (gHailoStatus != HAILO_SUCCESS)
    {
        EHSH_LOG_ERROR("hailo_make_output_vstream_params failed: %d\n", gHailoStatus);
        _error = EHS_ML_INIT_ERR;
        goto l_release_hef;
    }

    if (gInputVStreamsSize > HAILO_MAX_EDGE_LAYERS || gOutputVStreamsSize > HAILO_MAX_EDGE_LAYERS)
    {
        EHSH_LOG_ERROR("Trying to infer network with too many input/output virtual streams, Maximum amount is %d, (either change HEF or change the definition of HAILO_MAX_EDGE_LAYERS)\n", HAILO_MAX_EDGE_LAYERS);
        gHailoStatus = HAILO_INVALID_OPERATION;
        _error = EHS_ML_INIT_ERR;
        goto l_release_hef;
    }

    gHailoStatus = hailo_create_input_vstreams(gHailoConfiguredNNG, gHailoInputVStreamParams, gInputVStreamsSize, gInputVStream);
    if (gHailoStatus != HAILO_SUCCESS)
    {
        EHSH_LOG_ERROR("hailo_create_input_vstreams failed: %d\n", gHailoStatus);
        _error = EHS_ML_INIT_ERR;
        goto l_release_input_vstream;
    }

    gHailoStatus = hailo_create_output_vstreams(gHailoConfiguredNNG, gHailoOutputVStreamParams, gOutputVStreamsSize, gOutputVStream);
    if (gHailoStatus != HAILO_SUCCESS)
    {
        EHSH_LOG_ERROR("hailo_create_output_vstreams failed: %d\n", gHailoStatus);
        _error = EHS_ML_INIT_ERR;
        goto l_release_output_vstream;
    }

    gHailoStatus = hailo_get_input_vstream_frame_size(gInputVStream[0], &gInputBufferSize[0]);
    if (gHailoStatus != HAILO_SUCCESS)
    {
        EHSH_LOG_ERROR("hailo_get_input_vstream_frame_size failed: %d\n", gHailoStatus);
        _error = EHS_ML_INIT_ERR;
        goto l_release_output_vstream;
    }
    if (gInputBuffer == NULL) gInputBuffer = (ehs_uint8 *)EhsHMem_tempAlloc(gInputBufferSize[0] * sizeof(ehs_uint8));
    if (gInputBuffer == NULL)
    {
        EHSH_LOG_ERROR("EhsHMem_tempAlloc failed to allocate input buffer\n");
        gHailoStatus = HAILO_OUT_OF_HOST_MEMORY;
        _error = EHS_ML_MEMORY_ERR;
        goto l_release_output_vstream;
    }

    gHailoStatus = hailo_get_output_vstream_frame_size(gOutputVStream[0], &gOutputBufferSize[0]);
    if (gHailoStatus != HAILO_SUCCESS)
    {
        EHSH_LOG_ERROR("hailo_get_output_vstream_frame_size failed: %d\n", gHailoStatus);
        _error = EHS_ML_INIT_ERR;
        goto l_release_buffers;
    }
    // Assuming all output vstreams have the same frame size. This may not be true.
    if (gOutputBuffer == NULL) gOutputBuffer = (float32_t *)EhsHMem_tempAlloc(gOutputBufferSize[0] * sizeof(float32_t));
    if (gOutputBuffer == NULL)
    {
        EHSH_LOG_ERROR("EhsHMem_tempAlloc failed to allocate output buffer\n");
        gHailoStatus = HAILO_OUT_OF_HOST_MEMORY;
        _error = EHS_ML_MEMORY_ERR;
        goto l_release_buffers;
    }

    gHailoStatus = hailo_activate_network_group(gHailoConfiguredNNG, NULL, &gActivatedNetworkGroup);
    if (gHailoStatus != HAILO_SUCCESS)
    {
        EHSH_LOG_ERROR("hailo_activate_network_group failed: %d\n", gHailoStatus);
        _error = EHS_ML_INIT_ERR;
        goto l_release_buffers;
    }

    ctx->ml_model_ctx = (void*)gHailoConfiguredNNG;
    
    _error = EHS_ML_OK;
    return _error;

l_release_buffers:
    if (gInputBuffer != NULL)
    {
        EhsHMem_tempFree(gInputBuffer);
        gInputBuffer = NULL;
    }
    if (gOutputBuffer != NULL)
    {
        EhsHMem_tempFree(gOutputBuffer);
        gOutputBuffer = NULL;
    }
l_release_output_vstream:
    (void) hailo_release_output_vstreams(gOutputVStream, gOutputVStreamsSize);
    for (size_t i = 0; i < gOutputVStreamsSize; i++)
    {
        gOutputVStream[i] = NULL;
    }
l_release_input_vstream:
    (void) hailo_release_input_vstreams(gInputVStream, gInputVStreamsSize);
    for (size_t i = 0; i < gInputVStreamsSize; i++)
    {
        gInputVStream[i] = NULL;
    }
l_release_hef:
    (void) hailo_release_hef(gHailoHef);
    gHailoHef = NULL;
l_release_vdevice:
    (void) hailo_release_device(gHailoDevice);
    gHailoDevice = NULL;
l_error_exit:
    return _error;
}

void EhsML_Destroy(EhsML_Context* ctx)
{
    if (!check_hailo_version()) return;
    // Release resources
    if (gActivatedNetworkGroup != NULL)
    {
        hailo_deactivate_network_group(gActivatedNetworkGroup);
        gActivatedNetworkGroup = NULL;
    }
    if (gInputBuffer != NULL)
    {
        EhsHMem_tempFree(gInputBuffer);
        gInputBuffer = NULL;
    }
    if (gOutputBuffer != NULL)
    {
        EhsHMem_tempFree(gOutputBuffer);
        gOutputBuffer = NULL;
    }
    if (gInputVStream[0] != NULL)
    {
        (void) hailo_release_input_vstreams(gInputVStream, HAILO_MAX_EDGE_LAYERS);
        for (size_t i = 0; i < HAILO_MAX_EDGE_LAYERS; i++)
        {
            gInputVStream[i] = NULL;
        }
    }
    if (gOutputVStream[0] != NULL)
    {
        (void) hailo_release_output_vstreams(gOutputVStream, HAILO_MAX_EDGE_LAYERS);
        for (size_t i = 0; i < HAILO_MAX_EDGE_LAYERS; i++)
        {
            gOutputVStream[i] = NULL;
        }
    }
    if (gHailoHef != NULL)
    {
        (void) hailo_release_hef(gHailoHef);
        gHailoHef = NULL;
    }
    if (gHailoDevice != NULL)
    {
        (void) hailo_release_device(gHailoDevice);
        gHailoDevice = NULL;
    }
    gHailoStatus = HAILO_UNINITIALIZED;
    ctx->ml_model_ctx = NULL;
}

EhsML_Err EhsML_SetInputData(EhsML_Context* ctx, const void* data, ehs_uint32 size)
{
    if (data == NULL || size == 0)
    {
        EHSH_LOG_ERROR("Invalid argument: data is NULL or size is 0\n");
        return EHS_ML_MEMORY_ERR;
    }
    if (size > gInputBufferSize[0])
    {
        EHSH_LOG_ERROR("Input data size %u exceeds maximum allowed size %d\n", size, gInputBufferSize[0]);
        return EHS_ML_MEMORY_ERR;
    }
    if (!check_hailo_version()) return EHS_ML_INVALID_DEP;
    EhsMemcpy(gInputBuffer, data, size);
    return EHS_ML_OK;
}

EhsML_Err EhsML_RunOutputJson(EhsML_Context* ctx, ehs_char* json, ehs_uint32 size)
{
    if (json == NULL || size == 0)
    {
        EHSH_LOG_ERROR("Invalid argument: json is NULL or size is 0\n");
        return EHS_ML_MEMORY_ERR;
    }
    if (!check_hailo_version()) return EHS_ML_INVALID_DEP;
    gHailoStatus = infer(json, size, ctx->conf_thres);
    if (gHailoStatus != HAILO_SUCCESS)
    {
        EHSH_LOG_ERROR("infer failed: %d\n", gHailoStatus);
        return EHS_ML_FAILED;
    }
    return EHS_ML_OK;
}