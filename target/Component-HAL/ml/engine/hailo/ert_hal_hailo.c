#include "ert_hal_hailo.h"
#include "hal_ml.h"
#include "hal-api.h"

#include "hailo/hailort.h"
#include "hailo_thread.h"
#include <threads.h>

#define EHS_HAILO_LIB_VERSION_MAJOR 4
#define EHS_HAILO_LIB_VERSION_MINOR 22
#define EHS_HAILO_LIB_VERSION_REV 0

static hailo_version_t gHailoVersion = {0, 0, 0};

static ehs_bool check_hailo_version() {
    if (gHailoVersion.major == 0) hailo_get_library_version(&gHailoVersion);
    if (gHailoVersion.major != EHS_HAILO_LIB_VERSION_MAJOR ||
        gHailoVersion.minor < EHS_HAILO_LIB_VERSION_MINOR)
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

static size_t gInputBufferSize[HAILO_MAX_EDGE_LAYERS] = {0};
static size_t gOutputBufferSize[HAILO_MAX_EDGE_LAYERS] = {0};

static hailo_input_vstream gInputVStream[HAILO_MAX_EDGE_LAYERS] = {NULL};
static hailo_output_vstream gOutputVStream[HAILO_MAX_EDGE_LAYERS] = {NULL};

static size_t gInputVStreamsSize = HAILO_MAX_EDGE_LAYERS;
static size_t gOutputVStreamsSize = HAILO_MAX_EDGE_LAYERS;

static hailo_activated_network_group gActivatedNetworkGroup = NULL;

static clock_t stored_time = 0;

typedef struct {
    size_t index;
    EhsML_Context *ctx;
} HailoInputThreadArg;

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
    HailoInputThreadArg *thread_args = (HailoInputThreadArg *)args;
    {
        status = hailo_vstream_write_raw_buffer(gInputVStream[thread_args->index], thread_args->ctx->input_tensor[thread_args->index].data_ptr.ptr, gInputBufferSize[thread_args->index]);
        if (status != HAILO_SUCCESS)
        {
            EHSH_LOG_ERROR("hailo_vstream_write_raw_buffer failed: %d\n", status);
            printf("hailo_vstream_write_raw_buffer failed: %d\n", status);
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

typedef struct {
    size_t index;
    EhsML_Context *ctx;
} HailoOutputThreadArg;

static thread_return_type read_from_device(void *args)
{
    hailo_status status = HAILO_UNINITIALIZED;
    double time_spent = 0;
    HailoOutputThreadArg* thread_args = (HailoOutputThreadArg*)args;
    printf("read_from_device called for index %zu\n", thread_args->index);

    status = hailo_vstream_read_raw_buffer(gOutputVStream[thread_args->index], thread_args->ctx->output_tensor[thread_args->index].data_ptr.ptr, gOutputBufferSize[thread_args->index]);
    if (status != HAILO_SUCCESS)
    {
        EHSH_LOG_ERROR("hailo_vstream_read_raw_buffer failed: %d\n", status);
        printf("hailo_vstream_read_raw_buffer failed: %d\n", status);
        return (thread_return_type)status;
    }

    return (thread_return_type)HAILO_SUCCESS;
}

static hailo_status infer(EhsML_Context * ctx)
{
    hailo_status status = HAILO_UNINITIALIZED;
    hailo_thread write_threads[HAILO_MAX_EDGE_LAYERS] = {0};
    hailo_thread read_threads[HAILO_MAX_EDGE_LAYERS] = {0};
    hailo_status write_thread_status = HAILO_UNINITIALIZED;
    hailo_status read_thread_status = HAILO_UNINITIALIZED;
    ehs_uint32 input_threads_index = 0;
    ehs_uint32 output_threads_index = 0;
    ehs_uint32 i = 0;

    /* Each thread must receive its own args struct — passing a shared struct
     * and overwriting its index field before the thread starts causes all
     * threads to see the last index value (race condition).  For models with
     * a single output vstream (objdet) this is invisible, but for models with
     * multiple output vstreams (pose) it leaves most vstreams unread, causing
     * HailoRT to back up and report a timeout. */
    HailoInputThreadArg  write_thread_args[HAILO_MAX_EDGE_LAYERS];
    HailoOutputThreadArg read_thread_args[HAILO_MAX_EDGE_LAYERS];

    for (output_threads_index = 0; output_threads_index < gOutputVStreamsSize; output_threads_index++)
    {
        read_thread_args[output_threads_index].ctx   = ctx;
        read_thread_args[output_threads_index].index = output_threads_index;
        status = hailo_create_thread(read_from_device, &read_thread_args[output_threads_index], &read_threads[output_threads_index]);
        if (status != HAILO_SUCCESS)
        {
            EHSH_LOG_ERROR("hailo_create_thread failed: %d\n", status);
            goto l_cleanup;
        }
    }
    for (input_threads_index = 0; input_threads_index < gInputVStreamsSize; input_threads_index++)
    {
        write_thread_args[input_threads_index].ctx   = ctx;
        write_thread_args[input_threads_index].index = input_threads_index;
        status = hailo_create_thread(write_to_device, &write_thread_args[input_threads_index], &write_threads[input_threads_index]);
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

EhsML_Err EhsML_FW_Hailo_Create(EhsML_Context * ctx, const ehs_char * model_path, ehs_float conf_thres, ehs_sint32 thread_count)
{
    //
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

    if (gInputVStreamsSize > EHS_ML_LAYER_TENSORS_MAX)
    {
        EHSH_LOG_ERROR("Too many input vstreams (%d) for the context to handle. Max %d. Please increase EHS_ML_LAYER_TENSORS_MAX\n", 
            gInputVStreamsSize, EHS_ML_LAYER_TENSORS_MAX);
        _error = EHS_ML_INIT_ERR;
        goto l_release_output_vstream;
    }
    ctx->input_tensor_count = gInputVStreamsSize;

    for (int tt = 0 ; tt < gInputVStreamsSize ; tt++)
    {
        gHailoStatus = hailo_get_input_vstream_frame_size(gInputVStream[tt], &gInputBufferSize[tt]);
        if (gHailoStatus != HAILO_SUCCESS)
        {
            EHSH_LOG_ERROR("hailo_get_input_vstream_frame_size failed: %d\n", gHailoStatus);
            _error = EHS_ML_INIT_ERR;
            goto l_release_output_vstream;
        }
        hailo_vstream_info_t input_vstream_info;
        gHailoStatus = hailo_get_input_vstream_info(gInputVStream[tt], &input_vstream_info);
        if (gHailoStatus != HAILO_SUCCESS)
        {
            EHSH_LOG_ERROR("hailo_get_input_vstream_info failed: %d\n", gHailoStatus);
            _error = EHS_ML_INIT_ERR;
            goto l_release_output_vstream;
        }
        // printf("input stream name: %s, network_name: %s\n", input_vstream_info.name, input_vstream_info.network_name);
        // printf("input stream quant_info: scale=%f, zp=%f\n",
        //     input_vstream_info.quant_info.qp_scale,
        //     input_vstream_info.quant_info.qp_zp);
        // printf("input stream size in bytes: %zu\n",
        //     gInputBufferSize[0]);
        // printf("input stream shape: features=%zu, height=%zu, width=%zu\n",
        //     input_vstream_info.shape.features,
        //     input_vstream_info.shape.height,
        //     input_vstream_info.shape.width);
        switch (input_vstream_info.format.type) {
            case HAILO_FORMAT_TYPE_UINT8:
            {
                ctx->input_tensor[tt].data_type = EHS_ML_DATATYPE_UINT8;
                break;
            }
            case HAILO_FORMAT_TYPE_UINT16:
            {
                ctx->data_type = EHS_ML_DATATYPE_UINT16;
                ctx->input_tensor[tt].data_type = EHS_ML_DATATYPE_UINT16;
                break;
            }
            case HAILO_FORMAT_TYPE_FLOAT32:
            {
                ctx->data_type = EHS_ML_DATATYPE_FP32;
                ctx->input_tensor[tt].data_type = EHS_ML_DATATYPE_FP32;
                break;
            }
            default:
            {
                ctx->data_type = EHS_ML_DATATYPE_NONE;
                ctx->input_tensor[tt].data_type = EHS_ML_DATATYPE_NONE;
                break;
            }
        }
        // Input tensor size
        ctx->input_tensor[tt].num_dims = 3;
        ctx->input_tensor[tt].dims[0] = input_vstream_info.shape.features;
        ctx->input_tensor[tt].dims[1] = input_vstream_info.shape.height;
        ctx->input_tensor[tt].dims[2] = input_vstream_info.shape.width;
        // Allocate Input tensor
        if (ctx->input_tensor[tt].data_ptr.ptr != NULL)
        {
            _error = EHS_ML_MEMORY_ERR;
            goto l_release_output_vstream;
        }
        _error = EhsML_Tensor_Alloc(&ctx->input_tensor[tt], ctx->input_tensor[tt].data_type, ctx->input_tensor[tt].dims, ctx->input_tensor[tt].num_dims);
        if (_error != EHS_ML_OK)
        {
            EHSH_LOG_ERROR("Failed to allocate input tensor buffer!\n");
            gHailoStatus = HAILO_OUT_OF_HOST_MEMORY;
            goto l_release_buffers;
        }
    }

    if (gOutputVStreamsSize > EHS_ML_LAYER_TENSORS_MAX)
    {
        EHSH_LOG_ERROR("Too many output vstreams (%d) for the context to handle. Max %d. Please increase EHS_ML_LAYER_TENSORS_MAX\n", 
            gOutputVStreamsSize, EHS_ML_LAYER_TENSORS_MAX);
        _error = EHS_ML_INIT_ERR;
        goto l_release_buffers;
    }

    ctx->output_tensor_count = gOutputVStreamsSize;

    for (int tt = 0 ; tt < gOutputVStreamsSize ; tt++)
    {
        gHailoStatus = hailo_get_output_vstream_frame_size(gOutputVStream[tt], &gOutputBufferSize[tt]);
        if (gHailoStatus != HAILO_SUCCESS)
        {
            EHSH_LOG_ERROR("hailo_get_output_vstream_frame_size failed: %d\n", gHailoStatus);
            _error = EHS_ML_INIT_ERR;
            goto l_release_buffers;
        }
        hailo_vstream_info_t output_vstream_info;
        gHailoStatus = hailo_get_output_vstream_info(gOutputVStream[tt], &output_vstream_info);
        if (gHailoStatus != HAILO_SUCCESS)
        {
            EHSH_LOG_ERROR("hailo_get_output_vstream_info failed: %d\n", gHailoStatus);
            _error = EHS_ML_INIT_ERR;
            goto l_release_buffers;
        }
        ctx->output_tensor[tt].handle = malloc(sizeof(hailo_vstream_info_t));
        if (ctx->output_tensor[tt].handle == NULL){
            EHSH_LOG_ERROR("Failed to allocate memory for output tensor handle!\n");
            gHailoStatus = HAILO_OUT_OF_HOST_MEMORY;
            _error = EHS_ML_MEMORY_ERR;
            goto l_release_buffers;
        }
        ctx->output_tensor[tt].handle_owned = EHS_TRUE;
        EhsMemcpy(ctx->output_tensor[tt].handle, &output_vstream_info, sizeof(hailo_vstream_info_t));
        // printf("output stream name: %s, network_name: %s\n", output_vstream_info.name, output_vstream_info.network_name);
        // printf("output stream quant_info: scale=%f, zp=%f\n",
        //     output_vstream_info.quant_info.qp_scale,
        //     output_vstream_info.quant_info.qp_zp);
        // printf("output stream size in bytes: %zu\n",
        //     gOutputBufferSize[0]);
        // printf("output stream shape: features=%zu, height=%zu, width=%zu\n",
        //     output_vstream_info.shape.features,
        //     output_vstream_info.shape.height,
        //     output_vstream_info.shape.width);
        switch (output_vstream_info.format.type) {
            case HAILO_FORMAT_TYPE_UINT8:
            {
                ctx->data_type = EHS_ML_DATATYPE_UINT8;
                ctx->output_tensor[tt].data_type = EHS_ML_DATATYPE_UINT8;
                break;
            }
            case HAILO_FORMAT_TYPE_UINT16:
            {
                ctx->data_type = EHS_ML_DATATYPE_UINT16;
                ctx->output_tensor[tt].data_type = EHS_ML_DATATYPE_UINT16;
                break;
            }
            case HAILO_FORMAT_TYPE_FLOAT32:
            {
                ctx->data_type = EHS_ML_DATATYPE_FP32;
                ctx->output_tensor[tt].data_type = EHS_ML_DATATYPE_FP32;
                break;
            }
            default:
            {
                ctx->data_type = EHS_ML_DATATYPE_NONE;
                ctx->output_tensor[tt].data_type = EHS_ML_DATATYPE_NONE;
                break;
            }
        }
        // Output Quantisation parameters
        ctx->output_tensor[tt].quantisation_params.scale = output_vstream_info.quant_info.qp_scale;
        ctx->output_tensor[tt].quantisation_params.offset = output_vstream_info.quant_info.qp_zp;
        // Output tensor dimensions
        ctx->output_tensor[tt].num_dims = 3;
        ctx->output_tensor[tt].dims[0] = output_vstream_info.shape.features;
        ctx->output_tensor[tt].dims[1] = output_vstream_info.shape.height;
        ctx->output_tensor[tt].dims[2] = output_vstream_info.shape.width;
        // Assuming all output vstreams have the same frame size. This may not be true.
        // Allocate Output tensor variable
        if (ctx->output_tensor[tt].data_ptr.ptr != NULL)
        {
            _error = EHS_ML_MEMORY_ERR;
            goto l_release_buffers;
        }
        _error = EhsML_Tensor_Alloc(&ctx->output_tensor[tt], EHS_ML_DATATYPE_NONE, gOutputBufferSize, 1);
        if (_error != EHS_ML_OK)
        {
            EHSH_LOG_ERROR("Failed to allocate output tensor buffer!\n");
            gHailoStatus = HAILO_OUT_OF_HOST_MEMORY;
            goto l_release_buffers;
        }
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
    for (size_t tt = 0 ; tt < ctx->input_tensor_count ; tt++)
    {
        EhsML_Tensor_Free(&ctx->input_tensor[tt]);
    }
    for (size_t tt = 0 ; tt < ctx->output_tensor_count ; tt++)
    {
        EhsML_Tensor_Free(&ctx->output_tensor[tt]);
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

void EhsML_FW_Hailo_Destroy(EhsML_Context * ctx)
{
    //
    if (!check_hailo_version()) return;
    // Release resources
    if (gActivatedNetworkGroup != NULL)
    {
        hailo_deactivate_network_group(gActivatedNetworkGroup);
        gActivatedNetworkGroup = NULL;
    }
    EhsML_Tensor_Free(&ctx->input_tensor);
    EhsML_Tensor_Free(&ctx->output_tensor);
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

EhsML_Err EhsML_FW_Hailo_SetInputData(EhsML_Context * ctx, const void * input_data, ehs_uint32 data_size)
{
    if (input_data == NULL || data_size == 0)
    {
        EHSH_LOG_ERROR("Invalid argument: data is NULL or size is 0\n");
        return EHS_ML_MEMORY_ERR;
    }
    if (data_size > gInputBufferSize[0])
    {
        EHSH_LOG_ERROR("Input data size %u exceeds maximum allowed size %d\n", data_size, gInputBufferSize[0]);
        return EHS_ML_MEMORY_ERR;
    }
    if (ctx->input_tensor[0].data_ptr.ptr == NULL)
    {
        EHSH_LOG_ERROR("Input tensor is not allocated!\n");
        return EHS_ML_MEMORY_ERR;
    }
    if (!check_hailo_version()) return EHS_ML_INVALID_DEP;
    EhsML_Tensor_FillRaw(&ctx->input_tensor, input_data, data_size);
    printf("data_size: %u, gInputBufferSize[0]: %zu\n", data_size, gInputBufferSize[0]);
    return EHS_ML_OK;
}

EhsML_Err EhsML_FW_Hailo_GetOutputData(EhsML_Context * ctx)
{
    if (ctx == NULL)
    {
        EHSH_LOG_ERROR("Invalid argument: context is NULL\n");
        return EHS_ML_INIT_ERR;
    }
    if (!check_hailo_version()) return EHS_ML_INVALID_DEP;
    gHailoStatus = infer(ctx);
    if (gHailoStatus != HAILO_SUCCESS)
    {
        EHSH_LOG_ERROR("infer failed: %d\n", gHailoStatus);
        return EHS_ML_FAILED;
    }
    return EHS_ML_OK;
}