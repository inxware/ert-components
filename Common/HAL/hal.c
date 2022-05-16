/** @file hal.c
 * Controls initialisation of the hardware abstraction layer at various times
 * in the life of EHS.
 *
 * @author: inx limited
 * @version: $Revision: 5656 $
 * @date: $Date$
 *
 * Copyright (c) inx limited, 2007. All rights reserved.
 */


/**
 * @page Verification Verification report
 * @section hal_graphics_font
 * @anchor hal_graphics_font
 * @subsection misra MISRA compliance:
 * test.c demonstrated MISRA compliant on
 * Last modified on $Date:$
 *
 * This file contained no derogations to the MISRA standard.
 */

/*****************************************************************************/
/* Included files */

#define EHSL_MODULE_ID EHSH_LOG_MODULE_KERNEL /**< @todo define a special logger id here */
#include "hal-api.h"

#include "app_data.h" // Needed for the app meta data structure.
#include "targetos_init.h"

#ifdef EHS_COMMS_API_SUPPORT

#ifdef  EHS_DEBUG_TCPIP_CONSOLE
#include "hal_console.h"
#include "console_server.h"
#endif

#include "targetcomms_init.h"
#endif

//@todo the following are a bit specialist for here - should have an AV HAL.
#ifdef EHS_AV_SUPPORT
#ifdef EHS_VIDEO_SUPPORT
	#include "targetvideo_init.h"
#endif
#endif

#ifdef EHS_GUI_SUPPORT
	#include "hal_gui.h"
#endif


//@todo this probably doesn't need to be here when all the system calls are moved to their respective places.
#ifdef EHS_DEVMAN_MON_SUPPORT
	#include "hal_devman.h"
#endif

/* Use this to set the log level of each component */
void EhsHSetLogLevels() {
	EhsHLogger_setLogLevel("Kernel",EHSH_LOG_LEVEL_ERROR);
	EhsHLogger_setLogLevel("HalProcess",EHSH_LOG_LEVEL_ERROR); /*  set the log level if logging enabled in build */
	EhsHLogger_setLogLevel("Devman",EHSH_LOG_LEVEL_ERROR); /*  set the log level if logging enabled in build */
	EhsHLogger_setLogLevel("Undefined",EHSH_LOG_LEVEL_ERROR); /*  set the log level if logging enabled in build */
	EhsHLogger_setLogLevel("Logger", EHSH_LOG_LEVEL_ERROR);
	EhsHLogger_setLogLevel("Graphics", EHSH_LOG_LEVEL_ERROR);
	EhsHLogger_setLogLevel("HalMemory", EHSH_LOG_LEVEL_ERROR);
	EhsHLogger_setLogLevel("HalProcess",EHSH_LOG_LEVEL_ERROR);
	EhsHLogger_setLogLevel("HalString",EHSH_LOG_LEVEL_ERROR);
	EhsHLogger_setLogLevel("TgtViewport",EHSH_LOG_LEVEL_ERROR);
	EhsHLogger_setLogLevel("Network",EHSH_LOG_LEVEL_ERROR);
	EhsHLogger_setLogLevel("file",EHSH_LOG_LEVEL_ERROR);//|EHSH_LOG_LEVEL_WARNING|EHSH_LOG_LEVEL_INFO);
}
/* Log Levels
EHSH_LOG_LEVEL_ERROR	= 0x01,
EHSH_LOG_LEVEL_WARNING	= 0x02,
EHSH_LOG_LEVEL_INFO		= 0x04,
EHSH_LOG_LEVEL_ENTER	= 0x08,
EHSH_LOG_LEVEL_EXIT		= 0x10
*/

/*****************************************************************************/
/* Declare macros and local typedefs used by this file */


/*****************************************************************************/
/* Declare prototypes of local functions */

/*****************************************************************************/
/* Types  defined with file-scope */

/* Private data structs */

EhsMetaDataType EhsMetaData;

/* Variables Required from other components */
extern EhsApplicationMetaDataType EhsApplicationMetaData;


/*****************************************************************************/
/* Variables defined with global-scope */

/**
 * Flag to indicate that initialisation has been completed for the target.
 */
EHS_LOCAL ehs_bool EhsL_initComplete =EHS_FALSE;
ehs_bool *EhsHSys_initCompleteRef = &EhsL_initComplete;

/*****************************************************************************/
/* Function definitions */

EhsMetaDataType EhsMetaData={
		.DynamicUpdateTime=0,
		.bStaticUpdate=EHS_FALSE,
		.PairedOrganisationID=0,
		.PairedOrganisationIDRequested=0, // was 2 for some reason
		.NewDevmanMiscDLData=EHS_FALSE,
		.NewDevmanMiscULData=EHS_FALSE,
		.condDevmanNewMiscDLData = PTHREAD_COND_INITIALIZER,
		.mutexDevmanNewMiscDLData = PTHREAD_MUTEX_INITIALIZER,
		.devmanPingFail = EHS_FALSE,
		.devmanLastGoodPing = 0LL
}; //flag to identify static elements are valid;

#define EHSVERSIONINFOFILE "version.nfo"


/*
 * Populate the meta data with the start up info and identifiy the install directory
 * This does not work for ehs started from a search path command.
 * @todo cover the case for run from search path.
 */
#define EHS_DEBUG_ROOT_DIR
void EhsHStoreArgInfo(ehs_uint32 argc,ehs_char ** argv,ehs_char * start_dir) {
	ehs_char buf[EHS_MAXPATHLENGTH];
	ehs_char *temp;
	ehs_uint16 i=0;
	EhsStrcpy(EhsMetaData.zArgv0,argv[0]); // contains the calling command.
	if (argc>1) EhsStrcpy(EhsMetaData.zArgv1,argv[1]);  // contains the calling parameter.
	if (EhsStrcmp(EhsMetaData.zArgv1,"debug_on_restart") ) {  // @todo make this a better opt reader
		EhsMetaData.DebugOnStart=EHSMETADATA_DEBUGONRESTART;
	}
	else {
		EhsMetaData.DebugOnStart=EHSMETADATA_NODEBUGONSTARTS;
	}

/* ToDo this code should be moved to a taret specific cade area */
#ifdef EHS_MINGW // This method for windows
	printf("***************************************************************\n");
	printf("start_dir=%s\nArgv0=%s\n",&EhsMetaData.zInstallRootDirectory[i],EhsMetaData.zArgv0);
	EhsStrcpy(EhsMetaData.zEhsStartedDirectory,start_dir); // save the PWD - @todo do we need this?
	//EhsStrcpy(EhsMetaData.zInstallRootDirectory,EhsMetaData.zArgv0);
	if (EhsMetaData.zArgv0[1] !=':') { /* Check we haven;t been given an absolute path to the exe (e.g. by XP) */
		EhsStrcpy(EhsMetaData.zInstallRootDirectory,start_dir);
		EhsStrcat(EhsMetaData.zInstallRootDirectory,"\\");
		EhsStrcat(EhsMetaData.zInstallRootDirectory,EhsMetaData.zArgv0); //if not assume we might have a relative  path to the current dir
	}else { // we were give an absolute path by windows so chuck away PWD and use this
		EhsStrcpy(EhsMetaData.zInstallRootDirectory,EhsMetaData.zArgv0);
	}
		i=EhsStrlen(EhsMetaData.zInstallRootDirectory);

	if ( (i>0) && ((EhsMetaData.zInstallRootDirectory[i-1]=='/') || (EhsMetaData.zInstallRootDirectory[i-1] == '\\'))) {
		EhsMetaData.zInstallRootDirectory[i-1]='\0';//gets rid of a trailing in path
		i--;
	}
	// get the directory above.
	while ((i>=0) && (EhsMetaData.zInstallRootDirectory[i] != '\\') && (EhsMetaData.zInstallRootDirectory[i] != '/')) {
		i--; //gets rid of ehs.exe in path
	}
	//printf("AA= %s\n",&EhsMetaData.zInstallRootDirectory[i]); // check if we have a dot before the command
	if ( (i>0) && (EhsMetaData.zInstallRootDirectory[i-1]=='.')) {
			EhsMetaData.zInstallRootDirectory[i-2]='\0';//gets rid of a trailing in path
			i-=2; //remove the slash also
		}
	if (i>0) {
			EhsMetaData.zInstallRootDirectory[i]='\0';//gets rid of bin/ehs.exe in path
		}
	while ((i>=0) && (EhsMetaData.zInstallRootDirectory[i] != '\\') && (EhsMetaData.zInstallRootDirectory[i] != '/')) {
			i--;
		}
	//printf("BB= %s\n",EhsMetaData.zInstallRootDirectory);
	if (i>0) {
		EhsMetaData.zInstallRootDirectory[i]='\0';//gets rid of bin/ehs.exe in path
	}
	else { // have a stab..
		EhsStrcpy(EhsMetaData.zInstallRootDirectory,"../");
	}

#else


#ifdef EHS_ANDROID
	//EhsStrcpy(EhsMetaData.zInstallRootDirectory,"/sdcard/ehs"); /*@todo -  hardware specific ops here for now */
	//if (EhsTF_mkdir("/sdcard/ehs")) {
		//LOGI("Opened Android data directory OK");
	//}
	//else {
		//LOGI("Failed Opened Android data directory OK");
	//}

#else
	/* Add the command line path if there was one */
	EhsStrcpy(EhsMetaData.zEhsStartedDirectory,start_dir);
	if (EhsMetaData.zArgv0[0]=='/') { /* Check we haven;t been given an absolute path to the exe (e.g. by XP) */
		EhsStrcpy(EhsMetaData.zInstallRootDirectory, EhsMetaData.zArgv0); /* useful for running in GDB */
		temp=EhsStrstr(EhsMetaData.zInstallRootDirectory, "bin/ehs.exe");
		if (temp) {
			*temp='\0'; //truncate to the beginning
		}
		else { /* not much we can do here */
			//EhsMetaData.zInstallRootDirectory ;
		}

	} else { /* Build path by concatonation */
		EhsStrcpy(EhsMetaData.zInstallRootDirectory, start_dir);

		EhsStrcpy(buf, EhsMetaData.zArgv0);
		i = EhsStrlen(buf);
		while ((i >= 0) && (buf[i] != '\\') && (buf[i] != '/')) {
			i--;
		}
		if (i > 0) {
			buf[i] = '\0';//gets rid of bin/ehs.exe in path
		} else { // have a stab..
			buf[0] = '\0';
		}
		i = EhsStrlen(EhsMetaData.zInstallRootDirectory);
		//remove any trailing /s
		if ((i > 0) && ((EhsMetaData.zInstallRootDirectory[i] == '/')
				|| (EhsMetaData.zInstallRootDirectory[i] == '\\'))) {
			EhsMetaData.zInstallRootDirectory[i] = '\0';//gets rid of ehs.exe in path
			i--;
		}
		// get the directory above.
		while ((i >= 0) && (EhsMetaData.zInstallRootDirectory[i] != '\\')
				&& (EhsMetaData.zInstallRootDirectory[i] != '/')) {
			i--;
		}
		if (i > 0) {
			EhsMetaData.zInstallRootDirectory[i] = '\0';//gets rid of bin/ehs.exe in path
		} else { // have a stab..
			EhsStrcpy(EhsMetaData.zInstallRootDirectory, ".." EHS_TD_FILES_SEPARATOR_STR);
		}
		/* Add the command path */
		EhsStrcat(EhsMetaData.zInstallRootDirectory, EHS_TD_FILES_SEPARATOR_STR);
		if (buf[0] != '.') {
			EhsStrcat(EhsMetaData.zInstallRootDirectory, buf);
		} else {
			EhsStrcat(EhsMetaData.zInstallRootDirectory, &buf[1]);
		}
	} /* end of normal concatenation method */
#endif
#endif
#ifdef EHS_DEBUG_ROOT_DIR
	//EHSH_LOG_INFO("argv0=%s",EhsMetaData.zArgv0);
	//EHSH_LOG_INFO("startdir=%s",start_dir);
	EHSH_LOG_INFO("argv0=%s\n", EhsMetaData.zArgv0);
	EHSH_LOG_INFO("startdir=%s\n", start_dir);
	EHSH_LOG_INFO("Final install dir=%s\n", EhsMetaData.zInstallRootDirectory);
#endif
	//EHSH_LOG_INFO("Done all sys init code");
	/* And set the start time stamp */
	EhsHGetdateTime(EhsMetaData.zEHSStartDate,EHS_TRUE);
	//EHSH_LOG_INFO("Done all sys init code");
}

/* Version ans system status information */
EHS_GLOBAL void EhsHMetaUpdateStatic() {
	EhsTOsSys_UpdateEnvironment(&EhsMetaData,1);
	EhsMetaData.bStaticUpdate=EHS_TRUE;
}
EHS_GLOBAL void EhsHMetaUpdateDynamic() {
	EhsTOsSys_UpdateEnvironment(&EhsMetaData,2);
}
EHS_GLOBAL const ehs_char* EhsHMetaGetInstPath() {
	return EhsMetaData.zInstallRootDirectory;
}
EHS_GLOBAL const ehs_char* EhsHMetaGetUserPath() {
	return EhsMetaData.zUserDirectory;
}
EHS_GLOBAL const ehs_char* EhsHMetaGetHWID() {
	return EhsMetaData.zDeviceID;
}
EHS_GLOBAL const ehs_char* EhsHMetaGetIPAddr() {
	return EhsMetaData.zDeviceIPAddr;
}

EHS_GLOBAL void EhsHMetaSetHWID(const char * value) {
	if (value) EhsStrcpy(EhsMetaData.zDeviceID,value);
}

EHS_GLOBAL void EhsHMetaSetInstPath(const char * value) {
	if (value) EhsStrcpy(EhsMetaData.zInstallRootDirectory,value);
}

EHS_GLOBAL void EhsHMetaSetIPAddr(const char * value) {
	if (value) EhsStrcpy(EhsMetaData.zDeviceIPAddr,value);
}

EHS_GLOBAL const ehs_char* EhsHMetaGetEHSVersion(){

	//printf("EhsMetaData.zVersion =%s\n",EhsMetaData.zVersion);
	return EhsMetaData.zVersion;
}
EHS_GLOBAL const ehs_uint32 EhsHMetaGetRAMAvail(){
	//printf("hsMetaData.RAMAvail_KB =%s",hsMetaData.RAMAvail_KB);
	return EhsMetaData.RAMAvail_KB;
}
EHS_GLOBAL const ehs_uint32 EhsHMetaGetRAMUsedEHS() {
	//printf("EhsMetaData.RAMUsed_KB =%s\n",EhsMetaData.RAMUsed_KB);
	return EhsMetaData.RAMUsed_KB;
}
EHS_GLOBAL const ehs_uint32 EhsHMetaGetRAMTotal(){
	//printf("RAM Total get=%d\n",EhsMetaData.RAMTotal_KB);
	return EhsMetaData.RAMTotal_KB;
}
EHS_GLOBAL const ehs_uint32 EhsHMetaGetStorAvail(){
	//printf("hsHMetaGetStorAvail =%s\n",);
	return (EhsMetaData.nUserSpaceTotal_KB-EhsMetaData.nUserSpaceUsed_KB);
}
EHS_GLOBAL const ehs_uint32 EhsHMetaGetStorUsed(){
	//printf(" =%s\n",);
	return EhsMetaData.nUserSpaceUsed_KB;
}
EHS_GLOBAL const ehs_uint32 EhsHMetaGetStorTotal(){
	//printf(" =%s\n",);
	return EhsMetaData.nUserSpaceTotal_KB;
}
EHS_GLOBAL const ehs_uint16 EhsHMetaGetCPUUsage(){
//	printf("CPU_usage get=%d\n",EhsMetaData.CPUUsage);
	return EhsMetaData.CPUUsage;
}
EHS_GLOBAL const ehs_uint32 EhsHMetaGetSysAvail() {
	//printf(" =%s\n",);
	return (EhsMetaData.nSysSpaceTotal_KB -EhsMetaData.nSysSpaceUsed_KB);
}
EHS_GLOBAL const ehs_uint32 EhsHMetaGetSysTotal() {
	//printf(" =%s\n",);
	return EhsMetaData.nSysSpaceTotal_KB;
}
EHS_GLOBAL const ehs_uint32 EhsHMetaGetSysUsed() {
	//printf(" =%s\n",);
	return EhsMetaData.nSysSpaceUsed_KB;
}
EHS_GLOBAL const ehs_char * EhsHMetaGetVersion() {
	//printf("EhsMetaData.zVersion =%s\n",EhsMetaData.zVersion);
	return EhsMetaData.zVersion;
}
EHS_GLOBAL const ehs_char * EhsHMetaGetBuildDate() {
	//printf("EhsMetaData.zBuildDate =%s\n",EhsMetaData.zBuildDate);
	return EhsMetaData.zBuildDate;
}
EHS_GLOBAL const ehs_char * EhsHMetaGetTargetVariant() {
	//printf("EhsMetaData.zTargetVariant =%s\n",EhsMetaData.zTargetVariant);
	return EhsMetaData.zTargetVariant;
}
EHS_GLOBAL const ehs_char * EhsHMetaGetEHSStartDate() {
	//printf("EhsMetaData.zEHSStartDate =%s\n",EhsMetaData.zEHSStartDate);
	return EhsMetaData.zEHSStartDate;
}
EHS_GLOBAL ehs_startupmode_t EhsHMetaGetDebugOnStartMode() {
	return EhsMetaData.DebugOnStart;
}

EHS_GLOBAL void EhsHMetaAppSetCurrent(ehs_char * App) {
	/* todo should check length and return an error */
	EhsStrcpy(EhsMetaData.AppCurrentLive, App);
}

//@todo - needs renaming!
EHS_GLOBAL const ehs_char* EhsHMetaAppGetCurrent(){
	return EhsMetaData.AppCurrentLive;
}

EHS_GLOBAL void EhsHMetaSetNextAppToRun(ehs_char * App) {
	/* todo should check length and return an error */
	EhsStrcpy(EhsMetaData.NextAppToRun, App);
}

EHS_GLOBAL const ehs_char* EhsHMetaGetNextAppToRun(){
	return EhsMetaData.NextAppToRun;
}



/* And the same for the app data. This structure is from the app_data. module */

EHS_GLOBAL const ehs_uint32 EhsHAppMetaGetBuildNumber() {
	//printf(" =%s\n",);
	return EhsApplicationMetaData.nSODLBuildNumber;
}

EHS_GLOBAL const ehs_char * EhsHAppMetaGetVersionNumber() {
	//printf("hsApplicationMetaData.zVersion =%s\n",EhsApplicationMetaData.zVersion);
	return EhsApplicationMetaData.zVersion;
}

EHS_GLOBAL const ehs_char * EhsHAppMetaGetAppDate() {
	//printf("EhsApplicationMetaData.zSODLdate =%s\n",EhsApplicationMetaData.zSODLdate);
	return EhsApplicationMetaData.zSODLdate;
}

EHS_GLOBAL const ehs_char * EhsHAppMetaGetAppName() {
	return EhsApplicationMetaData.zApplicationName;
}


/* Retuens true if we have had a failed ping to the primary server */
ehs_bool EhsHMetaGetMissedPing() {
	return EhsMetaData.devmanPingFail;
}

/* returns the time in seconds since the last successful ping */
time_t EhsHMetaGetCPUMissedPingTime() {
	return time(NULL) - EhsMetaData.devmanLastGoodPing;
}

void EhsHMetaResetMissedPingTime(){
	EhsMetaData.devmanPingFail = EHS_FALSE;
	EhsMetaData.devmanLastGoodPing = time(NULL);
}

void EhsHMetaSetMissedPing() {
	EhsMetaData.devmanPingFail = EHS_TRUE;
}

/* The following are inter app related functions, but get data from the ES environment - not the app environment */
EHS_GLOBAL const ehs_char * EhsHAppMetaGetLiveDir() {
	return EhsMetaData.AppCurrentLive;
}

EHS_GLOBAL const ehs_char* EhsHMetaGetSysInfo() {
	return EhsMetaData.zSysInfo;
}
void EhsHMetaSetSyscInfo(const ehs_char* zSysInfo) {
	EhsStrcpy(EhsMetaData.zSysInfo, zSysInfo);
}

/* Miscelaneoud data (JSOn usually) to server */
/* Download data */
EHS_GLOBAL ehs_bool EhsHMetaIsNewDevmanMiscDLData() {
	return EhsMetaData.NewDevmanMiscDLData;
}

EHS_GLOBAL void EhsHMetaSetNewDevmanMiscDLDataNew(ehs_bool val) {
	EhsMetaData.NewDevmanMiscDLData = val;
}

EHS_GLOBAL ehs_char* EhsHMetaGetPtrToDevmanMiscDLData(){
	return EhsMetaData.zDevmanMiscDLData;
}


EHS_GLOBAL ehs_char* EhsHMetaGetPtrToDevmanMiscDLDataType(){
	return EhsMetaData.zDevmanMiscDLDataType;
}

/* Thread safe versions
 * todo:
 * needs to concatenate data into  zDevmanMiscDLData
 * needs to write data to file in changes
 * need  a new function for reading initial data from file.
 * */
EHS_GLOBAL void EhsHMetaSetDevmanMiscDLDataType(const ehs_char* zMiscInfo) {
	#ifdef EHS_DEVMAN_SUPPORT
		EhsTPMutex_lock(EhsTPMutex_devmanMiscBuffers);
		EhsStrcpy(EhsMetaData.zDevmanMiscDLDataType, zMiscInfo);
		EhsTPMutex_unlock(EhsTPMutex_devmanMiscBuffers);
	#endif //EHS_DEVMAN_SUPPORT
}

EHS_GLOBAL void EhsHMetaSetDevmanMiscDLData(const ehs_char* zMiscInfo) {
	#ifdef EHS_DEVMAN_SUPPORT
		EhsTPMutex_lock(EhsTPMutex_devmanMiscBuffers);
		EhsStrcpy(EhsMetaData.zDevmanMiscDLData, zMiscInfo); // * todo - this needs to merge JSON - Ideally not using  JSON library for portability
		EhsStrcpy(EhsMetaData.zDevmanNewMiscDLData, zMiscInfo);
		pthread_mutex_lock(&EhsMetaData.mutexDevmanNewMiscDLData);
		pthread_cond_broadcast(&EhsMetaData.condDevmanNewMiscDLData);
		pthread_mutex_unlock(&EhsMetaData.mutexDevmanNewMiscDLData);
		EhsTPMutex_unlock(EhsTPMutex_devmanMiscBuffers);
	#endif //EHS_DEVMAN_SUPPORT
}

EHS_GLOBAL pthread_cond_t * EhsHMetaGetDevmanMiscDLDataSemaphor() {
	return &EhsMetaData.condDevmanNewMiscDLData;

}
EHS_GLOBAL pthread_mutex_t * EhsHMetaGetDevmanMiscDLDataMutex() {
	return &EhsMetaData.mutexDevmanNewMiscDLData;
}

EHS_GLOBAL void EhsHMetaGetCpyDevmanMiscDLData(ehs_char* zMiscInfo) {
	#ifdef EHS_DEVMAN_SUPPORT
		EhsTPMutex_lock(EhsTPMutex_devmanMiscBuffers);
		EhsStrcpy(zMiscInfo,EhsMetaData.zDevmanMiscDLDataType);
		EhsTPMutex_unlock(EhsTPMutex_devmanMiscBuffers);
	#endif //EHS_DEVMAN_SUPPORT
}

/* this gets just the new part */
EHS_GLOBAL void EhsHMetaGetCpyDevmanNewMiscDLData(ehs_char* zMiscInfo) {
	#ifdef EHS_DEVMAN_SUPPORT
		EhsTPMutex_lock(EhsTPMutex_devmanMiscBuffers);
		EhsStrcpy(zMiscInfo,EhsMetaData.zDevmanNewMiscDLData);
		EhsTPMutex_unlock(EhsTPMutex_devmanMiscBuffers);
	#endif //EHS_DEVMAN_SUPPORT
}

/* this gets just the new part */
EHS_GLOBAL ehs_char*  EhsHMetaGetDevmanNewMiscDLDataPtr() {

	return EhsMetaData.zDevmanNewMiscDLData;
}


/* upload Data */
EHS_GLOBAL ehs_bool EhsHMetaIsNewDevmanMiscULData() {
	return EhsMetaData.NewDevmanMiscULData;
}
/************* Not thread safe */
EHS_GLOBAL void EhsHMetaSetNewDevmanMiscULDataNew(ehs_bool val) {
	EhsMetaData.NewDevmanMiscULData = val;
}

EHS_GLOBAL ehs_char* EhsHMetaGetPtrToDevmanMiscULData(){
	return EhsMetaData.zDevmanMiscULData;
}

/********************************/
EHS_GLOBAL void EhsHMetaSetDevmanMiscULData(const ehs_char* zMiscInfo) {
	#ifdef EHS_DEVMAN_SUPPORT
		EhsTPMutex_lock(EhsTPMutex_devmanMiscBuffers);
		EhsStrcpy(EhsMetaData.zDevmanMiscULData, zMiscInfo);
		EhsTPMutex_unlock(EhsTPMutex_devmanMiscBuffers);
	#endif //EHS_DEVMAN_SUPPORT
}

EHS_GLOBAL void EhsHMetaGetCpyDevmanMiscULData(ehs_char* zMiscInfo) {
	#ifdef EHS_DEVMAN_SUPPORT
		EhsTPMutex_lock(EhsTPMutex_devmanMiscBuffers);
	//	printf("XXXX[%x] = %s\n",zMiscInfo,EhsMetaData.zDevmanMiscULData);
		EhsStrcpy(zMiscInfo,EhsMetaData.zDevmanMiscULData);
	//	printf("XXXX2[%x] = %s\n",zMiscInfo,EhsMetaData.zDevmanMiscULData);
		EhsTPMutex_unlock(EhsTPMutex_devmanMiscBuffers);
	#endif //EHS_DEVMAN_SUPPORT
}


void EhsHGetEHSVersionInfo(EhsMetaDataType * EhsMetaData) {
	ehs_FILE* EhsVersionFile;
	ehs_char temp[256];

	EhsVersionFile = Ehs_SysFopen(EHSVERSIONINFOFILE,"r"); /* if sysdata doesn't exist create in default directory */
		if (!EhsVersionFile) {
			ehs_char szParameterFilePath[EHS_MAXPATHLENGTH];
			EhsTF_tryCanonicPath(szParameterFilePath, EHS_RUNTIME_SYSDATA_DIR ,EHSVERSIONINFOFILE,EHS_TRUE );
			EHSH_LOG_ERROR("Can't open version info file: %s",szParameterFilePath);
			EhsStrcpy(EhsMetaData->zVersion,"ehs-n/a");
			EhsStrcpy(EhsMetaData->zBuildDate,"ehs-n/a");
			EhsMetaData->nRepoID=-1; //@todo need formal error enumerator for data not present
			EhsStrcpy(EhsMetaData->zTargetVariant,"ehs-n/a");
			EhsStrcpy(EhsMetaData->zModuleList,"ehs-n/a");
		}
		else {
			EhsFgets(EhsMetaData->zVersion,EHS_STRING_LENGTH_MAX,EhsVersionFile);
			EhsFgets(EhsMetaData->zBuildDate,EHS_STRING_LENGTH_MAX,EhsVersionFile);
			EhsFgets(temp,64,EhsVersionFile); // skip the build tag hash
			if (EhsStrlen(temp)>0  && EhsStrlen(temp)< 64) EhsMetaData->nRepoID=atoi(temp);
			else {
				EhsMetaData->nRepoID=-2; //@todo need formal error enumerator for invalid data
			}
			//EhsFgets(EhsMetaData->zTargetVariant,EHS_STRING_LENGTH_MAX,EhsVersionFile); // skip the build tag hash
			EhsFgets(EhsMetaData->zTargetVariant,EHS_STRING_LENGTH_MAX,EhsVersionFile);
			EhsFgets(EhsMetaData->zModuleList,EHS_STRING_LENGTH_MAX,EhsVersionFile);
			EhsFclose(EhsVersionFile);
		}
}


/* EHS control interface @todo review if a new module should contain this..*/

/* Devman specific bits */
/* Device Pairing */
void EhsHAppMetaRequestPairedOrganisation() {
	//EhsMetaData.PairedOrganisationID = -1 ;
	EhsMetaData.PairedOrganisationIDRequested=2;
	//return EHS_TRUE;
}
/* 0: no pairing data updates, 1 pairing data valid, 2 pairing data pending, 3 paring data invalid*/
/* set the pairing ID */
void EhsHSysUpdatePairedOrganisation(ehs_uint32 id) {
	//EhsMetaData.PairedOrganisationIDRequeste=0;
	if (id == 0xFFFF || id == 0) {
		EhsMetaData.PairedOrganisationIDRequested=3; /* invalid data */
	}
	else {
		EhsMetaData.PairedOrganisationID= id;
		EhsMetaData.PairedOrganisationIDRequested=1; /* valid data */
	}
	//return EHS_TRUE;
}

/* return true of the device is definately paired */
ehs_uint8 EhsHSysGetPairedOrganisationStatus() {
	return EhsMetaData.PairedOrganisationIDRequested;
}

ehs_uint32 EhsHSysGetPairedOrganisation() {
	//EhsMetaData.PairedOrganisationIDRequeste=0;
	return EhsMetaData.PairedOrganisationID;
}



/********************** Requires target Sepcific support ***********************/

/* Date in W3c format */
ehs_bool EhsHSysUpdateDate(ehs_char * datestring) {
	//@todo move code to the target specific implementation
	EHSH_LOG_WARNING("EhsHSysUpdateDate(%s) - Not Implemented", datestring);
	return EHS_TRUE;
}

#if 0
/* expects =setting in json form: { ipaddr="x.x.x.x", gateway="x.x.x.x", mask="x.x.x.x", dns1="x.x.x.x" */
ehs_sint8 EhsSysSetStaticIpV4Addr(ehs_char * json) {
	const ehs_char ipaddr_label[] = "ipaddr:\"";
	const ehs_char gateway_label[] = "gateway:\"";
	const ehs_char mask_label[] = "mask:\"";
	const ehs_char dns1_label[] = "dns1:\"";
	const ehs_uint8 min_addr_str_len = 7;
	ehs_char zStrIpaddr[ 16];
	ehs_char zStrGateway[ 16];
	ehs_char zStrMask	[ 16];
	ehs_char zStrDns1	[ 16];
	ehs_char * next = json;
	ehs_char * found = NULL;
	if (json == NULL) return -1 ;
	found = EhsStrstr(next, "ipaddr:\"");
	if (found) {
		if (EhsStrlen(found) > EhsStrlen(ipaddr_label)+min_addr_str_len) {
			EhsStrcpyUIpto UpTo(zStrIpaddr,6)


	}

#endif

ehs_bool EhsHSysSetResetTime(ehs_char * datestring) {
	//@todo
	/* 		devmanCronFile = io.open("../sysdata/devman.cron", "w");
	 devmanCronFile:write(time.." /root/ehs/bin/reboot.sh > /dev/null\n"); -- this is unix specific.
	 devmanCronFile:close();

	 */
	EHSH_LOG_WARNING("EhsHSysSetResetTime(%s) - Not Implemented", datestring);
	return EHS_TRUE;
}

/* We may have variations on this that will delete only directories within */
ehs_bool EhsHFileClearUserData(ehs_char *time_string_floating_point) {

	Ehs_UserRm("playlist.smil");
	Ehs_UserRmdir("media");
	/*
	 * if string.find(response, "") then
	 for temp in string.gmatch(response, "<ClearStorage>(.-)<ClearStorage/>") do
	 days = temp;
	 end
	 if days == "0" then
	 --rmdir("../userdata/media"); -- todo this should only remove all content not the directory.*/
	 //os.execute("rm -Rf ../userdata/media/*");
	 /*else
	 days = days - 1;
	 print("days="..days);
	 os.execute("find ../userdata/ -atime +"..days.." | grep -v '../userdata/$' | xargs rm -rf");
	 end
	 end
	 -- restart ehs
	 */
	EHSH_LOG_WARNING("EhsHFileClearUserData(%s) - Not Fully Implemented", time_string_floating_point);
	return EHS_TRUE;
}

/* Sets a flag that EHS should close and restart (forcibly if necessary via WDT). */

ehs_bool EhsHSysReLoad() {
	return EHS_TRUE;
}

/* EHS FSM control functions - to be used by function blocks or target specific components to handle host app lifecycles requirements */

ehs_bool EhsHRequestEHSInterrupt() {
	if (EhsMetaData.InternallyRequestedCommand !=	EHS_CONTINUE) {return EHS_TRUE;}
	else  {return EHS_FALSE;}
}




/* This returns the requested state and sets it back to the default "continue" command */
EHS_GLOBAL Ehs_ConsoleCommand_Type EhsHFSMGetInternallyRequestedCommand(){
	Ehs_ConsoleCommand_Type state = EhsMetaData.InternallyRequestedCommand;
	EhsMetaData.InternallyRequestedCommand=	EHS_CONTINUE;
	return state;
}

#ifndef EHS_DEBUG_TCPIP_CONSOLE
/* satisfy kernel's processing if we don't have the console enabled with the bhe follwing stubs*/
//TODO2022 Refactor console support to be not potentially only TCPIP (e.g. also serial,...)and make it always included to avoid having to do things  like this
ehs_bool EhsConsoleInputHit() {return EHS_FALSE;}
ehs_uint16 EhsConsolePrintf(const ehs_char* fmt, ...) {return 0;}
ehs_bool EhsConsoleToFile(ehs_uint32 size, const ehs_char* name)  { return EHS_TRUE;}
ehs_uint32 EhsConsoleGetLine(char *buff, ehs_uint16 size) {return 0;}
#endif

/* Tell EHS to change state
 * This can bounce the request and return false if it thinks the current request is more important
 */
EHS_GLOBAL ehs_bool EhsHFSMSetInternallyRequestedCommand(Ehs_ConsoleCommand_Type state) {
	if (EhsMetaData.InternallyRequestedCommand == EHS_CONTINUE) { //Any other request is more important!
		EhsMetaData.InternallyRequestedCommand = state;
		EHSH_LOG_INFO("Setting command to run next time");
		return EHS_TRUE;
	} else {
		return EHS_FALSE;
	}
}

 /* don't check the current state is not EHS_CONTINUE */
EHS_GLOBAL void EhsHFSMForceInternallyRequestedCommand(Ehs_ConsoleCommand_Type state ){

		EhsMetaData.InternallyRequestedCommand = state;
}



/**
 * Shut down everything
 * @todo remove this function there are loads if EHS shutdowns!!
 *
 */
void EhsExit(ehs_uint16 exitCode)
{
	// todo attempt to do nice things with toolkits (e.g.g shutdown gtk, close sockets etc.)
	static ehs_bool bExited = EHS_FALSE; // don't let EhsHSys_term() run twice!
	#ifdef EHS_DEBUG_TCPIP_CONSOLE // todo2022 this should be a higher level comms close down - not just the console?
		EhsSvcTcp_closeConnection();
	#endif //EHS_COMMS_API_SUPPORT
	if (!bExited) {
		bExited = EHS_TRUE;
		EhsHSys_term();
	}
	EHSH_LOG_WARNING("Terminated System - exiting with exit code %d\n",exitCode);

	EhsTargetExit(exitCode);
	//*todo we should add a flag that the OS uses not to respawn EHS when it runs persistently.
}


ehs_bool EhsHSysRestart() {
	//@todo
	EhsExit(0);
	return EHS_TRUE;
}

ehs_bool EhsHSysReboot() {
	//@todo
	EHSH_LOG_ERROR("EhsHSysReboot() - Not Implemented\n");
	// Use Exec here so that a bash environment is not started. (this should run the reboot script in /bin.
	return EHS_TRUE;
}

/*********************** End of System services that can be called from components ********************************/


/**
 * Called to initialise the HAL during system initialisation time
 * @todo this should be in a separate kernel HAL - not exposed to components.
 * @todo - complex init sequence that is required to be run in predefined order, need to add a "init level" to each part of initialisation and test to see of at correct level before run next part of initialisation
 */



/**
 * \brief Populates OS/install meta data for the EHS instance.
 *
 */
ehs_bool EhsHOsSys_UpdateEnvironment() {
	return EhsHUpdateFilePathEnvironment(&EhsMetaData);
}

/**
 * Called to shutdown the HAL prior to shutting down EHS
 *
 * @todo the following should be common for all targets
 * @todo the component specific ones need to be moved to toolboxes
 */
void EhsHSys_term(void) {

   /* Shutdown the application & component threads*/
	EhsHApp_term();
	EHSH_LOG_INFO("Terminated Application\n");
  /* Shutdown the system */
	EhsTOsSys_term();
	EHSH_LOG_INFO("Terminated Application\n");
	#ifdef EHS_COMMS_API_SUPPORT
		EhsTCommsSys_term();
	#endif //EHS_COMMS_API_SUPPORT
#ifdef EHS_GUI_SUPPORT
	EhsTGfxSys_term();
	EHSH_LOG_INFO("Terminated Graphics Subsystem\n");
#endif
#ifdef EHS_AV_SUPPORT
#ifdef EHS_VIDEO_SUPPORT
	EhsTVideoSys_term();
	EHSH_LOG_INFO("Terminated A/V Subsystem\n");
#endif
#endif
}

void EhsHSys_init() {
	EhsHMem_init(); /* this function has no pre-requisites: it should be called first */
	EhsHLogger_init(); /* initialise the system Logger */
	/* EHSH_LOG_LEVEL_ERROR|EHSH_LOG_LEVEL_WARNING|EHSH_LOG_LEVEL_INFO|EHSH_LOG_LEVEL_ENTER|EHSH_LOG_LEVEL_EXIT */
	EhsHSetLogLevels();

#ifdef EHS_NETWORKING_SUPPORT
	EhsHURLGlobalInit();
#endif

	EhsTOsSys_init(); /* initialise the Operating System */
#ifdef EHS_DEBUG_TCPIP_CONSOLE
	EhsHThread_execute(EhsSvcTcp_server,NULL,-90);
#endif
	EhsHGetEHSVersionInfo(&EhsMetaData); /*Populate the version information table */
	/* Path info is defined at first start before any cds etc */
	EhsHOsSys_UpdateEnvironment(); /*Call generic OS code e.g. user data path */
	EhsTOsSys_UpdateEnvironment(&EhsMetaData,0); /*Call target specific Dynamic infor e.g. Network ID, etc. */
	#ifdef EHS_COMMS_API_SUPPORT
	EhsTCommsSys_init();
	#endif //EHS_COMMS_API_SUPPORT
	EhsBinSearchPath(); /* Add a search path to bin (@todo Still needed?) */

	//@todo ALL THIS SHOULD BE MOVED TO THE MODULE INITIALISATION CODE
#ifdef EHS_DEVMAN_MON_SUPPORT
	DevmanMon_init(); // Non Component devman functions are considered part of the HAL
#endif
#ifdef EHS_LUA_SUPPORT
//#ifdef LUA scripting component
	EhsHLuaInit() ; // sets up the environment path for its utilities
#endif

	//@todo move these to the function-block-space initialisers
#ifdef EHS_GUI_SUPPORT
	// @todo move to widget initialisation
	EhsTGfxSys_init();
#endif
#ifdef EHS_AV_SUPPORT
#ifdef EHS_VIDEO_SUPPORT
	EhsTVideoSys_init();
#endif
#endif
	/* set flag to indicate to other threads that EHS is now in business */

	EhsMetaData.PairedOrganisationIDRequested=0; //was 2 for some reason?
	EhsHMetaSetNewDevmanMiscDLDataNew(EHS_FALSE);
	ehs_char* data = EhsHMetaGetPtrToDevmanMiscDLData();
	data[0]='\0';
	data = EhsHMetaGetPtrToDevmanMiscDLDataType();
	data[0]='\0';

	EhsL_initComplete = EHS_TRUE;
}


/**
 * Called to prepare the HAL for loading in a new application.
 * @todo any of the below which are purely used in function blocks should be moved to
 * function block initialisation system. FB shred init system TBC!
 */
void EhsHApp_init() {
	EhsTOsApp_init(); /* Target Operating System initialisation */
	#ifdef EHS_COMMS_API_SUPPORT
		EhsTCommsApp_init();/* Console comms interface - for debugging */
	#endif //EHS_COMMS_API_SUPPORT
#ifdef EHS_GUI_SUPPORT
	EhsTGfxApp_init();
	EhsGraphicsFontTable_init();
	EhsWidgetViewport_default_config();//Clear the old viewport parameters in case the new one has no widget for it
#endif
#ifdef EHS_AV_SUPPORT
#ifdef EHS_VIDEO_SUPPORT
	EhsTVideoApp_init();
#endif
#endif
}

/**
 * Called to prepare the HAL to start executing an application (or to reset
 * an existing application). This needs to preserve the SODL and init data.
 */
void EhsHApp_reset() {
	/* clear up from earlier phase */
	EhsTOsApp_reset(); //doesn't so anything currently
	#ifdef EHS_COMMS_API_SUPPORT
		EhsTCommsApp_reset(); //doesn't so anything currently
	#endif
#ifdef EHS_GUI_SUPPORT
	EhsTGfxApp_reset(); //doesn't so anything currently
	EhsGraphicsFontTable_init(); // the memory is wiped by the garbage collector.
	EhsWidgetViewport_default_config();//Clear the old viewport parameters in case the new one has no widget for it
#endif
#ifdef EHS_AV_SUPPORT
#ifdef EHS_VIDEO_SUPPORT
	EhsTVideoApp_reset(); //doesn't so anything currently
#endif
#endif
	/* entered new phase - start by switching memory pool - this clears the memory allocated for the app! */
	EhsHMem_switchPool(EHSHM_POOL_APP_DATA); //clears memory
#ifdef EHS_GUI_SUPPORT
	//EhsGraphicsFontTable_init();
#endif
}


/* Return when the target signals all is good to go
 * timeout in seconds or -1 for wait for ever
 * todo review if passing void (*target_loop_iteration)(void *),void * target_env_blob is better than just having these as global variables
 * */
void EhsHWaitForTarget(Ehs_ConsoleCommand_Type (*target_loop_iteration)(void *),void * target_env_blob,ehs_sint16 timeout) {
	ehs_sint32 i=0;
	ehs_bool contin=EHS_TRUE;
	while (contin) {
		if (EhsTPlatformReady( (void*) target_loop_iteration,target_env_blob) ) break;
		if (timeout>0 && i> timeout*10) break;
		i=1+1;
		EhsSleep(100); //sleep for 100ms
	}
	// where do we do this - if at all - pthread cond destroy
}



/**
 * Called to activate the thread to show the app
 */
void EhsHApp_show(void) {
#ifdef EHS_GUI_SUPPORT
	EhsTV_show(&EhsTV);
#endif
}

/**
 * Called to clean up after the execution of an application in order to make
 * sure everything has been stopped.
 */
void EhsHApp_term(void) {
	EhsTOsApp_term();
	//EhsTCommsApp_term();  we don't want to kill the comms for an app do we?
/* This is done by the system shutdown only we don't terminate these during an App term
	#ifdef EHS_GUI_SUPPORT
	EhsTGfxApp_term();
#endif
#ifdef EHS_AV_SUPPORT
#ifdef EHS_VIDEO_SUPPORT
	EhsTVideoApp_term();
#endif
#endif
*/
}
