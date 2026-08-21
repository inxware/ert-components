/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * LGPLv3 license - see https://www.gnu.org/licenses/lgpl-3.0.txt
 ***************************************************************/

#ifndef INXWARE_sgp32_download
#define INXWARE_sgp32_download
#include "inx-component.h"

/*****************************************************
 * Remote eSIM profile download (GSMA SGP.32 RSP)
 *****************************************************/
#define INXWARE_FB_ID_sgp32_download 0x0C2B
#define INXWARE_FB_NAME_sgp32_download "sgp32_download"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(sgp32_download); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(sgp32_download); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(sgp32_download); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(sgp32_download_do_set_eim); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(sgp32_download_do_download); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(sgp32_download)

#endif /* INXWARE_sgp32_download */
