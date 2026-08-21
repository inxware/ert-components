/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/
/* file.c
 *
 * functions for use in the EHS system providing POSIX compliant file access.
 *
 * For definition of arguments in Ncapsa functions (Identify_, Init_ and Run_)
 * please see types.h.
 *
*/

#include <math.h>

#include "globals.h"
#include "rtinfo.h"
#include "setCompletes.h"

#include "hal-api.h" //needed for  version info

#define CR '\r'
#define LF '\n'


/**
 * file object type structure...
 */
typedef struct
{

} rtinfoObj;


/******************************************************************************/
/* Define FILE_ReadOnly_Bool function block */
//@todo These function blocks should include a method set the filename.

EHS_FB_IDENTIFY_FUNCTION(runtimeinfo);
EHS_FB_INIT_FUNCTION(runtimeinfo);
EHS_FB_RUN_FUNCTION(runtimeinfo_updatestatic);
EHS_FB_RUN_FUNCTION(runtimeinfo_updatedynamic);
EHS_FB_RUN_FUNCTION(runtimeinfo_request_pairing_id);

EHS_FB_FUNCTIONS_START(runtimeinfo)

EHS_FB_FUNCTION_ENTRY("getstat", 0x01, runtimeinfo_updatestatic)

EHS_FB_FUNCTION_ENTRY("getdyn", 0x02, runtimeinfo_updatedynamic)

EHS_FB_FUNCTION_ENTRY("reqpairid", 0x03, runtimeinfo_request_pairing_id)
EHS_FB_FUNCTIONS_END

//define the port mappings (not using API2 so these start at 0)
#define EHS_RTINFO_GETSTAT_PORT_FINISH 1
#define EHS_RTINFO_GETSTAT_MiscAppName_IN 0

#define EHS_RTINFO_GETSTAT_DeviceID_OUT 0
#define EHS_RTINFO_GETSTAT_RAM_KB_OUT 1
#define EHS_RTINFO_GETSTAT_Storage_KB_OUT 2
#define EHS_RTINFO_GETSTAT_EHS_Version_OUT 3
#define EHS_RTINFO_GETSTAT_ModuleList_OUT 4
#define EHS_RTINFO_GETSTAT_InstallDir_OUT 5

#define EHS_RTINFO_GETDYN_PORT_FINISH 1
#define EHS_RTINFO_GETDYN_IPAddr_OUT 0
#define EHS_RTINFO_GETDYN_RAMAvail_KB_OUT 1
#define EHS_RTINFO_GETDYN_StorAvail_KB_OUT 2
#define EHS_RTINFO_GETDYN_CPU_USAGE_OUT 3 // cpu for eRT process
#define EHS_RTINFO_GETDYN_PAIR_ID_OUT 4
#define EHS_RTINFO_GETDYN_RAM_USAGE_OUT 5 // ram for eRT process
#define EHS_RTINFO_GETDYN_CPU_MISC_USAGE_OUT 6 // cpu for misc user-specified process
#define EHS_RTINFO_GETDYN_RAM_MISC_USAGE_OUT 7 // RAM for misc user-specified process
#define EHS_RTINFO_GETDYN_PAIR_FINISH 2

//@todo Add a parameter that adds a minumum time interval that must have elapsed for the function to return a new value (for cpu % time interval).

/**
 * Identify the function block. Determine the memory required.
 *
 * This function provides access to:
 *  EHS_FB_IDENTIFY_PARAMETERS - string containing parameter text
 *  EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this
 *   function block's context
 *
 */
EHS_FB_IDENTIFY_FUNCTION(runtimeinfo)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(rtinfoObj);
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(runtimeinfo)
{
    /* parse parameter string */
    EhsHMetaUpdateStatic(); /* This is an expensive system stat config process */
    EhsHMetaUpdateDynamic();
    //EhsHAppMetaRequestPairedOrganisation(); //post a request forgetting the Pairing ID.
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(runtimeinfo_updatestatic)
{
    //rtinfoObj* pRtinfoObj = (rtinfoObj*)EHS_FB_RUN_CONTEXT;

    /* Write all the static data to buffers and config*/
    if (EHS_FB_IN_CONNECTED(EHS_RTINFO_GETSTAT_MiscAppName_IN)) {
        EhsStrcpy(EhsHMetaGetMiscAppNamePtr(),EHS_FB_IN_S(EHS_RTINFO_GETSTAT_MiscAppName_IN));
        EhsHMetaUpdateStatic();
    }
    //EhsHAppMetaRequestPairedOrganisation(); /* request Devman to retrieve paring information */
    if (EHS_FB_OUT_CONNECTED(EHS_RTINFO_GETSTAT_DeviceID_OUT))
        EHS_FB_OUT_S_SET(EHS_RTINFO_GETSTAT_DeviceID_OUT, EhsHMetaGetHWID());
    if (EHS_FB_OUT_CONNECTED(EHS_RTINFO_GETSTAT_RAM_KB_OUT))
        EHS_FB_OUT_I(EHS_RTINFO_GETSTAT_RAM_KB_OUT)=EhsHMetaGetRAMTotal();
    if (EHS_FB_OUT_CONNECTED(EHS_RTINFO_GETSTAT_Storage_KB_OUT))
        EHS_FB_OUT_I(EHS_RTINFO_GETSTAT_Storage_KB_OUT)=EhsHMetaGetStorTotal();
    if (EHS_FB_OUT_CONNECTED(EHS_RTINFO_GETSTAT_EHS_Version_OUT)) 
        EHS_FB_OUT_S_SET(EHS_RTINFO_GETSTAT_EHS_Version_OUT, EhsHMetaGetEHSVersion());
    if (EHS_FB_OUT_CONNECTED(EHS_RTINFO_GETSTAT_ModuleList_OUT))
        EHS_FB_OUT_S_SET(EHS_RTINFO_GETSTAT_ModuleList_OUT, "N/A");
    if (EHS_FB_OUT_CONNECTED(EHS_RTINFO_GETSTAT_InstallDir_OUT))
        EHS_FB_OUT_S_SET(EHS_RTINFO_GETSTAT_InstallDir_OUT, EhsHMetaGetInstPath());
    EHS_FB_FINISH(EHS_RTINFO_GETSTAT_PORT_FINISH);
}

/* Function to request the pairing for the device
 * This doesn't assert a finished or error, which is not ideal, but a big issue particularly either */
EHS_FB_RUN_FUNCTION(runtimeinfo_request_pairing_id)
{
    EhsHAppMetaRequestPairedOrganisation(); /* post a request to Devman to get this for a later call */
    EHSH_LOG_INFO("Requesting current pairing ID");
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(runtimeinfo_updatedynamic)
{
    //rtinfoObj* pRtinfoObj = (rtinfoObj*)EHS_FB_RUN_CONTEXT;
    ehs_sint32 PairingID=EhsHSysGetPairedOrganisation(); /* get the current known ID */
    EhsHMetaUpdateDynamic();
#ifdef EHS_GNU
    if (EhsTDFiles_countDirFiles ( "/proc/self/fd" ) > 100 )
        EHSH_LOG_ERROR("More than 100 file descriptoers are being used by EHS!!!")
#endif
    /* and the dynamic data */
    /* call the os-specfic call e.g. EHSTos_GetFileInfo(&pRtinfoObj); */
    if (EHS_FB_OUT_CONNECTED(EHS_RTINFO_GETDYN_IPAddr_OUT))
        EHS_FB_OUT_S_SET(EHS_RTINFO_GETDYN_IPAddr_OUT, EhsHMetaGetIPAddr());
    EHS_FB_OUT_I(EHS_RTINFO_GETDYN_RAMAvail_KB_OUT)=EhsHMetaGetRAMAvail();             /* Get total spare RAM */
    EHS_FB_OUT_I(EHS_RTINFO_GETDYN_StorAvail_KB_OUT)=EhsHMetaGetStorAvail();           /* File storage available for user data */
    EHS_FB_OUT_I(EHS_RTINFO_GETDYN_CPU_USAGE_OUT)=EhsHMetaGetCPUUsage();               /* CPU for eRT process */
    EHS_FB_OUT_I(EHS_RTINFO_GETDYN_RAM_USAGE_OUT)=EhsHMetaGetRAMUsedEHS_kB();          /* ram for eRT process */
    EHS_FB_OUT_I(EHS_RTINFO_GETDYN_CPU_MISC_USAGE_OUT)=EhsHMetaGetMiscAppCPUUsage();   /* cpu for misc user-specified process */
    EHS_FB_OUT_I(EHS_RTINFO_GETDYN_RAM_MISC_USAGE_OUT)=EhsHMetaGetMiscAppRAMUsed_kB(); /* RAM for misc user-specified process */
/* Handling if we have received a Devman organisation pairing code                                 */
/* Value should be set to 2: indicating we are waiting for a 1 or 3:                               */
/* 0: no pairing data updates, 1 pairing data valid, 2 pairing data pending, 3 paring data invalid */

    switch (EhsHSysGetPairedOrganisationStatus())
    {
    case  1 : /* always assert if an update was pending */
        EHS_FB_OUT_I(EHS_RTINFO_GETDYN_PAIR_ID_OUT)=(ehs_sint32)PairingID; /* return -1 if not yet updated */
        EHS_FB_FINISH(EHS_RTINFO_GETDYN_PAIR_FINISH);
        break;
    case  2 : /* pairing data still pending - do nothing */
        break;
    case  3 : //write out -1
        EHS_FB_OUT_I(EHS_RTINFO_GETDYN_PAIR_ID_OUT)=-1; /* return -1 if not yet updated */
        EHS_FB_FINISH(EHS_RTINFO_GETDYN_PAIR_FINISH);
        break;
    default :
        break;
    }
    EHS_FB_FINISH(EHS_RTINFO_GETDYN_PORT_FINISH);
}



