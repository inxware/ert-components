#ifndef INXWARE_ota_data_parser
#define INXWARE_ota_data_parser
#include "inx-component.h"
/*****************************************************
 * 
 *****************************************************/


#define INXWARE_FB_ID_ota_data_parser 0x2A7D
#define INXWARE_FB_NAME_ota_data_parser "ota_data_parser"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(ota_data_parser); // this relates to C file identify function
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(ota_data_parser); // this relates to C-file init function
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(ota_data_parser); //this relates to C-file destroy function
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ota_data_parser_start_parser); //a function executed by a start port
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ota_data_parser_parse_data); //a function executed by a start port
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ota_data_parser_abort_parsing); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ota_data_parser)

#endif /* INXWARE_ota_data_parser */
