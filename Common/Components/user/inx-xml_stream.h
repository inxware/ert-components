#ifndef INXWARE_xml_stream_parser
#define INXWARE_xml_stream_parser
#include "inx-component.h"

/*****************************************************
 * Parsers XML as a stream generating events for the XML chunks it sees.
 *****************************************************/
#define INXWARE_FB_xml_stream_parser "xml_stream_parser"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(xml_stream_parser); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(xml_stream_parser); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(xml_stream_parser); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(xml_stream_parser_eos); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(xml_stream_parser_read); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(xml_stream_parser_parse); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(xml_stream_parser_streamFinished); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(xml_stream_parser)

#endif /* INXWARE_xml_stream_parser */
