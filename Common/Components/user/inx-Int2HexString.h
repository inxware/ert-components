#ifndef INXWARE_Int2HexString
#define INXWARE_Int2HexString
#include "inx-component.h"

/*****************************************************
 * Convert an integer to the string that contains hex data, e.g. 10 to "0A".
 *****************************************************/
#define INXWARE_FB_ID_Int2HexString 0x0FE6
#define INXWARE_FB_NAME_Int2HexString "Int2HexString"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(Int2HexString); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(Int2HexString); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(Int2HexString); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(Int2HexString_convert); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(Int2HexString)

#endif /* INXWARE_Int2HexString */
