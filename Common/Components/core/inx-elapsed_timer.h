#ifndef INXWARE_elapsed_timer
#define INXWARE_elapsed_timer
#include "inx-component.h"

/*****************************************************
 * This returns the elapsed time in microseconds between two event triggers.
 *****************************************************/
#define INXWARE_FB_ID_elapsed_timer 0x90EC
#define INXWARE_FB_NAME_elapsed_timer "elapsed_timer"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(elapsed_timer); // this relates to C file identify function
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(elapsed_timer); // this relates to C-file init function
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(elapsed_timer); //this relates to C-file destroy function
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(elapsed_timer_count_elasped); //a function executed by a start port
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(elapsed_timer_stop_elapsed); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(elapsed_timer)

#endif /* INXWARE_elapsed_timer */
