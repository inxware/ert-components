#ifndef INXWARE_FrameCrop
#define INXWARE_FrameCrop
#include "inx-component.h"

/*****************************************************
 * Crops camera frame
 *****************************************************/
#define INXWARE_FB_ID_FrameCrop 0x6631
#define INXWARE_FB_NAME_FrameCrop "FrameCrop"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(FrameCrop); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(FrameCrop); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(FrameCrop); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(FrameCrop_run_cropping); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(FrameCrop)

#endif /* INXWARE_FrameCrop */
