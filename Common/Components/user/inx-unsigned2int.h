#ifndef INXWARE_Unsigned2Int
#define INXWARE_Unsigned2Int
#include "inx-component.h"

/*****************************************************
 * Convert unsigned integer with any length to a signed integer.
 *****************************************************/
#define INXWARE_FB_ID_Unsigned2Int 0x4F75
#define INXWARE_FB_NAME_Unsigned2Int "Unsigned2Int"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(Unsigned2Int); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(Unsigned2Int); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(Unsigned2Int); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(Unsigned2Int_convert); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(Unsigned2Int)

#endif /* INXWARE_Unsigned2Int */
