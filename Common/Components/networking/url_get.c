/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/


//#define EHSL_MODULE_ID EHSH_LOG_MODULE_HAL_NETWORK


#include "globals.h"
#include "ehs_fb_types.h"
#include "url_get.h"
#include "setCompletes.h"
#include "hal_string.h"
#include "app_data.h"
//#include "callback_queue.h"
#include "hal-api.h"


#define URL_GET_DEBUG
#ifdef URL_GET_DEBUG
#define _printf(...) printf(__VA_ARGS__)
#else
#define _printf(...) do{}while(0)
#endif

#define EHS_URLGET_COOKIEFILE_BASE "configs/cookies/"

EHS_FB_FUNCTIONS_START(UrlGet)

EHS_FB_FUNCTION_ENTRY("getpost", 0x01, UrlGet_get)

EHS_FB_FUNCTION_ENTRY("abort", 0x02, UrlGet_abort)

EHS_FB_FUNCTION_ENTRY("savecookies", 0x03, UrlGet_savecookies)

EHS_FB_FUNCTION_ENTRY("clearcookies", 0x04, UrlGet_clearcookies)

EHS_FB_FUNCTION_ENTRY("getmoredata", 0x05, UrlGet_getmore)
EHS_FB_FUNCTIONS_END

/* port identifiers for getpost */
/* input data */
#define EHS_FB_URLGET_URL 			(0)
#define EHS_FB_URLGET_POST_DATA 	(1)
#define EHS_FB_URLGET_LOCAL_PATH	(2)
#define EHS_FB_URLGET_URLDIRDUMP	(3)
#define EHS_FB_URLGET_PRIORITY		(4)
#define EHS_FB_URLGET_USECOOKIES    (5)
#define EHS_FB_URLGET_SETHEADER_STRING (6)
#define EHS_FB_URLGET_SETCREDENTIALS_USER (7)
#define EHS_FB_URLGET_SETCREDENTIALS_PASS (8)
#define EHS_FB_URLGET_SETSSL_CERTIFCATE   (9)
#define EHS_FB_URLGET_SETSSL_PASSWORD 	  (10)
#define EHS_FB_URLGET_USE_PROXY_FLAG      (11)
#define EHS_FB_URLGET_USE_POST 		      (12)
#define EHS_FB_URLGET_USE_OVERWRITE       (13)

/* Output data */
#define EHS_FB_URLGET_RESPONSE		(0) //#14+1
#define EHS_FB_URLGET_HEADERS		(1) //#14+2
#define EHS_FB_URLGET_ENUM			(3) //#14+3
#define EHS_FB_URLGET_ESTRING		(4) //#14+4
#define EHS_FB_URLGET_RESPONSE_SIZE (5) //19
#define EHS_FB_URLGET_FINALURL      (2) //19


/* Completes */
#define EHS_FB_URLGET_GETTING 		(1) //starts at 20
#define EHS_FB_URLGET_DATAREADY		(2)
#define EHS_FB_URLGET_DATACOMPLETE  (3)
#define EHS_FB_URLGET_ERROR			(4)

/* and the other config ports...*/

#define EHS_FB_URLGET_SENDCOOKIES_DONE  (1)

#define EHS_FB_URLGET_SAVECOOKIES_DONE (1)

#define EHS_FB_URLGET_CLEARCOOKIES_DONE (1)

#define EHS_FB_URLGET_ABORT 	(1)


/* port identifiers for abort function */
#define EHS_FB_URLGET_ABORTED 0

//#define CURL_GLOBAL_HANDLE
#ifdef CURL_GLOBAL_HANDLE
static CURL * _global_curl=NULL;
#define PURLLGET_CURL _global_curl

#else

#define PURLLGET_CURL pUrlGet->curl

#endif

struct EhsFbUrlGetStruct   //Note make all ints and bools ehs_uint32 to avoid scanf ("%d") problem overwriting the next entry...!
{
    ehs_bool bBusy; /* This is set when a thread starts and is unset when the thread is complete (We don't do multiple gets yet).*/

    ehs_bool bPost; /*Make the http a POST not GETrequest.*/

    ehs_uint16 nCertificateLocation; /* ssl certificate base location  0:Operating System default,  1:user, 2 : app, 3:devman, 4:system, 5:not set */
    /* Not currently used.
    	 * ehs_char szHeaderReceived[EHS_STRING_LENGTH_MAX];
    	 */
    ehs_bool bUseproxy; /* falg for the http requests to use the system proxy seettings */
    ehs_bool bKeepPath; /* set to Keep the path of theServer locally. */
    ehs_bool bOverwriteAll; /* Overwrite - even if the file is same time stamp as the server */
    ehs_bool bWriteToFile; /*write the data to file, even if we have a data output connection */
    ehs_bool bDataPortConnected; /* Flag set if data is to be pumped out.- i.e. requiring the file status to be ignored */
    ehs_bool bSendCookies;
    ehs_uint32 nPathLevelsToIgnore; /* ignore some of the paths in the URI when making the local tree */ //todo implement this.
    ehs_uint32 nConnectTmeout; /* timeout if we are longer than this (seconds)*/
    ehs_uint32 nPriority; /* This is actually the duty gap that  can be inserted to avoid network flooding */
    EhsH_URLwrite_data_bufferType *write_data_buffer_struct; /* this contains the buffer information so that other functions can prod it for more data */
    CURL *curl; /* THis allows session cookies to be used for this one block*/
    /*	EhsCallbackQueueEntryType xCallbackOut; [ internal system variable!! */

    ehs_char szHeaderToSend[EHS_STRING_LENGTH_MAX]; // comma separated list of header lines (libcurl format) //TODO:STRINGLENGTH!
    ehs_char szHeadersReceived[EHS_STRING_LENGTH_MAX]; // comma separated list of header lines (libcurl format) //TODO:STRINGLENGTH!

    ehs_char szSslCertificatePath[EHS_STRING_LENGTH_MAX]; /* SSL client certificate filename (in secure directory)*/ //TODO:STRINGLENGTH!
    ehs_char szSslCertificatePassword[EHS_STRING_LENGTH_MAX]; /* SSL key filename if it has one */ //TODO:STRINGLENGTH!
    ehs_char szCaSslCertificate[EHS_STRING_LENGTH_MAX];   /* SSL host certificate file (overrides system ones) */ //TODO:STRINGLENGTH!
    ehs_char szURL[EHS_MAX_URL_LENGTH]; //todo check this legnth rubbish out one day!
    ehs_char szPostData[EHS_STRING_LENGTH_MAX*8]; //TODO:STRINGLENGTH!!! This can be pretty big no? and may need to as big as a dynamic size port
    ehs_char szSavePath[EHS_STRING_LENGTH_MAX];
    ehs_char szActualSavePath[EHS_STRING_LENGTH_MAX]; // this is constructed from the above and URL information.
    ehs_char szUname[EHS_STRING_LENGTH_MAX];
    ehs_char szPasswd[EHS_STRING_LENGTH_MAX];
    ehs_char szPuname[EHS_STRING_LENGTH_MAX]; /*Proxy username and password from a config file */
    ehs_char szPpasswd[EHS_STRING_LENGTH_MAX]; /*Proxy username and password from a config file */

};

//EhsCallbackQueueType EhsUrlGetCallback;

/**
 * Define the identify function.
 */
EHS_FB_IDENTIFY_FUNCTION(UrlGet)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(struct EhsFbUrlGetStruct);
}

/**
 * Initialise the curl getter.
 */

EHS_FB_INIT_FUNCTION(UrlGet)
{
    struct EhsFbUrlGetStruct* pUrlGet =
        (struct EhsFbUrlGetStruct *) EHS_FB_INIT_CONTEXT;
    ehs_char szBuff[EHS_STRING_LENGTH_MAX] = { '\0' }; /* buffer for data to the server     */
    const ehs_char* pBuff = &szBuff[0];
    static ehs_bool libcurl_inited = EHS_FALSE;
    //safe initialisation;
    pUrlGet->bBusy=EHS_FALSE;
    pUrlGet->bPost = EHS_FALSE;
    pUrlGet->bWriteToFile = EHS_FALSE;
    pUrlGet->bOverwriteAll=EHS_FALSE;
    pUrlGet->bKeepPath = EHS_FALSE;
    pUrlGet->bSendCookies=EHS_TRUE;
    pUrlGet->bDataPortConnected=EHS_FALSE;
    //pUrlGet->bKeepPath=EHS_FALSE;
    pUrlGet->nConnectTmeout = 60;
    pUrlGet->nPathLevelsToIgnore = 0;
    pUrlGet->nPriority = 0; /* fiull speed */
    pUrlGet->curl= NULL;

    EhsStrcpy(pUrlGet->szPostData, "");
    EhsStrcpy(pUrlGet->szSavePath, "");
    EhsStrcpy(pUrlGet->szURL, "");
    EhsStrcpy(pUrlGet->szUname, "");
    EhsStrcpy(pUrlGet->szPasswd, "");
    EhsStrcpy(pUrlGet->szPuname, "");
    EhsStrcpy(pUrlGet->szPpasswd, "");
    EhsStrcpy(pUrlGet->szHeaderToSend, "");
    EhsStrcpy(pUrlGet->szHeadersReceived, "");
    EhsSscanf(EHS_FB_INIT_PARAMETERS, "%s%s%s%hhd%hhd%d%d%d%hhd%s%s%s%s%s%hd%hhd%hhd",
              pUrlGet->szURL, pUrlGet->szPostData, pUrlGet->szSavePath,
              &pUrlGet->bPost, &pUrlGet->bKeepPath,
              &pUrlGet->nPathLevelsToIgnore, &pUrlGet->nPriority,
              &pUrlGet->nConnectTmeout, &pUrlGet->bSendCookies,
              pUrlGet->szUname, pUrlGet->szPasswd,
              pUrlGet->szSslCertificatePath, pUrlGet->szSslCertificatePassword, pUrlGet->szCaSslCertificate,&pUrlGet->nCertificateLocation,
              &pUrlGet->bUseproxy, &pUrlGet->bOverwriteAll); //@todo may need to make temp vars of int * type and convert as scanf wont do this properly.
    /* remove any NULL strings*/
    if (EhsStrcmp(pUrlGet->szPostData, "NULL") == 0)
        pUrlGet->szPostData[0] = '\0';
    if (EhsStrcmp(pUrlGet->szURL, "NULL") == 0)
        pUrlGet->szURL[0] = '\0';
    if (EhsStrcmp(pUrlGet->szSavePath, "NULL") == 0)
        pUrlGet->szSavePath[0] = '\0';
    if (EhsStrcmp(pUrlGet->szUname, "NULL") == 0)
        pUrlGet->szUname[0] = '\0';
    if (EhsStrcmp(pUrlGet->szPasswd, "NULL") == 0)
        pUrlGet->szPasswd[0] = '\0';
    if (EhsStrcmp(pUrlGet->szPuname, "NULL") == 0)
        pUrlGet->szPuname[0] = '\0';
    if (EhsStrcmp(pUrlGet->szPpasswd, "NULL") == 0)
        pUrlGet->szPpasswd[0] = '\0';
    if (EhsStrcmp(pUrlGet->szSslCertificatePassword, "NULL") == 0)
        pUrlGet->szSslCertificatePassword[0] = '\0';
    if (EhsStrcmp(pUrlGet->szSslCertificatePath, "NULL") == 0)
        pUrlGet->szSslCertificatePath[0] = '\0';
    if (EhsStrcmp(pUrlGet->szCaSslCertificate, "NULL") == 0)
        pUrlGet->szCaSslCertificate[0] = '\0';
    if (EhsStrlen(pUrlGet->szSavePath))
        pUrlGet->bWriteToFile = EHS_TRUE;
    /* set all null strings to ""  */
//#define EHS_DEBUG_URLGET
#ifdef EHS_DEBUG_URLGET
           "pUrlGet->szPostData(%x)=[%s]\n "
           "pUrlGet->szSavePath=[%s]\n "
           "pUrlGet->bKeepPath=[%d]\n   "
           "pUrlGet->bUseproxy=[%d]\n   "
           "pUrlGet->nPathLevelsToIgnore=[%d]\n "
           "pUrlGet->nPriority=[%d]\n  "
           "pUrlGet->nConnectTmeout=[%d]\n", /*"pUrlGet->bPost=%d\n "*/
           pUrlGet->szURL, pUrlGet->szURL, pUrlGet->szPostData, pUrlGet->szPostData,
           pUrlGet->szSavePath,
           /*pUrlGet->bPost, todo out this back in the idf parms?*/
           pUrlGet->bKeepPath, pUrlGet->bUseproxy, pUrlGet->nPathLevelsToIgnore,
           pUrlGet->nPriority, pUrlGet->nConnectTmeout;
#endif
    pUrlGet->write_data_buffer_struct=NULL;
    //EHSH_LOG_INFO("PBB url_get init finished");
    return EHS_TRUE; /* initialisation always succeeds */
}


/*
 * This is the thread function that reads data - this does not get a copy of the state data (like it should if we wanted parallel downloads??) - so will fail when busy.
 * It returns the amount of data actually wrtten so that the caller can call again with the rest...
 * @todo This needs refactoring into using hal_url functions ... some design yet to do on how the proxy settings are configure across applications (persistence model).
 * @todo should pass in a copy of the args so that we are rentrant without a critical section.
 */

/* callback function called by the HAL URL get callback function to fire this context's ports */


/* The following function must fit the prototype typedef EhsHURLget_write_chunk_CB */
/*typedef ehs_uint32 (*EhsHURLget_write_chunk_CB)(void * userdata,void * bufferdata,ehs_uint32 amount);*/

ehs_uint32    FireChunkOutputs(void *context, void * buffer,ehs_uint32 size)
{
    ehs_uint32 size_written= (size>EHS_STRING_LENGTH_MAX)?(EHS_STRING_LENGTH_MAX-1):size; /* use maximum of size submitted and output port size */
    EhsFunctionInstanceDataType* pFIdata = (EhsFunctionInstanceDataType*)context;;

    char* temp=EHS_FB_OUT_S(EHS_FB_URLGET_RESPONSE);
    if (temp)
    {
        memcpy(temp, buffer, size_written);
        temp[size_written]='\0'; /* OK -  EHS_STRING_LENGTH_MAX is 1 less than actual buffer size */
    }
    else
    {
        size_written = 0;
    }
    /*output the amount of data (in case data isn't treated a string */
    EHS_FB_OUT_I(EHS_FB_URLGET_RESPONSE_SIZE)=size_written;
    EHS_FB_FINISH(EHS_FB_URLGET_DATAREADY); //@todo change to  a new complete port
    return size_written;
}



/* Function to make a nice domain name from a URL */
/* @todo This should do the canonical path bit too */

ehs_bool EhsURL_EncodeURLtoCookieJarFSName(ehs_char* out,ehs_char * szURL)
{
    ehs_char* temp;
    ehs_uint16 i;
    ehs_bool more=EHS_TRUE;

    EhsStrcpy(out,EHS_URLGET_COOKIEFILE_BASE);
    out+=EhsStrlen(EHS_URLGET_COOKIEFILE_BASE);/*append to this*/
    if (!szURL)
        return EHS_FALSE; /* don't event try */

    if ( (temp = EhsStrstr(szURL, "http")) ) // move temp up to the first http if we have one @todo what if utl has no http buta apramters does?
        temp = temp + EhsStrlen("http");
    else temp =szURL;

    if (EhsStrncmp(temp, "://",EhsStrlen("://")) == 0 )
        temp = temp + EhsStrlen("://");
    else
    {
        if (EhsStrncmp(temp, "s://",EhsStrlen("s://")) == 0)
            temp = temp + EhsStrlen("s://");
        else
        {
            if (EhsStrncmp(temp, "s:",EhsStrlen("s:")) == 0)
                temp = temp + EhsStrlen("s:");
            else
            {
                if (EhsStrncmp(temp, ":",EhsStrlen(":")) == 0)
                    temp = temp + EhsStrlen(":");
                else
                    temp = szURL; /* assume this starts at the beginning then */
            }
        }
    }
    /* temp is pointing somewhere sensible */
    for (i=0; (i<EHS_STRING_LENGTH_MAX) && more; i++)
    {
        switch (temp[i])
        {
        case ':': /* shouldn't get this, but just in case ... */
            out[i] = '_';
            break;
        case '.':
            out[i]='_';
            break;
        case '\0': /* end of inout before any slashes */
            out[i] = '\0';
            more=EHS_FALSE;
            break;
        case '/': /* hit the end of the Domain. */
            out[i]='\0';
            more=EHS_FALSE;
            break;
        default:
            out[i]=temp[i];
        }
    }
    return EHS_TRUE;
}

#ifdef EHS_ENABLE_CURL_VERBOSE
int ehs_curl_debug_callback(CURL *handle, curl_infotype type, char *data, size_t size, void *userptr) {
    (void)handle; // unused
    (void)userptr; // unused

    switch (type) {
        case CURLINFO_TEXT:
            EHSH_LOG_ERROR("CURL [INFO] %.*s", (int)size, data);
            break;
        case CURLINFO_HEADER_IN:
            EHSH_LOG_ERROR("CURL [HEADER_IN] %.*s", (int)size, data);
            break;
        case CURLINFO_HEADER_OUT:
            EHSH_LOG_ERROR("CURL [HEADER_OUT] %.*s", (int)size, data);
            break;
        case CURLINFO_DATA_IN:
            // EHSH_LOG_ERROR("CURL [DATA_IN] %.*s", (int)size, data);
            break;
        case CURLINFO_DATA_OUT:
          //  EHSH_LOG_ERROR("CURL [DATA_OUT] %.*s", (int)size, data);
            break;
        default: /* Do nothing */ ;
    }

    return 0;
}
#endif

EHS_FB_THREAD_FUNCTION(GetUrl_thread)
{
    ehs_char szTempString[EHS_STRING_LENGTH_MAX]="";
    ehs_char szTempFilePath2[EHS_MAXPATHLENGTH]="";
    //ehs_char *formatted_postdata = NULL;
    ehs_uint32 buffer_size=0;
    ehs_sint32 ret32=-998;
    long remotefiletimestamp = 0;
    ehs_uint64 ret = 100;
    int proxy = EHS_FALSE;
    struct utimbuf new_times;
    ehs_bool bFileexists = EHS_FALSE, bNeed_to_do = EHS_TRUE, bNetworkError = EHS_FALSE; //bWriting_to_file
    ehs_bool request_config_error = EHS_FALSE;
    ehs_char * zTemp = NULL;
    int authentication = EHS_NETWORK_SECURITY_NONE;
    RuntimePathType location;;
    ehs_bool use_ca_certificate = EHS_FALSE;
    ehs_bool use_client_certificate = EHS_FALSE;
    //ehs_FILE * writefile = NULL;
    struct stat statbuf = {};
    EhsTPMutex_lock(EhsTPMutex_UrlGet); // We Need to serialise these
    struct EhsFbUrlGetStruct* pUrlGet = (struct EhsFbUrlGetStruct *) EHS_FB_RUN_CONTEXT;
    EhsNetworkServerInfo_t server_info = { .http_username = "",.http_password = "", .authentication = EHS_NETWORK_SECURITY_BEST };

    CURLcode success;
    success = 99999;
    bNetworkError = EHS_FALSE;

    EhsTPMutex_unlock(EhsTPMutex_UrlGet);
    EHSH_LOG_INFO("========Started Getting path [%s] : URL[%s]",pUrlGet->szActualSavePath, pUrlGet->szURL);
    Ehs_FB_ThreadStarted();
    /* Invariant initialisation - create a read buffer for data also */
    /* make our read buffer no greater than the EHS_STRING_LENGTH_MAX as this is all we can push onto the bus*/
    if (!PURLLGET_CURL)
    {
        PURLLGET_CURL = curl_easy_init();
#ifdef EHS_ENABLE_CURL_VERBOSE
        curl_easy_setopt(PURLLGET_CURL, CURLOPT_DEBUGFUNCTION, ehs_curl_debug_callback);
        curl_easy_setopt(PURLLGET_CURL, CURLOPT_VERBOSE, 1L);
#endif
    }
    /* Setup the cookie files */
    if (PURLLGET_CURL && EhsStrlen(pUrlGet->szURL))   /* and do some extra config. for cookies */
    {
        ehs_char temp[EHS_MAX_URL_LENGTH];
        ehs_char temp2[EHS_MAX_URL_LENGTH];
        //EhsStrcpy(temp2,EHS_URLGET_COOKIEFILE_BASE);
        EhsURL_EncodeURLtoCookieJarFSName(temp2,pUrlGet->szURL);
        if (pUrlGet->bSendCookies && EhsTF_tryCanonicPath(temp,EHS_RUNTIME_USERDATA_DIR, temp2, EHS_TRUE))  //try to create the path whatever
        {
            /* Debug */

            if (curl_easy_setopt(PURLLGET_CURL, CURLOPT_COOKIEFILE, temp) != CURLE_OK)   // todo extend with [app]/[URL].cookies
            {
                EHSH_LOG_INFO("No cookie file for %s found",temp);
            }
            else
            {
                curl_easy_setopt(PURLLGET_CURL, CURLOPT_COOKIEFILE, ""); /* maybe this causes a read */
            }

        }
        //else EHSH_LOG_ERROR("Cookie PATH NOT OK");
    }
    /* We need to decide if we need a buffer - if so only needs to be as big as the output buffer size */
    if (pUrlGet->bDataPortConnected) buffer_size = EHS_STRING_LENGTH_MAX;
    else buffer_size = 0;

    pUrlGet->write_data_buffer_struct = EhsHDoAllGenericConfig(PURLLGET_CURL,&server_info, buffer_size,pUrlGet->nPriority,pUrlGet->nConnectTmeout);/* @todo FB should contai a paramter for transfer timout also.. 2 minute timeouts and 50ms chunk gap */
    if (!pUrlGet->write_data_buffer_struct)
    {
        EHSH_LOG_ERROR("Could not create a buffer");
        goto error; // todo we should handle this with request_config_error so that errr events are asserted
    }
    if (!PURLLGET_CURL)
    {
        EHSH_LOG_ERROR("Error: Could not init CURL - Skipping call");
        goto error;
    }
    /* Progress */
    //	 Set the progress callback by using CURLOPT_PROGRESSFUNCTION. And pass a pointer to a function that matches this prototype:
    // int progress_callback(void *clientp,   double dltotal,   double dlnow,   double ultotal,   double ulnow);


    //curl_easy_setopt(easyhandle, CURLOPT_HTTPPROXYTUNNEL, 1L);

    /*
     * Use client ssl certificate if set.
     * */
    {
        //Start a sub variable scope for rarely used features needed on start
        ehs_char szTempFilePath[EHS_MAXPATHLENGTH]="";
        switch (pUrlGet->nCertificateLocation)
        {
        case 3 : //devman - use the devman paths if no others are provided. provided ones must be located in in ./devman/core/certs
            if (EhsStrlen(pUrlGet->szSslCertificatePath)>0) {
                EhsStrcpy(szTempFilePath,pUrlGet->szSslCertificatePath); 
                use_client_certificate = EHS_TRUE;
            } else {
                //EhsStrcpy(szTempFilePath,EHS_DEVMAN_CLIENT_CERTIFICATE_KEY); // not needed if we have dynamic domain checking
//                printf("------------------------>1\n");
                use_client_certificate = (EhsHGetDevmanCombinedClientCertificateKeyPath(szTempFilePath,pUrlGet->szURL) > 0 );
            }
            if (EhsStrlen(pUrlGet->szCaSslCertificate)>0) {
                EhsStrcpy(szTempFilePath2,pUrlGet->szCaSslCertificate); 
                use_ca_certificate = EHS_TRUE;
            } else {
 //               printf("------------------------>2\n");
                //EhsStrcpy(szTempFilePath2,EHS_DEVMAN_CA_CERTIFICATE);  
                use_ca_certificate = (EhsHGetDevmanCaCertificatePath(szTempFilePath2,pUrlGet->szURL) > 0);
            }            
            break;
        case 0 : // Operating System 
        //todo 2025 - currently most systems only work if the cert bundle we provide in the Devman/core/certs directory are installed.
                use_client_certificate = (EhsHGetDevmanCombinedClientCertificateKeyPath(szTempFilePath,pUrlGet->szURL) > 0 );
            break;
        case 1 : // any user
        case 2 : // this user (private space for app \todo make app-specific private user space.
        case 4 : // system
        case 5 : // os root
            EhsStrcpy(szTempFilePath,pUrlGet->szSslCertificatePath);
            EhsStrcpy(szTempFilePath2,pUrlGet->szCaSslCertificate);
            //use_ca_certificate = EHS_TRUE;
            //use_client_certificate = EHS_TRUE;
            break;
        default:
            break;
        }
        /* Check to see if we have certificates set */
        if (EhsStrlen(pUrlGet->szSslCertificatePath)>0) use_client_certificate = EHS_TRUE;
        if (EhsStrlen(pUrlGet->szCaSslCertificate)>0) use_ca_certificate = EHS_TRUE;

        /* set the location with another switch like above */
        switch (pUrlGet->nCertificateLocation)
        {
        case 3 : //devman
            location = EHS_RUNTIME_DEVMAN_DIR;
            break;
        case 0 : // app // todo this should be devman instead of 3 really as thhis is the FB default.
            location = EHS_RUNTIME_APPDATA_DIR;
            break;
        case 1 : // any user
            location = EHS_RUNTIME_USERDATA_DIR;
            break;
        case 2 : // this user
            EHSH_LOG_ERROR("user/app specific user data not yet supported");///\todo make  app-specific private data areas.
            location = EHS_RUNTIME_USERDATA_DIR;
            break;
        case 4 : // system
            location = EHS_RUNTIME_SYSDATA_DIR;
            break;
        case 5 : // os root
            location = EHS_RUNTIME_OS_ROOT;
            break;

        default:
            location = EHS_RUNTIME_APPDATA_DIR; // no op
            use_ca_certificate = EHS_FALSE;
            use_client_certificate = EHS_FALSE;
            EHSH_LOG_INFO("Could not set certificate root location %d. Ignoring certificates",pUrlGet->nCertificateLocation);
            break;
        }
        if (EhsStrlen(pUrlGet->szSslCertificatePassword) > 0)
        {
            EhsHSetUpClientTlsCertificate(PURLLGET_CURL, location, szTempFilePath, NULL /* combined in PEM */, pUrlGet->szSslCertificatePassword);
            EHSH_LOG_INFO("Setting a cert password");
        }
        else
        {
            EHSH_LOG_ERROR("NOT Setting a cert passwod");
            printf("------------------------>3\n");
                
            EhsHSetUpClientTlsCertificate(PURLLGET_CURL, location, szTempFilePath, NULL /* combined in PEM */, NULL);
        }
        if (use_ca_certificate == EHS_TRUE)   // use custom CA certificates
        {
                EHSH_LOG_INFO("Setting CA custom path to [%s]",szTempFilePath2);
                printf("------------------------%s>4\n",szTempFilePath2);
            
                EhsHSetUpCaTlsCertificate(PURLLGET_CURL, location, szTempFilePath2);
        }
        else
        {    
            EhsHSetUpCaTlsCertificate(PURLLGET_CURL, location, EHS_DEVMAN_CA_CERTIFICATE);
        }
    } // ed of init sub scope
    /* Choose if we add post data - note this might be get data URI also - bad nameing! */
    /* set up the URL and http header */
    if ( (pUrlGet->szPostData != NULL)  && (EhsStrlen(pUrlGet->szPostData) > 0))   /* Require we have a string to pass if it a post */
    {
        EhsHURLConfigPostGet(PURLLGET_CURL,NULL,pUrlGet->szURL, pUrlGet->szPostData,pUrlGet->bPost==EHS_TRUE); /* this implies a post in libcurl */
    }
    else
    {
        EhsHURLConfigPostGet(PURLLGET_CURL,NULL,pUrlGet->szURL, NULL,pUrlGet->bPost==EHS_TRUE);
    }


    if (EhsStrlen(pUrlGet->szHeaderToSend) > 0)
    {
        ehs_char * next2, *next = pUrlGet->szHeaderToSend;
        do
        {
            struct curl_slist *chunk = NULL;
            next2 = EhsStrchr(next,',');
            if (next2)
            {
                EhsStrncpy(szTempString,next,next2-next);
                next=next2+1;
            }
            else
            {
                EhsStrcpy(szTempString,next);
                next = NULL;
            }
            chunk = curl_slist_append(chunk,szTempString);
            /* set our custom set of headers */
            curl_easy_setopt(PURLLGET_CURL, CURLOPT_HTTPHEADER, chunk);

        }
        while (next);
    }
    /* Do some special configuration */
    /* If there are passwords set then use them */
    if ((EhsStrlen(pUrlGet->szUname) > 0))
    {
        //EHSH_LOG_INFO("!!!!!!!!! Setting Crdentials Get username = %s : %s ",pUrlGet->szUname,pUrlGet->szPasswd);
        EhsStrcpy(server_info.http_username, pUrlGet->szUname);
        EhsStrcpy(server_info.http_password, pUrlGet->szPasswd);
        EhsHSetUpServerSecurity(PURLLGET_CURL, &server_info);
    }

    if (pUrlGet->bUseproxy)
    {
        EHSH_LOG_ERROR("Using PROXY as asked %d",pUrlGet->bUseproxy);
        EhsHSetUpLocalProxy(PURLLGET_CURL); //uses global setting from the proxy info server
    }
    if (!pUrlGet->bSendCookies)   // snd some specfic cookies not in our cookie files
    {
        //curl_easy_setopt(PURLLGET_CURL, CURLOPT_COOKIE, ""); /* send blank cookies */
        /* uncomment the following would mean forget session cookies - don't want this necessarilly */
        //curl_easy_setopt(PURLLGET_CURL,CURLOPT_COOKIESESSION,1); /* ignore previous cookie sessions */
        //curl_easy_setopt(PURLLGET_CURL, CURLOPT_COOKIELIST,"ALL"); /* clears all cookies (but enables the cookie session from now on ??? weird) */
    }/* here to force merge decision read next line!!!! */
    /* Use this it's an override for the cookie session. not exclusive with send Cookies which is really don't remember cookies */
    /* todo2024 re-instate this?ss
    if (EhsStrlen(pUrlGet->szCookiesToSend)) {
    	curl_easy_setopt(PURLLGET_CURL, CURLOPT_COOKIE, pUrlGet->szHeaderToSend);
    } else {
    	//curl_easy_setopt(PURLLGET_CURL, CURLOPT_COOKIE, "");
    }
     */
    szTempFilePath2[0]='\0';
    if (pUrlGet->bWriteToFile)   /* Followign code is just for comaring remote and local file times to see if files need getting */
    {

        curl_easy_setopt(PURLLGET_CURL, CURLOPT_FILETIME, 1); // we set this for al cases of potentially writing a file

   
        pUrlGet->write_data_buffer_struct->filehandle=NULL;/* make sure we don't try to write anything for getting a header */

        if (!pUrlGet->bOverwriteAll && !pUrlGet->bDataPortConnected )   /* if only overvwrite new flag is not set or we have to push data out (we don't cache in this case */
        {
            /* Attempt to get the URL header and time stamp in all file case  except clobber mode*/
            curl_easy_setopt(PURLLGET_CURL, CURLOPT_NOBODY, 1); /* get no data just file info from headers required here */
            EhsHURLset_write_data_buffer_waiting_mode(pUrlGet->write_data_buffer_struct, EHS_FALSE); /* make sure this doesn't block the request */
            /* Get the remote file time */
            success = EhsHURLdoRequest(PURLLGET_CURL); /* This is just to get the time info - don't use the write function */
            if (!pUrlGet) goto error; /* HAck in case threads have been closed down */
            if (*bNewSodlFlagRef) goto getyourcoat;
            if (success == 200)   /* success here is the HTTP/FTP return number - could add 220 for SMTP */
            {
                /*  got remote info */
                success = curl_easy_getinfo(PURLLGET_CURL,CURLINFO_FILETIME, &remotefiletimestamp); //returns seconds since 1970.

                if (success == CURLE_OK)
                {
                    /* Only need to check the local file if we are not going to clobber it (i.e. we can get a server file date */
                    //EhsTF_tryCanonicPath(szTempFilePath, EHS_RUNTIME_USERDATA_DIR,pUrlGet->szActualSavePath, EHS_TRUE);
                    bFileexists = Ehs_UserStat(pUrlGet->szActualSavePath, &statbuf); //@todo We want a hal_file version of stat - should have localpath// exemption.
                    /* Only get the heeader if we already have the file to check against */
                    if (bFileexists == EHS_TRUE)   /* file exists*/
                    {
                        if (statbuf.st_size == 0)   //This must be a dogey file so remove it and try again
                        {
                            EHSH_LOG_WARNING("Downloader found a zero byte version of the file - overwriting");
                            bNeed_to_do = EHS_TRUE;
                        }
                        else
                        {
                            if (statbuf.st_mtime == remotefiletimestamp)   // if we have the same time stamp it don't download
                            {
                                bNeed_to_do = EHS_FALSE;
                                EHSH_LOG_INFO("Got [%s] already time-stamps:[%d == %d]",pUrlGet->szActualSavePath,(ehs_sint32)statbuf.st_mtime,(ehs_sint32) remotefiletimestamp);
                            }
                            else
                            {
                                //	EHSH_LOG_INFO("nOT got it already -downloading! [%s] [%d == %d]",pUrlGet->szActualSavePath,(ehs_sint32)statbuf.st_mtime, (ehs_sint32)remotefiletimestamp);
                            }

                        }
                    }
                    else     /*if no local file then we need to do request */
                    {
                        bNeed_to_do = EHS_TRUE;
                        EHSH_LOG_ERROR("5 --");
                    }
                }
                else     // couldn't get a time from header l
                {
                    /* NOTE: Shouldn't set bNeed_to_do = EHS_FALSE here because some servers don't support FILETIME so we still need to get the file */
                    bNeed_to_do = EHS_TRUE; /* get the file of Request was 200 but no time stamp available */
                    EHSH_LOG_WARNING("Not Got remote file info from request. error=%d",success);
                }
            }
            else     /* Coudn't get any response from the server with a body only request */
            {
                EHSH_LOG_INFO("No response from server for header only request (E=%d)",success);
                bNeed_to_do = EHS_TRUE; // We'll try again anyway ...
            }
        }
        else     /* we are in overwrite or memory data mode */
        {
            bNeed_to_do = EHS_TRUE; // We'll try again anyway ...
        }


    }
    else     /* Not writing to file (memory only!) */
    {
        EHSH_LOG_ERROR("9 --");
        bNeed_to_do = EHS_TRUE; // We'll try again anyway ...
    }
    /******************************************************************************/
    /* Done chcking file time stamps and errors : need for request is established
     * Now do the rquest  */
    /******************************************************************************/

    if (bNeed_to_do && request_config_error != EHS_TRUE)
    {
        /* First check if we need to create a file handle for the write to write to */
        if (pUrlGet->bWriteToFile)  //bWriting_to_file) {
        {
            EHSH_LOG_ERROR("Write to file....... ");
            if (EhsStrlen(pUrlGet->szActualSavePath) > 0)
            {
                EHSH_LOG_ERROR("++ 11");

                EhsStrcpy(szTempFilePath2, pUrlGet->szActualSavePath);
                EhsStrcat(szTempFilePath2, ".TMP");

                /* alays save to temp first to avoid clobbering existing files */
                //make a ctmep copy name

#ifdef EHS_MINGW
                ehs_char szCanonicalFilePath[EHS_MAXPATHLENGTH];
                EhsTF_tryCanonicPath(szCanonicalFilePath, EHS_RUNTIME_USERDATA_DIR, szTempFilePath2, EHS_TRUE);
                Ehs_MakePath(szCanonicalFilePath, EHS_TRUE);
                pUrlGet->write_data_buffer_struct->filehandle = Ehs_UserFopen(szTempFilePath2, "wb");
#else
                /* todo2024 Need to document this. Seems to try the file lots of different ways in case of a fopen issue?*/
                pUrlGet->write_data_buffer_struct->filehandle = Ehs_UserFopen(szTempFilePath2, "w");
                if ( pUrlGet->write_data_buffer_struct->filehandle &&
                        ( fcntl(fileno(pUrlGet->write_data_buffer_struct->filehandle), F_GETFD) == -1 || errno == EBADF ) )
                {
                    EHSH_LOG_ERROR(" Download file descriptor error. trying again : fd= %d",fileno(pUrlGet->write_data_buffer_struct->filehandle));
                    FILE * f = pUrlGet->write_data_buffer_struct->filehandle;
                    pUrlGet->write_data_buffer_struct->filehandle = Ehs_UserFopen(szTempFilePath2, "w");
                    EhsFclose(f);
                    if (pUrlGet->write_data_buffer_struct->filehandle &&
                           ( fcntl(fileno(pUrlGet->write_data_buffer_struct->filehandle), F_GETFD) == -1 || errno == EBADF ) )
                    {
                        EHSH_LOG_ERROR(" Download file descriptor error - retry unseccessful:  fd = %d",fileno(pUrlGet->write_data_buffer_struct->filehandle));
                        request_config_error = EHS_TRUE; //- todo we could still try to recover this?
                        ret32 = -1;
                    }
                }
                else
                {
                    /* Try an actual command and retry also */
                    fseek(pUrlGet->write_data_buffer_struct->filehandle,0,SEEK_SET); // * check the file */
                    if (fcntl(fileno(pUrlGet->write_data_buffer_struct->filehandle), F_GETFD) == -1 || errno == EBADF )
                    {
                        FILE *  f;
                        f = pUrlGet->write_data_buffer_struct->filehandle;
                        pUrlGet->write_data_buffer_struct->filehandle = Ehs_UserFopen(szTempFilePath2, "w"); // try again and hope for the best
                        EhsFclose(f);
                    }
                }
#endif
                if (!pUrlGet->write_data_buffer_struct->filehandle)
                {
                    EHSH_LOG_ERROR("Couldn't write file %s",pUrlGet->szActualSavePath);
                    ret32 = -1;
                    request_config_error = EHS_TRUE;
                }

            }
            else     // if only the filename is wrong we should have already tried this we will write the data to the output port
            {
                EHSH_LOG_ERROR("No File path given");
                pUrlGet->write_data_buffer_struct->filehandle = NULL;
            }
        }
        else pUrlGet->write_data_buffer_struct->filehandle = NULL;

        //curl_easy_setopt(PURLLGET_CURL, CURLOPT_FILETIME, 0); - Leave FILETIME request in - we always want to know the file time to time stamp the file - if we didn't before.
        curl_easy_setopt(PURLLGET_CURL, CURLOPT_NOBODY, 0); /* now we do want the body */

        if (pUrlGet->bDataPortConnected)   /* if we have a data connection we will download in chunks */
        {
            EhsHURLset_write_data_buffer_waiting_mode(pUrlGet->write_data_buffer_struct, EHS_TRUE); /* make the call back function pump data out is it is gotten */
            EhsHAddChunkEndCallback(pUrlGet->write_data_buffer_struct,FireChunkOutputs, (void*) pFIdata); /* add the call back we want that will fire the data got port */
        }

        ret32 = EhsHURLdoRequest(PURLLGET_CURL); /* This will block untill the call back has written out data with pauses etc. */
        EHSH_LOG_ERROR("Got return from Request %s returned %d",pUrlGet->szActualSavePath,ret32);
        /* Hack in case threads have been closed down */
        if (*bNewSodlFlagRef)
        {
            if (pUrlGet->write_data_buffer_struct->filehandle)
            {
                EHSH_LOG_ERROR("**************** 1 Fclose = %d",fileno(pUrlGet->write_data_buffer_struct->filehandle));
                EhsFclose(pUrlGet->write_data_buffer_struct->filehandle);
                pUrlGet->write_data_buffer_struct->filehandle = NULL;
            }
            goto getyourcoat;
        }

    }
    else     /* We didn't do the get because we already have it*/
    {
#ifdef EHS_URL_DEBUG
#endif
        EHSH_LOG_ERROR("NOT NEEDED for %s",pUrlGet->szActualSavePath);
    }
    /**********************************************************************************************/
    /* Request is done.
     * At this point we may have some config errors, but continue to run with sepcfici exceptions */
    /**********************************************************************************************/

    if (pUrlGet->write_data_buffer_struct->filehandle)   /* write the time stamp data */
    {
        EhsFclose(pUrlGet->write_data_buffer_struct->filehandle);
        pUrlGet->write_data_buffer_struct->filehandle = NULL;
        //EHSH_LOG_ERROR("!!!!Checking status of downloaded file %s",szTempFilePath2);
        bFileexists = Ehs_UserStat(szTempFilePath2, &statbuf); //@todo We want a hal_file version of stat - should have localpath// exemption.
        if (statbuf.st_size == EHS_TRUE) request_config_error = EHS_TRUE;
    }

    if (ret32 == 200)
    {
        if (pUrlGet->bWriteToFile)
        {
            // if all is good then mv to expected file position file
            if (request_config_error == EHS_FALSE)
            {
                Ehs_UserRm(pUrlGet->szActualSavePath);

                Ehs_UserRename(szTempFilePath2,pUrlGet->szActualSavePath);
#ifndef EHS_MINGW
                EhsFsync(); // odo wemight have wnted to use fsync her to be more specific, but we don't have a file handle to this file at mo.
#endif
                /* We will update the time of the file with the server time so that we can test for updates*/
                if (remotefiletimestamp == 0)   // we haven't got the time from the header test - first time file is written so stamp it with remote
                {
                    success = curl_easy_getinfo(PURLLGET_CURL, CURLINFO_FILETIME, &remotefiletimestamp); //returns seconds since 1970.
                }

                if (remotefiletimestamp > 0 )   // time stamp the local file if we got a file or detected the file on the server
                {
                    EHSH_LOG_INFO("Setting time  stamp for [%s] - from %s",pUrlGet->szActualSavePath,pUrlGet->szURL);
                    new_times.actime = remotefiletimestamp;
                    new_times.modtime = remotefiletimestamp;
                    EHSH_LOG_INFO("Setting time stamp on [%s],time=%d",pUrlGet->szActualSavePath,(ehs_sint32)remotefiletimestamp);
                    Ehs_UserUtime(pUrlGet->szActualSavePath, &new_times); // note szTempFilePath is the full absolute pathafter naming back to normal  not user user relevant
                }
                else EHSH_LOG_ERROR(" BAD! -- SUCCESS  = %d [%s][%s]",success,pUrlGet->szActualSavePath,pUrlGet->szURL);
            }
        }

    }
    else     // Networking Problem - leve all as is
    {
        EHSH_LOG_ERROR(" Not got %s", szTempFilePath2);
        if (pUrlGet->bWriteToFile) Ehs_UserRm(szTempFilePath2); // tidy up the temp file if we coldn't actually download it.
        EHSH_LOG_INFO("GetUrl_thread: get info returned %d [%d]",success,ret32);
        bNetworkError = EHS_TRUE;
    }
    if (ret32 == 200)   // for all good events
    {
        EHSH_LOG_INFO(" HTTP RET = 200");

        if (EHS_FB_OUT_CONNECTED(EHS_FB_URLGET_FINALURL))   /* write out the final redirected URL */
        {
            ehs_char *szRedirect = NULL;
            success = curl_easy_getinfo(PURLLGET_CURL, CURLINFO_EFFECTIVE_URL,&szRedirect);
            EHSH_LOG_INFO(" Effective URL = [%s]",szRedirect);
            if (szRedirect && success == CURLE_OK) EhsStrcpy(EHS_FB_OUT_S(EHS_FB_URLGET_FINALURL), szRedirect);
            else EhsStrcpy(EHS_FB_OUT_S(EHS_FB_URLGET_FINALURL), "");
        }
/* todo 2024 - re-instate this for community version*/
#ifdef ENABLE_GET_HEADERS
        if ( EHS_FB_OUT_CONNECTED(EHS_FB_URLGET_HEADERS))   /* move to a function call! */
        {
            struct curl_slist *slist=NULL;
            success = curl_easy_getinfo(PURLLGET_CURL, CURLINFO_HEADERLIST,&slist);
            if ((success == CURLE_OK) && slist)
            {
                EhsStrcpy(EHS_FB_OUT_S(EHS_FB_URLGET_COOKIES),"");
                while (slist && ((EhsStrlen(slist->data) + EhsStrlen(EHS_FB_OUT_S(EHS_FB_URLGET_COOKIES))) < (EHS_STRING_LENGTH_MAX-2) ))   //@todo this truncates if longer than string size...
                {
                    EhsStrcat(EHS_FB_OUT_S(EHS_FB_URLGET_COOKIES),slist->data);
                    EhsStrcat(EHS_FB_OUT_S(EHS_FB_URLGET_COOKIES),"\n");
                    slist=slist->next;
                }
            }
            else EhsStrcpy(EHS_FB_OUT_S(EHS_FB_URLGET_COOKIES),"");
            if (slist) curl_slist_free_all(slist);

        }
#endif

        /*write out the cookies if connected */
#ifdef ENABLE_COOKIES_OUTPUTS
        if ( EHS_FB_OUT_CONNECTED(EHS_FB_URLGET_COOKIES))   /* move to a function call! */
        {
            struct curl_slist *slist=NULL;
            success = curl_easy_getinfo(PURLLGET_CURL, CURLINFO_COOKIELIST,&slist);
            if ((success == CURLE_OK) && slist)
            {
                EhsStrcpy(EHS_FB_OUT_S(EHS_FB_URLGET_COOKIES),"");
                while (slist && ((EhsStrlen(slist->data) + EhsStrlen(EHS_FB_OUT_S(EHS_FB_URLGET_COOKIES))) < (EHS_STRING_LENGTH_MAX-2) ))   //@todo this truncates if longer than string size...
                {
                    EhsStrcat(EHS_FB_OUT_S(EHS_FB_URLGET_COOKIES),slist->data);
                    EhsStrcat(EHS_FB_OUT_S(EHS_FB_URLGET_COOKIES),"\n");
                    slist=slist->next;
                }
            }
            else EhsStrcpy(EHS_FB_OUT_S(EHS_FB_URLGET_COOKIES),"");
            if (slist) curl_slist_free_all(slist);

        }
#endif
        /* handle existing when a new app is downloaded */
        if (*bNewSodlFlagRef)
        {
            goto getyourcoat;
        }
        /* Report what happened if good or bad */
        if (request_config_error == EHS_FALSE)
        {
            EhsTPMutex_lock(EhsTPMutex_fbIO);
            if(pUrlGet->write_data_buffer_struct->bAbort==EHS_FALSE){ // only notify compleate if not aborted
                EhsStrcpy(EHS_FB_OUT_S(EHS_FB_URLGET_ESTRING), "OK: HTTP: 200"); //@todo add the errornumber "ERROR: HTTP %d",ret)
                EHS_FB_OUT_I(EHS_FB_URLGET_ENUM) = ret32; //@todo this have the actual http return value.
                EHS_FB_FINISH(EHS_FB_URLGET_DATACOMPLETE); //@todo we Need a all data complete version of this and a chunk version
            }
            EhsTPMutex_unlock(EhsTPMutex_fbIO);
        }
        else
        {
            EhsTPMutex_lock(EhsTPMutex_fbIO);
            EhsStrcpy(EHS_FB_OUT_S(EHS_FB_URLGET_ESTRING), "Internal File System Error"); //@todo add the errornumber "ERROR: HTTP %d",ret)
            EHS_FB_OUT_I(EHS_FB_URLGET_ENUM) = ret32;
            EHS_FB_FINISH(EHS_FB_URLGET_ERROR);
            EhsTPMutex_unlock(EhsTPMutex_fbIO);
        }
    }
    else     /* If we haven't actually downloaded we report what was done */
    {
        EhsTPMutex_lock(EhsTPMutex_fbIO);
        if (bNeed_to_do)
        {
            if (bNetworkError == EHS_TRUE)   /* Used a previous version */
            {
                if (ret32 > 0 )
                {
                    EhsSprintf(szTempString, "http errno %d", ret32);
                }
                else
                {

                    switch (ret32 * -1)
                    {
                    case  0 :
                        EhsSprintf(szTempString, "CURL OK unexplained Error"); // Shouldn't be here
                        break;
                    case 1 :
                        EhsSprintf(szTempString, "CURLE_UNSUPPORTED_PROTOCOL");
                        break;
                    case 3 :
                        EhsSprintf(szTempString, "CURLE_URL_MALFORMAT");
                        break;
                    case 6 :
                        EhsSprintf(szTempString, "CURLE_COULDNT_RESOLVE_HOST");
                        break;
                    case 7 :
                        EhsSprintf(szTempString, "CURLE_COULDNT_CONNECT");
                        break;
                    case 9 :
                        EhsSprintf(szTempString, "CURLE_REMOTE_ACCESS_DENIED");
                        break;
                    case 18 :
                        EhsSprintf(szTempString, "CURLE_PARTIAL_FILE");
                        break;
                    case 27 :
                        EhsSprintf(szTempString, "CURLE_OUT_OF_MEMORY");
                        break;
                    case 28 :
                        EhsSprintf(szTempString, "CURLE_OPERATION_TIMEDOUT");
                        break;
                    case 34 :
                        EhsSprintf(szTempString, "CURLE_HTTP_POST_ERROR");
                        break;
                    case 35 :
                        EhsSprintf(szTempString, "CURLE_SSL_CONNECT_ERROR");
                        break;
                    case 47 :
                        EhsSprintf(szTempString, "CURLE_TOO_MANY_REDIRECTS");
                        break;
                    case 52 :
                        EhsSprintf(szTempString, "CURLE_GOT_NOTHING");
                        break;
                    case 55 :
                        EhsSprintf(szTempString, "CURLE_SEND_ERROR");
                        break;
                    case 56 :
                        EhsSprintf(szTempString, "CURLE_RECV_ERROR");
                        break;
                    case 58 :
                        EhsSprintf(szTempString, "CURLE_SSL_CERTPROBLEM");
                        break;
                    case 59 :
                        EhsSprintf(szTempString, "CURLE_SSL_CIPHER");
                        break;
                    case 60 :
                        EhsSprintf(szTempString, "CURLE_PEER_FAILED_VERIFICATION");
                        break;
                    case 77 :
                        EhsSprintf(szTempString, "CURLE_SSL_CACERT_BADFILE");
                        break;
                    default :
                        EhsSprintf(szTempString, "CURL Error No. [%d]",-1 * ret32);
                    }
                }
                EhsStrcpy(EHS_FB_OUT_S(EHS_FB_URLGET_ESTRING), szTempString); //@todo add the errornumber "ERROR: HTTP %d",ret)
                EHS_FB_OUT_I(EHS_FB_URLGET_ENUM) = ret32;
                EHS_FB_FINISH(EHS_FB_URLGET_ERROR);
            }
            else   // Shouldn't get to this one
            {
                EhsSprintf(szTempString, "unknown errno %d", ret32);
                EhsStrcpy(EHS_FB_OUT_S(EHS_FB_URLGET_ESTRING), szTempString); //@todo add the errornumber "ERROR: HTTP %d",ret)
                EHS_FB_OUT_I(EHS_FB_URLGET_ENUM) = ret32;
                EHS_FB_FINISH(EHS_FB_URLGET_DATACOMPLETE); //@todo we Need a all data complete version of this and a chunk version
            }
        }
        else     /* Didn't need to do - we can ignore network errors as we would only get here if the network worked sifficiently to know what we have is OK*/
        {
            EhsStrcpy(EHS_FB_OUT_S(EHS_FB_URLGET_ESTRING), "OK: FILE EXISTS"); //@todo add the errornumber "ERROR: HTTP %d",ret)
            EHS_FB_OUT_I(EHS_FB_URLGET_ENUM) = 0; //@todo this have the actual http return value.
            EHS_FB_FINISH(EHS_FB_URLGET_DATACOMPLETE); //@todo we Need a all data complete version of this and a chunk version
        }
        EhsTPMutex_unlock(EhsTPMutex_fbIO);
    }
getyourcoat:
    //pUrlGet->bBusy = EHS_FALSE
    if (*bNewSodlFlagRef)
    {
        curl_easy_cleanup(PURLLGET_CURL);
    } /* clean up only if the app has finished */
error:
    EhsHURLfree_write_data_buffer(pUrlGet->write_data_buffer_struct); // Don't worry this checks for null
    pUrlGet->bBusy = EHS_FALSE;
    Ehs_FB_ThreadComplete();
    EhsHThread_exit()
    ;

}


/* Creates the path name from the URL directories and/or the local path depending on format.

 if the write path / terminated then create the file from the URL path extension.
 */

void EHS_UrlGet_CreateSavePath(struct EhsFbUrlGetStruct* pUrlGet)
{

    ehs_char * zTemp;
    ehs_char * zSavePath = pUrlGet->szActualSavePath;
    /*Try to open and create the file @todo this is a bit messy - the Fopen should only be called from one place at the end */

    if ((EhsStrlen(pUrlGet->szSavePath) > 0) && (EhsStrcmp(pUrlGet->szSavePath,"NULL")))
    {
        if (!pUrlGet->bKeepPath)
        {
            EhsStrcpy(zSavePath, pUrlGet->szSavePath);
        }
        else
        {
            /* if the write path / terminated then create the file from the URL path extension. */
            if (pUrlGet->szSavePath[EhsStrlen(pUrlGet->szSavePath) - 1] == '/')
            {
                zTemp = EhsGetPathFromURL(pUrlGet->szURL,pUrlGet->nPathLevelsToIgnore);
                EhsStrcpy(zSavePath, pUrlGet->szSavePath);
                EhsStrcat(zSavePath, zTemp);
            }
            else     // We want all data to be written to a specific fixed file
            {
                EhsStrcpy(zSavePath, pUrlGet->szSavePath);
            }
        }
    }
    else     /* No path provided */
    {
        if (pUrlGet->bKeepPath)
        {
            zTemp = EhsGetPathFromURL(pUrlGet->szURL,pUrlGet->nPathLevelsToIgnore);
            EhsStrcpy(zSavePath, zTemp);

        }
        else     /* use the URL anyway - we don't want this duplicated */
        {
            zTemp = EhsGetPathFromURL(pUrlGet->szURL,-1); /* remove all paths and get just the file */
            EhsStrcpy(zSavePath, zTemp);
            //EhsStrcpy(zSavePath, "./temp_url_get.http");
        }
    }

}

EHS_GLOBAL EHS_FB_RUN_FUNCTION(UrlGet_get)
{
    ehs_bool OK = EHS_TRUE;
    struct EhsFbUrlGetStruct* pUrlGet = (struct EhsFbUrlGetStruct *) EHS_FB_RUN_CONTEXT;
    EHSH_LOG_INFO("-> In Thread to get [%s] - current=[%s]",EHS_FB_IN_S(EHS_FB_URLGET_LOCAL_PATH),pUrlGet->szURL);

    if (pUrlGet->bBusy == EHS_TRUE)
    {
        EHS_FB_OUT_I(EHS_FB_URLGET_ENUM) = -1; // @todo define the error flags
        EhsSnprintf(EHS_FB_OUT_S(EHS_FB_URLGET_ESTRING),EHS_STRING_LENGTH_MAX,"Busy with %s",pUrlGet->szURL);
        //EHSH_LOG_ERROR("xxxxxxxx BUSY with %s getting [%s]",pUrlGet->szURL,EHS_FB_IN_S(EHS_FB_URLGET_URL ));
        OK = EHS_FALSE;
    }
    else
    {
        EhsStrcpy(EHS_FB_OUT_S(EHS_FB_URLGET_ESTRING),"");
        EhsFunctionInstanceDataType* blob = (EhsFunctionInstanceDataType*) pFIdata;
        if (EHS_FB_IN_CONNECTED(EHS_FB_URLGET_URL ))
        {
            EhsStrcpy(pUrlGet->szURL, EHS_FB_IN_S(EHS_FB_URLGET_URL ));
        }
        if (EHS_FB_IN_CONNECTED(EHS_FB_URLGET_POST_DATA))
        {
            EhsStrcpy(pUrlGet->szPostData, EHS_FB_IN_S(EHS_FB_URLGET_POST_DATA));
            //set post to true if not already forced.
            //pUrlGet->bPost = EHS_TRUE;
        }
        if (EHS_FB_IN_CONNECTED(EHS_FB_URLGET_LOCAL_PATH))
        {
            EhsStrcpy(pUrlGet->szSavePath,
                      EHS_FB_IN_S(EHS_FB_URLGET_LOCAL_PATH));
            pUrlGet->bWriteToFile = EHS_TRUE;
        } // else is the default and connection is constant at runtime
        if (EHS_FB_IN_CONNECTED(EHS_FB_URLGET_URLDIRDUMP))
        {
            pUrlGet->nPathLevelsToIgnore = EHS_FB_IN_I(EHS_FB_URLGET_URLDIRDUMP);
        }
        if (EHS_FB_IN_CONNECTED(EHS_FB_URLGET_PRIORITY))
        {
            pUrlGet->nPriority = EHS_FB_IN_I(EHS_FB_URLGET_PRIORITY);
        }
        if (EHS_FB_IN_CONNECTED(EHS_FB_URLGET_USECOOKIES))
        {
            pUrlGet->bSendCookies=EHS_FB_IN_B(EHS_FB_URLGET_USECOOKIES);
        }
        if (EHS_FB_IN_CONNECTED(EHS_FB_URLGET_SETHEADER_STRING))
        {
            EhsStrcpy(pUrlGet->szHeaderToSend,EHS_FB_IN_S(EHS_FB_URLGET_SETHEADER_STRING));
        }
        if (EHS_FB_IN_CONNECTED(EHS_FB_URLGET_SETCREDENTIALS_USER))
        {
            EhsStrcpy(pUrlGet->szUname,EHS_FB_IN_S(EHS_FB_URLGET_SETCREDENTIALS_USER));
        }
        if (EHS_FB_IN_CONNECTED(EHS_FB_URLGET_SETCREDENTIALS_PASS))
        {
            EhsStrcpy(pUrlGet->szPasswd, EHS_FB_IN_S(EHS_FB_URLGET_SETCREDENTIALS_PASS));
        }
        if (EHS_FB_IN_CONNECTED(EHS_FB_URLGET_SETSSL_CERTIFCATE))
        {
            EhsStrcpy(pUrlGet->szSslCertificatePath,EHS_FB_IN_S(EHS_FB_URLGET_SETSSL_CERTIFCATE));
        }
        if (EHS_FB_IN_CONNECTED(EHS_FB_URLGET_SETSSL_PASSWORD))
        {
            EhsStrcpy(pUrlGet->szSslCertificatePassword,EHS_FB_IN_S(EHS_FB_URLGET_SETSSL_CERTIFCATE));
        }
        if (EHS_FB_IN_CONNECTED(EHS_FB_URLGET_USE_PROXY_FLAG))
        {
            pUrlGet->bUseproxy=EHS_FB_IN_B(EHS_FB_URLGET_USE_PROXY_FLAG);
        }
        if (EHS_FB_IN_CONNECTED(EHS_FB_URLGET_USE_POST))
        {
            pUrlGet->bPost=EHS_FB_IN_B(EHS_FB_URLGET_USE_POST);
        }
        if (EHS_FB_IN_CONNECTED(EHS_FB_URLGET_USE_OVERWRITE))
        {
            pUrlGet->bOverwriteAll=EHS_FB_IN_B(EHS_FB_URLGET_USE_OVERWRITE);
        }
        if (EHS_FB_OUT_CONNECTED(EHS_FB_URLGET_RESPONSE))
        {
            pUrlGet->bDataPortConnected=EHS_TRUE;
        }
        else
        {
            pUrlGet->bDataPortConnected=EHS_FALSE;
        }
        /* Check status and either do things, setting flags as required */
        if (OK)  //@todo need to update EHS_FB_START_THREAD with thread priority info in here
        {
            EHS_UrlGet_CreateSavePath(pUrlGet); // construct a save path
            pUrlGet->bBusy = EHS_TRUE;
            EHS_FB_START_THREAD(GetUrl_thread,-99); // @todo we want to make a specific version of this that makes a new copy of the object data and repoints the pfi data to it, so that the paramters changing whilst processing.
            EHS_FB_FINISH(EHS_FB_URLGET_GETTING);
        }
        else
        {
            EHS_FB_FINISH(EHS_FB_URLGET_ERROR);
        }
    }
}

/* This should only be set if the block i actually busy */
EHS_GLOBAL EHS_FB_RUN_FUNCTION(UrlGet_abort)
{
    struct EhsFbUrlGetStruct* pUrlGet = (struct EhsFbUrlGetStruct *) EHS_FB_RUN_CONTEXT;
    if (pUrlGet->write_data_buffer_struct)
    {

        if (pUrlGet->bBusy)
        {
            pUrlGet->write_data_buffer_struct->bAbort=EHS_TRUE;
        }

    }

//todo to check the abort flag
    EHS_FB_FINISH(EHS_FB_URLGET_ABORT);
}

EHS_GLOBAL EHS_FB_RUN_FUNCTION(UrlGet_savecookies)
{
    struct EhsFbUrlGetStruct* pUrlGet = (struct EhsFbUrlGetStruct *) EHS_FB_RUN_CONTEXT;
    //CURLOPT_COOKIEFILE
    ehs_char temp[EHS_STRING_LENGTH_MAX];
    ehs_char temp2[EHS_STRING_LENGTH_MAX];
    if (PURLLGET_CURL && EhsStrlen(pUrlGet->szURL) )
    {
        EhsURL_EncodeURLtoCookieJarFSName(temp2,pUrlGet->szURL);
        if (EhsTF_tryCanonicPath(temp, EHS_RUNTIME_USERDATA_DIR, temp2,EHS_TRUE))  //try to create the path whatever
        {
            Ehs_MakePath(temp,EHS_TRUE); /* makethe dir path - but last in path is a file */
            curl_easy_setopt(PURLLGET_CURL, CURLOPT_COOKIEJAR, temp); // todo extend with [user]/[URL].cookies
            curl_easy_setopt(PURLLGET_CURL, CURLOPT_COOKIELIST, "FLUSH"); //wrte to file
        }
    }
    //todo Need to do some locking here ..
    //curl_easy_cleanup(PURLLGET_CURL);
    //PURLLGET_CURL=NULL;//force this to be recreated next start
    //curl_easy_setopt(PURLLGET_CURL, CURLOPT_COOKIELIST,
    //@todo Need to write info to a canonical place ../userdata/configs/[app]/[URL].cookies
    EHS_FB_FINISH(EHS_FB_URLGET_SAVECOOKIES_DONE);
}

EHS_GLOBAL EHS_FB_RUN_FUNCTION(UrlGet_clearcookies)
{
    struct EhsFbUrlGetStruct* pUrlGet = (struct EhsFbUrlGetStruct *) EHS_FB_RUN_CONTEXT;
    if (PURLLGET_CURL && EhsStrlen(pUrlGet->szURL))
    {
        ehs_char temp[EHS_STRING_LENGTH_MAX];
        EhsURL_EncodeURLtoCookieJarFSName(temp,pUrlGet->szURL);
        curl_easy_setopt(PURLLGET_CURL, CURLOPT_COOKIELIST, "ALL"); //[app]/[URL].cookies
        curl_easy_setopt(PURLLGET_CURL, CURLOPT_COOKIESESSION, 1); //Start a new cookie session to prevent any loading (belt and braces?)
        Ehs_UserRm(temp); /* remove the stored file */
    } //else do nothing
    EHS_FB_FINISH(EHS_FB_URLGET_CLEARCOOKIES_DONE);
}

EHS_GLOBAL EHS_FB_RUN_FUNCTION(UrlGet_getmore)
{
    struct EhsFbUrlGetStruct* pUrlGet = (struct EhsFbUrlGetStruct *) EHS_FB_RUN_CONTEXT;
    if (pUrlGet->write_data_buffer_struct)
    {
        EhsHURLset_write_data_buffer_continue(pUrlGet->write_data_buffer_struct); /*Let the get thread get more data and do the event assertions */
    }
}
















