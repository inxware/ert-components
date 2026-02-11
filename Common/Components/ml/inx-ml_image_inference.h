#ifndef INXWARE_ml_image_inference
#define INXWARE_ml_image_inference
#include "inx-component.h"

/*****************************************************
 * Inference of data input based on loaded model with vaiants like yolov8-seg. Hardware Acceleration is available for supported targets.
 *****************************************************/
#define INXWARE_FB_ID_ml_image_inference 0xF512
#define INXWARE_FB_NAME_ml_image_inference "ml_image_inference"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ml_image_inference); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ml_image_inference); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(ml_image_inference); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ml_image_inference_load_model); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ml_image_inference_inference); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ml_image_inference)

#endif /* INXWARE_ml_image_inference */
