/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file hal_ota_data_parser.h
 *
 * @author: inx limited
 *
 */

#ifndef EHS_OTA_DATA_PARSER_H
#define EHS_OTA_DATA_PARSER_H

/*****************************************************************************/
/* Included files */
#include "globals.h"

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

typedef struct inx_ota_data_parser_state
{
	ehs_bool hasSha265;
	ehs_sint32 crc16;
	ehs_bool bStarted;
	ehs_uint32 startByte;
	ehs_uint32 numBytes;
	ehs_char version[VERSION_MAX_LENGTH];
} inx_ota_data_parser_state_type;


ehs_sint32 EhsOTAParser_ParseStartInfoPayload(inx_ota_data_parser_state_type* parser_state, const ehs_char* payload, ehs_char* ota_sha256);

ehs_sint32 EhsOTAParser_ParseRawData(inx_ota_data_parser_state_type* parser_state, const ehs_char* rawData, ehs_uint32 rawDataSize, 
                                                ehs_char* otaData, ehs_sint32* otaDataSize, ehs_sint32* otaOffset, volatile ehs_bool* isDone);

ehs_bool EhsOTAParser_CreateNextChunkRequestPayload(inx_ota_data_parser_state_type* parser_state, ehs_char* payload);


#endif /* EHS_OTA_DATA_PARSER_H */