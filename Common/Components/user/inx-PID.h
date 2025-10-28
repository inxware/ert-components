#ifndef INXWARE_PID
#define INXWARE_PID
#include "inx-component.h"

/*****************************************************
 * A PID controller with tailored specific configurations.
 *****************************************************/
#define INXWARE_FB_ID_PID 0xB237
#define INXWARE_FB_NAME_PID "PID"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(PID); // this relates to C file identify function
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(PID); // this relates to C-file init function
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(PID); //this relates to C-file destroy function
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(PID_init); //a function executed by a start port
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(PID_new_measure); //a function executed by a start port
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(PID_set_pid_config); //a function executed by a start port
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(PID_set_io_config); //a function executed by a start port
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(PID_get_isr_value); //a function executed by a start port
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(PID_get_ctrl_value); //a function executed by a start port
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(PID_set_point_value); //a function executed by a start port
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(PID_disable_ctrl); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(PID)

#endif /* INXWARE_PID */
