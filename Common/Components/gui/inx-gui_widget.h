#ifndef INXWARE_gui_widget
#define INXWARE_gui_widget

#include "inx-component.h"

/*****************************************************
 * This widget displaed on the GUI.
 *****************************************************/

/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(gui_widget); // this relates to C file identify function
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(gui_widget); // this relates to C-file init function
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(gui_widget); //this relates to C-file destroy function

/* Per-type init functions — data type is encoded at compile time, bypassing GUI parameter string parsing */
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(gui_text_string2);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(gui_text_float2);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(gui_text_int2);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(gui_text_bool2);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(gui_widget_create); //a function executed by a start port
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(gui_widget_destroy); //a function executed by a start port
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(gui_widget_show); //a function executed by a start port
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(gui_widget_hide); //a function executed by a start port
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(gui_widget_update); //a function executed by a start port


/*****************************************************************************/
/* "String" textbox */
#define EHS_FB_NAME_gui_text_string2 "gui_text_string2"
#define EHS_FB_ID_gui_text_string2 0x150B

EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(gui_text_string2_data); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(gui_text_string2)

/*****************************************************************************/
/* "Real" textbox */

//#ifdef EHS_TARGET_FP_SUPPORT
#define EHS_FB_NAME_gui_text_real2 "gui_text_real2"
#define EHS_FB_ID_gui_text_real2 0x5149

EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(gui_text_float2_data); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(gui_text_float2)
//#endif

/*****************************************************************************/
/* Integer textbox */

#define EHS_FB_NAME_gui_text_int2 "gui_text_int2"
#define EHS_FB_ID_gui_text_int2 0x4709

EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(gui_text_int2_data); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(gui_text_int2)

/*****************************************************************************/
/* Boolean textbox */

#define EHS_FB_NAME_gui_text_bool2 "gui_text_bool2"
#define EHS_FB_ID_gui_text_bool2 0x89EA

EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(gui_text_bool2_data); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(gui_text_bool2)

#endif /* INXWARE_gui_widget */
