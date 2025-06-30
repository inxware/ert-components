/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

#include "hal_ota_data_parser.h"
#include "hal_string.h"
#include "target_config.h"

EHS_GLOBAL ehs_sint32 EhsOTAParser_ParseStartInfoPayload(inx_ota_data_parser_state_type* parser_state, const ehs_char* payload, ehs_char* ota_sha256)
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

EHS_GLOBAL ehs_sint32 EhsOTAParser_ParseRawData(inx_ota_data_parser_state_type* parser_state, const ehs_char* rawData, ehs_uint32 rawDataSize, 
                                                ehs_char* otaData, ehs_sint32* otaDataSize, ehs_sint32* otaOffset, ehs_bool* isDone)
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

EHS_GLOBAL ehs_bool EhsOTAParser_CreateNextChunkRequestPayload(inx_ota_data_parser_state_type* parser_state, ehs_char* payload)
{
	if(payload == NULL || parser_state == NULL){
		return EHS_FALSE;
	}
	ehs_bool ret = EHS_TRUE;
	EhsSprintf(payload, "{\"startByte\":%u,\"numBytes\":%u,\"firmware\":\"%s\"}", parser_state->startByte, parser_state->numBytes, parser_state->version);
	return ret;
}