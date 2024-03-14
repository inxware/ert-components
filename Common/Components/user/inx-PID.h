#ifndef INXWARE_PID
#define INXWARE_PID
#include "inx-component.h"

/*****************************************************
 * A PID controller with tailored specific configurations.
 *****************************************************/
#define INXWARE_FB_ID_PID 0xB237
#define INXWARE_FB_NAME_PID "PID"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(PID); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(PID); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(PID); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(PID_set_value); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(PID_set_config); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(PID_control_cb); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(PID_get_value); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(PID)

#endif /* INXWARE_PID */
