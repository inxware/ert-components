/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * LGPLv3 license - see https://www.gnu.org/licenses/lgpl-3.0.txt
 ***************************************************************/

#ifndef INXWARE_esim_factory_prov
#define INXWARE_esim_factory_prov
#include "inx-component.h"

/*****************************************************
 * Local (in-factory) eSIM profile package install
 *****************************************************/
#define INXWARE_FB_ID_esim_factory_prov 0x4604
#define INXWARE_FB_NAME_esim_factory_prov "esim_factory_prov"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(esim_factory_prov); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(esim_factory_prov); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(esim_factory_prov); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(esim_factory_prov_do_read_eid); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(esim_factory_prov_do_begin_install); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(esim_factory_prov_do_push_segment); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(esim_factory_prov_do_commit_install); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(esim_factory_prov_do_abort_install); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(esim_factory_prov)

#endif /* INXWARE_esim_factory_prov */
