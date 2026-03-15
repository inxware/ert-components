#ifndef INXWARE_ml_frame_inference
#define INXWARE_ml_frame_inference
#include "inx-component.h"

/*****************************************************
 * Pass image frame to a machine learning model for inference.
 * Supports yolov5, yolov8 and other object detection models
 * via the EhsML HAL (TFLite, Hailo, etc.).
 *****************************************************/
#define INXWARE_FB_ID_ml_frame_inference 0xFB15
#define INXWARE_FB_NAME_ml_frame_inference "ml_frame_inference"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(ml_frame_inference); // this relates to C file identify function
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(ml_frame_inference); // this relates to C-file init function
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(ml_frame_inference); //this relates to C-file destroy function
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ml_frame_inference_load_model); //a function executed by a start port
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ml_frame_inference_do_inference); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ml_frame_inference)

#endif /* INXWARE_ml_frame_inference */
