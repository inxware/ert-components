/* file.c
 *
 * functions for use in the EHS system providing POSIX compliant file access.
 *
 * For definition of arguments in Ncapsa functions (Identify_, Init_ and Run_)
 * please see types.h.
 *
 * Lucid project stage one - NcapsaLtd - May 2005 - ADS
*/

#include <math.h>
#include "target.h"
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

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(runtimeinfo);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(runtimeinfo);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(runtimeinfo_updatestatic);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(runtimeinfo_updatedynamic);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(runtimeinfo_request_pairing_id);

EHS_FB_FUNCTIONS_START(runtimeinfo)
EHS_FB_FUNCTION_ENTRY("getstat", runtimeinfo_updatestatic)
EHS_FB_FUNCTION_ENTRY("getdyn", runtimeinfo_updatedynamic)
EHS_FB_FUNCTION_ENTRY("reqpairid", runtimeinfo_request_pairing_id)
EHS_FB_FUNCTIONS_END

//define the port mappings
#define EHS_RTINFO_GETSTAT_PORT_FINISH 1

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
#define EHS_RTINFO_GETDYN_CPU_USAGE_OUT 3
#define EHS_RTINFO_GETDYN_PAIR_ID_OUT 4
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
	//EhsHMetaUpdateStatic(); /* Todo - this currently runs the same thing twice
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

/* Write all the static data */
// don't need to update static info or mutex.
	//EhsHMetaUpdateStatic();
	//EhsHAppMetaRequestPairedOrganisation(); /* request Devman to retrieve paring information */
	EhsStrcpy(EHS_FB_OUT_S(EHS_RTINFO_GETSTAT_DeviceID_OUT),EhsHMetaGetHWID());
	EHS_FB_OUT_I(EHS_RTINFO_GETSTAT_RAM_KB_OUT)=EhsHMetaGetRAMTotal();
	EHS_FB_OUT_I(EHS_RTINFO_GETSTAT_Storage_KB_OUT)=EhsHMetaGetStorTotal();
	EhsStrcpy(EHS_FB_OUT_S(EHS_RTINFO_GETSTAT_EHS_Version_OUT),EhsHMetaGetEHSVersion());
	EhsStrcpy(EHS_FB_OUT_S(EHS_RTINFO_GETSTAT_ModuleList_OUT),"N/A");
	EhsStrcpy(EHS_FB_OUT_S(EHS_RTINFO_GETSTAT_InstallDir_OUT),EhsHMetaGetInstPath());
	EHS_FB_FINISH(EHS_RTINFO_GETSTAT_PORT_FINISH);
}


/* Function to request the pairing for the device
 * This doesn't assert a finished so breaks the law!!
 * */

EHS_FB_RUN_FUNCTION(runtimeinfo_request_pairing_id)
{
	EhsHAppMetaRequestPairedOrganisation(); /* post a request to Devman to get this for a later call */
	//EHSH_LOG_INFO("Requesting current pairing ID");
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
	//printf("EhsHMetaGetIPAddr[%x]=%s\n",EhsHMetaGetIPAddr(),EhsHMetaGetIPAddr());
	EhsStrcpy(EHS_FB_OUT_S(EHS_RTINFO_GETDYN_IPAddr_OUT),EhsHMetaGetIPAddr());
	EHS_FB_OUT_I(EHS_RTINFO_GETDYN_RAMAvail_KB_OUT)=EhsHMetaGetRAMAvail();
	EHS_FB_OUT_I(EHS_RTINFO_GETDYN_StorAvail_KB_OUT)=EhsHMetaGetStorAvail();
	EHS_FB_OUT_I(EHS_RTINFO_GETDYN_CPU_USAGE_OUT)=EhsHMetaGetCPUUsage();
//must set this to -2 tp indicate we are waiting for a -1 or something else


	/* 0: no pairing data updates, 1 pairing data valid, 2 pairing data pending, 3 paring data invalid*/

	switch (EhsHSysGetPairedOrganisationStatus()){
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



