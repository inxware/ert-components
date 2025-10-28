/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

#ifndef INXWARE_application_run
#define INXWARE_application_run
#include "inx-component.h"

/*****************************************************
 * Runs an app
 *****************************************************/
#define INXWARE_FB_NAME_application_run "application_run"
#define INXWARE_FB_ID_application_run 0xB273

/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(application_run); // this relates to C file identify function
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(application_run); // this relates to C-file init function
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(application_run_runApp); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(application_run)

#endif /* INXWARE_application_run */
