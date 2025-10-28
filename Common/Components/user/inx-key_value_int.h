#ifndef INXWARE_key_value_int
#define INXWARE_key_value_int
#include "inx-component.h"

/*****************************************************
 * Key Value Pair Storage
 *****************************************************/
#define INXWARE_FB_ID_key_value_int 0x292C
#define INXWARE_FB_NAME_key_value_int "key_value_int"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(key_value_int); // this relates to C file identify function
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(key_value_int); // this relates to C-file init function
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(key_value_int); //this relates to C-file destroy function
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(key_value_int_upsert); //a function executed by a start port
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(key_value_int_query); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(key_value_int)

#endif /* INXWARE_key_value_int */
