/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

#ifndef INXWARE_JSONObject
#define INXWARE_JSONObject
#include "inx-component.h"

/*****************************************************
 * Holds a memory representation of a JSON object for querying and building.
 *****************************************************/

#define INXWARE_FB_NAME_JSONObject "JSONObject"
#define INXWARE_FB_ID_JSONObject 0x94F2

/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(JSONObject); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(JSONObject); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(JSONObject); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(JSONObject_reset); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(JSONObject_read); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(JSONObject_query); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(JSONObject)

#endif /* INXWARE_JSONObject */
