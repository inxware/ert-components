/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

/** @file hal_xml.c
 * Definitions for Common utilities for supporting xml access
 *
 * @author: inx limited
 *
 */


#define EHSL_MODULE_ID EHSH_LOG_MODULE_HAL_NETWORK

#include "globals.h"
#include "hal-api.h"

#include "app_data.h" /* Needed for the NewSODL ref flag */

/*** \todo Remove temporary debug */

//#define EHS_DEBUG_CERTS //- also needs EHS_DEBUG_URL enabling
//#define EHS_DEBUG_URL
#ifdef EHS_DEBUG_URL
#define EhsDebugPrintf(...) do{}while(0)
//#define EhsDebugPrintf(...) printf(__VA_ARGS__) // almost sensible general debug
#define _printf(...) printf(__VA_ARGS__) // scriibly debug
#else
#define EhsDebugPrintf(...) do{}while(0)
#define _printf(...) do{}while(0)
#endif


/*****************************************************************************/
/* Included files */

/*****************************************************************************/


/*****************************************************************************/
/* Declare prototypes of local functions */

/*****************************************************************************/
/* Variables defined with file-scope */

/*****************************************************************************/
/* Variables defined with global-scope */

/*****************************************************************************/

/* Switch on some verbosew debugging */
//#define EHS_DEBUG_NETWORKING

/* Function definitions */

/* INternally managed data that applies to all extra-LAN-WAN network connections */

typedef struct EhsHGlobalNetworkConfigStruct
{
    ehs_char proxy_server[EHS_STRING_LENGTH_MAX]; // can contain port with : notation
    ehs_char proxy_username[EHS_STRING_LENGTH_MAX];
    ehs_char proxy_password[EHS_STRING_LENGTH_MAX];
    ehs_uint32 proxy_port; // @todo this is not used yet - :port number in the URL instead
    ehs_bool proxy_required;
} EhsHGlobalNetworkConfigType;

EhsHGlobalNetworkConfigType EhsHGlobalNetworkConfig;

/* recipe for disaster C code */
struct EhsHGlobalNetworkConfigStruct EhsHGlobalNetworkConfig= {"","","",0,EHS_FALSE};



/**
 * \brief Extracts the file name (not whole path) from the end of a URL (Or pathname) containing https:. https:, ftp:. ftps:
 */
ehs_bool EhsCopyFileNameFromURL(ehs_char * szFile,
                                const ehs_char * szUrl)
{
    ehs_sint16 i,j;//,l;
    //ehs_sint16 length2cut=0;
    if (    EhsStrncmp(szUrl,"http:",EhsStrlen("http:"))   == 0 ||
            EhsStrncmp(szUrl,"https:",EhsStrlen("https:")) == 0 ||
            EhsStrncmp(szUrl, "ftp:",EhsStrlen("ftp:"))    == 0  ||
            EhsStrncmp(szUrl, "ftps:",EhsStrlen("ftps:"))  == 0  ||
            EhsStrncmp(szUrl, "__$__",EhsStrlen("__$__"))  == 0  || // this is used by Devman sometimes
            EhsStrncmp(szUrl, "ssh:",EhsStrlen("ssh:"))  == 0)
    {
        /* search backward from the end of the string to identify if we have path info */
        j = 0;
        //l = EhsStrlen(szUrl);
        for (i = EhsStrlen(szUrl); i >= 0; i--)
        {
            j++;
            if (szUrl[i] == '?' || szUrl[i] == '#')  j = 0;  /* Remove any trailing query string */
            else if ((szUrl[i] == '/') || (szUrl[i] == '\\')
                     || (szUrl[i] == ':' /* shouldn't happen */))
            {
                EhsStrncpy(szFile, &szUrl[i + 1], j-1); // we'll drop the slash shall we with i+1?
                szFile[j-1] = '\0';
                return EHS_TRUE;
            }
        }
        EHSH_LOG_WARNING("URL file extraction error");
    }
    else
    {
        //_printf("Didn't extract anything from %s\n",szUrl);
    }
    /* if there is no marker (or slash or colon) found then assume the whole string is a well formed file name */
    EhsStrcpy(szFile, szUrl);
    return EHS_FALSE; // notify that this does not have a full path
}

/** \brief Remove URI illegal charcters -
 *
 *  \warning not robust for preencoded URIs. bestusing on path only part of URI and not query strings
 */
ehs_bool EhsHUrlPathEscape(ehs_char * szUrlOut, const ehs_char * szUrlIn)
{
    ehs_bool copyrest = EHS_FALSE;

    while (szUrlIn && szUrlOut && *szUrlIn != '\0')
    {
        if (copyrest == EHS_FALSE)
        {
            switch (*szUrlIn)
            {
            case ' ':
                *szUrlOut++ = '%';
                *szUrlOut++ = '2';
                *szUrlOut++ = '0';
                break;
            case '%':
                *szUrlOut++ = '%';
                *szUrlOut++ = '2';
                *szUrlOut++ = '5';
                break;
            case '~':
                *szUrlOut++ = '%';
                *szUrlOut++ = '7';
                *szUrlOut++ = 'E';
                break;
            case '?': // we need stop if we find a query and copy everything else as is.
                *szUrlOut++ =*szUrlIn; // copy the ?
                copyrest=EHS_TRUE;
                break;
            default:
                *szUrlOut++ =*szUrlIn;
                break;
            }
        }
        else   // copy the rest
        {
            *szUrlOut++ =*szUrlIn;
        }
        szUrlIn++;
    }
    if (szUrlOut)
    {
        *szUrlOut++ = '\0';
        return EHS_TRUE;
    }
    else
    {
        return EHS_FALSE;
    }
}

/**
 * \brief returns a pointer to the path part of the URL (i.e. after first trailing / after the Domain in a URL) from a URL
 * and copies the host only part into szHost pointer string buffer provided.
 * Todo need to add a variable max string length to these arguements. Currently we don't support any strings larger than max string length.
 * Warning - this requires a http pefix
 */
ehs_char* EhsGetHostAndPathFromURL(ehs_char * szHost, const ehs_char * szUrl)
{
    ehs_sint16 i,j=0;
    ehs_sint16 offset = 0 ;
    ehs_char * szPath = NULL;
    if (szUrl == NULL || szUrl[0] != 'h') {
        EHSH_LOG_ERROR("Url is null or without prefix http");
    }
    //todo replace this with left first search for //
    EhsStrcpy(szHost,szUrl); // default is to use it as we got it if no path is found..
    for (i = 0; i < EhsStrlen(szUrl); i++) {
        if (szUrl[i] == '/') j++; //not trusting strstr
        if (j > 1) {
            offset = i;
            break;
        }
    }
    
    /* search forward to find the beginning of the path*/
    j=0;
    for (i = offset+1; i < EhsStrlen(szUrl); i++)
    {
        //printf("%c",szUrl[i]);
        szHost[j]=szUrl[i];
        j++;
        if ((szUrl[i] == '/') || (szUrl[i] == '\\') ||  (szUrl[i] == '\0') ||  i > (EHS_STRING_LENGTH_MAX-1) ) //find end of domain part
        {
            szPath = (ehs_char*)&szUrl[i+1]; // 
            //EhsStrncpy(szHost, szPath,i); // don't include the /
            szHost[j-1]='\0'; //add EOL where end of
            break;
        }
    }
    return szPath;
}


#define EHS_URL_PROXYSETTINGSPATH "configs/proxy"
/** Read Proxy file formated:
 * URL:port
 *username:password
 */
void EhsHURLSetProxyDataFromFile()
{
    ehs_FILE * file;
    ehs_char szTempString[EHS_STRING_LENGTH_MAX];
    file = Ehs_UserFopen(EHS_URL_PROXYSETTINGSPATH,"r");
    if (file)
    {
        EhsFscanf(file, "%s%d\n", EhsHGlobalNetworkConfig.proxy_server,
                  &EhsHGlobalNetworkConfig.proxy_port);
        EhsFscanf(file, "%s%s", EhsHGlobalNetworkConfig.proxy_username,
                  EhsHGlobalNetworkConfig.proxy_password);
        EhsHGlobalNetworkConfig.proxy_required = EHS_TRUE;
        EhsFclose(file);
    }
    else
    {
        EhsHGlobalNetworkConfig.proxy_required = EHS_FALSE;
        EHSH_LOG_INFO("Could not open proxy file info at %s",EHS_URL_PROXYSETTINGSPATH);
    }
}


/*
 * This should be called by the global ehs instance init
 */
ehs_bool EhsHURLGlobalInit()
{
    EhsHURLSetProxyDataFromFile(); /* Load the proxy info - Needs a reboot unless this is done again somewhere */
    if (curl_global_init(CURL_GLOBAL_DEFAULT/* CURL_GLOBAL_ALL CURL_GLOBAL_NOTHING CURL_GLOBAL_ALL*/))
    {
        return EHS_FALSE;// something wrong
    }
    else
    {
        return EHS_TRUE;
    }
}

/***
 * \brief Create Post String
 *
 * \param [in] curl if the curl curl is NULL the function does not URL encode.
 * \note trusted client
 */

ehs_bool EhsHCreateQueryString(CURL * curl,ehs_char * current_post, ehs_char * tag, const ehs_char* value, ehs_uint32 max_length)
{
#define EHS_POST_SEPERATOR "&"
#define EHS_POST_ASSIGNEMENT "="

    const ehs_char * formatted_postdata = NULL;
    ehs_bool ret = EHS_FALSE;
    ehs_char szNothing[]=""; // in case we have a null calue pointer passed in
    ehs_bool allocatedFormattedPost = EHS_FALSE;

    if (current_post)
    {
        /* first url encode the data */
        if (curl && value)
        {
            formatted_postdata = curl_easy_escape(curl, value, 0); //sometimes we don't want to encode
            allocatedFormattedPost = EHS_TRUE;
        }
        else formatted_postdata=value;
        //formatted_postdata=value;
        if (!value) formatted_postdata = szNothing;

        if ((EhsStrlen(current_post) + EhsStrlen(tag) + EhsStrlen(formatted_postdata)) > max_length)
        {
            ret = EHS_FALSE;
        }
        else     /* OKl-make the string! */
        {
            if (EhsStrlen(current_post) > 0)   // Only if we're not the first
            {
                EhsStrcat(current_post, EHS_POST_SEPERATOR);
            }
            EhsStrcat(current_post, tag);
            EhsStrcat(current_post, EHS_POST_ASSIGNEMENT);
            EhsStrcat(current_post, formatted_postdata);
            ret = EHS_TRUE;
            if (allocatedFormattedPost == EHS_TRUE && curl && formatted_postdata && value)
                curl_free((ehs_char*)formatted_postdata);
        }
    }
    return ret;
}


/***
 * \brief  Post string Version for dealing with adding integer parameters
 */
ehs_bool EhsHCreateQueryStringNum(ehs_char * current_post, ehs_char * tag,const ehs_uint32 value, ehs_uint32 max_length)
{
    ehs_char sZtemp[16]; //big enough for a 32 bit number
    EhsSprintf(sZtemp, "%d", value);
    return EhsHCreateQueryString(NULL,current_post, tag, sZtemp, max_length);
}


/*** \brief Sets an SSL/TLS client certificate for the communication.
 * 			Does not use an ssl engine
 * 	\param [in] curl handle to configure.
 * 	\param [in] directory_type enum for the ehs tree location the path is relevant to.
 *  \param [in] cert_path is the certificate path relative to ehs/
 *  \param [in] key_path is the key path relative to ehs/. If this is NULL it is presumed the the certificate includes the key in PEM format also.
 *  \param [in] pPassphrase - optional passphrase for the certificate (NULL for none)
 */

ehs_bool EhsHSetUpClientTlsCertificate(CURL *curl, RuntimePathType location, ehs_char * cert_path,ehs_char * key_path, ehs_char * pPassphrase)
{
// @TODO - mve these buffer off the stack... one day

    ehs_char full_cert_path[EHS_STRING_LENGTH_MAX];
    ehs_char full_key_path[EHS_STRING_LENGTH_MAX];

    ehs_bool retval = EHS_FALSE;
    ehs_bool keypresent = EHS_FALSE;
    ehs_bool certpresent = EHS_FALSE;

    if ( EhsTF_tryCanonicPath(full_cert_path,location, cert_path, EHS_TRUE) )    // make a canonical path and check it exists.
    {
        certpresent = EhsTF_exists(full_cert_path);

    }

    if (key_path)
    {
        if (EhsTF_tryCanonicPath(full_key_path,location, key_path, EHS_TRUE))
        {
            keypresent  = EhsTF_exists(full_key_path);
        }
    }

    if (certpresent)
    {
        EHSH_LOG_INFO("Setting Certficate");
        curl_easy_setopt(curl, CURLOPT_SSLCERTTYPE, "PEM");
        retval = (CURLE_OK == curl_easy_setopt(curl, CURLOPT_SSLCERT, full_cert_path));
        //HSH_LOG_INFO("Could not set the TLS certificate paths" );
        if(pPassphrase)
            curl_easy_setopt(curl, CURLOPT_KEYPASSWD, pPassphrase);
    }
    if (keypresent)
    {
        EHSH_LOG_INFO("Setting Key");
        /* if using a crypto engine this would be set to "ENG" */
        curl_easy_setopt(curl, CURLOPT_SSLKEYTYPE, "PEM");

        /* set the private key (file or ID in engine) */

        retval  &= (CURLE_OK == curl_easy_setopt(curl, CURLOPT_SSLKEY, full_key_path)); /* note - libcurl may have already found this in the certificate file and may not be necessary in this case */
    }

#ifdef EHS_DEBUG_CERTS
    printf("\nClient Cert path: %s\n",full_cert_path);
    printf("Client Key  path: %s\n",full_key_path);
#endif
    if (retval == EHS_FALSE)
    {
        //EHSH_LOG_ERROR("Could not set the TLS certificate paths=[%s] & [%s]",cert_path, key_path);// this is a false alarm when a combined client cert and key is used.
    }
    return retval;
}


/*** \brief Sets an SSL/TLS client certificate for the communication.
 * 			Does not use an ssl engine
 * 	\param [in] curl handle to configure.
 * 	\param [in] directory_type enum for the ehs tree location the path is relevant to.
 *  \param [in] CAcert_path is the certificate path relative to ehs/
 */

ehs_bool EhsHSetUpCaTlsCertificate(CURL *curl, RuntimePathType location, ehs_char * cert_path)
{

    ehs_char full_cert_path[EHS_STRING_LENGTH_MAX];
    if (EhsTF_tryCanonicPath(full_cert_path,location, cert_path, EHS_TRUE) && EhsTF_exists(full_cert_path) )
    {


        /* set the file with the certs vaildating the server */

//#define DONT_____EHS_NETWORKING_NO_CA_CHECK/#define EHS_TLS_DISABLE_SERVER_AUTH
//#define EHS_TLS_DISABLE_SERVER_AUTH
#ifdef EHS_TLS_DISABLE_SERVER_AUTH

#warning "NOT DOING TLS HOST_AUTHENTICATION"
        //curl_easy_setopt(curl, CURLOPT_CAINFO, full_cert_path);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0); /* for hosts that we can't find certificates */
#else
        /* disconnect if we can't validate server's cert */
        curl_easy_setopt(curl, CURLOPT_CAINFO, full_cert_path);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L); /* 2 means check and fail (not just check) */
#endif
#ifdef EHS_DEBUG_CERTS
        printf("CA Cert path: %s\n",full_cert_path);
#endif
        return EHS_TRUE;
    }
    else
    {
        //EHSH_LOG_ERROR("Could not find CA certificate files");
        // don't warn as this function maybe used to see if there are certificates as well as loading them.
        return EHS_FALSE;
    }

}

/*** \brief Function for all clients to use for server login.
 *  		A central repository for LAN config is then required? todo?
 *  		Parameterised as a component
 *  		If there are passwords set then use them
 *  */
ehs_bool EhsHSetUpServerSecurity(CURL *curl,EhsNetworkServerInfo_t * server_info)
{
    ehs_bool ret=EHS_TRUE;
    CURLcode success;
    ehs_char szTempString[EHS_STRING_LENGTH_MAX];
    if ((EhsStrlen(server_info->http_username) > 0) && (EhsStrlen(server_info->http_password) > 0))
    {
        EhsSprintf(szTempString, "%s:%s", server_info->http_username,server_info->http_password);
        EhsDebugPrintf("szTempString=%s\n",szTempString);
        curl_easy_setopt(curl, CURLOPT_USERPWD, szTempString);
    }
    /* Set up security */
    switch (server_info->authentication)   //@todo implement this in the function block paramaters
    {
    case EHS_NETWORK_SECURITY_SPECIFIC:
        /*	We don't want to do proxy stuff here...
         * if (EhsStrlen(EhsHGlobalNetworkConfig.proxy_server) > 0 ) {
        		success=curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_DIGEST); //@todo
        		if (CURLE_OK != success)
        				EHSH_LOG_ERROR("CURL ERROR: CURLOPT_HTTPAUTH");
        	}
        	else {*/
        success=curl_easy_setopt(curl, CURLOPT_PROXYAUTH, CURLAUTH_DIGEST); // @todo these arguements need to be made specific
        if (CURLE_OK != success)
        {
            EHSH_LOG_ERROR("CURL ERROR: CURLOPT_PROXYAUTH");
            ret=EHS_FALSE;
        }
        //}
        EhsDebugPrintf("hal_url::Set security to digest only with uname %s and password %s\n",server_info->http_username, server_info->http_password);
        break;
    case EHS_NETWORK_SECURITY_BEST:
        success=curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
        if (CURLE_OK != success)
        {
            EHSH_LOG_ERROR("CURL ERROR: CURLOPT_HTTPAUTH");
            ret=EHS_FALSE;
        }
        break;
    case EHS_NETWORK_SECURITY_NONE:
    default:
        //curl_easy_setopt(easyhandle, CURLOPT_HTTPAUTH, XXCURLAUTH_DIGEST);
        ;
    }
    return ret;
}

ehs_bool EhsHSetUpLocalProxy(CURL *curl)
{
    ehs_char szTempString[EHS_STRING_LENGTH_MAX];
    if (EhsHGlobalNetworkConfig.proxy_required)
    {
        // proxy server
        if (EhsHGlobalNetworkConfig.proxy_port > 0)
        {
            EhsSprintf(szTempString, "%s:%d",EhsHGlobalNetworkConfig.proxy_server,EhsHGlobalNetworkConfig.proxy_port);
        }
        else
        {
            EhsStrcpy(szTempString, EhsHGlobalNetworkConfig.proxy_server);
        }
        curl_easy_setopt(curl, CURLOPT_PROXY, szTempString );
        EhsSprintf(szTempString, "%s:%s",EhsHGlobalNetworkConfig.proxy_username,EhsHGlobalNetworkConfig.proxy_password);
        curl_easy_setopt(curl, CURLOPT_PROXYUSERPWD, szTempString);
        curl_easy_setopt(curl, CURLOPT_PROXYTYPE, CURLPROXY_SOCKS4); //@todo need to make this a parameter too
    }
    return EHS_TRUE;
}

/*** \brief Returns a structure in which data can be downloaded using LIBCURL'swrite function
 * \return Returns NULL if any allocations fail
 * */

EhsH_URLwrite_data_bufferType * EhsHURLmake_write_data_buffer(ehs_uint32 size)
{

    EhsH_URLwrite_data_bufferType * temp = EhsHMem_permAlloc(sizeof(EhsH_URLwrite_data_bufferType)); //@todo this should be switchable between app and system memory.
    if (!temp) return NULL;// give up and report with NULL
    if (size)
    {
        temp->buffer=EhsHMem_permAlloc(sizeof(ehs_char)*size);
        if (temp->buffer == NULL)
        {
            EHSH_LOG_ERROR("Could not allocate memory for URL write buffer");
        }
        else
        {
            EhsMemset(temp->buffer,0,sizeof(ehs_char)*size); // to keep valgrind happy
        }
    }
    else temp->buffer=NULL;

    temp->bOverflowed = EHS_FALSE;
    temp->filehandle = NULL;
    temp->current_position = 0;
    temp->size=size;
    temp->bWait=EHS_FALSE; /* clear any waiting buffers flagging */
    temp->bAbort=EHS_FALSE;
    temp->vChunkDoneCallbackFunctionContext=NULL; // default is we don't use this
    temp->fpChunkDoneCallbackFunction=NULL;
    temp->nDutyPause_us=0;
    temp->bWaitToContinue=EHS_FALSE; /* Default we wont pump data out unless asked to */
    if (!temp->buffer && (size))
    {
        EHSH_LOG_ERROR("Could not create URL write data buffer");
        EhsHMem_permFree(temp); /* remove the whole struct if we can't make it all */
        return NULL;
    }
    else
    {
        EhsHURLreset_write_data_buffer(temp);
        return temp;
    }
}

ehs_bool EhsHURLfree_write_data_buffer(EhsH_URLwrite_data_bufferType * temp)
{
    if (!temp) return EHS_FALSE;// give up and report with Error
    if (temp->buffer) EhsHMem_permFree(temp->buffer);
    temp->buffer = NULL;// should not be useful, but just in case...
    EhsHMem_permFree(temp);
    return EHS_TRUE;
}

/**
 * Reset and allows the same buffer to be re-used without changing the buffer mode
 * */
ehs_bool EhsHURLreset_write_data_buffer(EhsH_URLwrite_data_bufferType * temp)
{
    if (!temp) return EHS_FALSE;// give up and report with NULL
    temp->current_position=0;
    //temp->bWaitToContinue=EHS_FALSE; /* leave the mode in tact
    temp->bAbort=EHS_FALSE;
    temp->bWait=EHS_FALSE; /* clear any waiting buffers flagging */
    temp->bOverflowed=EHS_FALSE;
    temp->filehandle=NULL;
    if (temp->size>0)
        temp->buffer[0]='\0';//truncate string to 0 - only if the set size was > 0
    return EHS_TRUE;
}

/** @brief set the buffering mode to chunked or all in one go
 * trust: temp exists
 * */
void EhsHURLset_write_data_buffer_waiting_mode(EhsH_URLwrite_data_bufferType * temp, ehs_bool bWaitToContinue)
{
    if (temp) temp->bWaitToContinue=bWaitToContinue;
}

/* @brief clears the buffer flag to get more data if it is in wait mode */
void EhsHURLset_write_data_buffer_continue(EhsH_URLwrite_data_bufferType * temp)
{
    EhsDebugPrintf("hal_url::Getting More\n");
    if (temp) temp->bWait=EHS_FALSE;
}
/* Just returns the data buffer */
ehs_char* EhsHURLget_write_data_buffer(EhsH_URLwrite_data_bufferType * temp)
{
    EhsDebugPrintf("hal_url::EhsHURLget_write_data_buffer\n");
    if (!temp) return NULL;// give up and report with NULL
    else return temp->buffer;
}

//static size_t Urlget_write_data(void *buffer, size_t size, size_t nmemb,
///		void *userdata)

/** CALLBACK FUNCTION that writes to our data buffer
 *
 * Supports 3 modes of operation:
 * 1) oneshot: download incrementally into a buffer - return a single populated buffer when finished and flagging any overload.
 * 2) download and pump into a user provided call back function -don't store locally.
 * 3) download and save in file (Note this can be in combination with 1 or 2 above also)
 *
 * 1)
 * If the URLwrite_data struct has user callback function (i.e. is non-null) then this function will call this with
 * any data as soon as it arrives from libURL, pumping it out if the call back function returns a lower value written the submitted.
 * It will the then return to allow more to be got from the libcurl caller.
 *
 * 2)
 * If there isn't a user call back function then the data is buffered up to the size of the buffer. If this overflows then
 * this function continues to save to file (if a file is defined) - otherwise return value is less than expected causing the do URL call function
 * to exit with the error.
 *
 * 3) This happens if a file handle pointer is provided in the struct - it can be in combination with 1 or 2 and will cause
 * different buffer overflow behavious in the caller
 *
 * This leaves the string null terminates at each chunk of data, in case it is a string to simplify reading.
 * */
static size_t EhsHURLget_write_data(ehs_char *ptr, size_t size, size_t nmemb,ehs_char *userdata)
{
    EhsH_URLwrite_data_bufferType * datastruct=(EhsH_URLwrite_data_bufferType *)userdata;

    EhsDebugPrintf(">>>> EhsHURLget_write_data (size =%d, nmemb =%d)", size,  nmemb);

    ehs_uint32 i;
    ehs_uint32 index = datastruct->current_position; /* This is used as the pointer where to write data into the buffer */
    ehs_uint32 indexOut=0; /* This is the index of data that is being written out incrementally - active when not in oneshot (no callback)*/
    //ehs_bool send_in_chunks=EHS_FALSE;
    //ehs_sint32 amount_total;
    ehs_sint32 amount = size * nmemb;
    ehs_sint32 amount_received = amount;
    ehs_sint32 amount_to_report = amount;//size * nmemb;
    ehs_bool one_shot=(datastruct->fpChunkDoneCallbackFunction == NULL) && (datastruct->buffer != NULL); /* identify if we should try and increment data into a single buffer */

    ehs_uint32 msLoops = 1; // number of throttle loops + 1 - so at least one buffer check loop is done.
    if (datastruct->nDutyPause_us/1000 > 0) msLoops = datastruct->nDutyPause_us/1000 + 1;
    else if (datastruct->nDutyPause_us != 0) msLoops = 2; // if it's smaller than a ms then do a ms.

    /* Write things to file if we have one */
    /* Append data to file if we have one*/
    if (datastruct->filehandle)
    {
        int d,f;
        //d =	EhsFwrite;
#ifndef USE_WRITE
        d = fwrite( ptr, amount,1,datastruct->filehandle);//) { //; /* Write the data to disk in one block */
        //d = fwrite( ptr, 1, amount,datastruct->filehandle);//) { //; /* Write the data to disk in one block */
#else
        f = fileno(datastruct->filehandle);
        d = write(f,ptr,amount);
#endif
        //_printf("FD=[%x:%d]",datastruct->filehandle,fileno(datastruct->filehandle));
        if ( d < 1 )  //amount) {
        {
            perror("********************************************** Write() X"); //we'll ignore this and continue  fordata outut with a warning
            //datastruct->
            //while (1) {}
            amount_to_report = 0;
        }
        //EhsFwrite( ptr,1,amount,datastruct->filehandle); /* Write the data to disk in one block */
        _printf("[%d,%d]",d,amount);
    }
    if (amount_to_report > 0)
    {
        if (one_shot)   /* copy data to internal buffer and keep tabs on the buffer pointer */
        {
            EhsDebugPrintf(">>>> EhsHURLget_write_data ONESHOT");
            /* Check if we are overlflowing a buffer and identify the amount of data we ca read in amount*/
            if ((amount + index) >= datastruct->size)
            {
                amount = (index - datastruct->size); /* returning mounting different to sent will cause crl to stop */
                datastruct->bOverflowed = EHS_TRUE;
                //EHSH_LOG_WARNING
                EhsDebugPrintf("hal_url::File reader buffer overflow -non-fatal trying to ad %s\n", ptr);
            }
            /*Append the data into our buffer if we have one...*/
            if (datastruct->buffer && amount > 0)   /* if we have a buffer add to this - if there is room left */
            {
                EhsMemcpy(&(datastruct->buffer[index]), ptr, amount);
                datastruct->current_position = index + amount;
                datastruct->buffer[index + amount] = '\0'; // terminate if pre-gunged - this might be a string
                EhsDebugPrintf("hal_url::Added %s to make %s\n", ptr,datastruct->buffer);
            }
        }
        else if (datastruct->fpChunkDoneCallbackFunction != NULL) /* write the data straight out (in chunks if necessary) and then exit */
        {
            /*  push the data out bit by bit if we are not in oneshot mode */
            //amount_total = amount;
            EhsDebugPrintf("fpChunkDoneCallbackFunction NOT NULL!!!!!");
            while (amount > 0)   /* Loop while we still have data to send */
            {
                if (datastruct->bWaitToContinue) datastruct->bWait=EHS_TRUE; /* reset the wait flag if in wait mode */
                indexOut +=	datastruct->fpChunkDoneCallbackFunction(datastruct->vChunkDoneCallbackFunctionContext, &ptr[indexOut], amount); /* write out the data - report how much has been written*/
                amount = amount_received - indexOut;
                /* If there's more we need to wait */
                datastruct->current_position = 0;/* just in case this is set */
                EhsDebugPrintf(">> bWaitToContinue =%d, bWait=%d",datastruct->bWaitToContinue,datastruct->bWait);
                //todo - this breaks things - what was it for? to remove permenantly if (amount <= 0 ) break;/* all data outputted - don't hang around for the next laggy bit */
                EhsDebugPrintf("Got an amount !!!");
                while (datastruct->bWaitToContinue && datastruct->bWait)   /* wait on every send - blocking further data if we are asked to - expect only Wait to change*/
                {
                    EhsDebugPrintf("bWaitToContinue =%d, bWait=%d",datastruct->bWaitToContinue, datastruct->bWait);
                    if (*bNewSodlFlagRef || datastruct->bAbort)   /* check if we are asked to abort */
                    {
                        amount_to_report = 0; //amount_received - indexOut; /* make the caller finish if we haven't written all*/
                        amount = 0; /* make the outer loop exit */
                        EhsDebugPrintf("EXITING WAIT LOOP to abort ");
                        break; /* Allow thread to Exit nicely if we are tearing down the app */
                    }
                    EhsSleep(EHS_TIME_us(1000)); /* friendly to CPU time to pole for ready flag*/
                }
            } /* while we have some more data to push */
        }
        else EhsDebugPrintf("fpChunkDoneCallbackFunction IS NULL!!!!!");
        // for eaither case we may want to wait till a flag is cleared - todo does this next need to be conditional?
        /* Now wait a period of time before getting another chunk from lubcurl*/


        for (i=0; i<msLoops; i++) /* We need to run this once - even if we don't have a delay */
        {

            if (*bNewSodlFlagRef)
            {
                amount_to_report = 0;//amount;
                break; /* Allow thread to Exit nicely if we are tearing down the app */
            }
            if (msLoops > 1 ) EhsSleep(EHS_TIME_us(1000)); /* Pause to slow data rate if ther's more 1ms poling time */
            /*wait on every receive in 1ms chunks - blocking further data if we are asked to */
        }

        EhsDebugPrintf("RETURNED Some Data (size = %d)\n",amount_to_report);
    }
    return amount_to_report;/* always return the amount got as we can write to file if overflowed a buffer */
}

/*
 * This does the config to get a call-back that writes to memory
 * This creates the buffer - call once & call free when finished.
 * returns a pointer to the buffer struct so it can be destroyed when finished with
 * datasize can be 0 if this struct id for data pumping or just file writing
 */

EhsH_URLwrite_data_bufferType * EhsHConfigCallback(CURL * curl, ehs_uint32 data_size, ehs_uint32 chunk_pause_time)
{
    /* Create the buffer */
    CURLcode success;
    //ehs_bool OK=EHS_FALSE;
    EhsH_URLwrite_data_bufferType * data_struct = NULL;
    data_struct = EhsHURLmake_write_data_buffer(data_size); /// todo should add the call back info here
    if (data_struct)
    {
        data_struct->nDutyPause_us = chunk_pause_time;
        success = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,EhsHURLget_write_data);
        if (CURLE_OK != success)
            EHSH_LOG_ERROR("HAL URL: CURLOPT_WRITEFUNCTION option set failed");
        success= curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*) data_struct); //We'll write XML into a buffer in here data directly here
        if (CURLE_OK != success)
            EHSH_LOG_ERROR("HAL URL:  NOT_WRITE_DATA: option set failed");
        //#define EHS_DEBUG_NETWORKING

    }
    return data_struct;// will be null if didn't work
}


/* Set the callback functions */
void  EhsHAddChunkEndCallback(EhsH_URLwrite_data_bufferType *data_struct, EhsHURLget_write_chunk_CB _fpChunkDoneCallbackFunction, void * _fpChunkDoneCallbackFunction_data )
{
    data_struct->fpChunkDoneCallbackFunction=_fpChunkDoneCallbackFunction;
    data_struct->vChunkDoneCallbackFunctionContext=_fpChunkDoneCallbackFunction_data;
}

/* Public Functions */

/*** \brief Does All generic initialisation, including URL handle
 *
 * This needs to be run once before a request to configure the call to the prevailing conditions
 * \param: data size - the size of the data buffer with which to retrieve data
 * \param: chunk_pause_time - delay time to dwell before achowledging a chunk is read to stall the read pipeline. Used for throttling bandwidth
 * */

EhsH_URLwrite_data_bufferType * EhsHDoAllGenericConfig (CURL * curl,EhsNetworkServerInfo_t * server_info, ehs_uint32 data_size, ehs_uint32 chunk_pause_time_us, ehs_uint64 transfertimeout_s)   //, ehs_uint64 connecttimeout_s) {
{
    EhsH_URLwrite_data_bufferType * buffer_struct;

    /* Some system specific configs @todo review the x-platform validity*/
//#define EHS_USE_CURLS_LAME_SIGNAL_AVOIDANCE
    //this is needed apparently
//#define	EHS_DONT_LET_CURL_GENERATE_OR_HANDLE_SIGNALS_AND_REPOND_TO_SIGNALS
#ifdef EHS_DONT_LET_CURL_GENERATE_OR_HANDLE_SIGNALS_AND_REPOND_TO_SIGNALS
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1); /* avoid long jump crash - remove signal handlers @todo may have DNS look up time out problem).
	@todo Consider building libcurl with c-ares support to enable asynchronous DNS lookups*/
    /* Curl will apparently ask the system to ignore SIGNALS if this is not set */
#else
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 0); /* */
#endif
    //if (transfertimeout > 0 ) curl_easy_setopt(curl, CURLOPT_TIMEOUT, transfertimeout);
    //else curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3600); //if not specified default to 1 hour so threads would eventually die off

    if (transfertimeout_s > 0  )
    {
#ifdef EHS_USE_HARSH_TIMEOUTS_WITH_CURL_SIDEEFFECTS
        //curl_easy_setopt(curl,CURLOPT_CONNECTTIMEOUT,connecttimeout_s); // 10 seconds not 300s allowed to establish a connection
        curl_easy_setopt(curl,CURLOPT_TIMEOUT,transfertimeout_s); // 10 seconds not 300s allowed to establish a connection
#else
        //curl_easy_setopt(curl,CURLOPT_CONNECTTIMEOUT,0); // standard connection timeout of 300s - presumably this is the default (not infinitte).
        curl_easy_setopt(curl,CURLOPT_LOW_SPEED_LIMIT,1L); // less than 10 bytes in the timeout then bail
        curl_easy_setopt(curl,CURLOPT_LOW_SPEED_TIME,(long)transfertimeout_s); // 10 seconds not 300s allowed to establish a connection
        _printf("TIMEOUT US is %d\n",transfertimeout_s);
#endif
    }

    /* We will allow following redirects by default */
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1); /* default to follow forward responses */
#ifdef EHS_DEBUG_URL
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(curl, CURLOPT_CERTINFO, 1L);
#endif

#ifdef CURLOPT_REDIR_PROTOCOLS
    /* available on some versions of CURL */
    curl_easy_setopt(curl,CURLOPT_REDIR_PROTOCOLS,CURLPROTO_ALL);
#endif
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (X11; CrOS i686 2268.111.0) AppleWebKit/536.11 (KHTML, like Gecko) Chrome/20.0.1132.57 Safari/536.11");

    /* todo we need to resolve the following: */
    //curl_easy_setopt(curl, CURLOPT_CAPATH,"/some-path-to-an-openssl-hashed-certificate directory");
    EhsHSetUpServerSecurity(curl,server_info);

    // Don't do this here: EhsHSetUpLocalProxy(curl); // this picks up the global values for the device
    buffer_struct=EhsHConfigCallback(curl, data_size,chunk_pause_time_us);
    return buffer_struct;
}

/*  @brief searches for certicates in ./devman/core/certs/<DOMAIN>/.. for certificates to use instead of defaults.

   Extracts the domain from a URL and produced a path to the certifiate (or the default certificate if not found)
   returns:
    0 : if no certificate found (and no default is set which it always has at the moment)
    1  : a default certificate was found
    2  : a particular certificate was found 
    pCertPath is set to the full absolute path to the relevant certificate in the Devman directory.
*/
ehs_uint8 EhsHGetDevmanCaCertificatePath(ehs_char* pCertPath,const ehs_char * pServerUrl) {
    ehs_char szTemp[EHS_STRING_LENGTH_MAX];
    if (pServerUrl != NULL ) {
        EhsStrcpy(szTemp,EHS_DEVMAN_CERTIFICATES_BASE);
        EhsGetHostAndPathFromURL(&szTemp[sizeof(EHS_DEVMAN_CERTIFICATES_BASE)-1], pServerUrl);// get the domain to check the path with the normal try method for Devman directories
        //printf("Looking for ca certs for URL=%s and domain=|%s|\n",pServerUrl,szTemp);
        EhsStrcat(szTemp,EHS_TD_FILES_SEPARATOR_STR EHS_DEVMAN_CA_CERTIFICATE_FILENAME); // addd a trailing slash of course
        if (EhsTF_tryCanonicPath(pCertPath, EHS_RUNTIME_DEVMAN_DIR,szTemp, EHS_FALSE) == EHS_TRUE && EhsTF_exists(pCertPath) > 0) { // check if we have Devman directory for this domain}
            EhsStrcpy(pCertPath,szTemp);
            //printf("Found dedicated ca certs (%s)\n",pCertPath);
            return 2;
        }else {
            if ( EhsTF_tryCanonicPath(pCertPath, EHS_RUNTIME_DEVMAN_DIR,EHS_DEVMAN_CA_CERTIFICATE, EHS_FALSE) == EHS_TRUE && EhsTF_exists(pCertPath) > 0)  // Do we have a default?
            {
                EhsStrcpy(pCertPath,EHS_DEVMAN_CA_CERTIFICATE);
                //printf("Found default ca certs in (%s)\n",pCertPath);
                return 1;
            }
            else {
                EhsStrcpy(pCertPath,EHS_DEVMAN_CA_CERTIFICATE);
                //printf("Not Found any ca certs for (%s)\n",pCertPath);
                return 0; //we'll still return the default path
            }
        }
    }
    else {
        EHSH_LOG_ERROR("URL is null");
    }
    return 0;
}

/*  @brief searches for certicates in ./devman/core/certs/<DOMAIN>/* for certificates to use instead of defaults.

   Extracts the domain from a URL and produced a path to the certifiate (or the default certificate if not found)
   returns:
    0 : if no certificate found (and no default is set which it always has at the moment)
    1  : a default certificate was found
    2  : a particular certificate was found 
    pCertPath is set to the full absolute path to the relevant certificate in the Devman directory.
*/
ehs_uint8 EhsHGetDevmanCombinedClientCertificateKeyPath(ehs_char* pCertPath,const ehs_char * pServerUrl) {
    ehs_char szTemp[EHS_STRING_LENGTH_MAX];
    EhsStrcpy(szTemp,EHS_DEVMAN_CERTIFICATES_BASE);
    EhsGetHostAndPathFromURL(&szTemp[sizeof(EHS_DEVMAN_CERTIFICATES_BASE)-1], pServerUrl);// get the domain to check the path with the normal try method for Devman directories
    //printf("Looking for combined certs for URL=%s and domain=|%s|\n",pServerUrl,szTemp);
    EhsStrcat(szTemp,EHS_TD_FILES_SEPARATOR_STR EHS_DEVMAN_CLIENT_CERTIFICATE_KEY_FILENAME); // addd a trailing slash of course
    if (EhsTF_tryCanonicPath(pCertPath, EHS_RUNTIME_DEVMAN_DIR,szTemp, EHS_FALSE) == EHS_TRUE && EhsTF_exists(pCertPath) > 0) { // check if we have Devman directory for this domain}
            EhsStrcpy(pCertPath,szTemp);
            //printf("Found dedicated combined certs (%s)\n",pCertPath);
            return 2;
    }else {
        if ( EhsTF_tryCanonicPath(pCertPath, EHS_RUNTIME_DEVMAN_DIR,EHS_DEVMAN_CLIENT_CERTIFICATE_KEY, EHS_FALSE) == EHS_TRUE && EhsTF_exists(pCertPath) > 0)  // Do we have a default?
        {
            EhsStrcpy(pCertPath, EHS_DEVMAN_CLIENT_CERTIFICATE_KEY);
            //printf("Found default combined certs in (%s)\n",pCertPath);
            return 1;
        }
        else {
            EhsStrcpy(pCertPath, EHS_DEVMAN_CLIENT_CERTIFICATE_KEY);
            //printf("Not Found any combined certs for (%s)\n",pCertPath);
            return 0; //we'll still return the default path
        }
    }
    return 0;
}

/* WARNING!!! Not tested or probably complete as we don't bother testing ofr client certifcates just the Ca certificates for each server at the moment
    because we may have combined keys and certificate files and it's getting a bit boring!
    TODO2025 - finish this
    Extracts the domain from a URL and produced a path to the certifiate (or the default certificate if not found)
   returns:
    0  : if no certificate found (and no default is set which it always has at the moment)
    1  : a default certificate was found
    2  : a particular certificate was found 

    +10  : if no certificate found (and no default is set which it always has at the moment)
    +11  : a default certificate was found
    +12  : a particular certificate was found 
    pCertPath is set to the full absolute path to the relevant certificate in the Devman directory.
*/
ehs_uint8 EhsHGetDevmanClientCertificateKeyPaths(ehs_char* pCertPath,ehs_char* pKeyPath,const ehs_char * pServerUrl) {
    ehs_char szTemp[EHS_STRING_LENGTH_MAX];
    ehs_uint8 ret = 0;
    EhsStrcpy(szTemp,EHS_DEVMAN_CERTIFICATES_BASE);
    EhsGetHostAndPathFromURL(&szTemp[sizeof(EHS_DEVMAN_CERTIFICATES_BASE)-1], pServerUrl);// get the domain to check the path with the normal try method for Devman directories
    printf("Looking for certs for URL=%s and domain=|%s|\n",pServerUrl,szTemp);
    EhsStrcat(szTemp,EHS_TD_FILES_SEPARATOR_STR EHS_DEVMAN_CLIENT_CERTIFICATE_FILENAME); // addd a trailing slash of course
    if (EhsTF_tryCanonicPath(pCertPath, EHS_RUNTIME_DEVMAN_DIR,szTemp, EHS_FALSE) == EHS_TRUE && EhsTF_exists(pCertPath) > 0) { // check if we have Devman directory for this domain}
            EhsStrcpy(pCertPath,szTemp);
            printf("Found dedicated ca certs (%s)\n",pCertPath);
            ret = 2;
    }else {
        if ( EhsTF_tryCanonicPath(pCertPath, EHS_RUNTIME_DEVMAN_DIR,EHS_DEVMAN_CLIENT_CERTIFICATE, EHS_FALSE) == EHS_TRUE && EhsTF_exists(pCertPath) > 0)  // Do we have a default?
        {
            EhsStrcpy(pCertPath,EHS_DEVMAN_CLIENT_CERTIFICATE);
            printf("Found default ca certs in (%s)\n",pCertPath);
            ret = 1;
        }
        else {
            EhsStrcpy(pCertPath,EHS_DEVMAN_CLIENT_CERTIFICATE);
            printf("Not Found any ca certs for (%s)\n",pCertPath);
            ret = 0; //we'll still return the default path
        }
    }
    /* Now test for the Client Key */
    EhsStrcpy(szTemp,EHS_DEVMAN_CERTIFICATES_BASE);
    EhsGetHostAndPathFromURL(&szTemp[sizeof(EHS_DEVMAN_CERTIFICATES_BASE)-1], pServerUrl);// get the domain to check the path with the normal try method for Devman directories
    printf("(Looking for client key for URL=%s and domain=|%s|\n",pServerUrl,szTemp);
    EhsStrcat(szTemp,EHS_TD_FILES_SEPARATOR_STR EHS_DEVMAN_CLIENT_PRIVATEKEY_FILENAME); // addd a trailing slash of course
    if (EhsTF_tryCanonicPath(pKeyPath, EHS_RUNTIME_DEVMAN_DIR,szTemp, EHS_FALSE) == EHS_TRUE && EhsTF_exists(pKeyPath) > 0) { // check if we have Devman directory for this domain}
            EhsStrcpy(pKeyPath, szTemp);
            printf("Found dedicated client key (%s)\n",pKeyPath);
            //ret += 12;
    }else {
        if ( EhsTF_tryCanonicPath(pKeyPath, EHS_RUNTIME_DEVMAN_DIR,EHS_DEVMAN_CLIENT_PRIVATEKEY, EHS_FALSE) == EHS_TRUE && EhsTF_exists(pKeyPath) > 0)  // Do we have a default?
        {
            EhsStrcpy(pKeyPath, EHS_DEVMAN_CLIENT_PRIVATEKEY);
            printf("Found default client key in (%s)\n",pKeyPath);
            ret += 10;
        }
        else {
            EhsStrcpy(pKeyPath, EHS_DEVMAN_CLIENT_PRIVATEKEY);
            printf("Not Found any client key for (%s)\n",pKeyPath);
            ret += 11; //we'll still return the default path
        }
    }
    return ret;
}

/**
 * Function to set up an http post or get, adding query data to the URI if not null
 * If this is post it uses curl's post string stuff, otherwise it concats a URL encoded query
 * string into a get request URI (with a ? added for you)
 *
 * This configures the dynamic parameters and does the post or get
 * it returns the http error number or 0 for a complete fail
 * and the data pointer
 *
 * @input curl is the libcurl handle
 * @input URL is the URL excluding the path
 * @post_data is the post data to send - this does not get URL encoded in here
 * @bUsePost changes the HTTP header between from get (FALSE) to post (TRUE)
 *
 * @todo remove the buffer_struct varibale it is not used
 * @todo the return data (double pointer) should be remove from here. The initial seta ll should instead assign a static pointer to point at the buffer..
 */

ehs_bool EhsHURLConfigPostGet(CURL *curl,EhsH_URLwrite_data_bufferType * buffer_struct, const ehs_char* URL,const ehs_char* szQueryString, ehs_bool bUsePost)
{

    ehs_bool ret = EHS_FALSE; //default if nothing works
    CURLcode success;
    ehs_char encodedUrl[EHS_STRING_LENGTH_MAX]; // fully sanatised URL

    /* first sort out the pre-query string (i.e. host and path) for any escaping that might be needed */
    if ( URL)
    {
        //szUrlPathQuery = EhsGetHostAndPathFromURL(encodedUrl,URL);
        //if (szUrlPathQuery) {
        EhsHUrlPathEscape(encodedUrl, URL);
        //		EhsStrcat(encodedUrl,szUrlEncodedPathQuery);
        //	}
        //}
        // else the encodedUrl is left as it is.
    }
    else
    {
        return EHS_FALSE;
    }

    if (bUsePost)
    {
        success = curl_easy_setopt(curl, CURLOPT_POST, 1);
        success = curl_easy_setopt(curl, CURLOPT_HTTPGET, 0);
        if (CURLE_OK != success)
        {
            EHSH_LOG_ERROR("CURLOPT_POST failed");
        }
        else     /* handle both explcit post-data arg or look for URI with ? in URL */
        {
            if (szQueryString)
            {
                success = curl_easy_setopt(curl, CURLOPT_URL, encodedUrl);/*@todo todo we need to test each of these successes */
                success = curl_easy_setopt(curl, CURLOPT_POST, 1);//force a post
                //we cant use this on the whole string - only on the data elements apparently: formatted_querydata = curl_easy_escape(curl, szQueryString, 0);
                success = curl_easy_setopt(curl, CURLOPT_POSTFIELDS,szQueryString/*formatted_querydata*/);
                //if (curl && formatted_querydata) curl_free((ehs_char*)formatted_querydata);
                if (CURLE_OK != success)
                {
                    EHSH_LOG_ERROR("CURLOPT_POSTFIELDS failed");
                }
                else
                {
                    ret = EHS_TRUE;
                }
            }
            else
            {
                success = curl_easy_setopt(curl, CURLOPT_URL, encodedUrl);/*@todo Do some testing upstream here */
                success = curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, 0);//overwerite previous
                success = curl_easy_setopt(curl, CURLOPT_POST, 1);//force a post
                //					}
                if (CURLE_OK == success) ret = EHS_TRUE;
                success = curl_easy_setopt(curl, CURLOPT_POST, 1);//force a post
                if (CURLE_OK == success) ret = EHS_TRUE;
            }
        }

    }
    else     /* make a GET URI by concat URL with query string */
    {
        ehs_char szTempString[EHS_STRING_LENGTH_MAX*8]; //TODO:STRINGLENGTH!!! - suggested hack for now
        //success = curl_easy_setopt(curl, CURLOPT_POST, 0);
        success = curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
        EhsStrcpy(szTempString, encodedUrl);
        if (szQueryString && EhsStrlen(szQueryString) )
        {
            EhsStrcat(szTempString, "?");
            //formatted_querydata = curl_easy_escape(curl, szQueryString, 0);
            EhsStrcat(szTempString, szQueryString/*formatted_querydata*/);
            //if (curl && formatted_querydata) curl_free((ehs_char*)formatted_querydata);
        }
        success = curl_easy_setopt(curl, CURLOPT_URL, szTempString);/*@todo Do some testing here */

        if (CURLE_OK != success)
        {
            EHSH_LOG_ERROR("CURLOPT_URL failed for URL=[%x] post=%s",(unsigned int) curl, URL);
            ret = EHS_FALSE;
        }
        else
        {
            ret = EHS_TRUE;
        }
    }
    return ret;
}

/**
 * @brief Function to do a http post
 * This configures the dynamic parameters and does the post
 * it returns the http error number or 0 fro a complete fail
 * and the data pointer
 *
 * returns 0 if perform could not start or the HTTP code if the request occured
 *
 * @todo the return data (double pointer) should be remove from here. The initial seta ll should instead assign a static pointer to point at the buffer..
 */
#ifdef INX_HAL_URL_EXTRA_ERROR_DIAG
char EhsCurlErrorBuffer[CURL_ERROR_SIZE];
#endif

long EhsHURLdoRequest(CURL *curl)
{
    long ret32 = -999L; //default if nothing works
#ifdef INX_HAL_URL_EXTRA_ERROR_DIAG
    EhsCurlErrorBuffer[0]= "\0";
#endif

    CURLcode success = !CURLE_OK;
    if (curl)
    {
#ifdef INX_HAL_URL_EXTRA_ERROR_DIAG
        //EhsDebugPrintf("DoingEHSURLdoRequest()"); //todo make these conditional on debug logging switched on
        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, EhsCurlErrorBuffer);
#endif

        success = curl_easy_perform(curl);
        if(CURLE_OPERATION_TIMEDOUT == success)
        {
            _printf("TIMEDOUT!!!!!!!!\n");
        }
        if (CURLE_OK == success)
        {
            success = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &ret32); //we call this as we would have at least read the server file and need to check it there.
            if (CURLE_OK != success)
            {
                ret32 = -1 * success;
            }
            //		*data = EhsHURLget_write_data_buffer(buffer_struct);
            //we don't remove the local file if the server file has gone.
        }
        else
        {
            ret32 = -1 * success;
#ifdef INX_HAL_URL_EXTRA_ERROR_DIAG
            //_printf("DoingEHSURLdoRequest returned FAIL = %d\n*************\n%s\n**************\n",success,EhsCurlErrorBuffer);
            EHSH_LOG_ERROR("CURL requestfailed (returned %d) : %s",success,EhsCurlErrorBuffer);
#endif
        }
        return ret32;
    }
    else
    {
        _printf("Trying to do a curl without a valid object\n");
        EHSH_LOG_ERROR("Trying to do a curl without a valid object");
        return -2L;
    }
}
