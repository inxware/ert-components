#ifndef INXWARE_modbus_write
#define INXWARE_modbus_write
#include "inx-component.h"

/*****************************************************
 * MODBUS-RT output/write 
 *****************************************************/
#define INXWARE_FB_ID_modbus_write 0x7801
#define INXWARE_FB_NAME_modbus_write "modbus_write"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(modbus_write); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(modbus_write); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(modbus_write); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(modbus_write_write); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(modbus_write)

#endif /* INXWARE_modbus_write */
