/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

#ifndef INXWARE_DevmanInterface
#define INXWARE_DevmanInterface
#include "inx-component.h"

/*****************************************************
 * This component reports data to the Portal using Devman Monitor and gets the Devman settings
 *****************************************************/

#define INXWARE_FB_NAME_DevmanInterface "DevmanInterface"
#define INXWARE_FB_ID_DevmanInterface 0x061C
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(DevmanInterface); // this relates to C file identify function
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(DevmanInterface); // this relates to C-file init function
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(DevmanInterface); //this relates to C-file destroy function
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DevmanInterface_send); //a function executed by a start port
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DevmanInterface_getServerUrl); //a function executed by a start port
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DevmanInterface_setServerUrl); //a function executed by a start port
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DevmanInterface_listen4New); //a function executed by a start port
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DevmanInterface_getMiscData); //a function executed by a start port
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(DevmanInterface_getNetStatus); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(DevmanInterface)

#endif /* INXWARE_DevmanInterface */
