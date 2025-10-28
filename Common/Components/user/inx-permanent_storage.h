/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

#ifndef INXWARE_permanent_storage
#define INXWARE_permanent_storage
#include "inx-component.h"
/*****************************************************
 * permanent_storage
 *****************************************************/

#define INXWARE_FB_NAME_permanent_storage_int "permanent_storage_int"
#define INXWARE_FB_ID_permanent_storage_int 0xCDB8

/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(permanent_storage_int); // this relates to C file identify function
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(permanent_storage_int); // this relates to C-file init function
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(permanent_storage_int); //this relates to C-file destroy function
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(permanent_storage_int_write); //a function executed by a start port
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(permanent_storage_int_read); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(permanent_storage_int)


#define INXWARE_FB_NAME_permanent_storage_bool "permanent_storage_bool"
#define INXWARE_FB_ID_permanent_storage_bool 0x4B94

/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(permanent_storage_bool); // this relates to C file identify function
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(permanent_storage_bool); // this relates to C-file init function
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(permanent_storage_bool); //this relates to C-file destroy function
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(permanent_storage_bool_write); //a function executed by a start port
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(permanent_storage_bool_read); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(permanent_storage_bool)


#define INXWARE_FB_NAME_permanent_storage_real "permanent_storage_real"
#define INXWARE_FB_ID_permanent_storage_real 0xE9B4

/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(permanent_storage_real); // this relates to C file identify function
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(permanent_storage_real); // this relates to C-file init function
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(permanent_storage_real); //this relates to C-file destroy function
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(permanent_storage_real_write); //a function executed by a start port
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(permanent_storage_real_read); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(permanent_storage_real)


#define INXWARE_FB_NAME_permanent_storage_string "permanent_storage_string"
#define INXWARE_FB_ID_permanent_storage_string 0x5332

/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(permanent_storage_string); // this relates to C file identify function
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(permanent_storage_string); // this relates to C-file init function
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(permanent_storage_string); //this relates to C-file destroy function
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(permanent_storage_string_write); //a function executed by a start port
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(permanent_storage_string_read); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(permanent_storage_string)

#endif /* INXWARE_permanent_storage */
