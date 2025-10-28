#ifndef INXWARE_ui_chart
#define INXWARE_ui_chart
#include "inx-component.h"

/*****************************************************
 * User interface plot chart widget
 *****************************************************/
#define INXWARE_FB_ID_ui_chart 0x839F
#define INXWARE_FB_NAME_ui_chart "ui_chart"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(ui_chart); // this relates to C file identify function
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(ui_chart); // this relates to C-file init function
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(ui_chart); //this relates to C-file destroy function
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ui_chart_create); //a function executed by a start port
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ui_chart_destroy); //a function executed by a start port
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ui_chart_show); //a function executed by a start port
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ui_chart_hide); //a function executed by a start port
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ui_chart_update); //a function executed by a start port
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ui_chart_data_update); //a function executed by a start port
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ui_chart_data); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ui_chart)

#endif /* INXWARE_ui_chart */
