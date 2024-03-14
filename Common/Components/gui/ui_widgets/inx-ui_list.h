#ifndef INXWARE_ui_list
#define INXWARE_ui_list
#include "inx-component.h"

/*****************************************************
 * User interface plot chart widget
 *****************************************************/
#define INXWARE_FB_ID_ui_list 0xA851
#define INXWARE_FB_NAME_ui_list "ui_list"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ui_list); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ui_list); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(ui_list); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ui_list_create); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ui_list_destroy); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ui_list_show); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ui_list_hide); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ui_list_update); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ui_list_append); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ui_list_remove); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ui_list)

#endif /* INXWARE_ui_list */
