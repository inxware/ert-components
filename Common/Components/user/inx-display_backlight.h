#ifndef INXWARE_Backlight
#define INXWARE_Backlight
#include "inx-component.h"

/*****************************************************
 * Set the dislpay backlight intensity
 *****************************************************/
#define INXWARE_FB_ID_Backlight 0x37A7
#define INXWARE_FB_NAME_Backlight "Backlight"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(Backlight); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(Backlight); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(Backlight); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(Backlight_set_intensity); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(Backlight)

#endif /* INXWARE_Backlight */
