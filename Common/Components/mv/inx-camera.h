#ifndef INXWARE_Camera
#define INXWARE_Camera
#include "inx-component.h"

/*****************************************************
 * Run machine learning camera
 *****************************************************/
#define INXWARE_FB_ID_Camera 0xC05A
#define INXWARE_FB_NAME_Camera "Camera"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(Camera); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(Camera); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(Camera); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(Camera_startCamera); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(Camera_grabFrame); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(Camera_stopCamera); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(Camera)

#endif /* INXWARE_Camera */
