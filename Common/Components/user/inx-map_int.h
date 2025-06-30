#ifndef INXWARE_MapInt
#define INXWARE_MapInt
#include "inx-component.h"

/*****************************************************
 * This an integer mapping function.
 *****************************************************/
#define INXWARE_FB_ID_MapInt 0x4455
#define INXWARE_FB_NAME_MapInt "MapInt"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(MapInt); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(MapInt); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(MapInt); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(MapInt_Run_Input); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(MapInt)

#endif /* INXWARE_MapInt */
