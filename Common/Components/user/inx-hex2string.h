#ifndef INXWARE_HexString2String
#define INXWARE_HexString2String
#include "inx-component.h"

/*****************************************************
 * This will convert the string that contains the hex to the string that contains the actual characters. The odd number of characters in the hex string will be prepended with 0.
 *****************************************************/
#define INXWARE_FB_ID_HexString2String 0x4746
#define INXWARE_FB_NAME_HexString2String "HexString2String"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(HexString2String); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(HexString2String); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(HexString2String); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(HexString2String_convert); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(HexString2String)

#endif /* INXWARE_HexString2String */
