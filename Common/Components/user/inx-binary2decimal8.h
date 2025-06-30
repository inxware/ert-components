#ifndef INXWARE_bin2dec8
#define INXWARE_bin2dec8
#include "inx-component.h"

/*****************************************************
 * Convert an 8-bit bool binary to an integer.
 *****************************************************/
#define INXWARE_FB_ID_bin2dec8 0xB069
#define INXWARE_FB_NAME_bin2dec8 "bin2dec8"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(bin2dec8); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(bin2dec8); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(bin2dec8); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(bin2dec8_convert); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(bin2dec8)

#endif /* INXWARE_bin2dec8 */
