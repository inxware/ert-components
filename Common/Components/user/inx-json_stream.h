/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

#ifndef INXWARE_json_stream_parser
#define INXWARE_json_stream_parser
#include "inx-component.h"

/*****************************************************
 * Processes a text stream and generates events related to JSON parsing.
 *****************************************************/

#define INXWARE_FB_NAME_json_stream_parser "json_stream_parser"
#define INXWARE_FB_ID_json_stream_parser 0xDCB6

/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(json_stream_parser); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(json_stream_parser); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(json_stream_parser); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(json_stream_parser_read); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(json_stream_parser_parse); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(json_stream_parser_eos); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(json_stream_parser)

#endif /* INXWARE_json_stream_parser */
