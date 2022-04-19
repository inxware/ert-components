#ifndef INXWARE_DevmanInterface
#define INXWARE_DevmanInterface
#include "inx-component.h"

/*****************************************************
 * This component reports data to the Portal using Devman Monitor and gets the Devman settings
 *****************************************************/
#define INXWARE_FB_DevmanInterface "DevmanInterface"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(DevmanInterface); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(DevmanInterface); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(DevmanInterface); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DevmanInterface_send); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DevmanInterface_getServerUrl); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DevmanInterface_listen4New); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DevmanInterface_getMiscData); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(DevmanInterface)

#endif /* INXWARE_DevmanInterface */
