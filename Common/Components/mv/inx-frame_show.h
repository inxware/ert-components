#ifndef INXWARE_frame_show
#define INXWARE_frame_show
#include "inx-component.h"

/*****************************************************
 * Show the frame in a pop window with given frame ID input.
 *****************************************************/
#define INXWARE_FB_ID_frame_show 0x578F
#define INXWARE_FB_NAME_frame_show "frame_show"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(frame_show); // this relates to C file identify function
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(frame_show); // this relates to C-file init function
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(frame_show); //this relates to C-file destroy function
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(frame_show_show); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(frame_show)

#endif /* INXWARE_frame_show */
