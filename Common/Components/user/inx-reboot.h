#ifndef INXWARE_Reboot
#define INXWARE_Reboot
#include "inx-component.h"

/*****************************************************
 * Trigger to reboot the device.
 *****************************************************/
#define INXWARE_FB_ID_Reboot 0xC843
#define INXWARE_FB_NAME_Reboot "Reboot"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(Reboot); // this relates to C file identify function
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(Reboot); // this relates to C-file init function
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(Reboot); //this relates to C-file destroy function
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(Reboot_reboot); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(Reboot)

#endif /* INXWARE_Reboot */
