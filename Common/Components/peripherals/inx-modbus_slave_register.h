#ifndef INXWARE_modbus_slave_register
#define INXWARE_modbus_slave_register
#include "inx-component.h"

/*****************************************************
 * Slave register for the MODBUS protocol.
 *****************************************************/
#define INXWARE_FB_ID_modbus_slave_register 0x5267
#define INXWARE_FB_NAME_modbus_slave_register "modbus_slave_register"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(modbus_slave_register); // this relates to C file identify function
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(modbus_slave_register); // this relates to C-file init function
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(modbus_slave_register); //this relates to C-file destroy function
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(modbus_slave_register_set); //a function executed by a start port
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(modbus_slave_register_get); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(modbus_slave_register)

#endif /* INXWARE_modbus_slave_register */
