#ifndef INXWARE_modbus_read
#define INXWARE_modbus_read
#include "inx-component.h"

/*****************************************************
 * MODBUS-RT input/read 
 *****************************************************/
#define INXWARE_FB_ID_modbus_read 0xC573
#define INXWARE_FB_NAME_modbus_read "modbus_read"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(modbus_read); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(modbus_read); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(modbus_read); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(modbus_read_read); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(modbus_read)

#endif /* INXWARE_modbus_read */
