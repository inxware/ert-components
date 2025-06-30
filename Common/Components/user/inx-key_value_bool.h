#ifndef INXWARE_key_value_bool
#define INXWARE_key_value_bool
#include "inx-component.h"

/*****************************************************
 * Key Value Pair Storage
 *****************************************************/
#define INXWARE_FB_ID_key_value_bool 0xE471
#define INXWARE_FB_NAME_key_value_bool "key_value_bool"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(key_value_bool); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(key_value_bool); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(key_value_bool); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(key_value_bool_upsert); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(key_value_bool_query); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(key_value_bool)

#endif /* INXWARE_key_value_bool */
