#ifndef INXWARE_interface_manager
#define INXWARE_interface_manager
#include "inx-component.h"

/*****************************************************
 * This manages the network interface. It can switch the interface between Ethernet and Wi-Fi.
 *****************************************************/
#define INXWARE_FB_ID_interface_manager 0xF2F0
#define INXWARE_FB_NAME_interface_manager "interface_manager"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(interface_manager); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(interface_manager); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(interface_manager); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(interface_manager_configure); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(interface_manager)

#endif /* INXWARE_interface_manager */
