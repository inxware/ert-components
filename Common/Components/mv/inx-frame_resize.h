#ifndef INXWARE_FrameResize
#define INXWARE_FrameResize
#include "inx-component.h"

/*****************************************************
 * Resize camera frame
 *****************************************************/
#define INXWARE_FB_ID_FrameResize 0xE4D7
#define INXWARE_FB_NAME_FrameResize "FrameResize"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(FrameResize); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(FrameResize); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(FrameResize); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(FrameResize_resize); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(FrameResize)

#endif /* INXWARE_FrameResize */
