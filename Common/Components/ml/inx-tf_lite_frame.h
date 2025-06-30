#ifndef INXWARE_TF_Lite_from_frame
#define INXWARE_TF_Lite_from_frame
#include "inx-component.h"

/*****************************************************
 * Pass image frame to machine learning Tensorflow-lite model. Supports yolov5 ...
 *****************************************************/
#define INXWARE_FB_ID_TF_Lite_from_frame 0xFB15
#define INXWARE_FB_NAME_TF_Lite_from_frame "TF_Lite_from_frame"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(TF_Lite_from_frame); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(TF_Lite_from_frame); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(TF_Lite_from_frame); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(TF_Lite_from_frame_load_model); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(TF_Lite_from_frame_do_inference); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(TF_Lite_from_frame)

#endif /* INXWARE_TF_Lite_from_frame */
