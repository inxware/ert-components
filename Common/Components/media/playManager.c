/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

#include "globals.h"
#include "hal-api.h" /* Needed for logging */
#include "playManager.h"

#include "libxml/xmlreader.h"

#ifdef EHS_DEBUG_SMILPARSER
    /* Stack tracing code */
    #ifndef EHS_ANDROID
    #include <execinfo.h>
    #endif
    #include <stdio.h>
    #include <stdlib.h>

    /* Obtain a backtrace and print it to stdout. */
    void
    print_trace (void)
    {
    #ifndef EHS_ANDROID
        void *array[10];
        size_t size;
        char **strings;
        size_t i;

        size = backtrace (array, 10);
        strings = backtrace_symbols (array, size);

        printf ("Obtained %zd stack frames.\n", size);

        for (i = 0; i < size; i++)
            printf ("%s\n", strings[i]);

        free (strings);
    #endif
    }
    /* end of stacj tracing code */
    #define EHS_SMILPARSER_TICK_TIME_US 100000
    #define EHS_SMILPARSER_DEFAULT_START_GRACE 6000000 // 60 second start time @todo should be a block config parameters
    #define EHS_WAITINGFORSTARTACKCOUNTDOWN 50 // 5 seconds
    #error "YOU ARE BUILDING THE DEBUG VERSION _ARE YOU SURE?"
#else
    #ifdef EHS_MINGW
    // Using fast (1ms) iteration time for windows to make revolver more responsive
    #define EHS_SMILPARSER_TICK_TIME_US 1000
    #else
    // 10 ms for our linux and android - as we don't have preicuse timing for these currently, but could test making this faster and no CPU 
    #define EHS_SMILPARSER_TICK_TIME_US 10000 

    #endif

    #define EHS_SMILPARSER_DEFAULT_START_GRACE 20000000 //@todo make this a parameter of the function block.
    #define EHS_WAITINGFORSTARTACKCOUNTDOWN 250 // 5 seconds
    #endif

    #ifdef  EHS_DEBUG_SMILPARSER_EX
    #ifndef EHS_ANDROID
    #define _debug_ID_signalling_printf printf
    #else
    #define _debug_ID_signalling_printf EHSH_LOG_INFO
    #endif
    #else
    #define _debug_ID_signalling_printf(...) //printf
#endif

#define EHS_SMILPARSER_TIMEOUT_ACK_FOR_PARENT_LIST_US 30000000

/* Set this (default set) to use dater clib malloc and not EHS's traced malloc manager */
#define EHS_DONT_USE_EHS_MANAGED_HEAP

EHS_FB_FUNCTIONS_START(PlayManager)
EHS_FB_FUNCTION_ENTRY("Next_Playlist", 0x01, PlayManager_Next_Playlist)
EHS_FB_FUNCTION_ENTRY("Tick", 0x02, PlayManager_Tick)
EHS_FB_FUNCTION_ENTRY("Played", 0x03, PlayManager_Played)
EHS_FB_FUNCTION_ENTRY("Next_URL", 0x04, PlayManager_Next_URL)
EHS_FB_FUNCTION_ENTRY("Got", 0x05, PlayManager_Got)
EHS_FB_FUNCTION_ENTRY("Set_Paths", 0x06, PlayManager_Set_Paths)
EHS_FB_FUNCTION_ENTRY("Started", 0x07, PlayManager_Started)
EHS_FB_FUNCTIONS_END

/* Port Mappings */
/* data outputs */
#define EHS_FB_PLAYMANAGER_TICK_MEDIAFILE 0 // 1
#define EHS_FB_PLAYMANAGER_TICK_CLASS 1 // 2
#define EHS_FB_PLAYMANAGER_TICK_TEXT 2 // 3
#define EHS_FB_PLAYMANAGER_TICK_TYPE 3 // 4
#define EHS_FB_PLAYMANAGER_TICK_ID 4 // 5
#define EHS_FB_PLAYMANAGER_TICK_NEXT_EVENT_START 5 //7
#define EHS_FB_PLAYMANAGER_TICK_NEXT_EVENT_END 6 //7
#define EHS_FB_PLAYMANAGER_TICK_LAYOUT 7 // this is the region layout JSON database set when a new section is started.
#define EHS_FB_PLAYMANAGER_TICK_ASPECTRATIO 8 // this is set when a new region layout is outputetted and is usually used just to switch screen portrait/landscape modes
#define EHS_FB_PLAYMANAGER_TICK_REGION 9 // this is the region assigned for a media object
#define EHS_FB_PLAYMANAGER_TICK_SOUNDLEVEL 10 // SMIL sound Level for an media object
#define EHS_FB_PLAYMANAGER_TICK_FADE 11 //

/* Completes */
#define EHS_FB_PLAYMANAGER_TICK_PLAYNOW 1 // 6
#define EHS_FB_PLAYMANAGER_TICK_PLAYREMOTE 2 //7
#define EHS_FB_PLAYMANAGER_TICK_NEXT_EVENT_CHANGED 3 //7
#define EHS_FB_PLAYMANAGER_TICK_STOPPLAY 4 //7
#define EHS_FB_PLAYMANAGER_TICK_NEWLAYOUT 5

//#define EHS_FB_PLAYMANAGER_NEXT_PLAYLIST_INPATH 1 //-0
#define EHS_FB_PLAYMANAGER_NEXT_PLAYLIST_PLAYLIST_PATH_IN 0 //-1
#define EHS_FB_PLAYMANAGER_NEXT_PLAYLIST_PLAYLIST_PATH_OUT 0 //-2
#define EHS_FB_PLAYMANAGER_NEXT_PLAYLIST_URL 1 //-3
#define EHS_FB_PLAYMANAGER_NEXT_PLAYLIST_LOCAL_FILE 2 //-4
#define EHS_FB_PLAYMANAGER_NEXT_PLAYLIST_ID 3 //-6
#define EHS_FB_PLAYMANAGER_NEXT_PLAYLIST_FILE_COUNT 4
#define EHS_FB_PLAYMANAGER_NEXT_PLAYLIST_FINISH 1 //-7
#define EHS_FB_PLAYMANAGER_NEXT_PLAYLIST_GET 2 //-8
#define	EHS_FB_PLAYMANAGER_NEXT_PLAYLIST_NOMORE 3

#define EHS_TICK_MEDIA_FILE 0 //first output in IDF
#define EHS_TICK_CLASS_INFO 1
#define EHS_TICK_SMIL_TEXT 2
#define EHS_TICK_EVENT_TYPE 3
#define EHS_TICK_PLAY_NOW 1 //first event
#define EHS_TICK_PLAY_REMOTE 2 //second event

#define EHS_FB_PLAYMANAGER_GOT_ID 2
#define EHS_FB_PLAYMANAGER_GOT_URL 0
#define EHS_FB_PLAYMANAGER_GOT_LOCAL 1


#define EHS_FB_PLAYMANAGER_SETPATHS_DL_URL 0
#define EHS_FB_PLAYMANAGER_SETPATHS_DL_LOCAL 1
#define EHS_FB_PLAYMANAGER_SETPATHS_START_MODE 2

#define EHS_NODE_TYPE_ATTRIBUTE_FIELD 2
#define EHS_NODE_TYPE_COMMENT 8
#define EHS_NODE_TYPE_START_ELEMENT 1 //a normal element
#define EHS_NODE_TYPE_END_ELEMENT 15 //a normal element

#define EHS_PLAYMANAGER_TIMER_UNDEFINED 0xFFFFFFFF //@todo we should make this 64bit for the 2038 issue..

/* Thi needs to be ordered from lowest repeat rate to hghest so that parent/child repeats can be managed easily */
typedef enum
{
    EHS_PLAYMANAGER_REPEAT_NONE = 0,	/*yy-mm-dd{T}HH:MM:SS   17 */
    EHS_PLAYMANAGER_REPEAT_YEAR = 1,	/*mm-dd{T}HH:MM:SS      14 */
    EHS_PLAYMANAGER_REPEAT_MONTH = 2,	/*dd{T}HH:MM:SS         11 */
    EHS_PLAYMANAGER_REPEAT_2WEEK = 3,	/*{W1/2}D{T}HH:MM:SS    12  Week 1 or week 2*/
    EHS_PLAYMANAGER_REPEAT_WEEK = 4,	/*D{T}HH:MM:SS          10 */
    EHS_PLAYMANAGER_REPEAT_DAY = 5, 	/*HH:MM:SS              8 */
    EHS_PLAYMANAGER_REPEAT_HOUR = 6, 	/*HH:MM:SS              8 */
    //EHS_PLAYMANAGER_REPEAT_IMMEDIATELY= 7,	/* no date marking is expected */
} EhsPlayManagerRepeatEnum;

/* We won use an enum fr special loop values as these are noramlly just integers */
#define EHS_PLAYMANAGER_LOOP_INDEFINITE -1
#define EHS_PLAYMANAGER_LOOP_NONE 0


//This is used to identify what type of node we are dealing with. Begin means we are in a list - not just the beginning.
typedef enum
{
    EHS_PLAYMANAGER_EVENT_OBJECT = 0,
    EHS_PLAYMANAGER_EVENT_NEWLIST = 1,
    EHS_PLAYMANAGER_EVENT_MIDLIST = 2,
    EHS_PLAYMANAGER_EVENT_END = 3,
    EHS_PLAYMANAGER_EVENT_UNKOWN = 4,
    EHS_PLAYMANAGER_EVENT_NEWDYNAMICLIST = 5
} EhsPlayManagerEventTypeEnum;

/*
 * Used to identify if a list object is to be parsed as seq, par or excl.
 */
typedef enum
{
    EHS_PLAYMANAGER_EVENTLIST_SEQ = 0,
    EHS_PLAYMANAGER_EVENTLIST_SEQREF = 4,
    EHS_PLAYMANAGER_EVENTLIST_PAR = 1,
    EHS_PLAYMANAGER_EVENTLIST_EXCL = 2,
    EHS_PLAYMANAGER_EVENTLIST_UNKOWN = 3
} EhsPlayManagerEventListTypeEnum;

/*
 * This is thype returned by the SMIL parser to indicate to the event manager if the SMIL chunk event should be destroyed or not
 */

typedef enum
{
    EHS_PLAYMANAGER_SMIL_CHUNK_REMOVE = 0,
    EHS_PLAYMANAGER_SMIL_CHUNK_KEEP = 1,
    EHS_PLAYMANAGER_SMIL_CHUNK_ENDED = 2,
    EHS_PLAYMANAGER_SMIL_CHUNK_CANTREAD = 3
} EhsPlayManagerSMILNodeActionEnum;


typedef enum {CLEAR_ALL=0, KEEP_EARLIERTHANNEW=1,KEEP_ALL=2} EhsPlayManagerNewSmilBehavType;
typedef enum {LIST_DEFAULT, LIST_OLD, LIST_NEW} EhsPlayManagerMarkedType;


/*
 * The following is used to record the media files that have been downloaded.
 */
typedef struct EhsPlayManagerMediaLocationStruct
{
    struct EhsPlayManagerMediaLocationStruct *pNext;
    ehs_uint32 id;
    ehs_bool marked;
    ehs_char url[EHS_STRING_LENGTH_MAX]; //TODO:STRINGLENGTH!
    ehs_char localFile[EHS_STRING_LENGTH_MAX]; //TODO:STRINGLENGTH!
} EhsPlayManagerMediaLocation;

typedef struct EhsPlayManagerEventListForParStruct
{
    struct EhsPlayManagerEventListForParStruct *pNext;
    ehs_uint32 ID;
} EhsPlayManagerEventListForParType;

/*
 * Records XML strings of playlist content associated with a partiular event.
 * This is the list of all playable nodes that have been parsed.
 * The structure identifies the following:
 * The event is a repeat event
 * The event is a list and there is more to parse (in pPlaylist)
 * The meta data for the object that has already been parsed.
 */
#define EHS_PLAYMANAGER_MAX_XML_STRINGLENGTH 8096
typedef struct EhsPlayManagerEventStruct
{
    struct EhsPlayManagerEventStruct *pNext; // the  full list of events for iteration
    struct EhsPlayManagerEventStruct *pParent; // list of parents (e.g. to look up parent durations, or expiry times

    time_t eventTime; // this is the start time that is checkedon each clock tick..
    time_t eventEndTime; // we need to keep this for lists and NOW FOR EVERY EVENT THAT HAS ONE!- end times with a copy of the data...
    //time_t startedTime;  // this s the time an event actually starts (e.g. if in a sequence or non-timed parent
    ehs_uint32 eventDuration; // this is a relatve duration time for the event as defined in SMIL in ms.
    ehs_bool bRelativeEndTime; // this can be set to true if the end time is calculated on the fly. The negative case implyinh the end time is an absolute time is however how this is gnerally used
    EhsPlayManagerRepeatEnum repeat; // if the event is not fully qualified (e.g. by date) then this identifies the repeat period which is assumed when course tiem data is not specified.
    EhsPlayManagerEventTypeEnum type; // is this a list begin type, end of list type, or a SINGLE track type (with specific data).
    ehs_bool parent_has_time; /* This is used to indicate that a parent node had time - and therefore normally report should not be called for at this level because it will be done by parent*/
    EhsPlayManagerEventListTypeEnum eventListType; // this is either seq, par, or excl.
    ehs_uint32 playlistSize;
    xmlTextReaderPtr memoryReader; //Reads the in memory playlist we are currently playing

    ehs_char* pPlaylist; // this contains chunks of playlists containing lists that are to be iterated.
    ehs_char* pMediaSource; //this is the source attribute for a media object. populated by the media object parser.
    ehs_char* class; // class this is used by the app to divert stuff around.
    ehs_char* soundLevel; // 0-100 value for per track sound level
    ehs_sint32 fade; // seconds fade time for this specific track
    ehs_char* srcType;// SMIL file specified srcType
    ehs_char* tagType; // SML tag name (e.g. smilText, Media,...)
    ehs_char* pathToRefFile; // Normally NULL, but set if a "refseq" tag is found.
    ehs_uint32 orderSeed; // this is used for random play to allow potentially synchroised scrambling across independent devices
    ehs_bool  WaitingRefseqRead;
    ehs_char *dynamicInnerXml;// this is normallyNUL, but is set to dynamic SMIL data read at the point of playout for dynseq sections.
    ehs_char smilText[EHS_PLAYMANAGER_MAX_XML_STRINGLENGTH]; // this is the inline text that might be set in SMIL files.
    ehs_sint32 repeatCount; /* smil2's repeat count = 0 means "indefinate" */

    ehs_char* region; // this is the region the media is assigned to (if we are a media event
    ehs_char* layoutSection; // this is the layout ID for a section (seq, par, exl).

    /* Dynamic control flags and vars*/
    ehs_bool bWaiting2EndList;  /* flag set when an immediate repeat list that has completed but not expired */
    ehs_uint32 bEndFiredWaitingForAck; /* timeout (in loop iterations) set when the stop event has been fired, but waiting for an ackowledge before the item dissapears */
    ehs_bool repeatTrigger; /* transitory flag set to instruct the player to play the event again if it has expired r finished. */
    ehs_sint32 repeatCountDown; /* used to count how many times content has been repeated */
    ehs_bool bCurrentPlayStarted; // this for any object or list that has started. When this is set the list's end time is checked.
    ehs_bool bAllChildrenExpired; // this is set to TRUE by default but any children the xomplete with out expiring (e.h. don't have an end time, but have run their course can set thisto false to stop the parent looping (potentially 10000s of repeats) )
    EhsPlayManagerMarkedType marked;    // THis is a marker so that stuff can be marked to be removed at some future point.
    ehs_sint32 WaitingForObjectUniqueID; // this is the unique ID that needs to be checked and reset if a list event is pending entries to complete..
    ehs_sint32 SendersObjectUniqueID; // this is the unique ID that will be emitted when the event asserts (if this is a firable one) - 1 for invalid.
    ehs_sint32 ListUniqueID; /*This is nearly obsolete but is used to indicate an event is a list event if > 0*/ // this is the unique ID shared between start and end events - so that end events can kill all events created.
    /* Note it should not be necessary for child objects or lists to be removed when an end event occurs as long as none are posted where the begin event is later than the end event */
    EhsPlayManagerEventListForParType * parEventList; //This is populated for para list types and contains IDs of all the objects held in the list that have yet to be received.

} EhsPlayManagerEvent;

typedef enum FontSize
{
    _small,
    _medium,
    _large
} FontSize;

typedef enum FontType
{
    sarif,
    sansarif,
    typewriter
} FontType;

typedef struct EhsPlaymanegerLayoutTextStyleStruct EhsPlaymanegerLayoutTextStyle;
struct EhsPlaymanegerLayoutTextStyleStruct
{
    EhsPlaymanegerLayoutTextStyle * next;
    FontType fontType;
    FontSize fontSize;
    ehs_char fontStyleId[EHS_STRING_LENGTH_MAX];
};

typedef struct EhsPlaymanegerLayoutRegionStruct EhsPlaymanegerLayoutRegion;
struct EhsPlaymanegerLayoutRegionStruct
{
    ehs_uint32 id;
    ehs_uint16 left;
    ehs_uint16 right;
    ehs_uint16 top;
    ehs_uint16 bottom;

    FontType fontType; // Populate these when all the textstyles and regions have been parsed for easier
    FontSize fontSize;
    ehs_char xmlId[EHS_STRING_LENGTH_MAX];
    ehs_char fontStyleId[EHS_STRING_LENGTH_MAX];
    EhsPlaymanegerLayoutRegion  * next;
};

typedef struct EhsPlaymanegerLayoutStruct EhsPlayManagerLayout;
struct EhsPlaymanegerLayoutStruct
{
    ehs_uint32 id;
    ehs_uint16 top;
    ehs_uint16 height;
    ehs_uint16 left;
    ehs_uint16 width;
    ehs_uint16 tableColumns;
    ehs_uint16 tableRows;
    ehs_char json[EHS_STRING_LENGTH_MAX];
    EhsPlaymanegerLayoutRegion * regions;
    EhsPlayManagerLayout * next;
} ;


typedef struct EhsPlaymanegerLayoutRootStruct
{
    ehs_uint16 rootWidth;
    ehs_uint16 rootHeight;

    EhsPlayManagerLayout * EhsPlayManagerLayout;// first layout
    EhsPlaymanegerLayoutTextStyle * EhsPlayManagerTextStyle;// first textstyle
} EhsPlaymanegerLayoutRoot;


/* Parser main Structure */

typedef struct
{
    //EhsTimerType xTimer; //depricated to use a thread instead
    ehs_sint32 NewPlaylistStartGracePeriod; // this is the counted loop iterations countdown (in clock ticks that must occur before any events from a new playlist are asserted
    ehs_sint32 initial_grace_seconds; // this is the constant values set in the parameters. if it set to -1 then no timeout or grace value is used.

    EhsCallbackQueueEntryType xEntry; // this is the space for the callback queue struct.
    EhsCallbackQueueType tickCallBackQueue; // This is a pointer to it.
    EhsTickType tPeriod;
    ehs_bool bRetriggerable;
    ehs_bool bPeriodic;
    EhsPlayManagerEvent *pEvent;
    ehs_sint32 pWaitingOnEvent; // This is the ID of the last event STARTED that must be acked by Started Play ID port before ANYTHING is done (tick function). = 0 when acked.
    ehs_sint32 pWaitingOnEventCounter; // count out how many times we tick waiting for an event set;
    ehs_sint32 ObjectUniqueID; // this is the instance global value incremented for every object fired. This is used for knowing when the next started can be fired.
    ehs_sint32 ListUniqueID; // current unique ID that is given to list events.
    //This is to serialise the firing of each event (i.e. the started event must occur for each fire, until the next one is allowed.
    pthread_mutex_t EhsL_eventList;
    EhsTPMutexClass EhsTPMutex_eventList; //Not currently used, use global mutex in target_process.c
    xmlTextReaderPtr fileReader; //Reads the master playlist from the file system
    xmlTextReaderPtr srcFileReader; //Reads the master playlist from the file system for generating GET events
    EhsPlayManagerMediaLocation* pMediaList;

    ehs_bool NewSmil; // this is set unttriedAllDownloadsl the first entry of the playlist has been played.
    EhsPlayManagerNewSmilBehavType NewSmilBehaviour; // enumeration type for what SMIL parser should do with new SMIL data.
    ehs_bool playing;
    ehs_bool triedAllDownloads; /* flag set to false while downloading and true when a complete playlist has been parsed */
    ehs_bool waitForAllDownloads;
    //ehs_uint8 checkParentExpiry; /* counter (usally starts at 1 - as only need one iteration). This is set when a list expires (or resets times) so that all events check if they should expire next tick */
    EhsPlaymanegerLayoutRoot *layoutInfo; // the root of a linked list of info for a layout
    EhsFunctionInstanceDataType* pFireFIData; // this is populated with the tick call-back pfi data, so that other functions (like next playlist) can call the fire function.
    time_t earliestPlayTime;
    ehs_bool changedStart;
    EhsPlayManagerEventTypeEnum earliestPlayTimeNodeType;
    time_t earliestEndTime;
    ehs_bool changedEnd;
    EhsPlayManagerEventTypeEnum earliestEndTimeNodeType;
    time_t lastTickTime;
    ehs_char downloadURL[EHS_STRING_LENGTH_MAX];//TODO:STRINGLENGTH!
    ehs_char downloadPath[EHS_STRING_LENGTH_MAX];//TODO:STRINGLENGTH!
#define EHS_NUM_LASTSRC_CACHESIZE 5
    ehs_char lastSrcPath[EHS_NUM_LASTSRC_CACHESIZE][EHS_STRING_LENGTH_MAX]; // remember the last 3 readSrc entry and skip it if seen again //TODO:STRINGLENGTH!
    ehs_uint32 lstSrcIndex;
    ehs_uint32 playlistFileCount;
} EhsPlayManagerType;

/*
 * Local prototypes
 */
void FreeAllEventMemory(EhsPlayManagerEvent* pEvent);
void removeEvent(EhsPlayManagerEvent* pEvent, EhsPlayManagerType* pPlayManager);
void ResetMemoryReader(EhsPlayManagerEvent* pEvent, ehs_bool resetLoopCounter);
ehs_bool isaSmilOtherImmediateElement(ehs_char* nextTag);
ehs_bool isaSmilRefElement(ehs_char* nextTag);
ehs_bool NotifyParallelListIsComplete(EhsPlayManagerType* pPlayManager,	ehs_uint32 played,time_t currentt);
ehs_bool NotifyAnObjectIsComplete(EhsPlayManagerType* pPlayManager,	ehs_uint32 played);
ehs_bool HandleEndEvent(EhsPlayManagerType* pPlayManager, EhsPlayManagerEvent **Event, ehs_bool endEvent, time_t currentt);
ehs_bool HandleStartEvent(EhsPlayManagerType* pPlayManager, EhsPlayManagerEvent **ppEvent, time_t currentt);
void AssertTimeWindowPorts(EhsFunctionInstanceDataType* pFIdata,EhsPlayManagerType* pPlayManager);


static void PrintEvent(EhsPlayManagerEvent *pEvent)
{
    EHSH_LOG_INFO("//EVENT//\neventTime=%d\ntype=%u\npPlaylist=%s\npMediaSource=%s, region=%s, section=%s, ID=%u, TYPE=%s, ListType=%u Addr=%x\n",
                  (unsigned int)pEvent->eventTime, (unsigned int)pEvent->type, pEvent->pPlaylist,
                  pEvent->pMediaSource,pEvent->region,pEvent->layoutSection,(unsigned int)pEvent->WaitingForObjectUniqueID,pEvent->srcType,(unsigned int)pEvent->eventListType,(unsigned int)pEvent);
}

EHS_LOCAL ehs_bool ClearMediaList(EhsPlayManagerType* pPlayManager, ehs_bool marked_only);


/* Utilities */


/*
 * Pseudo random number generator - max 32 bit  unsigned value
 * Must be provided with last value. which can be 0 if a seed > 0 is provided
 * You must provide the laste value returned as arg 1 and not the returned value iin the require range to avoid non-repeating values
 * */
ehs_uint32 pseudorand(ehs_uint32 * lastvalue, ehs_uint32 range)
{
    *lastvalue = (*lastvalue * 1103515245 + 12345) % 32768;
    return *lastvalue % (range);
}


/** This starts the tick function running periodically
 * THis is started at init time. (
 * @todo should be started on next playlist)
 * */
EHS_FB_THREAD_FUNCTION(PlayManagerThread)
{

    EhsPlayManagerType* pPlayManager = (EhsPlayManagerType*) EHS_FB_RUN_CONTEXT;
    Ehs_FB_ThreadStarted();

    while (1)
    {
        if (*bNewSodlFlagRef == EHS_TRUE)
        {
            break;
        }
        if (pPlayManager->tickCallBackQueue != NULL && *bNewSodlFlagRef == EHS_FALSE && *bRuntablesReadyRef == EHS_TRUE)
        {
            EhsCallbackQueue_execute(&pPlayManager->tickCallBackQueue); //@todo we seem to get a null instance here - why is this thread running before an instance or after deleted?
        }
        //EhsSleep(EHS_TIME_us(pPlayManager->tPeriod)); - no idea what units for EhsSleep should be... 
        EhsSleepUs(pPlayManager->tPeriod);
    }
    EHSH_LOG_ERROR("Exiting Playmanager thread OK");
    Ehs_FB_ThreadComplete();
    EhsHThread_exit();
}



EHS_FB_IDENTIFY_FUNCTION(PlayManager)
{
    EHS_TRACE_FUNCTION(EHS_FB_IDENTIFY_NAME(PlayManager));
    EHS_FB_IDENTIFY_MEMORY = sizeof(EhsPlayManagerType);
}

void resetPlaylistFlags(EhsPlayManagerType * pPlayManager)
{
    /* Dynamic parameters & flags */
    pPlayManager->pWaitingOnEvent = 0;
    pPlayManager->bPeriodic = EHS_TRUE;
    pPlayManager->bRetriggerable = EHS_TRUE;
    pPlayManager->ObjectUniqueID = 1;
    pPlayManager->ListUniqueID=1;
    pPlayManager->playing = EHS_FALSE;
    pPlayManager->NewSmil = EHS_FALSE;
    pPlayManager->triedAllDownloads = EHS_FALSE;
    pPlayManager->earliestPlayTime = 0;
    pPlayManager->changedStart = EHS_FALSE;
    pPlayManager->earliestEndTime=0;
    pPlayManager->changedEnd = EHS_FALSE;
    pPlayManager->lastTickTime = 0;
    pPlayManager->lastSrcPath[0][0] = '\0';
    pPlayManager->lastSrcPath[1][0] = '\0';
    pPlayManager->lastSrcPath[2][0] = '\0';
    pPlayManager->lastSrcPath[3][0] = '\0';
    pPlayManager->lastSrcPath[4][0] = '\0';

    pPlayManager->lstSrcIndex = 0;
}

EHS_FB_INIT_FUNCTION(PlayManager)
{
    EhsPlayManagerType* pPlayManager = EHS_FB_INIT_CONTEXT;
    const char* pParams;
    ehs_uint32 nPeriodIn_uS;
    ehs_uint8 nByte;
    EHS_TRACE_FUNCTION(EHS_FB_INIT_NAME(PlayManager));
    const ehs_char* pBuff = EHS_FB_INIT_PARAMETERS;
    ehs_uint8 tempuint8;
    ehs_sint32 tempsint32;
    xmlInitParser(); //According to the docs for libxml this is required for safe multi threaded use of the libxml libraries
    pPlayManager->NewSmilBehaviour = CLEAR_ALL; //default behaviour

    pBuff = EhsGetUint8FromString(&tempuint8, pBuff); // this is an integer as described by the enumeration type
    pPlayManager->NewSmilBehaviour=(ehs_uint8)tempuint8;

    pBuff = EhsGetUint8FromString(&tempuint8, pBuff); // boolean flag
    pPlayManager->waitForAllDownloads=(tempuint8 > 0);

    pBuff = EhsGetSint32FromString(&tempsint32, pBuff);
    pPlayManager->initial_grace_seconds=tempsint32; // number of seconds

    if (pPlayManager->waitForAllDownloads == EHS_FALSE && pPlayManager->initial_grace_seconds < 0 ) pPlayManager->initial_grace_seconds = 1; // if the user has set no wait for download and an infinite grace period - then override to start straight away.

    /* set up the callback queue to call the callback function */
    /* Initialise the timer */

    pPlayManager->tPeriod = EHS_SMILPARSER_TICK_TIME_US; // This should be quick (less than 10ms as we may need several iterations for some events and we are sometimes doing more real-time stuff)

    pPlayManager->downloadURL[0] = '\0';
    pPlayManager->downloadPath[0] = '\0';
    pPlayManager->lastSrcPath[0][0] = '\0';
    pPlayManager->lastSrcPath[1][0] = '\0';
    pPlayManager->lastSrcPath[2][0] = '\0';
    pPlayManager->lastSrcPath[3][0] = '\0';
    pPlayManager->lastSrcPath[4][0] = '\0';
    pPlayManager->lstSrcIndex = 0;
    pPlayManager->tickCallBackQueue = NULL;

    /* Dynamic Parsers and list section pointers*/
    pPlayManager->pEvent = NULL;
    pPlayManager->fileReader = NULL;
    pPlayManager->srcFileReader = NULL;
    pPlayManager->pMediaList = NULL;
    pPlayManager->layoutInfo = NULL;
    //pPlayManager->checkParentExpiry = 0;

    resetPlaylistFlags(pPlayManager);

    pPlayManager->NewPlaylistStartGracePeriod = pPlayManager->initial_grace_seconds*1000000/EHS_SMILPARSER_TICK_TIME_US; //(EHS_SMILPARSER_DEFAULT_START_GRACE/EHS_SMILPARSER_TICK_TIME_US); /* Grace period is defined as tick periods - currently fixed */
    pPlayManager->pFireFIData = EHS_FB_INIT_CALLBACK_FUNCTION_INSTANCE(-1); /* add the instance data for the fire function */
    EhsCallbackQueue_register(&(pPlayManager->tickCallBackQueue),EHS_FB_RUN_NAME(PlayManager_Tick),EHS_FB_INIT_CALLBACK_FUNCTION_INSTANCE(-1), &(pPlayManager->xEntry));
    EHS_FB_START_THREAD(PlayManagerThread, -90);
    return EHS_TRUE; /* initialisation always succeeds */
}

/*
 * This get the next tag in the SMIL file and returns its string.
 * This will just skip comments and inter node text
 * The next position is ready for a read using whatever read the client wants
 */

static ehs_char* getNextTag(xmlTextReaderPtr reader, int * nodeType)
{
    ehs_char* nextTag = NULL;
    ehs_char* name;
    int ret = 1;
    if (nodeType) *nodeType=0;
    while (ret == 1)
    {
        name = (ehs_char*)xmlTextReaderConstName(reader); // This is dealoocated with reader  -don't deallocate!
        /* Ignore #text, #comment */
        if (name)
        {
            if (nodeType) *nodeType=xmlTextReaderNodeType(reader); // need to get the type here before we move on with xmlReaderReads)(
            if ((strcmp("#text", name) == 0) || (strcmp("#comment", name) == 0))
            {
                //node is a comment or node body so should be discarded for the next node
                ret = xmlTextReaderRead(reader);
            }
            else
            {
                //this is not a comment or text body so is therefore the next node to be parsed so stop looping
                nextTag = name; // @todo is this bit causes a crashes when the the list is cleared (and the XML is destroyed)?
                ret = 0; //this breaks the loop
            }
        }
        else
        {
            ret = 0;
        }

    }
    return nextTag;
}


/* This updates the start and end time if these are repeat events
 * ( if the end time is == 0 the start times are not changed ).
 *
 * returns false if there isn't a proper repeat type and
 * returns TRUE if the event will be repeated, but also FALSE if:
 *  -  a repeat is not needed(e.g. repeat count has expired).
 *  - a parent has a fastr or equal repeat rate (and not loop repeat) - in which case the repeat is ignored.
 *
 * end only flag is used to update only the end time.
 */

ehs_bool UpdateEventTime(EhsPlayManagerEvent *pEvent, ehs_bool end_only, time_t currentt, ehs_bool bTestOnly )
{
    struct tm eventTM;
    ehs_bool doRepeat = EHS_TRUE;

    // print_trace();
    /* Avoid any double updating an event - there is never a case where we update an event that is already in the future */
    if (pEvent->eventTime > currentt)
    {
        return EHS_TRUE;
    }
    /* first check if the item has a more frequent or same frequeny repeat */
    switch (pEvent->repeat)
    {
    case EHS_PLAYMANAGER_REPEAT_YEAR:
        //Add 1 year to event time
        if (! bTestOnly)
        {
            if (! end_only)
            {
                //gmtime_r(&(pEvent->eventTime), &eventTM);
#ifdef EHS_TRY_TO_FORCE_GMT
                gmtime_r(&pEvent->eventTime, &eventTM);
#else
                localtime_r(&pEvent->eventTime, &eventTM); /* @todo proper fix maybe to feed time into something so it's not gmt... */
#endif
                eventTM.tm_year = eventTM.tm_year + 1;
                eventTM.tm_isdst = -1; // let mktime determine DST with the adjustment
                pEvent->eventTime = mktime(&eventTM);
            }
            if (pEvent->eventEndTime && (pEvent->eventEndTime!=EHS_PLAYMANAGER_TIMER_UNDEFINED))
            {
                //gmtime_r(&(pEvent->eventEndTime), &eventTM);
#ifdef EHS_TRY_TO_FORCE_GMT
                gmtime_r(&pEvent->eventEndTime, &eventTM);
#else
                localtime_r(&pEvent->eventEndTime, &eventTM); /* @todo proper fix maybe to feed time into something so it's not gmt... */
#endif
                eventTM.tm_year = eventTM.tm_year + 1;
                eventTM.tm_isdst = -1; // let mktime determine DST with the adjustment
                pEvent->eventEndTime = mktime(&eventTM);

            }
        }
        break;
    case EHS_PLAYMANAGER_REPEAT_MONTH:
        //Add 1 month to event time
        if (! bTestOnly)
        {
            if (! end_only)
            {
                //gmtime_r(&(pEvent->eventTime), &eventTM);
#ifdef EHS_TRY_TO_FORCE_GMT
                gmtime_r(&pEvent->eventTime, &eventTM);
#else
                localtime_r(&pEvent->eventTime, &eventTM); /* @todo proper fix maybe to feed time into something so it's not gmt... */
#endif
                //Deal with year rolling over - we might not really need to do this as mktime should sort it out?
                if (eventTM.tm_mon == 12)
                {
                    eventTM.tm_mon = 1;
                    eventTM.tm_year = eventTM.tm_year + 1;
                }
                else
                {
                    eventTM.tm_mon = eventTM.tm_mon + 1;
                }
            }
            eventTM.tm_isdst = -1; // let mktime determine DST with the adjustment
            pEvent->eventTime = mktime(&eventTM);
            if (pEvent->eventEndTime && (pEvent->eventEndTime!=EHS_PLAYMANAGER_TIMER_UNDEFINED))
            {
                //gmtime_r(&(pEvent->eventEndTime), &eventTM);
#ifdef EHS_TRY_TO_FORCE_GMT
                gmtime_r(&pEvent->eventEndTime, &eventTM);
#else
                localtime_r(&pEvent->eventEndTime, &eventTM); /* @todo proper fix maybe to feed time into something so it's not gmt... */
#endif
                if (eventTM.tm_mon == 12)
                {
                    eventTM.tm_mon = 1;
                    eventTM.tm_year = eventTM.tm_year + 1;
                }
                else
                {
                    eventTM.tm_mon = eventTM.tm_mon + 1;
                }
                eventTM.tm_year = eventTM.tm_year + 1;
                eventTM.tm_isdst = -1; // let mktime determine DST with the adjustment
                pEvent->eventEndTime = mktime(&eventTM);
            }
        }
        break;
    case EHS_PLAYMANAGER_REPEAT_DAY:

        //Add 24 hours to event time
        if (! bTestOnly)
        {

            if (! end_only)
            {
                localtime_r(&pEvent->eventTime, &eventTM); /* @todo proper fix maybe to feed time into something so it's not gmt... */
                eventTM.tm_mday = eventTM.tm_mday + 1;
                eventTM.tm_isdst = -1; // let mktime determine DST with the adjustment
                pEvent->eventTime = mktime(&eventTM);
            }
            if (pEvent->eventEndTime && (pEvent->eventEndTime!=EHS_PLAYMANAGER_TIMER_UNDEFINED))
            {
                localtime_r(&pEvent->eventEndTime, &eventTM); /* @todo proper fix maybe to feed time into something so it's not gmt... */
                eventTM.tm_mday = eventTM.tm_mday + 1;
                eventTM.tm_isdst = -1; // let mktime determine DST with the adjustment
                pEvent->eventEndTime = mktime(&eventTM);

            }
        }
        break;
    case EHS_PLAYMANAGER_REPEAT_HOUR:

        //Add 1 hours to event time
        if (! bTestOnly)
        {

            if (! end_only)
            {
                localtime_r(&pEvent->eventTime, &eventTM); /* @todo proper fix maybe to feed time into something so it's not gmt... */
                eventTM.tm_hour = eventTM.tm_hour + 1;
                eventTM.tm_isdst = -1; // let mktime determine DST with the adjustment
                pEvent->eventTime = mktime(&eventTM);
            }
            if (pEvent->eventEndTime && (pEvent->eventEndTime!=EHS_PLAYMANAGER_TIMER_UNDEFINED))
            {
                localtime_r(&pEvent->eventEndTime, &eventTM); /* @todo proper fix maybe to feed time into something so it's not gmt... */
                eventTM.tm_mday = eventTM.tm_mday + 1;
                eventTM.tm_isdst = -1; // let mktime determine DST with the adjustment
                pEvent->eventEndTime = mktime(&eventTM);

            }
        }
        break;
    case EHS_PLAYMANAGER_REPEAT_WEEK:
        if (! bTestOnly)
        {
            //Add 7 days to event time

            if (! end_only)
            {
                localtime_r(&pEvent->eventTime, &eventTM); /* @todo proper fix maybe to feed time into something so it's not gmt... */
                eventTM.tm_mday = eventTM.tm_mday + 7;
                eventTM.tm_isdst = -1; // let mktime determine DST with the adjustment
                pEvent->eventTime = mktime(&eventTM);
            }
            if (pEvent->eventEndTime && (pEvent->eventEndTime!=EHS_PLAYMANAGER_TIMER_UNDEFINED))
            {
                localtime_r(&pEvent->eventEndTime, &eventTM); /* @todo proper fix maybe to feed time into something so it's not gmt... */
                eventTM.tm_mday = eventTM.tm_mday + 7;
                eventTM.tm_isdst = -1; // let mktime determine DST with the adjustment
                pEvent->eventEndTime = mktime(&eventTM);
            }
        }
        break;
    case EHS_PLAYMANAGER_REPEAT_2WEEK:
        //Add 14 days to event time
        if (! bTestOnly)
        {
            if (! end_only)
            {
                localtime_r(&pEvent->eventTime, &eventTM); /* @todo proper fix maybe to feed time into something so it's not gmt... */
                eventTM.tm_mday = eventTM.tm_mday + 14;
                eventTM.tm_isdst = -1; // let mktime determine DST with the adjustment
                pEvent->eventTime = mktime(&eventTM);
            }
            if (pEvent->eventEndTime && (pEvent->eventEndTime!=EHS_PLAYMANAGER_TIMER_UNDEFINED))
            {
                localtime_r(&pEvent->eventEndTime, &eventTM); /* @todo proper fix maybe to feed time into something so it's not gmt... */
                eventTM.tm_mday = eventTM.tm_mday + 14;
                eventTM.tm_isdst = -1; // let mktime determine DST with the adjustment
                pEvent->eventEndTime = mktime(&eventTM);
            }
        }
        break;
    default:
        doRepeat = EHS_FALSE; /* Don't what to do with this - caller may want to delete */
        break;
    }

    return doRepeat; /* Dealt with OK */
}

/* Called from Handle end event to see if we are looping or not
*/

ehs_bool CheckUpdateEventLoop(EhsPlayManagerEvent *pEvent,time_t currentt)
{
    ehs_bool doRepeat = EHS_FALSE;


    if ((pEvent->bAllChildrenExpired == EHS_TRUE && pEvent->type != EHS_PLAYMANAGER_EVENT_OBJECT )||
            (pEvent->eventEndTime != 0 && pEvent->eventEndTime != EHS_PLAYMANAGER_TIMER_UNDEFINED && currentt >= pEvent->eventEndTime ))
    {
        pEvent->repeatCountDown=0;
        doRepeat = EHS_FALSE;
    }
    else
    {
        /* handle any repeat counts*/
        if (pEvent->repeatCountDown > 0 || pEvent->repeatCount == EHS_PLAYMANAGER_LOOP_INDEFINITE)
        {
            // greater than 1 not 0 because we repeat 1 time for two plays.
            if (pEvent->repeatCount != EHS_PLAYMANAGER_LOOP_INDEFINITE)
            {
                //! bTestOnly) {
                pEvent->repeatCountDown--;
                // pEvent->eventEndTime = pEvent->eventEndTime	+ ( pEvent->eventDuration / 1000); - we don't do this, because duration is not fir each repetition, but the time all repeats are allowed.
                if (pEvent->repeatCountDown < 1)
                {
                    //we have ran out of repeats
                    doRepeat = EHS_FALSE;
                }
                else
                {
                    doRepeat = EHS_TRUE;
                }
            }
            else
            {
                doRepeat = EHS_TRUE;
            }
        }
    }
    return doRepeat;
}

/** @brief parses hh,mins,seconds and returns a duration in time_t units
 *
 * returns millisconds
 */


static ehs_uint32 parseDurationValue(ehs_char * value)
{
    if (value == NULL) return 0;
    int length = strlen(value);
    int endOffset = length>0?length-1:0;
    ehs_uint32 nDurationMs = 0;
    ehs_uint32 hours= 0;
    ehs_uint32 minutes = 0;
    float floatingNumber = 0;
    ehs_bool doColons = EHS_TRUE; //
    int i,colCount = 0;
    for (i =0; i< length; i++)
    {
        if (value[i] == ':') colCount++;
    }
    if (colCount == 0)  // check for a suffix
    {
        switch (value[endOffset])
        {
        case 'h':
            if (EhsSscanf(value,"%fh", &floatingNumber))
            {
                nDurationMs = (int)(floatingNumber*3600.0f*1000.0f);
                doColons = EHS_FALSE;
            }
            break;
        case 'm':
            if (EhsSscanf(value,"%fm", &floatingNumber))
            {
                nDurationMs = (int)(floatingNumber*60000.0f);
                doColons = EHS_FALSE;
            }
            break;
        case 'n': //or  min
            if (EhsSscanf(value,"%fmin", &floatingNumber))
            {
                nDurationMs = (int)(floatingNumber*60000.0f);
                doColons = EHS_FALSE;
            }
            break;
        case 's':
            if (length>1 && value[length-1] == 'm')
            {
                if (EhsSscanf(value,"%fms", &floatingNumber))
                {
                    nDurationMs = (int)(floatingNumber);
                    doColons = EHS_FALSE;
                }
            }
            else
            {
                if (EhsSscanf(value,"%fs", &floatingNumber))
                {
                    nDurationMs = (int)(floatingNumber*1000);
                    doColons = EHS_FALSE;
                }
            }
            break;
        default:
            break;
        }
    }

    if (doColons)
    {
        switch (colCount)
        {
        case 0:
            if (EhsSscanf(value,"%f",&floatingNumber))
            {
                nDurationMs = (int)(floatingNumber*1000.0f);
            }
            break;
        case 1:
            if (EhsSscanf(value,"%d:%f",&minutes,&floatingNumber))
            {
                nDurationMs = (minutes*60000)+(int)(floatingNumber*1000.0f);
            }
            break;
        case 2:
            if (EhsSscanf(value,"%d:%d:%f",&hours,&minutes,&floatingNumber))
            {
                nDurationMs = (hours*3600000+minutes*60000)+(int)(floatingNumber*1000.0f);
            }
            break;
        default:
            break;
        }
    }
    return nDurationMs;
}

/** @brief parses inx modified W3C wallclock format to allow for various repeat patterns
 *
 * Daily    : HH:MM:SS
 * Weekly	: wTHH:MM:SS
 * 2 weekly : W1wTHH:MM:SS / W2wTHH:MM:SS
 * monthly	: ddTHH:MM:SS
 * yearly	: mm-ddTHH:MM:SS
 * One only : YY-mm-ddTHH:MM:SS
 */
static void parseWallClockValue(ehs_char* value, EhsPlayManagerEvent *pEvent, ehs_bool bEndTime)
{
    int length = 0;
    time_t t = -1;
    struct tm tm;		// broken down time for the read-in time
    struct tm currenttm; // current time
    struct tm currenttm_0; // time at current midnight (the day before) -  used for 2week schedule
    time_t currentt;
    time_t currentt_0;
    ehs_char *pEndBrace, *pWallClockValue;

    void * ret=NULL;
    void * ret2=NULL;
    struct tm reftm;
    reftm.tm_min=0;
    reftm.tm_sec=0;
    reftm.tm_hour=0;

    ehs_sint32 add =0;
#define EHS_WEEKOFFSET 10
    ehs_char refdatestr[EHS_WEEKOFFSET+1];
    time_t diff = -1;
    time_t reft;

    /* test length of value, if it is longer than "wallclock(" then test to see if it starts with wallclock */
    if (strlen(value) >= 10)
    {
        if (strncmp(value, "wallclock(", 10) == 0)
        {
            //starts with wallclock, now chomp whitespace from that brace
            pWallClockValue = &value[10];
            while (*pWallClockValue == ' ')
            {
                pWallClockValue++;
            }
            //get positon of closing brace
            pEndBrace = strchr(value, ')');
            if (pEndBrace != NULL)
            {
                //then chomp whitespace backwards
                do
                {
                    pEndBrace--;
                }
                while (*pEndBrace == ' ');
                /* make sure the end is greater than the start otherwise something has gone wrong (like an empty wallclock value) */
                if (pWallClockValue < pEndBrace)
                {
                    /* Set the character one after the end our wallclock value as null so that our string terminates */
                    *(pEndBrace + 1) = '\0'; //left hand side says "increment pEndBrace to point at the next char then set the value held within", we must remember to set this to non null otherwise we risk leaking memory in the xml clean up function
                    /* get current time and date so we can fill in any missing values */
                    currentt = time(NULL);
                    /* with gmtime  we get the UTC time - and the date might be different to the local date, and roll-over repeats and list entries get screwed up
                     * We have to use local time otherwise we get repeats not at midnight, which is awkward.  - assume this is because time() gets local time!!
                     * gmtime_r(&currentt, &currenttm);
                     */
#ifdef EHS_TRY_TO_FORCE_GMT
                    gmtime_r(&currentt, &currenttm);
                    gmtime_r(&currentt, &currenttm_0);
                    //currenttm.tm_isdst = 0;
#else
                    localtime_r(&currentt, &currenttm); /* we assume m_isdst will be set by localtime_r */

                    localtime_r(&currentt, &currenttm_0); /* move this into the 2week case for efficiency */
                    //currenttm.tm_isdst = 1; this would force all times at all times of year to be daylight adjusted
#endif
                    currenttm_0.tm_min=0; /* zero time of day to get a time_t at midnight */
                    currenttm_0.tm_sec=0;
                    currenttm_0.tm_hour=0;
                    currenttm_0.tm_isdst=-1; /* set to -1 mso we can recheck DST at midnight in mktime*/
                    currentt_0 = mktime(&currenttm_0);

                    /*Find out what format the time is in */
                    /* we are going to strlen, since each datetime format is of a different length we can use that to reliably determine the format */
                    length = strlen(pWallClockValue);
                    //tm.tm_isdst=1; /* this would force all times at all times of year to be daylight adjustede */
                    tm.tm_isdst= -1 ; // we want mktime to determine the actual time if this is read during daylight saving time.
                    switch (length)
                    {
                    case 5: // mm:ss
                        if ((void *) strptime((char*) pWallClockValue,"%M:%S", &tm) != NULL)
                        {
                            tm.tm_mday = currenttm.tm_mday;
                            tm.tm_mon = currenttm.tm_mon;
                            tm.tm_year = currenttm.tm_year;
                            tm.tm_hour = currenttm.tm_hour;
                            //if (!bEndTime) @todo this should be conditional on the start and end time format - both need to be OK..
                            pEvent->repeat = EHS_PLAYMANAGER_REPEAT_HOUR; //repeat daily
                        }
                        break;
                    case 8:
                        if ((void *) strptime((char*) pWallClockValue,"%H:%M:%S", &tm) != NULL)
                        {
                            tm.tm_mday = currenttm.tm_mday;
                            tm.tm_mon = currenttm.tm_mon;
                            tm.tm_year = currenttm.tm_year;
                            //if (!bEndTime) @todo this should be conditional on the start and end time format - both need to be OK..
                            pEvent->repeat = EHS_PLAYMANAGER_REPEAT_DAY; //repeat daily
                        }
                        break;
                    case 10: /* day of week given - single digit*/
                        if ((void *) strptime((char*) pWallClockValue,"%wT%H:%M:%S", &tm) != NULL)
                        {
                            ehs_sint32 x=tm.tm_wday-currenttm.tm_wday; // calculate difference to add to day of month.
#ifdef PUSH_FORWARD_IF_PAST
                            if (x >=0) tm.tm_mday = currenttm.tm_mday + x;
                            else tm.tm_mday = currenttm.tm_mday + x +7; /* note that mktime allows tm_mday > 31 and does the right thing*/
#else
                            tm.tm_mday = currenttm.tm_mday + x; /* allowing this to be negative so start times in the past are still done */
#endif
                            tm.tm_mon = currenttm.tm_mon;
                            tm.tm_year = currenttm.tm_year;
                            //if (!bEndTime) @todo this should be conditional on the start and end time format - both need to be OK..

                            pEvent->repeat = EHS_PLAYMANAGER_REPEAT_WEEK; //repeat daily

                        }
                        break;
                    case 22: /* /* YY-mm-dd{W1/2}D{T}HH:MM:SS      12 */ //strptime(pWallClockValue, "%Y-%m-%dT%H:%M:%S", &tm);
                    {

                        EhsStrncpy(refdatestr,(char*) pWallClockValue,10);
                        refdatestr[EHS_WEEKOFFSET]=0;
                        ret2  = strptime((char*) refdatestr,"%Y-%m-%d", &reftm) ;
                        if (ret2)
                        {
                            reftm.tm_isdst=-1; /* -1 meands Use the system timezone to deterine DST is on or off??*/
                            reft = mktime(&reftm); // reference start time of 2 week cycle
                            diff = currentt_0-reft;

                            tm.tm_mday = reftm.tm_mday;
                            tm.tm_mon = reftm.tm_mon;
                            tm.tm_year = reftm.tm_year;

                            if ( pWallClockValue[EHS_WEEKOFFSET+1] == '1' ) ret  = strptime((char*) &pWallClockValue[EHS_WEEKOFFSET],"W1%wT%H:%M:%S", &tm) ;
                            if ( pWallClockValue[EHS_WEEKOFFSET+1] == '2' ) ret  = strptime((char*) &pWallClockValue[EHS_WEEKOFFSET],"W2%wT%H:%M:%S", &tm) ;
                            if (ret && ret2)
                            {
                                //		strftime(buffer, 26, "%Y:%m:%d %H:%M:%S",&tm);
                                //		buffer[26]='\0';


                                if ( pWallClockValue[EHS_WEEKOFFSET+1] == '2' ) add = 7; /* Add another week if spec is for week 2 */
                                else add =0;
                                //ehs_sint32 x=tm.tm_wday+add-currenttm.tm_wday; // calculate difference to add to day of month.
                                /* find the number of days of 2week difference between now and ref date to see if we are odd or even */

                                ehs_sint32 x;
                                if (diff >=0) x = ((diff)/EHS_FB_MEDIA_PLAYMANAGER_DAY_SECONDS)%14; // probably can just use % for -ve numbers, but not taking chances!
                                else  x = -1*(((-1*diff)/EHS_FB_MEDIA_PLAYMANAGER_DAY_SECONDS)%14); //

#ifdef PUSH_FORWARD_IF_PAST
                                if (x >=0) tm.tm_mday = currenttm.tm_mday - x + add + tm.tm_wday;
                                else tm.tm_mday = currenttm.tm_mday - x +14 +add + tm.tm_wday; /* note that mktime allows tm_mday > 31 and does the right thing*/
#else
                                tm.tm_mday = currenttm.tm_mday - x + add + tm.tm_wday; /* allowing this to be negative so start times in the past are still done, but will be repeated in the future if the finish time has expired too */
#endif
                                tm.tm_mon = currenttm.tm_mon;
                                tm.tm_year = currenttm.tm_year;
                                //if (!bEndTime) @todo this should be conditional on the start and end time format - both need to be OK..
//								strftime(buffer, 26, "%Y:%m:%d %H:%M:%S",&tm);
                                pEvent->repeat = EHS_PLAYMANAGER_REPEAT_2WEEK; //repeat daily

                            }
                        }
                        break;
                    }
                    case 11:
                        if ((void *) strptime(pWallClockValue, "%dT%H:%M:%S",
                                              &tm) != NULL)
                        {
                            tm.tm_mon = currenttm.tm_mon;
                            tm.tm_year = currenttm.tm_year;
                            pEvent->repeat = EHS_PLAYMANAGER_REPEAT_MONTH; //repeat monthly, 1 is reserved for week repeat
                        }
                        break;
                    case 14:
                        if ((void *) strptime(pWallClockValue,
                                              "%m-%dT%H:%M:%S", &tm) != NULL)
                        {
                            tm.tm_year = currenttm.tm_year;
                            //if (!bEndTime) @todo this should be conditional on the start and end time format - both need to be OK..
                            pEvent->repeat = EHS_PLAYMANAGER_REPEAT_YEAR; //repeat yearly, probably not needed
                        }
                        break;
                    case 19:
                        strptime(pWallClockValue, "%Y-%m-%dT%H:%M:%S", &tm);
                        pEvent->repeat = EHS_PLAYMANAGER_REPEAT_NONE; //no repeat
                        break;
                    default:
                        if (length > 19)   // try umatching a fully qualified time - perhaps has time zone implemented.
                        {
                            strptime(pWallClockValue, "%Y-%m-%dT%H:%M:%S", &tm);
                            pEvent->repeat = EHS_PLAYMANAGER_REPEAT_NONE; //no repeat
                            break;
                        }
                        else
                        {
                        }
                    }
                    tm.tm_isdst= -1 ; // we want mktime to adjust for the final time to include DST.
                    t = mktime(&tm);
                    if (!bEndTime)
                    {
                        pEvent->eventTime = t;
                    }
                    else   // if the end time is before the start time, but less than 24 hours before then assume it is the next day. We don't bother doing this for weeks, months though
                    {
                        if (t < pEvent->eventTime && t > (pEvent->eventTime - (3600*24)))
                            pEvent->eventEndTime = t + (3600*24);
                        else
                            pEvent->eventEndTime = t;
                    }
                    *(pEndBrace + 1) = ' '; /* Clear away null character we put in earlier so that we don't leak memory (?? what??) */
                }
                else
                {
                    //badly positioned braces for wallclock value
                }
            }
            else
            {
                //could not find end of wallclock value
            }
        }
        else
        {
            //does not start with wallclock(
        }
    }
    else
    {
        //not long enough to be a wallclock value
    }
}

/*
 * called when the closing body tag is expected
 */

static ehs_bool closingBodyTag(xmlTextReaderPtr reader)
{
    ehs_char* nextTag;
    int nodeType;
    nextTag = getNextTag(reader,&nodeType );
    xmlTextReaderRead(reader); // @togo  we should check this success? - but can't in this function ...
    if (nextTag != NULL)
    {
        if ((strcmp(nextTag, "smil") == 0) && (nodeType == 15))
        {
            //closing smilTag found so just stop!
            return EHS_TRUE;
        }
        else
        {
            /* nothing */
        }
    }
    else
    {
        /* nothing */
    }
    return EHS_FALSE;
}

/*
 * Call this to post an event into the list for playing.
 * The event supplied will be pre-populated with a time or a waiting flag if it belongs to a sequence.
 */

static void addToEventList(EhsPlayManagerEvent* pNewEvent,
                           EhsPlayManagerType* pPlayManager)
{
    EhsPlayManagerEvent *pCurrentEvent = NULL;
    /* if list is empty just put it on as the first item */
    pNewEvent->pNext = NULL;//Just in case it was not inited properly.
    if (pPlayManager->pEvent == NULL)
    {
        pPlayManager->pEvent = pNewEvent;
    }
    else
    {
        /*List is not empty so run over it and find the new place for it */
        pCurrentEvent = pPlayManager->pEvent;
        /* Positioning logic is that it should go at the end of the list, we can thus assume that the list will be time ordered due to assuming we are only dealing with time ordered playlists at this phase */
        while (pCurrentEvent->pNext != NULL)
        {
            pCurrentEvent = pCurrentEvent->pNext;
        }
        /* We have reached the end of the event list. pCurrentEvent should point to the last entry in the list */
        pCurrentEvent->pNext = pNewEvent;
    }
}


/* same as for events..
 * We don't check the entry is valid !!! */
void removeMediaEntry(EhsPlayManagerMediaLocation *pML_prev,
                      EhsPlayManagerMediaLocation * pML, EhsPlayManagerType* pPlayManager)
{
    if (pML_prev == NULL)
    {
        if (pML->pNext)
        {
            pPlayManager->pMediaList = pML_prev->pNext;
        }
        else
        {
            pPlayManager->pMediaList = NULL;
        }
    }
    else
    {
        pML_prev->pNext = pML->pNext;
    }
    EhsHMem_tempFree(pML);
}


/*
 * Removes event from the linked list that is searched in ticks()
 * and removes anyreferences to it from other (child events)
 */

void removeEvent( EhsPlayManagerEvent* pEvent, EhsPlayManagerType* pPlayManager)
{
#ifdef EHS_DEBUG_SMILPARSER
    int id = pEvent->SendersObjectUniqueID;
#endif
    /* first look for references and set to null */
    EhsPlayManagerEvent* pPrev = NULL;
    ehs_bool found = EHS_FALSE;
    EhsTPMutex_lock(EhsTPMutex_playManager);
    EhsPlayManagerEvent* event = pPlayManager->pEvent;

    if (pEvent == NULL) return;

    if (pEvent == pPlayManager->pEvent)
    {
        /* check for any children pointing at this */
        while (event)
        {
            if (event->pParent == pEvent)
            {
                event->pParent = NULL;
            }
            event = event->pNext;
        }
        found = EHS_TRUE;
    }
    else
    {
        while (event)
        {
            if ( event->pNext == pEvent)
            {
                pPrev = event; // need to identif the previous event so relink
                found = EHS_TRUE;
            }
            if (event->pParent == pEvent)   // compare pointers to find matches
            {
                event->pParent = NULL;
            }
            event = event->pNext;
        }
    }

    /* Now remove the event */

    if (pPrev == NULL)
    {
        pPlayManager->pEvent = pEvent->pNext;
    }
    else
    {
        /* not the first event so we have to update the pointer of the previous one to skip it */
        pPrev->pNext = pEvent->pNext;
    }

    FreeAllEventMemory(pEvent);

    pEvent = NULL;
    EhsTPMutex_unlock(EhsTPMutex_playManager);
    /* check what's gone */
#ifdef EHS_DEBUG_SMILPARSER
    event = pPlayManager->pEvent;
    while (event)
    {
        event = event->pNext;
    }
#endif
}


/*
 * Clear media List for the playlist
 */

EHS_LOCAL ehs_bool ClearMediaList(EhsPlayManagerType* pPlayManager,
                                  ehs_bool only_marked)
{
    EhsPlayManagerMediaLocation *pML_prev = NULL;
    EhsPlayManagerMediaLocation *pML_temp;
    EhsTPMutex_lock(EhsTPMutex_playManager);
    EhsPlayManagerMediaLocation * pML = pPlayManager->pMediaList;
    while (pML != NULL)
    {
        pML_temp = pML->pNext;
        if (only_marked)
        {
            if (pML->marked == LIST_OLD)   /* remove old and default - defaults can be created from old events */
            {
                removeMediaEntry(pML_prev, pML, pPlayManager); // this sets pPlayManager to null if necessary
            }
            else
            {
                pML_prev=pML;
            }
        }
        else
        {
            EhsHMem_tempFree(pML);
        }
        pML = pML_temp;
    }
    if (!only_marked)
        pPlayManager->pMediaList = NULL;
    EhsTPMutex_unlock(EhsTPMutex_playManager);
    return EHS_TRUE;
}


/*
 * Clears all events for the playmanager or just the marked ones
 */
EHS_LOCAL void clearEventList(EhsPlayManagerType* pPlayManager,ehs_bool only_marked)
{

    EhsTPMutex_lock(EhsTPMutex_playManager);
    EhsPlayManagerEvent *pCurrentEvent = pPlayManager->pEvent;
    EhsPlayManagerEvent *pEventTemp;

    EHSH_LOG_INFO("Clearing Event List with %d", only_marked);

    while (pCurrentEvent != NULL)
    {
        pEventTemp = pCurrentEvent->pNext;
        if (only_marked)
        {
            if (pCurrentEvent->marked == LIST_OLD)   //just remove one link
            {
                EhsTPMutex_lock(EhsTPMutex_playManager);
                NotifyAnObjectIsComplete(pPlayManager,pCurrentEvent->SendersObjectUniqueID); /*need to remove this ID from any waiters and let them move on*/
                NotifyParallelListIsComplete(pPlayManager,pCurrentEvent->SendersObjectUniqueID,0); // This will set a flag on the par event to complete.
                removeEvent( pCurrentEvent, pPlayManager); // this sets pPlayManager to null if necessary /*@todo we should have a remove_with_tidy function to do the above */
                EhsTPMutex_unlock(EhsTPMutex_playManager);
            }
        }
        else     /* we're going to delete everything so just free the memory */
        {
            FreeAllEventMemory(pCurrentEvent);
        }
        pCurrentEvent = pEventTemp;
    }
    ClearMediaList(pPlayManager, only_marked); /* clear what's needed */
    if (!only_marked)
        pPlayManager->pEvent = NULL; /* flag it is empty */
    EhsTPMutex_unlock(EhsTPMutex_playManager);
}


/* Mark current events with ddefault mark flag */
EHS_LOCAL void  setDefaultMarks(EhsPlayManagerType* pPlayManager)
{

    EhsTPMutex_lock(EhsTPMutex_playManager);
    EhsPlayManagerEvent *pCurrentEvent = pPlayManager->pEvent;
    EhsPlayManagerMediaLocation * pML = pPlayManager->pMediaList;

    while (pCurrentEvent != NULL)
    {
        pCurrentEvent->marked=LIST_DEFAULT;
        pCurrentEvent = pCurrentEvent->pNext;
    }
    while (pML != NULL)
    {
        pML->marked=LIST_DEFAULT;
        pML = pML->pNext;
    }
    EhsTPMutex_unlock(EhsTPMutex_playManager);
}

/* marks the current list so that stuff can be removed when a new one starts firing events */

EHS_LOCAL void markEventList(EhsPlayManagerType* pPlayManager)
{
    EhsTPMutex_lock(EhsTPMutex_playManager);
    EhsPlayManagerEvent *pCurrentEvent = pPlayManager->pEvent;
    EhsPlayManagerEvent *pNextEvent = NULL;
    while (pCurrentEvent != NULL)
    {
        pNextEvent = pCurrentEvent->pNext;
        pCurrentEvent->marked=LIST_OLD;
        pCurrentEvent = pNextEvent;
    }
    EhsTPMutex_unlock(EhsTPMutex_playManager);
}


/* Do some base "class" initialisation of the event */
void InitEvent(EhsPlayManagerEvent * Event)
{

    Event->eventDuration = 0 ;
    Event->eventTime = 0;
    Event->bRelativeEndTime = EHS_FALSE;
    Event->eventEndTime = 0;
    Event->repeat = EHS_PLAYMANAGER_REPEAT_NONE;
    Event->parent_has_time=EHS_FALSE; /* Assume parent doesn't have timing info */
    Event->pNext = NULL;
    Event->pParent = NULL;
    Event->type = EHS_PLAYMANAGER_EVENT_UNKOWN;
    Event->eventListType = EHS_PLAYMANAGER_EVENTLIST_UNKOWN;
    Event->playlistSize = 0;
    Event->memoryReader = NULL;
    Event->pPlaylist = NULL;
    Event->pMediaSource = NULL;
    Event->class = NULL;
    Event->soundLevel = NULL;
    Event->class = NULL;
    Event->srcType = NULL;
    Event->fade = 0;
    Event->tagType = NULL;
    Event->region = NULL;
    Event->layoutSection = NULL;
    Event->WaitingForObjectUniqueID = 0; 	//  Default is 0 so we play non-list stuff
    Event->SendersObjectUniqueID = -1; 		// Default sender is different to 0 in case it slips through.
    Event->ListUniqueID = 0; 				//set to zero to indicate this is not a list related event
    Event->bCurrentPlayStarted =EHS_FALSE; /* Set as soon as event has started */
    Event->bAllChildrenExpired = EHS_TRUE; // default is this isn't true
    Event->parEventList=NULL;
    Event->marked=LIST_DEFAULT;
    Event->smilText[0]='\0';

    Event->pathToRefFile=NULL;
    Event->orderSeed=0;
    Event->pathToRefFile=EHS_FALSE;
    Event->dynamicInnerXml=NULL;
    Event->WaitingRefseqRead=EHS_FALSE;

    Event->repeatTrigger = EHS_FALSE;
    Event->repeatCount = 0; //0 indicates no repeat
    Event->repeatCountDown = 0;
    Event->bWaiting2EndList = EHS_FALSE;
    Event->bEndFiredWaitingForAck = 0;

    //Event->startedTime = 0;  // this can be used by child nodes to work out relative timings etc.
}

/**
 *
 * @brief CreateTimeEvent - populates the event structure. from SMIL attributes.
 *
 * DOES NOT INCREMENT THE READER
 *
 * Populates an end event structure if there is an end time present.
 * The end time structure is always of the type EHS_PLAYMANAGER_EVENT_END - and should be populated
 * with the ID and resource name.
 *
 * Call this to populate an event (supplied) with the time data is parsed from the next SMIL node. *
 *
 * This is used for all SMIL nodes that might have time data begin & end.
 * If there is no time data the default parent time value is assigned as the begin time.
 */

//@todo this should check the inout end time against the read-in one and use the the earliset for the end time.
ehs_bool CreateTimeEvent(xmlTextReaderPtr reader,
                         EhsPlayManagerEvent **pStartEvent, EhsPlayManagerEvent **pEndEvent,
                         time_t * parent_start_time, time_t * parent_end_time, EhsPlayManagerEvent * pParent)
{

    ehs_char * value = NULL;
    ehs_char * layoutId = NULL;
    ehs_bool ret = EHS_FALSE;
    EhsTPMutex_lock(EhsTPMutex_playManager);
    /* We will always have a start event - even if the begin is not present. We will use the parent time in this case. */
    *pStartEvent = (EhsPlayManagerEvent*) EhsHMem_tempAlloc(sizeof(EhsPlayManagerEvent)); //assume we have an end event for now
    if ((*pStartEvent) != NULL)
    {
        InitEvent(*pStartEvent);

        ret = EHS_TRUE;
        (*pStartEvent)->pNext = NULL; 							// just an initialiser for the linked list.
        (*pStartEvent)->repeat = EHS_PLAYMANAGER_REPEAT_NONE; 	//assume no repeat to start with
        (*pStartEvent)->pParent = pParent; 						//assign parent
        value = (ehs_char*) xmlTextReaderGetAttribute(reader, "begin"); 	// get the start time wall clock value

        if (value == NULL)
        {
#define EHS_PLAYMAN_INHERITPARENTTIMES
//#ifdef EHS_PLAYMAN_INHERITPARENTTIMES
// don't update the start time because some events in a section will start later than the section, but none will end after....
//			(*pStartEvent)->eventTime = *parent_start_time;//This will cause it to fire on the parent time instead
//#else
            (*pStartEvent)->eventTime = 0;//EHS_PLAYMANAGER_TIMER_UNDEFINED;
//#endif
        }
        else
        {
            parseWallClockValue(value, (*pStartEvent), EHS_FALSE); // populate the event data with the time value. This creates an absolute time and records if it was a a regular event.
            xmlFree(value); // remove the XML for the attribute only ??
            value=NULL;
        }
        if (*parent_start_time !=  0 && *parent_start_time != EHS_PLAYMANAGER_TIMER_UNDEFINED) (*pStartEvent)->parent_has_time = EHS_TRUE; /* This may be used to stop repeating nodes that have parents that are repeating */
        /* Now read the end time if we have one */
        value = (ehs_char*) xmlTextReaderGetAttribute(reader, "end");

        *pEndEvent = NULL; // we don't use this atall

        if ((value == NULL) && (*parent_end_time == 0 || *parent_end_time == EHS_PLAYMANAGER_TIMER_UNDEFINED))   // there is not end time implied for this
        {
            (*pEndEvent) = NULL;
            (*pStartEvent)->eventEndTime = 0;//EHS_PLAYMANAGER_TIMER_UNDEFINED; // this means we don't care - this is 2038 problem too!
        }
        else     /* we do have an end time implied or specific */
        {
            /* Yes this is when we have found an end time  in the SMIL, we add it to the start event .... */
            if (value != NULL)
            {
                parseWallClockValue(value, (*pStartEvent), EHS_TRUE); // now add the end event info to the start event..
                xmlFree(value);
            }
            else     //we do have a parent time -- see above
            {

#ifdef EHS_PLAYMAN_INHERITPARENTTIMES // we don't do this because it screws up repeats that fill lists - it probably does not change any other nehaviour?
//so we do need to do this so that sections )dynamic ones at least) will end OR we can traverse for waiting on events and end all those (perhaps?) too  though this might not work for parallel lists. .... TBC  what the problem with the former thing actually is...
                (*pStartEvent)->eventEndTime = *parent_end_time;
#else
                (*pStartEvent)->eventEndTime = 0;//EHS_PLAYMANAGER_TIMER_UNDEFINED;
#endif
            }
            /* Do increments if we are before parent time - e.g. it has been updated @todo this shold iterate until the future condition is true */
            /* @todo the following update should only occur if the repeat period of this is shorter than the parent - and not the last one!!!! - for now we only repeat when parents have no time set */
        }
        /* Check if we have a duration time and save this to be deal with at the actual start time */
        value = (ehs_char*) xmlTextReaderGetAttribute(reader, "dur");
        if (value == NULL)
        {
            //set this to undefined value as we now want to support duration=0.
            (*pStartEvent)->eventDuration = EHS_PLAYMANAGER_TIMER_UNDEFINED ; // we don't inherit parent durations - though we may look at them to calculate an end time.
        }
        else
        {
            (*pStartEvent)->eventDuration = parseDurationValue(value); // populate the event data with the time value. This creates an absolute time and records if it was a a regular event.
            xmlFree(value); // remove the XML for the attribute only ??
            value=NULL;
        }


        /* Do we have a repeat (loop) immediate flag? */
        ehs_char * repeat = (ehs_char*)xmlTextReaderGetAttribute(reader, "repeatCount");
        if (repeat)
        {
            if (EhsStrcmp(repeat,"indefinite") == 0 || EhsStrcmp(repeat,"-1") == 0)
            {
                (*pStartEvent)->repeatCount = EHS_PLAYMANAGER_LOOP_INDEFINITE;
            }
            else
            {
                (*pStartEvent)->repeatCount = EHS_PLAYMANAGER_LOOP_NONE;// just in case scannf fails.
                EhsSscanf(repeat,"%d",&(*pStartEvent)->repeatCount); // we don't do frations of a repeat to do duration like SMIL says you might want to do...
            }
            //(*pStartEvent)->repeat = EHS_PLAYMANAGER_REPEAT_IMMEDIATELY;
            //PBB we just use the enum of the schedule repeat so we update start and end times correctly
            if ((*pStartEvent)->repeatCount > 0 ) (*pStartEvent)->repeatCountDown  = (*pStartEvent)->repeatCount;
            else (*pStartEvent)->repeatCountDown = 0;
            xmlFree(repeat);
        }

        ehs_char * playseed = (ehs_char*) xmlTextReaderGetAttribute(reader, "orderSeed");
        if (playseed)   // this will trigger dynamic playlist shuffling on each play of the content if > 0
        {
            (*pStartEvent)->orderSeed = strtol (playseed, NULL,10);
            xmlFree(playseed);
        }
    }
    else     // we couldn't alloc a start event
    {
        ret = EHS_FALSE;
        EHSH_LOG_ERROR("Could not allocate start event memory");
    }
    EhsTPMutex_unlock(EhsTPMutex_playManager);
    return ret;
}

void	UpdateEventLayout(xmlTextReaderPtr reader, EhsPlayManagerEvent *pStartEvent)
{
    if (reader && pStartEvent)
    {
        //ehs_char* layoutId =
        pStartEvent->layoutSection = (ehs_char*) xmlTextReaderGetAttribute(reader, "xml:id");
    }
}

/*
* Called when a list type (seq, par or excl tag) is first found and the active node in the reader is the
* beginning of one of these list types.
*
* Extracts the header info and creates a begin entry for a time when it will be parsed.
* This is done mainly for the sequence type entries where the timing is only known once media is playing.
* par and excl types are done with similar "live " parsing to make things simpler.
*
* The input time is used if nothing is specified. This doesn't need to be updated because the whole sequence that it would apply to is dealt with here.
*/

ehs_uint16 CreateListEvent(xmlTextReaderPtr reader,
                           EhsPlayManagerType* pPlayManager, time_t * parent_start_time,time_t * parent_end_time,  EhsPlayManagerEvent * pParent,
                           EhsPlayManagerEventListTypeEnum ListEventType, ehs_uint32 nEntryID,EhsPlayManagerMarkedType mark)
{
    time_t begin, end;
    ehs_char* nextTag;
    ehs_char* value;
    xmlTextReaderPtr tempNewReaderPtr;
    ehs_uint16 ret = EHS_TRUE;
    ehs_char* innerXML = NULL;
    ehs_char* ehsInnerXML = NULL;
    ehs_uint32 innerXMLsize = 0;
    EhsPlayManagerEvent *pStartEvent=NULL;
    EhsPlayManagerEvent		*pEndEvent=NULL;

    innerXML = (ehs_char*) xmlTextReaderReadOuterXml(reader); //We use OuterXml to keep start and end tags

    xmlTextReaderNext(reader); // go into sublevel list
    /*
     * Extract the sequential section to be parsed when it's start time occurs (could be now!).
     *  Get the first entry and store it in the event handler structure to parsed as we progress through the sequence
     *  Start reading the sequence entries - Get the XML that represents this sequence
     */
    if (innerXML != NULL)
    {
        innerXMLsize = strlen(innerXML);
        ehsInnerXML = innerXML; //@todo this is a remnant of legacy copying from xml mem to EHS mem - can be refactored
        tempNewReaderPtr = xmlReaderForMemory(ehsInnerXML, innerXMLsize, NULL, NULL, XML_PARSE_RECOVER);
        //don't ask...
        if (tempNewReaderPtr)
        {
            if (xmlTextReaderRead(tempNewReaderPtr) == 1)  // got to the new bit which is the next node - do read to expose
            {
                CreateTimeEvent(tempNewReaderPtr, &pStartEvent, &pEndEvent,parent_start_time, parent_end_time, pParent);//@todo we need parent start and end time... create and populate the events and time data from the attribs or default to the parent time (which maybe now).
                UpdateEventLayout(tempNewReaderPtr,pStartEvent);
                if (xmlTextReaderRead(tempNewReaderPtr) != 1) EHSH_LOG_ERROR("Could not readnext in sublist"); // and move on to next
            }
        }
        else
        {
            EHSH_LOG_ERROR("Could not create reader for sublist");
        }
        if (pStartEvent)
        {
            pPlayManager->ListUniqueID++;
            pStartEvent->ListUniqueID = pPlayManager->ListUniqueID;
            pStartEvent->type = EHS_PLAYMANAGER_EVENT_NEWLIST; /* this used by fire to decide if more parsing is required. */
            pStartEvent->eventListType = ListEventType;
            pStartEvent->pPlaylist = ehsInnerXML;
            pStartEvent->playlistSize = innerXMLsize; //these are needed in addition to the parser if we need to reset the playlist parser
            pStartEvent->memoryReader = tempNewReaderPtr;
            pStartEvent->SendersObjectUniqueID = nEntryID; /* assign the node ID so that end events are posted to waiting tasks. */
            pStartEvent->marked = mark;
            /* And create a new reader */
            addToEventList(pStartEvent, pPlayManager);
        }
        else
        {
            xmlTextReaderClose(tempNewReaderPtr);
            xmlFreeTextReader(tempNewReaderPtr);
            EHSH_LOG_ERROR("Couldn't create pStartEvent\n");
        }
    }
    else
    {
        EHSH_LOG_ERROR("seqTag(): Failed to get the innerXML, probably means the playlist is corrupt\n");
    }
    return ret;
}

ehs_uint16 unknownTag(xmlTextReaderPtr reader, EhsPlayManagerType* pPlayManager, time_t * parent_start_time, time_t * parent_end_time,  EhsPlayManagerEvent * pParent, ehs_uint32 nEntryID)
{
    ehs_char* nextTag = NULL;
    int nodeType;
    ehs_bool ret = EHS_FALSE;
    /* Read off this node */
    if (xmlTextReaderRead(reader) == 1) ret = EHS_TRUE;

    return ret;
}

ehs_uint16 seqTag(xmlTextReaderPtr reader, EhsPlayManagerType* pPlayManager,
                  time_t * parent_start_time, time_t * parent_end_time,  EhsPlayManagerEvent * pParent, ehs_uint32 nEntryID,EhsPlayManagerMarkedType mark)
{
    return CreateListEvent(reader, pPlayManager, parent_start_time, parent_end_time, pParent, EHS_PLAYMANAGER_EVENTLIST_SEQ,  nEntryID,mark); // just do par header. Don't parse everything inside.
}

/* returns the number of items read
 * The scheduler will provide a seed, typically date and the time of day
 * and also the day of year is used to further randomise the sequence in
 * predicable way across devices with the same schedule.
 * Writes dynamic playlist into XML arg.
 * If the pathToRefFile is null and an event pointer to a sequence is provided instead
 * then this is used to generate the dynamic playlist instead.
 * If reading playlist smil section this function will notreturn the reader to any
 * previous state, so it is assumed it
 * will be a copy that will be discarded after being read here
 * */
ehs_uint32 parseListSnippet(ehs_char ** XML, ehs_char* pathToRefFile,ehs_uint32 orderSeed,ehs_char* volume, ehs_sint32 repeatCount,xmlTextReaderPtr sequenceEventReader)
{
    ehs_char * snippet_array[4096];
    ehs_uint32 count = 0, write_index = 0;
    ehs_uint32 size_read = 0 ;
    ehs_uint32 total_size = 0 ;
    ehs_char * _XML = NULL;
    ehs_sint32 node_type = 0;
    /* for reading from a playlist sourced section */
    ehs_char* ehsInnerXML = NULL;
    ehs_uint32 innerXMLsize = 0;

    ehs_bool readFromFile = EHS_TRUE;
    *XML=NULL;
    if ( pathToRefFile == NULL && sequenceEventReader != NULL) readFromFile = EHS_FALSE;

    if (pathToRefFile != NULL || sequenceEventReader != NULL)
    {
        ehs_FILE * file = NULL;
        if (readFromFile)
            file = Ehs_UserFopen(pathToRefFile,"r");
        if ( (readFromFile && file) || (!readFromFile && sequenceEventReader))
        {
            ehs_uint32 i;
#ifdef EHS_DONT_USE_EHS_MANAGED_HEAP
            ehs_char * tempbuffer = NULL;
            if (readFromFile)
            {
                tempbuffer = malloc(4096);
                tempbuffer[0]='\0';
            }
#else
            if (readFromFile)
            {
                tempbuffer = EhsHMem_tempAlloc(4096); // assume we won't read a file path bigger than this
                tempbuffer[0]='\0';
            }
#endif


            while (count < 4096 && (tempbuffer || !readFromFile) && (file || sequenceEventReader ) )
            {
                if (readFromFile)
                {
                    if (EhsFscanf(file,"%4090[^\n]",tempbuffer) <= 0) break;
                    EhsFscanf(file,"%*[\n]");// read off the following for next time round (if it is there...
                    size_read=EhsStrlen(tempbuffer);
                }
                else     // read from a section reference otherwise
                {
                    if (sequenceEventReader && xmlTextReaderRead(sequenceEventReader) == 1)  // got to the new bit which is the next node - do read to expose
                    {
                        tempbuffer = (ehs_char*) xmlTextReaderReadOuterXml(sequenceEventReader); // read the whole SMIL node
                        size_read = EhsStrlen(tempbuffer);

                        node_type = xmlTextReaderNodeType(sequenceEventReader);
                        if (node_type == 15/* end node */ )   // check if it's the end of the list
                        {
                            if (tempbuffer) xmlFree(tempbuffer); //delete each of these creaed by reding (outer) node
                            tempbuffer = NULL;
                            break;
                        }
                        else if (node_type == 14 /*SignificantWhiteSpace*/ || node_type == 13 /* other white space */ )
                        {
                            if (tempbuffer) xmlFree(tempbuffer); //delete each of these creaed by reding (outer) node
                            tempbuffer = NULL;
                            continue;
                        }

                    }
                    else
                    {
                        tempbuffer = NULL;
                        size_read = 0;
                        break; // no more to read, so done!
                    }
                }
#ifdef EHS_DONT_USE_EHS_MANAGED_HEAP
                snippet_array[count] = malloc(size_read+1);
#else
                snippet_array[count]=EhsHMem_tempAlloc(size_read+1);
#endif

                if (snippet_array[count])
                {
                    EhsStrcpy(snippet_array[count],tempbuffer); // we trust this to be null terminated.
                    count++;
                    if (volume != NULL) total_size+=(size_read + sizeof("soundLevel=\"100dB\" ")); // add the mximum size the volume is likely to be
                    else total_size+=size_read;
                }
                else
                {
                    EHSH_LOG_ERROR("Out of Memory reading dynamic sesequence %s",pathToRefFile);
                    break; // may as well try with what we have ...
                }
                if (!readFromFile)
                {
                    if (tempbuffer) xmlFree(tempbuffer); //delete each of these creaed by reding (outer) node
                    tempbuffer = NULL;
                }

            }
#ifdef EHS_DONT_USE_EHS_MANAGED_HEAP
            if (tempbuffer) free(tempbuffer);
            if ( (*XML = malloc(total_size + 4096)) )
            {
#else
            if (tempbuffer) EhsHMem_tempFree(tempbuffer); // allocate space to copy in snippets andd addd sequence section header
            if ( (*XML = EhsHMem_tempAlloc(total_size + 4096)) )
            {
#endif
                int charsWritten;
                /* Now creat our List */
                ehs_uint32 j,jj = orderSeed; /* use a non-zero code seed too - this must be the same for all devices that need to sync */
                ehs_char * temp_snippet_array = NULL;
                _XML= *XML;

                charsWritten = EhsSprintf(_XML,"<seq repeatCount=\"%d\">\n", repeatCount);
                if (charsWritten>=0) write_index+=charsWritten; //write_index+=sizeof("<seq>\n");write_index--;

                for (i=0; i<count; i++)
                {
                    /* generate a random sequence - Fisher–Yates shuffle - with extra last value swap for bad rand() functions */
                    j = pseudorand(&jj,count-1);
                    temp_snippet_array = snippet_array[i];
                    snippet_array[i] = snippet_array[j];
                    snippet_array[j] = temp_snippet_array;
                }
                /* write out the random sequence */

                for (i=0; i<count; i++)
                {
                    if (volume != NULL)   // just write the first part of the snippet array and appaend the volume smil
                    {
                        ehs_char * _smilEnd = EhsStrstr(snippet_array[i],"/>");
                        if (_smilEnd != NULL)
                        {
                            *_smilEnd = '\0';//remove the smil end
                            write_index += EhsSprintf(&_XML[write_index],"%s soundLevel=\"%s\" />\n",snippet_array[i],volume);
                        }
                        else   // don't bother trying something is bad, but what can you do eh?
                        {
                            write_index += EhsSprintf(&_XML[write_index],"%s \n",snippet_array[i]);
                        }
                    }
                    else
                    {
                        write_index += EhsSprintf(&_XML[write_index],"%s \n",snippet_array[i]);
                    }

#ifdef EHS_DONT_USE_EHS_MANAGED_HEAP
                    free(snippet_array[i]);
#else
                    EhsHMem_tempFree(snippet_array[i]);
#endif
                }
                EhsSprintf(&_XML[write_index],"</seq>");
                write_index+=sizeof("</seq>");
                write_index--; // for the null terminator
            }
            if (readFromFile && file) EhsFclose(file);
        }
        else
        {
            EHSH_LOG_ERROR("Couldn't open %s",pathToRefFile);
        }
    }
    else
    {
        EHSH_LOG_ERROR("No path to dynamic playlist file");
    }
    return write_index;

}

/* Called at the point the seqref event is fired
 * This calls to get the XML and binds it to the event
 * Note for seqreferences read from file the event list passes here is typically a newly generted one (without a reader object)
 * Bit for normal sequences it will be an existing event already with a meory reader, that will need to be destroyed, before being replaced
 */

ehs_bool populateDynSeqList(EhsPlayManagerType* pPlayManager, EhsPlayManagerEvent *pEvent,time_t currentt,ehs_bool bReadFromFle)
{
    ehs_uint32 ret;
    ehs_char * XML = NULL;
    ehs_uint32 seed;
    struct tm time_day,*time_day_ptr;

    if (pEvent->eventTime !=0 && pEvent->eventTime != EHS_PLAYMANAGER_TIMER_UNDEFINED)
    {
        time_day_ptr = localtime_r(&(pEvent->eventTime), &time_day); // use the start time as this will indicate precise expected start time
    }
    else  // othwerwise use the measured start time
    {
        time_t temp_currentt = currentt+30;
        time_day_ptr = localtime_r(&temp_currentt, &time_day); // add 30s so we round to the nearest minute
    }

    if (time_day_ptr)
    {
        seed = pEvent->orderSeed + time_day_ptr->tm_yday + time_day_ptr->tm_hour + time_day_ptr->tm_min;
    }
    else
    {
        EHSH_LOG_ERROR("Could not get day of the year for the random number generator");
        seed = pEvent->orderSeed;

    }
    if (bReadFromFle)
        ret = parseListSnippet( &XML,pEvent->pathToRefFile,seed,pEvent->soundLevel,pEvent->repeatCount,NULL);
    else // otherwise use the section list for this sequence event
        ret = parseListSnippet( &XML,NULL,seed,pEvent->soundLevel,pEvent->repeatCount,pEvent->memoryReader);
    //todo we  may also support references to other sequences other than immediate list in the future too.
    if (ret && XML)
    {
        // replace the memory reader with the stub for the generated XML
        if (pEvent->memoryReader)
        {
            xmlTextReaderClose(pEvent->memoryReader);
            xmlFreeTextReader(pEvent->memoryReader);
        }
        pEvent->memoryReader = xmlReaderForMemory(XML, ret, "/", NULL, XML_PARSE_RECOVER);
        if (pEvent->memoryReader)
        {
            xmlTextReaderRead(pEvent->memoryReader);

        }
        else
        {
            EHSH_LOG_ERROR("Could not assign memory reader");
        }
    }
    else EHSH_LOG_ERROR("Could not get a list to randomise");
    pEvent->dynamicInnerXml = XML; // need to assign this to clean it up when the event is destroyed

    if (ret > 0 ) return EHS_TRUE;
    else return EHS_FALSE;
}

/*
 * Called when this section is dicovered and not when fired
 *
 * Near duplication of createNewList Event, but does a few thigns a bit different
 * Creates a (potentially randomied) sequence from an external SMIL snippet
 *
 * This should work mostly like a sequence setion, but will swap in a (randomised media src list body from the specificed file
 */

ehs_uint16 seqrefTag(xmlTextReaderPtr reader, EhsPlayManagerType* pPlayManager,
                     time_t * parent_start_time, time_t * parent_end_time,  EhsPlayManagerEvent * pParent, ehs_uint32 nEntryID,EhsPlayManagerMarkedType mark)
{

    //time_t * parent_start_time = &pParentEvent->eventTime;
    //time_t * parent_end_time = &pParentEvent->eventEndTime;
    //EhsPlayManagerEvent * pParent = pParentEvent;
    //EhsPlayManagerEventListTypeEnum ListEventType = pParentEvent->eventListType; // should be sequence I think?
    //ehs_uint32 nEntryID = pPlayManager->ObjectUniqueID++; // we need to do this dynamically because we might not want to use the caller's?
    //EhsPlayManagerMarkedType mark = pParentEvent->marked;
    //ehs_char* nextTag;
    //ehs_char* value;
    ehs_char* filePath, *soundLevel, *playseed = NULL;

    xmlTextReaderPtr tempNewReaderPtr;
    ehs_uint16 ret = EHS_TRUE;
    ehs_char* innerXML = NULL;
    ehs_char* ehsInnerXML = NULL;
    ehs_uint32 innerXMLsize = 0;
    EhsPlayManagerEvent *pStartEvent=NULL;
    EhsPlayManagerEvent		*pEndEvent=NULL;

    // --------------------------------------


    innerXML = xmlTextReaderReadOuterXml(reader); //We use OuterXml to keep start and end tags

    if (innerXML != NULL)
    {
        innerXMLsize = strlen(innerXML);
        ehsInnerXML = innerXML; //@todo this is a remnant of legacy copying from xml mem to EHS mem - can be refactored
        tempNewReaderPtr = xmlReaderForMemory(ehsInnerXML, innerXMLsize, NULL, NULL, XML_PARSE_RECOVER);
        //don't ask...
        if (tempNewReaderPtr)
        {
            //#error -assuming we are creating a duplicate setion header event here? or was the original never creatd as an event?
            if (xmlTextReaderRead(tempNewReaderPtr) == 1)  // got to the new bit which is the next node - do read to expose section info and read metadata
            {
                CreateTimeEvent(tempNewReaderPtr, &pStartEvent, &pEndEvent,parent_start_time, parent_end_time, pParent);//@todo we need parent start and end time... create and populate the events and time data from the attribs or default to the parent time (which maybe now).
                UpdateEventLayout(tempNewReaderPtr,pStartEvent);
                //if (xmlTextReaderRead(tempNewReaderPtr) != 1) EHSH_LOG_ERROR("Could not readnext in sublist"); // and move on to next
            }
            else
            {
            }
        }
        else
        {
            EHSH_LOG_ERROR("Could not create reader for sublist");
        }
        if (pStartEvent->eventTime != -1) 	/* in case we tried to read the clock data and got garbage.*/
        {
            /* process src attribute and populate new structure */
            pPlayManager->ListUniqueID++;
            pStartEvent->ListUniqueID = pPlayManager->ListUniqueID;
            /* SOme dynamic list specific stuff here */
            filePath = xmlTextReaderGetAttribute(reader, "href"); //@todo refactor out these local vars
            playseed = xmlTextReaderGetAttribute(reader, "orderSeed");
            if (playseed) pStartEvent->orderSeed = strtol (playseed, NULL,10); //@todo refactor out these local vars
            pStartEvent->pathToRefFile = filePath;
            soundLevel = xmlTextReaderGetAttribute(reader, "soundLevel");

            pStartEvent->type = EHS_PLAYMANAGER_EVENT_NEWDYNAMICLIST; // we need to know it'snot an ordinary start node when it fires.
            /* add some usual stuff again */
            pStartEvent->soundLevel = soundLevel;

            pStartEvent->SendersObjectUniqueID = nEntryID; // todo check this - It should get overwritten?
            pStartEvent->marked = mark; /* inherit parent mark - in case we are getting a new playlist rigt away?*/

            pStartEvent->eventListType = EHS_PLAYMANAGER_EVENTLIST_SEQREF;
            pStartEvent->pPlaylist = ehsInnerXML;
            pStartEvent->playlistSize = innerXMLsize; //these are needed in addition to the parser if we need to reset the playlist parser
            pStartEvent->memoryReader = tempNewReaderPtr;
            addToEventList(pStartEvent, pPlayManager); /* and add to the event list */
        }
        else
        {
            xmlTextReaderClose(tempNewReaderPtr);
            xmlFreeTextReader(tempNewReaderPtr);
            EHSH_LOG_WARNING("WARNING: begin time not in the correct format");
        }

    }
    else
    {
        EHSH_LOG_ERROR("Could not get memory allocation for the event");
    }
    if (playseed) xmlFree(playseed);
    // don't read the ext node as these are normally self closing.
    if (xmlTextReaderRead(reader) == 1) return EHS_TRUE; //- this is just to check the sequence has something in?
    else return EHS_FALSE;

}


/*
 * Called when an excl tag entry is found
 *
 * todo this couls parent flags to work out the excl logic
 */
ehs_uint16 exclTag(xmlTextReaderPtr reader, EhsPlayManagerType* pPlayManager,
                   time_t * parent_start_time,time_t * parent_end_time,  EhsPlayManagerEvent * pParent, ehs_uint32 nEntryID,EhsPlayManagerMarkedType mark)
{
    /* @todo Possibly we need to do something here to indicate that the following sequence events are going to exclusive rather than parrallel. Haven't figured that out yet. */

    return CreateListEvent(reader, pPlayManager, parent_start_time, parent_end_time, pParent, EHS_PLAYMANAGER_EVENTLIST_EXCL,  nEntryID,mark); // just do par header. Don't parse everything inside.
}

/*
 * We have found a parallel tag - We iterate through all in the node and continue.
 * Then we need to look for the structural ones and continue.
 * We can call this recursively if we find another parallel.
 *
 */

ehs_uint16 parTag(xmlTextReaderPtr reader, EhsPlayManagerType* pPlayManager,time_t * parent_start_time, time_t * parent_end_time,  EhsPlayManagerEvent * pParent, ehs_uint32 nEntryID,EhsPlayManagerMarkedType mark)
{
    //We might want to parse the whole thing here and create immediates, but for now lets do the same as for sequentials.
    return CreateListEvent(reader, pPlayManager, parent_start_time, parent_end_time, pParent, EHS_PLAYMANAGER_EVENTLIST_PAR,  nEntryID,mark); // just do par header. Don't parse everything inside.
}

/*
 * This is called when we have found an Object (i.e. media) tag in a ;i
 * This is fully parsed and the event structure is all that is needed to fire it.
 * This does notfire the event even if it is ready. Instead it is added
 * to the events list, even if no  wallclock value is found.
 * This is almost a duplication of the CreateListEvent FUnction
 */

static ehs_bool refObjectTag(xmlTextReaderPtr reader,
                             EhsPlayManagerType* pPlayManager, time_t * parent_start_time, time_t * parent_end_time,  EhsPlayManagerEvent * pParent, ehs_uint32 nEntryID, ehs_char * tagType, EhsPlayManagerMarkedType mark)
{
    ehs_char* value, *class, *type, *region, * repeat, *soundLevel, *fade;
    ehs_char* src;
    ehs_char* nextTag;
    ehs_uint32 valueSize = 0;
    /* Need to create an event comprising of the src and the begin time */
    EhsPlayManagerEvent *pEndEvent, *pStartEvent;//alocated in EhsHMem_tempAlloc
    /* Read any specific time info for the file */

    if (CreateTimeEvent(reader, &pStartEvent, &pEndEvent, parent_start_time, parent_end_time, pParent ))  //populate the time from the attribs or default to the parent time (which maybe now).
    {
        if (pStartEvent->eventTime != -1) 	/* in case we tried to read the clock data and got garbage.*/
        {
            /* process src attribute and populate new structure */
            value = (ehs_char*) xmlTextReaderGetAttribute(reader, "src"); //@todo refactor out these local vars
            class = xmlTextReaderGetAttribute(reader, "class");
            soundLevel = xmlTextReaderGetAttribute(reader, "soundLevel"); // todo we should convert this to int here like we do for fade...
            type = xmlTextReaderGetAttribute(reader, "type"); //rtsp/http/ftp @todo store this for caching and new output port required.
            region = xmlTextReaderGetAttribute(reader, "region");
            fade = xmlTextReaderGetAttribute(reader, "fade");
            if (fade)
            {
                pStartEvent->fade = atoi(fade);
                xmlFree(fade);
            }
            if (EhsStrcmp(tagType,"smilText") == 0 /*&& xmlTextReaderHasValue(reader) == 1 */)
            {
                if (xmlTextReaderRead(reader) == 1)
                {
                    const ehs_char* text =  xmlTextReaderConstValue(reader);
                    if (text) EhsStrncpy(pStartEvent->smilText,text,EHS_PLAYMANAGER_MAX_XML_STRINGLENGTH);
                    else pStartEvent->smilText[0] = '\0';
                    xmlTextReaderRead(reader);
                }
            }
            pStartEvent->tagType = tagType; // this is allocated strangely by XML reader

#ifdef EHS_DEBUG_SMILPARSER
#endif
            pStartEvent->pMediaSource = value; 	// don't need to copy -assign allocated pointers
            pStartEvent->class = class; 		// class this is used by the app to divert stuff around.
            pStartEvent->soundLevel = soundLevel;
            pStartEvent->srcType=type;		 //src type (not tag type!)
            pStartEvent->region = region; 		// label for the region

            pStartEvent->type = EHS_PLAYMANAGER_EVENT_OBJECT;
            pStartEvent->SendersObjectUniqueID = nEntryID;
            pStartEvent->marked=mark; /* inherit parent mark */
            addToEventList(pStartEvent, pPlayManager); /* and add to the event list */
        }
        else
        {
            EHSH_LOG_WARNING("WARNING: begin time not in the correct format");
        }

    }
    else
    {
        EHSH_LOG_ERROR("Could not get memory allocation for the event");
    }
    if (xmlTextReaderRead(reader) == 1) return EHS_TRUE;
    else return EHS_FALSE;
}

ehs_bool  AddParallelListMustComplete(EhsPlayManagerEvent * parEventList, ehs_uint32 ID);

/***
 * @brief This is called and pass the body of a new playlist.
 *
 * It selects the correct parser for an element
 * The element parser will parse all untill it's end tag - so it will operate within one level of the tree per call.
 * This function can be called recursively for each level of the subtree
 *
 * Returns status of node - so that the caller can delete events for list types if appropriate.
 * If an end node is found then the name is returned in element_type.
 * If no recognised node was found then element_type is an empty string.
 *
 * element_type returns the type of element that was found - in case the caller is interested - if not then set this to null and it will be ignored.
 *
 * This function is also passed the default start time (i.e. of the parent) if no start time is prescribed).
 * If the object type is a structural element (i.e. a parent) then the time is updated in the return value.
 *
 * sets pEvent to NULL if it has been removed in any processing
 *
 * todo element_type paramters doesn't seem to be used - should remove.
 */

EhsPlayManagerSMILNodeActionEnum ProcessNextSMILbody(xmlTextReaderPtr reader,EhsPlayManagerEvent **ppEvent,EhsPlayManagerType* pPlayManager,ehs_char * element_type, time_t * parent_start_time,time_t * parent_end_time,  EhsPlayManagerEvent * pParent, time_t currentt)
{
    ehs_char* nextTag = NULL;
    int nodeType;
    ehs_bool event_created = EHS_FALSE; // used to indicate if an event was created from the node
    EhsPlayManagerSMILNodeActionEnum ret = EHS_PLAYMANAGER_SMIL_CHUNK_REMOVE; // default we will remove
    EhsPlayManagerMarkedType mark = LIST_DEFAULT;
    ehs_uint32 lastObjectUniqueID; // keep this as we want to record the last sequence listID before parallel content is read.
    EhsPlayManagerEvent *pEvent;

    if (ppEvent != NULL) pEvent =*ppEvent;
    else pEvent = NULL;

    if (pEvent)
        mark = pEvent->marked;

    nextTag = getNextTag(reader, &nodeType); /* 2011-06-28 PBB - Try removing this getNextTag call to fix missing first element of parallel playlists. */// this is returned so be careful to tidy up !



    if (element_type)
        EhsStrcpy(element_type, "");
    if (nextTag != NULL)
    {
        lastObjectUniqueID = pPlayManager->ObjectUniqueID;
        pPlayManager->ObjectUniqueID++; // get the ID ready for this event
#ifndef EHS_ANDROID
        EhsHThread_yield();
#endif
        if (nodeType == EHS_NODE_TYPE_START_ELEMENT)   // this the beginning of an XML element
        {
            /* Test for what type of item we've parsed and create the event types for them*/
            if ((strcmp("excl", nextTag) == 0))
            {
                ret = EHS_PLAYMANAGER_SMIL_CHUNK_KEEP;
                exclTag(reader, pPlayManager, parent_start_time,parent_end_time,pParent, pPlayManager->ObjectUniqueID,mark); //read the attributes for this and then come back here to read the contents.
                if (ret && element_type)
                    EhsStrcpy(element_type, "excl");
                event_created=EHS_TRUE;
            }
            else if (strcmp("par", nextTag) == 0)
            {
                ret = EHS_PLAYMANAGER_SMIL_CHUNK_KEEP;
                parTag(reader, pPlayManager, parent_start_time,parent_end_time,pParent, pPlayManager->ObjectUniqueID,mark); //read the attributes for this and then come back here to read the contents.
                if (ret && element_type)
                    EhsStrcpy(element_type, "par");
                event_created=EHS_TRUE;
            }
            else if (strcmp("seq", nextTag) == 0)
            {
                ret = EHS_PLAYMANAGER_SMIL_CHUNK_KEEP;
                seqTag(reader, pPlayManager, parent_start_time,parent_end_time,pParent, pPlayManager->ObjectUniqueID,mark); //read the attributes for this and post the section to be parsed by the scheduler. return here to parse the remaining SMIL
                if (ret && element_type)
                    EhsStrcpy(element_type, "seq"); //@todo shouldn't this be done in the tag reader (e.g. seqTag)?
                event_created=EHS_TRUE;
            }
            else if (strcmp("seqref", nextTag) == 0)     // Include a SMIL snippet use a date-rel-token
            {
                ret = EHS_PLAYMANAGER_SMIL_CHUNK_KEEP;
                seqrefTag(reader, pPlayManager, parent_start_time,parent_end_time,pParent, pPlayManager->ObjectUniqueID,mark); //
                if (ret && element_type)
                    EhsStrcpy(element_type, "seqref"); //@todo shouldn't this be done in the tag reader (e.g. seqTag)?
                /* Test for new Object to be parsed and an firable event to be produced */
                event_created=EHS_TRUE;
            }
            else if (isaSmilOtherImmediateElement(nextTag)   // read an immediate object tag and create an event for it.
                     || isaSmilRefElement(nextTag))
            {
                refObjectTag(reader, pPlayManager, parent_start_time,parent_end_time,pParent, pPlayManager->ObjectUniqueID,nextTag,mark); // this will not return a new time as it is not a parent, pass in the list event so it can ben tagged with the ID when playing.
                ret = EHS_PLAYMANAGER_SMIL_CHUNK_KEEP;
                event_created=EHS_TRUE;
            }
            else     /* this something we don't know or care about (e.g. body). IMPORTANT - THIS MEANS WE DON'T SPIN ON TINGS WE DON'T UNDERSTAND */
            {
                if (reader)
                {
                    ret = unknownTag(reader, pPlayManager, parent_start_time,parent_end_time,pParent, pPlayManager->ObjectUniqueID); //read the attributes for this and post the section to be parsed by the scheduler. return here to parse the remaining SMIL
                    ret = EHS_PLAYMANAGER_SMIL_CHUNK_KEEP;
                }
                if (ret && element_type)
                {
                    EhsStrcpy(element_type, "unknown");
                    ret = EHS_PLAYMANAGER_SMIL_CHUNK_KEEP;
                }
                event_created=EHS_FALSE; /* We don't want any waits on these */
            }
            /* We need to assign a unique ID to every entry in SMIL: objects & list entries. */
            if (event_created && pEvent)   // new node created AND we must have a parent event - We don't have one for the first list/object.
            {
                if (pEvent->eventListType == EHS_PLAYMANAGER_EVENTLIST_SEQ || pEvent->eventListType == EHS_PLAYMANAGER_EVENTLIST_SEQREF)   // for sequences we need to give the parent the ID to know when to read the next..
                {
                    pEvent->WaitingForObjectUniqueID = pPlayManager->ObjectUniqueID;// we set this to the index the child has just been set to. (not pEvent->SendersObjectUniqueID;
                }
                else if (pEvent->eventListType == EHS_PLAYMANAGER_EVENTLIST_PAR)
                {
                    pEvent->WaitingForObjectUniqueID = 0;// 0; // do this for parallels, so it only is asserted after all it's entries are complete..
                    AddParallelListMustComplete(pEvent,pPlayManager->ObjectUniqueID); // par types need to know all the
                }
                else if (pEvent->eventListType == EHS_PLAYMANAGER_EVENTLIST_EXCL)
                {
                    pEvent->WaitingForObjectUniqueID = 0;// @todo we don't do excl properly - we do the same as parallel
                    AddParallelListMustComplete(pEvent,pPlayManager->ObjectUniqueID); // par types need to know all childrens activities
                }
            }
        } //end of this being a node start type, remaining is for an end node
        else if (pEvent && nodeType == EHS_NODE_TYPE_END_ELEMENT)
        {
            if ((strcmp("seq", nextTag) == 0) || (strcmp("par", nextTag) == 0) || (strcmp("excl", nextTag) == 0) )   // Only deal with closed list tags - direct ref end tags are ignored.
            {
                if (pEvent->eventListType==EHS_PLAYMANAGER_EVENTLIST_SEQ || pEvent->eventListType == EHS_PLAYMANAGER_EVENTLIST_SEQREF)
                {
                    /* If the sequence as a member of a parallel list then notify it's closed. */
                    /* handle the end of a sequence list */
                    /* if we are a dynamic list we need to give the parent list a kick to get it to end properly rather than linger */
                    if (pEvent->pParent && pEvent->pParent->eventListType == EHS_PLAYMANAGER_EVENTLIST_SEQREF)
                    {
                        HandleEndEvent(pPlayManager,&pEvent->pParent,EHS_TRUE,currentt); // update start and stop times,pEvent->pParent, pPlayManager, element_type, &(pEvent->pParent->eventTime),&(pEvent->pParent->eventEndTime),pEvent->pParent->pParent,currentt); // we will just read an entry here
                    } // todo REVIEW - should tis have an else? No reason to handle end event twice I would expect?
                    else if  (pEvent->eventEndTime == 0 || pEvent->eventEndTime == EHS_PLAYMANAGER_TIMER_UNDEFINED  )   /* Only fire an end event if there is no end time - if there is the ticker will assert an expiry instead*/
                    {
                        HandleEndEvent(pPlayManager,ppEvent,EHS_TRUE,currentt); // update start and stop times
                    }
                    else  if  ((pEvent->eventEndTime == 0 || pEvent->eventEndTime == EHS_PLAYMANAGER_TIMER_UNDEFINED ) && pEvent->repeatCountDown > 0 )
                    {
                        ret = EHS_PLAYMANAGER_SMIL_CHUNK_KEEP;
                    }
                    else if (*ppEvent == NULL)
                    {
                        pEvent = NULL;
                    }
                    else
                    {
                        HandleEndEvent(pPlayManager,ppEvent,EHS_TRUE,currentt);
                    }
                }
                else if (pEvent->eventListType==EHS_PLAYMANAGER_EVENTLIST_PAR)   // just cos we got to the end doesn't mean all stuff has played or ended..
                {
                    //find any pending events..
                    pEvent->WaitingForObjectUniqueID=pPlayManager->ObjectUniqueID; // Ignored for parallels as these are notified instead, but need to set this to not execute with a non-zero number that won't be fired by an object.
                    ret = EHS_PLAYMANAGER_SMIL_CHUNK_KEEP; //
                }
                else if (pEvent->eventListType == EHS_PLAYMANAGER_EVENTLIST_EXCL)     // just cos we got to the end doesn't mean all stuff has played or ended..
                {
                    //find any pending events..
                    pEvent->WaitingForObjectUniqueID = pPlayManager->ObjectUniqueID; // need to set this to not execute with a non-zero number that won't be fired by an object.
                    ret = EHS_PLAYMANAGER_SMIL_CHUNK_KEEP;
                }
                else
                {
                    if (element_type) EhsStrcpy(element_type, "end"); // make a copy in case the caller wants to check what got closed.
                    ret = EHS_PLAYMANAGER_SMIL_CHUNK_ENDED;
                }
            }
            else
            {
                ret = EHS_PLAYMANAGER_SMIL_CHUNK_ENDED;
            }
        }
        else
        {
            ret = EHS_PLAYMANAGER_SMIL_CHUNK_REMOVE;
        }
    } // end of not NULL
    else
    {
        ret = EHS_PLAYMANAGER_SMIL_CHUNK_CANTREAD;
    }
    return ret;
}

/*
 * Called when SMIL header close tag is expected. This calls the next SMIL body read function.
 */

static void closingHeadTag(xmlTextReaderPtr reader, EhsPlayManagerType* pPlayManager)
{
    ehs_char* nextTag;
    int nodeType;
    time_t parent_start_time = 0x0;
    time_t parent_end_time = EHS_PLAYMANAGER_TIMER_UNDEFINED; // end time is 2038
    EhsPlayManagerSMILNodeActionEnum SMIL_chunk_property;
    ehs_char element_type[128];//@todo this is nasty - how big?.
    nextTag = getNextTag(reader,&nodeType);
    if (xmlTextReaderRead(reader) != 1) EHSH_LOG_ERROR("closingHeadTagread failed"); // @todo flag this failing properly
    if (nextTag != NULL)
    {
        if ((strcmp("body", nextTag) == 0) && (nodeType== 1))
        {
            //@todo this should iterate untill we get a return value of REMOVE - which means we have done all in the head list.
            SMIL_chunk_property = ProcessNextSMILbody(reader, NULL, pPlayManager,element_type, &parent_start_time, &parent_end_time, NULL,0);// this parses the entire file (storing sequences in the events for the sequencces).
            //@todo do we need to clean this up (Remove Event)?
        }
        else
        {
            EHSH_LOG_ERROR("closingHeadTag: expected opening body. found %s", nextTag);
        }
    }
    else
    {
        EHSH_LOG_ERROR("closingHeadTag: failed to get nextTag");
    }
}


/** Layout management
 *
 */
void Layout_InitLayout(EhsPlayManagerLayout * layout,ehs_uint32 id)
{
    layout->id = id;
    layout->regions = NULL;
    layout->next = NULL;
    layout->top = 0;
    layout->top = 0;
    layout->top = 0;
    layout->top = 0;
    layout->top = 0;
    layout->json[0]='\0';
}

/** Layout management
 * @brief Creates a new root info object if not set and assigns values to it.
 */
static void Layout_SetLayoutRootInfo(EhsPlayManagerType* pPlayManager,ehs_uint16 width, ehs_uint16 height)
{

    if (pPlayManager->layoutInfo == NULL)
    {
        pPlayManager->layoutInfo = EhsHMem_tempAlloc(sizeof(EhsPlaymanegerLayoutRoot));
        pPlayManager->layoutInfo->EhsPlayManagerLayout = NULL;
        pPlayManager->layoutInfo->EhsPlayManagerTextStyle = NULL;
    }
    if (pPlayManager->layoutInfo)
    {
        pPlayManager->layoutInfo->rootWidth = width;
        pPlayManager->layoutInfo->rootHeight = height;
    }
}


/** Layout management
 * @brief returns NULL if not found and create is not set (or out of memory).
 */

static EhsPlayManagerLayout* Layout_GetCreateLayoutRef(EhsPlayManagerType* pPlayManager, ehs_uint32 id, ehs_bool create)
{
    EhsPlayManagerLayout * ret = NULL;
    ehs_bool found = EHS_FALSE;
    EhsPlayManagerLayout * layout = NULL;
    EhsPlayManagerLayout * lastLayout = NULL;

    if (pPlayManager->layoutInfo == NULL)
    {
        Layout_SetLayoutRootInfo(pPlayManager,0, 0); // create the root info structure.
    }

    if (pPlayManager->layoutInfo)
    {
        layout = pPlayManager->layoutInfo->EhsPlayManagerLayout;

        while (layout)
        {
            if (layout->id == id )
            {
                ret = layout;
                found = EHS_TRUE;
                break;
            }
            lastLayout = layout;
            layout = layout->next;
        }
    }
    if (found == EHS_FALSE && create == EHS_TRUE)
    {
        if (pPlayManager->layoutInfo == NULL)
        {
            pPlayManager->layoutInfo = EhsHMem_tempAlloc(sizeof(EhsPlaymanegerLayoutRoot));
            pPlayManager->layoutInfo->rootHeight = 0;
            pPlayManager->layoutInfo->rootWidth = 0;

        }
        if (pPlayManager->layoutInfo)
        {
            EhsPlayManagerLayout * newLayout = EhsHMem_tempAlloc(sizeof (EhsPlayManagerLayout));

            Layout_InitLayout(newLayout,id); //checks for NULL

            if (lastLayout)   // this is not the first layout
            {
                lastLayout->next = newLayout;
            }
            else
            {
                pPlayManager->layoutInfo->EhsPlayManagerLayout = newLayout;
            }
            ret = newLayout;
        }
    }
    return ret;
}

/** Layout management
 * @brief returns NULL if not found and create is not set (or out of memory).
 */

static EhsPlaymanegerLayoutTextStyle* Layout_GetCreateTextStyle(EhsPlayManagerType* pPlayManager, ehs_char * id, ehs_bool create)
{
    EhsPlaymanegerLayoutTextStyle * ret = NULL;
    ehs_bool found = EHS_FALSE;
    EhsPlaymanegerLayoutTextStyle * textstyle = NULL;
    EhsPlaymanegerLayoutTextStyle * lastTextstyle = NULL;

    if (pPlayManager->layoutInfo == NULL)
    {
        Layout_SetLayoutRootInfo(pPlayManager,0, 0); // create the root info structure.
    }

    if (pPlayManager->layoutInfo)
    {
        textstyle = pPlayManager->layoutInfo->EhsPlayManagerTextStyle;

        while (textstyle)
        {
            if (EhsStricmp(textstyle->fontStyleId,  id ) == 0 )
            {
                ret = textstyle;
                found = EHS_TRUE;
                break;
            }
            lastTextstyle = textstyle;
            textstyle = textstyle->next;
        }
    }
    if (found == EHS_FALSE && create == EHS_TRUE)
    {
        if (pPlayManager->layoutInfo)
        {
            EhsPlaymanegerLayoutTextStyle * newTextstyle = EhsHMem_tempAlloc(sizeof (EhsPlaymanegerLayoutTextStyle));

            //Layout_InitLayout(newLayout,id); //checks for NULL
            newTextstyle->fontSize=0;
            newTextstyle->fontType=0;
            newTextstyle->next = NULL;
            newTextstyle->fontStyleId[0]='\0';

            if (lastTextstyle)   // this is not the first layout
            {
                lastTextstyle->next = newTextstyle;
            }
            else
            {
                pPlayManager->layoutInfo->EhsPlayManagerTextStyle = newTextstyle;
            }
            ret = newTextstyle;
        }
    }
    return ret;
}


/**
 * @brief Populate all regions with the specific text style parameters looked up in the text style database
 *
 **/

static void Layout_PopulateLayoutTextStyles(EhsPlayManagerType* pPlayManager)
{

    if (pPlayManager->layoutInfo)
    {
        EhsPlayManagerLayout * layout = pPlayManager->layoutInfo->EhsPlayManagerLayout;
        while (layout)
        {

            // set each layout section to the root dimensions (used for Aspect ratio)
            layout->height = pPlayManager->layoutInfo->rootHeight;
            layout->width = pPlayManager->layoutInfo->rootWidth;
            layout->left = 0; //todo do we want this to be offest from full screen at any time?
            layout->top = 0;

            EhsPlaymanegerLayoutRegion * region = layout->regions;
            while (region)
            {
                EhsPlaymanegerLayoutTextStyle * textstyle = pPlayManager->layoutInfo->EhsPlayManagerTextStyle;
                while (textstyle)
                {
                    if (EhsStricmp(region->fontStyleId,textstyle->fontStyleId  ) == 0)
                    {
                        region->fontSize = textstyle->fontSize;
                        region->fontType = textstyle->fontType;
                        break;
                    }
                    textstyle = textstyle->next;
                }
                region = region->next;
            }
            layout = layout->next;
        }
    }
}

/** Layout management
 * @brief removes all layout information, including the root info.
 */

static void Layout_DestroyLayoutInfo(EhsPlayManagerType* pPlayManager)
{

    if (pPlayManager->layoutInfo)
    {
        EhsPlayManagerLayout * layout = pPlayManager->layoutInfo->EhsPlayManagerLayout;
        while (layout)
        {
            EhsPlayManagerLayout * nextlayout = layout->next;
            EhsPlaymanegerLayoutRegion * region = layout->regions;
            while (region)   // delete the regions
            {
                EhsPlaymanegerLayoutRegion * nextregion = region->next;
                EhsHMem_tempFree(region);
                region = nextregion;
            }
            EhsHMem_tempFree(layout);
            layout = nextlayout;
        }

        /* Remove all TextStyles */
        EhsPlaymanegerLayoutTextStyle * textstyle = pPlayManager->layoutInfo->EhsPlayManagerTextStyle;
        while (textstyle)   // delete the regions
        {
            EhsPlaymanegerLayoutTextStyle * nextTextstyle = textstyle->next;
            EhsHMem_tempFree(textstyle);
            textstyle = nextTextstyle;
        }
        pPlayManager->layoutInfo->rootHeight=0;
        pPlayManager->layoutInfo->rootWidth=0;

        EhsHMem_tempFree(pPlayManager->layoutInfo);
        pPlayManager->layoutInfo = NULL;
    }
}


/** @brief	Parse ID for the layout ID and extract values set the columns and rows parameters
 *
 * */
static void  Layout_AddTableStyle(EhsPlayManagerType* pPlayManager,ehs_char * tableConfigId,ehs_char * rowscolumns)
{
    ehs_uint32 id = 0;
    ehs_uint32 rows = 0;
    ehs_uint32 columns = 0;
    ehs_bool gotId = EhsSscanf( tableConfigId, "tableconfig_%d",&id); //extract the layout ID from the tableconfig_$id.
    ehs_bool gotrowscols = EhsSscanf( rowscolumns, "%d,%d",&rows, &columns);

    if (gotId)
    {
        EhsPlayManagerLayout * layout = Layout_GetCreateLayoutRef(pPlayManager, id, EHS_TRUE);
        if (layout != NULL && gotrowscols)
        {
            layout->tableRows=rows;
            layout->tableColumns=columns;
        }
    }
}

static void Layout_AddRooteStyle(EhsPlayManagerType* pPlayManager,ehs_char * width,ehs_char * height)
{

    ehs_uint16 nWidth = 0;
    ehs_uint16 nHeight = 0;
    if (width) 	EhsSscanf (  width,"%hd",&nWidth);
    if (width) 	EhsSscanf (  height,"%hd",&nHeight);
    if (pPlayManager && pPlayManager->layoutInfo)
    {
        pPlayManager->layoutInfo->rootHeight=nHeight;
        pPlayManager->layoutInfo->rootWidth=nWidth;
    }
}

/* Adds Region meta data and references to test style IDs */
static void Layout_AddRegions(EhsPlayManagerType* pPlayManager,ehs_char * LayoutRegionId,ehs_char * top,ehs_char * left,ehs_char * right,ehs_char * bottom,ehs_char * textStyle)
{

    ehs_uint32 nLayoutId = 0;
    ehs_uint32 nLayoutRegionId = 0;
    ehs_uint32  ntop = 0 ;
    ehs_uint32  nleft = 0;
    ehs_uint32  nright = 0 ;
    ehs_uint32  nbottom = 0;

    ehs_bool gotId = EhsSscanf (  LayoutRegionId,"RegionId_%u_%u",&nLayoutId, &nLayoutRegionId);
    if ( gotId > 0 )
    {

        if(top) EhsSscanf (  top,"%d",&ntop);
        if(left) EhsSscanf (  left,"%d",&nleft);
        if(right) EhsSscanf (  right,"%d",&nright);
        if(bottom)  EhsSscanf (  bottom,"%d",&nbottom);

        /* if nLayout is 0 we assume this might be valid and create an entry anyway */
        EhsPlayManagerLayout* layout = Layout_GetCreateLayoutRef( pPlayManager, nLayoutId,EHS_TRUE);
        if (layout)
        {


            EhsPlaymanegerLayoutRegion * region = layout->regions;
            EhsPlaymanegerLayoutRegion * lastregion = NULL;
            EhsPlaymanegerLayoutRegion * newregion = EhsHMem_tempAlloc(sizeof(EhsPlaymanegerLayoutRegion));
            newregion->id = nLayoutRegionId;
            newregion->left = nleft;
            newregion->right = nright;
            newregion->top = ntop;
            newregion->bottom = nbottom;
            EhsStrncpy(newregion->xmlId,LayoutRegionId,EHS_STRING_LENGTH_MAX);
            //textStyle[EHS_STRING_LENGTH_MAX]='\0';
            if (textStyle) EhsStrncpy (newregion->fontStyleId, textStyle,EHS_STRING_LENGTH_MAX);
            else newregion->fontStyleId[0]='\0';

            newregion->next = NULL;
            while (region)
            {
                lastregion = region;
                region = region->next;
            }
            if (lastregion) lastregion->next = newregion;
            else layout->regions = newregion;
        }
    }

}

/**
 * createLayoutJSON
 *
 * returns a dynamic temp string that should be freed appropriately by the caller.
 * Region information will be in % values. Absolute position and size values set by the section layout info will be ignored here,
 * though this should be used to change the aspect ratio of the screen.
 * this maybe output as a special additional flag by checking AR?
 */
ehs_char * createLayoutJSON (EhsPlayManagerType* pPlayManager, ehs_uint32 id, ehs_char * aspectRatio )
{

    EhsPlayManagerLayout* layout = Layout_GetCreateLayoutRef(pPlayManager, id, EHS_FALSE);
    if (layout)
    {
        ehs_char * json = EhsHMem_tempAlloc(EHS_STRING_LENGTH_MAX); //TODO:STRINGLENGTH!!!

        if (json)
        {
            json[0]='\0';
            if(EhsStrlen(layout->json)>0)
            {
                EhsStrncpy(json,layout->json,EHS_STRING_LENGTH_MAX); //TODO:STRINGLENGTH!!!
            }
            else
            {
                EhsPlaymanegerLayoutRegion * region =   layout->regions;
                ehs_char chunk[EHS_STRING_LENGTH_MAX]; //TODO:STRINGLENGTH!!!
                /* These are the layout section global values
                layout->left; //ignored
                layout->width;//ignored
                layout->top;//ignored
                layout->height;//ignored
                layout->tableRows;
                layout->tableColumns;
                */

                EhsStrcat(json,"{" );
                /* for each region we'll add a JASON array input. */
                while (region)
                {

                    EhsSprintf(chunk,"\"%s\":{\"bottom\":%d,\"top\":%d,\"left\":%d,\"right\":%d,\"fontType\":\"%s\",\"fontSize\":%d,\"textRows\":%d,\"textColumns\":%d},",
                               region->xmlId,
                               region->bottom,
                               region->top,
                               region->left,
                               region->right,
                               region->fontStyleId,
                               region->fontSize,
                               layout->tableRows,
                               layout->tableColumns);
                    if ( (EhsStrlen(json) + EhsStrlen(chunk)) < EHS_STRING_LENGTH_MAX ) //TODO:STRINGLENGTH!
                    {
                        EhsStrcat(json, chunk);
                        EhsSprintf(chunk,"\"%d\":{\"bottom\":%d,\"top\":%d,\"left\":%d,\"right\":%d,\"fontType\":\"%s\",\"fontSize\":%d,\"textRows\":%d,\"textColumns\":%d},",
                                   region->id,
                                   region->bottom,
                                   region->top,
                                   region->left,
                                   region->right,
                                   region->fontStyleId,
                                   region->fontSize,
                                   layout->tableRows,
                                   layout->tableColumns);
                        if ( (EhsStrlen(json) + EhsStrlen(chunk)) < EHS_STRING_LENGTH_MAX ) //TODO:STRINGLENGTH!
                        {
                            EhsStrcat(json, chunk);
                        }
                        else
                        {
                            EHSH_LOG_ERROR("Layout JSON string longer than max string length. Ignoring subsequent regions");
                            break;
                        }
                    }
                    else
                    {
                        EHSH_LOG_ERROR("Layout JSON string longer than max string length. Ignoring subsequent regions");
                        break;
                    }
                    region = region->next;
                }

                if (EhsStrlen(json) > 1)
                {
                    json[EhsStrlen(json)-1]='\0'; // remove last comma
                }
                EhsStrcat(json,"}" );
            }


            /* And set the Apsect Ration data too */
            if (aspectRatio)
            {
                EhsSprintf(aspectRatio, "{\"width\":%hu,\"height\":%hu}", layout->width,layout->height);
            }
            return json;

        }
        else
        {
            return NULL;
        }
    }
    else
    {
        return NULL;
    }
}


/**
 * Update layout databses from header
 *
 * Assumes all meta information is table info.
 */

static void readMetaTag(EhsPlayManagerType* pPlayManager, xmlTextReaderPtr reader)
{


    ehs_char* tableConfigId = xmlTextReaderGetAttribute(reader, "name");
    ehs_char* rowscolumns = xmlTextReaderGetAttribute(reader, "content");

    if (tableConfigId != NULL)
    {
        if(EhsStrcmp("fileCount", tableConfigId) == 0){
            // check if total file count has been set in the SMIL head
            if(rowscolumns != NULL){
                pPlayManager->playlistFileCount = atoi(rowscolumns);
            }
        }else{
            /* if it starts with http: then it is a full URL to be left unmangled */
            Layout_AddTableStyle(pPlayManager,tableConfigId,rowscolumns);
        }
        xmlFree(tableConfigId);

        /* This would be used if we allowed http to be downloaded - we don't this means it is streamed */
    }
    if (rowscolumns) xmlFree(rowscolumns);

}

/**
 * Create a text Style Entry in the
 */

void Layout_AddTextStyle(EhsPlayManagerType* pPlayManager, ehs_char * textStyleId, ehs_char * textStyle,ehs_char *  textSize)
{

    FontType ntextStyle = sansarif;
    FontSize ntextSize = _medium;
    EhsPlaymanegerLayoutTextStyle * textstyle = Layout_GetCreateTextStyle(pPlayManager, textStyleId, EHS_TRUE);
    if (textstyle)
    {
        if (EhsStricmp("Arial",textStyle) == 0) ntextStyle = sansarif;
        if (EhsStricmp("Sansarif",textStyle) == 0) ntextStyle = sansarif;
        if (EhsStricmp("Typewriterf",textStyle) == 0) ntextStyle = typewriter;
        if (EhsStricmp("Sarif",textStyle) == 0) ntextStyle = sarif;
        if (EhsStricmp("TimesRoman",textStyle) == 0) ntextStyle = sarif;
        textstyle->fontType = ntextStyle;

        if (EhsStricmp("Small",textSize) == 0) ntextSize = _small;
        if (EhsStricmp("Medium",textSize) == 0) ntextSize = _medium;
        if (EhsStricmp("Large",textSize) == 0) ntextSize = _large;
        textstyle->fontSize = ntextSize;

        EhsStrcpy(textstyle->fontStyleId,textStyleId);
    }
}

/* Creates a textstyle entry */

static void readtextStylingTag(EhsPlayManagerType* pPlayManager, xmlTextReaderPtr reader)
{

    ehs_char* nextTag;
    int nodeType;
    int countOut=1000;
    nextTag = getNextTag(reader,&nodeType);
    while (nextTag != NULL && countOut > 0 )
    {

        if ((strcmp("textStyle", nextTag) == 0) && nodeType == 1 )
        {
            ehs_char* textStyleId = xmlTextReaderGetAttribute(reader, "xml:id");
            ehs_char* textFontFamily = xmlTextReaderGetAttribute(reader, "textFontFamily");
            ehs_char* textFontSize = xmlTextReaderGetAttribute(reader, "textFontSize");
            if (textStyleId)
            {
                Layout_AddTextStyle(pPlayManager, textStyleId,textFontFamily,textFontSize);
                xmlFree(textStyleId);
            }
            if (textFontFamily) xmlFree(textFontFamily);
            if (textFontSize) xmlFree(textFontSize);

        }
        else if ((strcmp("textStyling", nextTag) == 0) && nodeType == 15 )   // check for the closing tag for the Stlyling section
        {
            break;
        }
        else
        {
            EHSH_LOG_WARNING("Found unexpected tags in <textStyling> %s", nextTag);
            countOut--;
        }

        if ( xmlTextReaderRead(reader) == 1)
            nextTag = getNextTag(reader, &nodeType);
        else nextTag = NULL;
    }

}
/** @brief Reads all the layout tag entries in the SMIL header
 *
 */
static void readLayoutTag(EhsPlayManagerType* pPlayManager, xmlTextReaderPtr reader)
{

    ehs_char* nextTag;
    int nodeType;
    int countOut=1000;
    nextTag = getNextTag(reader,&nodeType);

    while (nextTag != NULL && countOut > 0)
    {

        if ((strcmp("root-layout", nextTag) == 0) && nodeType == 1  )
        {
            //ehs_char* layoutId = xmlTextReaderGetAttribute(reader, "xml:id");
            ehs_char* width = xmlTextReaderGetAttribute(reader, "width");
            ehs_char* height = xmlTextReaderGetAttribute(reader, "height");
            Layout_AddRooteStyle(pPlayManager, width,height);
            //	xmlFree(layoutId);
            if (width) xmlFree(width);
            if (height) xmlFree(height);

        }
        else if ((strcmp("region", nextTag) == 0) && nodeType == 1  )
        {
            ehs_char* LayoutRegionId = (ehs_char*)xmlTextReaderGetAttribute(reader, "xml:id");
            ehs_char* top = (ehs_char*)xmlTextReaderGetAttribute(reader, "top");
            ehs_char* left = (ehs_char*)xmlTextReaderGetAttribute(reader, "left");
            ehs_char* right = (ehs_char*)xmlTextReaderGetAttribute(reader, "right");
            ehs_char* bottom = (ehs_char*)xmlTextReaderGetAttribute(reader, "bottom");
            ehs_char* textStyle = (ehs_char*)xmlTextReaderGetAttribute(reader, "textStyle");
            if (LayoutRegionId)
            {
                Layout_AddRegions(pPlayManager,LayoutRegionId,top,left,right,bottom,textStyle);
                xmlFree(LayoutRegionId);
            }
            if (top) xmlFree(top);
            if (left) xmlFree(left);
            if (right) xmlFree(right);
            if (bottom) xmlFree(bottom);
            if (textStyle) xmlFree(textStyle);

        }
        else if(strcmp("inxjson",nextTag)==0 && nodeType==1)
        {
            ehs_char* LayoutRegionId = (ehs_char*)xmlTextReaderGetAttribute(reader, "xml:id");
            if(LayoutRegionId)
            {
                ehs_char* inxjsonString = (ehs_char*) xmlTextReaderReadString(reader);
                if(inxjsonString)
                {
                    ehs_uint32 nLayoutId = 0;
                    ehs_bool gotId = EhsSscanf (LayoutRegionId,"SectionId_%u",&nLayoutId);
                    if ( gotId > 0 )
                    {
                        EhsPlayManagerLayout* pLayout=Layout_GetCreateLayoutRef(pPlayManager,nLayoutId,EHS_TRUE);
                        if(pLayout)
                        {
                            EhsStrncpy(pLayout->json,inxjsonString,EHS_STRING_LENGTH_MAX);
                        }
                        else
                        {
                        }
                    }
                    else
                    {
                    }
                    xmlFree(inxjsonString);
                }
                xmlFree(LayoutRegionId);
            }
        }
        else if ((strcmp("layout", nextTag) == 0) && nodeType == 15 )   // check for the closing tag
        {
            break;
        }
        else
        {
            EHSH_LOG_WARNING("Found unexpected tags in <layout> %s", nextTag);
            countOut--;
        }

        if (xmlTextReaderRead(reader) == 1)
            nextTag = getNextTag(reader, &nodeType);
        else nextTag = NULL;
    }

}

void doHeaderProcessing(EhsPlayManagerType* pPlayManager)
{
    Layout_PopulateLayoutTextStyles( pPlayManager);
}

/* Extract header info */

static void headTag(xmlTextReaderPtr reader, EhsPlayManagerType* pPlayManager)
{
    ehs_char* nextTag;
    int nodeType;
    int countOut = 1000;
    nextTag = getNextTag(reader,&nodeType);

    while (nextTag != NULL && countOut > 0 )
    {
        if ((strcmp("meta", nextTag) == 0) && (nodeType == 1))
        {

            readMetaTag(pPlayManager, reader);
        }
        else if ((strcmp("textStyling", nextTag) == 0) && (nodeType == 1))
        {

            readtextStylingTag(pPlayManager, reader);
        }
        else if ((strcmp("layout", nextTag) == 0) && (nodeType == 1))
        {
            readLayoutTag(pPlayManager, reader);
        }
        /* else the closing tag */
        else if ((strcmp("head", nextTag) == 0) && (nodeType == 15))
        {

            doHeaderProcessing(pPlayManager);  // tie up any data we have read in the layout info and/or otherheader sections.

       if ( xmlTextReaderRead(reader) == 1);
            closingHeadTag(reader, pPlayManager);
            break;
        }
        else
        {
            EHSH_LOG_ERROR("expected closing head. found \"%s\"\n", nextTag);
            countOut--;
        }

        if (xmlTextReaderRead(reader) == 1)
        {

            nextTag = getNextTag(reader, &nodeType);
        }
        else
        {
            nextTag = NULL;
        }

    }
}

/**
 * Start reading SMIL and look for head tag
 */

static void smilTag(xmlTextReaderPtr reader, EhsPlayManagerType* pPlayManager)
{
    ehs_char* nextTag;
    int nodeType;
    nextTag = getNextTag(reader,&nodeType);
    if (xmlTextReaderRead(reader) == 1)
    {
        if (nextTag != NULL)
        {
            if (strcmp("head", nextTag) == 0)
            {
                headTag(reader, pPlayManager);
            }
            else
            {
                EHSH_LOG_ERROR("expected head found %s", nextTag);
            }
        }
        else
        {
            EHSH_LOG_ERROR("smilTag: failed to get nextTag");
        }
    }
    else EHSH_LOG_ERROR("smilTag: failed to read SMIL tags");
}


/** @brief called for identifying all the source elements that need to be downloaded
 *
 * Parses the SMIL XML body for media content and outputs URLS to download it (constructed from parameters or dynamic inputs).
 * Is called initially and the subsequently from the nextURL to get
 * the next in a sequence is read-off and events.
 * The next URL port should be asserted after the got function has completed to make sure the src list has been updated properly.
 *
 * Trust: - all is mutexed before we get here!
 * Promise: - will exit when NewSODL flag asserted
 *
 *@todo this function assumes all timed events are arranged in order in the file (e.g. earliest events come first in the file - this is not generally SMIL's case
 *@todo Resource management of this code is not very clear - what happens when the events list is cleared?
 */

EHS_LOCAL ehs_bool readSrcs(EhsPlayManagerType* pPlayManager,
                            EhsFunctionInstanceDataType* pFIdata, ehs_uint32 getPortNumber,
                            ehs_uint32 urlPortNumber, ehs_uint32 pathPortNumber,
                            ehs_uint32 nomorePortNumber)
{
    ehs_uint32 fired = 0;
    ehs_char* nextTag;
    ehs_char* value;
    ehs_char preparedOutput[EHS_STRING_LENGTH_MAX]; //TODO:STRINGLENGTH!!!!
    ehs_char* slashPos = NULL;
    int nodeType;
    int readRet;
    readRet = xmlTextReaderRead(pPlayManager->srcFileReader);
    nextTag = getNextTag(pPlayManager->srcFileReader, &nodeType); // find the next tag in the SMIL file
    pPlayManager->triedAllDownloads = EHS_FALSE;
    while ((nextTag != NULL) && (fired == 0)) //stop if we can't read anymore or we have fired the GET port
    {
        /* Try to find a src attribute */


        value = (ehs_char*) xmlTextReaderGetAttribute(pPlayManager->srcFileReader, "src"); //
        if (value != NULL)
        {

            int pathLen=EhsStrlen(value);
            //printf ("lastSrcPath = value {%s=%s}\n",pPlayManager->lastSrcPath,value);
#if EHS_NUM_LASTSRC_CACHESIZE < 5
#error "EHS_NUM_LASTSRC_CACHESIZE must be 3!"
#endif
            if (EhsStrcmp(pPlayManager->lastSrcPath[0],value) == 0  ||
                    EhsStrcmp(pPlayManager->lastSrcPath[1],value) == 0  ||
                    EhsStrcmp(pPlayManager->lastSrcPath[2],value) == 0  ||
                    EhsStrcmp(pPlayManager->lastSrcPath[3],value) == 0  ||
                    EhsStrcmp(pPlayManager->lastSrcPath[4],value) == 0  || // todp this is not very nice as we have NUM_LASTSRC_CACHESIZE as the upper limit which may be < 3
                    (pathLen>4 && EhsStrcmp(".000",&value[pathLen-4]) == 0 ))   // if dupe or a .000 file skip to next
            {
                xmlFree(value);
                xmlTextReaderRead(pPlayManager->srcFileReader); //
                nextTag = getNextTag(pPlayManager->srcFileReader, &nodeType);
                fired = 0; //need this to carry on reading to the end
            }
            else
            {
                /* if it starts with http: then it is a full URL to be left unmangled */
                if (pathLen>4 && (EhsStrcmp(".000",&value[pathLen-4]) != 0) )
                {
                    EhsStrcpy(pPlayManager->lastSrcPath[pPlayManager->lstSrcIndex],value);
                    pPlayManager->lstSrcIndex ++;
                    if (pPlayManager->lstSrcIndex >= EHS_NUM_LASTSRC_CACHESIZE) pPlayManager->lstSrcIndex = 0; // add the last 3 unique filenames to the cache register
                }
                if (EhsStrncmp("http:", value, 5) == 0 || EhsStrncmp("https:", value, 6) == 0 || EhsStrncmp("ftp:", value, 4) == 0)
                {
                    xmlFree(value);
                    xmlTextReaderRead(pPlayManager->srcFileReader); //
                    nextTag = getNextTag(pPlayManager->srcFileReader, &nodeType);
                    fired = 0; //need this to carry on reading to the end

                }
                else
                {
                    /*
                     * Assume path is a local path so we need to prepend the value of the download server */
                    EhsStrcpy(preparedOutput, pPlayManager->downloadURL);
                    EhsStrcat(preparedOutput, value);
                    EhsTPMutex_lock(EhsTPMutex_fbIO);
                    EhsStrcpy(EHS_FB_OUT_S(urlPortNumber), preparedOutput);
                    /* and set the play path */
                    EhsStrcpy(preparedOutput, pPlayManager->downloadPath);
                    EhsStrcat(preparedOutput, value);
                    EhsStrcpy(EHS_FB_OUT_S(pathPortNumber), preparedOutput);
                    xmlFree(value);

                    //xmlTextReaderRead(pPlayManager->srcFileReader); //
                    //nextTag = getNextTag(pPlayManager->srcFileReader, &nodeType);
                    EHS_FB_FINISH(getPortNumber); /* Not having this makes the whole system lock up?*/
                    EhsTPMutex_unlock(EhsTPMutex_fbIO);
                    fired = 1;
                }
            }
            /* This would be used if we allowed http to be downloaded - we don't this means it is streamed */
        }
        else     /* Reader next when no more attributes available */
        {

            if (xmlTextReaderRead(pPlayManager->srcFileReader) == 1)
                nextTag = getNextTag(pPlayManager->srcFileReader, &nodeType);
            else nextTag = NULL;

        }
        // - we thread so don't need to pause - EhsSleep(EHS_TIME_us(5000));
        if (*bNewSodlFlagRef)
            break; /* Get out nicel if we are asked */
    } // end of while
    /* if nextTag is NULL then assert the no more event and clear the reader because we don't need it anymore */
    if (nextTag == NULL)
    {
        xmlTextReaderClose(pPlayManager->srcFileReader);
        xmlFreeTextReader(pPlayManager->srcFileReader);
        pPlayManager->srcFileReader = NULL;
        pPlayManager->triedAllDownloads = EHS_TRUE;
        EHSH_LOG_INFO("Got all listed media");
        EHS_FB_FINISH(nomorePortNumber);
        return EHS_FALSE;
    }
    else
    {
        return EHS_TRUE;
    }
}


/*
 * Read a new playlist file. - done as a thread - this was causing EHS blocks for first pass parsing with large playlists.
 * First read header and check we have a SMIL file.
 * Create (and destroy previous) xmls reader object and then call readSrcs
 * to assign pfi data to event call- backs.
 *
 * This is mutexed by EhsTPMutex_playManager
 */
EHS_FB_THREAD_FUNCTION(PlayManagerNextPlayListThread)
{

    xmlTextReaderPtr fileReader = NULL;
    ehs_char* nextTag;
    Ehs_FB_ThreadStarted();
    ehs_bool clear = EHS_FALSE;
    int nodeType;
    EhsTPMutex_lock(EhsTPMutex_playManager);
    EhsPlayManagerType* pPlayManager = (EhsPlayManagerType*) EHS_FB_RUN_CONTEXT;
    ehs_char filename[EHS_STRING_LENGTH_MAX]; //Initialise some memory for reading in the filename
    resetPlaylistFlags(pPlayManager); // particularl need to sait waiting on play to 0 in case we are locked up.
    pPlayManager->NewSmil=EHS_TRUE;
    pPlayManager->NewPlaylistStartGracePeriod =  pPlayManager->initial_grace_seconds*1000000/EHS_SMILPARSER_TICK_TIME_US;//(EHS_SMILPARSER_DEFAULT_START_GRACE/EHS_SMILPARSER_TICK_TIME_US); // restart the count down when we get a new file

    /* Cancel any current downloading and wait for it to cancel */

    //if (pPlayManager->srcFileReader != NULL) {
    //	X//todo if the downloading flag is set we need to wait (withtimeout) that this is complete and tell the downloader not to try any more.
    //}

    /* Clear the playlist @todo this will be a configuration option = alternative is to lean all, or leave untill first event in new playlist is asserted */
    switch (pPlayManager->NewSmilBehaviour)
    {
    case CLEAR_ALL:
        clearEventList(pPlayManager, EHS_FALSE); // clear all
        break;
    case KEEP_EARLIERTHANNEW:
        markEventList(pPlayManager);// Mark old events as old - if we have any
        break;
    case KEEP_ALL:
        break;
    default:
        clearEventList(pPlayManager, EHS_FALSE); // clear all
    }
    /* Read in the SMIL playlist filename */
    EhsTPMutex_lock(EhsTPMutex_fbIO);
    EhsTF_tryCanonicPath(filename, EHS_RUNTIME_USERDATA_DIR,EHS_FB_IN_S(0), EHS_TRUE);
    EhsTPMutex_unlock(EhsTPMutex_fbIO);
    /* Create a player reader for it, being careful to dispose of our existing one */
    if (pPlayManager->fileReader != NULL)
    {
        xmlTextReaderClose(pPlayManager->fileReader);
        xmlFreeTextReader(pPlayManager->fileReader);
        pPlayManager->fileReader = NULL;
    }
    /* Remove the previous layout info */
    Layout_DestroyLayoutInfo(pPlayManager);

    /* We create a seperate source reader, so remove any previous ones */
    if (pPlayManager->srcFileReader != NULL)
    {
        xmlTextReaderClose(pPlayManager->srcFileReader);
        xmlFreeTextReader(pPlayManager->srcFileReader);
        pPlayManager->srcFileReader = NULL;
    }

    if (EhsStrcmp(filename,"__clear.smil") != 0)
    {
#ifdef EHS_MINGW
        /* A workaround for Windows used as a fix for renaming smil file
         * that is in use. */
        ehs_char win_filename[EHS_STRING_LENGTH_MAX]; //TODO:STRINGLENGTH!
        EhsStrcpy(win_filename, filename);
        EhsStrcat(win_filename, ".win");
        if(EhsTF_exists(win_filename) == 1){
            if(!EhsHRemove(win_filename)){
                EHSH_LOG_ERROR("Win playlist file - failed to remove a previous copy!");
            }
        }
        if(!EhsHCopy(filename, win_filename)){
            EHSH_LOG_ERROR("Win playlist file - failed to make a copy!");
        }
        if(EhsTF_exists(win_filename) == 1){
            EhsStrcpy(filename, win_filename);
        }
        EHSH_LOG_INFO("Win playlist file = %s", filename);
#endif
        fileReader = xmlReaderForFile(filename, NULL, 0);

        if (fileReader != NULL)   /* We stop the current download if ther is one - the rest of previous SMIL won't be downloaded */
        {
            if (xmlTextReaderRead(fileReader) == 1 )   /* first call after reader is created */
            {
                nextTag = getNextTag(fileReader, &nodeType); /* read the first tag */
                xmlTextReaderRead(fileReader);
            }
            else nextTag = NULL;
            if (nextTag != NULL)
            {
                if (strcmp("smil", nextTag) == 0)
                {
                    pPlayManager->playlistFileCount = 0; // reset playlist file count to be read from the header
                    smilTag(fileReader, pPlayManager); /* This parses the entire SMIL file and creates sub-systems (creating it's own parent time). */
                    EhsTPMutex_lock(EhsTPMutex_fbIO);
                    EHS_FB_OUT_I(EHS_FB_PLAYMANAGER_NEXT_PLAYLIST_FILE_COUNT) = pPlayManager->playlistFileCount;
                    EhsTPMutex_unlock(EhsTPMutex_fbIO);
                    printf("playlist=%s, fileCount=%d\n", filename, pPlayManager->playlistFileCount); // apply playlist file count to the port
                    /* The exception is for sequences that are parsed as they become live */
                    /* update the instance fileReader so that we can read the XML later on when an event is asserted */
                    /* After we have done parsing for the events then set the parser for the src attributes going */
                    pPlayManager->fileReader = fileReader; /* we have one file reader */

                    pPlayManager->srcFileReader = xmlReaderForFile(filename, NULL,0);
                    if (!pPlayManager->srcFileReader)
                    {
                        EHSH_LOG_ERROR("Could Not open SMIL file %s for reading",filename);
                    }
                    /* expose the first node of the SMIL playlist for getting source info */
                    if (xmlTextReaderRead(pPlayManager->srcFileReader) == 1 && pPlayManager->srcFileReader != NULL )   /* now read the first entry */
                    {
                        /* @todo consider removing this and triggering first readSRC by the get next URL only - this means all will start OK without bothering with the downloads */
                        //EHSH_LOG_ERROR("---->Reading sources....\n");
                        readSrcs(pPlayManager, pFIdata, EHS_FB_PLAYMANAGER_NEXT_PLAYLIST_GET, EHS_FB_PLAYMANAGER_NEXT_PLAYLIST_URL,EHS_FB_PLAYMANAGER_NEXT_PLAYLIST_LOCAL_FILE,EHS_FB_PLAYMANAGER_NEXT_PLAYLIST_NOMORE); // find the first event data
                    }
                    else
                    {
                        EHSH_LOG_ERROR( "PlayManager_Next_Playlist: Failed to open file");
                    }
                }
                else
                {
                    // free up resources and release file
                    xmlTextReaderClose(fileReader);
                    xmlFreeTextReader(fileReader);
                    EHSH_LOG_ERROR( "PlayManager_Next_Playlist: expected smil, found %s",
                                    nextTag);
                }
            }
            else
            {
                // free up resources and release file
                xmlTextReaderClose(fileReader);
                xmlFreeTextReader(fileReader);
                EHSH_LOG_ERROR( "PlayManager_Next_Playlist: failed to read first tag");
            }
        }
        else
        {
            EHSH_LOG_WARNING("PlayManager_Next_Playlist: failed to open SMIL file");
        }
    }
    else
    {
        EHSH_LOG_INFO("Cleared playlist ");
    }

    /*
     We keep the file reader for the readSrc function to iterate through the SMIL to get downloads
     */

    EhsTPMutex_unlock(EhsTPMutex_playManager);
    EHS_FB_FINISH(EHS_FB_PLAYMANAGER_NEXT_PLAYLIST_FINISH);
    EHSH_LOG_ERROR("Exiting PlayManagerNextPlayListThread");
    Ehs_FB_ThreadComplete();
    EhsHThread_exit();
}

/* Defer this event to thread processing */
EHS_FB_RUN_FUNCTION(PlayManager_Next_Playlist)
{

    /* Only allow one operational instance - do this in the thread .... */
    //EHSH_LOG_ERROR("----> PlayManager_Next_Playlist()");
    EHS_FB_START_THREAD(PlayManagerNextPlayListThread, -90);
}

/*
 * THis function finds the local path of the file from the entry in the Playlist.
 * @todo this function should contain a mode switch (flag set as a parameter) so that the the global local file name
 */

EHS_LOCAL ehs_char* lookupMedia(ehs_char* value,EhsPlayManagerType* pPlayManager)
{
    ehs_char prepared[EHS_STRING_LENGTH_MAX]; // this is used to build a temporary URL pattern to match against those retrieved. //TODO:STRINGLENGTH!
    int i = 0;
    EhsPlayManagerMediaLocation* pML = NULL;
    if (EhsStrncmp("http:", value, 5) == 0 || EhsStrncmp("ftp:", value, 4) == 0 || EhsStrncmp("https:", value, 6) == 0 )
    {
        /* full URL so we do not need to do any manipulation */
        return value;
    }
    else
    {
        /* assume path is a local path so we need to prepend the value of the download server */
        EhsStrcpy(prepared, pPlayManager->downloadURL);
        EhsStrcat(prepared, value);
        value = prepared;
    }
    /* loop through media list, when you find a match on the URL then return the path, otherwise return NULL */
    pML = pPlayManager->pMediaList;
    while (pML != NULL)
    {
        i++;
        if (EhsStrcmp(value, pML->url) == 0)
        {
            return pML->localFile;
        }
        pML = pML->pNext;
    }
    EHSH_LOG_INFO("lookupMedia value=%s",value);
    return NULL;
}

/* This is for elements that reference external resources and need a download..
 * @todo this doesn't currently process prefetches - a swithc to allow amd switch off the auto pre-downloader should be added one day
 *  */
ehs_bool isaSmilRefElement(ehs_char* nextTag)
{
    if ((strcmp("ref", nextTag) == 0) || (strcmp("audio", nextTag) == 0)
            || (strcmp("media", nextTag) == 0) || (strcmp("video", nextTag)
                    == 0) || (strcmp("img", nextTag) == 0) || (strcmp("text", nextTag)
                            == 0) || (strcmp("animation", nextTag) == 0) || (strcmp(
                                        "textstream", nextTag) == 0))
    {
        return EHS_TRUE;
    }
    else
    {
        return EHS_FALSE;
    }
}

/* This ifor elements that cause an output but do not need a download.. */
ehs_bool isaSmilOtherImmediateElement(ehs_char* nextTag)
{
    if ((strcmp("smilText", nextTag) == 0))  //list of items that are not url references go here
    {
        return EHS_TRUE;
    }
    else
    {
        return EHS_FALSE;
    }
}




/**
 * Outputs the layout info in JSON form.
 * This will usually be called at the beginning of a section.
 *  */
ehs_bool assertLayoutOutputs(EhsFunctionInstanceDataType* pFIdata, EhsPlayManagerEvent* pEvent)
{


    EhsPlayManagerType* pPlayManager = (EhsPlayManagerType*) pFIdata->pObjData;

    if (pEvent->layoutSection)   // check this is dynamic
    {

        ehs_sint32 id = -1;
        EhsSscanf(pEvent->layoutSection, "SectionId_%d",&id);
        if (id > 0)
        {
            ehs_char sectionAspectRatioJSON[EHS_STRING_LENGTH_MAX] =""; //TODO:STRINGLENGTH! create layotu JSON to know max string length
            ehs_char * layoutJSON = createLayoutJSON( pPlayManager, (ehs_uint32)id,sectionAspectRatioJSON);

            EhsTPMutex_lock(EhsTPMutex_fbIO);
            if (layoutJSON)
            {
                /* and output the Aspect ratio values */
                EhsStrcpy(EHS_FB_OUT_S(EHS_FB_PLAYMANAGER_TICK_LAYOUT), layoutJSON);
                EhsStrcpy(EHS_FB_OUT_S(EHS_FB_PLAYMANAGER_TICK_ASPECTRATIO),sectionAspectRatioJSON );
                /* and output the new region layout database for this section */

                EhsHMem_tempFree(layoutJSON);
            }
            else
            {
                EhsStrcpy(EHS_FB_OUT_S(EHS_FB_PLAYMANAGER_TICK_LAYOUT), "");
                EHSH_LOG_WARNING("Could not create layout for %s",pEvent->layoutSection);
            }

            EHS_FB_FINISH(EHS_FB_PLAYMANAGER_TICK_NEWLAYOUT);
            EhsTPMutex_unlock(EhsTPMutex_fbIO);
        }
        else
        {
            EHSH_LOG_WARNING("Could not parse SectionId_ for %s",pEvent->layoutSection);
        }
    }
    return EHS_TRUE;
}


/*
 * Outputs the event info on the ports
 * If path is null (the local media path) then the function will attempt to output the media source info instead
 * This can be used also to assert end of media events, by passing an empty string as path to tell the media player to play nothing
 *
 * pEvent is the Event structure from which the events meta data will be draw
 * path is the local file path - that will typucaly be looked up in the internal database.
 * bEndEvent is set to true if you want this to fire the end port rather than the Play Now port
 *
 * return true if successful
 *
 * This does not change the pEvent struct in any way, remove it from the active list or destroy it.
 */
ehs_bool assertObjectEventOutputs(EhsFunctionInstanceDataType* pFIdata, EhsPlayManagerEvent* pEvent, ehs_char* path,ehs_bool bEndEvent)
{
    EhsPlayManagerType* pPlayManager = (EhsPlayManagerType*) pFIdata->pObjData;
    ehs_bool doImmediate = EHS_FALSE;
#define EHS_DEBUG_SMILPARSER_OUTPUTS
#ifdef EHS_DEBUG_SMILPARSER_OUTPUTS
    EHSH_LOG_INFO("-------------------------------------------------------->\n");
    PrintEvent(pEvent);
    EHSH_LOG_INFO("-------------------------------------------------------->\n");
#endif
    if (bEndEvent && pEvent->bEndFiredWaitingForAck > 0)
    {
        pEvent->bEndFiredWaitingForAck--;
        if (pEvent->bEndFiredWaitingForAck == 0)
        {

            EHSH_LOG_ERROR("EndEvent timed out waiting for Ack (%s)!!",pEvent->pMediaSource);
            HandleEndEvent(pPlayManager,&pEvent,EHS_TRUE,0); // update start and stop times
        }
    }
    else
    {
        EHSH_LOG_INFO("PLAYING ID (type%d)=%d\n",bEndEvent, pEvent->SendersObjectUniqueID);


        if (pEvent->class)
        {
            EhsStrcpy(EHS_FB_OUT_S(EHS_FB_PLAYMANAGER_TICK_CLASS), pEvent->class);
        }
        else EhsStrcpy(EHS_FB_OUT_S(EHS_FB_PLAYMANAGER_TICK_CLASS), "");

        EHS_FB_OUT_I(EHS_FB_PLAYMANAGER_TICK_FADE) = pEvent->fade;

        if (pEvent->soundLevel)
        {
            EHS_FB_OUT_I(EHS_FB_PLAYMANAGER_TICK_SOUNDLEVEL) = atoi(pEvent->soundLevel);
        }
        else EHS_FB_OUT_I(EHS_FB_PLAYMANAGER_TICK_SOUNDLEVEL) = 100;

        if (EhsStrlen(pEvent->smilText)>0) doImmediate = EHS_TRUE;
        if (!bEndEvent)
        {
            if (   (pEvent->pParent && pEvent->pParent->eventListType == EHS_PLAYMANAGER_EVENTLIST_SEQREF )
                    || (pEvent->pParent && pEvent->pParent->pParent &&pEvent->pParent->pParent->eventListType == EHS_PLAYMANAGER_EVENTLIST_SEQREF))   // this because dynamic sequencelistsaresometimes continaed in the original sequence list..
            {

                doImmediate = EHS_TRUE; // we don't wantto emitthis start event as a remote content type (e.g. a slowstarting stream...
            }
        }
        EhsStrncpy(EHS_FB_OUT_S(EHS_FB_PLAYMANAGER_TICK_TEXT), pEvent->smilText,EHS_STRING_LENGTH_MAX);

        if (pEvent->tagType)
        {
            EhsStrncpy(EHS_FB_OUT_S(EHS_FB_PLAYMANAGER_TICK_TYPE), pEvent->tagType,EHS_STRING_LENGTH_MAX);
        }
        else EhsStrcpy(EHS_FB_OUT_S(EHS_FB_PLAYMANAGER_TICK_TYPE), "");
        /* @todo
        if (pEvent->srcType) {
        	EhsStrcpy(EHS_FB_OUT_S(EHS_FB_PLAYMANAGER_TICK_TYPE), pEvent->srcType);
        }
        else EhsStrcpy(EHS_FB_OUT_S(EHS_FB_PLAYMANAGER_TICK_SRCTYPE), "");
         */

        if (pEvent->region)
        {

            EhsStrcpy(EHS_FB_OUT_S(EHS_FB_PLAYMANAGER_TICK_REGION), pEvent->region);
        }
        else EhsStrcpy(EHS_FB_OUT_S(EHS_FB_PLAYMANAGER_TICK_REGION), "");

        if (path != NULL)
        {
            EhsTPMutex_lock(EhsTPMutex_fbIO);
            EhsStrcpy(EHS_FB_OUT_S(EHS_FB_PLAYMANAGER_TICK_MEDIAFILE),path);
            EhsTPMutex_unlock(EhsTPMutex_fbIO);
        }
        else     // @todo: this should be a config parameter - we try to play the file we would expect in any case - it may already be there - we just on't know yet.
        {
            if (pEvent->pMediaSource)   // We'll build the expected path anyway - in case the player wants to have a go!
            {
                EhsTPMutex_lock(EhsTPMutex_fbIO);
                if (EhsStrncmp(pEvent->pMediaSource,"http:",EhsStrlen("http:"))==0 || EhsStrncmp(pEvent->pMediaSource,"ftp:",EhsStrlen("ftp:"))==0 || EhsStrncmp(pEvent->pMediaSource,"https:",EhsStrlen("https:"))==0)
                {

                    EhsStrcpy(EHS_FB_OUT_S(EHS_FB_PLAYMANAGER_TICK_MEDIAFILE),pEvent->pMediaSource);
                }
                else
                {
                    EhsStrcpy(EHS_FB_OUT_S(EHS_FB_PLAYMANAGER_TICK_MEDIAFILE),pPlayManager->downloadPath);
                    EhsStrcat(EHS_FB_OUT_S(EHS_FB_PLAYMANAGER_TICK_MEDIAFILE),pEvent->pMediaSource);

                }
                EhsTPMutex_unlock(EhsTPMutex_fbIO);
            }
            else
            {
                EhsTPMutex_lock(EhsTPMutex_fbIO);
                EhsStrcpy(EHS_FB_OUT_S(EHS_FB_PLAYMANAGER_TICK_MEDIAFILE), "");
                EhsTPMutex_unlock(EhsTPMutex_fbIO);
            }
        }

        EhsTPMutex_lock(EhsTPMutex_fbIO);
        EHS_FB_OUT_I(EHS_FB_PLAYMANAGER_TICK_ID) = pEvent->SendersObjectUniqueID; // output the unique ID if we are an object.
        if (bEndEvent)
        {
            pEvent->bEndFiredWaitingForAck=EHS_SMILPARSER_TIMEOUT_ACK_FOR_PARENT_LIST_US/EHS_SMILPARSER_TICK_TIME_US; // 30s in uS We wait thn timeout so that notification for parent lists work ...
            EHS_FB_FINISH(EHS_FB_PLAYMANAGER_TICK_STOPPLAY);
            /* We don't remove/update the event until we get an ack or timeout. */
        }
        else
        {
            if (path || doImmediate)
            {
                EHS_FB_FINISH(EHS_FB_PLAYMANAGER_TICK_PLAYNOW); /* We have a local path */
            }
            else EHS_FB_FINISH(EHS_FB_PLAYMANAGER_TICK_PLAYREMOTE);  // We could add a futher delay here or test to see if the file exits in any case.
        }
        EhsTPMutex_unlock(EhsTPMutex_fbIO);
    }
    return EHS_TRUE;
}

/* We have got a done ID from the player so delete the pending list events */
EHS_LOCAL void DeleteListEventsWithListID(EhsPlayManagerType* pPlayManager,ehs_uint32 ListID)
{

    EhsPlayManagerEvent* pCurrentEvent,*pTempEvent;
    pCurrentEvent = pPlayManager->pEvent; // this is the linked list of events - not a specific event.

    while (pCurrentEvent != NULL)
    {
        pTempEvent = pCurrentEvent;
        if (pCurrentEvent->ListUniqueID == ListID)
        {

            //HandleEndEvent(EhsPlayManagerType* pPlayManager, EhsPlayManagerEvent **ppEvent, ehs_bool endEvent, time_t currentt)
            removeEvent( pCurrentEvent, pPlayManager);
            /*todo should we run the tidy-up code (handle event) here too in case something is waiting on these, or is this impossible? */
        }
        pCurrentEvent = pTempEvent->pNext; // use temp as the current may have gone
    }
}



/*
 * Reset a memory reader to start at the beginning again.
 *  it will also reset the list to a state where it can be re-started as ithe first run
 */
void ResetMemoryReader(EhsPlayManagerEvent* pEvent,ehs_bool resetLoopCounter)
{
    if (resetLoopCounter)
    {
        pEvent->repeatCountDown = pEvent->repeatCount; /* chheky putting this here perhaps? */
    }
    pEvent->bAllChildrenExpired = EHS_TRUE; // always reset this when restrting a list. Assume true so any that don't expire during iteration set to false
    pEvent->bWaiting2EndList = EHS_FALSE;

    if (pEvent->memoryReader)
    {
        xmlTextReaderClose(pEvent->memoryReader);
        xmlFreeTextReader(pEvent->memoryReader); // safe to do this if not anceitn libxml2 library
    }
    pEvent->memoryReader=xmlReaderForMemory(pEvent->pPlaylist, pEvent->playlistSize, NULL,NULL, XML_PARSE_RECOVER);
    xmlTextReaderRead(pEvent->memoryReader); /* first call after reader is created */
    xmlTextReaderRead(pEvent->memoryReader); /* setup next read */
}



/*
 * Asserts outputs for objects, layouts for sections and iterates through sequence lists.
 *
 * Returns the status list reading.
 *
 * This function is called by the timer (can be called by others too - the PFI data is in the object to use.
 * The PFI data is shared with the fire function so macros for this apply to do port operations,
 * the pEvent info on what the context of the event is (e.g. a sequence begin)
 * And the PlayManager contains the current state of the XML parser.
 *
 *
 * returns status after identifying firing state
 */
EHS_LOCAL  EhsPlayManagerSMILNodeActionEnum fireEvent(EhsFunctionInstanceDataType* pFIdata, EhsPlayManagerEvent** ppEvent,EhsPlayManagerType* pPlayManager, ehs_bool bEndEvent, time_t currentt)
{

    ehs_char* nextTag = NULL;
    ehs_char* path = NULL;
    ehs_char element_type[32];// this written into by ProcessNextSMILbody().
    EhsPlayManagerEvent* pEvent = *ppEvent;
    //ehs_sint32 layoutId = -1;
    EhsPlayManagerSMILNodeActionEnum SMIL_chunk_property = EHS_PLAYMANAGER_SMIL_CHUNK_REMOVE;


    if (bEndEvent == EHS_FALSE)   // we are not an end event - so do the start event stuff:
    {
        if (pEvent->type == EHS_PLAYMANAGER_EVENT_NEWLIST) // We require further parsing for this type (  par, seq, excl.)
        {
            /* Do the SMIL processing at the top of a list and set flag for what we've read.*/
            if (pEvent->orderSeed > 0)   // assume this needs to be randomised
            {
                populateDynSeqList(pPlayManager,pEvent,currentt,EHS_FALSE);
            }

            SMIL_chunk_property = ProcessNextSMILbody(pEvent->memoryReader, ppEvent, pPlayManager, element_type, &(pEvent->eventTime),&(pEvent->eventEndTime),pEvent,currentt);
            if (pEvent->layoutSection != NULL) assertLayoutOutputs(pFIdata,pEvent);
            if (SMIL_chunk_property != EHS_PLAYMANAGER_SMIL_CHUNK_ENDED)  /* override return value as we know current state - parser doesn't */
            {
                SMIL_chunk_property = EHS_PLAYMANAGER_SMIL_CHUNK_KEEP;
            }
            else
            {
                SMIL_chunk_property = EHS_PLAYMANAGER_SMIL_CHUNK_REMOVE;
            }
            pEvent->type = EHS_PLAYMANAGER_EVENT_MIDLIST;
        }
        else if (pEvent->type == EHS_PLAYMANAGER_EVENT_MIDLIST) // We require further parsing for this type (  par, seq, excl.)
        {
            /* This doesn't fire an event - this just extracts the next object (or other!) */
            SMIL_chunk_property = ProcessNextSMILbody(pEvent->memoryReader,ppEvent, pPlayManager, element_type, &(pEvent->eventTime),&(pEvent->eventEndTime),pEvent,currentt); // we will just read an entry here
        }
        else if (pEvent->type == EHS_PLAYMANAGER_EVENT_NEWDYNAMICLIST)
        {
            pEvent->WaitingRefseqRead = EHS_TRUE; /* we don't want this to be done again on the system tick if we are still loading data.*/
            if (populateDynSeqList(pPlayManager,pEvent,currentt,EHS_TRUE))
            {
                pEvent->type = EHS_PLAYMANAGER_EVENT_NEWLIST; // now we can process this as normal new list
                SMIL_chunk_property = ProcessNextSMILbody(pEvent->memoryReader, ppEvent, pPlayManager, element_type, &(pEvent->eventTime),&(pEvent->eventEndTime),pEvent,currentt);
                if (pEvent->layoutSection != NULL) assertLayoutOutputs(pFIdata,pEvent);
            }
            else SMIL_chunk_property = EHS_PLAYMANAGER_SMIL_CHUNK_ENDED; // remove the chunk with the next steps
            if (SMIL_chunk_property != EHS_PLAYMANAGER_SMIL_CHUNK_ENDED)  /* override return value as we know current state - parser doesn't */
            {
                SMIL_chunk_property = EHS_PLAYMANAGER_SMIL_CHUNK_KEEP;
            }
            else
            {
                SMIL_chunk_property = EHS_PLAYMANAGER_SMIL_CHUNK_REMOVE;
            }
            pEvent->type = EHS_PLAYMANAGER_EVENT_MIDLIST;
        }
        else if (pEvent->type == EHS_PLAYMANAGER_EVENT_OBJECT)
        {
            if (pEvent->pMediaSource)
            {

                path = lookupMedia(pEvent->pMediaSource, pPlayManager);
            }
            else
            {
                path = NULL; /*leave path as null and let the output assert function choose the alternative action when missing.*/
            }
            pPlayManager->pWaitingOnEvent = pEvent->SendersObjectUniqueID; // when we assert objects we need set the port output marshaling flags.
            pPlayManager->pWaitingOnEventCounter = EHS_WAITINGFORSTARTACKCOUNTDOWN ;
            assertObjectEventOutputs(pFIdata, pEvent, path, EHS_FALSE); /* Assert outputs to start playing */
            if (pEvent->eventEndTime && (pEvent->eventEndTime != EHS_PLAYMANAGER_TIMER_UNDEFINED))
            {
                SMIL_chunk_property = EHS_PLAYMANAGER_SMIL_CHUNK_KEEP; // keep the event so that the end time event is used
            }
            else
            {
                SMIL_chunk_property = EHS_PLAYMANAGER_SMIL_CHUNK_REMOVE; // if no end time then remove the event
            }
        }
    }

    else
    {
        /* We're an end event expiration -  end the event (remove from list and assert stuff if we are an object...).
        			This will not remove the event if it is an immediate object. AS we wait for the acknowledge before we do that.
        			However perhaps we should/could?
        		*/
        /* Determine if this playlist is the current one being played ->ListUniqueID is the indicator (we could also test one of the list types...)*/
        if (pEvent->ListUniqueID)
        {
            /* Do the following in fire events and cleanup () function */
            SMIL_chunk_property = SMIL_chunk_property != EHS_PLAYMANAGER_SMIL_CHUNK_ENDED;
            //pEvent->type = EHS_PLAYMANAGER_EVENT_NEWLIST; // this is so it's ready to start again...
            //ResetMemoryReader(pEvent); // this should have already been done in HandleEndEvent();
        }
        else     // end event of an object
        {
            if (pEvent->pMediaSource)
            {
                path = lookupMedia(pEvent->pMediaSource, pPlayManager);
            }
            else
            {
                path = NULL; /*leave path as null and let the output assert function choose the alternative action when missing.*/
            }
            assertObjectEventOutputs(pFIdata, pEvent, path, EHS_TRUE); /* Assert the stop event with all that goes with that*/
            /* Don't remove because ack doesn't work : removeEvent(pEvent, pPlayManager); // !!!!!!!!!!!!!!!!!! PMLD should we be ding this in handleend event?*/
            //SMIL_chunk_property = EHS_PLAYMANAGER_SMIL_CHUNK_REMOVE;
            /* We still need to register stopped acknowledges */
        }
    }

    return SMIL_chunk_property;
}

/*** @brief
 *  removes the linked list of children that parallel events keep
 *  */
void freeParList(EhsPlayManagerEvent* pEvent)
{
    EhsPlayManagerEventListForParType* parEvent = pEvent->parEventList;
    EhsPlayManagerEventListForParType* parEventLast;
    while (parEvent != NULL)
    {
        parEventLast = parEvent;
        parEvent = parEvent->pNext;
        EhsPlayManagerEventListForParType(parEventLast);
    }
}

/* Free the memory used by an event entry */
void FreeAllEventMemory(EhsPlayManagerEvent* pEvent)
{
    if (pEvent)
    {
        freeParList(pEvent);
        if (pEvent->class)
            xmlFree(pEvent->class);
        if (pEvent->soundLevel)
            xmlFree(pEvent->soundLevel);
        if (pEvent->pathToRefFile)
            xmlFree(pEvent->pathToRefFile);
        if (pEvent->dynamicInnerXml)
        {

            //if (pEvent->seed - this is an integer - not string.
#ifdef EHS_DONT_USE_EHS_MANAGED_HEAP
            free (pEvent->dynamicInnerXml);
#else
            EhsHMem_tempFree(pEvent->dynamicInnerXml);
#endif
        }
        if (pEvent->pMediaSource)
            xmlFree(pEvent->pMediaSource);
        if (pEvent->region)
            xmlFree(pEvent->region);
        if (pEvent->layoutSection)
            xmlFree(pEvent->layoutSection);
        if (pEvent->srcType)
            xmlFree(pEvent->srcType);
        //if (pEvent->fade) - this is in integer
        // 			xmlFree(pEvent->fade);
        if (pEvent->memoryReader)
        {
            xmlTextReaderClose(pEvent->memoryReader);
            pEvent->pPlaylist = NULL; // Possibly this is deallocated with above as the next line crashes
            xmlFreeTextReader(pEvent->memoryReader);
        }
        if (pEvent->pPlaylist)
        {
            xmlFree(pEvent->pPlaylist);
        }

        //  Note: pEvent->tagType is a const deallocated when the reader is finished.
        EhsHMem_tempFree(pEvent);
        pEvent=NULL;
    }
}


/* This function is called from the interface for indicating a media object has been started and new data can now be sent out.
 * This only needs to look at the last event that has been posted (We hardly need to do use an ID but we will check anyway
 *
 * This function is similar to the played function except started is serialised so that start events must be dealt with sequentially
 * as they are produced.
 */

EHS_FB_RUN_FUNCTION(PlayManager_Started)
{
    EhsPlayManagerType* pPlayManager = (EhsPlayManagerType*) EHS_FB_RUN_CONTEXT;
    ehs_uint32 played;

    if (pPlayManager->pWaitingOnEvent == 0)
    {
        EHSH_LOG_ERROR("We were'nt waiting for an event, but should have been");
        return;
    }

    EhsTPMutex_lock(EhsTPMutex_fbIO);
    played = EHS_FB_IN_I(0);
    EhsTPMutex_unlock(EhsTPMutex_fbIO);
    EhsTPMutex_lock(EhsTPMutex_playManager);
    if ( pPlayManager->pWaitingOnEvent== played)
    {
        pPlayManager->pWaitingOnEvent = 0; // signal to fire that the next fire can be done.
        //pPlayManager->pWaitingOnEventCounter = 0; // don't need to reset this

    }
    else
    {
        //EHSH_LOG_WARNING("STARTED: Didn't get the marshalling ID (%u - expecting %u) that the previous has been played yet\n",(unsigned int)played, (unsigned int)pPlayManager->pWaitingOnEvent);
    }
    EhsTPMutex_unlock(EhsTPMutex_playManager);
    // EHS_FB_FINISH(1); - We don't assert a done for this event - port removed.
}

/**
 * \brief This sets the waiting flag for a sequence list with the fired event ID or the ended list ID.
 *
 * It also signals that the next sequence (if it was a sequence) should play by setting
 * 	pPlayManager->pWaitingOnEvent = 0
 */
ehs_bool NotifyAnObjectIsComplete(EhsPlayManagerType* pPlayManager, ehs_uint32 played)
{
    EhsPlayManagerEvent *pCurrentEvent = NULL;
    pCurrentEvent = pPlayManager->pEvent;
    while (pCurrentEvent != NULL)
    {

        if (pCurrentEvent->WaitingForObjectUniqueID == played)
        {
            pCurrentEvent->WaitingForObjectUniqueID = 0; //set this to zero - it will be reset if there is more for this event.
        }
        pCurrentEvent = pCurrentEvent->pNext;
    }
    return EHS_TRUE;
}

/*
 * adds a new ID to a parallel list of child objects that must be done before the par list is finished.
 * two copies of this are maintained to make resetting easie for repeat versions.
 */

ehs_bool AddParallelListMustComplete(EhsPlayManagerEvent * parEvent,
                                     ehs_uint32 ID)
{
    EhsPlayManagerEventListForParType *IDlink = NULL;
    EhsPlayManagerEventListForParType *IDlinktemp = NULL;

    IDlink = parEvent->parEventList;
    if (IDlink == NULL)   // this means we are the beginning of the list.
    {
        parEvent->parEventList = EhsHMem_tempAlloc(sizeof(EhsPlayManagerEventListForParType));
        parEvent->parEventList->pNext = NULL;
        parEvent->parEventList->ID = ID;
    }
    else
    {
        while (IDlink != NULL)
        {
            IDlinktemp = IDlink;
            IDlink = IDlink->pNext;
        }
        if (IDlinktemp)
        {
            IDlinktemp->pNext = EhsHMem_tempAlloc(sizeof(EhsPlayManagerEventListForParType));
            if (IDlinktemp->pNext)
            {
                IDlinktemp->pNext->pNext = NULL;
                IDlinktemp->pNext->ID = ID;
            }
        }
    }
    return EHS_TRUE;
}


// if we are going to repeat a parallel list then we need to reset it.
// NOTE THIS IS NOT CURENTLY USED - SHOULDNOTBE USED FOR LOOP RESETS PROBABLY
void resetParalleList(EhsPlayManagerEvent *pEvent)
{
    ResetMemoryReader(pEvent,EHS_TRUE);
}


/* Called when an element of a parallel list has completed (and can  be run other times too if you're not sure!).
 * Each parallel event has a linked list of IDs of it's children - when the list is 0 we can move on!
 *
 *
 * Cleans up any events that are played in a parallel list.
 *
 *The current time must be provided here to decide if a list should be removed if the end time has expired - though it should probably have gone already if it had expired
 * */

ehs_bool NotifyParallelListIsComplete(EhsPlayManagerType* pPlayManager,ehs_uint32 played,time_t currentt)
{
    EhsPlayManagerEvent *pCurrentEvent = NULL;
    EhsPlayManagerEventListForParType *parEventList = NULL;
    EhsPlayManagerEventListForParType *parEventListPrev = NULL;
    EhsPlayManagerEventListForParType *tempEventList = NULL;
    ehs_bool ret=EHS_FALSE;// return not the end of the list here...
    ehs_bool remove = EHS_FALSE; // irrespective of expiry time???
    pCurrentEvent = pPlayManager->pEvent; 									// this is the first linked list of events - not a specific event.
    ehs_bool restartEventList= EHS_FALSE;

    /* We need this sometimes to decide what lists to delete */

    while (pCurrentEvent != NULL)
    {
        if (pCurrentEvent->parEventList)   									// this has a list of events in a par group.
        {
            parEventList = pCurrentEvent->parEventList;
            parEventListPrev = NULL;
            while (parEventList != NULL)
            {

                if (parEventList->ID == played)   							// remove ID from list.
                {
                    /* First remove the parallell list */
                    if (parEventListPrev)  									// we are not at the beginning of the list
                    {
                        parEventListPrev->pNext = parEventList->pNext; 		// this skips the entry
                    }
                    else   //this is the head of the list
                    {
                        pCurrentEvent->parEventList = parEventList->pNext; 	// this skips the entry too
                    }
                    tempEventList = parEventList;
                    parEventList = parEventList->pNext; 				// set the next version
                    EhsHMem_tempFree(tempEventList); 					// remove this we don't need anything now - just need to tidy up the links for the remaining search.
                    /* Now check if the list is empty - then this node should be removed */
                    if (pCurrentEvent->parEventList == NULL)   			// IMPORTANT: this is the end of the list - this means the node has finished...
                    {

                        //#define CAN_HANDLE_THIS_DIFFERENTLY

                        /* Potential end of a parallel list - but check end time first */
                        if (pCurrentEvent->eventEndTime == 0 || pCurrentEvent->eventEndTime == EHS_PLAYMANAGER_TIMER_UNDEFINED ||  currentt > pCurrentEvent->eventEndTime )
                        {
                            HandleEndEvent(pPlayManager,&pCurrentEvent,EHS_TRUE,currentt);
                        }
                        /* don't restart the list if we haven't deleted the event */
                        if (pCurrentEvent == NULL)
                        {
                            restartEventList= EHS_TRUE;
                            break;
                        }
                    }
                }
                else   												// skip to next
                {
                    parEventListPrev = parEventList;
                    if (parEventList)
                        parEventList = parEventList->pNext;
                }
            }
        }/* end parlist while */
        if (restartEventList == EHS_TRUE)
        {
            pCurrentEvent = pPlayManager->pEvent;
            restartEventList= EHS_FALSE;
            continue;
        }
        else pCurrentEvent = pCurrentEvent->pNext;
    }
    return ret;
}

/* This function is called from the interface for indicating a media object has completed.
 * This typically leads to the next file in a sequence if we are in a sequence section.
 *
 * this function will look up the media that has been played:
 * if this is for any type (par or seq)
 * it will Read to the next node from the XMIL snippet in the pEvent structure.
 * if this is not the end of sequence it will post this to be fired next time.
 *
 */

EHS_FB_RUN_FUNCTION(PlayManager_Played)
{
    EhsPlayManagerType* pPlayManager = (EhsPlayManagerType*) EHS_FB_RUN_CONTEXT;
    ehs_char *nextTag = NULL;
    ehs_char *value = NULL;
    ehs_char *path = NULL;
    EhsPlayManagerEvent *pTempEvent,*pCurrentEvent = NULL;
    //EhsPlayManagerEvent **ppCurrentEvent = NULL;
    time_t currentt = time(NULL);

    ehs_uint32 played;

    EhsTPMutex_lock(EhsTPMutex_playManager);
    //loop through and set the ID to 0 (indicating the parent list is ready to continue).
    played = EHS_FB_IN_I(0); // we only read the ID here
    pCurrentEvent = pPlayManager->pEvent; // this is the linked list of events - not a specific event.
    //first look for the start event and remove from the list so we don't have stray end events in the list
    //remove the send event:
    ehs_bool notifyAllDone = EHS_TRUE;
    while (pCurrentEvent != NULL)
    {
        pTempEvent = pCurrentEvent->pNext;
        if (pCurrentEvent->SendersObjectUniqueID == played)
        {
            HandleEndEvent(pPlayManager,&pCurrentEvent,EHS_TRUE,currentt); // update start and stop times of the waiting event.
            if (pCurrentEvent == NULL)   /* Something has been deleted so start the loop again - DON'T OTHERWISE!*/
            {
                pCurrentEvent = pPlayManager->pEvent;
                continue; // restart the loop from the begining to avod dangling
            }
        }
        pCurrentEvent = pTempEvent; //Note (code above) there is a possibility that if we notified other lists the pNext could be left dangling
    }
    EhsTPMutex_unlock(EhsTPMutex_playManager);
    EHS_FB_FINISH(1);
}

/* Update the two variables that contain the earliest and latest events - and write the data directly to the output ports
 * Trusts all input variables are valid
 * */
void UpdateTimeWindow(EhsFunctionInstanceDataType* pFIdata,EhsPlayManagerEvent * pCurrentEvent, time_t currentt)
{

    EhsPlayManagerType* pPlayManager = (EhsPlayManagerType*) EHS_FB_RUN_CONTEXT;
    if (pCurrentEvent == NULL) return;
    //EhsTPMutex_lock(EhsTPMutex_playManager);

    time_t * earliestPlayTime  = &pPlayManager->earliestPlayTime;
    time_t * earliestEndTime   = &pPlayManager->earliestEndTime;
    EhsPlayManagerEventTypeEnum * earliestPlayNodeType = &pPlayManager->earliestPlayTimeNodeType;
    EhsPlayManagerEventTypeEnum * earliestEndNodeType =  & pPlayManager->earliestEndTimeNodeType;

    if (pCurrentEvent->eventTime && pCurrentEvent->eventTime != EHS_PLAYMANAGER_TIMER_UNDEFINED && pCurrentEvent->eventTime != 0)
    {
        if ((*earliestPlayTime == 0) || (*earliestPlayTime == EHS_PLAYMANAGER_TIMER_UNDEFINED))
        {
            *earliestPlayTime = pCurrentEvent->eventTime;
            *earliestPlayNodeType = pCurrentEvent->type;
            pPlayManager->changedStart = EHS_TRUE;
        }
        else  // see if we have any actual
        {
            /*
             * We want to show times for current played stuff or next to be played if nothing is playing. hence:
             *
             * Rules:
             *  Start Events:
             *   1. earliest object
             *   2. or latest past started list
             *   3. or earliest next list
             *  End Events:
             *  1. Earlist of any kind - assume we won't get them in the past.
            */
            if ( *earliestPlayNodeType == EHS_PLAYMANAGER_EVENT_OBJECT )
            {
                if (pCurrentEvent->type == EHS_PLAYMANAGER_EVENT_OBJECT )
                {
                    if (pCurrentEvent->eventTime < *earliestPlayTime)
                    {
                        *earliestPlayTime = pCurrentEvent->eventTime;
                        *earliestPlayNodeType = EHS_PLAYMANAGER_EVENT_OBJECT;
                        pPlayManager->changedStart = EHS_TRUE;
                    }
                }
                else   // don't override an object with any list types unless in the future - then find the earliest
                {
                    if (*earliestPlayTime > currentt && pCurrentEvent->eventTime > currentt && pCurrentEvent->eventTime < *earliestPlayTime)
                    {
                        *earliestPlayTime = pCurrentEvent->eventTime;
                        *earliestPlayNodeType = pCurrentEvent->type;
                        pPlayManager->changedStart = EHS_TRUE;
                    }

                }
            } // earliest is not object so override with the most recent or the earliest next
            else
            {
                if (pCurrentEvent->type == EHS_PLAYMANAGER_EVENT_OBJECT )   // aways override with an object of we have one with a start time.
                {
                    *earliestPlayTime = pCurrentEvent->eventTime;
                    *earliestPlayNodeType = EHS_PLAYMANAGER_EVENT_OBJECT;
                    pPlayManager->changedStart = EHS_TRUE;
                }
                else
                {
                    if (pCurrentEvent->eventTime > currentt && *earliestPlayTime > currentt && pCurrentEvent->eventTime < *earliestPlayTime  )   // aways override with an object of we have one with a start time.
                    {
                        *earliestPlayTime = pCurrentEvent->eventTime;
                        *earliestPlayNodeType = pCurrentEvent->type;
                        pPlayManager->changedStart = EHS_TRUE;
                    }
                    else if (pCurrentEvent->eventTime < currentt && *earliestPlayTime < currentt && pCurrentEvent->eventTime > *earliestPlayTime  )    // aways override with an object of we have one with a start time.
                    {
                        *earliestPlayTime = pCurrentEvent->eventTime;
                        *earliestPlayNodeType = pCurrentEvent->type;
                        pPlayManager->changedStart = EHS_TRUE;
                    }
                    else if (pCurrentEvent->eventTime < currentt && *earliestPlayTime > currentt  )   // aways override with an object of we have one with a start time.
                    {
                        *earliestPlayTime = pCurrentEvent->eventTime;
                        *earliestPlayNodeType = pCurrentEvent->type;
                        pPlayManager->changedStart = EHS_TRUE;
                    }
                }
            }
        }
    }
    /* End times   - always choose the earliest */
    if (pCurrentEvent->eventEndTime && pCurrentEvent->eventEndTime != EHS_PLAYMANAGER_TIMER_UNDEFINED && pCurrentEvent->eventEndTime != 0)   // end times are either objects or list end times ..
    {
        if ((*earliestEndTime == 0) || (*earliestEndTime == EHS_PLAYMANAGER_TIMER_UNDEFINED))
        {
            *earliestEndTime = pCurrentEvent->eventEndTime;
            pPlayManager->changedEnd = EHS_TRUE;

        }
        else   // we have valid event times and earliest times
        {
            if (pCurrentEvent->eventEndTime < *earliestEndTime)   // assume if end is in the past the event will not exist
            {
                *earliestEndTime = pCurrentEvent->eventEndTime;
                pPlayManager->changedEnd = EHS_TRUE;
            }
        }
    }
    if (*earliestEndTime < *earliestPlayTime)
    {
        *earliestEndTime = 0;
        pPlayManager->changedEnd = EHS_TRUE;
    }
    //EhsTPMutex_unlock(EhsTPMutex_playManager);
}

//asserts ports of the start or ent times have changed from previous iterations

void AssertTimeWindowPorts(EhsFunctionInstanceDataType* pFIdata, EhsPlayManagerType* pPlayManager)
{
    time_t * earliestPlayTime  = &pPlayManager->earliestPlayTime;
    time_t * earliestEndTime   = &pPlayManager->earliestEndTime;
    ehs_char date_string[EHS_STRING_LENGTH_MAX];
    struct tm tm;

    //if (pPlayManager->changedStart || pPlayManager->changedEnd) {
    if (pPlayManager->changedStart == EHS_TRUE)
    {
        if (*earliestPlayTime == 0)
        {
            EhsStrcpy(EHS_FB_OUT_S(EHS_FB_PLAYMANAGER_TICK_NEXT_EVENT_START),"");
        }
        else
        {
#ifdef EHS_TRY_TO_FORCE_GMT
            gmtime_r(earliestPlayTime, &tm);
#else
            localtime_r(earliestPlayTime, &tm); /* @todo proper fix maybe to feed time into something so it's not gmt... */
            tm.tm_isdst = -1; // check the DST is correct if localtime_r doesn't sort it out properly
            mktime(&tm);
#endif

            EhsSprintf(EHS_FB_OUT_S(EHS_FB_PLAYMANAGER_TICK_NEXT_EVENT_START), "%02d-%02d-%02d %02d:%02d:%02d", (tm.tm_year) % 100,
                       tm.tm_mon+1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);//, tm.tm_wday, tm.tm_yday);
        }
    }
    if (pPlayManager->changedEnd == EHS_TRUE)
    {
        if (*earliestEndTime == 0)
        {
            EhsStrcpy(EHS_FB_OUT_S(EHS_FB_PLAYMANAGER_TICK_NEXT_EVENT_END),"");
        }
        else
        {
#ifdef EHS_TRY_TO_FORCE_GMT
            gmtime_r(earliestEndTime, &tm);
#else
            localtime_r(earliestEndTime, &tm); /* @todo proper fix maybe to feed time into something so it's not gmt... */
            tm.tm_isdst = -1; // check the DST is correct if localtime_r doesn't sort it out properly
            mktime(&tm);
#endif
            EhsSprintf(EHS_FB_OUT_S(EHS_FB_PLAYMANAGER_TICK_NEXT_EVENT_END), "%02d-%02d-%02d %02d:%02d:%02d", (tm.tm_year) % 100,
                       tm.tm_mon+1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);//, tm.tm_wday, tm.tm_yday);
        }
    }

    EHS_FB_FINISH(EHS_FB_PLAYMANAGER_TICK_NEXT_EVENT_CHANGED);


    //} - dome this check already in the calling code
    pPlayManager->changedStart  = EHS_FALSE;
    pPlayManager->changedEnd = EHS_FALSE;
}


/*** @brief Event list debug printer for Tick() function
 *
 */
void EhsPrintDebugEventList(EhsPlayManagerType* pPlayManager,EhsPlayManagerEvent * pCurrentEvent)
{

#ifdef EHS_MACHINEY_DEBUG
          /* pPlayManager->pWaitingOnEvent,
           (unsigned int)pCurrentEvent->bCurrentPlayStarted,
           (unsigned int)pCurrentEvent->repeat,
           (unsigned int)pCurrentEvent,
           (unsigned int)pCurrentEvent->SendersObjectUniqueID,
           (unsigned int)pCurrentEvent->WaitingForObjectUniqueID,
           (unsigned int)pCurrentEvent->ListUniqueID,
           (unsigned int)pCurrentEvent->eventTime,
           (unsigned int)pCurrentEvent->eventEndTime,
           (char*)pCurrentEvent->pMediaSource,
           pCurrentEvent->type,
           pCurrentEvent->eventListType,
           pCurrentEvent->layoutSection,
           pCurrentEvent->region
          );*/
#else
    char startTime[128];
    char endTime[128];
//char* ptr, size_t maxsize, const char* format,const struct tm* timeptr
    struct tm *timeTm;
    if (pCurrentEvent->eventTime != 0 && pCurrentEvent->eventTime != EHS_PLAYMANAGER_TIMER_UNDEFINED)
    {
        timeTm=localtime(&pCurrentEvent->eventTime);
        strftime(startTime, 128, "%a %T",timeTm);
    }
    else
    {
        strcpy(startTime,"--- --:--:--");
    }
    if (pCurrentEvent->eventEndTime != 0 && pCurrentEvent->eventEndTime != EHS_PLAYMANAGER_TIMER_UNDEFINED)
    {
        timeTm=localtime(&pCurrentEvent->eventEndTime);
        strftime(endTime, 128, "%a %T",timeTm);
    }
    else
    {
        strcpy(endTime,"--- --:--:--");
    }
/*
           (unsigned int)pCurrentEvent->bCurrentPlayStarted,
           (unsigned int)pCurrentEvent->SendersObjectUniqueID,
           (unsigned int)pCurrentEvent->WaitingForObjectUniqueID,
           (unsigned int)pCurrentEvent->ListUniqueID,
           startTime,endTime,
           pCurrentEvent->type,
           (unsigned int)pCurrentEvent->repeat,
           pCurrentEvent->eventListType,
           pCurrentEvent->pParent?pCurrentEvent->pParent->SendersObjectUniqueID:-1,
           (char*)pCurrentEvent->pMediaSource,
           pCurrentEvent->layoutSection,
           pCurrentEvent->region
          );*/
#endif

}

/*** @brief Updates the end time of an event when it is started, based in the start and end time of it's parents.
 * (which should normally be set at the point the event is fired)
 *
 * This should be called before the firing test is carried out so the event can be eaten if it has already expired
 * It is assume that any parent end times will have been updated with a duration.
 * if no end times or durations for parents are found then the current duration is applied from now.
 * @todo consider adding the usual parent start and end time infor into this function.
 *
 * returns of the resulting end time has expired.
 */
ehs_bool updateEndTimeWithDuration(EhsPlayManagerEvent * pCurrentEvent,time_t currentt)
{
    ehs_bool bUseRelativeEndTime = EHS_FALSE;

    EhsTPMutex_lock(EhsTPMutex_playManager);
    EhsPlayManagerEvent *event = pCurrentEvent;
    //EhsPlayManagerEvent *event = pCurrentEvent;
    time_t earliest = 0 ; //EHS_PLAYMANAGER_TIMER_UNDEFINED;

    time_t thisEndTime = 0 ; //EHS_PLAYMANAGER_TIMER_UNDEFINED;
    /* if we have a specific start time then the duration will be relative to this, otherwise
     * if we are in a sequence then the duration is relevant to when ever the item is started */
    if (pCurrentEvent->eventDuration != EHS_PLAYMANAGER_TIMER_UNDEFINED)
    {
        if (pCurrentEvent->eventTime > 0 && pCurrentEvent->eventTime != EHS_PLAYMANAGER_TIMER_UNDEFINED)
            thisEndTime = pCurrentEvent->eventDuration/1000  + pCurrentEvent->eventTime;
        else
        {
            thisEndTime = pCurrentEvent->eventDuration/1000  + currentt;
            bUseRelativeEndTime = EHS_TRUE;

        }
    }
    //first work out the earliest end time for this event
    if (event->eventEndTime != 0 && event->eventEndTime != EHS_PLAYMANAGER_TIMER_UNDEFINED &&
            (thisEndTime  == 0 || event->eventEndTime < thisEndTime ))
    {
        earliest = event->eventEndTime;
        bUseRelativeEndTime = EHS_FALSE;

    }
    else
    {
        if (pCurrentEvent->eventDuration != EHS_PLAYMANAGER_TIMER_UNDEFINED)
        {
            earliest = thisEndTime; // set end time to relevant time

        }
        else
        {
            //bUseRelativeEndTime = EHS_FALSE;
            earliest = EHS_PLAYMANAGER_TIMER_UNDEFINED; // leave this max 2038
        }
    }

    /* then get the earliest end time of any parent*/ //@todo - do we need this really? - doesn't recursive notification handle this?
    event = event->pParent;
    while (event)
    {
        if (event->eventEndTime && event->eventEndTime != EHS_PLAYMANAGER_TIMER_UNDEFINED &&
                (earliest == 0 || event->eventEndTime < earliest ))
        {

            earliest = event->eventEndTime;
            if (event->bRelativeEndTime == EHS_FALSE) bUseRelativeEndTime = EHS_FALSE;
        }
        event = event->pParent;
    }
    if (bUseRelativeEndTime) pCurrentEvent->bRelativeEndTime = EHS_TRUE;
    else pCurrentEvent->bRelativeEndTime = EHS_FALSE;
    // We should now have the earliest end time @todo the following iff can be removed and assign EHS_PLAYMANAGER_TIMER_UNDEFINED in all cases
    pCurrentEvent->eventEndTime = earliest;
    EhsTPMutex_unlock(EhsTPMutex_playManager);
    return EHS_TRUE;

}

/*************************************************************************************************************
 * 	TOP LEVEL FUNCTION! - fireEventandCleanUp()
 *
 * Key Sub Call Responsibilities:
 *
 * 1 - fireEventandCleanUp() - calls fireEvent() to assert outputs or read a list (see 2).
 *                           - for end envents ONLY! it updates repeat timings removes any events/lists that are not to be repeated.
 *
 * 2 - fireEvent()   - Asserts outputs for objects, layouts for sections or iterates through sequence lists.
 *                   - Returns the status of list reads to return (does not do any updates on these) .
 *
 *
 *3  - ProcessNextSMILbody () - Called only to handle list events.
 *                            - This may handle it's own Event Timings for special cases such as when the end of a sequence list is reached (i.e. wont be handle in fireEventandCleanUp().
 *                            - par list ending are always handled by an end event being fired as a result of a tick expiry.
 *
 *
 **************************************************************************************************************/
EhsPlayManagerSMILNodeActionEnum fireEventandCleanUp( EhsFunctionInstanceDataType* pFIdata, EhsPlayManagerEvent** pEvent, EhsPlayManagerType* pPlayManager, time_t currentt, ehs_bool bEndEvent)
{

    // remove this : if ( currentt > 0)  pEvent->startedTime = currentt; // assign the started time so children mightuse it calculate relative timings
    if (!pFIdata)
    {
        EHSH_LOG_ERROR("No Valid object handle exiting");
        return EHS_PLAYMANAGER_SMIL_CHUNK_CANTREAD;
    }
    if (!pEvent || !*pEvent)
    {
        return EHS_PLAYMANAGER_SMIL_CHUNK_CANTREAD;
    }
    /* Fire the event - this may call other calls into fire events if parsing SMIL is caused */
    EhsPlayManagerSMILNodeActionEnum SMIL_chunk_property = fireEvent(pFIdata,pEvent, pPlayManager, bEndEvent,currentt); // returns EHS_PLAYMANAGER_SMIL_CHUNK_REMOVE for all end events
    // - we do this for all events in case they might have finished already ... if (bEndEvent) { /* Check if this is an end event */
    if (*pEvent != NULL)
    {
        (*pEvent)->bCurrentPlayStarted = EHS_TRUE; // this allows this to be tested for the end time and fire will ignore it...
        if (SMIL_chunk_property != EHS_PLAYMANAGER_SMIL_CHUNK_KEEP)   // a list event that hasn't finished or any other type of event
        {
            // Only handle events if the fire_event function hasn't already or they will be handled when their end has actually been acknowledged:
            // Objects will be handled in fireEvent() and we wait for the ack before removing them, but we don't with lists that are finished don't want to double count.
            if (*pEvent != NULL && (*pEvent)->type != EHS_PLAYMANAGER_EVENT_OBJECT )
            {
                if (bEndEvent)
                {
                    if (HandleEndEvent(pPlayManager,pEvent,bEndEvent,currentt) == EHS_TRUE)   // event has been removed
                    {
                        *pEvent = NULL;
                    }
                    else
                    {
                    }
                }
                else
                {
                    HandleStartEvent(pPlayManager,pEvent,currentt) ;
                }
            }
            else
            {
#ifdef EHS_REMOVE_EVENTS_BEFORE_ACKNOWLEDGE
                /* We usually wait forthe ack from the player before sorting out the event nrepeats or removal */
                if (bEndEvent)
                {
                    if (HandleEndEvent(pPlayManager,pEvent,EHS_TRUE,currentt) == EHS_TRUE)   // event has been removed
                    {
                        *pEvent = NULL;
                    }
                    else
                    {
                    }
                }
#endif
            }
        }
        else
        {
            /* We can quietly remove the object now and let the finish ackowledge do any notifications. - this may never run?
            				@todo we could set a time-out for this to be removed if no ackowledge happens, in which case we would do all the notifications at the timeout too*/
            if (bEndEvent)
            {
                if (HandleEndEvent(pPlayManager,pEvent,EHS_TRUE,currentt) == EHS_TRUE)   // event has been removed
                {
                    *pEvent = NULL;
                }
                else
                {
                }
            }
            //why do we get here what is the chunk type
        }
    }
    else
    {
    }
    return SMIL_chunk_property;
}

/* Utility used by HandleEndEvent() to check if a completed event has actually expired
  returns true if the event has actually expired and false if it might have just run out of items.
*/
ehs_bool checkEventEndTimeAndFlagEvent( EhsPlayManagerEvent *pEvent, time_t currentt, time_t unupdatedEndTime)
{
    EhsTPMutex_lock(EhsTPMutex_playManager);
    if (pEvent->eventEndTime == 0 || pEvent->eventEndTime == EHS_PLAYMANAGER_TIMER_UNDEFINED ||  currentt >= unupdatedEndTime )   // check if the first iteration has expired
    {
        pEvent->bWaiting2EndList = EHS_FALSE;
        EhsTPMutex_unlock(EhsTPMutex_playManager);
        return EHS_TRUE;
    }
    else
    {
        pEvent->bWaiting2EndList = EHS_TRUE;
        EhsTPMutex_unlock(EhsTPMutex_playManager);
        return EHS_FALSE;
    }
}

/** Checks all events to see if a parent end time has accured.
 * If so the event is stopeed/fired and removed (no repeats are continued)
 *
 * returns true if expired parent found
 */
ehs_bool checkEventParentExpiry(EhsPlayManagerType* pPlayManager,time_t currentt)
{

    ehs_bool ret = EHS_FALSE;
    EhsTPMutex_lock(EhsTPMutex_playManager);
    EhsPlayManagerEvent *pTempEvent,*parentEvent,*tempParentEvent;
    EhsPlayManagerEvent *pCurrentEvent = pPlayManager->pEvent;
    while (pCurrentEvent != NULL)
    {
        pTempEvent = pCurrentEvent->pNext;
        parentEvent = pCurrentEvent->pParent;
        while (parentEvent)
        {
            tempParentEvent = parentEvent->pParent;
            if (parentEvent->eventEndTime && parentEvent->eventEndTime != EHS_PLAYMANAGER_TIMER_UNDEFINED &&
                    parentEvent->eventEndTime <= currentt )
            {
                // remove this event by setting its end time to expire, so it gets handled normally.
                pCurrentEvent->eventEndTime = currentt;
                pCurrentEvent->repeat=EHS_PLAYMANAGER_REPEAT_NONE;
                //if (pCurrentEvent->bAllChildrenExpired) pCurrentEvent->bAllChildrenExpired = EHS_TRUE; // shouldn't need to do this here so removing
                pCurrentEvent->repeatCountDown=-1; // just in case
                ret = EHS_TRUE;
            }
            parentEvent = tempParentEvent;
        }
        pCurrentEvent = pTempEvent; //Note (code above) there is a possibility that if we notified other lists the pNext could be left dangling
    }
    EhsTPMutex_unlock(EhsTPMutex_playManager);
    return ret;
}

/* Handlestart event - case some timeup dates are needed */
ehs_bool HandleStartEvent(EhsPlayManagerType* pPlayManager, EhsPlayManagerEvent **ppEvent, time_t currentt)
{
    EhsTPMutex_lock(EhsTPMutex_playManager);
    //UpdateEventTime(*ppEvent,EHS_FALSE,currentt,EHS_FALSE); //update the time
    //NotifyAnObjectIsComplete(pPlayManager,(*ppEvent)->SendersObjectUniqueID); /*need to remove this ID from any waiters and let them move on*/
    //NotifyParallelListIsComplete(pPlayManager,(*ppEvent)->SendersObjectUniqueID,currentt); // This will set a flag on the par event to complete.
    //checkEventParentExpiry() ; //

    EhsTPMutex_unlock(EhsTPMutex_playManager);

    return EHS_TRUE;
}



/***
 *
 * Call when an object or list item has finished.
 *
 * This will handle the repeat and event removal behaviour
 *
 * currentt is needed for notifications - can be null.
 *
 * Expected behaviour:
 *
 * EHS_PLAYMANAGER_REPEAT_NONE 			: remove event always
 * EHS_PLAYMANAGER_REPEAT_IMMEDIATELY	: updated time and report if updated or not. if repeated the
 * EHS_PLAYMANAGER_REPEAT_[any period]	: update the time and don't remove
 *
 * In cases where an end time is available the expiry will be checked in case of an early finish and a flag is set to give the end time "minimum behaviour"
 * For non-immedate repeaters the end time is treated as a maximum time of the content.
 *
 * todo Why do we sometimes need to call this when it is not an end event? Is this the only way to start the first repeats and set the actual starte end times depending on current time?
 *
 */
ehs_bool HandleEndEvent(EhsPlayManagerType* pPlayManager, EhsPlayManagerEvent **ppEvent, ehs_bool endEvent, time_t currentt)
{
    EhsTPMutex_lock(EhsTPMutex_playManager);
    EhsPlayManagerEvent *pEvent = *ppEvent;

    //print_trace();

    time_t unupdatedEndTime = pEvent->eventEndTime;

    ehs_bool _repeat = EHS_FALSE;
    ehs_bool _loop = EHS_FALSE;
    ehs_bool _doNotifications = EHS_TRUE;
    ehs_bool _removeEvent = EHS_TRUE;

    /* First see if this is a loop event finishing and if it might have expired one way or another.*/
    if (currentt == 0 ) currentt = time(NULL); // for cases we didn't already have the current time.

    if (pEvent->repeatCount != EHS_PLAYMANAGER_LOOP_NONE && CheckUpdateEventLoop(pEvent, currentt) == EHS_TRUE)
    {
        _loop = EHS_TRUE;
        _doNotifications = EHS_FALSE;
        _removeEvent = EHS_FALSE;
        //ResetMemoryReader(pEvent); //we do this later after checking if it is a repeater also
    }
    else
    {
    }

    /* See why we ended and set some flags that might be useful */
    if (pEvent->eventEndTime != 0 && pEvent->eventEndTime != EHS_PLAYMANAGER_TIMER_UNDEFINED &&  currentt > pEvent->eventEndTime )   // expired?
    {
        if (pEvent->bRelativeEndTime == EHS_TRUE && pEvent->pParent)   // if the end time is relevant then we might not have actually expired so carry on looping
        {
            pEvent->pParent->bAllChildrenExpired = EHS_FALSE; // *** this one hasn't so set false  but we're not looping in this bit of code so prolly a waste of time... but future proof?
        }


    }
    else
    {

        if (pEvent->pParent)
        {
            pEvent->pParent->bAllChildrenExpired = EHS_FALSE; // *** this one hasn't so set false  but we're not looping in this bit of code so prolly a waste of time... but future proof?
        }
    }


    /* now check any periodic timer stuff */


// - we need to do all the list reset and notifications even without periodic stuff:	if ( pEvent->repeat != EHS_PLAYMANAGER_REPEAT_NONE) { /**** sort PERIODIC REPEATERS ONLY in the following****/

    /* TODO - the following check should be for parent repeats that are for parent repeats at a lower repeat rate for more functionality
            - if we do this we need a way for telling children to sopt repeating and end if end time of the parent has expired. (Perhaps we alrady do this, but prolly not?)
    */
    if (pEvent->parent_has_time)      /* Cases where we never repeat. Only if we are not looping and have periodically repeating parent at the same or faster rate */
    {
        if (_loop == EHS_TRUE )
        {
            if (pEvent->type != EHS_PLAYMANAGER_EVENT_OBJECT && pEvent->type != EHS_PLAYMANAGER_EVENT_UNKOWN )   /* for next lists */
            {
                _removeEvent = EHS_FALSE;
                _doNotifications = EHS_FALSE;
                ResetMemoryReader(pEvent, EHS_FALSE); // reset the list reader if we are repeating a list
            }
            else
            {
                _removeEvent = EHS_FALSE;
                _doNotifications = EHS_FALSE;
                pEvent->bCurrentPlayStarted = EHS_FALSE; // Need to reset this to allow it to run again
                pEvent->WaitingForObjectUniqueID = 0 ;
                pEvent->repeatTrigger = EHS_TRUE;
                //pEvent->bWaiting2EndList = EHS_FALSE;
            }
        }
        else      /* Parent has time and not a loop */
        {
            if (pEvent->type != EHS_PLAYMANAGER_EVENT_OBJECT && pEvent->type != EHS_PLAYMANAGER_EVENT_UNKOWN )   /* for next lists */
            {
                _repeat = EHS_FALSE;
                _removeEvent = EHS_TRUE;
                _doNotifications = EHS_TRUE;
            }
            else   /* for objects we'll wait for the end event ,but set  a timeout if it doesn't happen */
            {
                _repeat = EHS_FALSE;
                _removeEvent = EHS_TRUE;
                _doNotifications = EHS_TRUE;
            }
        }
    }
    else   /* Parent has no overridding  time, so cases we might repeat  */
    {
        _repeat = UpdateEventTime(pEvent,EHS_FALSE,currentt,EHS_TRUE); // test for repeat only X

        /*****   LOOP MIGHT BE POSSIBLE *****/
        if ( pEvent->repeatCount != EHS_PLAYMANAGER_LOOP_NONE )    // the case for looping and possibly also periodic repeat
        {
            if (_repeat == EHS_FALSE)    /* A repeat event has expired  or there's no periodic repeat to update*/
            {
                if (_loop == EHS_TRUE )
                {
                    if (pEvent->type != EHS_PLAYMANAGER_EVENT_OBJECT && pEvent->type != EHS_PLAYMANAGER_EVENT_UNKOWN )   /* for next lists */
                    {
                        _removeEvent = EHS_FALSE;
                        _doNotifications = EHS_FALSE;
                        //pEvent->type = EHS_PLAYMANAGER_EVENT_NEWLIST; // reset status of progress through list
                        ResetMemoryReader(pEvent, EHS_FALSE); // reset the list reader if we are repeating a list
                    }
                    else
                    {
                        _removeEvent = EHS_FALSE;
                        _doNotifications = EHS_FALSE;
                        pEvent->bCurrentPlayStarted = EHS_FALSE; // Need to reset this to allow it to run again
                        pEvent->WaitingForObjectUniqueID = 0 ;
                        pEvent->repeatTrigger = EHS_TRUE;
                        // pEvent->bWaiting2EndList = EHS_FALSE;
                    }
                }
                else     /* no more loops to do */
                {
                    _removeEvent = EHS_TRUE;
                    _doNotifications = EHS_TRUE;
                }
            }
            else    /* There is a repeat possible, but need to continue loops first */
            {
                if (_loop == EHS_TRUE )
                {
                    if (pEvent->type != EHS_PLAYMANAGER_EVENT_OBJECT && pEvent->type != EHS_PLAYMANAGER_EVENT_UNKOWN )
                    {
                        _removeEvent = EHS_FALSE;
                        _doNotifications = EHS_FALSE;
                        //pEvent->type = EHS_PLAYMANAGER_EVENT_NEWLIST; // reset status of progress through list
                        ResetMemoryReader(pEvent,EHS_FALSE); // reset the list reader if we are repeating a list
                    }
                    else
                    {
                        /* We don''t need to do anything for objects, just make sure we keep the item */
                        _removeEvent = EHS_FALSE;
                        _doNotifications = EHS_FALSE;
                        /* Possibly don't need the rest of this. */
                        pEvent->bCurrentPlayStarted = EHS_FALSE; // Need to reset this to allow it to run again
                        pEvent->WaitingForObjectUniqueID = 0 ;
                        pEvent->repeatTrigger = EHS_TRUE;

                        ResetMemoryReader(pEvent,EHS_FALSE);  //PMLD ____!!!
                    }
                }
                else       /* not a looping this one  but is periodic */
                {
                    if (checkEventEndTimeAndFlagEvent(pEvent,currentt,unupdatedEndTime) == EHS_TRUE)   // check if the first iteration has expired, but leave the next iteration to sort it out.
                    {
                        pEvent->bCurrentPlayStarted = EHS_FALSE; // Need to reset this to allow it to run again
                        pEvent->WaitingForObjectUniqueID = 0 ;
                        //if (pEvent->pParent) pEvent->pParent->bAllChildrenExpired = EHS_TRUE; - we don't set this because it is the default assumption until *** below
                        //pEvent->repeatTrigger = EHS_TRUE; // TODO - IS THIS NEEDED? NOT ACTUALLY USED! signal the tick function to run it again
                        //pEvent->bWaiting2EndList = EHS_FALSE;
                        _loop = EHS_FALSE;
                        _repeat = EHS_FALSE; // expired to so don't continue with any repeats or loops
                        if  (UpdateEventTime(pEvent,EHS_FALSE,currentt,EHS_FALSE) == EHS_TRUE)   // check if this is will repeat
                        {
                            _removeEvent = EHS_FALSE;
                            _doNotifications = EHS_TRUE;
                            ResetMemoryReader(pEvent,EHS_TRUE);
                        }
                        else
                        {
                            _removeEvent = EHS_TRUE;
                            _doNotifications = EHS_TRUE;
                        }
                    }
                    else     // the end time has not expired, but we may still repeat,:
                    {
                        pEvent->bCurrentPlayStarted = EHS_FALSE; // Need to reset this to allow it to run again
                        pEvent->WaitingForObjectUniqueID = 0 ;
                        if (pEvent->pParent)
                        {
                            pEvent->pParent->bAllChildrenExpired = EHS_FALSE; // *** this one hasn't so set false  but we're not looping in this bit of code so prolly a waste of time... but future proof?
                        }
                        //pEvent->repeatTrigger = EHS_TRUE; // TODO - IS THIS NEEDED? NOT ACTUALLY USED! signal the tick function to run it again
                        //pEvent->bWaiting2EndList = EHS_FALSE;
                        _loop = EHS_FALSE;
                        _repeat = EHS_FALSE; // expired to so don't continue with any repeats or loops
                        if (UpdateEventTime(pEvent,EHS_FALSE,currentt,EHS_FALSE)  ==  EHS_FALSE)   // todo makethe check the same asa above for consistency
                        {
                            _removeEvent = EHS_TRUE;
                            _doNotifications = EHS_TRUE;
                        }
                        else
                        {
                            ResetMemoryReader(pEvent,EHS_TRUE);  //always reset loops if we re periodic repeating
                            _removeEvent = EHS_FALSE;
                            _doNotifications = EHS_TRUE; // we still need to prgress any parent lists
                        }
                    }
                }
            }
        }
        else     /* END   LOOP MIGHT BE POSSIBLE  */
        {
            /* LOOPING NOT POSSIBLE but might repeat */

            if (endEvent)    // the fiollowing onlyhandles end events ... perhaps the whole function should todo TBC
            {
                if (_repeat == EHS_TRUE)    /* This will repeat -tested with UpdateEventTime() */
                {
                    if (pEvent->type != EHS_PLAYMANAGER_EVENT_OBJECT && pEvent->type != EHS_PLAYMANAGER_EVENT_UNKOWN )   /* for list types */
                    {

                        pEvent->bCurrentPlayStarted = EHS_FALSE; // Need to reset this to allow it to run again
                        pEvent->WaitingForObjectUniqueID = 0 ;
                        // pEvent->repeatTrigger = EHS_TRUE; // TODO - IS THIS NEEDED? NOT ACTUALLY USED! signal the tick function to run it again
                        //pEvent->bWaiting2EndList = EHS_FALSE;
                        //_loop = EHS_FALSE;
                        //_repeat = EHS_FALSE; // expired to so don't continue with any repeats or loops
                        _removeEvent = EHS_FALSE;
                        _doNotifications = EHS_TRUE;
                        ResetMemoryReader(pEvent, EHS_TRUE ) ;  //resetting loop counter just in case but shouldn't be necessary
                        /* _removeEvent has been set earlier */
                    }
                    else   // Object ends - we may as well do notiications e.g. if this is a repeat event in a list that doesn't repeat then we should notify the next in th list.
                    {
                        _removeEvent = EHS_FALSE;
                        _doNotifications = EHS_TRUE;
                    }
                    UpdateEventTime(pEvent,EHS_FALSE,currentt,EHS_FALSE); // always actually update these

                    //all these list resets need to  be done on  start as  it seems to di the first one whilefading out ohterwise... check where list reset is done...
                }
                else   /* This will not repeat remove and notify parents */
                {
                    _removeEvent = EHS_TRUE;
                    _doNotifications = EHS_TRUE; // if we have finished and expired then do notifications
                }
            }
            else    /* Seems we nee to handle some other events here just incase  - so assume these are repeaters?*/
            {
            }
        }
    } /* END OF ELSE For Parent has no overridding  time, so cases we might repeat  */
//	}
//	else {		/**** NON PERIODIC REPEATERS ****/
    /* We only want to do this here if we want to leave dead air until the expiry date. If so we need to change behaviour elsewhere too - removing:
    if (pEvent->repeat == XX || checkEventEndTimeAndFlagEvent(pEvent,currentt,unupdatedEndTime) == EHS_TRUE) { // check if the first iteration has expired111* */
    //----PMLD--UpdateEventTime(pEvent,endEvent,currentt,EHS_FALSE); // Don't need this @todo
    //----PMLD--_removeEvent = EHS_TRUE;
    //----PMLD--_doNotifications = EHS_TRUE;
    /*}
    else {
    	_removeEvent = EHS_FALSE;
    	_doNotifications = EHS_FALSE;
    }*/
//	}


    if (_doNotifications == EHS_TRUE)
    {
        NotifyAnObjectIsComplete(pPlayManager,pEvent->SendersObjectUniqueID); /*need to remove this ID from any waiters and let them move on*/
        NotifyParallelListIsComplete(pPlayManager,pEvent->SendersObjectUniqueID,currentt); // This will set a flag on the par event to complete.
    }

    if (_removeEvent == EHS_TRUE )
    {
        removeEvent( pEvent, pPlayManager);
        *ppEvent = NULL;
    }
    else
    {
    }
    EhsTPMutex_unlock(EhsTPMutex_playManager);
    return _removeEvent;
}


/*
 * Prefer not to have this behaviour at the moment - i.e. the new material should start playing instead of
 * current playlist in the time interval. However the current track should ideally not be interrupted.
 * This will probably be handled by the application whereby the New Playlist trigger is delayed untill the end of the current track (if one is playing)
 * @todo if there is an easy way of doing this delay within the function block code this would be ideal.
 *  @todo - this behaviour could be made a function block enabled feature switch (via an inout integer to select behaviour).
 *  i.e.
 *  (1) new content only occurs next day/week/month if already started (as commented code below ++++ ).
 *  (2) New content starts as soon as next track is complete if within schedule.
 *  (3) New content starts straight away, interrupting current track.
 *
 *  @todo this system expects there is only a single event occurring at a time
 */

/*
 * This function is called periodically and decides if an any events in the list are to be fired.
 * It also decides if the event is a repeat type and reposts it (via a call to update time) if it is.
 */


/*
 * @todo This function needs refactoring
 */

EHS_FB_RUN_FUNCTION( PlayManager_Tick)
{
    EhsPlayManagerType* pPlayManager; // we only use this to look up cached media
    time_t currentt;//, earliestPlayTime, earliestEndTime;
    EhsPlayManagerEvent *pCurrentEvent = NULL;
    EhsPlayManagerEvent *pNext = NULL;
    ehs_bool bUpdateTimeWindow = EHS_FALSE;
    ehs_bool bUpdateTimeWindowAll = EHS_FALSE;
    ehs_bool bEventListChanged = EHS_FALSE; // stop iterating if we change the list (dont just relie on the current event being deleted ...
    EhsPlayManagerSMILNodeActionEnum SMIL_chunk_property = EHS_PLAYMANAGER_SMIL_CHUNK_REMOVE; /* set a default to remove */
    /* Get the instance data for the playManager so that we have access to the event list */

    if (EhsTPMutex_playManager)
    {
        EhsTPMutex_lock(EhsTPMutex_playManager);
    }
    else
    {
        return;   /* We can be called after a tear down it seems when the mutexes have gone...*/
    }

    pPlayManager = (EhsPlayManagerType*) EHS_FB_RUN_CONTEXT;
    if (!pPlayManager)
    {
        EHSH_LOG_ERROR("Play Manager tick running when context has gone");
        EhsTPMutex_unlock(EhsTPMutex_playManager);
        return;
    }
    /* timeout the waiting to start flag */
    if (pPlayManager->pWaitingOnEvent > 0 ) pPlayManager->pWaitingOnEventCounter --;
    if (pPlayManager->pWaitingOnEventCounter < 1)
    {
        pPlayManager->pWaitingOnEvent = 0;
    }
    /* todo  consider looping from a few times to process event creations more quickly 
    - but be careful not to count down some of the counters for each loop thoug as we only want to do that per tick
       - so we might need a flag for "first_run" to modify counting to only happen once per tick */
    pCurrentEvent = pPlayManager->pEvent; /* this is the head of the linked list of events */
    if (    /* Are we ready to go??? */
        (pPlayManager->waitForAllDownloads == EHS_FALSE && pPlayManager->NewPlaylistStartGracePeriod == 0 )
        || (pPlayManager->triedAllDownloads == EHS_TRUE && pPlayManager->waitForAllDownloads == EHS_TRUE )
    )   /* wait for the grace period before doing anything after a new playlist is loaded */
    {
        if (pCurrentEvent != NULL)
        {
            currentt = time(NULL);

            if (currentt > pPlayManager->lastTickTime + 1)   // clock resolution is 1s
            {
                bUpdateTimeWindowAll = EHS_TRUE; // do a full check every second (or two to avoid any strange over updating the GUI)).
                pPlayManager->lastTickTime = currentt;
            }

#ifdef EHS_DEBUG_SMILPARSER
#endif
            while (pCurrentEvent != NULL)   /* Iterate through the active events  */
            {
                bEventListChanged = EHS_FALSE;
                if (bUpdateTimeWindowAll == EHS_TRUE) bUpdateTimeWindow = EHS_TRUE;
                else bUpdateTimeWindow = EHS_FALSE;
                bEventListChanged = EHS_FALSE;
                pNext = pCurrentEvent->pNext; // get a record of the next even before we might remove this event. Always break from loop if something might be deleted

#ifdef EHS_DEBUG_SMILPARSER
                EhsPrintDebugEventList(pPlayManager,pCurrentEvent);
#endif
                if (  /* Check Event status - has it not been start-fired, not waiting for start ack, or  waiting for child AND is it's time up?  */
                    ((!pCurrentEvent->bCurrentPlayStarted || pCurrentEvent->ListUniqueID > 0) && /*only fire an event if it is not started or a list event */
                     (pPlayManager->pWaitingOnEvent == 0 &&  /* If we are not waiting for an event start to be kicked off (and acked by the started ID port) then don't do anything.*/
                      (currentt >= pCurrentEvent->eventTime || pCurrentEvent->repeatTrigger == EHS_TRUE) && /* is it's "start expired "or retirggered & has the previous one been dealt with */
                      pCurrentEvent->WaitingForObjectUniqueID == 0) /* Have we got the ID we are looking for to move on? @todo - Is this a necessary condition for all of the following? What about parallel lists with a sequence running inside?  */
                    )
                )/* if the event has a pending finish to continue*/
                {
                    if (pCurrentEvent->repeatTrigger == EHS_TRUE )
                    {
                        pCurrentEvent->repeatTrigger = EHS_FALSE; //reset this always.
                    }

                    /********** ONCE ONLY CLEAN UP CODE: If new playlist check if the marked list needs deleting **********************/
                    /* If this is an object it means it really is the first event - not just a list is ready
                    * So we need to remove any old stuff if this is a new list   */
                    /* Only do event handling now when wait for all is set */
                    if (pPlayManager->NewSmil
                            && (pCurrentEvent->type == EHS_PLAYMANAGER_EVENT_OBJECT)
                            && (pCurrentEvent->marked != LIST_OLD)/* found the first new list event so need to remove all old (marked) ones//@todo we should use LIST_NEW here really... */
                       )
                    {
                        switch (pPlayManager->NewSmilBehaviour)   /*CLEAR_ALL=0, KEEP_EARLIERTHANNEW=1,KEEP_ALL=3 */
                        {
                        case KEEP_ALL: // do nothing
                            break;
                        case KEEP_EARLIERTHANNEW:
                            clearEventList(pPlayManager, EHS_TRUE); /* clear only the marked ones. */
                            break;
                        case CLEAR_ALL: /* leave it on the queue (as "late") so we will play the track as soon as the playing flag is dropped */
                            /* do nothing all previous events would already be cleared */
                            pPlayManager->earliestPlayTime=0;
                            pPlayManager->earliestEndTime=0;
                            break;
                        default: // raise the event even if playing.
                            break;
                        }
                        pPlayManager->NewSmil = EHS_FALSE;
                    }
                    /********** ONCE ONLY CLEAN UP CODE: Finished ******************************************************************/


                    /* Norml Tick Functions */
                    /* First see if we have a an event with duration (and not an end time) */
                    //if (pCurrentEvent->eventEndTime == 0 || pCurrentEvent->eventEndTime == EHS_PLAYMANAGER_TIMER_UNDEFINED ) {
                    // We should run this all the time as duration can be less than an end time
                    updateEndTimeWithDuration(pCurrentEvent,currentt);
                    //}
                    /* first update the endtime based on all parents duration */

                    /* Now find the events that are actually in time scope considering the end time too  */
                    // check that the event hasn't expired (using <= for ones that might fire once then expire immediately) and fire event
                    if (currentt <= pCurrentEvent->eventEndTime	|| (pCurrentEvent->eventEndTime == EHS_PLAYMANAGER_TIMER_UNDEFINED || pCurrentEvent->eventEndTime == 0) )
                    {
                        if (pCurrentEvent->bWaiting2EndList == EHS_FALSE && pCurrentEvent->WaitingForObjectUniqueID == 0  && pCurrentEvent->WaitingRefseqRead == EHS_FALSE)
                        {
                            /* Do the start stuff */
                            if (pCurrentEvent->type != EHS_PLAYMANAGER_EVENT_OBJECT && currentt == pCurrentEvent->eventEndTime)
                            {
                                // don't fire event if this is smultaneously expired and not an object (otherwise it keeps on firing ..)
                            }
                            else
                            {
                                fireEventandCleanUp(pFIdata,&pCurrentEvent, pPlayManager,currentt, EHS_FALSE); // only do this for objects -as we might have some with duration 0, which is still an internval in some cases
                            }
                            bEventListChanged = EHS_TRUE;
                            pPlayManager->earliestPlayTime=0;
                            pPlayManager->earliestEndTime=0;
                        }
                        else   // if here with undefined undetime then do something magical...
                        {
                            if (currentt >= pCurrentEvent->eventEndTime) pCurrentEvent->bWaiting2EndList = EHS_FALSE; // We're now ready to fire it next iteration
                        }
                    }
                    else
                    {
                        /* Deal with expired lists and events that have just been started.
                                   Need to remove or repeat action
                                   */
                        if (pCurrentEvent->eventEndTime != 0 && pCurrentEvent->eventEndTime != EHS_PLAYMANAGER_TIMER_UNDEFINED)
                        {
                            HandleEndEvent(pPlayManager,&pCurrentEvent,EHS_TRUE,currentt); // update start and stop times
                            bEventListChanged = EHS_TRUE;
                        }
                    }
                }
                else
                {
                    /* Deal with firing endTime expired events:
                    	       do we have an end event expired and we have fired the last media request (we don't care if it finished if we ignore misses in the media finished tests..)
                    	    */
                    if (pCurrentEvent->bCurrentPlayStarted && pCurrentEvent->eventEndTime != EHS_PLAYMANAGER_TIMER_UNDEFINED && currentt >= pCurrentEvent->eventEndTime && pPlayManager->pWaitingOnEvent == 0 )
                    {
                        fireEventandCleanUp(pFIdata, &pCurrentEvent, pPlayManager, currentt, EHS_TRUE);  // This should handle the above notification logic
                        bEventListChanged = EHS_TRUE;
                        pPlayManager->earliestPlayTime=0; // invalidate these after the UpdateEventTime() is called.
                        pPlayManager->earliestEndTime=0;

                    }
                    else     /* This is the default path - do nothing */
                    {
                        /* eventTime is still in the future so we can ignore it */
                    }
                }
                if(bUpdateTimeWindow) UpdateTimeWindow(pFIdata, pCurrentEvent,currentt); //Update the next play time window value - every so often..
                if (bEventListChanged == EHS_TRUE) break; // wait for the next tick and re-itrate toavoid any corner case linked list deletions.
                if (pCurrentEvent) pCurrentEvent = pNext;//If pCurrentEvent==NULL and we break out this loop and do another iteration from the start.
                /* Check if we are asked to leave */
                // - make less blocky??? no need we are a threadEhsSleep(EHS_TIME_us(5000));
                if (*bNewSodlFlagRef) break; /* Get out nicel if we are asked */

            } /* end while */
        }
    }
    else     /* We're in the grace period so count down. Do this uselessly even if we are in wait for all mode */
    {
        if (pPlayManager->NewPlaylistStartGracePeriod > 0) pPlayManager->NewPlaylistStartGracePeriod--; // don't negative - this is a flag for waiting for ever for downloads.
    }
    if (pPlayManager->changedStart || pPlayManager->changedEnd) AssertTimeWindowPorts(pFIdata,pPlayManager);
    EhsTPMutex_unlock(EhsTPMutex_playManager);
    EHS_TRACE_FUNCTION(EHS_FB_RUN_NAME(PlayManager_Tick));
}

/*
 * @brief This function is for iterating through the playlist and producing URLS for each content file.
 *
 * Once a function has been retrieved the got event can asserted by downloader to register that the file has been
 * received. The received got registers the file in a list so that at the time of play it can be identified if the
 * file should be played from the local copy. The got event does NOT TRIGGER THE NEXT URL TO BE DOWNLOADED - Typically the got event output
 * trigger will be connected to trigger the next URL trigger.
 */
EHS_FB_RUN_FUNCTION(PlayManager_Next_URL)
{
    EhsPlayManagerType* pPlayManager = (EhsPlayManagerType*) EHS_FB_RUN_CONTEXT;
    //EHSH_LOG_ERROR("---->PlayManager_Next_URL()\n");
    EhsTPMutex_lock(EhsTPMutex_playManager);
    //X//todo Avoiding overlapping downloads: if there is a new URL thread started we need to detect this and send a semaphore to it that it can now start
    // and then bail from this function with no finish.
    if (pPlayManager->srcFileReader != NULL)
    {
        // ehs_uint32 getPortNumber,ehs_uint32 urlPortNumber, ehs_uint32 pathPortNumber)
        //EHSH_LOG_ERROR("---->Reading sources 22222 ....\n");
        readSrcs(pPlayManager, pFIdata, 2, 0, 1,3); // get the next event data @todo use macros here for the port numbers here
    }
    EhsTPMutex_unlock(EhsTPMutex_playManager);
    EHS_FB_FINISH(1);
}


/** @brief Register all the media that is found in the pMediaList and identify the play path (e.g. if local).
 * This does not contribute to the tried all flag.
 *
 * */
EHS_FB_RUN_FUNCTION(PlayManager_Got)
{
    EhsTPMutex_lock(EhsTPMutex_playManager);
    EhsPlayManagerType* pPlayManager = (EhsPlayManagerType*) EHS_FB_RUN_CONTEXT;
    EhsPlayManagerMediaLocation* pMediaLocation = NULL;
    EhsPlayManagerMediaLocation* pCurrent = NULL;
    //Read input values
    ehs_char inURL[EHS_STRING_LENGTH_MAX]; //TODO:STRINGLENGTH!
    inURL[0] = '\0';
    ehs_char inLocalFile[EHS_STRING_LENGTH_MAX]; //TODO:STRINGLENGTH!
    inLocalFile[0] = '\0';

    if (EHS_FB_IN_CONNECTED(EHS_FB_PLAYMANAGER_GOT_URL))
    {
        EhsStrcpy(inURL, EHS_FB_IN_S(EHS_FB_PLAYMANAGER_GOT_URL));
    }
    if (EHS_FB_IN_CONNECTED(EHS_FB_PLAYMANAGER_GOT_LOCAL))
    {
        EhsStrcpy(inLocalFile, EHS_FB_IN_S(EHS_FB_PLAYMANAGER_GOT_LOCAL));
    }
    EhsStrcpy(EHS_FB_OUT_S(0), inURL); //@todo this could be removed to save some space
    EhsStrcpy(EHS_FB_OUT_S(1), inLocalFile); //@todo this could be removed to save some space

    //EHSH_LOG_ERROR("GOT %s",inURL);
    //Create a new media location object
    pMediaLocation = (EhsPlayManagerMediaLocation*) EhsHMem_tempAlloc(sizeof(EhsPlayManagerMediaLocation));
    /* need to initialise the linked list to pointers to null */
    if (pMediaLocation != NULL)
    {
        if (EHS_FB_IN_CONNECTED(EHS_FB_PLAYMANAGER_GOT_ID)) pMediaLocation->id=EHS_FB_IN_I(EHS_FB_PLAYMANAGER_GOT_ID);
        /* Initialise the media struct*/
        EhsStrncpy(pMediaLocation->url, inURL, EHS_STRING_LENGTH_MAX-1);
        EhsStrncpy(pMediaLocation->localFile, inLocalFile, EHS_STRING_LENGTH_MAX-1);
        pMediaLocation->pNext = NULL;
        pMediaLocation->marked=LIST_DEFAULT;
        /* Add an entry to our list of locations - if list is empty just put it on as the first item */
        if (pPlayManager->pMediaList == NULL)
        {
            pPlayManager->pMediaList = pMediaLocation;
        }
        else
        {
            //List is not empty so run over it and find the new place for it
            pCurrent = pPlayManager->pMediaList;
            //Positioning logic is that it should go at the end of the list, we can thus assume that the list will be time ordered due to assuming we are only dealing with time ordered playlists at this phase
            while (pCurrent->pNext != NULL)
            {
                pCurrent = pCurrent->pNext;
            }
            //We have reached the end of the event list. pCurrentEvent should point to the last entry in the list
            pCurrent->pNext = pMediaLocation;
        }
    }
    EhsTPMutex_unlock(EhsTPMutex_playManager);
    EHS_FB_FINISH(1);
}

EHS_FB_RUN_FUNCTION(PlayManager_Set_Paths)
{
    EhsPlayManagerType* pPlayManager = (EhsPlayManagerType*) EHS_FB_RUN_CONTEXT;
    EhsPlayManagerNewSmilBehavType NewSmilBehavTemp;
    EhsTPMutex_lock(EhsTPMutex_playManager);
    EhsTPMutex_lock(EhsTPMutex_fbIO);
    if (EHS_FB_IN_CONNECTED(EHS_FB_PLAYMANAGER_SETPATHS_DL_URL))
    {
        EhsStrcpy(pPlayManager->downloadURL, EHS_FB_IN_S(EHS_FB_PLAYMANAGER_SETPATHS_DL_URL));
        if (pPlayManager->downloadURL[EhsStrlen(pPlayManager->downloadURL)-1]!='/' || pPlayManager->downloadURL[EhsStrlen(pPlayManager->downloadURL)-1]!='\\' )
        {
            EhsStrcat(pPlayManager->downloadURL,"/");
        }
        EhsStrcpy(EHS_FB_OUT_S(EHS_FB_PLAYMANAGER_SETPATHS_DL_URL), pPlayManager->downloadURL);
    }
    if (EHS_FB_IN_CONNECTED(EHS_FB_PLAYMANAGER_SETPATHS_DL_LOCAL))
    {
        EhsStrcpy(pPlayManager->downloadPath, EHS_FB_IN_S(EHS_FB_PLAYMANAGER_SETPATHS_DL_LOCAL));
        EhsStrcpy(EHS_FB_OUT_S(EHS_FB_PLAYMANAGER_SETPATHS_DL_LOCAL), EHS_FB_IN_S(EHS_FB_PLAYMANAGER_SETPATHS_DL_LOCAL));
    }
    if (EHS_FB_IN_CONNECTED(EHS_FB_PLAYMANAGER_SETPATHS_START_MODE))
    {
        NewSmilBehavTemp = EHS_FB_IN_I(EHS_FB_PLAYMANAGER_SETPATHS_START_MODE);
        if ((NewSmilBehavTemp == CLEAR_ALL) || (NewSmilBehavTemp == KEEP_EARLIERTHANNEW) || (NewSmilBehavTemp == KEEP_ALL) )
        {
            pPlayManager->NewSmilBehaviour = NewSmilBehavTemp;
            EHSH_LOG_INFO("Setting Playlist Start Mode=%d\n",pPlayManager->NewSmilBehaviour);
        } //else don't update.
    }
    EhsTPMutex_unlock(EhsTPMutex_fbIO);
    EhsTPMutex_unlock(EhsTPMutex_playManager);
    EHS_FB_FINISH(1);
}
