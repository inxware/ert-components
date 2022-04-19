#ifndef INXWARE_permanent_storage
#define INXWARE_permanent_storage
#include "inx-component.h"
/*****************************************************
 * permanent_storage
 *****************************************************/
#define INXWARE_FB_permanent_storage_int "permanent_storage_int"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(permanent_storage_int); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(permanent_storage_int); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(permanent_storage_int); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(permanent_storage_int_write); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(permanent_storage_int_read); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(permanent_storage_int)

#define INXWARE_FB_permanent_storage_bool "permanent_storage_bool"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(permanent_storage_bool); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(permanent_storage_bool); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(permanent_storage_bool); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(permanent_storage_bool_write); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(permanent_storage_bool_read); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(permanent_storage_bool)

#define INXWARE_FB_permanent_storage_real "permanent_storage_real"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(permanent_storage_real); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(permanent_storage_real); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(permanent_storage_real); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(permanent_storage_real_write); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(permanent_storage_real_read); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(permanent_storage_real)

#define INXWARE_FB_permanent_storage_string "permanent_storage_string"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(permanent_storage_string); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(permanent_storage_string); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(permanent_storage_string); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(permanent_storage_string_write); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(permanent_storage_string_read); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(permanent_storage_string)

#endif /* INXWARE_permanent_storage */
