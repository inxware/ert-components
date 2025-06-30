#ifndef INXWARE_FrameFormatter
#define INXWARE_FrameFormatter
#include "inx-component.h"

/*****************************************************
 * Modifies camera frame format e.g. int8_3 to float32_3
 *****************************************************/
#define INXWARE_FB_ID_FrameFormatter 0x521F
#define INXWARE_FB_NAME_FrameFormatter "FrameFormatter"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(FrameFormatter); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(FrameFormatter); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(FrameFormatter); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(FrameFormatter_format); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(FrameFormatter)

#endif /* INXWARE_FrameFormatter */
