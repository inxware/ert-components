/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

/** @file devman_player.c
 *
 * Implementation of the devman player functions for the networking toolkit
 *
 * @author: inx limited
 * @todo the numbers for te arguments should be translated into
 * #defs in readiness for autogenerating them producing human readable code.
 */
/**
 * @page FunctionReference Lucid Function Block Reference Guide
 * @section Networking
 * @anchor DevmanPlayer
 * @subsection appliesTo Applies To:
 * Networking &gt; Devman &gt; Player
 * @subsection properties Properties:
 * Nonev
 * @subsection description Description:
 * This function block implements the devman player.
 *
 * This provides contact with a devman server that supports the devman-player support.
 * This is responsible for detecting new playlists made available on the devman server, downloading them
 * and notifying the application that it is available and where it is stored. It is not playlist format
 * aware and does not parse the playlists to identify tracks that need to be downloaded. This Function
 * block is neither responsible for downloading content. this is done by the playlist manager in conjunction
 * with a downloader function block.
 *
 * eStartChecking: (starts timer for checking if not already started by the default static parameter
 * zDevManURL to start it init -).
 *
 * eStopChecking: (stops checking if not already started).
 *
 * eReconfigure: this will read in any data on the configuration inputs and overwrite the static parameters.
 *
 * @todo The DEVMAN XML tags should be included from headers in a DEVMAN config directory.
 */


#include "ehs_fb_types.h"
#include "devman_player.h"
#include "setCompletes.h"
#include "target.h"
#include "hal_string.h"
#include "hal_file.h"
#include "app_data.h"
#include "hal_time.h"
#include "callback_queue.h"
#include "hal-api.h"
#include "hal_linkedlist.h"
#include "hal_jsonlist.h"


#ifdef EHS_DEVMAN_PLAYER_USE_LUA
#include "lua.h"
#error "and some Laux bits that were deleted."
#endif


/*actual mapping of Functions to IDF names - depricate the MACROs!!!*/
EHS_FB_FUNCTIONS_START(DevmanPlayer)
EHS_FB_FUNCTION_ENTRY("startchecking", 0x00, DevmanPlayer_start_checking) 
EHS_FB_FUNCTION_ENTRY("stopchecking", 0x01, DevmanPlayer_stop_checking)
EHS_FB_FUNCTION_ENTRY("changeurl", 0x02, DevmanPlayer_change_url)
EHS_FB_FUNCTION_ENTRY("reconfigure", 0x03, DevmanPlayer_reconfigure)
EHS_FB_FUNCTION_ENTRY("trackchanged", 0x04, DevmanPlayer_track_changed)
EHS_FB_FUNCTION_ENTRY("passthru_next", 0x05, DevmanPlayer_pass_thru_get_next)
EHS_FB_FUNCTION_ENTRY("passthru_send", 0x06, DevmanPlayer_pass_thru_send_next)
EHS_FB_FUNCTION_ENTRY("out", 0x07, DevmanPlayer_out)
EHS_FB_FUNCTION_ENTRY("reset", 0x08, DevmanPlayer_reset)
EHS_FB_FUNCTIONS_END
//EHS_FB_FUNCTION_ENTRY(EHS_FB_DEVMAN_PLAYER_START_FROM_DISK,  DevmanPlayer_StartFromDisk)
//EHS_FB_FUNCTION_ENTRY(EHS_FB_DEVMAN_PLAYER_CLEAR_PLAYLIST, DevmanPlayer_clearPlaylist)

/* port identifiers */
#define PORT_FINISH_START 1
#define PORT_FINISH_STOP 1
#define PORT_FINISH_CHANGE 1
#define PORT_FINISH_RECONFIGURE 1
#define PORT_CALLBACK_OUT -1

/* This parameter allows for retrying devman URLS from the top if the list every so often
 * It defaults to be the same as the DEVMAN CORE cvalue*/
#ifndef EHS_DEVMAN_PLAYER_RETRY_PERIOD_FIRSTURL
#define EHS_DEVMAN_PLAYER_RETRY_PERIOD_FIRSTURL EHS_DEVMAN_CORE_RETRY_PERIOD_FIRSTURL
#endif

#ifndef EHS_DEVMAN_PLAYER_RETRY_TIMES_FIRSTURL
#define EHS_DEVMAN_PLAYER_RETRY_TIMES_FIRSTURL 2 //EHS_DEVMAN_CORE_RETRY_TIMES_FIRSTURL
#endif

/* Set up some standard fail over paths */
#ifndef EHS_DEVMAN_PLAYERURLS
#define	EHS_DEVMAN_PLAYERURLS	"plugins/1/DEVMANPLAYERURL"
#endif

/* Set up some defaults if a URL file is not found */
#ifndef EHS_DEVMAN_PLAYERDEFAULTURL_PATH
// this ss the old version to be killed off: #define EHS_DEVMAN_PLAYERDEFAULTURL_PATH	"/cgi-bin/devman_player.cgi"
#define EHS_DEVMAN_PLAYERDEFAULTURL_PATH	"/devmanPlayerCGI.php"
// TODO - change this to /devmanPlayerCGI.php (no cgi-bin/)
#endif


#ifndef EHS_DEVMAN_PLAYERDEFAULTURL
#define EHS_DEVMAN_PLAYERDEFAULTURL	"https://devman.inx-systems.com" EHS_DEVMAN_PLAYERDEFAULTURL_PATH
#endif

#ifndef EHS_DEVMAN_PLAYER_MAX_SERVER_URLS
#define EHS_DEVMAN_PLAYER_MAX_SERVER_URLS 10
#endif

#ifdef EHS_DEVMAN_PLAYER_USE_LUA
const ehs_char* LUA_FILE = "../devman/plugins/1/devman_player.inx";
#endif

#define EHS_STRING_LENGTH_MAX_LARGE (EHS_STRING_LENGTH_MAX*4) //@todo formalise this
struct EhsFbDevmanPlayerStruct   //Note make all ints and bools ehs_uint32 to avoid scanf ("%d") problem overwriting the next entry...!
{
    ehs_bool bCheckingFlag; /** [Variable] Poll server */
    ehs_bool bPlayListLoadAtInit; /* [Parameter] toindicate stored playlist data should be loaded when activated. */
    ehs_bool bdoPlayListLoad; /* [Variable] This flag is used to kick start the player when a playlist has been downloaded */
    ehs_bool bResetingMedia; // this is set so that cleared playlist is pushed out the data ports (but no new playlist event is emitted
    ehs_bool bMediaParmChange; // Set this at init if volume or channels has been read to allow this to be asserted
    ehs_bool bMuteParmChange; // Set this at init if a mute state has been read from the state info
    ehs_char szUrl[EHS_STRING_LENGTH_MAX]; /* [devman-variable] This is the devman server URL */
    ehs_char szUrl_parameter [EHS_STRING_LENGTH_MAX]; /* This is the stored value of the given parameter - priority over the devman/core/config value */
    ehs_char szUrl_input [EHS_STRING_LENGTH_MAX];	/* This is the input value of the devman - priority over the devman/core/config value and parameter*/
    ehs_uint16 iPingPeriod; /* [Parameter] also adjustable by pulse rate input */
    ehs_uint32 nMute; /*[devman-output-variable] output for controlling player volume @todo this should be enumerated for different players*/
    ehs_uint32 nPlayerState; /*[devman-input-variable] inout to identify player state parameter.*/
    ehs_uint32 nChannels; /* UNUSED [devman-ouput-variable] to identify mono. stereo, 5.1 audio output modes */
    ehs_uint32 nPlayMode; /*[devman-ouput-variable] to identify player loop or random modes */
    ehs_char szPlaylistURL[EHS_STRING_LENGTH_MAX]; /*[devman-ouput-variable] to identify the fully qualified URL of where playlists are downloaded from - if devman specfies only the filename then the media source URL prepended here by devman code?*/
    ehs_char szPlaylistURLReport[EHS_STRING_LENGTH_MAX]; /* This is the application's reported URL that is reported back to the portal - it isn't used for anything else */
    ehs_char szPlaylistFile[EHS_STRING_LENGTH_MAX]; /*[devman-ouput-variable] to identify the filename of the playlist (located at the base URL) */
    ehs_char szMediaSrcUrl[EHS_STRING_LENGTH_MAX]; /*[devman-ouput-variable] to identify the BASE URL for content references in the playlist */
    ehs_char szLocalMediaPath[EHS_STRING_LENGTH_MAX]; /* [Parameter] NOT CURRENTLY USED - identifies the local path - should have an output port to define a constant path */
    ehs_bool bUseDevmanCoreUrl; /*[Parameter] to identify that devman should ignore URL specs and revert to the core URL */
    ehs_uint32 iPlayerMode; /*[devman-input-variable] to identify the current index in the playlist being played */
    ehs_char szCurrentTrack[EHS_STRING_LENGTH_MAX]; /*[devman-input-variable] to identify the current media name in the playlist being played */
    ehs_char szTrackStartTime[EHS_STRING_LENGTH_MAX]; /*[devman-input-variable] to identify the next wall clock time when the next media will be played */
    ehs_uint32 iCurrentTrackDuration; /*[devman-input-variable] to identify the current time (in seconds) that the current media has being played */// @todo check 16 bit is correct
    ehs_char jsonPlayerInfo[EHS_STRING_LENGTH_MAX]; /* Volume measured on input port */
    ehs_char jsonPlayerParameters[EHS_STRING_LENGTH_MAX]; /* incoming from devman server JSON commands for volumes for each channel */
    EhsLinkedList* pIncomingJsonAppPassThruDataList; /* aggregates incoming JSON object in a linked list. */
    EhsJsonList* pOutgoingJsonAppPassThruDataList; /* used for sending JSON array to devman server. */
    EhsCallbackQueueType EhsDevmanPlayerOutCallback; //pointer to the call back
    EhsCallbackQueueEntryType xCallbackOut; /* [ internal system variable!! */
    ehs_char * szXml;// now malloced by the URL code - [EHS_STRING_LENGTH_MAX_LARGE]; /* [ internal system variable!! @todo this is dangerous this is a fixed size char array for the SMIL data - needs to be dynamically allocated*/
    ehs_sint16 CurrentURLindex; // counter for the server index it is trying - 2 is reserved for the input port value, and -1 for the FB parameter
    ehs_uint16 nHTTPReturnNo; // return value of the server post
};

#ifdef EHS_DEVMAN_PLAYER_USE_LUA
static int traceback2(lua_State *L)
{
    return 1;
}
#endif

/**        Some utility functions **/


/*
 * @todo This can be moved to the HAL
 */

/* todo move this to the cheap XML parser file
 *
 * @brief Cheap Function to get an XML attribute value.
   First parameter is attribute name followed by equals e.g. playlist=
   Second parameter is xml string. It is best to pass the string starting at
   the node required for cases when different nodes have same attribute name
 */

int getAttributeValue(ehs_char* szAttribute, ehs_char* szXml, ehs_char* szTempStr2)
{
    ehs_char* pTempStr3;
    ehs_char* pTempStr;
    int i;
    ehs_char szTempStr1[EHS_STRING_LENGTH_MAX] = { '\0' }; /* @todo we don't need this - should use the input string*/
    ehs_char* pTempStr1 = &szTempStr1[0];
    ehs_char* pTempStr2 = &szTempStr2[0];
    ehs_uint8 quoteCount = 0;
    ehs_uint16 j = 0;

    if (pTempStr3 = EhsStrstr(szXml, szAttribute))
    {
        // crashy if string too long! EhsStrcpy(pTempStr1, pTempStr3); // don't use n version as this always reads all (up to n) from source
        for (i =0 ; i< EHS_STRING_LENGTH_MAX; i++)
        {
            pTempStr1[i] = pTempStr3[i];
            if (pTempStr3[i] == '\0') break;

        }
        pTempStr1[EHS_STRING_LENGTH_MAX-1]='\0'; // in case we got right to the end!
        if (strlen(pTempStr1) > 0)
        {
            for (i = 0; i < strlen(pTempStr1); i++)
            {
                if (quoteCount == 1)
                {
                    if (szTempStr1[i] == '"')
                    {
                        quoteCount++;
                    }
                    else
                    {
                        szTempStr2[j] = szTempStr1[i];
                        j++;
                    }
                }
                if (szTempStr1[i] == '"')
                {
                    quoteCount++;
                }
            }
            szTempStr2[j] = '\0'; // terminate string properly
        }
        else
            return -1;
    }
    else
    {
        return -1;
    }
    return 0; /* All good */
}


/* function for finding a URL from the function block or from the global Devman IP address pools */
/* The following convention for the Current URL index is used:
 *     -2: FB port input
 *     -1: FB parameter
 *     0-999: Devman player  - read from ./plugins/1/DEVMANPLAYERURL.xxx
 *     1000-:Devman core  -  - read from ./core/config/DEVMANURL.xxx
 */
void EhsGetDevmanPlayerURL(struct EhsFbDevmanPlayerStruct* pDevmanPlayer)
{

    ehs_char tempPath[EHS_STRING_LENGTH_MAX];
    if (pDevmanPlayer->CurrentURLindex <= -2 )
    {
        if (EhsStrlen(pDevmanPlayer->szUrl_input) < 8) pDevmanPlayer->CurrentURLindex=-1; /* We haven't got a proper URL so don't bother with this */
        else EhsStrcpy(pDevmanPlayer->szUrl, pDevmanPlayer->szUrl_input);
    }
    if (pDevmanPlayer->CurrentURLindex == -1 )
    {
        if (EhsStrlen(pDevmanPlayer->szUrl_parameter) < 8 ) pDevmanPlayer->CurrentURLindex=0;
        else EhsStrcpy(pDevmanPlayer->szUrl, pDevmanPlayer->szUrl_parameter);
    }
    if (pDevmanPlayer->CurrentURLindex >= 0 && pDevmanPlayer->CurrentURLindex < 1000 )
    {
        if (!EhsHDevmanGetURL(pDevmanPlayer->szUrl, EHS_DEVMAN_PLAYERURLS, EHS_MAXDEVMANNAMELEN, pDevmanPlayer->CurrentURLindex))
            pDevmanPlayer->CurrentURLindex=1000; /* Try the devman core list */
    }
    if (pDevmanPlayer->CurrentURLindex >= 1000 && pDevmanPlayer->CurrentURLindex < 2000 )
    {
        if (EhsHDevmanGetURL(pDevmanPlayer->szUrl, EHS_DEVMAN_COREURLS, EHS_MAXDEVMANNAMELEN, pDevmanPlayer->CurrentURLindex-1000))  /* Only try first the top of the list at this point */
            EhsStrcat(pDevmanPlayer->szUrl,EHS_DEVMAN_PLAYERDEFAULTURL_PATH); /* if this path is OK it won't have the player specific extension so need to add it */
        else
            pDevmanPlayer->CurrentURLindex=2000; /* Try the devman core list */
    }
    if (pDevmanPlayer->CurrentURLindex == 2000)
    {
        EhsStrcpy(pDevmanPlayer->szUrl,EHS_DEVMAN_PLAYERDEFAULTURL); /* try the hard-wired inx-systems.com  */
    }
    if (pDevmanPlayer->CurrentURLindex > 2000)
    {
        pDevmanPlayer->CurrentURLindex=-2; /* start from the top again, but wait to loop around - results in two loops before -2 option is actualy tried */
    }
    //EhsStrcpy(pDevmanPlayer->szUrl, "http://devman.inx-systems.com/cgi-bin/devman_player.cgi");
    EHSH_LOG_INFO("Using Devman Player URL:%s (index %d)\n", pDevmanPlayer->szUrl,pDevmanPlayer->CurrentURLindex);
}


/* global statics are used when the szXML member hasn't got any info. */
ehs_char *EMPTY_PAGE_RETURNED = "EMPTY_PAGE_RETURNED";
ehs_char *SERVER_CONNECTION_ERROR = "SERVER CONNECTION ERROR - 404";
ehs_char *SERVER_INTERNAL_ERROR = "SERVER INTERNAL ERROR - 500";
ehs_char *SERVER_OTHER_ERROR = "SERVER OTHER ERROR";
ehs_char *INTERNAL_ERROR = "DEVICE INTERNAL ERROR";


/* Note the following should use the libURL function to get post data rather than repeat the code here - so that proxies etc. can be handled.*/
EHS_FB_THREAD_FUNCTION(DevmanPlayer_thread)
{
    //ehs_char szBuff[EHS_POST_STRING_LENGTH_MAX] = {'\0'};

    struct tm *tm = NULL;
    time_t xtimet = 0;
    ehs_bool status = EHS_FALSE;
    long http_no = 0;
    ehs_char * PostString = NULL;
    ehs_bool bRunnning = EHS_FALSE;
    ehs_sint32 retry=EHS_DEVMAN_PLAYER_RETRY_PERIOD_FIRSTURL; /* this counts how many times we're not using the top of list URL */
    ehs_sint32 retrytimes=EHS_DEVMAN_PLAYER_RETRY_TIMES_FIRSTURL; /* this counts how many times we're not using the top of list URL */
    ehs_bool trynext=EHS_TRUE; /*flag to identify if the next in the list should be tried */
    ehs_char sZtemp[EHS_STRING_LENGTH_MAX*2];


    ehs_uint16 iSleepCount = 0;
    int authentication = 0;
    ehs_char *zRet = NULL;
    CURLcode success = -1;
    CURL *curl = NULL;
    CURL * curl_temp = NULL;
    EhsH_URLwrite_data_bufferType * write_data_buffer_struct = NULL; // this is used to store all data read from the target.
    ehs_uint32 ret32 = -1; //=(ehs_uint32*) (&sZuserdata[EHS_STRING_LENGTH_MAX_LARGE-(sizeof(ehs_uint32) )]);
    EhsNetworkServerInfo_t server_info= {.http_username="",.http_password="",.authentication=0};
    ehs_char *returndata = NULL; // this points to the data generated in EhsH_URLwrite_data_bufferType. It is cleared by the free function for EhsHwrite_data_bufferTypes
    ehs_bool bDostuff = EHS_TRUE;
    Ehs_FB_ThreadStarted();
    EhsTPMutex_lock(EhsTPMutex_devmanPlayerData);
    struct EhsFbDevmanPlayerStruct* pDevmanPlayer = (struct EhsFbDevmanPlayerStruct *) EHS_FB_INIT_CONTEXT;
    EhsTPMutex_unlock(EhsTPMutex_devmanPlayerData);
    pDevmanPlayer->szXml = NULL; // We would prefer a memory leak than a random
    // This is required for when new sodl is sent

    /* Start the Lib URL configuration - Use a 64K buffer for the data, and a 50ms chunk delay*/
    curl=curl_easy_init(); /* todo this should be moved to init as it isn't thread safe */

    if (!curl)
    {
        EHSH_LOG_ERROR("Could not initialise URL access");
        goto curl_init_error;
    }
    write_data_buffer_struct=EhsHDoAllGenericConfig (curl,&server_info, 64*1024, 50000,120);/* 2 minute timeouts and 50ms chunk gap */

    /* Add any ssl certificates */
    EhsHSetUpClientTlsCertificate(curl, EHS_RUNTIME_DEVMAN_DIR, EHS_DEVMAN_CLIENT_CERTIFICATE_KEY, NULL /* combined in PEM */, NULL);
    EhsHSetUpCaTlsCertificate(curl, EHS_RUNTIME_DEVMAN_DIR, EHS_DEVMAN_CA_CERTIFICATE);

    EhsHSetUpLocalProxy(curl);
    PostString = EhsHMem_tempAlloc(EHS_POST_STRING_LENGTH_MAX); //more than enough -we get this from the app-lifetime memory pool
    if (!PostString)
    {
        EHSH_LOG_ERROR("Couldn't create a past string - disabling devman player");
        goto error;
    }

    /* Start Polling Loop */

    EhsSleep(EHS_TIME_s(2)); /* wait a bit in case app sets a URL input to FB*/

    pDevmanPlayer->CurrentURLindex=-2;/* start at the top of the list */
    while (PostString && bDostuff)
    {
        if (pDevmanPlayer->bCheckingFlag && *bRuntablesReadyRef==EHS_TRUE)
        {


            EhsTPMutex_lock(EhsTPMutex_devman_request); /* don't block the next mutex for long so need to be outside this one */
            EhsTPMutex_lock(EhsTPMutex_devmanPlayerData);
            /* Create the post message */

            PostString[0]='\0'; //reset for the following concats

//#ifdef EHS_MMC_URL_ENCODE
            curl_temp=curl; /* Lets always do url encoding of the data... hey? */
//#else
//			curl_temp=NULL;
//#endif
            EhsHCreateQueryString(curl_temp,PostString, "Group", "DEFAULT",EHS_POST_STRING_LENGTH_MAX);
            EhsHCreateQueryString(curl_temp,PostString, "DeviceType", "DEFAULT",EHS_POST_STRING_LENGTH_MAX); //optional
            EhsHCreateQueryString(curl_temp,PostString, "DeviceID", EhsHMetaGetHWID(),EHS_POST_STRING_LENGTH_MAX);
            //EhsHCreatePostString(curl_temp,PostString,pDevmanPlayer->bUseDevmanCoreUrl);
            EhsHCreateQueryString(curl_temp,PostString, "playlist_name",pDevmanPlayer->szPlaylistURLReport, EHS_POST_STRING_LENGTH_MAX);
            EhsHCreateQueryString(curl_temp,PostString, "current_track",pDevmanPlayer->szCurrentTrack, EHS_POST_STRING_LENGTH_MAX);
            EhsHCreateQueryString(curl_temp,PostString, "track_start_time",pDevmanPlayer->szTrackStartTime, EHS_POST_STRING_LENGTH_MAX); // this is the elapsed time
            EhsHCreateQueryStringNum(PostString, "player_mode",pDevmanPlayer->iPlayerMode, EHS_POST_STRING_LENGTH_MAX);
            EhsHCreateQueryStringNum(PostString, "current_track_duration",pDevmanPlayer->iCurrentTrackDuration,EHS_POST_STRING_LENGTH_MAX); // this is the file metadata total duration
            //* a few more stragglers * /
            EhsHCreateQueryStringNum(PostString, "ping_period",pDevmanPlayer->iPingPeriod, EHS_POST_STRING_LENGTH_MAX);
            EhsHCreateQueryString(curl_temp,PostString, "player_parameters",pDevmanPlayer->jsonPlayerInfo, EHS_POST_STRING_LENGTH_MAX);
            EhsHCreateQueryStringNum(PostString, "mute_state",pDevmanPlayer->nMute, EHS_POST_STRING_LENGTH_MAX);
            EhsHCreateQueryStringNum(PostString, "player_state",pDevmanPlayer->nPlayerState, EHS_POST_STRING_LENGTH_MAX);
            EhsHCreateQueryStringNum(PostString, "device_space_used",(EhsHMetaGetStorAvail()),EHS_POST_STRING_LENGTH_MAX); // report in MB to save MHz* /
            EhsHCreateQueryStringNum(PostString, "device_space_available",(EhsHMetaGetStorTotal()),EHS_POST_STRING_LENGTH_MAX);

            // send JSON pass thru data
            EhsJsonList* json_list = pDevmanPlayer->pOutgoingJsonAppPassThruDataList;
            if(json_list && json_list->length)
            {
                EhsJsonArrayString* json_array_string = ehs_pop_json_array_string(json_list);
                EHSH_LOG_INFO("PASSTHROUGH: %s",json_array_string->data);
                if(!EhsHCreateQueryString(curl_temp, PostString, "jsonAppPassThruData", json_array_string->data, EHS_POST_STRING_LENGTH_MAX))
                {
                    EHSH_LOG_ERROR("JSON pass thru data size (%d) exceeds the post message size limit (%d).\n", json_array_string->size, EHS_POST_STRING_LENGTH_MAX);
                }
                ehs_free_json_array_string(json_array_string);
            }

            EhsTPMutex_unlock(EhsTPMutex_devmanPlayerData); //Not sure we need this but anyhow

            time(&xtimet); // Convert to w3c "%Y-%m-%dT%H:%M:%S", date_temp) //
#ifdef EHS_TRY_TO_FORCE_GMT
            tm = gmtime(&xtimet);// * tm is a static structure - overwritten by gmtime (nasty!) * /
#else
            tm = localtime(&xtimet); /* @todo use tmlocaltime_r(&pEvent->eventTime, &eventTM); */
#endif
            EhsSprintf(sZtemp, "%02d-%02d-%02dT%02d:%02d:%02d", (tm->tm_year)%100, tm->tm_mon+1,
                       tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);//, tm.tm_wday, tm.tm_yday);
            EhsHCreateQueryString(curl_temp,PostString, "local_time", sZtemp,EHS_POST_STRING_LENGTH_MAX);
            pDevmanPlayer->szXml=NULL;//Set this to nothing so we don't parse it again.


            /* Try the list of URLs starting with app specfic then target config. files and set a pDevmanPlayer->szUrl */
            EhsGetDevmanPlayerURL(pDevmanPlayer); /* Note CurrentURL index in range >=1000 are from the core devman URL list*/
            if (EhsHURLConfigPostGet(curl,write_data_buffer_struct,pDevmanPlayer->szUrl, PostString,EHS_TRUE))
            {
                http_no=EhsHURLdoRequest(curl);
            }

            EhsTPMutex_unlock(EhsTPMutex_devman_request);

            if (*bNewSodlFlagRef)   //May have lingered along time in the request, so exit in case memory's been swiped
            {
                bDostuff = EHS_FALSE;
                break; // get out quick to stop the loop
            }
            if (http_no == 200)
            {
                status = EHS_TRUE;
                //EHSH_LOG_INFO("Got DEVMAN PLayer Server OK %s",pDevmanPlayer->szUrl);
                pDevmanPlayer->szXml=EhsHURLget_write_data_buffer(write_data_buffer_struct);//@todo check for memory leak of returned string
            }
            else
            {
                pDevmanPlayer->szXml=NULL;
                status = EHS_FALSE;
                EHSH_LOG_WARNING("Error DEVMAN PLayer Server %s %d",pDevmanPlayer->szUrl,http_no);
            }

            if (status == EHS_TRUE)   // OK
            {
                if (pDevmanPlayer->szXml && (EhsStrlen(pDevmanPlayer->szXml) > 5))   //ignore small chunks
                {

                    /* do nothing here */
                }
                else
                {
                    //EhsTPMutex_lock(EhsTPMutex_devmanPlayerData);
                    //EhsStrcpy(pDevmanPlayer->szXml, "EMPTY_PAGE_RETURNED"); // write it in here for error logging purposes
                    pDevmanPlayer->szXml=EMPTY_PAGE_RETURNED;
                    //EhsTPMutex_unlock(EhsTPMutex_devmanPlayerData);
                }
                /* and deal with retries of the URL in case our preferred is back on line or has changed */
                if (pDevmanPlayer->CurrentURLindex != 0)
                {
                    retry--;
                    if (retry <= 0)   /* We will read and try first choice every so often even when we are the most preferred*/
                    {
                        pDevmanPlayer->CurrentURLindex=-2;/* start at the top of the list */
                        retry = EHS_DEVMAN_PLAYER_RETRY_PERIOD_FIRSTURL;
                    }
                }
            } // code cumbersome to avoid potential null string manipulation
            else   /* We have no response so try the next on the list */
            {
                // - Don't need szXML is only accesses here EhsTPMutex_lock(EhsTPMutex_devmanPlayerData);
                trynext = EHS_TRUE;
                if (pDevmanPlayer->CurrentURLindex <= 0)   /* try a bit harder with the first on the list - only skip to next after a few tries */
                {
                    retrytimes--;
                    if (retrytimes <= 0)
                    {
                        trynext=EHS_TRUE;
                        retrytimes = EHS_DEVMAN_PLAYER_RETRY_TIMES_FIRSTURL;
                    }
                    else
                        trynext = EHS_FALSE; //trigger getting the next on the list.
                }
                if (trynext)
                {
                    /* Go down the list of priorities from the current URL index */
                    pDevmanPlayer->CurrentURLindex++;
                }
                //else EHSH_LOG_INFO("PLayer URL = %s",pDevmanPlayer->szUrl);
                /* Check the return codes and set error flags */
                switch (http_no)
                {
                case 404:
                    pDevmanPlayer->szXml = SERVER_CONNECTION_ERROR;
                    break;
                case 500:
                    pDevmanPlayer->szXml = SERVER_INTERNAL_ERROR;
                    break;
                case 0:
                    pDevmanPlayer->szXml = INTERNAL_ERROR;
                    break;
                default:
                    pDevmanPlayer->szXml = SERVER_OTHER_ERROR;
                }
                // - Don't need - EhsTPMutex_unlock(EhsTPMutex_devmanPlayerData);
            }
            if (!pDevmanPlayer->EhsDevmanPlayerOutCallback)
            {
                EHSH_LOG_ERROR("Callback function has disappeared while restarting probably\n");
                exit(0);
            }

            EhsCallbackQueue_execute(&pDevmanPlayer->EhsDevmanPlayerOutCallback); //Parses the data and spits out any info
            EhsHURLreset_write_data_buffer(write_data_buffer_struct); // reset for the next round
        }
        else     // we're not running so do nothing and fall thru to pole sleep
        {

        }
        //EhsTPMutex_lock(EhsTPMutex_devmanPlayerData);
        iSleepCount = pDevmanPlayer->iPingPeriod;
        //EhsTPMutex_unlock(EhsTPMutex_devmanPlayerData);

        if (iSleepCount < 5)
            iSleepCount = 5; // always more than 5 seconds between pings
        for (int i = 0; i < iSleepCount / 2; i++)
        {
            if (*bNewSodlFlagRef == EHS_TRUE)   //@todo bNewSodlFlagRef - looks strange - should use a function call rather than global?!
            {
                bDostuff = EHS_FALSE; //EHS_THREAD_END; //PPP: this must be a return %^&^%&*ing Macros!
                break; // get out quick to stop the loop
            }
            EhsSleep(EHS_TIME_s(1)); // sleep for 2s - this is fast enough to exit befoe we are killed off
            // & avoid excessive poling processing.
            if (*bNewSodlFlagRef == EHS_TRUE)
            {
                break;
            }
        }
    } /* end of while loop */
    curl_easy_cleanup(curl);
    EhsHURLfree_write_data_buffer(write_data_buffer_struct);

    if (PostString) EhsHMem_tempFree(PostString);

error:
curl_init_error:
    Ehs_FB_ThreadComplete();
    EhsHThread_exit();
}

/**
 * Define the identify function.
 */
EHS_FB_IDENTIFY_FUNCTION(DevmanPlayer)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(struct EhsFbDevmanPlayerStruct);
}

/*
 * Loads saved state into function block data.
 */
void LoadStateFromFile(struct EhsFbDevmanPlayerStruct* pDevmanPlayer)
{
    ehs_FILE * file;
    if (file = Ehs_UserFopen("configs/devman-player/PlayListURL.cfg", "r"))
    {
        EhsFscanf(file, "%[^\n]", pDevmanPlayer->szPlaylistURL);
        EhsCopyFileNameFromURL(pDevmanPlayer->szPlaylistFile,
                               pDevmanPlayer->szPlaylistURL);
        EhsFclose(file);
    }
    else
    {
        /* Do nothing */
    }
    if (file = Ehs_UserFopen("configs/devman-player/PingPeriod.cfg", "r"))
    {
        EhsFscanf(file, "%d", &pDevmanPlayer->iPingPeriod);
        EhsFclose(file);
    }
    /*
     * Deprecated for the standard devman server list
     if (file = Ehs_UserFopen("configs/devman-player/url.cfg", "r")) {
    	EhsFscanf(file, "%s", pDevmanPlayer->szUrl);
    	EhsFclose(file);
    }
    / * we don't do this for commands in JSON - device and app dependent so done in apps
    if (file = Ehs_UserFopen("configs/devman-player/Player.cfg", "r")) {
    	EhsFscanf(file, "%d", &pDevmanPlayer->jsonPlayerParameters);
    	pDevmanPlayer->bMediaParmChange=EHS_TRUE;
    	EhsFclose(file);
    }
    if (file = Ehs_UserFopen("configs/devman-player/Channels.cfg", "r")) {
    	EhsFscanf(file, "%d", &pDevmanPlayer->nChannels);
    	pDevmanPlayer->bMediaParmChange=EHS_TRUE;
    	EhsFclose(file);
    }
    */

    if ( (file = Ehs_UserFopen("configs/devman-player/Mute.cfg", "r")) )
    {
        EhsFscanf(file, "%d", &pDevmanPlayer->nMute);
        pDevmanPlayer->bMuteParmChange=EHS_TRUE;
        EhsFclose(file);
    }


    if ( (file = Ehs_UserFopen("configs/devman-player/PlayMode.cfg", "r")) )
    {
        EhsFscanf(file, "%d", &pDevmanPlayer->nPlayMode);
        EhsFclose(file);
    }

    if ( (file = Ehs_UserFopen("configs/devman-player/MediaSrcUrl.cfg", "r")) )
    {
        EhsFscanf(file, "%s", pDevmanPlayer->szMediaSrcUrl);
        EhsFclose(file);
    }
}

/**
 * Initialise devman player.
 *
 */
EHS_FB_INIT_FUNCTION(DevmanPlayer)
{
    ehs_FILE * file;
    struct EhsFbDevmanPlayerStruct* pDevmanPlayer =
        (struct EhsFbDevmanPlayerStruct *) EHS_FB_INIT_CONTEXT;
    ehs_char szBuff[EHS_STRING_LENGTH_MAX] = { '\0' }; /* buffer for data to the server     */
    const ehs_char* pBuff = EHS_FB_INIT_PARAMETERS;//&szBuff[0];
    ehs_uint8 iTemp;
    /* Set some defaults incase we miss soething */
    pDevmanPlayer->bCheckingFlag = 0; /** Poll server */
    pDevmanPlayer->szUrl[0] = '\0'; /* user definable */
    pDevmanPlayer->szUrl_parameter[0] = '\0'; /* user definable */
    pDevmanPlayer->szUrl_input[0] = '\0'; /* user definable */
    pDevmanPlayer->iPingPeriod = 30; /* user definable */
    pDevmanPlayer->szPlaylistURL[0] = '\0'; /* requested URL to download from portal  */
    pDevmanPlayer->szPlaylistURLReport[0] = '\0'; /* URL reported by app */
    pDevmanPlayer->szPlaylistFile[0] = '\0';
    pDevmanPlayer->szLocalMediaPath[0] = '\0'; /* @todo this is not actually uses - convert to log info to send??? */
    pDevmanPlayer->szMediaSrcUrl[0] = '\0';
    pDevmanPlayer->bUseDevmanCoreUrl = 0; /* user definable */
    EhsStrcpy(pDevmanPlayer->jsonPlayerParameters,""); /* This is platform dependent - we will not set a default main output here!*/
    pDevmanPlayer->bPlayListLoadAtInit = EHS_FALSE; /* This flag is used to kick start the player when a playlist has been downloaded. //@todo this should possibly have a seperate outout trigger so that the content download doesn't re-occur. */
    pDevmanPlayer->bdoPlayListLoad = EHS_FALSE;
    pDevmanPlayer->bResetingMedia = EHS_FALSE;
    pDevmanPlayer->bMediaParmChange=EHS_FALSE; // Set this at init if volume or channels has been read to allow this to be asserted
    pDevmanPlayer->bMuteParmChange=EHS_FALSE; // Set this at init if a mute state has been read from the state info
    pDevmanPlayer->nMute = EHS_FALSE;
    pDevmanPlayer->nPlayerState = 0; /* not playing */
    pDevmanPlayer->nChannels = 2; // Deprecated  - all information is now stored in JSON for various audio channels
    pDevmanPlayer->nPlayMode = 0; /* normal play */
    /* Dynamic variables */
    pDevmanPlayer->iPlayerMode = 0;
    pDevmanPlayer->szCurrentTrack[0] = '\0';
    pDevmanPlayer->szTrackStartTime[0] = '\0';
    pDevmanPlayer->iCurrentTrackDuration = 0; // @todo check 16 bit is correct
    EhsStrcpy(pDevmanPlayer->jsonPlayerInfo,""); /* default input measured volume */
    //EhsCallbackQueueEntryType xCallbackOut;		/**< used to callback out when xml is returned */
    pDevmanPlayer->szXml = NULL; // allocated later
    pDevmanPlayer->CurrentURLindex=-2; /*Start Looking at the input port URL */
    pDevmanPlayer->nHTTPReturnNo=0;
    /* set the static defaults - maybe overwritten by persistane user configs */
    pBuff = EhsGetWordFromString(pDevmanPlayer->szUrl, pBuff); //We will read in the default value the app requests and use this if nothing else comes along.
    EhsStrcpy(pDevmanPlayer->szUrl_parameter, pDevmanPlayer->szUrl); //Make a copy in case we want to revert
    pBuff = EhsGetWordFromString(pDevmanPlayer->szLocalMediaPath, pBuff); //@todo this paramter is not currently used and shoud not be written into this place!! is not the playlist URL this is the local path
    pBuff = EhsGetUint16FromString(&pDevmanPlayer->iPingPeriod, pBuff);

    /* If this is the first run then create the directories for any updates */
    if (!Ehs_User_PathExists("configs"))
        Ehs_UserMkdir("configs");
    if (!Ehs_User_PathExists("configs/devman-player"))
        Ehs_UserMkdir("configs/devman-player");

    // @todo Write EhsGetBoolFromString function
    pBuff = EhsGetUint8FromString(&iTemp, pBuff);
    pDevmanPlayer->bUseDevmanCoreUrl = (ehs_bool) iTemp;
    pBuff = EhsGetUint8FromString(&iTemp, pBuff);
    pDevmanPlayer->bCheckingFlag = (ehs_bool) iTemp; // Start on Init parameter
    pBuff = EhsGetUint8FromString(&iTemp, pBuff); // read in the flag to assert remebered info at start time.
    pDevmanPlayer->bPlayListLoadAtInit = (ehs_bool) iTemp; // assert last downloaded parameters on data lines (NO EVENT ASSERTED PLEASE!)

    /* check if a config file for any saved state - if we are init-start enabled. */
    if (pDevmanPlayer->bPlayListLoadAtInit)
    {
        LoadStateFromFile(pDevmanPlayer);
        pDevmanPlayer->bdoPlayListLoad = EHS_TRUE;
    }

    pDevmanPlayer->iPlayerMode = 0;
    EhsStrcpy(pDevmanPlayer->szCurrentTrack, "");
    EhsStrcpy(pDevmanPlayer->szTrackStartTime, "");
    pDevmanPlayer->iCurrentTrackDuration = 0;

    /* put this item into the out devman player callback queue */
    pDevmanPlayer->EhsDevmanPlayerOutCallback = NULL;
    /*
    */
    EhsCallbackQueue_register(&pDevmanPlayer->EhsDevmanPlayerOutCallback,
                              EHS_FB_RUN_NAME(DevmanPlayer_out),
                              EHS_FB_INIT_CALLBACK_FUNCTION_INSTANCE(PORT_CALLBACK_OUT),
                              &pDevmanPlayer->xCallbackOut);
    EHS_FB_START_THREAD(DevmanPlayer_thread, -90);

    /* Create incoming json pass thru data list */
    pDevmanPlayer->pIncomingJsonAppPassThruDataList = ehs_create_list();
    /* Create outgoing json pass thru data list */
    pDevmanPlayer->pOutgoingJsonAppPassThruDataList = ehs_create_json_list();

    return EHS_TRUE; /* initialisation always succeeds */
}

EHS_FB_DESTROY_FUNCTION(DevmanPlayer)
{
    EHSH_LOG_INFO("DevmanPlayer-DESTROY\n");
    struct EhsFbDevmanPlayerStruct* pDevmanPlayer =
        (struct EhsFbDevmanPlayerStruct *) EHS_FB_RUN_CONTEXT;
    ehs_free_list(pDevmanPlayer->pIncomingJsonAppPassThruDataList);
    ehs_free_json_list(pDevmanPlayer->pOutgoingJsonAppPassThruDataList);
}

/**
 * Starts timer for checking if not already started by the default static parameter
 */
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DevmanPlayer_start_checking)
{
    struct EhsFbDevmanPlayerStruct* pDevmanPlayer =
        (struct EhsFbDevmanPlayerStruct *) EHS_FB_RUN_CONTEXT;

    pDevmanPlayer->bCheckingFlag = 1;
    EhsTPMutex_lock(EhsTPMutex_devmanPlayerData);

    LoadStateFromFile(pDevmanPlayer); /* called unconditionally - for the case where start on it is not set: todo - we might want a different flag to separate loading media data from persistance on any start type */
    EhsStrcpy(EHS_FB_OUT_S(0), pDevmanPlayer->szUrl);
    EhsTPMutex_unlock(EhsTPMutex_devmanPlayerData);

    EHS_FB_FINISH(PORT_FINISH_START);
    EHS_FB_FINISH(2);

}

/**
 * Stops timer for checking
 */
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DevmanPlayer_stop_checking)
{
    EhsTPMutex_lock(EhsTPMutex_devmanPlayerData);
    struct EhsFbDevmanPlayerStruct* pDevmanPlayer =
        (struct EhsFbDevmanPlayerStruct *) EHS_FB_RUN_CONTEXT;

    pDevmanPlayer->bCheckingFlag = 0;
    EhsTPMutex_unlock(EhsTPMutex_devmanPlayerData);
    EHS_FB_FINISH(PORT_FINISH_STOP);
}

/**
 *
 */

EHS_GLOBAL EHS_FB_RUN_FUNCTION(DevmanPlayer_change_url)
{
    ehs_FILE * file;
    EhsTPMutex_lock(EhsTPMutex_devmanPlayerData);
    struct EhsFbDevmanPlayerStruct* pDevmanPlayer =
        (struct EhsFbDevmanPlayerStruct *) EHS_FB_RUN_CONTEXT;
    if (EHS_FB_IN_CONNECTED(0))
    {
        EhsStrncpy(pDevmanPlayer->szUrl, EHS_FB_IN_S(0), EHS_STRING_LENGTH_MAX);
        EhsStrcpy(pDevmanPlayer->szUrl_input, pDevmanPlayer->szUrl); /* make a backup copy if we need to revert back from the failover list */
        pDevmanPlayer->CurrentURLindex = -2; // This is the one to try next!
        /* We don't want to save this permanently to a save in file
        	if (!EhsHDevmanAddURLtoHeadList(EHS_DEVMAN_PLAYERURLS,pDevmanPlayer->szUrl)) {
        		EHSH_LOG_ERROR("Couldn't save the new devman player URL");
        	}
        	else {
        		EhsHDevmanRemoveURLSFromList(EHS_DEVMAN_PLAYERURLS,EHS_DEVMAN_PLAYER_MAX_SERVER_URLS);
        	}
        	*/
    }
    /* We don't want to write this...
    	if (file = Ehs_UserFopen("configs/devman-player/url.cfg", "w")) {
    		EhsFprintf(file, "%s\n", pDevmanPlayer->szUrl);
    		EhsFclose(file);
    	}
    */
    EhsTPMutex_unlock(EhsTPMutex_devmanPlayerData);
    EhsStrcpy(EHS_FB_OUT_S(0), pDevmanPlayer->szUrl);
    EHS_FB_FINISH(PORT_FINISH_CHANGE);
}

/**
 * This function updates some of the internal parameters of the Devman Polling
 * It also write the configuration data to persistent storage so that any updates are remember at the next start.
 * - it required attention as comments below
 */
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DevmanPlayer_reconfigure)
{
    ehs_FILE * file;
    EhsTPMutex_lock(EhsTPMutex_devmanPlayerData);
    struct EhsFbDevmanPlayerStruct* pDevmanPlayer =
        (struct EhsFbDevmanPlayerStruct *) EHS_FB_RUN_CONTEXT;

    /* Check the paylist path (Not URL) */
    if (EHS_FB_IN_CONNECTED(0))
    {
        if (EhsStrlen( EHS_FB_IN_S(0) ))
        {
            EhsStrcpy(pDevmanPlayer->szLocalMediaPath, EHS_FB_IN_S(0)); //@todo this doesn't do anything!
            // - this doesn't include the local path name EhsCopyFileNameFromURL(pDevmanPlayer->szPlaylistFile,pDevmanPlayer->szPlaylistURL);
        }
    }
    if (EHS_FB_IN_CONNECTED(1))
        pDevmanPlayer->iPingPeriod = (ehs_uint16) EHS_FB_IN_I(1);
    // write data to a file
    /*
     *
     if (file = Ehs_UserFopen("configs/devman-player/PlayListURL.cfg", "w")) {
    	EhsFprintf(file, "%s\n", pDevmanPlayer->szPlaylistURL);
    	EhsFclose(file);
    }
     */
    if (file = Ehs_UserFopen("configs/devman-player/PingPeriod.cfg", "w"))
    {
        EhsFprintf(file, "%s\n", pDevmanPlayer->iPingPeriod);
        EhsFclose(file);
    }

    EhsTPMutex_unlock(EhsTPMutex_devmanPlayerData);
    EHS_FB_FINISH(PORT_FINISH_RECONFIGURE);
}


/**
 * Reset all media source data sent from the server and remove persistent memory of it
 */
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DevmanPlayer_reset)
{
    struct EhsFbDevmanPlayerStruct* pDevmanPlayer =
        (struct EhsFbDevmanPlayerStruct *) EHS_FB_RUN_CONTEXT;

    EhsStrcpy(pDevmanPlayer->szMediaSrcUrl,"");
    Ehs_UserRm("configs/devman-player/MediaSrcUrl.cfg");

    EhsStrcpy(pDevmanPlayer->szPlaylistFile,"");
    EhsStrcpy(pDevmanPlayer->szPlaylistURL,"");
    Ehs_UserRm("configs/devman-player/PlayListURL.cfg");

    pDevmanPlayer->nPlayMode = 0;
    Ehs_UserRm("configs/devman-player/PlayMode.cfg");

    pDevmanPlayer->bResetingMedia = EHS_TRUE; /* cause all the data outputs to be cleared at the next update event */
    //pDevmanPlayer->nMute=FALSE; /* don't need to change this - events are only generated on change - and we don't want to do this

    /* we don't touch these:
    pDevmanPlayer->nVolume 		: configs/devman-player/Volume.cfg
    pDevmanPlayer->nChannels 	: configs/devman-player/Channels.cfg
    pDevmanPlayer->nMute 		: configs/devman-player/Mute.cfg"
    */

}
/**
 * Retrieves currently playing media (track) data from the app to send to the devman server
 */
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DevmanPlayer_track_changed)
{
    EhsTPMutex_lock(EhsTPMutex_devmanPlayerData);
    struct EhsFbDevmanPlayerStruct* pDevmanPlayer =
        (struct EhsFbDevmanPlayerStruct *) EHS_FB_RUN_CONTEXT;

    if (EHS_FB_IN_CONNECTED(0))
        pDevmanPlayer->iPlayerMode = (ehs_uint32) EHS_FB_IN_I(0);
    else
        pDevmanPlayer->iPlayerMode = 0;

    if (EHS_FB_IN_CONNECTED(1))
        EhsStrncpy(pDevmanPlayer->szCurrentTrack, EHS_FB_IN_S(1),
                   EHS_STRING_LENGTH_MAX);
    else
        EhsStrcpy(pDevmanPlayer->szCurrentTrack, "");

    if (EHS_FB_IN_CONNECTED(2))
        EhsStrncpy(pDevmanPlayer->szTrackStartTime, EHS_FB_IN_S(2),
                   EHS_STRING_LENGTH_MAX);
    else
        EhsStrcpy(pDevmanPlayer->szTrackStartTime, "");

    if (EHS_FB_IN_CONNECTED(3))
        pDevmanPlayer->iCurrentTrackDuration = (ehs_uint32) EHS_FB_IN_I(3);
    else
        pDevmanPlayer->iCurrentTrackDuration = 0;
    /* New This is for identifyback to Devman what the current Playlist is (can be different to the suggested). */
    if (EHS_FB_IN_CONNECTED(4))
    {
        EhsStrcpy(pDevmanPlayer->szPlaylistURLReport, EHS_FB_IN_S(4));//,EHS_STRING_LENGTH_MAX);
    }
    if (EHS_FB_IN_CONNECTED(5))
        pDevmanPlayer->nPlayerState = (ehs_uint32) EHS_FB_IN_I(5);
    else
        pDevmanPlayer->nPlayerState = 0;

    if (EHS_FB_IN_CONNECTED(6))
        EhsStrcpy(pDevmanPlayer->jsonPlayerInfo, EHS_FB_IN_S(6));
    //else
    //	EhsStrcpy(pDevmanPlayer->szMiscSysInfo, "");

    EhsTPMutex_unlock(EhsTPMutex_devmanPlayerData);
    // @todo find out if there's a finish port for this function as its not in the spec.
}

/**
 *
 */
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DevmanPlayer_pass_thru_get_next)
{
    EhsTPMutex_lock(EhsTPMutex_devmanPlayerData);
    struct EhsFbDevmanPlayerStruct* pDevmanPlayer = (struct EhsFbDevmanPlayerStruct *) EHS_FB_RUN_CONTEXT;
    EhsLinkedList* list = pDevmanPlayer->pIncomingJsonAppPassThruDataList;
    ehs_free_node( ehs_pop_back(list) ); // remove last node
    if(!ehs_is_empty(list))  // output next element
    {
        EhsNode* node = list->back;
        if((node->data_size + 1) < EHS_STRING_LENGTH_MAX)
        {
            EhsStrncpy(EHS_FB_OUT_S(0),node->data,node->data_size);
            (EHS_FB_OUT_S(0))[node->data_size]='\0';
            EHS_FB_FINISH(1);
        }
        else
        {
            EHSH_LOG_ERROR("JSON pass thru data size exceeds the EHS_STRING_LENGTH_MAX size limit (%d).\n", EHS_STRING_LENGTH_MAX);
        }
    }
    EhsTPMutex_unlock(EhsTPMutex_devmanPlayerData);
}

/**
 *
 */
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DevmanPlayer_pass_thru_send_next)
{
    EhsTPMutex_lock(EhsTPMutex_devmanPlayerData);
    struct EhsFbDevmanPlayerStruct* pDevmanPlayer = (struct EhsFbDevmanPlayerStruct *) EHS_FB_RUN_CONTEXT;
    if (EHS_FB_IN_CONNECTED(0))
    {
        EhsJsonList* json_list = pDevmanPlayer->pOutgoingJsonAppPassThruDataList;
        ehs_char* jsonData = EHS_FB_IN_S(0);
        ehs_push_json(json_list, jsonData, EhsStrlen(jsonData));
    }
    EhsTPMutex_unlock(EhsTPMutex_devmanPlayerData);
}

/**
 * This is the internal function fired by the timer that reads devman data.
 * It is asserted data to ports and back to the devman server
 */
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DevmanPlayer_out)
{
    ehs_FILE * file;

    ehs_char pTempStr[EHS_STRING_LENGTH_MAX];
    ehs_char* zTempStr2 = NULL;
    int bSome, nNewPlaylistFromDevMan = EHS_FALSE;
    //ehs_bool bPlayerParmsChanged=EHS_FALSE; // todo this can be removed now we don't cache data for the player.
    ehs_bool bPlayerMuteChanged=EHS_FALSE;
    EhsTPMutex_lock(EhsTPMutex_devmanPlayerData); // todo!! need to make these  mutexes more fine grained - we are doing some file ops in here...
    static ehs_bool first_time = 1;
    struct EhsFbDevmanPlayerStruct* pDevmanPlayer = (struct EhsFbDevmanPlayerStruct *) EHS_FB_RUN_CONTEXT;
    ehs_uint16 XML_length;


    if  (!pDevmanPlayer || !pDevmanPlayer->szXml)
    {
        //EHSH_LOG_ERROR("DEVMAN PLAYER xml buffer not allocated");
        EhsTPMutex_unlock(EhsTPMutex_devmanPlayerData);
        return;
    }
    XML_length = EhsStrlen(pDevmanPlayer->szXml);
    /********* PARSING ROUTINE *************/
    if (XML_length > 5 )
    {
        bSome = 0;
        zTempStr2 = strstr(pDevmanPlayer->szXml, "New_Playlist");
        if (zTempStr2)
        {

            /* Note this is not useful on it own so doesn't set some*/
            if (0 == getAttributeValue("mediaSrcURL=", zTempStr2, pTempStr))
            {
                EhsStrncpy(pDevmanPlayer->szMediaSrcUrl, pTempStr,
                           EHS_STRING_LENGTH_MAX);
                // return the pointer to within the string provided.
                bSome = 1; /* We probably don't need to save this is this is only used during a download which we are assuming isn't interrupted */
                //@todo should the init check all current data is present and reload it?
                if (file = Ehs_UserFopen("configs/devman-player/MediaSrcUrl.cfg", "w"))
                {
                    EhsFprintf(file, "%s\n", pDevmanPlayer->szMediaSrcUrl);
                    EhsFclose(file);
                }
            }
            /* We do this one after the MediaSrcURL so we can concatenate if the playlist is not fully qualified. */
            if (0 == getAttributeValue("playlist=", zTempStr2, pTempStr))
            {
                if (EhsCopyFileNameFromURL(pDevmanPlayer->szPlaylistFile,pTempStr))   // we have a fully qualified URL
                {
                    EhsStrncpy(pDevmanPlayer->szPlaylistURL, pTempStr,EHS_STRING_LENGTH_MAX);
                    nNewPlaylistFromDevMan=EHS_TRUE;

                }
                else     //create a fully qualified URL from the media source URL - if it doesn't exist in the XML we get the remembered value or worst case is that we just get the file name
                {
                    EhsStrncpy(pDevmanPlayer->szPlaylistURL,pDevmanPlayer->szMediaSrcUrl, EHS_STRING_LENGTH_MAX);
                    EhsStrcat(pDevmanPlayer->szPlaylistURL, "/"); // in case one wasn't added
                    EhsStrcat(pDevmanPlayer->szPlaylistURL, pTempStr); //add the filename onto theURL @todo Check white space is chomped!
                    /* and update the FIle name version */
                    EhsStrcpy(pDevmanPlayer->szPlaylistFile,pTempStr);
                    nNewPlaylistFromDevMan=EHS_TRUE;
                }
                /* output the data */
                bSome = 1;
                if (file = Ehs_UserFopen("configs/devman-player/PlayListURL.cfg", "w"))
                {
                    EhsFprintf(file, "%s\n", pDevmanPlayer->szPlaylistURL);
                    EhsFclose(file);
                }
            }

            /* else Don't change EhsStrncpy(EHS_FB_OUT_S(1),"\0",1); */
        }

        if (zTempStr2 = strstr(pDevmanPlayer->szXml, "NewDevmanPlayerURL"))
        {
            if (0 == getAttributeValue("url=", zTempStr2, pTempStr))
            {

                EhsStrncpy(pDevmanPlayer->szUrl, pTempStr,EHS_STRING_LENGTH_MAX);
                //permanently save in file
                if (EhsHDevmanAddURLtoHeadList(EHS_DEVMAN_PLAYERURLS,pDevmanPlayer->szUrl))
                {
                }
                /* Depricated for devman managed server list
                if (file = Ehs_UserFopen("configs/devman-player/url.cfg", "w")) {
                	EhsFprintf(file, "%s\n", pDevmanPlayer->szUrl);
                	EhsFclose(file);
                } else
                	printf(
                			"ERROR: Cant Write to configs/devman-player/url.cfg\n");
                			*/
                /* ouput to environments */
                EhsStrcpy(EHS_FB_OUT_S(11), pDevmanPlayer->szUrl);
                EHS_FB_FINISH(9); // Set the Set Url Complete
            }
        }
        if (zTempStr2 = strstr(pDevmanPlayer->szXml, "RemovePlaylist"))
        {
            if (0 == getAttributeValue("playlist=", zTempStr2, pTempStr))
            {
                EhsStrncpy(EHS_FB_OUT_S(2), pTempStr, EHS_STRING_LENGTH_MAX);
                EHS_FB_FINISH(2);
            }
        }
        if (zTempStr2 = strstr(pDevmanPlayer->szXml, "RemoveOldContent"))
        {
            if (0 == getAttributeValue("playlist=", zTempStr2, pTempStr))
            {
                ;
                //EhsStrncpy(EHS_FB_OUT_S(8), pTempStr, EHS_STRING_LENGTH_MAX);
            }
            EHS_FB_FINISH(3);
        }
        if (strstr(pDevmanPlayer->szXml, "RemoveAllContent"))
        {
            EHS_FB_FINISH(4);
        }
        if (zTempStr2 = strstr(pDevmanPlayer->szXml,"Override_Content_Directory"))
        {
            EHSH_LOG_INFO("Received Override Signal %s\n",zTempStr2);
            if (0 == getAttributeValue("path=", zTempStr2, pTempStr))
            {
                /* Override is not persistent - If needed the logic of undoing override needs to be defined (e.g. stop overriding on any  next new Playlist).
                 }
                 */
                EhsStrncpy(EHS_FB_OUT_S(3), pTempStr, EHS_STRING_LENGTH_MAX);
                EHS_FB_FINISH(5);
            }
        }
        if (zTempStr2 = strstr(pDevmanPlayer->szXml, "Set_Playmode"))
        {

            if (0 == getAttributeValue("mode=", zTempStr2, pTempStr))
            {
                pDevmanPlayer->nPlayMode = atoi(pTempStr);
                if (file = Ehs_UserFopen("configs/devman-player/PlayMode.cfg","w"))
                {
                    EhsFprintf(file, "%d\n", pDevmanPlayer->nPlayMode); //@tdp this shoud be fired by start of it is active
                    EhsFclose(file);
                }
                //* @todoIDF NEEDED*/ EhsStrncpy(EHS_FB_OUT_I(9),pTempStr,EHS_STRING_LENGTH_MAX);
                //*EHS_FB_FINISH(9);

            }
        }
        bSome = 0;
#ifdef INX_OLD_VERSION_NOT_USED
        if (zTempStr2 = strstr(pDevmanPlayer->szXml, "SetPlayerParameters"))
        {
            if (0 == getAttributeValue("volume=", zTempStr2, pTempStr))
            {
                EhsStrcpy(pDevmanPlayer->jsonPlayerParameters,pTempStr);
                if (file = Ehs_UserFopen("configs/devman-player/Volume.cfg","w"))
                {
                    EhsFprintf(file, "%s\n", pDevmanPlayer->jsonPlayerParameters); //@tdp this shoud be fired by start of it is active
                    EhsFclose(file);
                }
                EhsStrcpy(EHS_FB_OUT_S(4),pDevmanPlayer->jsonPlayerParameters);
                bSome = 1;
            }
            if (0 == getAttributeValue("channels=", pDevmanPlayer->szXml,
                                       pTempStr))
            {
                pDevmanPlayer->nChannels = atoi(pTempStr);
                if (file = Ehs_UserFopen("configs/devman-player/Channels.cfg","w"))
                {
                    EhsFprintf(file, "%d\n", pDevmanPlayer->nChannels); //@tdp this shoud be fired by start of it is active
                    EhsFclose(file);
                }
                EHS_FB_OUT_I(5) = atoi(pTempStr);
                bSome = 1;
            }
            if (bSome)   // @todo don't need this? Set flag to say we've got new parameters for things we have found at start up.
            {
                //bPlayerParmsChanged=EHS_TRUE;
                EHS_FB_FINISH(6);
            }
        }
#else
        if (zTempStr2 = Ehs_ReadXMLTag(pDevmanPlayer->szXml, "SetPlayerParameters"))
        {

            Ehs_CopyXMLTagElement(pDevmanPlayer->jsonPlayerParameters, zTempStr2, EHS_STRING_LENGTH_MAX, EHS_TRUE);
            /* We don't do this any more - app dependent - just send out the command and let the apps store what's needed
            if (file = Ehs_UserFopen("configs/devman-player/Players.cfg","w")) {
            	EhsFprintf(file, "%s\n", pDevmanPlayer->jsonPlayerParameters);
            	EhsFclose(file);
            }
            */
            EhsStrcpy(EHS_FB_OUT_S(4),pDevmanPlayer->jsonPlayerParameters);
            EHS_FB_OUT_I(5) = 2 ;// number of channels?? todo - we can remove this port to save space. We Probably just want to bin this output as this sort of thing will go in the JSON instead.

            //bPlayerParmsChanged=EHS_TRUE;
            EHS_FB_FINISH(6);
        }

#endif
        /* Receiving and buffering JSON data from Devman */
        if (zTempStr2 = Ehs_ReadXMLTag(pDevmanPlayer->szXml, "jsonAppPassThruData"))
        {
            ehs_uint32 jsonBufferSize = EhsStrlen(zTempStr2) + 1;
            ehs_char* jsonBuffer = (ehs_char*)malloc(jsonBufferSize);
            Ehs_CopyXMLTagElement(jsonBuffer, zTempStr2, jsonBufferSize, EHS_TRUE);
            ehs_uint32 jsonDataSize = EhsStrlen(jsonBuffer);
            if(jsonDataSize > 0)
            {
                EhsLinkedList* list = pDevmanPlayer->pIncomingJsonAppPassThruDataList;
                ehs_uint32 json_count = ehs_push_json_array_string(list, jsonBuffer, jsonDataSize);
                EHSH_LOG_INFO("PASSTHROUGH - (%d) (%d) (%s)",jsonDataSize,json_count,jsonBuffer);
                if(list->count > 0 && list->count == json_count)  // output first element
                {
                    EhsNode* node = list->back;
                    if((node->data_size + 1) < EHS_STRING_LENGTH_MAX)
                    {
                        EhsStrncpy(EHS_FB_OUT_S(12),node->data,node->data_size);
                        (EHS_FB_OUT_S(12))[node->data_size]='\0';
                        EHS_FB_FINISH(11);
                    }
                    else
                    {
                        EHSH_LOG_ERROR("JSON pass thru data size exceeds the EHS_STRING_LENGTH_MAX size limit (%d).\n", EHS_STRING_LENGTH_MAX);
                    }
                }
            }
            free(jsonBuffer);
        }

        if (zTempStr2 = strstr(pDevmanPlayer->szXml, "Mute"))
        {
            if (0 == getAttributeValue("state=", zTempStr2, pTempStr))
            {
                pDevmanPlayer->nMute = atoi(pTempStr);
                if (file = Ehs_UserFopen("configs/devman-player/Mute.cfg", "w"))
                {
                    EhsFprintf(file, "%d\n", pDevmanPlayer->nMute); //@tdp this shoud be fired by start of it is active
                    EhsFclose(file);
                }
                EHS_FB_OUT_B(6) = pDevmanPlayer->nMute;
                bPlayerMuteChanged=EHS_TRUE;
                EHS_FB_FINISH(7); //@todo this is all messy - all the xml parsers hould simply write to the object struct and flag a change, then assertions made at the end including the init value flag
            }
        }
    }
    /**** END OF PARSING ROUTINE ********************/
    /* If we have loaded info at init then we need to assert this at activate time up (or at least first run of poling */

    if (pDevmanPlayer->bdoPlayListLoad || pDevmanPlayer->bResetingMedia || nNewPlaylistFromDevMan)   // if no new playlist and we already have one then retry playing it.
    {
        if (EhsStrlen(pDevmanPlayer->szMediaSrcUrl) > 0)
        {
            EhsStrncpy(EHS_FB_OUT_S(1), pDevmanPlayer->szMediaSrcUrl,
                       EHS_STRING_LENGTH_MAX);
            EhsStrncpy(EHS_FB_OUT_S(8), EhsGetPathFromURL(pDevmanPlayer->szMediaSrcUrl,0), EHS_STRING_LENGTH_MAX);
        }
        else
        {
            EhsStrcpy(EHS_FB_OUT_S(1), "");
            EhsStrcpy(EHS_FB_OUT_S(8), "");
        }
        EhsStrncpy(EHS_FB_OUT_S(0), pDevmanPlayer->szPlaylistURL,EHS_STRING_LENGTH_MAX);
        EhsStrncpy(EHS_FB_OUT_S(9), pDevmanPlayer->szPlaylistFile,EHS_STRING_LENGTH_MAX);

        /* and all the other static parameters - note we might be doing these twice, but we don't care? @todo do we care?*/
        if (nNewPlaylistFromDevMan)
        {
            EHS_FB_FINISH(1); // fire the new playlist port as we read one at init
            //EHSH_LOG_INFO("PBB new playlist %s",EHS_FB_OUT_S(0));
        }
#ifdef EHS_ENABLE_EVENT_FOR_LOADATINIT
        if (pDevmanPlayer->bdoPlayListLoad) EHS_FB_FINISH(9); // assert the loaded from disk event
#endif
        pDevmanPlayer->bResetingMedia  = EHS_FALSE;
        pDevmanPlayer->bdoPlayListLoad = EHS_FALSE;

    }
    /* this is only for read from file stuff - which we don't do with the JSON player commands any more
    if (pDevmanPlayer->bMediaParmChange) { //@todo needs a tidy with the changed flags
    	//EHSH_LOG_INFO("Start At Init\n");
    	if (!bPlayerParmsChanged) { // make sure we don't do twice if we get new data at init..
    		EHS_FB_OUT_I(5) = pDevmanPlayer->nChannels; // Should deprecate this and use the JSON instead
    		EhsStrcpy(EHS_FB_OUT_S(4), pDevmanPlayer->jsonPlayerParameters);
    		EHS_FB_FINISH(6); // fire the done events when all is OK.
    	}
    	pDevmanPlayer->bMediaParmChange=EHS_FALSE;
    }
    */
    if (pDevmanPlayer->bMuteParmChange)
    {
        if (!bPlayerMuteChanged)   //this should stop
        {
            EHS_FB_OUT_B(6) = pDevmanPlayer->nMute;
            EHS_FB_FINISH(7);
        }
        pDevmanPlayer->bMuteParmChange=EHS_FALSE;
    }
    //@todo this crashes when the app is stopped and started again with iab specially when run under GDB.
    if (pDevmanPlayer->szXml)
    {
        EhsStrncpy(EHS_FB_OUT_S(10), pDevmanPlayer->szXml, EHS_STRING_LENGTH_MAX); // ouptut some debug
        (EHS_FB_OUT_S(10))[EHS_STRING_LENGTH_MAX - 2] = '\0';

        if ((EhsStrcmp(pDevmanPlayer->szXml, "ERROR_INTERNAL1") == 0)
                || (EhsStrcmp(pDevmanPlayer->szXml, "ERROR_INTERNAL2") == 0))
        {
            // @todo: implement integer error output
            EHS_FB_OUT_I(7) = 10; // enumeration type for no server connection
        }
        else if (EhsStrcmp(pDevmanPlayer->szXml, "ERROR_NETWORK") == 0)
        {
            // @todo: implement integer error output
            EHS_FB_OUT_I(7) = pDevmanPlayer->nHTTPReturnNo; // enumeration type for no server connection
        }
        else     // must be Good data or no data - has been pumped out for inspection!
        {
            EHS_FB_OUT_I(7) = pDevmanPlayer->nHTTPReturnNo; // enumeration type for no server connection
        }
    }
    else EHS_FB_OUT_I(7) = 10;  // enumeration type for no server connection

    EHS_FB_FINISH(8); // guess this is asserted the status event?
    EhsTPMutex_unlock(EhsTPMutex_devmanPlayerData);
}
