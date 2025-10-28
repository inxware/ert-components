//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-apriltag.h"
//ICB HEADER MACRO END -- DO NOT ALTER
#include "apriltag.h"
#include "tag36h11.h"
#include "tag25h9.h"
#include "tag16h5.h"
#include "tagCircle21h7.h"
#include "tagCircle49h12.h"
#include "tagStandard41h12.h"
#include "tagStandard52h13.h"
#include "tagCustom48h12.h"
#include "hal_mv.h"
//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef enum {
    E_EHS_APRILTAG_ERROR_OK = 0,
    E_EHS_APRILTAG_ERROR_NO_STREAM_ID,
    E_EHS_APRILTAG_ERROR_INVALID_STREAM,
    E_EHS_APRILTAG_ERROR_INSUFFICIENT_RESOURCE,
    E_EHS_APRILTAG_ERROR_NO_OUTPUT,
    E_EHS_APRILTAG_ERROR_INVALID_INPUT,
    E_EHS_APRILTAG_ERROR_MAX
} e_ehs_apriltag_errno_t;
typedef enum {
    E_EHS_APRILTAG_FAMILY_tag36h11 = 0,
    E_EHS_APRILTAG_FAMILY_tag25h9,
    E_EHS_APRILTAG_FAMILY_tag16h5,
    E_EHS_APRILTAG_FAMILY_tagCircle21h7,
    E_EHS_APRILTAG_FAMILY_tagCircle49h12,
    E_EHS_APRILTAG_FAMILY_tagStandard41h12,
    E_EHS_APRILTAG_FAMILY_tagStandard52h13,
    E_EHS_APRILTAG_FAMILY_tagCustom48h12,
    E_EHS_APRILTAG_FAMILY_MAX,
} e_ehs_apriltag_family_t;
typedef struct inx_apriltag_state
{
    apriltag_detector_t *pAprilTagDetector;
    apriltag_family_t *pAprilTagFamily;
    e_ehs_apriltag_family_t apriltag_family;
    ehs_float quad_decimate;
    ehs_float quad_sigma;
    ehs_bool refine_edges;
    ehs_float decode_sharpening;
} inx_apriltag_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(apriltag)
EHS_FB_FUNCTION_ENTRY("config", 0x01, apriltag_config)
EHS_FB_FUNCTION_ENTRY("detect", 0x02, apriltag_detect)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_apriltag_ARG_config_quad_sigma 1
#define INX_apriltag_ARG_config_quad_decimate 2
#define INX_apriltag_ARG_config_decode_shapening 3
#define INX_apriltag_ARG_config_refine_edge 4
#define INX_apriltag_ARG_config_apriltag_family 5
#define INX_apriltag_ARG_config_config_OK 1
#define INX_apriltag_ARG_detect_stream_id 1
#define INX_apriltag_ARG_detect_errno 1
#define INX_apriltag_ARG_detect_json 2
#define INX_apriltag_ARG_detect_detect_done 1
#define INX_apriltag_ARG_detect_err 2
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER
//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
/* Parameters */
/* Create some macros for the default parameters */
#define INX_FB_apriltag_apriltag_family 0
#define INX_FB_apriltag_quad_decimate 2
#define INX_FB_apriltag_quad_sigma 0
#define INX_FB_apriltag_refine_edges 1
#define INX_FB_apriltag_decode_sharpening 0.25
//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(apriltag)
{
/* Uncomment the following if you need to parse the parameters to calculate memory required */
/*
    ehs_sint32 apriltag_family;
    ehs_float quad_decimate;
    ehs_float quad_sigma;
    ehs_bool refine_edges;
    ehs_float decode_sharpening;
    EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,"%d %f %f %d %f",&apriltag_family,&quad_decimate,&quad_sigma,&refine_edges,&decode_sharpening); */
    EHS_FB_IDENTIFY_MEMORY = sizeof(inx_apriltag_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

static apriltag_family_t *create_apriltag_family(e_ehs_apriltag_family_t family)
{
    switch (family) {
        case E_EHS_APRILTAG_FAMILY_tag36h11:		return tag36h11_create();
        case E_EHS_APRILTAG_FAMILY_tag25h9:			return tag25h9_create();
        case E_EHS_APRILTAG_FAMILY_tag16h5:			return tag16h5_create();
        case E_EHS_APRILTAG_FAMILY_tagCircle21h7:	return tagCircle21h7_create();
        case E_EHS_APRILTAG_FAMILY_tagCircle49h12:	return tagCircle49h12_create();
        case E_EHS_APRILTAG_FAMILY_tagStandard41h12:return tagStandard41h12_create();
        case E_EHS_APRILTAG_FAMILY_tagStandard52h13:return tagStandard52h13_create();
        case E_EHS_APRILTAG_FAMILY_tagCustom48h12:	return tagCustom48h12_create();
        default:									return tag36h11_create();
    }
}

static void destroy_apriltag_families(inx_apriltag_state_type *inx_apriltag_state)
{
    switch (inx_apriltag_state->apriltag_family) {
        case E_EHS_APRILTAG_FAMILY_tag36h11:		return tag36h11_destroy(inx_apriltag_state->pAprilTagFamily);
        case E_EHS_APRILTAG_FAMILY_tag25h9:			return tag25h9_destroy(inx_apriltag_state->pAprilTagFamily);
        case E_EHS_APRILTAG_FAMILY_tag16h5:			return tag16h5_destroy(inx_apriltag_state->pAprilTagFamily);
        case E_EHS_APRILTAG_FAMILY_tagCircle21h7:	return tagCircle21h7_destroy(inx_apriltag_state->pAprilTagFamily);
        case E_EHS_APRILTAG_FAMILY_tagCircle49h12:	return tagCircle49h12_destroy(inx_apriltag_state->pAprilTagFamily);
        case E_EHS_APRILTAG_FAMILY_tagStandard41h12:return tagStandard41h12_destroy(inx_apriltag_state->pAprilTagFamily);
        case E_EHS_APRILTAG_FAMILY_tagStandard52h13:return tagStandard52h13_destroy(inx_apriltag_state->pAprilTagFamily);
        case E_EHS_APRILTAG_FAMILY_tagCustom48h12:	return tagCustom48h12_destroy(inx_apriltag_state->pAprilTagFamily);
        default:									return tag36h11_destroy(inx_apriltag_state->pAprilTagFamily);
    }
    inx_apriltag_state->pAprilTagFamily = NULL;
}

EHS_FB_INIT_FUNCTION(apriltag)
{
    ehs_bool bRet = EHS_TRUE; /* assume success */
    //this is the reference to the object data for this instance of the function block
    inx_apriltag_state_type* inx_apriltag_state = (inx_apriltag_state_type*)EHS_FB_INIT_CONTEXT;
    /* read the initialisation parameters */
    EhsSscanf(EHS_FB_INIT_PARAMETERS,"%d " EHS_FL_FMT " " EHS_FL_FMT " %hhd " EHS_FL_FMT,
        &inx_apriltag_state->apriltag_family,
        &inx_apriltag_state->quad_decimate,
        &inx_apriltag_state->quad_sigma,
        &inx_apriltag_state->refine_edges,
        &inx_apriltag_state->decode_sharpening
    );

    inx_apriltag_state->pAprilTagDetector = apriltag_detector_create();
    inx_apriltag_state->pAprilTagFamily = create_apriltag_family((e_ehs_apriltag_family_t)inx_apriltag_state->apriltag_family);
    apriltag_detector_add_family(inx_apriltag_state->pAprilTagDetector, inx_apriltag_state->pAprilTagFamily);
    inx_apriltag_state->pAprilTagDetector->quad_decimate = (float)inx_apriltag_state->quad_decimate;
    inx_apriltag_state->pAprilTagDetector->quad_sigma = (float)inx_apriltag_state->quad_sigma;
    inx_apriltag_state->pAprilTagDetector->refine_edges = (bool)inx_apriltag_state->refine_edges;
    inx_apriltag_state->pAprilTagDetector->decode_sharpening = (double)inx_apriltag_state->decode_sharpening;
    inx_apriltag_state->pAprilTagDetector->nthreads = 1;
    inx_apriltag_state->pAprilTagDetector->debug = 0;

    /* Add any further intialisation code here */
    return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(apriltag)
{
    inx_apriltag_state_type *inx_apriltag_state = (inx_apriltag_state_type*)EHS_FB_DESTROY_CONTEXT;
    //Your code below here
    apriltag_detector_destroy(inx_apriltag_state->pAprilTagDetector);
    destroy_apriltag_families(inx_apriltag_state);
    return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION config MACRO START -- DO NOT ALTER
/**
 * Definition of apriltag_config.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(apriltag_config)
{
    inx_apriltag_state_type* inx_apriltag_state = (inx_apriltag_state_type*)EHS_FB_RUN_CONTEXT;

    // Your code here
    if (EHS_FB_IN_CONNECTED_API2(INX_apriltag_ARG_config_quad_sigma))
    {
        if (EHS_FB_IN_F_API2(INX_apriltag_ARG_config_quad_sigma) >= 0)
            inx_apriltag_state->pAprilTagDetector->quad_sigma = (float) EHS_FB_IN_F_API2(INX_apriltag_ARG_config_quad_sigma) ;
    }
    if (EHS_FB_IN_CONNECTED_API2(INX_apriltag_ARG_config_quad_decimate))
    {
        if (EHS_FB_IN_F_API2(INX_apriltag_ARG_config_quad_decimate) > 0)
            inx_apriltag_state->pAprilTagDetector->quad_decimate = (float) EHS_FB_IN_F_API2(INX_apriltag_ARG_config_quad_decimate) ;
    }
    if (EHS_FB_IN_CONNECTED_API2(INX_apriltag_ARG_config_decode_shapening))
    {
        if (EHS_FB_IN_F_API2(INX_apriltag_ARG_config_decode_shapening) >= 0)
            inx_apriltag_state->pAprilTagDetector->decode_sharpening = (float) EHS_FB_IN_F_API2(INX_apriltag_ARG_config_decode_shapening) ;
    }
    if (EHS_FB_IN_CONNECTED_API2(INX_apriltag_ARG_config_refine_edge))
        inx_apriltag_state->pAprilTagDetector->refine_edges = (bool) EHS_FB_IN_B_API2(INX_apriltag_ARG_config_refine_edge) ;
    if (EHS_FB_IN_CONNECTED_API2(INX_apriltag_ARG_config_apriltag_family))
    {
        if (EHS_FB_IN_I_API2(INX_apriltag_ARG_config_apriltag_family) >= 0 &&
            EHS_FB_IN_I_API2(INX_apriltag_ARG_config_apriltag_family) < E_EHS_APRILTAG_FAMILY_MAX &&
            EHS_FB_IN_I_API2(INX_apriltag_ARG_config_apriltag_family) != inx_apriltag_state->apriltag_family
        )
        {
            apriltag_detector_clear_families(inx_apriltag_state->pAprilTagDetector);
            destroy_apriltag_families(inx_apriltag_state);
            inx_apriltag_state->apriltag_family = EHS_FB_IN_I_API2(INX_apriltag_ARG_config_apriltag_family);
            inx_apriltag_state->pAprilTagFamily = create_apriltag_family(inx_apriltag_state->apriltag_family);
            apriltag_detector_add_family(inx_apriltag_state->pAprilTagDetector, inx_apriltag_state->pAprilTagFamily);
        }
    }
    EHS_FB_FINISH(INX_apriltag_ARG_config_config_OK);
}//ICB FUNCTION config MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION detect MACRO START -- DO NOT ALTER
/**
 * Definition of apriltag_detect.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(apriltag_detect)
{
    inx_apriltag_state_type* inx_apriltag_state = (inx_apriltag_state_type*)EHS_FB_RUN_CONTEXT;
    e_ehs_apriltag_errno_t error = E_EHS_APRILTAG_ERROR_OK;

    // Your code here
    // No Stream ID given
    if (!EHS_FB_IN_CONNECTED_API2(INX_apriltag_ARG_detect_stream_id))
    {
        error = E_EHS_APRILTAG_ERROR_NO_STREAM_ID;
        goto tag_return;
    }

    // Negative Stream ID is invalid
    if (EHS_FB_IN_I_API2(INX_apriltag_ARG_detect_stream_id) < 0)
    {
        error = E_EHS_APRILTAG_ERROR_INVALID_INPUT;
        goto tag_return;
    }

    // No action should be taken if JSON output is not connected
    if (!EHS_FB_OUT_CONNECTED_API2(INX_apriltag_ARG_detect_json))
    {
        error = E_EHS_APRILTAG_ERROR_NO_OUTPUT;
        goto tag_return;
    }

    // Get the pointer to the JSON output.
    //TODO This should get the string length defined somewhere, better to find and use it for memory safety
    EhsDataflowStringType detect_json = EHS_FB_OUT_S_API2(INX_apriltag_ARG_detect_json);

	EhsCameraFrame* src_frame = EhsCameraFrameGetById(EHS_FB_IN_I_API2(INX_apriltag_ARG_detect_stream_id));

    // The frame is invalid, skip
    if (src_frame == NULL)
    {
        error = E_EHS_APRILTAG_ERROR_INVALID_INPUT;
        goto tag_return;
    }

    // The image is invalid, skip
    if (src_frame->width == 0 || src_frame->height == 0)
    {
        error = E_EHS_APRILTAG_ERROR_INVALID_INPUT;
        goto tag_return;
    }

    void *frame_data = NULL;
    ehs_uint32 frame_size = 0;
    if(EHS_FALSE == EhsCameraFrameGetData(src_frame, &frame_data, &frame_size))
    {
        error = E_EHS_APRILTAG_ERROR_INVALID_STREAM;
        goto tag_return;
    }

    // The third data member is stride. The frame data must be in grayscale
    image_u8_t im = {
        src_frame->width * gEhsCameraDataFormatChanLen[src_frame->fmt], 
        src_frame->height, 
        src_frame->width * gEhsCameraDataFormatChanLen[src_frame->fmt], 
        frame_data
    };

    zarray_t *detections = apriltag_detector_detect(inx_apriltag_state->pAprilTagDetector, &im);
    if (detections == NULL)
    {
        error = E_EHS_APRILTAG_ERROR_INSUFFICIENT_RESOURCE;
        goto tag_return;
    }

    // Temporary string for each detection with worst case string length
    // i.e. len('"id00":12345,"xlt00":1234.12,"ylt00":1234.12,"xlt00":1234.12,"ylt00":1234.12,"xlt00":1234.12,"ylt00":1234.12,"xlt00":1234.12,"ylt00":1234.12,') = 141
    ehs_char temp_json[145];
    // Set JSON string start
    EhsSprintf(detect_json, "{\"n\":%d", zarray_size(detections));

    ehs_uint16 detection_id = 0;
    apriltag_detection_t *det;
    for (detection_id = 0 ; detection_id < zarray_size(detections) ; detection_id++)
    {
        zarray_get(detections, detection_id, &det);
        EhsSprintf(temp_json, ",\"id%1$02d\":%2$5d,\"xlt%1$02d\":%3$7.2lf,\"ylt%1$02d\":%4$7.2lf,\"xrt%1$02d\":%5$7.2lf,\"yrt%1$02d\":%6$7.2lf,\"xrb%1$02d\":%7$7.2lf,\"yrb%1$02d\":%8$7.2f,\"xlb%1$02d\":%9$7.2lf,\"ylb%1$02d\":%10$7.2lf",
            detection_id,
            det->id,
            det->p[0][0] / ((float)gEhsCameraDataFormatChanLen[src_frame->fmt]), det->p[0][1],
            det->p[1][0] / ((float)gEhsCameraDataFormatChanLen[src_frame->fmt]), det->p[1][1],
            det->p[2][0] / ((float)gEhsCameraDataFormatChanLen[src_frame->fmt]), det->p[2][1],
            det->p[3][0] / ((float)gEhsCameraDataFormatChanLen[src_frame->fmt]), det->p[3][1]
        );
        EhsStrcat(detect_json, temp_json);
    }

    apriltag_detections_destroy(detections);

    // End JSON string
    detect_json[EhsStrlen(detect_json) + 1] = '\0';
    detect_json[EhsStrlen(detect_json)] = '}';
    //printf("JSON: %s\r", detect_json);

tag_return:
    if (EHS_FB_OUT_CONNECTED_API2(INX_apriltag_ARG_detect_errno))
        EHS_FB_OUT_I_API2(INX_apriltag_ARG_detect_errno) = error;
    if (error != E_EHS_APRILTAG_ERROR_OK) EHS_FB_FINISH(INX_apriltag_ARG_detect_err);
    else EHS_FB_FINISH(INX_apriltag_ARG_detect_detect_done);
}//ICB FUNCTION detect MACRO END -- DO NOT ALTER THIS LINE
