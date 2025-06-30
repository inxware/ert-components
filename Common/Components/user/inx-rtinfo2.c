//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-rtinfo2.h"
//ICB HEADER MACRO END -- DO NOT ALTER

#include "hal-api.h" //needed for  version info

//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_RuntimeInfo2_state
{
} inx_RuntimeInfo2_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(RuntimeInfo2)
EHS_FB_FUNCTION_ENTRY("getstat", 0x01, RuntimeInfo2_getstat)
EHS_FB_FUNCTION_ENTRY("getdyn", 0x02, RuntimeInfo2_getdyn)
EHS_FB_FUNCTION_ENTRY("reqpairid", 0x03, RuntimeInfo2_reqpairid)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_RuntimeInfo2_ARG_getstat_GetStatic 1
#define INX_RuntimeInfo2_ARG_getstat_Device_ID 1
#define INX_RuntimeInfo2_ARG_getstat_RAM__KB__ 2
#define INX_RuntimeInfo2_ARG_getstat_Stor__KB_ 3
#define INX_RuntimeInfo2_ARG_getstat_EHS_version 4
#define INX_RuntimeInfo2_ARG_getstat_ModuleList 5
#define INX_RuntimeInfo2_ARG_getstat_Install_Dir 6
#define INX_RuntimeInfo2_ARG_getstat_ 1
#define INX_RuntimeInfo2_ARG_getdyn_IP_Addr 1
#define INX_RuntimeInfo2_ARG_getdyn_RAM_Avail__KB_ 2
#define INX_RuntimeInfo2_ARG_getdyn_Stor_Avail__KB_ 3
#define INX_RuntimeInfo2_ARG_getdyn_CPU_eRT 4
#define INX_RuntimeInfo2_ARG_getdyn_RAM_eRT 5
#define INX_RuntimeInfo2_ARG_getdyn_CPU 6
#define INX_RuntimeInfo2_ARG_getdyn_RAM_Misc 7
#define INX_RuntimeInfo2_ARG_getdyn_TEMP_CPU 8
#define INX_RuntimeInfo2_ARG_getdyn_TEMP_OP 9
#define INX_RuntimeInfo2_ARG_getdyn_Pair_ID 10
#define INX_RuntimeInfo2_ARG_getdyn_ 1
#define INX_RuntimeInfo2_ARG_getdyn_Pair_ID 2
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER
//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
/* Parameters */
/* Create some macros for the default parameters */
//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(RuntimeInfo2)
{
/* Uncomment the following if you need to parse the parameters to calculate memory required */
/*
	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,""); */
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_RuntimeInfo2_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(RuntimeInfo2)
{
	ehs_bool bRet = EHS_TRUE; /* assume success */
	//this is the reference to the object data for this instance of the function block
	//inx_RuntimeInfo2_state_type* inx_RuntimeInfo2_state = (inx_RuntimeInfo2_state_type*)EHS_FB_INIT_CONTEXT;
	/* read the initialisation parameters */
	//EhsSscanf(EHS_FB_INIT_PARAMETERS,"");

	EhsHMetaUpdateStatic(); /* This is an expensive system stat config process */
    EhsHMetaUpdateDynamic();

	/* Add any further intialisation code here */
	return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(RuntimeInfo2)
{
	inx_RuntimeInfo2_state_type *inx_RuntimeInfo2_state = (inx_RuntimeInfo2_state_type*)EHS_FB_DESTROY_CONTEXT;
	//Your code below here
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION getstat MACRO START -- DO NOT ALTER
/**
 * Definition of RuntimeInfo2_getstat.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(RuntimeInfo2_getstat)
{
	inx_RuntimeInfo2_state_type* inx_RuntimeInfo2_state = (inx_RuntimeInfo2_state_type*)EHS_FB_RUN_CONTEXT;

	// Your code here
	if (EHS_FB_IN_CONNECTED_API2(INX_RuntimeInfo2_ARG_getstat_GetStatic)){
		EhsStrcpy(EhsHMetaGetMiscAppNamePtr(),EHS_FB_IN_S_API2(INX_RuntimeInfo2_ARG_getstat_GetStatic)); // misc app name
        EhsHMetaUpdateStatic();
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_RuntimeInfo2_ARG_getstat_Device_ID))
		EhsStrcpy(EHS_FB_OUT_S_API2(INX_RuntimeInfo2_ARG_getstat_Device_ID),EhsHMetaGetHWID());
		 
	if (EHS_FB_OUT_CONNECTED_API2(INX_RuntimeInfo2_ARG_getstat_RAM__KB__))
		EHS_FB_OUT_I_API2(INX_RuntimeInfo2_ARG_getstat_RAM__KB__)=EhsHMetaGetRAMTotal();

	if (EHS_FB_OUT_CONNECTED_API2(INX_RuntimeInfo2_ARG_getstat_Stor__KB_))
		EHS_FB_OUT_I_API2(INX_RuntimeInfo2_ARG_getstat_Stor__KB_)=EhsHMetaGetStorTotal();

	if (EHS_FB_OUT_CONNECTED_API2(INX_RuntimeInfo2_ARG_getstat_EHS_version))
		EhsStrcpy(EHS_FB_OUT_S_API2(INX_RuntimeInfo2_ARG_getstat_EHS_version),EhsHMetaGetEHSVersion());

	if (EHS_FB_OUT_CONNECTED_API2(INX_RuntimeInfo2_ARG_getstat_ModuleList))
		EhsStrcpy(EHS_FB_OUT_S_API2(INX_RuntimeInfo2_ARG_getstat_ModuleList),"N/A");

	if (EHS_FB_OUT_CONNECTED_API2(INX_RuntimeInfo2_ARG_getstat_Install_Dir))
		EhsStrcpy(EHS_FB_OUT_S_API2(INX_RuntimeInfo2_ARG_getstat_Install_Dir),EhsHMetaGetInstPath());

	EHS_FB_FINISH(INX_RuntimeInfo2_ARG_getstat_);
}//ICB FUNCTION getstat MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION getdyn MACRO START -- DO NOT ALTER
/**
 * Definition of RuntimeInfo2_getdyn.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(RuntimeInfo2_getdyn)
{
	inx_RuntimeInfo2_state_type* inx_RuntimeInfo2_state = (inx_RuntimeInfo2_state_type*)EHS_FB_RUN_CONTEXT;

	ehs_sint32 PairingID=EhsHSysGetPairedOrganisation(); /* get the current known ID */
    EhsHMetaUpdateDynamic();
	#ifdef EHS_GNU
		if (EhsTDFiles_countDirFiles ( "/proc/self/fd" ) > 100 )
			EHSH_LOG_ERROR("More than 100 file descriptoers are being used by EHS!!!")
	#endif
	// Your code here
	if (EHS_FB_OUT_CONNECTED_API2(INX_RuntimeInfo2_ARG_getdyn_IP_Addr))
		EhsStrcpy(EHS_FB_OUT_S_API2(INX_RuntimeInfo2_ARG_getdyn_IP_Addr),EhsHMetaGetIPAddr());
		
	if (EHS_FB_OUT_CONNECTED_API2(INX_RuntimeInfo2_ARG_getdyn_RAM_Avail__KB_))
		EHS_FB_OUT_I_API2(INX_RuntimeInfo2_ARG_getdyn_RAM_Avail__KB_)=EhsHMetaGetRAMAvail();             /* Get total spare RAM */

	if (EHS_FB_OUT_CONNECTED_API2(INX_RuntimeInfo2_ARG_getdyn_Stor_Avail__KB_))
		EHS_FB_OUT_I_API2(INX_RuntimeInfo2_ARG_getdyn_Stor_Avail__KB_)=EhsHMetaGetStorAvail();           /* File storage available for user data */

	if (EHS_FB_OUT_CONNECTED_API2(INX_RuntimeInfo2_ARG_getdyn_CPU_eRT))
		EHS_FB_OUT_I_API2(INX_RuntimeInfo2_ARG_getdyn_CPU_eRT)=EhsHMetaGetCPUUsage();                    /* CPU for eRT process */

	if (EHS_FB_OUT_CONNECTED_API2(INX_RuntimeInfo2_ARG_getdyn_RAM_eRT))
		EHS_FB_OUT_I_API2(INX_RuntimeInfo2_ARG_getdyn_RAM_eRT)=EhsHMetaGetRAMUsedEHS_kB();               /* ram for eRT process */

	if (EHS_FB_OUT_CONNECTED_API2(INX_RuntimeInfo2_ARG_getdyn_CPU))
		EHS_FB_OUT_I_API2(INX_RuntimeInfo2_ARG_getdyn_CPU)=EhsHMetaGetMiscAppCPUUsage();                 /* cpu for misc user-specified process */

	if (EHS_FB_OUT_CONNECTED_API2(INX_RuntimeInfo2_ARG_getdyn_RAM_Misc))
		EHS_FB_OUT_I_API2(INX_RuntimeInfo2_ARG_getdyn_RAM_Misc)=EhsHMetaGetMiscAppRAMUsed_kB();          /* RAM for misc user-specified process */
	
	if (EHS_FB_OUT_CONNECTED_API2(INX_RuntimeInfo2_ARG_getdyn_TEMP_CPU))
		EHS_FB_OUT_I_API2(INX_RuntimeInfo2_ARG_getdyn_TEMP_CPU)=EhsHMetaGetCPUTemp();
	
	if (EHS_FB_OUT_CONNECTED_API2(INX_RuntimeInfo2_ARG_getdyn_TEMP_OP))
		EHS_FB_OUT_I_API2(INX_RuntimeInfo2_ARG_getdyn_TEMP_OP)=0; // @TODO
	
	/* Handling if we have received a Devman organisation pairing code                                 */
	/* Value should be set to 2: indicating we are waiting for a 1 or 3:                               */
	/* 0: no pairing data updates, 1 pairing data valid, 2 pairing data pending, 3 paring data invalid */
	if (EHS_FB_OUT_CONNECTED_API2(INX_RuntimeInfo2_ARG_getdyn_Pair_ID)){
		switch (EhsHSysGetPairedOrganisationStatus())
		{
		case  1 : /* always assert if an update was pending */
			EHS_FB_OUT_I_API2(INX_RuntimeInfo2_ARG_getdyn_Pair_ID)=(ehs_sint32)PairingID; /* return -1 if not yet updated */
			EHS_FB_FINISH(INX_RuntimeInfo2_ARG_getdyn_Pair_ID);
			break;
		case  2 : /* pairing data still pending - do nothing */
			break;
		case  3 : //write out -1
			EHS_FB_OUT_I_API2(INX_RuntimeInfo2_ARG_getdyn_Pair_ID)=-1; /* return -1 if not yet updated */
			EHS_FB_FINISH(INX_RuntimeInfo2_ARG_getdyn_Pair_ID);
			break;
		default :
			break;
		}
	}
	EHS_FB_FINISH(INX_RuntimeInfo2_ARG_getdyn_);
}//ICB FUNCTION getdyn MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION reqpairid MACRO START -- DO NOT ALTER
/**
 * Definition of RuntimeInfo2_reqpairid.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(RuntimeInfo2_reqpairid)
{
	//inx_RuntimeInfo2_state_type* inx_RuntimeInfo2_state = (inx_RuntimeInfo2_state_type*)EHS_FB_RUN_CONTEXT;
	EhsHAppMetaRequestPairedOrganisation(); /* post a request to Devman to get this for a later call */
    EHSH_LOG_INFO("Requesting current pairing ID");
	// Your code here
}//ICB FUNCTION reqpairid MACRO END -- DO NOT ALTER THIS LINE
