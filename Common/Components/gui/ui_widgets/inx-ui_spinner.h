#ifndef INXWARE_ui_spinner
#define INXWARE_ui_spinner
#include "inx-component.h"

/*****************************************************
 * User interface spinner widget
 *****************************************************/
#define INXWARE_FB_ID_ui_spinner 0xAB56
#define INXWARE_FB_NAME_ui_spinner "ui_spinner"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ui_spinner); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ui_spinner); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(ui_spinner); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ui_spinner_create); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ui_spinner_destroy); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ui_spinner_show); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ui_spinner_hide); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ui_spinner_update); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ui_spinner)

#endif /* INXWARE_ui_spinner */
