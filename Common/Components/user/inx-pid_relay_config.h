#ifndef INXWARE_pid_relay_config
#define INXWARE_pid_relay_config
#include "inx-component.h"

/*****************************************************
 * Configure PID controller relayes
 *****************************************************/
#define INXWARE_FB_ID_pid_relay_config 0xCB2A
#define INXWARE_FB_NAME_pid_relay_config "pid_relay_config"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(pid_relay_config); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(pid_relay_config); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(pid_relay_config); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(pid_relay_config_configure); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(pid_relay_config)

#endif /* INXWARE_pid_relay_config */
