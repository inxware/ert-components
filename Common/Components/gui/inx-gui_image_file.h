#ifndef INXWARE_GUI_Image_File
#define INXWARE_GUI_Image_File
#include "inx-component.h"

/*****************************************************
 * 
 *****************************************************/
#define EHS_FB_NAME_GUI_Image_File "GUI_Image_File"
#define EHS_FB_ID_GUI_Image_File 0xE8E1
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(GUI_Image_File); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(GUI_Image_File); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(GUI_Image_File); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(GUI_Image_File_create); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(GUI_Image_File_destroy); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(GUI_Image_File_load); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(GUI_Image_File_show); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(GUI_Image_File_hide); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(GUI_Image_File_update); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(GUI_Image_File)

#endif /* INXWARE_GUI_Image_File */
