/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/
#ifndef _HAL_URL_H_
#define _HAL_URL_H_
/** @file hal_xml.h
 * Definitions for Common utilities for supporting xml access
 *
 * @author: inx limited
 *
 */

#include "globals.h" // need stuff from a few places in the hal
#include "hal-api.h"
#include "curl/curl.h"

ehs_bool EhsHURLGlobalInit() ;

/*
 * Create Post String (This should be moved to the HAL Layer
 * annoying this needs the curl handle to do a string op...
 *
 * if curl is null the function does not URL encode. Just to hide some stuff in a rubbish API..
 *
 * @note trusted client
 *
 *
 */
#define EHS_NETWORK_SECURITY_NONE 0
#define EHS_NETWORK_SECURITY_SPECIFIC 1
#define EHS_NETWORK_SECURITY_BEST 3

/* This is an arbitrary constant that functions can use if they want but don't need to
 * The post string can be larger
 * */
#define EHS_POST_STRING_LENGTH_MAX 50000U


typedef struct EhsNetworkServerInfo
{
    ehs_char http_username[EHS_STRING_LENGTH_MAX];
    ehs_char http_password[EHS_STRING_LENGTH_MAX];
    ehs_uint32 authentication;
} EhsNetworkServerInfo_t;

/** This is the call back function prototype that can be called at the end of a receiving a chunk
 * This returns the amount of data actually bing written
 * */
typedef ehs_uint32 (*EhsHURLget_write_chunk_CB)(void * userdata,void * bufferdata,ehs_uint32 amount);

/** Data buffer struct that URL data is written into
 * This can accumulate data from the URL reader without pause
 * and can also be configured to pause reading (e.g.when a data threshold has been reached)
 *  by setting the WaitToContinue falg to true.
 */
typedef struct EhsHwrite_data_buffer
{
    FILE *filehandle; /* File handle, if not null data will also be written into here */
    //ehs_char poisontest1[64];
    ehs_uint32 size;   /* size of buffer chunk */
    ehs_uint32 current_position; /* NUmber of bytes downloaded */
    ehs_uint32 nDutyPause_us; /* delay that the function shoiuld have before accepting more data */
    ehs_bool bWaitToContinue; /* wait before continuing - this is set by the user for the mode */
    ehs_bool bWait; /* semaphore to wait before continuing */
    ehs_bool bOverflowed; /* set if data is larger than the remaing buffer size */
    ehs_bool bAbort; /* flag set by client to signal aborting the request */
    EhsHURLget_write_chunk_CB   fpChunkDoneCallbackFunction; /* function called if not null when a chunk has been downloaded. */
    //ehs_char poisontest6[64];
    void *   vChunkDoneCallbackFunctionContext; /* context of the above function call */
    //ehs_char poisontest2[64];;
    ehs_char * buffer; /* buffer for data chunk data */
    //ehs_char poisontest3[64];
} EhsH_URLwrite_data_bufferType;

/** \brief Remove URI illegal charcters -
 *
 *  \warning not robust for preencoded URIs. bestusing on path only part of URI and not query strings
 */
ehs_bool EhsHUrlPathEscape(ehs_char * szUrlOut, const ehs_char * szUrlIn);

/**
 * \brief Extracts the file name from the end of a URL (Or pathname) containing https:. https:, ftp:. ftps:
 * @todo this can be moved to the HAL
 */
ehs_bool EhsCopyFileNameFromURL(ehs_char * szFile,const ehs_char * szURL) ;

/**
 * \brief returns a pointer to the path and any querystring part of the URL (i.e. after first / in path) from a URL
 * and copies the host only part into szHost
 */
ehs_char* EhsGetHostAndPathFromURL(ehs_char * szHost, const ehs_char * szUrl);

/*
 * Create Post String (This should be moved to the HAL Layer
 */
ehs_bool EhsHCreateQueryString(CURL * curl,ehs_char * current_querystring, ehs_char * tag,
                               const ehs_char* value, ehs_uint32 max_length);

/* and a version foe dealing with integers
 * don't need the curl handle here because we don't need to URL encode integers
 * */
ehs_bool EhsHCreateQueryStringNum( ehs_char * current_post, ehs_char * tag,
                                   const ehs_uint32 value, ehs_uint32 max_length);


EHS_GLOBAL RuntimePathType xdirectory_type;

/* Security and authentication configuration */
ehs_bool EhsHSetUpClientTlsCertificate(CURL *curl, RuntimePathType directory_type, ehs_char * cert_path,ehs_char * key_path, ehs_char * pPassphrase);
ehs_bool EhsHSetUpCaTlsCertificate(CURL *curl, RuntimePathType location, ehs_char * cert_path);
ehs_bool EhsHSetUpLocalProxy(CURL *curl);
ehs_bool EhsHSetUpServerSecurity(CURL *curl,EhsNetworkServerInfo_t * server_info);

/* Returns a structure in which data can be downloaded using LIBCURL'swrite function */
/* Returns NULL if any allocations fail */

EhsH_URLwrite_data_bufferType * EhsHURLmake_write_data_buffer(ehs_uint32 size);

ehs_bool EhsHURLfree_write_data_buffer(EhsH_URLwrite_data_bufferType * temp);

/* Allows the same buffer to be re-used */
ehs_bool EhsHURLreset_write_data_buffer(EhsH_URLwrite_data_bufferType * temp);

/* Just returns the data buffer */
ehs_char* EhsHURLget_write_data_buffer(EhsH_URLwrite_data_bufferType * temp);
//static size_t Urlget_write_data(void *buffer, size_t size, size_t nmemb,

/* Set the buffering mode. Also allows release of a blocked read by unsetting the wait flag */
void EhsHURLset_write_data_buffer_waiting_mode(EhsH_URLwrite_data_bufferType * temp, ehs_bool bWaitToContinue);
/* clear the sempahore to get more data */
void EhsHURLset_write_data_buffer_continue(EhsH_URLwrite_data_bufferType * temp);

/* CALLBACK FUNCTION that writes to our data buffer
 *
 * This leaves the string null terminates at each chunk of data
 * */
static size_t EhsHURLget_write_data(ehs_char *ptr, size_t size, size_t nmemb,ehs_char *userdata);

/*
 * This does the config to get a call-back that writes to memory
 * This creates the buffer - call once & call free when finished.
 * returns a pointer to the buffer struct so it can be destroyed when finished with
 */
EhsH_URLwrite_data_bufferType * EhsHConfigCallback(CURL * curl, ehs_uint32 data_size, ehs_uint32 chunk_pause_time);


/* Public Functions */

/* Allow user's call back functions and context to be called from the libcurl call back */
void  EhsHAddChunkEndCallback(EhsH_URLwrite_data_bufferType *data_struct, EhsHURLget_write_chunk_CB _fpChunkDoneCallbackFunction, void * _fpChunkDoneCallbackFunction_data );

/* Does All generic initialisation, including URL handle
 *
 * This needs to be run once before a request to configure the call to the prevailing conditions
 *
 * */
EhsH_URLwrite_data_bufferType * EhsHDoAllGenericConfig (CURL * curl,EhsNetworkServerInfo_t * server_info, ehs_uint32 data_size, ehs_uint32 chunk_pause_time,ehs_uint64 transfertimeoue);//,ehs_uint64 connecttimeout);

/*
 * Function to do a http post
 *
 * This configures the dynamic parameters and does the post
 * it returns the http error number or 0 fro a complete fail
 * and the data pointer
 *
 * @todo the return data (double pointer) should be remove from here. The initial seta ll should instead assign a static pointer to point at the buffer..
 */
ehs_bool EhsHURLConfigPostGet(CURL *curl,EhsH_URLwrite_data_bufferType * buffer_struct,const ehs_char* URL, const ehs_char* post_data,ehs_bool bPost);
long EhsHURLdoRequest(CURL *curl);

#endif
