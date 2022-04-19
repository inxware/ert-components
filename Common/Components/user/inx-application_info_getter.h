#ifndef INXWARE_application_info_getter
#define INXWARE_application_info_getter
#include "inx-component.h"

/*****************************************************
 * Will retrieve the list of locally installed apps along with details of them such as name, description and icons.
 *****************************************************/
#define INXWARE_FB_application_info_getter "application_info_getter"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(application_info_getter); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(application_info_getter); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(application_info_getter); // this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(application_info_getter_getList); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(application_info_getter_getInfo); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(application_info_getter_setDefault); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(application_info_getter_deleteApp); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(application_info_getter_getDefault); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(application_info_getter)

#endif /* INXWARE_application_info_getter */
