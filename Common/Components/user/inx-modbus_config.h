/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

#ifndef INXWARE_modbus_config
#define INXWARE_modbus_config
#include "inx-component.h"

/*****************************************************
 * modbus_config
 *****************************************************/

#define INXWARE_FB_NAME_modbus_config "modbus_config"
#define INXWARE_FB_ID_modbus_config 0x4DB3

/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(modbus_config); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(modbus_config); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(modbus_config); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(modbus_config_enable); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(modbus_config_disable); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(modbus_config)

#endif /* INXWARE_modbus_config */
