#ifndef INXWARE_network_config
#define INXWARE_network_config
#include "inx-component.h"

/*****************************************************
 * Configures network interface
 *****************************************************/
#define INXWARE_FB_ID_network_config 0x6B0B
#define INXWARE_FB_NAME_network_config "network_config"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(network_config); // this relates to C file identify function
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(network_config); // this relates to C-file init function
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(network_config); //this relates to C-file destroy function
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(network_config_get_status); //a function executed by a start port
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(network_config_set_config); //a function executed by a start port
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(network_config_get_config); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(network_config)

#endif /* INXWARE_network_config */
