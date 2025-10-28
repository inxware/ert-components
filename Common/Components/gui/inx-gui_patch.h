#ifndef INXWARE_gui_patch
#define INXWARE_gui_patch
#include "inx-component.h"

/*****************************************************
 * This widget displays a rectangular patch on the  GUI output.
 *****************************************************/
#define EHS_FB_ID_gui_patch 0xB38B
#define EHS_FB_NAME_gui_patch "gui_patch"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(gui_patch); // this relates to C file identify function
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(gui_patch); // this relates to C-file init function
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(gui_patch); //this relates to C-file destroy function
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(gui_patch_create); //a function executed by a start port
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(gui_patch_destroy); //a function executed by a start port
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(gui_patch_show); //a function executed by a start port
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(gui_patch_hide); //a function executed by a start port
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(gui_patch_update); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(gui_patch)

#endif /* INXWARE_gui_patch */
