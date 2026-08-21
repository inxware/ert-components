/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file app_data.h
 * Declarations of global application data.
 *
 * This is needed by all FB components to reference the FID and data tables
 * TODO - This shouldn't be includeded directly by function blocks? We have macros that do this that probably should include this file?
 *
 * @author: inx limited
 *
 */

#ifndef EHS_APP_DATA_H
#define EHS_APP_DATA_H

#include "globals.h"
//#include "group_table.h"
#include "trigger_table.h"
#include "fidt.h"

#define EHS_STRING_ENDBUFFER 1

/*********************************************************************************************/
/* Data transfer table table */

/**
 * Type definition of the structure used to hold data passed between function blocks
 */
typedef struct
{
    ehs_char* xDummy; /**< Just points to a chunk of memory (optional if e have trusted clients) //TODO2024 - volatile here conflicts with pointer..
										containing the largest of the possible
										data types. this is for OUTPUTS*/
    volatile ehs_sint64 xDummyIn[1]; /* Seperate dummy for inputs to avoid random data read into FBs - this is 64 bit to allow for ou worst case lvalue size of a float (todo may need to support larget in the future (wasa char...)*/
    EhsDataflowIntType* pnData;
    ehs_bool* pbMonitorInt;
    ehs_uint32 nNumInts; /* This is strictly speaking the highest index -1 */
    EhsDataflowBoolType* pbData;
    ehs_bool* pbMonitorBool;
    ehs_uint32 nNumBools; /* This is strictly speaking the highest index -1 */
    EhsDataflowStringType* pszData; /**< rows each containing a string of string_length_max characters */
    ehs_bool* pbMonitorString;
    ehs_uint32 nNumStrings; /* This is strictly speaking the highest index -1 */
#ifdef EHS_TARGET_FP_SUPPORT
    EhsDataflowFloatType* pfData;
    ehs_bool* pbMonitorFloat;
    ehs_uint32 nNumFloats; /* This is strictly speaking the highest index -1 */
#endif /* EHS_TARGET_FP_SUPPORT */
    EhsDataflowUserType* pusrData; /**< rows each containing a user data pointer */
    ehs_bool* pbMonitorUser;
    ehs_uint32 nNumUsers; /* This is strictly speaking the highest index -1 */
    ehs_uint32* pszDataSizes; /* List data buffer size for each entry of pszData */
} EhsDataConnectionTableType;

/**
 * Flag to indicate loading of new sodl so that threads MUST be terminated
 */
EHS_EXTERN ehs_bool* bNewSodlFlagRef;

/**
 * Flag to indicate the runtime tables are ready - no port activity untill this is true
 */
EHS_EXTERN ehs_bool* bRuntablesReadyRef;

/* Lower level function for application environment */

/*
 * @todo structures that are used at run time should be an array as eventually multiple applications will run on EHS.
 *
 */
/* structure fr currently running  app meta data */
typedef struct
{
    ehs_uint32 uSODLPayloadHash;
    ehs_uint32 nSODLBuildNumber;
    ehs_char zVersion[EHS_STRING_LENGTH_MAX];
    ehs_char zSODLdate[EHS_STRING_LENGTH_MAX];
    ehs_char zApplicationName[EHS_STRING_LENGTH_MAX];
} EhsApplicationMetaDataType;

/* Destructor Function List Element */

typedef struct EhsDestroyObjElementStruct
{
    EhsDestroyFuncType funcPtr;
    EhsFunctionInstanceDataType* pFuncInst;
    struct EhsDestroyObjElementStruct * next;
} EhsDestroyObjElementType;


/**
 * Declares the global data table
 */
EHS_GLOBAL EhsDataConnectionTableType EhsDataConnectionTable;

/**
 * This function is called prior to loading a new SODL file. It allocates memory for new
 * connections and resets all connection values.
 * @param[in] nInt Number of integer connections that are required
 * @param[in] nBool Number of boolean connections that are required
 * @param[in] nString Number of string connections that are required
 * @param[in] nFloat Number of float connections taht are required (ignored if EHS_TARGET_FP_SUPPORT isn't defined)
 * @param[in] nUser Number of user data connections that are required
 * @param[in] pStrMemSizeList Linked-List containing string connections buffers sizes
 * @param[in] nStrMemSizeListLength Size of pStrMemSizeList
 * @return true if memory is successfully allocated for these connections.
 */
EHS_GLOBAL ehs_bool EhsDataConnectionTable_init(ehs_uint32 nInt, ehs_uint32 nBool, ehs_uint32 nString, ehs_uint32 nFloat, ehs_uint32 nUser, ehs_uint32* pStrMemSizeList, ehs_uint32 nStrMemSizeListLength);

/**
 * Reset the application data to make the application ready to start running from its initial state.
 * To restart the application, each of the groups need to be started, an initial event needs to be
 * added to the event queue
 * @todo Add code to initialise the data areas for each function block instance.
 */
EHS_GLOBAL void EhsApplicationReset();

/**
 * This function is called prior to loading a new SODL file. It frees all memory
 * used by applications, and resets application-specific data.
 */
EHS_GLOBAL void EhsApplicationInit();

/* This does what exactly @todo */
EHS_GLOBAL void Ehs_AppStart();

/* This sets the EHS instance info with the name of the currently live application */
EHS_GLOBAL ehs_bool EhsAppInitLiveAppDir();

/* Create the downloaded OK token in the download directory (full path must be provided)
 *
 * Run me at the end of the download
 * */
EHS_GLOBAL ehs_bool EhsAppSetDownloadOKToken(ehs_char * canonicalName);

/*
 * Set the flag for any (Legacy) Functions blocks to close down their threads.
 * Wait for the the thread counter to reach zero, then proceed (or timeout).
 * @todo this should be removed when all destructors are implemented.
 * Finally do things properly and call each function block's destructor.
 */

EHS_GLOBAL void EhsCloseAppThreadsAndWaitForTearDown();

/* Function to add a destructor function to the Applicatio Tear down */
ehs_bool EhsAddToDestructorList(EhsDestroyFuncType func,EhsFunctionInstanceDataType* pFuncInst);

/**
 * Clear the monitoring flags. This function should be called upon entering debug mode
 */
EHS_GLOBAL void EhsDataConnectionTable_resetMonitorFlags(void);

/* Bounded access to string data connections (EhsDataString_*) is declared in
 * ehs_fb_types.h, alongside the EHS_FB_OUT_S_* macros that wrap it. */

/**
 * Returns EHS_TRUE if a new application has been loaded and started since the
 * last call to this function, then clears the flag so subsequent calls return
 * EHS_FALSE until the next application load.
 *
 * Intended for use by ert-component targets that need to poll for a fresh
 * application load event (e.g. to re-initialise component state after a
 * SODL reload).
 *
 * @return EHS_TRUE if a new application was loaded since last checked, EHS_FALSE otherwise.
 */
EHS_GLOBAL ehs_bool EhsGetAndClearNewAppLoaded(void);


//These functions should be used to enter and exit component dynamic threads - declared in public header ehs_fb_types.h
/* THese are declared in ehs_fb_types */
//void Ehs_FB_ThreadStarted();
//void Ehs_FB_ThreadComplete();

#endif /* EHS_APP_DATA_H */
