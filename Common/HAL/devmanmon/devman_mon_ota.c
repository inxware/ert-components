
#define EHSL_MODULE_ID EHSH_LOG_MODULE_HAL_NETWORK

#include "hal-api.h"
#include "devman_mon_ota.h"
#include "hal_ota_data_parser.h"
#include "hal_ota.h"
#include "hal_file.h"
#include "hal_logger.h"
#include "target_process.h"

inx_ota_data_parser_state_type EhsOtaDevmanMonDataParser = { 0 };

// raw OTA data members
ehs_char g_sha256[DEVMAN_MON_OTA_SHA256_BUFF_SIZE] = { 0 };
ehs_char g_nextPayload[EHS_STRING_LENGTH_MAX] = { 0 };

// parsed OTA data memebers
ehs_char g_parsedData[EHS_STRING_LENGTH_MAX] = { 0 };
ehs_sint32 g_nParsedDataSize = 0;
ehs_sint32 g_nParsedDataOffset = 0;

// OTA parsing is done
volatile ehs_bool g_isDone = EHS_FALSE;
// gets set to true when a parsed data chunk is ready to be written
volatile ehs_bool g_bChunkReady = EHS_FALSE;

// callback used for publishing mqtt request for a next OTA data chunk
EhsOtaDevmanMonNextCallback fnNextCallback = NULL;

// called when ota is aborted
void EhsOtaOnAborted(){
    printf("Devman mon OTA Aborted ! \n");
    EhsOtaDevmanMonDataParser.bStarted = EHS_FALSE;
}


ehs_bool EhsOtaDevmanMonSupportInit(EhsOtaDevmanMonNextCallback callback)
{
    if(callback == NULL){
        return EHS_FALSE;
    }

    fnNextCallback = callback;
    g_isDone = EHS_FALSE;
    g_bChunkReady = EHS_FALSE;
    EhsOtaDevmanMonDataParser.version[0] = '\0';
    EhsOtaDevmanMonDataParser.crc16 = 0;
    EhsOtaDevmanMonDataParser.bStarted = EHS_FALSE;
    EhsOtaDevmanMonDataParser.startByte = 0;
    EhsOtaDevmanMonDataParser.numBytes = DEVMAN_MON_OTA_CHUNK_SIZE;
    EhsOtaDevmanMonDataParser.hasSha265 = EHS_TRUE; // @TODO - this may need to be conditional for nxp (using crc32)
    thOTA_on_abort_callback(EhsOtaOnAborted);

    return EHS_TRUE;
}

ehs_bool EhsOtaDevmanMonStart(const ehs_char* infoData, ehs_uint32 infoDataSize)
{
    if(fnNextCallback == NULL || EhsOtaDevmanMonDataParser.bStarted == EHS_TRUE || infoDataSize == 0){
        return EHS_FALSE;    
    }

    g_isDone = EHS_FALSE;
    g_bChunkReady = EHS_FALSE;

    ehs_sint32 nError = EhsOTAParser_ParseStartInfoPayload(&EhsOtaDevmanMonDataParser, infoData, g_sha256);

    if(nError == OTA_PARSER_NO_ERROR){
        // After OTA update compleates, we should always start app from 'default' location. Hence, we must make sure that
        // app2run.nfo file with 'temp' is not present.
        if(Ehs_SysRm("app2run.nfo") == EHS_TRUE){
            printf("Removed 'sysdata/app2run.nfo' file \n");
        }
        // start OTA
        printf("Start devman mon OTA \n");
        nError = thOTA_begin(DEVMAN_MON_OTA_PARTITION_ALT, DEVMAN_MON_OTA_PARTITION_NUMBER);
        if(nError == OTA_PARSER_NO_ERROR){
            // request next chunk right after starting
            if(EhsOTAParser_CreateNextChunkRequestPayload(&EhsOtaDevmanMonDataParser, g_nextPayload) == EHS_TRUE){
                fnNextCallback(g_nextPayload);
            }else{
                printf("Failed to request next chunk while devman mon OTA \n");
            }
        }else{
            printf("Failed to start devman mon OTA (error=%d) \n", nError);
        }
        return EHS_TRUE;
    }else{
        printf("Failed to parse start info while devman mon OTA (error=%d) \n", nError);
    }

    return EHS_FALSE;
}

ehs_bool EhsOtaDevmanMonStarted()
{
    return EhsOtaDevmanMonDataParser.bStarted;
}

ehs_bool EhsOtaDevmanMonWrite(const ehs_char* rawData, ehs_uint32 rawDataSize)
{
    if(fnNextCallback == NULL || EhsOtaDevmanMonDataParser.bStarted == EHS_FALSE || g_isDone == EHS_TRUE){
        return EHS_FALSE;    
    }

    ehs_sint32 nError = EhsOTAParser_ParseRawData( &EhsOtaDevmanMonDataParser, rawData, rawDataSize,
                                                    g_parsedData, &g_nParsedDataSize, &g_nParsedDataOffset, &g_isDone );
    if(nError != OTA_PARSER_NO_ERROR){
        printf("Failed to parse raw data while devman mon OTA \n");
        // we don't want to continue with bad data
        if(thOTA_current_state() != TARGET_OTA_IDLE){
            EhsOtaDevmanMonAbort();
        }
        EhsOtaDevmanMonDataParser.bStarted = EHS_FALSE;
        return EHS_FALSE;
    }
    if(g_isDone == EHS_TRUE){
        // notify OTA that writing has completed
        printf("OTA writing done!\n");
        thOTA_end();
    } else {
        // request the next chunk
        g_bChunkReady = EHS_TRUE;
    }
    return EHS_TRUE;
}

ehs_bool EhsOtaDevmanMonAbort()
{
    thOTA_abort();
    return EHS_FALSE;
}

void EhsOtaDevmanMonProcess()
{
    if(EhsOtaDevmanMonDataParser.bStarted){
        // check if OTA chunk is ready
        if(g_bChunkReady == EHS_TRUE) {
            // write parsed OTA data
            ehs_sint32 nError = (ehs_sint32)thOTA_write_passthrough(g_parsedData, g_nParsedDataSize, g_nParsedDataOffset);
            if(nError == OTA_PARSER_NO_ERROR){
                g_bChunkReady = EHS_FALSE;
                // request next chunk right after starting
                if(EhsOTAParser_CreateNextChunkRequestPayload(&EhsOtaDevmanMonDataParser, g_nextPayload) == EHS_TRUE){
                    fnNextCallback(g_nextPayload);
                }else{
                    printf("Failed to request next chunk while devman mon OTA \n");
                }
            }else if(nError == DEVMAN_MON_OTA_WRITE_IN_PROGRESS){
                printf("Devman mon OTA writing data in progress \n");
            }else{
                // @TODO - report this back to devman, abort ?
                printf("Failed to write chunk while devman mon OTA (error=%d) \n", nError);
            }
        }
    }
    if (g_isDone == EHS_TRUE){
        // wait until OTA end request completes
        if(thOTA_current_state() == TARGET_OTA_ENDED){
            // validate OTA and restart device once successful
            printf("Devman mon OTA validating ...\n");
            ehs_bool validated;
            validated = thOTA_checkChecksum(DEVMAN_MON_OTA_PARTITION_ALT, DEVMAN_MON_OTA_PARTITION_NUMBER, g_sha256, EhsStrlen(g_sha256));
            if (validated == EHS_FALSE) {
                printf("Failed to do checksum while devman mon OTA \n");
                goto err;
            }
            validated = thOTA_checkValid(DEVMAN_MON_OTA_PARTITION_ALT, DEVMAN_MON_OTA_PARTITION_NUMBER);
            if (validated == EHS_FALSE) {
                printf("Failed to validate while devman mon OTA \n");
                goto err;
            }
            ehs_bool switched = thOTA_switch(DEVMAN_MON_OTA_PARTITION_ALT, DEVMAN_MON_OTA_PARTITION_NUMBER);
            if (switched == EHS_FALSE) {
                printf("Failed to switch while devman mon OTA \n");
                goto err;
            }
            printf("Devman mon OTA Done! Rebooting ... \n");
            // call reboot
            EhsTargetReboot();
        err:
            thOTA_idle();
            g_isDone = EHS_FALSE;
        }else{
            // @TODO have some sort of timeout
            //if(timeout){
            //    thOTA_idle();
            //    g_isDone = EHS_FALSE;
            //}
        }
    }
}
