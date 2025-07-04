/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms 
* of the LGPLv3 license. You should have received a copy of the 
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
* not, please visit 
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

#ifndef INXWARE_adc_read
#define INXWARE_adc_read
#include "inx-component.h"

/*****************************************************
 * adc_read
 *****************************************************/

#define INXWARE_FB_NAME_adc_read "adc_read"
#define INXWARE_FB_ID_adc_read 0xA05C

/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(adc_read); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(adc_read); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(adc_read); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(adc_read_read); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(adc_read_sample); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(adc_read)

#endif /* INXWARE_adc_read */
