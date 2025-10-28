#ifndef INXWARE_pid_hdwr_config_int
#define INXWARE_pid_hdwr_config_int
#include "inx-component.h"

/*****************************************************
 * Configure PID controller hardware Integer
 *****************************************************/
#define INXWARE_FB_ID_pid_hdwr_config_int 0x53FD
#define INXWARE_FB_NAME_pid_hdwr_config_int "pid_hdwr_config_int"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(pid_hdwr_config_int); // this relates to C file identify function
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(pid_hdwr_config_int); // this relates to C-file init function
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(pid_hdwr_config_int); //this relates to C-file destroy function
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(pid_hdwr_config_int_configure); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(pid_hdwr_config_int)

#endif /* INXWARE_pid_hdwr_config_int */
