//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-ota_data_parser.h"
//ICB HEADER MACRO END -- DO NOT ALTER

#define OTA_DEFAULT_CHUNK_SIZE 128
#define OTA_CHUNK_HEADER_SIZE 6

#define VERSION_MAX_LENGTH 128

/* error code for ota parser */
#define OTA_PARSER_NO_ERROR 0
#define OTA_PARSER_START_DATA_INVALID 1
#define OTA_PARSER_FAILED_TO_REQUEST_NEXT_CHUNK 2
#define OTA_PARSER_OTA_DATA_PORTS_ARE_NOT_CONNECTED 3
#define OTA_PARSER_FAILED_PARSING_DATA 4
#define OTA_PARSER_INVALID_OTA_CHUNK_HEADER 5
#define OTA_PARSER_INVALID_OTA_CHUNK_SIZE 6
#define OTA_PARSER_SHA256_PORT_NOT_CONNECTED 7
#define OTA_PARSER_SHA256_INVALID 8

//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_ota_data_parser_state
{
	ehs_char* version[VERSION_MAX_LENGTH];
	ehs_bool hasSha265;
	ehs_sint32 crc16;
	ehs_bool bStarted;
	ehs_uint32 startByte;
	ehs_uint32 numBytes;
} inx_ota_data_parser_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(ota_data_parser)
EHS_FB_FUNCTION_ENTRY("start_parser", 0x01, ota_data_parser_start_parser)
EHS_FB_FUNCTION_ENTRY("parse_data", 0x02, ota_data_parser_parse_data)
EHS_FB_FUNCTION_ENTRY("abort_parsing", 0x03, ota_data_parser_abort_parsing)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_ota_data_parser_ARG_start_parser_ota_info 1
#define INX_ota_data_parser_ARG_start_parser_sha256 1
#define INX_ota_data_parser_ARG_start_parser_error_no 2
#define INX_ota_data_parser_ARG_start_parser_next_payload 3
#define INX_ota_data_parser_ARG_start_parser_ota_started 1
#define INX_ota_data_parser_ARG_start_parser_error 2
#define INX_ota_data_parser_ARG_start_parser_next_chunk 3
#define INX_ota_data_parser_ARG_parse_data_raw_data 1
#define INX_ota_data_parser_ARG_parse_data_raw_data_size 2
#define INX_ota_data_parser_ARG_parse_data_parsed_data 1
#define INX_ota_data_parser_ARG_parse_data_parsed_size 2
#define INX_ota_data_parser_ARG_parse_data_data_offset 3
#define INX_ota_data_parser_ARG_parse_data_error_no 4
#define INX_ota_data_parser_ARG_parse_data_next_payload 5
#define INX_ota_data_parser_ARG_parse_data_done 1
#define INX_ota_data_parser_ARG_parse_data_data_ready 2
#define INX_ota_data_parser_ARG_parse_data_error 3
#define INX_ota_data_parser_ARG_parse_data_next_chunk 4
#define INX_ota_data_parser_ARG_abort_parsing_aborted 1
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER
//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
/* Parameters */
/* Create some macros for the default parameters */
//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(ota_data_parser)
{
/* Uncomment the following if you need to parse the parameters to calculate memory required */
/*
	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,""); */
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_ota_data_parser_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(ota_data_parser)
{
	ehs_bool bRet = EHS_TRUE; /* assume success */
	//this is the reference to the object data for this instance of the function block
	inx_ota_data_parser_state_type* inx_ota_data_parser_state = (inx_ota_data_parser_state_type*)EHS_FB_INIT_CONTEXT;
	/* read the initialisation parameters */
	ehs_bool hasSha265 = EHS_TRUE;
	ehs_uint32 dataChunckSize = OTA_DEFAULT_CHUNK_SIZE;
	const char* pParams = EHS_FB_INIT_PARAMETERS;
	pParams = (pParams != NULL) ? EhsGetUint8FromString(&hasSha265, pParams) : NULL;
	pParams = (pParams != NULL) ? EhsGetUint32FromString(&dataChunckSize, pParams) : NULL;
	inx_ota_data_parser_state->version[0] = '\0';
	inx_ota_data_parser_state->crc16 = 0;
	inx_ota_data_parser_state->bStarted = EHS_FALSE;
	inx_ota_data_parser_state->startByte = 0;
	inx_ota_data_parser_state->numBytes = dataChunckSize;
	inx_ota_data_parser_state->hasSha265 = hasSha265;

	return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(ota_data_parser)
{
	//inx_ota_data_parser_state_type *inx_ota_data_parser_state = (inx_ota_data_parser_state_type*)EHS_FB_DESTROY_CONTEXT;
	//Your code below here
	return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE

ehs_sint32 parse_ota_start_info_payload(const ehs_char* payload, ehs_char* ota_sha256, inx_ota_data_parser_state_type* parser_state)
{
	parser_state->crc16 = 0;
	if(ota_sha256 != NULL){
		ota_sha256[0] = '\0';
	}
	if(parser_state->hasSha265 == EHS_TRUE){
		if(ota_sha256 != NULL){
			EhsSscanf(payload, "sha256=%64s,version=%s", (char*)ota_sha256, (char*)parser_state->version);
			//printf("sha256=%s,version=%s", ota_sha256, parser_state->version);
			if(strlen(ota_sha256) < 64){ // sha256 must have 64 bytes
				return OTA_PARSER_SHA256_INVALID;
			}
		}else{
			return OTA_PARSER_SHA256_PORT_NOT_CONNECTED;
		}
	}else{
		EhsSscanf(payload, "crc=%d,version=%s", &(parser_state->crc16), (char*)parser_state->version);
		//printf("crc=%d,version=%s",parser_state->crc16, parser_state->version);
	}
	parser_state->startByte = 0;
	parser_state->bStarted = EHS_TRUE;
	
	return OTA_PARSER_NO_ERROR;
}

ehs_bool create_next_chunk_request_payload(ehs_char* payload, inx_ota_data_parser_state_type* parser_state)
{
	if(payload == NULL || parser_state == NULL){
		return EHS_FALSE;
	}
	ehs_bool ret = EHS_TRUE;
	EhsSprintf(payload, "{\"startByte\":%u,\"numBytes\":%u,\"firmware\":\"%s\"}", parser_state->startByte, parser_state->numBytes, parser_state->version);
	return ret;
}

ehs_sint32 parse_ota_raw_data(const ehs_char* rawData, ehs_uint32 rawDataSize, ehs_char* otaData, 
								ehs_sint32* otaDataSize, ehs_sint32* otaOffset,
								inx_ota_data_parser_state_type* parser_state,
								ehs_bool* isDone)
{
	if(rawData == NULL || otaData == NULL || otaDataSize == NULL ||
		otaOffset == NULL || parser_state == NULL || isDone == NULL)
	{
		return OTA_PARSER_FAILED_PARSING_DATA;
	}

	*isDone = EHS_FALSE;

	if(rawDataSize < OTA_CHUNK_HEADER_SIZE){
		return OTA_PARSER_INVALID_OTA_CHUNK_HEADER;
	}
	// extract the header from the raw data
	//ehs_uint32 startByte=*rawData;
	ehs_uint16* pPublishedDataLength=(ehs_uint16*)&rawData[4];
	ehs_uint16 publishedDataLength=*pPublishedDataLength;
	ehs_uint16 calculatedDataLength=rawDataSize-OTA_CHUNK_HEADER_SIZE;
	if(publishedDataLength!=calculatedDataLength){
		printf("OTA error = dataLength != calculatedDataLength\n");
		return OTA_PARSER_INVALID_OTA_CHUNK_HEADER;
	}
	// check if we have reached the end of update
	if(publishedDataLength == 0){
		*isDone = EHS_TRUE;
		parser_state->bStarted = EHS_FALSE;
		printf("OTA update done!\n");
		return OTA_PARSER_NO_ERROR;
	}
	// extract the firmware data from the raw chunk
	ehs_sint32 chunkDataSize=(ehs_sint32) publishedDataLength; /* Using signed in case we want to support -1 as a not known flag */
	if(chunkDataSize > EHS_STRING_LENGTH_MAX){
		return OTA_PARSER_INVALID_OTA_CHUNK_SIZE;
	}
	// copy ota data chunck to the ota data port buffer
	EhsMemcpy(&otaData[0], &rawData[OTA_CHUNK_HEADER_SIZE], chunkDataSize);
	// set ota data size and offset
	*otaDataSize = chunkDataSize;
	*otaOffset = parser_state->startByte;
	// set size for the next request
	parser_state->startByte += chunkDataSize;

	return OTA_PARSER_NO_ERROR;
}

//ICB FUNCTION start_parser MACRO START -- DO NOT ALTER
/**
 * Definition of ota_data_parser_start_parser.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ota_data_parser_start_parser)
{
	inx_ota_data_parser_state_type* inx_ota_data_parser_state = (inx_ota_data_parser_state_type*)EHS_FB_RUN_CONTEXT;

	ehs_sint32 nError = OTA_PARSER_NO_ERROR;

	if(inx_ota_data_parser_state->bStarted == EHS_TRUE){
		// prevent disturbing ota if it has already begun
		// insead add a port to gracefully abort it
		return;
	}

	/* parse ota start info */
	if (EHS_FB_IN_CONNECTED_API2(INX_ota_data_parser_ARG_start_parser_ota_info)){
		const ehs_char* ota_info = EHS_FB_IN_S_API2(INX_ota_data_parser_ARG_start_parser_ota_info);
		ehs_char* ota_sha256 = NULL;
		if (EHS_FB_OUT_CONNECTED_API2(INX_ota_data_parser_ARG_start_parser_sha256)){
			ota_sha256 = (ehs_char*)EHS_FB_OUT_S_API2(INX_ota_data_parser_ARG_start_parser_sha256);
		}
		nError = parse_ota_start_info_payload(ota_info, ota_sha256, inx_ota_data_parser_state);
	}

	if(nError != OTA_PARSER_NO_ERROR){
		/* something went wrong, report error */
		if (EHS_FB_OUT_CONNECTED_API2(INX_ota_data_parser_ARG_start_parser_error_no)){
			EHS_FB_OUT_I_API2(INX_ota_data_parser_ARG_start_parser_error_no) = nError;
		}
		EHS_FB_FINISH(INX_ota_data_parser_ARG_start_parser_error);
	}else{
		/* notify that ota parsing has started */
		EHS_FB_FINISH(INX_ota_data_parser_ARG_start_parser_ota_started);
		/* request next ota chunk */
		if (EHS_FB_OUT_CONNECTED_API2(INX_ota_data_parser_ARG_start_parser_next_payload)){
			ehs_char* next_payload = EHS_FB_OUT_S_API2(INX_ota_data_parser_ARG_start_parser_next_payload);
			if(create_next_chunk_request_payload(next_payload, inx_ota_data_parser_state) == EHS_TRUE){
				EHS_FB_FINISH(INX_ota_data_parser_ARG_start_parser_next_chunk);
			}else{
				/* report error */
				if (EHS_FB_OUT_CONNECTED_API2(INX_ota_data_parser_ARG_start_parser_error_no)){
					EHS_FB_OUT_I_API2(INX_ota_data_parser_ARG_start_parser_error_no) = OTA_PARSER_FAILED_TO_REQUEST_NEXT_CHUNK;
				}
				EHS_FB_FINISH(INX_ota_data_parser_ARG_start_parser_error);
			}
		}
	}
}//ICB FUNCTION start_parser MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION parse_data MACRO START -- DO NOT ALTER
/**
 * Definition of ota_data_parser_parse_data.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ota_data_parser_parse_data)
{
	inx_ota_data_parser_state_type* inx_ota_data_parser_state = (inx_ota_data_parser_state_type*)EHS_FB_RUN_CONTEXT;

	ehs_sint32 nError = OTA_PARSER_NO_ERROR;

	if(inx_ota_data_parser_state->bStarted == EHS_FALSE){
		/* attempting to parse data before ota info is obtained */
		printf("cannot parse data ota data before ota start request!\n");
		return;
	}

	/* make sure all ota data ports are connected */
	if (!EHS_FB_IN_CONNECTED_API2(INX_ota_data_parser_ARG_parse_data_raw_data) ||
		!EHS_FB_IN_CONNECTED_API2(INX_ota_data_parser_ARG_parse_data_raw_data_size) ||
		!EHS_FB_OUT_CONNECTED_API2(INX_ota_data_parser_ARG_parse_data_parsed_data) ||
		!EHS_FB_OUT_CONNECTED_API2(INX_ota_data_parser_ARG_parse_data_parsed_size) ||
		!EHS_FB_OUT_CONNECTED_API2(INX_ota_data_parser_ARG_parse_data_data_offset))
	{
		nError = OTA_PARSER_OTA_DATA_PORTS_ARE_NOT_CONNECTED;
	}

	if(nError != OTA_PARSER_NO_ERROR){
		/* report error */
		if (EHS_FB_OUT_CONNECTED_API2(INX_ota_data_parser_ARG_parse_data_error_no)){
			EHS_FB_OUT_I_API2(INX_ota_data_parser_ARG_parse_data_error_no) = nError;
		}
		EHS_FB_FINISH(INX_ota_data_parser_ARG_parse_data_error);
		return;
	}

	ehs_bool isDone = EHS_FALSE;

	nError = parse_ota_raw_data(EHS_FB_IN_S_API2(INX_ota_data_parser_ARG_parse_data_raw_data), 
	                    		EHS_FB_IN_I_API2(INX_ota_data_parser_ARG_parse_data_raw_data_size),
								EHS_FB_OUT_S_API2(INX_ota_data_parser_ARG_parse_data_parsed_data),
								&EHS_FB_OUT_I_API2(INX_ota_data_parser_ARG_parse_data_parsed_size),
								&EHS_FB_OUT_I_API2(INX_ota_data_parser_ARG_parse_data_data_offset),
								inx_ota_data_parser_state, &isDone);

	if(nError != OTA_PARSER_NO_ERROR){
		/* report error */
		if (EHS_FB_OUT_CONNECTED_API2(INX_ota_data_parser_ARG_parse_data_error_no)){
			EHS_FB_OUT_I_API2(INX_ota_data_parser_ARG_parse_data_error_no) = nError;
		}
		EHS_FB_FINISH(INX_ota_data_parser_ARG_parse_data_error);
		return;
	}
	if(isDone == EHS_TRUE){
		EHS_FB_FINISH(INX_ota_data_parser_ARG_parse_data_done);
	}else{
		/* notify that ota data is parsed */
		EHS_FB_FINISH(INX_ota_data_parser_ARG_parse_data_data_ready);

		/* request next ota chunk until we're done */
		if (EHS_FB_OUT_CONNECTED_API2(INX_ota_data_parser_ARG_parse_data_next_payload) && isDone == EHS_FALSE){
			ehs_char* next_payload = EHS_FB_OUT_S_API2(INX_ota_data_parser_ARG_parse_data_next_payload);
			if(create_next_chunk_request_payload(next_payload, inx_ota_data_parser_state) == EHS_TRUE){
				EHS_FB_FINISH(INX_ota_data_parser_ARG_parse_data_next_chunk);
			}else{
				/* report error */
				if (EHS_FB_OUT_CONNECTED_API2(INX_ota_data_parser_ARG_parse_data_error_no)){
					EHS_FB_OUT_I_API2(INX_ota_data_parser_ARG_parse_data_error_no) = OTA_PARSER_FAILED_TO_REQUEST_NEXT_CHUNK;
				}
				EHS_FB_FINISH(INX_ota_data_parser_ARG_parse_data_error);
			}
		}
	}
	
}//ICB FUNCTION parse_data MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION abort_parsing MACRO START -- DO NOT ALTER
/**
 * Definition of ota_data_parser_abort_parsing.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ota_data_parser_abort_parsing)
{
	inx_ota_data_parser_state_type* inx_ota_data_parser_state = (inx_ota_data_parser_state_type*)EHS_FB_RUN_CONTEXT;

	if(inx_ota_data_parser_state != NULL){
		inx_ota_data_parser_state->bStarted = EHS_FALSE;
	}
	
	EHS_FB_FINISH(INX_ota_data_parser_ARG_abort_parsing_aborted);
}//ICB FUNCTION abort_parsing MACRO END -- DO NOT ALTER THIS LINE
