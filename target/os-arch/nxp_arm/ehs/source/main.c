/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

//todo  DIAGNOSTIC BUILD SETTING SHOULD BE A LIFESTYLE CHoic setting it seems
//todo  remove relays checks from the HRC variant (sensors checks are disabled but relays still actuate in IEC modes.
/*******************************************************************************
 * Includes
 ******************************************************************************/
#define BOARD_FTM_BASEADDR FTM0

/* Interrupt number and interrupt handler for the FTM instance used */
#define BOARD_FTM_IRQ_NUM FTM0_IRQn
#define BOARD_FTM_HANDLER FTM0_IRQHandler

/* Get source clock for FTM driver */
#define FTM_SOURCE_CLOCK (CLOCK_GetFreq(kCLOCK_BusClk))


#include "target_config.h"
#include "fsl_ftm.h"
#include "cr_section_macros.h"
#include <inx_iec.h>
#include "MK64F12.h"
#include "ksdk_mbedtls_config.h"
#include "certificate.h"
#include "heatrod_config.h"
#include "utz.h"
#include "zones.h"
#if INX_HEATROD_EHS
	#include "ehs_main.h"
	#include "console.h"
	#include "hal.h"
	#include "app_data.h"
	#include "statmodules.h"
	ehs_bool EhsPermanentStorageCopyToBuffer(const uint8_t* pBufferStart,const uint32_t bufferMaxSizeBytes);
#endif //INX_HEATROD_EHS

#include "bootloader/bl_context.h"
#include "bootloader/bl_reliable_update.h"
#include "lwip/opt.h"
#include "RNG1.h"
#include "errorLogs.h"
#include "sys_arch.h"
#include "fsl_wdog.h"
#include "fsl_rcm.h"
#include "bootloader/bl_shutdown_cleanup.h"
#include "mbedtls/oid.h"
#include "mbedtls/md5.h"


#ifdef LWIP_SOCKET
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include "lwip/dhcp.h"
#include "lwip/netif.h"
#include "lwip/sys.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#include "lwip/tcpip.h"
#include "lwip/ip.h"
#include "lwip/netifapi.h"
#include "lwip/sockets.h"
#include "netif/etharp.h"

#include "dns.h"
#include "fsl_phy.h"

#include "ethernetif.h"
#include "board.h"

#include "httpsrv.h"
#include "httpsrv_ws.h"
#include "mdns.h"

#include "fsl_device_registers.h"
#include "pin_mux.h"
#include "clock_config.h"

#include "fsl_adc16.h"
#include "fsl_rtc.h"
#include "app_dhcp.h"
#include "fsl_lptmr.h"
#include "fsl_pmc.h" //used for calibrating the internal temperature sensor

#include "mbedtls/ssl.h"
#include "mbedtls/ssl_cache.h"
#include "mbedtls/debug.h"
#include "mbedtls/net.h"
#include "fsl_rnga.h"
#include "mbedtls/memory_buffer_alloc.h"

#include "IEC60730_Safety.h"

#include "fsl_flash.h"
#if defined(FSL_FEATURE_HAS_L1CACHE) && FSL_FEATURE_HAS_L1CACHE
#include "fsl_cache.h"
#endif /* FSL_FEATURE_HAS_L1CACHE */

#include "queue.h"
#include <cr_section_macros.h>

#include "mb.h"
#include "mbport.h"

#include "inx_led.h"
#include "inx_buttons.h"
#include "inx_display.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

//#define pwmcalc_printf(...) printf(__VA_ARGS__)
#define pwmcalc_printf(...) {}

/*******************************************************************************
* Prototypes
******************************************************************************/

static void cgi_urldecode(char *url);
static int cgi_example(HTTPSRV_CGI_REQ_STRUCT *param);
static int ssi_date_time(HTTPSRV_SSI_PARAM_STRUCT *param);
static bool cgi_get_varval(const char *var_str, char *var_name, char *var_val, uint32_t length);

static bool handleProvisioningCertificate(const char* clientCert, bool bExtractPassword);
static bool compareCertificatePassword(char* data);

const uint32_t ERROR_STATE_CLEAR = 0x0000; // NO ERRORS
const uint32_t ERROR_STATE_MAP[ERROR_LOGS_END] =
{
	0x0001, // ERROR_STATE_INTERNAL_RELAY_STUCK_ON
	0x0002, // ERROR_STATE_INTERNAL_RELAY_STUCK_OFF
	0x0004, // ERROR_STATE_THERMAL_CUT_OUT
	0x0008, // ERROR_STATE_ADC_DISABLED
	0x0010, // ERROR_STATE_ADC_VALUES_UNSAFE
	0x0020, // ERROR_STATE_WATCH_DOG_RESETS
	0x0040, // ERROR_STATE_RAM_BOUNDARIES
	0x0100, // ERROR_STATE_RAM_WALK
	0x0200, // ERROR_STATE_INTERRUPTS
	0x0400, // ERROR_STATE_GPIO
	0x0800, // ERROR_STATE_FORCED_FACTORY_RESET
	0x1000, // ERROR_STATE_RTC_DISABLED
	0x2000, // ERROR_STATE_PROGRAM_FLASH_CRC
	0x4000, // ERROR_STATE_DISPLAY_BOUNDARY
	0x8000  // ERROR_STATE_USER_ALARM_TEMPERATURE
	// ...
	// ERROR_STATE_NEW_ERROR
};

static uint32_t getSystemErrorState();

/*******************************************************************************
* Variables
******************************************************************************/
/*
 * Authentication users
 */
/*
static const HTTPSRV_AUTH_USER_STRUCT users[] = {
    {"admin", "admin"}, {NULL, NULL}
};


static const HTTPSRV_AUTH_REALM_STRUCT auth_realms[] = {
    {"Please use uid:admin pass:admin to login", "/auth.html", HTTPSRV_AUTH_BASIC, users},
    {NULL, NULL, HTTPSRV_AUTH_INVALID, NULL}
};
*/

char cgi_data[CONFIG_CGI_DATA_LENGTH_MAX + 1];
char socket_data[CONFIG_CGI_DATA_LENGTH_MAX + 1];

const HTTPSRV_CGI_LINK_STRUCT cgi_lnk_tbl[] = {
    {"get", cgi_example},
	{"get_network",cgi_example},
    {"post", cgi_example},
	{"post_reset",cgi_example},
	{"post_hardware",cgi_example},
	{"post_network",cgi_example},
	{"post_mobile_provision",cgi_example},
	{"post_mobile_provisioning_information",cgi_example},
	{"post_mobile_status",cgi_example},
    {0, 0} // DO NOT REMOVE - last item - end of table
};

static ip4_addr_t gDNS[2]={0};
static bool gEhsTick=false;
Ehs_ConsoleCommand_Type Command;
extern volatile bool gGenerateDiagnosticReport;
static uint32_t gADCRTCLastGoodSeconds=0;
static volatile uint32_t gFTMTicks=0;
uint32_t gMqttConnections=0;
bool gDiagnosticBoot=false;
static bool gRTCStuck=false;
const HTTPSRV_SSI_LINK_STRUCT ssi_lnk_tbl[] = {{"date_time", ssi_date_time}, {0, 0}};
static bool gDiagnosticsDone=false;

typedef enum {
	PROVISION_STATE_UNPROVISIONED = 0,
	PROVISION_STATE_PROVISIONED,
	PROVISION_STATE_SEMI_PROVISIONED
} INX_Provison_State;
inxRelayCurrentState_t gRelayErrorState;
inxRelayCurrentState_t gStartupRelayErrorState;
volatile static uint8_t gADCSamplesPerSecond = 0;
volatile static uint8_t gADCSamplesCount = 0;
volatile float gCal1=0;
volatile float gCal2=0;
volatile float gCal3=0;
volatile bool gProvisioned=false;
static bool gProvisionCertificateValidated=false;
#define MAX_PROVISION_FAILED_COUNT 5
static uint8_t gProvisionFailedCount = 0;
#define UNKNOWN_ORGANISATION_ID (-1)
static int64_t gOrganisationID = UNKNOWN_ORGANISATION_ID;
uint32_t gLPTMRInterruptMilliseconds=CONFIG_LPTMR_DEFAULT_INTERRUPT_MILLISECONDS;
uint32_t gMinTicksSinceHeaterChange=0;
uint32_t gTicksSinceSettingsSave=0;
static NETWORK_State_t gNetworkState=NETWORK_STATE_INIT;
static bool gDoFactoryReset=false;
bool gDoReset=false;
inxNetworkConnectionMode_t gNetworkMode=INX_NETWORK_CONNECTION_MODE_OFF;

volatile inxAuxControlEnum_t gAuxControlOutput=INX_AUX_DISABLED;
volatile inxPowerModeEnum_t gPowerMode=INX_POWER_MODE_SINGLE_PHASE_240;
float gUserAlarmsAdcValues;

typedef enum {
	SUPERVISOR_NETWORK_MONITORING_INIT,
	SUPERVISOR_NETWORK_MONITORING_OK,
	SUPERVISOR_NETWORK_MONITORING_STOPPED
} SupervisorNetworkMonitoringState_t;
static SupervisorNetworkMonitoringState_t gSupervisorNetworkMonitoringState=SUPERVISOR_NETWORK_MONITORING_INIT;


/*
 * Settings and Factory defaults
 *
 * todo Mixed in with all sorts
 *
 * todo - consider making all defaults #defines in the config file rather than e.g. prepend FACTORY_DEFAULT_XXXX to each variable here to make a #define
 * */

/* Switcher board configurations */
#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRi
volatile inxControllerOutputMode_t gControllerOutputMode=INX_CONTROLLER_OUTPUT_MODE_EXTERNAL_RELAY;
#else
volatile inxControllerOutputMode_t gControllerOutputMode=INX_CONTROLLER_OUTPUT_MODE_DISABLED;
#endif

#ifdef INX_HEATROD_IOT_DIAGNOSTIC_BUILD
volatile inxSafetyMode_t gSafetyMode=INX_SAFETY_MODE_DISABLE_ALL_HARDWARE_CHECKS;
#else
volatile inxSafetyMode_t gSafetyMode=INX_SAFETY_MODE_EN60335;
#endif

volatile inxControllerContactorMode_t gControllerContactorMode=INX_CONTROLLER_CONTACTOR_MODE_EMR;

extern const unsigned char inx_default_sodl[];
volatile bool gControllerOutputUpdated=false;
volatile bool gDynamicSetPointUpdated=false;
/* Control algorithm config todo not in settings */
volatile float gDutyCycleLengthEMRSeconds=CONFIG_DEFAULT_DUTY_CYCLE_LENGTH_EMR_SECONDS;
volatile float gDutyCycleLengthSSRSeconds=CONFIG_DEFAULT_DUTY_CYCLE_LENGTH_SSR_SECONDS;
extern const uint32_t c_ui32StackTestFirstAddress;  /*defined in IEC60730_Safety.c */
extern const uint32_t c_ui32StackTestSecondAddress; /*defined in IEC60730_Safety.c */
extern uint32_t* gpInxIecHeapStart;
extern uint32_t* gpInxIecHeapEnd;
safety_common_t g_sSafetyCommon;
uint32_t gSettingsWriteId=0;
static uint32_t gHttpsrvHandle=0;
/* FS data.*/
extern const HTTPSRV_FS_DIR_ENTRY httpsrv_fs_data[];
static char gWebSocketData[CONFIG_CGI_DATA_LENGTH_MAX+1];
static ethernetif_config_t fsl_enet_config0 = {
	.phyAddress = CONFIG_ENET0_PHY_ADDRESS, .clockName = CONFIG_ENET0_CLOCK_NAME, .macAddress = CONFIG_MAC_ADDR,
};
static QueueHandle_t gAdcValuesQueue;
globalADCValues_t gValues;
static volatile int nValueIndex = 0;
static uint32_t gFlashSectorSize = 0;
static uint32_t gFlashTotalSize = 0;
static uint32_t gFlashBlockBase = 0;
static uint32_t gSettingsStorageFirstSectorAddress;
static WS_USER_CONTEXT_STRUCT gWebSocketContext;
static ip4_addr_t gfsl_netif0_ipaddr, gfsl_netif0_netmask, gfsl_netif0_gw;
volatile bool gSendSettings=false;
volatile bool gSendIdentification=false;
static WDOG_Type *wdog_base = WDOG;
static volatile bool gADCValuesUnsafe=false;
static volatile bool gADCValuesUnsafePreviousValue=false;
volatile bool gUserTemperatureAlarmTriggered=false;
volatile bool gSettingsUpdatedByUser=false;
volatile bool gErrorLogsUpdated=false;
volatile inxNoFactoryResetStorage_t gNoResetSettings={0};
volatile static permStorage_t gSettingsBuffer;
static uint8_t gSettingsToWrite[CONFIG_FLASH_SECTOR_SIZE_BYTES]; //data that we write to flash always be a full sector
static char gLocalName[CONFIG_LOCAL_NAME_LENGTH]="";
static char gLocation[CONFIG_LOCATION_LENGTH]="";
static char gOrganisationName[CONFIG_ORGANISATION_NAME_LENGTH]="Loading...";
static float gElementRatingKW=0.0;
struct netif fsl_netif0;
//static wd_test_t gsSafetyWdTest;
//static RCM_Type *rcm_base = RCM;
static globalADCValues_t gADCValues[2];
volatile bool gRTCDisabled=false;
static volatile uint32_t gADCRTCTicks=0;
bool gRTCInitDone=false;
uint32_t gRTCPreviousSeconds=0;
static uint32_t gRTCConseqGood=0;
extern inxButtonState_t gButtonState;
/* todo move these into a config struct perhaps similar to the flash write struct */
static char clientCpuId[INX_CPU_ID_MAX_LENGTH];
static char gHashedClientId[HASH_CLIENT_ID_MAX_LENGTH];
static char clientId[DISPLAY_CLIENT_ID_MAX_LENGTH];
static char gDevicePin[HASH_PIN_MAX_LENGTH];
static char gCertificatePassword[CONFIG_CERTIFICATE_PASSWORD_LENGTH];
static int64_t gCertificateOrganisationID = UNKNOWN_ORGANISATION_ID;
static mbedtls_x509_time gCertificateExpieryDate = {0, 0, 0, 0, 0, 0};
#if CONFIG_MDNS_ENABLED
#define MDNS_NAME_BUFFER_LENGTH (sizeof(CONFIG_MDNS_HOSTNAME) + HASH_CLIENT_ID_MAX_LENGTH + 1)
static char gMdnsName[MDNS_NAME_BUFFER_LENGTH];
#endif
static volatile bool gADCValuesReady = false;
static uint8_t gAuxiliary=0;
inxPID_t gPIDs[CONFIG_PID_CONTROLLERS_NUM]={0};

static bool gDHCP=true;
errorLogs_t gErrorLogs;
static uint32_t gCrcValue=0; //initialised by loadSettings
static uint32_t gProgramSizeBytes=0; //initialised by loadSettings
u8_t gOTAUpdateOneSectorBuffer[CONFIG_FLASH_SECTOR_SIZE_BYTES+CONFIG_FIRMWARE_CHUNK_SIZE]={0};
#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRi || INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
inxWatchDogState_t gWatchDogState = { true,false,0,0xffffffff,0,0xffffffff };
#else
inxWatchDogState_t gWatchDogState = { false,false,0,0xffffffff,0,0xffffffff };
#endif

static adc16_channel_config_t adc0ChnConfig;
static adc16_channel_config_t adc1ChnConfig;
//PBB not doing this for now as it is out of band functionality
//static uint32_t* gpWatchDogFlag = 0x1fff0000; //the flag that tells us when we got shut down, current points to first chunk of SRAM_LOWER

volatile dutyCycleState_t dutyCycleState0;
volatile dutyCycleState_t dutyCycleState1;
volatile dutyCycleState_t dutyCycleState2;
static dutyCycleHistory_t dutyCycleHistory0;
threadSafeDutyCycleHistory_t gMqttDutyCycleHistory0={0}; // Does this need to be volatile too?
static uint8_t gAdcCurrentElementCount=0;
INX_Local_UI_Mode_t gLocalUIMode=INX_LOCAL_UI_MODE_DISABLE;
adcRawValues_t gADCRawValues;
volatile pidConditionState_t gAdcConditionState={0};

typedef enum {
	INX_GPIO_CONDITION_DIAGNOSTICS,
	INX_GPIO_CONDITION_OK,
	INX_GPIO_CONDITION_DISABLED
} inxGpioConditionStateMode_t;

typedef struct _inxGpioConditionState_t {
	inxGpioConditionStateMode_t mode;
} inxGpioConditionState_t;
static volatile inxGpioConditionState_t gGpioConditionState;

/*******************************************************************************
 * Code
 ******************************************************************************/
static const uint64_t HASH_SEED_ID 	= 19;
static const uint64_t HASH_SEED_PIN = 89;
static const uint64_t HASH_MOD_ID 	= 10000000000;
static const uint64_t HASH_MOD_PIN 	= 100000;
static const uint64_t PRIME_1 		= 56893;
static const uint64_t PRIME_2 		= 70607;

#ifdef INX_HEATROD_IOT_DIAGNOSTIC_BUILD
/* Test-Only display functions - hacky duplicates of production functions */

/* blocks for about 1s - only use for debug vuilds */
void inxDiagnosticWaitLongTime(int i){
	volatile uint32_t count=0;
	while(count<1000000*i){
		count++;
	}
}
#endif

bool inxGPIOOn(GPIO_Type *base, uint32_t mask){
	bool ok=false;
	if(base){
		GPIO_PortSet(base, 1U << mask);
		ok=true;
	}
	return ok;
}

bool inxGPIOOff(GPIO_Type *base, uint32_t mask){
	bool ok=false;
	if(base){
		GPIO_PortClear(base, 1U << mask);
		ok=true;
	}
	return ok;
}


static void inxGPIORelayOff(GPIO_Type *base, uint32_t mask){
	if(base){
		#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_FRDMK64EVAL
			GPIO_PortClear(base, 1U << mask);
		#else
			GPIO_PortSet(base, 1U << mask);
		#endif
	}
}

static bool inxGPIORelayOn(GPIO_Type *base, uint32_t mask){
	bool ok=false;
	if(gWatchDogState.recoverableRTCError || gGpioConditionState.mode == INX_GPIO_CONDITION_DISABLED || gWatchDogState.deviceFailed || gControllerOutputMode == INX_CONTROLLER_OUTPUT_MODE_DISABLED){
		//ignore because we have a fault
	}else{
		if(base){
			#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_FRDMK64EVAL
				GPIO_PortSet(base, 1U << mask);
			#else
				GPIO_PortClear(base, 1U << mask);
			#endif
			ok=true;
		}
	}
	return ok;
}

static uint64_t string_hash(const char *s, const uint64_t length, const uint64_t seed)
{
	uint64_t hash = seed;
	while (*s) {
		hash = (hash * PRIME_1) ^ (s[0] * PRIME_2);
		s++;
	}
	return hash % length;
}

static void updateDisplayId(){
	int index = 0;
	int max_hash_len = 10;
	int hashLength = strlen(gHashedClientId);
	int zeroCount = max_hash_len - hashLength;
	for(int i = 0; (i < max_hash_len) && (index < DISPLAY_CLIENT_ID_MAX_LENGTH); i++){
		// add zeros in the front
		if(i < zeroCount){
			clientId[index] = '0';
		}else{
			clientId[index] = gHashedClientId[i - zeroCount];
		}
		index++;
		// add '-' after 2nd and 7th char to obtain a
		// following format XX-XXXX-XXXX
		if(index == 2 || index == 7){
			clientId[index] = '-';
			index++;
		}
	}
}

static INX_Provison_State getProvisionState(){
	INX_Provison_State provisionState=PROVISION_STATE_UNPROVISIONED;
	if(gProvisioned && gOrganisationID != UNKNOWN_ORGANISATION_ID){
		provisionState=PROVISION_STATE_PROVISIONED; // device is fully provisioned
	}else if(gProvisioned){
		provisionState=PROVISION_STATE_SEMI_PROVISIONED; // device is partially provisioned (without an organisation)
	}else{
		// do nothing - device is not provisioned
	}
	return provisionState;
}

/* Gloval varible getters */
uint32_t mainGetFlashSectorSize(){
	return gFlashSectorSize;
}

float mainGetDesiredTemp(){
	return gPIDs[0].desiredTemp;
}

char* mainGetGlobalClientId(){
	return clientId;
}

float mainGetLowerAlarmTemp(){
	return gPIDs[0].lowerAlarmTemp;
}

float mainGetUpperAlarmTemp(){
	return gPIDs[0].upperAlarmTemp;
}

float mainGetElementRatingKW(){
	return gElementRatingKW;
}

float mainGetPID1_KP(){
	return gPIDs[0].coefficients.kp;
}

float mainGetPID1_KI(){
	return gPIDs[0].coefficients.ki;
}

float mainGetPID1_KD(){
	return gPIDs[0].coefficients.kd;
}

char* mainGetLocalName(){
	return gLocalName;
}

char* mainGetLocation(){
	return gLocation;
}

/*!
 * @brief Gets the Watchdog timer output.
 *
 * @param base WDOG peripheral base address
 * @return Current value of watchdog timer counter.
 */
static inline uint32_t GetTimerOutputValue(WDOG_Type *base)
{
    return (uint32_t)((((uint32_t)base->TMROUTH) << 16U) | (base->TMROUTL));
}

static void inxSettingsChanged(const bool byUser){
	if(byUser){
		gSettingsUpdatedByUser=true;
	}else{
		gSendSettings=true;
	}
}

static void inxSetPIDRampDesired(inxPID_t* pPID,float newDesired,const bool force){
	if(newDesired>pPID->desiredTemp){
		newDesired=pPID->desiredTemp;
	}
	if(pPID->rampDesiredTemp==newDesired && force==false){
		//ignore same value
	}else{
		pPID->rampDesiredTemp=newDesired;
		pPID->rampSecondsAtNextIncrement=inxRTCGetSeconds()+pPID->rampSecondsPerDegree;
		gDynamicSetPointUpdated=true;
		inxSettingsChanged(false);
	}
}

static void inxSetPIDRamp(const bool byUser,inxPID_t* pPID,const uint32_t secondsPerDegree){
	if(secondsPerDegree==pPID->rampSecondsPerDegree){
		//ignore same value
	}else{
		pPID->rampSecondsPerDegree=secondsPerDegree;
		inxSetPIDRampDesired(pPID,pPID->rampDesiredTemp,true);
		inxSettingsChanged(byUser);
	}
}

//I think we do need to erase first as flash_program fails if you haven't erased that bit first
//loadSettingsFromFlash checks out validation value e.g 0xdeadbeef
//TODO we should do the byte padding for people calling us
status_t writeToFlash(void* pMem,size_t size,uint32_t offset,const bool useLower,const uint32_t base){
	uint8_t* toWrite=(uint8_t*)pMem;
	SW_DIAG_printf("writeToFlash %x size=%d base=%x\r\n",pMem,size,base);
	#if CONFIG_OTA_DIAG
		return kStatus_Success;
	#endif //CONFIG_OTA_DIAG
	/*for(uint32_t i=0;i<size;i++){
		SW_DIAG_printf("%d",toWrite[i]);
	}
	SW_DIAG_printf("\r\n");*/
	WDOG_Refresh(wdog_base); //we have to kick the watch dog on entrance and exit as we don't want to be shut down during a write
	taskENTER_CRITICAL(); //this has to be here or we end restarting the board when we actually try to do the flash write

	uint32_t flashAddr=base;
	if(useLower){
		//we are already in lower program flash
	}else{
		//we need to write this to the other half of program flash
		flashAddr=flashAddr+(gFlashTotalSize/2);
	}
	flashAddr=flashAddr+(offset*16);
	//LWIP_PLATFORM_DIAG(("writeToFlash base=0x%X flashAddr=0x%X size=%d",base,flashAddr,size));

	status_t result;
	size_t bytesLeftToWrite=size;
	
	volatile uint8_t* written=(uint8_t*)flashAddr;
	size_t currByte = 0;
	while(bytesLeftToWrite>0){
		size_t numBytesToWrite=size; //for now always try to write all of our data in one go
		if(numBytesToWrite>bytesLeftToWrite){
			numBytesToWrite=bytesLeftToWrite;
		}
		result=flash_mem_erase((uint32_t)&written[currByte],numBytesToWrite);
		if(result!=kStatus_Success){
			printf("PBB failed to erase sector %x %d\r\n",(uint32_t)&written[currByte],numBytesToWrite);
			goto writeToFlashExit;
		}
		result=flash_mem_write((uint32_t)&written[currByte],numBytesToWrite,&toWrite[currByte]);
		if(result!=kStatus_Success){
			printf("PBB failed to write sector\r\n");
			goto writeToFlashExit;
		}
		bytesLeftToWrite=bytesLeftToWrite-numBytesToWrite;
		currByte=currByte+numBytesToWrite;
	}


/*
	uint32_t failAddr, failDat;
	// Pre-preparation work about flash Cache/Prefetch/Speculation.
	FTFx_CACHE_ClearCachePrefetchSpeculation(g_bootloaderContext.allFlashCacheState, true);
	result = FLASH_Erase(g_bootloaderContext.allFlashState, flashAddr, size, kFTFx_ApiEraseKey);
	if (kStatus_FTFx_Success != result)
	{
		LWIP_PLATFORM_DIAG(("flash erase error\n"));
	}

	// Verify sector if it's been erased.
	result = FLASH_VerifyErase(g_bootloaderContext.allFlashState, flashAddr, size, kFTFx_MarginValueUser);
	if (kStatus_FTFx_Success != result)
	{
		LWIP_PLATFORM_DIAG(("verify erase error\n"));
	}

	// Program user buffer into flash
	//ProgramSection uses the flex ram buffer for speed, FLASH_Program is apparently slower
	result = FLASH_ProgramSection(g_bootloaderContext.allFlashState, flashAddr, (uint8_t *)pMem, size);
	if (kStatus_FTFx_Success != result)
	{
		LWIP_PLATFORM_DIAG(("writeToFlash failed FLASH_Program"));
		taskEXIT_CRITICAL();
		return result;
	}

	// Verify programming by Program Check command with user margin levels
	result = FLASH_VerifyProgram(g_bootloaderContext.allFlashState, flashAddr, size, (const uint8_t *)pMem, kFTFx_MarginValueUser,
								 &failAddr, &failDat);
	if (kStatus_FTFx_Success != result)
	{
		LWIP_PLATFORM_DIAG(("writeToFlash FLASH_VerifyProgram failed"));
	}

	// Post-preparation work about flash Cache/Prefetch/Speculation.
	FTFx_CACHE_ClearCachePrefetchSpeculation(g_bootloaderContext.allFlashCacheState, false);

	#if defined(FSL_FEATURE_HAS_L1CACHE) && FSL_FEATURE_HAS_L1CACHE
			L1CACHE_InvalidateCodeCache();
	#endif // FSL_FEATURE_HAS_L1CACHE

	#if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT
			// Clean the D-Cache before reading the flash data
			SCB_CleanInvalidateDCache();
	#endif
*/
	// Verify programming by reading back from flash directly
	if(memcmp(pMem,(void*)flashAddr,size)!=0){
		printf("PBB writeToFlash memcmp failed\n");
		for(currByte=0;currByte<size;currByte++){
			if(toWrite[currByte]!=written[currByte]){
				//LWIP_PLATFORM_DIAG(("currByte=%d toWrite=%d written=%d",currByte,toWrite[currByte],written[currByte]));
			}
		}
		result=1;
	}else{
		printf("PBB Successfully Programmed and Verified Location 0x%x -> 0x%x\n", flashAddr,(flashAddr + size));
	}
	writeToFlashExit:
		taskEXIT_CRITICAL();
		WDOG_Refresh(wdog_base);
		return result;
}

static void inxSettingsMigrate(const permStorage_t* pSettings){
	SW_DIAG_printf("inxSettingsMigrate %x\r\n",pSettings);
	inxNoFactoryResetStorage_t noReset={0};
	//copy to gSettingsBuffer as we can't edit a flash address directly
	memcpy((void*)&gSettingsBuffer,pSettings,sizeof(gSettingsBuffer));
	//copy the relevant values from general settings to non reset
	noReset.versionNumber=CONFIG_NO_RESET_VERSION_NUMBER;
	noReset.pt1001Offset=pSettings->cal1;
	gSettingsBuffer.cal1=0;
	noReset.pt1001Scale=pSettings->cal2;
	gSettingsBuffer.cal2=0;

	//write the settings to the correct place
	memset(&gSettingsToWrite[0],0,sizeof(gSettingsToWrite));
	memcpy(&gSettingsToWrite[0],&noReset,sizeof(inxNoFactoryResetStorage_t));
	memcpy(&gSettingsToWrite[INX_SETTINGS_BYTES_RESERVED_FOR_NON_RESET_STORAGE],(void*)&gSettingsBuffer,sizeof(permStorage_t));
		
	writeToFlash(&gSettingsToWrite,sizeof(gSettingsToWrite),0,true,(const uint32_t)pSettings);
}

static int8_t readSettingsFromFlash(permStorage_t* pDest,volatile inxNoFactoryResetStorage_t* pNoResetDest,u8_t** ppStoredSchedule){
	uint32_t largestWriteId=0;
	int8_t sectorIndexWithLargestWriteId=-1;
	uint8_t currentSector=0;
	uint32_t addr;
	permStorage_t* pSettings;

	//migrate any sectors that are still in the old storage format
	for(currentSector=0;currentSector<CONFIG_NUMBER_OF_FLASH_SECTORS_FOR_SETTINGS;currentSector++){
		addr=gSettingsStorageFirstSectorAddress+(currentSector*gFlashSectorSize);
		pSettings=(permStorage_t*)(addr);
		//LWIP_PLATFORM_DIAG(("SECTOR[%d] VALIDATION CODE=%X (WrieID=%d)\n",currentSector,pSettings->validationCode,pSettings->writeId));
		if(pSettings->validationCode==INX_FLASH_VALIDATION_CODE){
			inxSettingsMigrate(pSettings);
		}
	}

	//read each of our settings sectors, use the one with the largestWriteId
	for(currentSector=0;currentSector<CONFIG_NUMBER_OF_FLASH_SECTORS_FOR_SETTINGS;currentSector++){
		addr=gSettingsStorageFirstSectorAddress+(currentSector*gFlashSectorSize);
		inxNoFactoryResetStorage_t* noResetSettings=(inxNoFactoryResetStorage_t*)addr;
		addr=addr+INX_SETTINGS_BYTES_RESERVED_FOR_NON_RESET_STORAGE;
		pSettings=(permStorage_t*)(addr);
		//LWIP_PLATFORM_DIAG(("SECTOR[%d] VALIDATION CODE=%X (WrieID=%d)\n",currentSector,pSettings->validationCode,pSettings->writeId));
		if(noResetSettings->versionNumber==CONFIG_NO_RESET_VERSION_NUMBER && pSettings->validationCode==INX_FLASH_VALIDATION_CODE){
			if(noResetSettings->writeId > largestWriteId){
				largestWriteId=noResetSettings->writeId;
				sectorIndexWithLargestWriteId=currentSector;
				SW_DIAG_printf("found currentSector=%d writeId=%d at %x\r\n",currentSector,largestWriteId,addr);
			}
		}
	}
	if (sectorIndexWithLargestWriteId < 0) {
		//try to find some storage with a valid no reset but out of date regular settings
		for(currentSector=0;currentSector<CONFIG_NUMBER_OF_FLASH_SECTORS_FOR_SETTINGS;currentSector++){
			addr=gSettingsStorageFirstSectorAddress+(currentSector*gFlashSectorSize);
			inxNoFactoryResetStorage_t* noResetSettings=(inxNoFactoryResetStorage_t*)addr;
			//LWIP_PLATFORM_DIAG(("SECTOR[%d] VALIDATION CODE=%X (WrieID=%d)\n",currentSector,pSettings->validationCode,pSettings->writeId));
			if(noResetSettings->versionNumber==CONFIG_NO_RESET_VERSION_NUMBER){
				if(noResetSettings->writeId > largestWriteId){
					largestWriteId=noResetSettings->writeId;
					sectorIndexWithLargestWriteId=currentSector;
					SW_DIAG_printf("found currentSector=%d writeId=%d at %x\r\n",currentSector,largestWriteId,addr);
				}
			}
		}
		if(sectorIndexWithLargestWriteId>-1){
			addr=gSettingsStorageFirstSectorAddress+(sectorIndexWithLargestWriteId*gFlashSectorSize);
			memcpy((void*)pNoResetDest,(void*)addr,sizeof(inxNoFactoryResetStorage_t));	
		}
	}else{
		addr=gSettingsStorageFirstSectorAddress+(sectorIndexWithLargestWriteId*gFlashSectorSize);
		//get the schedule, schedule is always the next sector of flash
		u8_t scheduleIndex=sectorIndexWithLargestWriteId+1;
		if(scheduleIndex>=CONFIG_NUMBER_OF_FLASH_SECTORS_FOR_SETTINGS){
			scheduleIndex=0;
		}
		*ppStoredSchedule=(u8_t*)gSettingsStorageFirstSectorAddress+(scheduleIndex*gFlashSectorSize);
		//assume first bytes are the non resettable storage
		memcpy((void*)pNoResetDest,(void*)addr,sizeof(inxNoFactoryResetStorage_t));
		//assume the bit in the middle is regular storage
		addr=addr+INX_SETTINGS_BYTES_RESERVED_FOR_NON_RESET_STORAGE;
		memcpy(pDest,(void*)addr,sizeof(permStorage_t));
	}
	if (sectorIndexWithLargestWriteId > -1 ) {
		
	} else {
		LWIP_PLATFORM_DIAG(("ERROR: COULD NOT READ FROM FLASH!"));
	}
	return sectorIndexWithLargestWriteId;
}

#if INX_HEATROD_EHS
u8_t inxSODLFlashSectorIndex=0;
inxTSDL* inxSODLGetWritePointer(){
	return (inxTSDL*)gOTAUpdateOneSectorBuffer;
}
uint32_t inxSODLGetSectorAddress(const u8_t sectorIndex){
	return (gSettingsStorageFirstSectorAddress-(gFlashSectorSize*CONFIG_EHS_MAX_SODL_SECTORS))+(sectorIndex*gFlashSectorSize);
}
uint32_t inxEHSPermanentStorageAddress(){
	return inxSODLGetSectorAddress(0)-gFlashSectorSize;
}

typedef struct {
	uint16_t id;
	uint16_t valueSizeBytes;
} inxEHSPermanentStorageEntry;
bool inxEHSPermanentStorageGetValue(const uint16_t id,void* pValue){
	uint32_t flashAddress=inxEHSPermanentStorageAddress();
	inxEHSPermanentStorageEntry* pEntry=(inxEHSPermanentStorageEntry*)flashAddress;
	while(pEntry->id!=0){
		u8_t* pValueStored=(u8_t*)&pEntry->valueSizeBytes;
		pValueStored=pValueStored+sizeof(pEntry->valueSizeBytes);
		//void* pValueStored=()+(-1);
		//u8_t* pValueStoredBytes=(u8_t*)pValueStored;
		if(pEntry->id==id){
			memcpy(pValue,pValueStored,pEntry->valueSizeBytes);
			return true;
		}
		flashAddress=flashAddress+sizeof(inxEHSPermanentStorageEntry)+pEntry->valueSizeBytes;
		pEntry=(inxEHSPermanentStorageEntry*)flashAddress;
	}
	return false;
}

char* inxSODLGetReadPointer(){
	inxEHSFile_t file=inxEHSFilesFind("t.sdl");
	return (char*)inxEHSFilesGetData(file);
}

static void inxEHSPermanentStorageWrite(){
	writeToFlash(&gSettingsToWrite,sizeof(gSettingsToWrite),0,true,inxEHSPermanentStorageAddress());
}
static void saveEHSSettingsToFlash(const bool useLower,const u8_t sectorIndex){
	inxTSDL* pSDL=inxSODLGetWritePointer();
	uint32_t sectorAddress=inxSODLGetSectorAddress(sectorIndex);
	//get the address of the sector before our regular settings
	memset(&gSettingsToWrite,0,sizeof(gSettingsToWrite));
	memcpy(&gSettingsToWrite,pSDL,sizeof(inxTSDL));
	writeToFlash(&gSettingsToWrite,sizeof(gSettingsToWrite),0,useLower,sectorAddress);
}
void inxWriteSODLBuffer(){
	saveEHSSettingsToFlash(true,inxSODLFlashSectorIndex);
	inxSODLFlashSectorIndex++;
	if(inxSODLFlashSectorIndex>=CONFIG_EHS_MAX_SODL_SECTORS){
		inxSODLFlashSectorIndex=0;
	}
}
void inxEHSCopyDefaultToFlash(){
	ehs_FILE* fp=EhsFopen("t.sdl","w");
	EhsFwrite((void*)inx_default_sodl,1,strlen((const char*)inx_default_sodl)+1,fp);
	EhsFclose(fp);
}
#endif //INX_HEATROD_EHS

// todo @patrick - if we ever set the gControllerOutputMode = to INX_CONTROLLER_OUTPUT_MODE_DISABLES we should try to write this to flash. See search for setting this flag.. to find out where.

static void saveSettingsToFlash(const bool useLower,const uint32_t crcValue,const uint32_t programSizeBytes){
#if 1
	gTicksSinceSettingsSave=0;
	/*size_t settingsSize=sizeof(settings);
	UBaseType_t uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );
	printf("saveSettingsToFlash %d\n",uxHighWaterMark);*/
	//LWIP_PLATFORM_DIAG(("saveSettingsToFlash"));

	//so we can get rid of volatile warnings
	//assume nothing will modify settings buffer whilst we are saving todo - can we assume this? IS this mutexed?
	permStorage_t* pSettingsBuffer=(permStorage_t*)&gSettingsBuffer;

	//find out which sector we should write to
	//we give pSettingsBuffer twice since we don't care what it gives back
	//we just want the sector index
	u8_t* pScheduleStored=0;
	int8_t settingsSectorIndex=readSettingsFromFlash(pSettingsBuffer,(volatile inxNoFactoryResetStorage_t*)pSettingsBuffer,&pScheduleStored);
	settingsSectorIndex=settingsSectorIndex+1;
	if(settingsSectorIndex>=CONFIG_NUMBER_OF_FLASH_SECTORS_FOR_SETTINGS){
		settingsSectorIndex=0;
	}
	u8_t scheduleSettingsSectorIndex=settingsSectorIndex+1;
	if(scheduleSettingsSectorIndex>=CONFIG_NUMBER_OF_FLASH_SECTORS_FOR_SETTINGS){
		scheduleSettingsSectorIndex=0;
	}
	
	//write out the non factory reset stuff first
	gNoResetSettings.versionNumber=CONFIG_NO_RESET_VERSION_NUMBER;
	gNoResetSettings.internalRelay1OnCount=dutyCycleState0.internalOnCount;
	gNoResetSettings.internalRelay2OnCount=dutyCycleState1.internalOnCount;
	gNoResetSettings.internalRelay3OnCount=dutyCycleState2.internalOnCount;
	gNoResetSettings.externalRelay1OnCount=dutyCycleState0.externalOnCount;
	gNoResetSettings.externalRelay2OnCount=dutyCycleState1.externalOnCount;
	gNoResetSettings.externalRelay3OnCount=dutyCycleState2.externalOnCount;
	gSettingsWriteId=gSettingsWriteId+1;
	gNoResetSettings.writeId=gSettingsWriteId;
	SW_DIAG_printf("save settings counts to %d,%d,%d,%d,%d,%d\r\n",gNoResetSettings.externalRelay1OnCount,gNoResetSettings.internalRelay1OnCount,gNoResetSettings.externalRelay2OnCount,gNoResetSettings.internalRelay2OnCount,gNoResetSettings.externalRelay3OnCount,gNoResetSettings.internalRelay3OnCount);
	memset(&gSettingsToWrite,0,sizeof(gSettingsToWrite));
	memcpy(&gSettingsToWrite,(void*)&gNoResetSettings,sizeof(gNoResetSettings));
	//make sure the buffer we are using is going to be big enough
	BUILD_BUG_ON( INX_SETTINGS_BYTES_RESERVED_FOR_NON_RESET_STORAGE > sizeof(gSettingsToWrite) );

	//now write out all the other settings
	pSettingsBuffer->validationCode=INX_FLASH_VALIDATION_CODE;
	pSettingsBuffer->provisioned=gProvisioned;
	pSettingsBuffer->organisationID=gOrganisationID;
	pSettingsBuffer->crcValue=crcValue;
	pSettingsBuffer->programSizeBytes = programSizeBytes;
	pSettingsBuffer->networkDhcp=gDHCP;
	memcpy(&pSettingsBuffer->errorLogs,&gErrorLogs,sizeof(pSettingsBuffer->errorLogs));
	pSettingsBuffer->diagnosticsDone=gDiagnosticsDone;
	strncpy(pSettingsBuffer->localName,gLocalName,CONFIG_LOCAL_NAME_LENGTH);
	strncpy(pSettingsBuffer->location,gLocation,CONFIG_LOCATION_LENGTH);
	pSettingsBuffer->rtcDisabled=gRTCDisabled;
	pSettingsBuffer->cal1=gCal1;
	pSettingsBuffer->cal2=gCal2;
	pSettingsBuffer->cal3=gCal3;

	//before saving the settings we need to calculate a CRC value for them
	pSettingsBuffer->settingsCrcValue=0;
	pSettingsBuffer->size=sizeof(*pSettingsBuffer);
	inxIecSettingsCRC(pSettingsBuffer);
	memcpy(&gSettingsToWrite[INX_SETTINGS_BYTES_RESERVED_FOR_NON_RESET_STORAGE],pSettingsBuffer,sizeof(gSettingsBuffer));
	uint32_t sectorAddress=(settingsSectorIndex*gFlashSectorSize)+gSettingsStorageFirstSectorAddress;
	writeToFlash(&gSettingsToWrite,sizeof(gSettingsToWrite),0,useLower,sectorAddress);
	//LWIP_PLATFORM_DIAG(("saveSettingsToFlash done"));
//#error "Not wanting to compiling today"
#else
#warning "Writing settings to flash has been disabled!!!!!!!!!!!"
#endif
}

void inxSetPIDCoefficients(float* coefficient,const float value){
	*coefficient = value;
	gSettingsUpdatedByUser=true;
}

void inxSetPowerMode(const bool byUser,const uint16_t mode){
	if(mode==gPowerMode){
		//ignore same setting
	}else{
		switch(mode){
			case 0:
				gPowerMode=INX_POWER_MODE_THREE_PHASE_415;
				break;
			case 1:
				gPowerMode=INX_POWER_MODE_THREE_PHASE_240;
				break;
			case 2:
				gPowerMode=INX_POWER_MODE_SINGLE_PHASE_240;
				break;
			case 3:
				gPowerMode=INX_POWER_MODE_INDEPENDENT_CONNECTION;
				break;
		}
		inxSettingsChanged(byUser);
	}
}

static GPIO_Type* inxAuxGetGPIOBase(){
	GPIO_Type* base=NULL;
	switch(gAuxControlOutput){
		case INX_AUX_CONTROL_1:
			base=CONFIG_HEATER_ONE_GPIO_BASE;
			break;
		case INX_AUX_CONTROL_2:
			base=CONFIG_HEATER_TWO_GPIO_BASE;
			break;
		case INX_AUX_CONTROL_3:
			base=CONFIG_HEATER_THREE_GPIO_BASE;
			break;
		case INX_AUX_CONTROL_4:
			base=CONFIG_HEATER_ONE_GPIO_BASE;
			break;
		case INX_AUX_CONTROL_5:
			base=CONFIG_HEATER_TWO_GPIO_BASE;
			break;
		case INX_AUX_CONTROL_6:
			base=CONFIG_HEATER_THREE_GPIO_BASE;
			break;
		default:
			break;
	}
	return base;
}

static uint32_t inxAuxGetGPIOPin(){
	uint32_t pin=0;
	switch(gAuxControlOutput){
		case INX_AUX_CONTROL_1:
			pin=CONFIG_GPIO_CONTROL_INTERNAL_RELAY_1;
			break;
		case INX_AUX_CONTROL_2:
			pin=CONFIG_GPIO_CONTROL_INTERNAL_RELAY_2;
			break;
		case INX_AUX_CONTROL_3:
			pin=CONFIG_GPIO_CONTROL_INTERNAL_RELAY_3;
			break;
		case INX_AUX_CONTROL_4:
			pin=CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_1;
			break;
		case INX_AUX_CONTROL_5:
			pin=CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_2;
			break;
		case INX_AUX_CONTROL_6:
			pin=CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_3;
			break;
		default:
			break;
	}
	return pin;
}

void inxSetAuxControlOutput(const bool byUser,const uint16_t controlOutput){
	if(controlOutput==gAuxControlOutput){
		//ignore change
	}else{
		//validate change is in range
		if(controlOutput>=INX_AUX_DISABLED && controlOutput<=INX_AUX_CONTROL_6){
			//make sure it is off
			gAuxiliary=0;
			inxGPIORelayOff(inxAuxGetGPIOBase(),inxAuxGetGPIOPin());
			gAuxControlOutput=INX_AUX_DISABLED;
			bool ok=false;
			switch(controlOutput){
				case INX_AUX_CONTROL_1:
					if(gControllerOutputMode!=INX_CONTROLLER_OUTPUT_MODE_INTERNAL_RELAY){
						ok=true;
					}
					break;
				case INX_AUX_CONTROL_2:
					if(gControllerOutputMode!=INX_CONTROLLER_OUTPUT_MODE_INTERNAL_RELAY || (gPIDs[0].elementMode==ELEMENT_MODE_SINGLE)){
						ok=true;
					}
					break;
				case INX_AUX_CONTROL_3:
					if(	gControllerOutputMode!=INX_CONTROLLER_OUTPUT_MODE_INTERNAL_RELAY 
						|| (gPIDs[0].elementMode==ELEMENT_MODE_SINGLE) 
						|| (gPIDs[0].elementMode==ELEMENT_MODE_DOUBLE_STEPPED)
						|| (gPIDs[0].elementMode==ELEMENT_MODE_DOUBLE_TANDEM)){
						ok=true;
					}
					break;
				case INX_AUX_CONTROL_4:
					if(gControllerOutputMode!=INX_CONTROLLER_OUTPUT_MODE_EXTERNAL_RELAY){
						ok=true;
					}
					break;
				case INX_AUX_CONTROL_5:
					if(	gControllerOutputMode!=INX_CONTROLLER_OUTPUT_MODE_EXTERNAL_RELAY
						|| gPIDs[0].elementMode==ELEMENT_MODE_SINGLE){
						ok=true;
					}
					break;
				case INX_AUX_CONTROL_6:
					if(	gControllerOutputMode!=INX_CONTROLLER_OUTPUT_MODE_EXTERNAL_RELAY
						|| gPIDs[0].elementMode==ELEMENT_MODE_SINGLE
						|| gPIDs[0].elementMode==ELEMENT_MODE_DOUBLE_STEPPED
						|| gPIDs[0].elementMode==ELEMENT_MODE_DOUBLE_TANDEM){
						ok=true;
					}
					break;
				default:
					ok=false;
					break;
			}
			//we need to validate that this new setting won't overlap our control system
			if(ok){
				gAuxControlOutput=controlOutput;
			}
			inxSettingsChanged(byUser);
		}
	}
}

void inxSetSafetyMode(const bool byUser,const inxSafetyMode_t mode){
	if(mode==gSafetyMode){
		//ignore
	}else{
		switch(mode){
			case INX_SAFETY_MODE_EN60335:
			case INX_SAFETY_MODE_ADDITIONAL_CHECKS:
			case INX_SAFETY_MODE_DISABLE_SLOW_START_CHECKS:
			case INX_SAFETY_MODE_DISABLE_ALL_HARDWARE_CHECKS:
				gSafetyMode=mode;
				inxSettingsChanged(byUser);
				break;
			default:
				LWIP_PLATFORM_DIAG(("inxSetSafetyMode: unrecognised mode %d",mode));
				break;
		}
	}
}

void inxSetControllerSensors(const bool byUser,inxPID_t* pPID,const inxControllerSensors_t in){
	if(in==pPID->sensors){
		//ignore same value
	}else if(in>=INX_CONTROLLER_SENSORS_NONE && in<=INX_CONTROLLER_SENSORS_THERMOCOUPLE){
		pPID->sensors=in;
		inxSettingsChanged(byUser);
	}else{
		//ignore unrecognised value
	}
}

void inxSetPIDTimerDuration(const bool byUser,inxPID_t* pPID,const uint32_t seconds,const bool trigger){
	if(seconds==pPID->timerDurationSeconds){
		//ignore same value
	}else{
		pPID->timerDurationSeconds=seconds;
		inxSettingsChanged(byUser);
	}
	if(trigger){
		switch(pPID->timerState){
			case INX_TIMER_STATE_FINISHED:
			case INX_TIMER_STATE_READY:
				switch(pPID->scheduleMode){
					case SCHEDULE_MODE_TIMER_SET_POINT:
						//set desired temp as it will currently be at 0
						updateDesiredTemp(pPID,pPID->manualSetPoint);
						//set duration so we show the correct time on screen
						pPID->timerSecondsRemaining=pPID->timerDurationSeconds;
						//tell the controller to start getting up to temp
						pPID->timerState=INX_TIMER_STATE_GOING_TO_SET_POINT;
						//tell the ADCs to reset the duty cycle so heating starts immediately
						gControllerOutputUpdated=true;
						break;
					default:
						pPID->timerState=INX_TIMER_STATE_START;
						break;
				}
				break;
			default:
				break;
		}
	}
}

void inxSetElementMode(const bool byUser,inxPID_t* pPID,const uint16_t mode){
	if(mode==pPID->elementMode){
		//ignore same setting
	}else{
		if(mode>=ELEMENT_MODE_SINGLE && mode<=ELEMENT_MODE_SINGLE_TWO_POLE){
			pPID->elementMode=mode;
			//update our auxiliary mode to make sure no overlap
			inxAuxControlEnum_t auxControlOutput = gAuxControlOutput;
			inxSetAuxControlOutput(byUser,INX_AUX_DISABLED);
			//now it's disabled try turning it back on
			inxSetAuxControlOutput(byUser,auxControlOutput);
			inxSettingsChanged(byUser);
		}else{
			LWIP_PLATFORM_DIAG(("inxSetElementMode: unrecognised mode=%d",mode));
		}
	}
}

void inxResetMode(const uint16_t mode){
	switch(mode){
		case RESET_MODE_NONE:
			//LWIP_PLATFORM_DIAG(("Reset mode request = None"));
			break;
		case RESET_MODE_RESTART_DEVICE:
			//LWIP_PLATFORM_DIAG(("Reset mode request = Restart Device"));
			mainReset();
			break;
		case RESET_MODE_CLEAR_ERRORS_AND_RESET:
			//LWIP_PLATFORM_DIAG(("Reset mode request = Clear Errors and Restart"));
			// @TODO - it always clears err logs when reseting.
			mainReset();
			break;
		case RESET_MODE_FACTORY_RESET:
			//LWIP_PLATFORM_DIAG(("Reset mode request = Factory Reset (Deprovision)"));
			mainFactoryReset();
			break;
		default:
			//LWIP_PLATFORM_DIAG(("inxResetMode: unrecognised mode=%d",mode));
			break;
	}
}

void errorLogsClearSingleError(const errorLogsError_t error){
	//we don't bother sending back cleared errors so we dont update gErrorLogsChanged
	gErrorLogs.errors[error].unixTime=0;
}

err_t errorLogsClear(){
	err_enum_t err=ERR_OK;
	for(errorLogsError_t i=0;i<ERROR_LOGS_END;i++){
		if(gErrorLogs.errors[i].unixTime>0){
			gErrorLogs.errors[i].unixTime=0;
			err=ERR_MEM;
		}
	}
	return err;
}

uint32_t inxRTCGetSecondsAlways(){
	return RTC->TSR;
}

uint32_t inxRTCADCGetSeconds(){
	return gADCRTCLastGoodSeconds+(gADCRTCTicks*gLPTMRInterruptMilliseconds/1000);
}

uint32_t inxEHSGetUSeconds(){
	return gFTMTicks;
}

uint32_t inxRTCGetSeconds(){
		if(gRTCDisabled) {
			HW_DIAG_printf("RTC Is disabled\n");
			return 2;
		}
		else if (!gRTCInitDone) {
			return 0;
		}
		else if (gRTCStuck) {
			return 3;
		}
		else {
	#if CONFIG_RTC_FAKE_BEING_STUCK
	#error "RTC IS FAKED AS STUCK!!!"
			return 1;
	#elif CONFIG_ENABLE_RTC
			return RTC->TSR;
	#else //CONFIG_ENABLE_RTC
	#error "RTC IS DISABLED"
			return 1;
	#endif //CONFIG_ENABLE_RTC
		}
}

//you must use int16_t here are you get incorrect day of week returned
static uint8_t inxDOWGet(int16_t d,int16_t m,int16_t y){
	int dow=((d += m < 3 ? y-- : y - 2, 23*m/9 + d + 4 + y/4- y/100 + y/400)%7);
	return (uint8_t)dow;
}

static time_t uoffset_to_seconds(const uoffset_t *offset)
{
  time_t seconds = offset->hours * 3600;
  seconds += (offset->hours >= 0 ? offset->minutes : -offset->minutes) * 60;
  return seconds;
}

static void inxApplyTimeZone(const char* timezone,rtc_datetime_t* pDate){
	if(pDate->year>1970){
		uzone_t active_zone={0};
		uoffset_t offset={0};
		get_zone_by_name((char*)timezone, &active_zone);
		int weekday=dayofweek(pDate->year,pDate->month,pDate->day);
		if(strlen(active_zone.name)>0){
			udatetime_t dt = {0};
			dt.date.year = pDate->year;
			dt.date.month = pDate->month;
			dt.date.dayofmonth = pDate->day;
			dt.date.dayofweek = weekday;
			dt.time.hour = pDate->hour;
			dt.time.minute = pDate->minute;
			dt.time.second = pDate->second;
			get_current_offset(&active_zone, &dt, &offset);
		}
		uint32_t seconds=inxRTCGetSeconds();
		time_t localSeconds=uoffset_to_seconds(&offset);
		seconds+=localSeconds;
		RTC_ConvertSecondsToDatetime(seconds, pDate);
	}
}

void inxRTCSecondsToDate(const char* timezone,const uint32_t unixSeconds,uint16_t* pYear,uint8_t* pMonth,uint8_t* pDOM,uint8_t* pDOW,uint8_t* pHour,uint8_t* pMinute,uint8_t* pSecond){
	rtc_datetime_t datetime;
	RTC_ConvertSecondsToDatetime(unixSeconds,&datetime);
	inxApplyTimeZone(timezone,&datetime);
	*pYear=datetime.year;
	*pMonth=datetime.month;
	*pDOM=datetime.day;
	*pDOW=inxDOWGet(datetime.day,datetime.month,datetime.year);
	*pHour=datetime.hour;
	*pMinute=datetime.minute;
	*pSecond=datetime.second;
}

void errorLogsWrite(const errorLogsError_t error){
	if(gErrorLogs.locked){
		SW_DIAG_printf("errorLogsWrite:not writing %d because locked\n",error);
	}else{
		gErrorLogs.locked=true;
		//we want to only log the first instance of an error till we have sent it
		if(gErrorLogs.errors[error].unixTime>0){
			SW_DIAG_printf("errorLogsWrite:error already logged\n");
		}else{
			gErrorLogs.errors[error].unixTime=inxRTCGetSeconds();
			if(gErrorLogs.errors[error].unixTime<2){
				gErrorLogs.errors[error].unixTime=2; //minimum time to be reported back to devman is 2
			}
			gErrorLogsUpdated=true;
		}
		gErrorLogs.locked=false;
	}
}

void inxDutyCyclePowerThresholdSet(const bool byUser,volatile dutyCycleState_t* pState,const uint16_t powerPercent){
	pState->powerOnThresholdPercent=powerPercent;
	inxSettingsChanged(byUser);
}

uint32_t dutyCycleStateCalculatePowerPercentFromHistory(const dutyCycleHistory_t* pState,const uint8_t pidNum,const inxPID_t* pPID){
	uint32_t maxTicksOn=0;
	uint32_t percent=0;
	uint32_t i;
	float percentFloat=0;
	switch(pidNum){
		case 0:
			switch(pPID->elementMode){
				case ELEMENT_MODE_SINGLE:
				case ELEMENT_MODE_SINGLE_TWO_POLE:
					for(i=0;i<CONFIG_DUTY_CYCLE_HISTORY_LENGTH;i++){
						maxTicksOn=maxTicksOn+1;
						if(pState->powerHistory[i].duty1On){
							percent++;
						}
					}
					break;
				case ELEMENT_MODE_DOUBLE_STEPPED:
				case ELEMENT_MODE_DOUBLE_TANDEM:
					for(i=0;i<CONFIG_DUTY_CYCLE_HISTORY_LENGTH;i++){
						maxTicksOn=maxTicksOn+2;
						if(pState->powerHistory[i].duty1On){
							percent++;
						}
						if(pState->powerHistory[i].duty2On){
							percent++;
						}
					}
					break;
				case ELEMENT_MODE_TRIPLE_STEPPED:
				case ELEMENT_MODE_TRIPLE_TANDEM:
					for(i=0;i<CONFIG_DUTY_CYCLE_HISTORY_LENGTH;i++){
						maxTicksOn=maxTicksOn+3;
						if(pState->powerHistory[i].duty1On){
							percent++;
						}
						if(pState->powerHistory[i].duty2On){
							percent++;
						}
						if(pState->powerHistory[i].duty3On){
							percent++;
						}
					}
					break;
				default:
					break;

			}
			break;
		case 1:
			switch(pPID->elementMode){
				case ELEMENT_MODE_SINGLE:
				case ELEMENT_MODE_SINGLE_TWO_POLE:
					for(i=0;i<CONFIG_DUTY_CYCLE_HISTORY_LENGTH;i++){
						maxTicksOn=maxTicksOn+1;
						if(pState->powerHistory[i].duty2On){
							percent++;
						}
					}
					break;
				case ELEMENT_MODE_DOUBLE_STEPPED:
				case ELEMENT_MODE_DOUBLE_TANDEM:
					for(i=0;i<CONFIG_DUTY_CYCLE_HISTORY_LENGTH;i++){
						maxTicksOn=maxTicksOn+2;
						if(pState->powerHistory[i].duty2On){
							percent++;
						}
						if(pState->powerHistory[i].duty3On){
							percent++;
						}
					}
					break;
				default:
					break;

			}
			break;
		case 2:
			switch(pPID->elementMode){
				case ELEMENT_MODE_SINGLE:
				case ELEMENT_MODE_SINGLE_TWO_POLE:
					for(i=0;i<CONFIG_DUTY_CYCLE_HISTORY_LENGTH;i++){
						maxTicksOn=maxTicksOn+1;
						if(pState->powerHistory[i].duty3On){
							percent++;
						}
					}
					break;
				default:
					break;

			}
			break;
		default:
			break;
	}

	//prevent divide by zero
	if(maxTicksOn==0){
		maxTicksOn=1;
	}

	percentFloat=((float)percent/(float)maxTicksOn)*(float)100;
	percent=(uint32_t)percentFloat;
	return percent;
}

bool dutyCycleHistoryCopy(dutyCycleHistory_t* pSrc,threadSafeDutyCycleHistory_t* pDst){
	bool success=0;
	if(pDst->locked){

	}else{
		pDst->locked=1;
		memcpy(&pDst->state,pSrc,sizeof(dutyCycleHistory_t));
		pDst->locked=0;
		pDst->ready=1;
		success=1;
	}
	return success;
}

bool dutyCycleStateLog(dutyCycleHistory_t* pState,adcValuesQueueMessage_t message){
	bool full=false;
	if(message.log){
		pState->powerHistory[pState->powerHistoryHead]=message;
		pState->powerHistoryHead++;
		if(pState->powerHistoryHead>=CONFIG_DUTY_CYCLE_HISTORY_LENGTH){
			pState->powerHistoryHead=0;
			full=true;
		}
	}else{
		//ignore messages that are just for control and not for logging
		//log should be true once a second
	}
	return full;
}

/* Stateful Relay Control Layer functions */

void dutyCycleStateOff(volatile dutyCycleState_t* pState){
	pwmcalc_printf("\n====SWITCHING OFF (pstate=%d):\n",pState->on);
	/* DON'T CHECK WE ALWAYS WANT TO ASSERT or RE-ASSERT AN OFF in the offstate just incase
	 * Note the relays have negated output as the switcher board is designed this way
	   todo review if we want an abstraction layer to make the sense of these correct */
	switch(gControllerOutputMode){
		case INX_CONTROLLER_OUTPUT_MODE_EXTERNAL_RELAY:
			inxGPIORelayOff(pState->externalGPIOBase,pState->externalGPIOPin);
			break;
		case INX_CONTROLLER_OUTPUT_MODE_INTERNAL_RELAY:
			inxGPIORelayOff(pState->internalGPIOBase,pState->internalGPIOPin);
			break;
		default:
			inxGPIORelayOff(pState->internalGPIOBase,pState->internalGPIOPin);
			inxGPIORelayOff(pState->externalGPIOBase,pState->externalGPIOPin);
			break;
	}

	/* Do some state handling stuff on state transitions */
	if(pState->on){
		pwmcalc_printf("====pState PIN=%d\n",pState->internalGPIOPin);
		pState->on = false;
		//printf("Pstat=%d\n",);
		pState->ticksSinceChange=0;
		pwmcalc_printf("====dutyCycleState0%d,%d,%d\n",dutyCycleState0.on ,dutyCycleState1.on ,dutyCycleState2.on);
	}
	if(dutyCycleState0.on || dutyCycleState1.on || dutyCycleState2.on){

	}else{
		inxLedDutyModeSet(INX_LED_DUTY_MODE_OFF);
	}
}

static void updateControllerOutputMode(const bool byUser,inxControllerOutputMode_t newMode);

void dutyCycleStateOffForced(volatile dutyCycleState_t* pState){
	pwmcalc_printf("!!!!!!!!!!!!!!!! FORCED RELAYS OFF!!!!!!!!!!!!\n");
	pState->on = true; //if way we are on the dutyCycleStateOff will always turn us off
	dutyCycleStateOff(pState);
}
/* todo EN60XXXX GPIO / relay tests should only be done in internal relay mode */

void dutyCycleStateOn(volatile dutyCycleState_t* pState){
	bool ok=false;
	switch(gControllerOutputMode){
		case INX_CONTROLLER_OUTPUT_MODE_INTERNAL_RELAY:
			if(inxGPIORelayOn(pState->internalGPIOBase,pState->internalGPIOPin)){
				pState->internalOnCount++;
				ok=true;
			}
			break;
		case INX_CONTROLLER_OUTPUT_MODE_EXTERNAL_RELAY:
			if(inxGPIORelayOn(pState->externalGPIOBase,pState->externalGPIOPin)){
				pState->externalOnCount++;
				ok=true;
			}
			break;
		default:
			//do nothing
			SW_DIAG_printf("SW_DIAG - Duty Not handled here\n");
			break;
	}
	if(ok){
		inxLedDutyModeSet(INX_LED_DUTY_MODE_ON);
		pState->on = true;
		pState->ticksSinceChange=0; //reset tick count so that we wait correct time before checking state
	}
}

void dutyCycleStateResetCycle(volatile dutyCycleState_t* pState){
	pState->numInterrupts=pState->cycleInterruptLength;
}

float dutyCycleGetLengthSeconds(){
	float lengthSeconds=0;
	switch(gControllerContactorMode){
		case INX_CONTROLLER_CONTACTOR_MODE_EMR:
			lengthSeconds=gDutyCycleLengthEMRSeconds;
			break;
		case INX_CONTROLLER_CONTACTOR_MODE_SSR:
			//safety check to make sure we don't apply ssr timings to mechanical output
			#if CONFIG_ALLOW_SSR_TIMINGS_ON_INTERNAL
				lengthSeconds=gDutyCycleLengthSSRSeconds;
			#else
				switch(gControllerOutputMode){
					case INX_CONTROLLER_OUTPUT_MODE_INTERNAL_RELAY:
						//we shouldn't be able to get here
						lengthSeconds=gDutyCycleLengthEMRSeconds;
						break;
					default:
						lengthSeconds=gDutyCycleLengthSSRSeconds;
						break;
				}
			#endif
			break;
		default:
			break;
	}
	return lengthSeconds;
}

void dutyCycleStateSetLengthSeconds(volatile dutyCycleState_t* pState){
	float lengthSeconds=dutyCycleGetLengthSeconds();
	//convert seconds to number of expected interrupts
	uint32_t lengthMillis=lengthSeconds*1000;
	pState->cycleInterruptLength=lengthMillis/gLPTMRInterruptMilliseconds;
}

// Not trusted client
// staggerTicks tells us if the relay should shut off early so we can test the TCO
void dutyCycleStateSetPowerPercent(volatile dutyCycleState_t* pState,int32_t powerPercent,const uint32_t staggerTicks){
	powerPercent=(powerPercent<0)?0:powerPercent;
	powerPercent=(powerPercent>100)?100:powerPercent;
	pState->powerPercent=powerPercent;
	pState->powerOffInterruptCount=((pState->cycleInterruptLength*powerPercent)/100);
	if(pState->powerOffInterruptCount>=staggerTicks){
		pState->powerOffInterruptCount=pState->powerOffInterruptCount-staggerTicks;
	}
	pwmcalc_printf("PWM Duty info (from %d/%d)\n",pState->powerOffInterruptCount,pState->cycleInterruptLength,powerPercent);
}

/*  PWM function */
void dutyCycleStateTick(volatile dutyCycleState_t* pState){
	pState->numInterrupts++;
	pState->ticksSinceChange++;/*-Ints=%d,IntLen=%d*/
	pwmcalc_printf("\n IN dCycle #=%d, Off=%d, Loop=%d --- ",pState->numInterrupts, pState->powerOffInterruptCount, pState->cycleInterruptLength);
	pwmcalc_printf(" dCycle[%d]",pState->internalGPIOPin);
	if(pState->numInterrupts >= pState->cycleInterruptLength){
		//reached end of cycle so loop round
		pState->numInterrupts=0;
		pwmcalc_printf(" #I >>>> ");
		//do we need to turn the power on?
		if(pState->numInterrupts < pState->powerOffInterruptCount){
			pwmcalc_printf("__SWITCHING ON__ IN TICK \n");
			dutyCycleStateOn(pState);
		}else{
			pwmcalc_printf("__SWITCHING OFF A__ IN TICK \n");
			dutyCycleStateOff(pState);
		}
	}else if(pState->numInterrupts >= pState->powerOffInterruptCount){
		//we need to power off for this part of the duty cycle
		pwmcalc_printf("<<<< OFF IN TICK\n");
		dutyCycleStateOff(pState);
	}
	else {
		pwmcalc_printf("<<<< NOT HANDLED IN TICK\n");
	}
	// else do nothing?
	pwmcalc_printf("\n OUT dCycle ");
}

static void updateDutyCycleLengths(){
	dutyCycleStateSetLengthSeconds(&dutyCycleState0);
	dutyCycleStateSetLengthSeconds(&dutyCycleState1);
	dutyCycleStateSetLengthSeconds(&dutyCycleState2);
}

static void inxSetDutyCycleLengthEMRSeconds(const bool byUser,const float seconds){
	if(gDutyCycleLengthEMRSeconds==seconds){

	}else{
		gDutyCycleLengthEMRSeconds=seconds;
		gControllerOutputUpdated=true;
		inxSettingsChanged(byUser);
	}
}

static void inxSetDutyCycleLengthSSRSeconds(const bool byUser,const float seconds){
	if(gDutyCycleLengthSSRSeconds==seconds){

	}else{
		gDutyCycleLengthSSRSeconds=seconds;
		gControllerOutputUpdated=true;
		inxSettingsChanged(byUser);
	}
}

extern uint8_t gRamUsedPercent;
void buildStatusPayload(char* payload){
	int ret=snprintf(payload,INX_MQTT_MAX_PAYLOAD_SIZE,
			"{\"c\":%d,"
			"\"total_physmem\":%3.1f,"
			"\"device_system_space_total_KB\":%d,"
			"\"device_user_space_total_KB\":%d,"
			"\"device_user_space_used_KB\":%d,"
			"\"percmem_ehs\":%d,"
			"\"dhcp\":%d,"
			"\"netmask\":\"%u.%u.%u.%u\","
			"\"gateway\":\"%u.%u.%u.%u\","
			"\"percproc_ehs\":\"100\","
			"\"ehs_version\":\""CONFIG_FIRMWARE_VERSION"\","
			"\"controllerModel\":\""CONFIG_MODEL"\","
			"\"sdl_version\":\"0.0.0\","
			"\"ipaddr\":\"%u.%u.%u.%u\"}",
			gMqttConnections,
			(float)((float)CONFIG_HEAP_SIZE_BYTES/(float)1024), //total_physmem
			(gFlashTotalSize/2)/1024, //device_system_space_total_KB
			(gFlashTotalSize/2)/1024, //device_user_space_total_KB
			(gProgramSizeBytes+(CONFIG_NUMBER_OF_FLASH_SECTORS_FOR_SETTINGS * gFlashSectorSize))/1024, //device_user_space_used_KB
			gRamUsedPercent, //percmem_ehs
			gDHCP,
			((u8_t *)&fsl_netif0.netmask)[0],((u8_t *)&fsl_netif0.netmask)[1],((u8_t *)&fsl_netif0.netmask)[2],((u8_t *)&fsl_netif0.netmask)[3],
			((u8_t *)&fsl_netif0.gw)[0],((u8_t *)&fsl_netif0.gw)[1],((u8_t *)&fsl_netif0.gw)[2],((u8_t *)&fsl_netif0.gw)[3],
			((u8_t *)&fsl_netif0.ip_addr)[0],((u8_t *)&fsl_netif0.ip_addr)[1],((u8_t *)&fsl_netif0.ip_addr)[2],((u8_t *)&fsl_netif0.ip_addr)[3]);
	if(ret<0 || ret>=INX_MQTT_MAX_PAYLOAD_SIZE){
		//LWIP_PLATFORM_DIAG(("buildStatusPayload: bad snprintf ret=%d",ret));
	}
}

void mainSetNetworkState(const NETWORK_State_t newState){
	gNetworkState=newState;
}

static void updateGlobalString(char* globalString,const char* newString,const size_t maxLength){
	size_t nameLength=strlen(newString);
	if(nameLength>maxLength){
		nameLength=maxLength;
	}
	strncpy(globalString,newString,nameLength);
	globalString[nameLength]='\0';
}

/*
PBB 2020-07-02 notes on more flexible scheduling system
have an enum for schedule type e.g inxScheduleType_t gScheduleType
first 8 bytes of stored schedule should be the type,
we can make sure it matches our gScheduleType
or we discard the schedule
we can also use CRC checks to make sure schedule is correct

if we wanted to support partial schedule updates
update from devman would be type,slotNumber,data,updatedCRCforSchedule

is there any point in having schedule as array of pointers?
no, basically, because we still have to serialise and deserialise the data from flash
which means it ends up as an array of structs anyway

our settings structure will need to be altered so that the stored schedule type
is the biggest possible so that it doesn't overwrite other parts of the structure
as it changes

*/

static void updateLocalUIMode(const INX_Local_UI_Mode_t mode){
	if(gLocalUIMode==mode){

	}else{
		gLocalUIMode=mode;
		gSendSettings=true;
	}
}

void mainUpdateOrganisationID(const char* data){
	char tempString[CONFIG_ORGANISATION_NAME_LENGTH];
	if(cgi_get_varval(data,"id",tempString,CONFIG_ORGANISATION_NAME_LENGTH)){
		gOrganisationID=atol(tempString);
		if(cgi_get_varval(data,"name",tempString,CONFIG_ORGANISATION_NAME_LENGTH)){
			updateGlobalString(gOrganisationName,tempString,CONFIG_ORGANISATION_NAME_LENGTH);
		}
	}else{
		//assume old style update
		gOrganisationID = atol(data);
	}
	if(gProvisioned==false){
		//if not already provisioned then set safe defaults
		gProvisioned=true;
		updateLocalUIMode(INX_LOCAL_UI_MODE_MOBILE);
	}
	gSettingsUpdatedByUser=true;
}

void updateScheduleMode(const uint8_t pidNum,const bool byUser,inxPID_t* pPID,const Schedule_Mode_t newScheduleMode){
	SW_DIAG_printf("updateScheduleMode old=%d new=%d\r\n",pPID->scheduleMode,newScheduleMode);
	if(newScheduleMode==pPID->scheduleMode){
		//we don't need to do anything
		SW_DIAG_printf("Schedule mode: Not change not doing anythig in updateScheduleMode\r\n");
	}else{
		gWatchDogState.recoverableRTCError = false;
		pPID->scheduleMode=newScheduleMode;
		inxSettingsChanged(byUser);
		switch(newScheduleMode){
			case SCHEDULE_MODE_MANUAL:
				//we need to move to using our set point
				SW_DIAG_printf("XX Disable_scheduleMode = %d\n",pPID->manualSetPoint);
				updateDesiredTemp(pPID,pPID->manualSetPoint);
				inxLedHeatingModeSet(INX_LED_HEATING_MODE_MANUAL);
				break;
			case SCHEDULE_MODE_ENABLED:
				SW_DIAG_printf("XX schedule Mode Waiting for schedule - setting to 0 now\n");
				//go to zero and wait for the schedule to kick in
				updateDesiredTemp(pPID,0.0);
				inxLedHeatingModeSet(INX_LED_HEATING_MODE_SCHEDULE);
				break;
			case SCHEDULE_MODE_TIMER_SET_POINT:
			case SCHEDULE_MODE_TIMER:
				pPID->timerState=INX_TIMER_STATE_FINISHED;
				inxLedHeatingModeSet(INX_LED_HEATING_MODE_TIMER);
				break;
			default:
				SW_DIAG_printf("XX schedule Mode default - setting to 0 now\n");
				updateDesiredTemp(pPID,0.0);
				inxLedHeatingModeSet(INX_LED_HEATING_MODE_MANUAL);
				break;
		}
		if(pidNum==0){
			inxDisplaySetForScheduleMode(newScheduleMode);
		}
	}
}

/* Sets the manual set pint value -todo TBC if this also make the uti revert to manaul mode or
 *
 *if it just stores the new values and publishes it as the manual value.
 *if */

void updateManualSetPoint(const bool byUser,inxPID_t* pPID,const float newManualSetPoint){
	if(newManualSetPoint==pPID->manualSetPoint){
		SW_DIAG_printf("XXXX - Manual Ignoring  %d == %d\n ",newManualSetPoint,pPID->manualSetPoint);
	}else{
		SW_DIAG_printf("XXXX - Manual settin %f from %f\n ",newManualSetPoint,pPID->manualSetPoint);
		pPID->manualSetPoint=newManualSetPoint;
		inxSettingsChanged(byUser);
		switch(pPID->scheduleMode){
			case SCHEDULE_MODE_MANUAL:
				updateDesiredTemp(pPID,pPID->manualSetPoint);
				break;
			default:
				//don't update anything if we are in schedule mode
				break;
		}
	}
}

#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRi || INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
/* assert changes and publish them to things that need to know */
#endif
extern float gTemporarySetPoint; // shoild get rid of this... but might be needed as manual setpoints are button-centric
/* todo we probably need aquliafier if the temperature is a manual or schedule one so wwe can publish it properly to various places that want to know it. */
void updateDesiredTemp(inxPID_t* pPID,const float newDesiredTemp){
	if(newDesiredTemp==pPID->desiredTemp){
		//ignore
	}else{
		pPID->desiredTemp = newDesiredTemp;
		//reset the ramp information
		inxSetPIDRampDesired(pPID,pPID->currentValue+1,false);
		inxSettingsChanged(false);
	}
}
/* todo - should we set these values in modbus also? */
void updateLowerAlarmTemp(const bool byUser,inxPID_t* pPID,const float newDesiredTemp){
	if(pPID->lowerAlarmTemp==newDesiredTemp){

	}else{
		pPID->lowerAlarmTemp=newDesiredTemp;
		inxSettingsChanged(byUser);
	}
}

void updateUpperAlarmTemp(const bool byUser,inxPID_t* pPID,const float newDesiredTemp){
	if(pPID->upperAlarmTemp==newDesiredTemp){

	}else{
		pPID->upperAlarmTemp=newDesiredTemp;
		inxSettingsChanged(byUser);
	}
}

static void updateElementRatingKW(const bool byUser,const float newRatingKW){
	if(gElementRatingKW==newRatingKW){

	}else{
		gElementRatingKW=newRatingKW;
		inxSettingsChanged(byUser);
	}
}

void updatePID1_KP(const bool byUser,inxPID_t* pPID,const float newValue){
	if(pPID->coefficients.kp==newValue){

	}else{
		pPID->coefficients.kp=newValue;
		inxSettingsChanged(byUser);
	}
}

void updatePID1_KI(const bool byUser,inxPID_t* pPID,const float newValue){
	if(pPID->coefficients.ki==newValue){

	}else{
		pPID->coefficients.ki=newValue;
		inxSettingsChanged(byUser);
	}
}

void updatePID1_KD(const bool byUser,inxPID_t* pPID,const float newValue){
	if(pPID->coefficients.kd==newValue){

	}else{
		pPID->coefficients.kd=newValue;
		inxSettingsChanged(byUser);
	}
}

static void updateLocalName(char* newLocalName){
	updateGlobalString(gLocalName,newLocalName,CONFIG_LOCAL_NAME_LENGTH);
	gSendIdentification=true;
}

static void updateLocation(char* newLocation){
	updateGlobalString(gLocation,newLocation,CONFIG_LOCATION_LENGTH);
	gSendIdentification=true;
}

static void updateControllerOutputMode(const bool byUser,inxControllerOutputMode_t newMode){
	//if ( INX_CONTROLLER_OUTPUT_MODE_DISABLED != gControllerOutputMode  && INX_CONTROLLER_OUTPUT_MODE_DISABLED  != newMode) {
	/*todo - There are quite a few other ways relays might get switched on without a doce errors (e.g. of we have an aux outout mapped to the wrong relays,
	but we want any subsequent errors (e.g. tripped TCO or relays status errors)
	Hence we might want to add another state to this mode which is "permenantly" disabled, which can't be got out of without a factory reset or something harsh on the user.
	Or we might (better still) use the system state variable for gateing any DIO control of relays instead of the user configuravble outout mode)
	*/
		if (newMode==gControllerOutputMode){
			//ignore this settings change, unless it is setting the mode to disabled, which we do want to work and trigger any settings write etc.
		}else{
			//shut off all relays before we change mode in case one of them is on
			inxIecShutOffAllRelays(false);
			gControllerOutputMode=newMode;
			inxAuxControlEnum_t auxControlOutput = gAuxControlOutput;
			inxSetAuxControlOutput(byUser,INX_AUX_DISABLED);
			//now it's disabled try turning it back on
			inxSetAuxControlOutput(byUser,auxControlOutput);
			inxSettingsChanged(byUser);
			gControllerOutputUpdated = true;
			switch(newMode){
				case INX_CONTROLLER_OUTPUT_MODE_DISABLED:
					inxIecShutOffAllRelays(false);
				case INX_CONTROLLER_OUTPUT_MODE_MODBUS_ONLY:
				case INX_CONTROLLER_OUTPUT_MODE_EXTERNAL_RELAY:
				case INX_CONTROLLER_OUTPUT_MODE_INTERNAL_RELAY:
					//always disable the auxiliary schedule to prevent accidentally controlling something we shouldn't
					break;
				default:
					//printf("XXXXXXXXXXXXXX  - All wrong!\n");
					SW_DIAG_printf("updateControllerOutputMode: unrecognised mode %d - DISABLING ANYWAY\n",newMode);
					gControllerOutputMode = INX_CONTROLLER_OUTPUT_MODE_DISABLED;
					break;
			}
		}
	//}
}

static void updateControllerContactorMode(const bool byUser,inxControllerContactorMode_t newMode){
	if (newMode==gControllerContactorMode){
		//ignore this settings change
	}else{
		switch(newMode){
			case INX_CONTROLLER_CONTACTOR_MODE_EMR:
			case INX_CONTROLLER_CONTACTOR_MODE_SSR:
				gControllerContactorMode = newMode;
				inxSettingsChanged(byUser);
				gControllerOutputUpdated=true;
				break;
			default:
				LWIP_PLATFORM_DIAG(("updateControllerContactorMode: unrecognised mode %d",newMode));
				break;
		}
	}
}

static void inxSetCalibration1(const bool byUser,const float newValue){
	if(newValue==gCal1){
		//ignore
	}else{
		gCal1=newValue;
		inxSettingsChanged(byUser);
	}
}

static void inxSetCalibration2(const bool byUser,const float newValue){
	if(newValue==gCal2){
		//ignore
	}else{
		gCal2=newValue;
		inxSettingsChanged(byUser);
	}
}

static void inxSetCalibration3(const bool byUser,const float newValue){
	if(newValue==gCal3){
		//ignore
	}else{
		gCal3=newValue;
		inxSettingsChanged(byUser);
	}
}

static void loadSettingsFromFlash(const bool reset){
	int8_t readOK = -1;
	//get rid of volatile errors
	permStorage_t* pSettingsBuffer=(permStorage_t*)&gSettingsBuffer;
	u8_t* pStoredSchedule=0;
	if(reset){
		//LWIP_PLATFORM_DIAG(("RESET NOT - loadingSettingsFromFlash"));
		//don't bother reading from flash on a reset
	}else{
		//LWIP_PLATFORM_DIAG(("loadingSettingsFromFlash"));
		readOK = readSettingsFromFlash(pSettingsBuffer,&gNoResetSettings,&pStoredSchedule);
		//printf("readOK=%d versionNumber=%d\r\n",readOK,gNoResetSettings.versionNumber);
		//evaluate the non reset settings
		if(readOK>-1 && gNoResetSettings.versionNumber==CONFIG_NO_RESET_VERSION_NUMBER){
			//non reset settings ok
			dutyCycleState0.externalOnCount=gNoResetSettings.externalRelay1OnCount;
			dutyCycleState0.internalOnCount=gNoResetSettings.internalRelay1OnCount;
			dutyCycleState1.externalOnCount=gNoResetSettings.externalRelay2OnCount;
			dutyCycleState1.internalOnCount=gNoResetSettings.internalRelay2OnCount;
			dutyCycleState2.externalOnCount=gNoResetSettings.externalRelay3OnCount;
			dutyCycleState2.internalOnCount=gNoResetSettings.internalRelay3OnCount;
			gSettingsWriteId=gNoResetSettings.writeId;
			SW_DIAG_printf("setting counts to %d,%d,%d,%d,%d,%d\r\n",gNoResetSettings.externalRelay1OnCount,gNoResetSettings.internalRelay1OnCount,gNoResetSettings.externalRelay2OnCount,gNoResetSettings.internalRelay2OnCount,gNoResetSettings.externalRelay3OnCount,gNoResetSettings.internalRelay3OnCount);
		}else{
			//they need setting to safe defaults
			memset((void*)&gNoResetSettings,0,sizeof(gNoResetSettings));
			gNoResetSettings.versionNumber=CONFIG_NO_RESET_VERSION_NUMBER;
		}
	}

	err_enum_t result=inxIecSettingsCRC(pSettingsBuffer);
	if((readOK > -1 ) && !reset && gSettingsBuffer.validationCode==INX_FLASH_VALIDATION_CODE 
		&& result==ERR_OK){
		//LWIP_PLATFORM_DIAG(("Validation Code Good!"));
		//gSettingsBuffer are valid use them
	}else{
		//LWIP_PLATFORM_DIAG(("Resetting Config"));
		//initialise defaults
		if(reset){
			//if we are doing a reset then we need to use the current writeId
			gNoResetSettings.writeId=gSettingsWriteId;
		}else{
			gNoResetSettings.writeId=0;
		}
		memset(pSettingsBuffer,0,sizeof(gSettingsBuffer));
		gSettingsBuffer.validationCode=INX_FLASH_VALIDATION_CODE;
		gSettingsBuffer.organisationID=UNKNOWN_ORGANISATION_ID;
		gSettingsBuffer.networkDhcp=true;
		strncpy(pSettingsBuffer->localName,"Name",CONFIG_LOCAL_NAME_LENGTH);
		strncpy(pSettingsBuffer->location,"Location",CONFIG_LOCATION_LENGTH);
	}
	updateLocalName(pSettingsBuffer->localName);
	updateLocation(pSettingsBuffer->location);
	gCrcValue=gSettingsBuffer.crcValue;
	gProgramSizeBytes=gSettingsBuffer.programSizeBytes;
	gDHCP=gSettingsBuffer.networkDhcp;
	gProvisioned=gSettingsBuffer.provisioned;
	gDiagnosticsDone=gSettingsBuffer.diagnosticsDone;
	gOrganisationID=gSettingsBuffer.organisationID;
	gRTCDisabled=gSettingsBuffer.rtcDisabled;
	#ifdef INX_HEATROD_IOT_DIAGNOSTIC_BUILD
		//when in diag we always test RTC
		gRTCDisabled=false;
	#endif //INX_HEATROD_IOT_DIAGNOSTIC_BUILD
	inxSetCalibration1(false,gSettingsBuffer.cal1);
	inxSetCalibration2(false,gSettingsBuffer.cal2);
	inxSetCalibration3(false,gSettingsBuffer.cal3);
	//copy stored error logs
	memcpy(&gErrorLogs,&pSettingsBuffer->errorLogs,sizeof(gErrorLogs));
	
	//LWIP_PLATFORM_DIAG(("loadingSettingsFromFlash done"));
	#if CONFIG_FORCE_PROVISIONED
		gProvisioned=true;
		gLocalUIMode=INX_LOCAL_UI_MODE_FULL_SETTINGS;
		gOrganisationID=1;
	#endif //CONFIG_FORCE_PROVSIONED
}

static bool validateCertificate(char* buffer, char* data, int length, bool bExtractDetails)
{
	if(cgi_get_varval(buffer,"certificate",data,length)){
		cgi_urldecode(data);
		//LWIP_PLATFORM_DIAG(("certificate=%s",data));
		return handleProvisioningCertificate(data, bExtractDetails);
	}
	return false;
}

static bool isPinValid(char* buffer, char* data, int length)
{
	if(cgi_get_varval(buffer,"pin", data, length)){
		cgi_urldecode(data);
		if (strncmp(data, gDevicePin, sizeof(gDevicePin)) == 0){
			return true;
		}
	}
	return false;
}

static bool isPasswordValid(char* buffer, char* data, int length)
{
	if(cgi_get_varval(buffer,"password",data,length)){
		cgi_urldecode(data);
		//LWIP_PLATFORM_DIAG(("password=%s",data));
		// validate password
		if(compareCertificatePassword(data)){
			return true;
		}
	}
	return false;
}

static void httpServerRequestGet(char* reqName, char* data){
	if(strncmp(reqName,"get_network",sizeof("get_network"))==0){
		//send back our network information
		buildStatusPayload(data);
	}else{
		
#if CONFIG_MQTT_ENABLED
		MQTT_State_t mqttState=mqttGetGlobalState();
#else
		MQTT_State_t mqttState=MQTT_STATE_INIT;
#endif
		INX_Provison_State provisionState = getProvisionState();
		//  get system error state
		uint32_t systemErrorState = getSystemErrorState();
		//populate cgi_data with the current state of the system
		//PBB TODO this is getting close to the limit for our cgi_data, need to split up
		int ret=snprintf(data,CONFIG_CGI_DATA_LENGTH_MAX,
			"{\"cal1\":%2.1f,"
			"\"cal2\":%2.1f,"
			"\"cal3\":%2.1f,"
			"\"powerConnection\":%d,"
			"\"td1\":%d,\"td2\":%d,\"td3\":%d,"
			"\"cs1\":%d,"
			"\"cs2\":%d,"
			"\"cs3\":%d,"
			"\"em1\":%d,"
			"\"em2\":%d,"
			"\"em3\":%d,"
			"\"emrMode\":%2.1f,"
			"\"msp1\":%2.1f,"
			"\"msp2\":%2.1f,"
			"\"msp3\":%2.1f,"
			"\"pid_p1\":%.4f,\"pid_i1\":%.4f,\"pid_d1\":%.4f,"
			"\"pid_p2\":%.4f,\"pid_i2\":%.4f,\"pid_d2\":%.4f,"
			"\"pid_p3\":%.4f,\"pid_i3\":%.4f,\"pid_d3\":%.4f,"
			"\"ram1\":%d,\"ram2\":%d,\"ram3\":%d,"
			"\"ssrMode\":%2.1f,"
			"\"controllerContactor\":%d,"
			"\"safetyMode\":%d,"
			"\"p\":%d,"
			"\"sm1\":%d,\"sm2\":%d,\"sm3\":%d,"
			"\"steppedModeElement3Threshold\":%d,\"steppedModeElement2Threshold\":%d,"
			"\"lat1\":%2.1f,\"uat1\":%2.1f,\"lat2\":%2.1f,\"uat2\":%2.1f,\"lat3\":%2.1f,\"uat3\":%2.1f,"
			"\"elementRatingKW\":%2.1f,\"location\":\"%s\",\"localName\":\"%s\",\"mqttState\":%d,"
			"\"pid1\":%3.1f,\"pid2\":%2.1f,"
			"\"power1\":%d,\"pid3\":%3.1f,\"power2\":%d,\"deviceID\":\"%s\",\"controllerModel\":\""CONFIG_MODEL"\",\"com\":%d,\"systemErrors\":%d}",
			gCal1,
			gCal2,
			gCal3,
			gPowerMode,
			gPIDs[0].timerDurationSeconds,gPIDs[1].timerDurationSeconds,gPIDs[2].timerDurationSeconds,
			gPIDs[0].sensors,
			gPIDs[1].sensors,
			gPIDs[2].sensors,
			gPIDs[0].elementMode,
			gPIDs[1].elementMode,
			gPIDs[2].elementMode,
			gDutyCycleLengthEMRSeconds,
			gPIDs[0].manualSetPoint,
			gPIDs[1].manualSetPoint,
			gPIDs[2].manualSetPoint,
			gPIDs[0].coefficients.kp,gPIDs[0].coefficients.ki,gPIDs[0].coefficients.kd,
			gPIDs[1].coefficients.kp,gPIDs[1].coefficients.ki,gPIDs[1].coefficients.kd,
			gPIDs[2].coefficients.kp,gPIDs[2].coefficients.ki,gPIDs[2].coefficients.kd,
			gPIDs[0].rampSecondsPerDegree,gPIDs[1].rampSecondsPerDegree,gPIDs[2].rampSecondsPerDegree,
			gDutyCycleLengthSSRSeconds,
			gControllerContactorMode,
			gSafetyMode,
			provisionState,
			gPIDs[0].scheduleMode,gPIDs[1].scheduleMode,gPIDs[2].scheduleMode,
			dutyCycleState2.powerOnThresholdPercent,dutyCycleState1.powerOnThresholdPercent,
			gPIDs[0].lowerAlarmTemp,gPIDs[0].upperAlarmTemp,gPIDs[1].lowerAlarmTemp,gPIDs[1].upperAlarmTemp,gPIDs[2].lowerAlarmTemp,gPIDs[2].upperAlarmTemp,
			gElementRatingKW,gLocation,gLocalName,mqttState,
			gPIDs[0].currentValue,gPIDs[1].currentValue,
			gPIDs[0].powerPercent,gPIDs[2].currentValue,gPIDs[1].powerPercent,clientId,gControllerOutputMode,systemErrorState);
			
		if(ret<0 || ret>=CONFIG_CGI_DATA_LENGTH_MAX){
			SW_DIAG_printf("bad cgi_data write ret=%d",ret);
			data[0]='\0';
		}
	}
	/*char *c;
	// Replace '+' with spaces.
	while ((c = strchr(data, '+')) != NULL)
	{
		*c = ' ';
	}*/
}

static void httpServerRequestPost(char* reqName, char* buffer, char* data, uint32_t dataSize, void (*callback)(void*, char*), void* respObj){

	if(!gProvisioned && strncmp(reqName,"post_mobile_provision",sizeof("post_mobile_provision"))==0){
		if(validateCertificate(buffer, data, dataSize, /*extract details=*/true)){
			if((gProvisionFailedCount <= MAX_PROVISION_FAILED_COUNT) &&
					isPinValid(buffer, data, dataSize) && isPasswordValid(buffer, data, dataSize)){
				//we now need to send this certificate to devman via mqtt
				//then devman can pair us with the appropriate organisation
#if CONFIG_MQTT_ENABLED
				snprintf(data,CONFIG_CGI_DATA_LENGTH_MAX,"{\"orgID\":%lld,\"expire\":\"%d %d %d %d %d %d\"}",
						 gCertificateOrganisationID, gCertificateExpieryDate.year, gCertificateExpieryDate.mon,
						 gCertificateExpieryDate.day, gCertificateExpieryDate.hour, gCertificateExpieryDate.min,
						 gCertificateExpieryDate.sec);
				mqttSendProvisioningCertificate(data);
				//LWIP_PLATFORM_DIAG((data));
#endif //CONFIG_MQTT_ENABLED
				gProvisionCertificateValidated=true;
				gProvisionFailedCount=0;
				//now respond saying everything went ok
				snprintf(data,CONFIG_CGI_DATA_LENGTH_MAX,"{\"success\":1}");
			}else{
				gProvisionFailedCount++;
				if(gProvisionFailedCount <= MAX_PROVISION_FAILED_COUNT){ // MAX number of attempts
					snprintf(data,CONFIG_CGI_DATA_LENGTH_MAX, "{\"success\":0,\"count\":%d}",gProvisionFailedCount);
				}else{
					// Send zero after reaching the maximum number of attempts.
					snprintf(data,CONFIG_CGI_DATA_LENGTH_MAX,"{\"success\":0,\"count\":0}");
				}
			}
		}else{
			snprintf(data,CONFIG_CGI_DATA_LENGTH_MAX,"Bad certificate.");
		}
		(*callback) (respObj, data); // response callback
	}else if(gProvisionCertificateValidated && strncmp(reqName,"post_mobile_provisioning_information",sizeof("post_mobile_provisioning_information"))==0){
		bool success = cgi_get_varval(buffer,"location",data,dataSize);
		if(success){
			cgi_urldecode(data);
			updateLocation(data);
		}
		success = success && cgi_get_varval(buffer,"localName",data,dataSize);
		if(success){
			cgi_urldecode(data);
			updateLocalName(data);
		}
		success = success && cgi_get_varval(buffer,"localUIMode",data,dataSize);
		if(success){
			cgi_urldecode(data);
			int localUIMode = atoi(data);
			success = (localUIMode >= INX_LOCAL_UI_MODE_MOBILE) && (localUIMode < INX_LOCAL_UI_MODE_COUNT);
			if(success){
				updateLocalUIMode(localUIMode);
			}
		}
		if(success){
			gProvisioned=true;
			gProvisionCertificateValidated=false;
			snprintf(data,CONFIG_CGI_DATA_LENGTH_MAX,"{\"success\":1}");
		}else{
			snprintf(data,CONFIG_CGI_DATA_LENGTH_MAX,"{\"success\":0}");
		}

		(*callback) (respObj, data); // response callback

		gSettingsUpdatedByUser=true;
	}else if(strncmp(reqName,"post_mobile_status",sizeof("post_mobile_status"))==0){
		if(validateCertificate(buffer, data, dataSize, /*extract details=*/false)){
			if(gProvisioned){
				snprintf(data,CONFIG_CGI_DATA_LENGTH_MAX,"{\"status\":1,\"localName\":\"%s\",\"orgID\":\"%lld\"}", gLocalName, gOrganisationID);
			}else{
				snprintf(data,CONFIG_CGI_DATA_LENGTH_MAX,"{\"status\":0}");
			}
			(*callback) (respObj, data); // response callback
		}
	}else if(gProvisioned && strncmp(reqName,"post_network",sizeof("post_network"))==0){
		if(cgi_get_varval(buffer, "dhcp", data, dataSize)){
			cgi_urldecode(data);
			bool dhcp=false;
			if(strncmp(data,"on",sizeof("on"))==0){
				dhcp=true;
			}
			if(!dhcp){
				//read ip address config
				u32_t addr0,addr1,addr2,addr3;
				if(cgi_get_varval(buffer, "ipaddr", data, dataSize)){
					cgi_urldecode(data);
					sscanf(data,"%d.%d.%d.%d",&addr0,&addr1,&addr2,&addr3);
					IP4_ADDR(&gfsl_netif0_ipaddr,addr0,addr1,addr2,addr3);
				}
				if(cgi_get_varval(buffer, "gateway", data, dataSize)){
					cgi_urldecode(data);
					sscanf(data,"%d.%d.%d.%d",&addr0,&addr1,&addr2,&addr3);
					IP4_ADDR(&gfsl_netif0_gw,addr0,addr1,addr2,addr3);
				}
				if(cgi_get_varval(buffer, "netmask", data, dataSize)){
					cgi_urldecode(data);
					sscanf(cgi_data,"%d.%d.%d.%d",&addr0,&addr1,&addr2,&addr3);
					IP4_ADDR(&gfsl_netif0_netmask,addr0,addr1,addr2,addr3);
				}
			}
			gDHCP=dhcp;
			HW_DIAG_printf("do disconnect because network settings changeda\n");
			gNetworkState=NETWORK_STATE_DO_DISCONNECT; // todo - use the function
			gSettingsUpdatedByUser=true;
		}

	}else if(gProvisioned && strncmp(reqName,"post_reset",sizeof("post_reset"))==0){
		//LWIP_PLATFORM_DIAG(("reset post request."));
		if(cgi_get_varval(buffer, "resetMode", data, dataSize)){
			cgi_urldecode(data);
			inxResetMode(atoi(data));
		}
	}else if(gProvisioned && strncmp(reqName,"post_hardware",sizeof("post_hardware"))==0){
		inxUpdateSettings(buffer);
	}else if(gProvisioned && strncmp(reqName,"post",sizeof("post"))==0){
		inxUpdateSettings(buffer);
	}else{
		//LWIP_PLATFORM_DIAG(("unrecognised post request \"%s\"",reqName));
	}
}

void cgi_responseFunction(void* responseObj, char* data){
	HTTPSRV_CGI_RES_STRUCT* response = (HTTPSRV_CGI_RES_STRUCT*)responseObj;
	if(!response){
		// invalid response object
		return;
	}
	response->content_type = HTTPSRV_CONTENT_TYPE_PLAIN;
	response->data = data;
	response->data_length = strlen(data);
	response->content_length = response->data_length;
	HTTPSRV_cgi_write(response);
}

#define CGI_BUFFER_SIZE (sizeof("scheduleMode = steppedModeElement2Threshold = steppedModeElement3Threshold = elementMode = lowerAlarmTemperature = upperAlarmTemperature = manualSetPoint = tempScaleRange = localName = location = ") + CONFIG_CGI_DATA_LENGTH_MAX)
static char gCGIBuffer[CGI_BUFFER_SIZE] = {0};

/* Example Common Gateway Interface callback. */
static int cgi_example(HTTPSRV_CGI_REQ_STRUCT *param)
{
    HTTPSRV_CGI_RES_STRUCT response = {0};

    response.ses_handle = param->ses_handle;
    response.status_code = HTTPSRV_CODE_OK;
    if (param->request_method == HTTPSRV_REQ_GET)
    {
    	httpServerRequestGet(param->script_name, cgi_data);
        response.content_type = HTTPSRV_CONTENT_TYPE_PLAIN;
        response.data = cgi_data;
        response.data_length = strlen(cgi_data);
        response.content_length = response.data_length;
        HTTPSRV_cgi_write(&response);
    }
    else if (param->request_method == HTTPSRV_REQ_POST)
    {
        uint32_t length = 0;
        uint32_t read;
        memset(gCGIBuffer,0,CGI_BUFFER_SIZE);
        length = param->content_length;
        read = HTTPSRV_cgi_read(param->ses_handle, gCGIBuffer, (length > sizeof(gCGIBuffer)) ? sizeof(gCGIBuffer) : length);

        if (read > 0)
        {
        	httpServerRequestPost(param->script_name, gCGIBuffer, cgi_data, sizeof(cgi_data), cgi_responseFunction, &response);
        	if(strcmp(param->script_name,"post")==0 || strcmp(param->script_name,"post_reset")==0 || strcmp(param->script_name,"post_hardware")==0){
        		/* Write the response using chunked transmission coding. */
				/* Set content length to -1 to indicate unknown content length. */
				response.content_type = HTTPSRV_CONTENT_TYPE_HTML;
				response.content_length = -1;
				response.data = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">";
				response.data_length = strlen(response.data);
				HTTPSRV_cgi_write(&response);
				response.data = "<html><head><title>POST successfull!</title>";
				response.data_length = strlen(response.data);
				HTTPSRV_cgi_write(&response);
				if(strcmp(param->script_name,"post_reset")==0 || strcmp(param->script_name,"post_hardware")==0){
					response.data = "<meta http-equiv=\"refresh\" content=\"0; url=cgi.html?tab=hardware\"></head><body></body></html>";
				}else{
					response.data = "<meta http-equiv=\"refresh\" content=\"0; url=cgi.html\"></head><body></body></html>";
				}
				response.data_length = strlen(response.data);
				HTTPSRV_cgi_write(&response);
				response.data_length = 0;
				HTTPSRV_cgi_write(&response);
        	}else if(strcmp(param->script_name,"post_network")==0){
        		/* Write the response using chunked transmission coding. */
				/* Set content length to -1 to indicate unknown content length. */
				response.content_type = HTTPSRV_CONTENT_TYPE_HTML;
				response.content_length = -1;
				response.data = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">";
				response.data_length = strlen(response.data);
				HTTPSRV_cgi_write(&response);
				response.data = "<html><head><title>POST successfull!</title>";
				response.data_length = strlen(response.data);
				HTTPSRV_cgi_write(&response);
				response.data = "<meta http-equiv=\"refresh\" content=\"0; url=network.html\"></head><body></body></html>";
				response.data_length = strlen(response.data);
				HTTPSRV_cgi_write(&response);
				response.data_length = 0;
				HTTPSRV_cgi_write(&response);
        	}

        }
    }

    return (response.content_length);
}

static bool cgi_get_varval(const char *src, char *var_name, char *dst, uint32_t length)
{
    char *name;
    bool result;
    uint32_t index;
    uint32_t n_length;

    result = false;
    dst[0] = 0;
    name = (char*)src;

    n_length = strlen(var_name);

    while ((name = strstr(name, var_name)) != 0)
    {
        if (name[n_length] == '=')
        {
            name += n_length + 1;

            index = strcspn(name, "&");
            if (index >= length)
            {
                index = length - 1;
            }
            strncpy(dst, name, index);
            dst[index] = '\0';
            result = true;
            break;
        }
        else
        {
            name = strchr(name, '&');
        }
    }

    return (result);
}

/* Example Server Side Include callback. */
static int ssi_date_time(HTTPSRV_SSI_PARAM_STRUCT *param)
{
    /*if (strcmp(param->com_param, "time") == 0)
    {
        HTTPSRV_ssi_write(param->ses_handle, __TIME__, strlen(__TIME__));
    }
    else if (strcmp(param->com_param, "date") == 0)
    {
        HTTPSRV_ssi_write(param->ses_handle, __DATE__, strlen(__DATE__));
    }*/
    return (0);
}

/* Decode URL encoded string in place. */
static void cgi_urldecode(char *url)
{
    char *src = url;
    char *dst = url;

    while (*src != '\0')
    {
        if ((*src == '%') && (isxdigit((int)*(src + 1))) && (isxdigit((int)*(src + 2))))
        {
            *src = *(src + 1);
            *(src + 1) = *(src + 2);
            *(src + 2) = '\0';
            *dst++ = strtol(src, NULL, 16);
            src += 3;
        }
        else
        {
            *dst++ = *src++;
        }
    }
    *dst = '\0';
}

static void inxSetNoResetCalibration(const bool byUser,volatile float* pCal,const float newValue){
	const float oldValue=*pCal;
	if(oldValue==newValue){

	}else{
		*pCal=newValue;
		inxSettingsChanged(byUser);
	}
}

void inxUpdateSettings(const char* buffer){
	//printf("ZZZZ = %s\n",buffer);
	char data[CONFIG_CGI_DATA_LENGTH_MAX + 1];
	size_t dataSize=sizeof(data);

	if(cgi_get_varval(buffer,"noResetSettings",data,dataSize)){
		//update our no reset settings
		if(cgi_get_varval(buffer,"p1o",data,dataSize)){
			inxSetNoResetCalibration(true,&gNoResetSettings.pt1001Offset,atof(data));
		}
		if(cgi_get_varval(buffer,"p1s",data,dataSize)){
			inxSetNoResetCalibration(true,&gNoResetSettings.pt1001Scale,atof(data));
		}
		if(cgi_get_varval(buffer,"p2o",data,dataSize)){
			inxSetNoResetCalibration(true,&gNoResetSettings.pt1002Offset,atof(data));
		}
		if(cgi_get_varval(buffer,"p2s",data,dataSize)){
			inxSetNoResetCalibration(true,&gNoResetSettings.pt1002Scale,atof(data));
		}
		if(cgi_get_varval(buffer,"t1o",data,dataSize)){
			inxSetNoResetCalibration(true,&gNoResetSettings.thermoCoupleOffset,atof(data));
		}
		if(cgi_get_varval(buffer,"t1s",data,dataSize)){
			inxSetNoResetCalibration(true,&gNoResetSettings.thermoCoupleScale,atof(data));
		}
		if(cgi_get_varval(buffer,"z1o",data,dataSize)){
			inxSetNoResetCalibration(true,&gNoResetSettings.zeroTenVOffset,atof(data));
		}
		if(cgi_get_varval(buffer,"z1s",data,dataSize)){
			inxSetNoResetCalibration(true,&gNoResetSettings.zeroTenVScale,atof(data));
		}
		if(cgi_get_varval(buffer,"f1o",data,dataSize)){
			inxSetNoResetCalibration(true,&gNoResetSettings.fourTwentyMAOffset,atof(data));
		}
		if(cgi_get_varval(buffer,"f1s",data,dataSize)){
			inxSetNoResetCalibration(true,&gNoResetSettings.fourTwentyMAScale,atof(data));
		}
	}else{
		u8_t pidNum=0;
		if(cgi_get_varval(buffer,"pid",data,dataSize)){
			pidNum=atoi(data);
		}
		if(pidNum<0 || pidNum>=CONFIG_PID_CONTROLLERS_NUM){
			pidNum=0;
		}

		//regular settings update
		if(cgi_get_varval(buffer,"cal1",data,dataSize)){
			inxSetCalibration1(true,atof(data));
		}

		if(cgi_get_varval(buffer,"cal2",data,dataSize)){
			inxSetCalibration2(true,atof(data));
		}

		if(cgi_get_varval(buffer,"cal3",data,dataSize)){
			inxSetCalibration3(true,atof(data));
		}

		if(cgi_get_varval(buffer,"elementRatingKW",data,dataSize)){
			updateElementRatingKW(true,atof(data));
		}
		
		if(cgi_get_varval(buffer,"cs",data,dataSize)){
			inxSetControllerSensors(true,&gPIDs[pidNum],atoi(data));
		}

		if(cgi_get_varval(buffer,"td",data,dataSize)){
			inxSetPIDTimerDuration(true,&gPIDs[pidNum],atoi(data),false);
		}

		if(cgi_get_varval(buffer,"ram",data,dataSize)){
			inxSetPIDRamp(true,&gPIDs[pidNum],atoi(data));
		}

		if(cgi_get_varval(buffer,"em",data,dataSize)){
			inxSetElementMode(true,&gPIDs[pidNum],atoi(data));
		}
		
		if(cgi_get_varval(buffer,"safetyMode",data,dataSize)){
			inxSetSafetyMode(true,atoi(data));
		}

		if(cgi_get_varval(buffer,"controllerContactor",data,dataSize)){
			updateControllerContactorMode(true,atoi(data));
		}

		if(cgi_get_varval(buffer,"com",data,dataSize)){
			updateControllerOutputMode(true,atoi(data));
		}
		
		if(cgi_get_varval(buffer,"emrMode",data,dataSize)){
			inxSetDutyCycleLengthEMRSeconds(true,atof(data));
		}

		if(cgi_get_varval(buffer,"ssrMode",data,dataSize)){
			inxSetDutyCycleLengthSSRSeconds(true,atof(data));
		}

		if(cgi_get_varval(buffer,"powerConnection",data,dataSize)){
			inxSetPowerMode(true,atoi(data));
		}

		//schedule mode should always be first
		if(cgi_get_varval(buffer,"sm",data,dataSize)){
			updateScheduleMode(pidNum,true,&gPIDs[pidNum],atoi(data));
		}
		
		if(cgi_get_varval(buffer,"msp",data,dataSize)){
			cgi_urldecode(data);
			updateManualSetPoint(true,&gPIDs[pidNum],atof(data));
		}

		if(cgi_get_varval(buffer,"pid_p",data,dataSize)){
			cgi_urldecode(data);
			updatePID1_KP(true,&gPIDs[pidNum],atof(data));
		}
		
		if(cgi_get_varval(buffer,"pid_i",data,dataSize)){
			cgi_urldecode(data);
			updatePID1_KI(true,&gPIDs[pidNum],atof(data));
		}
		
		if(cgi_get_varval(buffer,"pid_d",data,dataSize)){
			cgi_urldecode(data);
			updatePID1_KD(true,&gPIDs[pidNum],atof(data));
		}

		if(cgi_get_varval(buffer,"lat",data,dataSize)){
			cgi_urldecode(data);
			updateLowerAlarmTemp(true,&gPIDs[pidNum],atof(data));
		}
		
		if(cgi_get_varval(buffer,"uat",data,dataSize)){
			cgi_urldecode(data);
			updateUpperAlarmTemp(true,&gPIDs[pidNum],atof(data));
		}
		
		if(cgi_get_varval(buffer,"steppedModeElement2Threshold",data,dataSize)){
			cgi_urldecode(data);
			inxDutyCyclePowerThresholdSet(true,&dutyCycleState1,atoi(data));
		}
		
		if(cgi_get_varval(buffer,"steppedModeElement3Threshold",data,dataSize)){
			cgi_urldecode(data);
			inxDutyCyclePowerThresholdSet(true,&dutyCycleState2,atoi(data));
		}

		if(cgi_get_varval(buffer,"localNameField",data,dataSize)){
			updateLocalName(data);
		}

		if(cgi_get_varval(buffer,"auxControlOutput",data,dataSize)){
			inxSetAuxControlOutput(true,atoi(data));
		}
	}
}

#if HTTPSRV_CFG_WEBSOCKET_ENABLED
/*
 * Echo plugin code - simple plugin which echoes any message it receives back to
 * client.
 */
#define WSC_BUFFER_SIZE CONFIG_CGI_DATA_LENGTH_MAX
#define WSC_CALLBACK_SIZE 128
#define WSC_NO_CUMULATIVE_DATA -1

static int32_t gWSCumulativeDataLength=WSC_NO_CUMULATIVE_DATA;

static uint32_t gWebSocketHandle=0;
uint32_t ws_echo_connect(void *param, WS_USER_CONTEXT_STRUCT context)
{
#if DEBUG_WS
	UBaseType_t uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );
	LWIP_PLATFORM_DIAG(("ws stack bytes free=%d",uxHighWaterMark*4));
	LWIP_PLATFORM_DIAG(("ws_echo_connect start gWebSocketHandle=%d context.handle=%d",gWebSocketHandle,context.handle));
#endif
	if(gWebSocketHandle==0){
		gWebSocketHandle=context.handle;
	}
#if DEBUG_WS
	LWIP_PLATFORM_DIAG(("ws_echo_connect end gWebSocketHandle=%d context.handle=%d",gWebSocketHandle,context.handle));
#endif
	gWSCumulativeDataLength=WSC_NO_CUMULATIVE_DATA;
    return (0);
}

uint32_t ws_echo_disconnect(void *param, WS_USER_CONTEXT_STRUCT context)
{
#if DEBUG_WS
	LWIP_PLATFORM_DIAG(("ws_echo_disconnect start gWebSocketHandle=%d context.handle=%d",gWebSocketHandle,context.handle));
#endif
	if(context.handle==gWebSocketHandle){
		gWebSocketHandle=0;
	}
#if DEBUG_WS
	LWIP_PLATFORM_DIAG(("ws_echo_disconnect end gWebSocketHandle=%d context.handle=%d",gWebSocketHandle,context.handle));
#endif
	gWSCumulativeDataLength=WSC_NO_CUMULATIVE_DATA;
    return (0);
}

typedef struct ws_response_struct{
	WS_USER_CONTEXT_STRUCT* context;
	const char* callback;
} WebSocketResponse;

static void ws_responseFunction(void* responseObj, char* data){
	WebSocketResponse* response = (WebSocketResponse*)responseObj;
	if(!response){
		// invalid response struct
		return;
	}
	snprintf(gWebSocketData,CONFIG_CGI_DATA_LENGTH_MAX,"{\"callback\":\"%s\",\"data\":%s}",
			 response->callback, data);
	/*response->context->data.length = strlen(gWebSocketData);
	response->context->data.data_ptr[response->context->data.length] = 0;
	response->context->data.data_ptr = (uint8_t*)gWebSocketData;
	WS_send(response->context);*/

	gWebSocketContext.handle=gWebSocketHandle;
	gWebSocketContext.data.data_ptr=(uint8_t*)gWebSocketData;
	gWebSocketContext.data.length=strlen(gWebSocketData);
	gWebSocketContext.data.type=WS_DATA_TEXT;
 	gWebSocketContext.fin_flag=true;
#if DEBUG_WS
	LWIP_PLATFORM_DIAG(("ws_responseFunction WS_send gWebSocketHandle=%d gWebSocketData=%s len=%d",gWebSocketHandle,gWebSocketData,strlen(gWebSocketData)));
#endif
 	WS_send(&gWebSocketContext);
#if DEBUG_WS
	LWIP_PLATFORM_DIAG(("ws_responseFunction WS_send done"));
#endif
}
static uint8_t gWSBuffer[WSC_BUFFER_SIZE]="";
static char gWSCallback[WSC_CALLBACK_SIZE]="";
uint32_t ws_echo_message(void *param, WS_USER_CONTEXT_STRUCT context)
{
	if (context.data.type == WS_DATA_TEXT)
	{
		WebSocketResponse response = {0};
		//LWIP_PLATFORM_DIAG(("WebSocket message received (length:%d,finished:%d): \r\n%s",
		//					context.data.length,context.fin_flag,context.data.data_ptr));
		// handle data that gets received in multiple stages
		if(!context.fin_flag && gWSCumulativeDataLength < WSC_BUFFER_SIZE){
			if(gWSCumulativeDataLength == WSC_NO_CUMULATIVE_DATA){
				context.data.data_ptr[context.data.length] = 0;
				strncpy((char*)gWSBuffer, (char*)context.data.data_ptr, WSC_BUFFER_SIZE);
				gWSCumulativeDataLength = context.data.length;
			}else{
				strncat((char*)gWSBuffer, (char*)context.data.data_ptr,WSC_BUFFER_SIZE);
				gWSCumulativeDataLength += context.data.length;
			}
			return (0);
		}else if(context.fin_flag && gWSCumulativeDataLength != WSC_NO_CUMULATIVE_DATA){
			strncat((char*)gWSBuffer, (char*)context.data.data_ptr,WSC_BUFFER_SIZE);
			gWSCumulativeDataLength += context.data.length;
			context.data.length = gWSCumulativeDataLength;
			context.data.data_ptr = gWSBuffer;
			//LWIP_PLATFORM_DIAG(("WebSocket cumulative message received (length:%d,finished:%d): \r\n%s",
			//							context.data.length,context.fin_flag,context.data.data_ptr));
			gWSCumulativeDataLength = WSC_NO_CUMULATIVE_DATA; // no more cumulative data
		}
		context.data.data_ptr[context.data.length] = 0;
		char* buffer = (char*)context.data.data_ptr;
		if(cgi_get_varval(buffer,"callback",socket_data,sizeof(socket_data))){ // decode method
			//LWIP_PLATFORM_DIAG(("WebSocket callback: %s", socket_data));
			strncpy((char*)gWSCallback,(char*)socket_data,WSC_CALLBACK_SIZE);
		}
#if CONFIG_WEB_SOCKET_MESSAGE_ENABLED
		// assign callback/message id
		cgi_urldecode(socket_data);
		response.callback = gWSCallback;
		//UBaseType_t uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );
		//LWIP_PLATFORM_DIAG(("ws_echo_message stack bytes free=%d",uxHighWaterMark*4));
		if(cgi_get_varval(buffer,"method",socket_data,sizeof(socket_data))){ // decode method
			//LWIP_PLATFORM_DIAG(("WebSocket method: %s", socket_data));
		}
		cgi_urldecode(socket_data);
		if((strncmp(socket_data,"GET",sizeof("GET"))==0)){
			if(cgi_get_varval(buffer,"url",socket_data,sizeof(socket_data))){ // decode method
				//LWIP_PLATFORM_DIAG(("WebSocket url: %s", socket_data));
				cgi_urldecode(socket_data);

				httpServerRequestGet(socket_data, socket_data);
				response.context = &context;
				ws_responseFunction(&response, socket_data);
			}
		}else{ // POST
			if(cgi_get_varval(buffer,"url",socket_data,sizeof(socket_data))){ // decode method
				//LWIP_PLATFORM_DIAG(("WebSocket url: %s", socket_data));
				cgi_urldecode(socket_data);
				response.context = &context;
				httpServerRequestPost(socket_data, buffer, socket_data, sizeof(socket_data), ws_responseFunction, &response);
			}
		}
#endif //CONFIG_WEB_SOCKET_MESSAGE_ENABLED
	}
    return (0);
}

uint32_t ws_echo_error(void *param, WS_USER_CONTEXT_STRUCT context)
{
#if DEBUG_WS
    PRINTF("WebSocket error: 0x%X.\r\n", context.error);
#endif
    return (0);
}

WS_PLUGIN_STRUCT ws_tbl[] = {{"/ws", ws_echo_connect, ws_echo_message, ws_echo_error, ws_echo_disconnect, NULL},
                             {0, 0, 0, 0, 0, 0}};
#endif /* HTTPSRV_CFG_WEBSOCKET_ENABLED */

typedef enum {
  SMTP_IDLE,
  SMTP_NAME_RESOLVING,
  SMTP_NAME_RESOLVED,
} smtp_state_t;

typedef enum {
  SMTP_ERR_NONE,
  SMTP_ERR_UNKNOWN_HOST,
} smtp_error_t;

typedef struct {
  smtp_state_t state;
  const char *serverName;
  ip_addr_t serverIP;
  smtp_error_t lastError;
} smtp_t;

static void smtp_serverFound(const char *name, const ip_addr_t *ipaddr, void *arg) {
  smtp_t *smtp = (smtp_t*)arg;

  if ((ipaddr) && (ipaddr->addr)) {
    ip4_addr_copy(smtp->serverIP, *ipaddr);
    smtp->lastError = SMTP_ERR_NONE;
    smtp->state = SMTP_NAME_RESOLVED;
    return;
  } else {
    smtp->lastError = SMTP_ERR_UNKNOWN_HOST;
  }
  smtp->state = SMTP_IDLE;
}

static void InitSMTP(smtp_t *smtp) {
  smtp->state = SMTP_IDLE;
  smtp->serverName = "";
  IP4_ADDR(&smtp->serverIP, 0, 0, 0, 0);
  smtp->lastError = SMTP_ERR_NONE;
}

typedef enum
{
	INX_DNS_LOOKUP_STATE_INIT,
	INX_DNS_LOOKUP_STATE_RESOLVING
} inxDNSLookupState;
static inxDNSLookupState gDNSLookupState=INX_DNS_LOOKUP_STATE_INIT;

static smtp_t smtp={0};
static err_t GetHostAddress(struct netif *netifp, const char *hostName, ip4_addr_t *addr) {
  /* see http://lwip.wikia.com/wiki/DNS */
  err_t res=ERR_ABRT;
  ip_addr_t resolved;
	
  switch(gDNSLookupState){
	  case INX_DNS_LOOKUP_STATE_INIT:
		InitSMTP(&smtp);
		if(gDHCP){
			//use the ones that we got from our DHCP request
			gDNS[0].addr=dns_getserver(0)->addr;
			gDNS[1].addr=dns_getserver(1)->addr;
		}else{
			//use our manually configured DHCP addresses
			dns_setserver(0, &gDNS[0]);
			dns_setserver(1, &gDNS[1]);
		}
		smtp.serverName = hostName;
		smtp.state = SMTP_NAME_RESOLVING;
		SW_DIAG_printf(" DNS resolving for: %s\r\n", smtp.serverName);
		res = dns_gethostbyname(smtp.serverName, &resolved, smtp_serverFound, &smtp);
		if (res==ERR_INPROGRESS) {
			gDNSLookupState=INX_DNS_LOOKUP_STATE_RESOLVING;
			res=ERR_INPROGRESS;
		}else if(res==ERR_OK){
			//assume this request was answered using the cache
	  		ip4_addr_copy(*addr, resolved);
	  		SW_DIAG_printf(" Address resolved to: %u.%u.%u.%u\r\n", ((u8_t *)addr)[0], ((u8_t *)addr)[1],
	              ((u8_t *)addr)[2], ((u8_t *)addr)[3]);
			gDNSLookupState=INX_DNS_LOOKUP_STATE_INIT;
			res=ERR_OK;
		}
	  	break;
	case INX_DNS_LOOKUP_STATE_RESOLVING:
		switch(smtp.state){
			case SMTP_NAME_RESOLVING:
				res=ERR_INPROGRESS;
				break;
			case SMTP_NAME_RESOLVED:
				SW_DIAG_printf(" Address resolved to: %u.%u.%u.%u\r\n", ((u8_t *)&smtp.serverIP)[0], ((u8_t *)&smtp.serverIP)[1],((u8_t *)&smtp.serverIP)[2], ((u8_t *)&smtp.serverIP)[3]);
    			ip4_addr_copy(*addr, smtp.serverIP); /* copy */
				gDNSLookupState=INX_DNS_LOOKUP_STATE_INIT;
				res=ERR_OK;
				break;
			default:
				gDNSLookupState=INX_DNS_LOOKUP_STATE_INIT;
				res=ERR_ABRT;
				break;
		}
		break;
  }

  return res;
}

#if HTTPSRV_CFG_MBEDTLS_ENABLE
static HTTPSRV_TLS_PARAM_STRUCT gHttpdTlsParams;
#endif

/* Redirect any pages that needs redirecting*/
#define HTTP_SERVER_REQUEST_UNPROVISIONED_PATH "\\provision.html"
#define HTTP_SERVER_REQUEST_RESTRICTED_PATH    "\\restricted.html"
#define HTTP_SERVER_REQUEST_ICON_PATH    	   "\\favicon.ico"

typedef enum
{
	HTTP_SERVER_WEBPAGE_PATH_SETTINGS = 0,
	HTTP_SERVER_WEBPAGE_PATH_INDEX,
	HTTP_SERVER_WEBPAGE_PATH_MOBILE_INDEX,
	HTTP_SERVER_WEBPAGE_PATH_NETWORK,
	HTTP_SERVER_WEBPAGE_PATH_DASHBOARD,
	HTTP_SERVER_WEBPAGE_PATH_MINI_DASHBOARD,
	HTTP_SERVER_WEBPAGE_PATH_COUNT,   	 // !!! this must always be after last specified web page
	HTTP_SERVER_WEBPAGE_PATH_UNSPECIFIED // this is used for marking web page not specified in the array
} HttpServerWebpage;
/*
static char* HTTP_SERVER_WEBPAGE_PATHS[HTTP_SERVER_WEBPAGE_PATH_COUNT] =
{
	"\\cgi.html",		// HTTP_SERVER_WEBPAGE_PATH_SETTINGS
	"\\index.html",		// HTTP_SERVER_WEBPAGE_PATH_INDEX
	"\\mobile.html",	// HTTP_SERVER_WEBPAGE_PATH_MOBILE_INDEX
	"\\network.html",	// HTTP_SERVER_WEBPAGE_PATH_NETWORK
	"\\welcome.html",	// HTTP_SERVER_WEBPAGE_PATH_DASHBOARD
	"\\minidash.html",	// HTTP_SERVER_WEBPAGE_PATH_MINI_DASHBOARD
};

static HttpServerWebpage getHttpServerWebpage(const char* path){
	for(int i = HTTP_SERVER_WEBPAGE_PATH_SETTINGS; i < HTTP_SERVER_WEBPAGE_PATH_COUNT; i++){
		const char* webpage = HTTP_SERVER_WEBPAGE_PATHS[i];
		if(strncmp(path, webpage, strlen(webpage)) == 0){
			return i;
		}
	}
	return HTTP_SERVER_WEBPAGE_PATH_UNSPECIFIED;
}

static bool isWebpageFullSettings(HttpServerWebpage webpage){
	return (webpage == HTTP_SERVER_WEBPAGE_PATH_DASHBOARD ||
			webpage == HTTP_SERVER_WEBPAGE_PATH_SETTINGS ||
		    webpage == HTTP_SERVER_WEBPAGE_PATH_NETWORK);
}

static bool isWebpageDashboard(HttpServerWebpage webpage){
	return (webpage == HTTP_SERVER_WEBPAGE_PATH_INDEX ||
			webpage == HTTP_SERVER_WEBPAGE_PATH_MOBILE_INDEX ||
			webpage == HTTP_SERVER_WEBPAGE_PATH_DASHBOARD);
}

static bool isWebpageUnspecified(HttpServerWebpage webpage){
	return (webpage == HTTP_SERVER_WEBPAGE_PATH_UNSPECIFIED);
}


static char* http_server_redirect(char* path)
{
	// ignore icon request
	if(strncmp(path, HTTP_SERVER_REQUEST_ICON_PATH, strlen(HTTP_SERVER_REQUEST_ICON_PATH)) == 0){
		return path;
	}
	HttpServerWebpage webpage = getHttpServerWebpage(path);
	if(!gProvisioned){ // redirect unprovisioned recognised pages
		if(webpage == HTTP_SERVER_WEBPAGE_PATH_MOBILE_INDEX){ // @TODO Atm we only allow to provision via mobile device
			return HTTP_SERVER_REQUEST_UNPROVISIONED_PATH;
		}else{
			//LWIP_PLATFORM_DIAG(("The web page path ( %s ) is not recognised.", path));
			return HTTP_SERVER_REQUEST_RESTRICTED_PATH;
		}
	}else{
		// Http server should no longer need to request the provisioning page
		if(isWebpageUnspecified(webpage) && strncmp(path, HTTP_SERVER_REQUEST_UNPROVISIONED_PATH,
				strlen(HTTP_SERVER_REQUEST_UNPROVISIONED_PATH)) == 0){
			//LWIP_PLATFORM_DIAG(("Should no longer need to request the provisioning page."));
			return HTTP_SERVER_REQUEST_RESTRICTED_PATH;
		}else{
			switch(gLocalUIMode){
				case INX_LOCAL_UI_MODE_MOBILE:
					if(webpage == HTTP_SERVER_WEBPAGE_PATH_MOBILE_INDEX || isWebpageFullSettings(webpage)){
						break; // correct web page request
					}else{
						LWIP_PLATFORM_DIAG(("Page ( %s ) not served in MOBILE mode.", path));
						return HTTP_SERVER_REQUEST_RESTRICTED_PATH;
					}
				case INX_LOCAL_UI_MODE_MINI_DASHBOARD:
					if(isWebpageDashboard(webpage)){
						char* minidash = HTTP_SERVER_WEBPAGE_PATHS[HTTP_SERVER_WEBPAGE_PATH_MINI_DASHBOARD];
						return minidash;
					}else{
						LWIP_PLATFORM_DIAG(("Page ( %s ) not served in MINI DASHBOARD mode.", path));
						return HTTP_SERVER_REQUEST_RESTRICTED_PATH;
					}
				case INX_LOCAL_UI_MODE_FULL_DASHBOARD:
					if(isWebpageDashboard(webpage)){
						return HTTP_SERVER_WEBPAGE_PATHS[HTTP_SERVER_WEBPAGE_PATH_DASHBOARD];
					}else{
						LWIP_PLATFORM_DIAG(("Page ( %s ) not served in FULL DASHBOARD mode.", path));
						return HTTP_SERVER_REQUEST_RESTRICTED_PATH;
					}
					break;
				case INX_LOCAL_UI_MODE_FULL_SETTINGS:
					if(isWebpageDashboard(webpage) || isWebpageFullSettings(webpage)){
						break; // correct web page request
					}else{
						LWIP_PLATFORM_DIAG(("Page ( %s ) not served in FULL SETTINGS mode.", path));
						return HTTP_SERVER_REQUEST_RESTRICTED_PATH;
					}
				case INX_LOCAL_UI_MODE_DISABLE:
					LWIP_PLATFORM_DIAG(("Device doesn't serve pages as it's in DISABLED mode."));
					return HTTP_SERVER_REQUEST_RESTRICTED_PATH;
				default:
					LWIP_PLATFORM_DIAG(("No pages are served in UNKNOWN mode."));
					return HTTP_SERVER_REQUEST_RESTRICTED_PATH;
			}
		}
	}
	return path;
}
*/
/*static void http_server_socket_init(void)
{
    HTTPSRV_PARAM_STRUCT params;
    httpsrv_set_redirect_function(http_server_redirect);
    HTTPSRV_FS_init(httpsrv_fs_data);
    memset(&params, 0, sizeof(params));
    params.root_dir = "";
    params.index_page = "/index.html";
    params.auth_table = auth_realms;
    params.cgi_lnk_tbl = cgi_lnk_tbl;
    params.ssi_lnk_tbl = ssi_lnk_tbl;
#if HTTPSRV_CFG_WEBSOCKET_ENABLED
    params.ws_tbl = ws_tbl;
#endif

#if HTTPSRV_CFG_MBEDTLS_ENABLE
    gHttpdTlsParams.certificate_buffer=gHttpdCrt;
    gHttpdTlsParams.certificate_buffer_size=gHttpdCrtSize;
    gHttpdTlsParams.private_key_buffer=gHttpdCrtKey;
    gHttpdTlsParams.private_key_buffer_size=gHttpdCrtKeySize;
    params.tls_param=&gHttpdTlsParams;
#endif
	gHttpsrvHandle = HTTPSRV_init(&params);
	if (gHttpsrvHandle == 0)
	{
		LWIP_PLATFORM_DIAG(("HTTPSRV_init() is Failed"));
	}
}*/

static uint16_t gDNSCount=0;
err_enum_t inxNetworkTaskWaitForDNS(const char* host,ip4_addr_t* pAddress){
	err_enum_t err=ERR_OK;
	#if CONFIG_MQTT_ENABLED
		ip4_addr_t newBrokerAddress;
		#if CONFIG_MQTT_LOCATION==CONFIG_MQTT_LOCATION_PATRICK
			IP4_ADDR(&newBrokerAddress, 192U, 168U, 2U, 199U);
			HW_DIAG_printf("IPv4 Address     : %u.%u.%u.%u\n", ((u8_t *)&newBrokerAddress)[0],
							((u8_t *)&newBrokerAddress)[1], ((u8_t *)&newBrokerAddress)[2],
							((u8_t *)&newBrokerAddress)[3]);
		#else
			ip4_addr_set_u32(&newBrokerAddress,ipaddr_addr(CONFIG_BROKER_HOST_IP));
			err_t res=GetHostAddress(&fsl_netif0, host, &newBrokerAddress);
			switch(res){
				case ERR_OK:
					err=ERR_OK;
					break;
				case ERR_INPROGRESS:
					err=ERR_INPROGRESS;
					break;
				default:
					LWIP_DEBUGF(MQTT_APP_DEBUG_TRACE,("ERROR: unable to get IP address for broker!\r\n"));
					//PRINTF("do disconnect because getting ip address of broker failed\n");
					err=ERR_INPROGRESS;
					break;	
			}
		#endif
	#endif //CONFIG_MQTT_ENABLED
	switch(err){
		case ERR_OK:
			gDNSCount=0;
			*pAddress=newBrokerAddress;
			break;
		default:
			gDNSCount++;
			if(gDNSCount>CONFIG_DNS_WAIT_TICKS){
				err=ERR_TIMEOUT;
				gDNSCount=0;
			}
			break;
	}
	return err;
}

/* See write up of MCG clock modes http://www.utasker.com/kinetis/MCG.html
 Tutorial on MCG https://www.nxp.com/files-static/training_pdf/28973_S08_MCG_WBT.pdf
 Also see chapter 4 of http://cache.freescale.com/files/32bit/doc/quick_ref_guide/KLQRUG.pdf?fsrch=1&sr=4
 Also see table 25-16 of https://cdn.sparkfun.com/datasheets/Dev/Arduino/Boards/K64P144M120SF5RM.pdf
 * */
static uint16_t gInxNetworkTaskWaitForPhyCount = 0;
static err_enum_t inxNetworkTaskWaitForPhy(){
	err_enum_t err=ERR_OK;
#if CONFIG_TLK106
	fsl_enet_config0.phyAddress=CONFIG_SMI_PHY_ADDR; //this is the current SMI address of our tlk106 chip
#endif

	fsl_netif0.state=&fsl_enet_config0;
	//LWIP_PLATFORM_DIAG(("gInxNetworkTaskWaitForPhyCount=%d",gInxNetworkTaskWaitForPhyCount));
	err=ethernetif0_init(&fsl_netif0);
	gInxNetworkTaskWaitForPhyCount++;
	if(gInxNetworkTaskWaitForPhyCount>CONFIG_WAIT_FOR_PHY_TICKS){
		gInxNetworkTaskWaitForPhyCount=0;
		err=ERR_TIMEOUT;
	}
	return err;
}

static err_enum_t inxNetworkTaskStartDHCP(){
	err_enum_t err=netifapi_netif_set_default(&fsl_netif0);
	if(err!=ERR_OK){
		LWIP_PLATFORM_DIAG(("could not set default ethernet"));
		return err;
	}
	err=netifapi_netif_set_up(&fsl_netif0);
	if(err!=ERR_OK){
		LWIP_PLATFORM_DIAG(("could not bring up ethernet"));
		return err;
	}
	if(gDHCP){
		err=dhcp_start(&fsl_netif0);
		if(err!=ERR_OK){
			LWIP_PLATFORM_DIAG(("could not start dhcp"));
		}else{
		}
		LWIP_PLATFORM_DIAG(("did start stop again"));
	}else{

	}
	return err;
}

static uint16_t gDhcpCount=0;
static err_enum_t inxNetworkTaskWaitForDHCP(){
	err_enum_t err=ERR_OK;
	if(!gDHCP || DHCP_IsBound(&fsl_netif0)){
		gDhcpCount=0;
		//dhcp done move to next state
		//http_server_socket_init();
		//if(gHttpsrvHandle==0){
			//PRINTF("do disconnect because starting web server failed\n");
			//#if CONFIG_MQTT_ENABLED
				//mqttSetGlobalState(MQTT_STATE_DO_DISCONNECT);
			//#endif
			//err=ERR_ABRT;
			//printf("PBB ignoring http error\n");
		//}

#if CONFIG_MDNS_ENABLED
		    mdns_resp_add_netif(&fsl_netif0, gMdnsName, 60);
		    mdns_resp_add_service(&fsl_netif0, gMdnsName, "_http", DNSSD_PROTO_TCP, 80, 300, NULL, NULL);
		    //LWIP_PLATFORM_DIAG(("mDNS hostname    : %s", gMdnsName));
#endif
			char ipaddrStr[16];
			snprintf(ipaddrStr,sizeof(ipaddrStr),"%u.%u.%u.%u",
				((u8_t *)&fsl_netif0.ip_addr)[0],
		        ((u8_t *)&fsl_netif0.ip_addr)[1],
				((u8_t *)&fsl_netif0.ip_addr)[2],
		        ((u8_t *)&fsl_netif0.ip_addr)[3]);
			printf("PBB 3103 %s\n",ipaddrStr);
			EhsHMetaSetIPAddr(ipaddrStr);
		    LWIP_PLATFORM_DIAG(("\r\n************************************************"));
		    LWIP_PLATFORM_DIAG((" IPv4 Address     : %s",ipaddrStr));
		    LWIP_PLATFORM_DIAG((" IPv4 Subnet mask : %u.%u.%u.%u", ((u8_t *)&fsl_netif0.netmask)[0],
		                        ((u8_t *)&fsl_netif0.netmask)[1], ((u8_t *)&fsl_netif0.netmask)[2],
		                        ((u8_t *)&fsl_netif0.netmask)[3]));
		    LWIP_PLATFORM_DIAG((" IPv4 Gateway     : %u.%u.%u.%u", ((u8_t *)&fsl_netif0.gw)[0], ((u8_t *)&fsl_netif0.gw)[1],
		                        ((u8_t *)&fsl_netif0.gw)[2], ((u8_t *)&fsl_netif0.gw)[3]));
		    LWIP_PLATFORM_DIAG(("************************************************"));
	}else{
		gDhcpCount++;
		if(gDhcpCount>CONFIG_DHCP_WAIT_TICKS){
			gDhcpCount = 0; //so if we are called again we behave sensibly
			err=ERR_TIMEOUT;
		}else{
			err=ERR_INPROGRESS;
		}
	}
	return err;
}

/*!
 * @brief Initializes lwIP stack.
 */
static err_t stack_init(void)
{
	err_t err=ERR_OK;
    // we're getting clashing MAC addresses with this.
	//copy last part of cpu id in to last part of mac address
	uint8_t uidlLastBit = SIM->UIDL & 0x000000FF;
	fsl_enet_config0.macAddress[5]=uidlLastBit;
	uidlLastBit = (SIM->UIDL & 0x0000FF00)>>8;
	fsl_enet_config0.macAddress[4]=uidlLastBit;
	uidlLastBit = (SIM->UIDL & 0x00FF0000)>>16;
	fsl_enet_config0.macAddress[3]=uidlLastBit;
	SW_DIAG_printf("MAC Address = %X:%X:%X:%X:%X:%X\n",fsl_enet_config0.macAddress[0],fsl_enet_config0.macAddress[1],fsl_enet_config0.macAddress[2],fsl_enet_config0.macAddress[3],fsl_enet_config0.macAddress[4],fsl_enet_config0.macAddress[5]);
	static ip4_addr_t fsl_netif0_ipaddr, fsl_netif0_netmask, fsl_netif0_gw;
	//ip4_addr_t mypc=IPADDR4_INIT_BYTES(192,168,2,55);
	//PBB used for testing network stuff
	//struct pbuf* p_out=pbuf_alloc(PBUF_TRANSPORT, sizeof(uint32_t)*2, PBUF_RAM);
	//struct udp_pcb* dhcp_pcb=udp_new();
	dhcp_release(&fsl_netif0);
	dhcp_stop(&fsl_netif0);
	dhcp_cleanup(&fsl_netif0);
	netifapi_netif_set_down(&fsl_netif0);
	netifapi_netif_remove(&fsl_netif0);
	if(gDHCP){
		//PBB force net to blank and we wait for DHCP
		IP4_ADDR(&fsl_netif0_ipaddr, 0U, 0U, 0U, 0U);
		IP4_ADDR(&fsl_netif0_netmask, 0U, 0U, 0U, 0U);
		IP4_ADDR(&fsl_netif0_gw, 0U, 0U, 0U, 0U);
#if CONFIG_TLK106
		fsl_enet_config0.phyAddress=CONFIG_SMI_PHY_ADDR; //this is the current SMI address of our tlk106 chip
		fsl_netif0.state=&fsl_enet_config0;
#endif
		err=netifapi_netif_add(&fsl_netif0, &fsl_netif0_ipaddr, &fsl_netif0_netmask, &fsl_netif0_gw, &fsl_enet_config0,ethernetif0_init, tcpip_input);
	}else{
		err=netifapi_netif_add(&fsl_netif0, &gfsl_netif0_ipaddr, &gfsl_netif0_netmask, &gfsl_netif0_gw, &fsl_enet_config0,ethernetif0_init, tcpip_input);
	}
	gInxNetworkTaskWaitForPhyCount=0; //so that we wait the correct amount of time for the phy
	return err;
}



#define INX_CPUID
#ifdef INX_CPUID
/* Unique CPU ID */
void getCpuId(char * buf) {
	/*    UID[0] = SIM->UIDH;
	    UID[1] = SIM->UIDMH;
	    UID[2] = SIM->UIDML;
	    UID[3] = SIM->UIDL;*/
	//PBB we don't use colons as that can cause problems with URL handling
	memset(buf,0x0,INX_CPU_ID_MAX_LENGTH);
	snprintf(buf,INX_CPU_ID_MAX_LENGTH, "%X-%X-%X-%X", SIM->UIDH, SIM->UIDMH,SIM->UIDML,SIM->UIDL);
	buf[INX_CPU_ID_MAX_LENGTH-1]=0x0;
	/* todo check the above are all actually 32bit wide values - we maybe getting some random bits if they are in fact smaller useful bitfields (e.g. some may be 16 bit). */
}
#endif

void copyGlobalADCValues(globalADCValues_t* pDst,globalADCValues_t* pSrc){
	memcpy(pDst,pSrc,sizeof(globalADCValues_t));
}

volatile static bool gWriteToBufferZero=1;
static void writeGlobalADCValues(globalADCValues_t* pNewValues){
	globalADCValues_t* pValues;
	//LWIP_PLATFORM_DIAG(("writeGlobalADCValues: gWriteToBufferZero=%d",gWriteToBufferZero));
	if(gWriteToBufferZero){
		pValues=&gADCValues[0];
	}else{
		pValues=&gADCValues[1];
	}
	copyGlobalADCValues(pValues,pNewValues);
}

static globalADCValues_t readGlobalADCValues(){
	globalADCValues_t values;
	//LWIP_PLATFORM_DIAG(("readGlobalADCValues: gWriteToBufferZero=%d",gWriteToBufferZero));
	if(gWriteToBufferZero){
		gWriteToBufferZero=false; //so that the ADC stops writing to it
		copyGlobalADCValues(&values,&gADCValues[0]);
	}else{
		gWriteToBufferZero=true;
		copyGlobalADCValues(&values,&gADCValues[1]);
	}
	return values;
}
/*
static void globalADCValuesPrint(globalADCValues_t* pValues){
	printf("Temp1=%3.1f\tTemp1Var=%3.1f\tDuty1=%3.1f\tDuty1Var=%3.1f\tTemp2=%3.1f\tTemp2Var=%3.1f\tDuty2=%3.1f\tDuty2Var=%3.1f\n",
			pValues->adc0.temperature,
			pValues->adc0.temperatureVar,
			pValues->adc0.duty,
			pValues->adc0.dutyVar,
			pValues->adc1.temperature,
			pValues->adc1.temperatureVar,
			pValues->adc1.duty,
			pValues->adc1.dutyVar);
}
*/
/* todo - review this value - we may want this to only apply if not a trend value */
#define INX_MAX_CELSIUS_DEVIATION 10.0f
/* todo - turn this into a few functions */
void calculateAverageAndVariance(globalADCValues_t* pValues) {
#ifdef INX_USE_OUTLIER_DETECTION
	float _average_1 = 0.0f;
	float _average_2 = 0.0f;
	int _good_count_1 = 0;
	int _good_count_2 = 0;
#endif
	//unsigned char _good_samples[INX_SAMPLEBUF_SIZE]; // explicitly assigned

	adcConvertedValues_t averageValues={0};
	adcConvertedValues_t varianceValues={0};

	//calculate average values
	for (int i=0; i < INX_SAMPLEBUF_SIZE;i++) {
		averageValues.pid1 += pValues->values[i].pid1;
		averageValues.pid2 += pValues->values[i].pid2;
		averageValues.cpuTemp1 += pValues->values[i].cpuTemp1;
		averageValues.pid3 += pValues->values[i].pid3;
	}
	averageValues.pid1 /= (float)INX_SAMPLEBUF_SIZE;
	gPIDs[0].currentValue = averageValues.pid1;
	averageValues.pid2 /= (float)INX_SAMPLEBUF_SIZE;
	gPIDs[1].currentValue = averageValues.pid2;
	averageValues.cpuTemp1 /= (float)INX_SAMPLEBUF_SIZE;
	averageValues.pid3 /= (float)INX_SAMPLEBUF_SIZE;
	gPIDs[2].currentValue = averageValues.pid3;
#ifdef INX_USE_OUTLIER_DETECTION

	/* recalculate mean temperature excluding outliers */
	for (int i=0; i < INX_SAMPLEBUF_SIZE;i++) {
		if (fabs(pValues->values[i].temperature1-averageValues.temperature1) > INX_MAX_CELSIUS_DEVIATION) {
			//_good_samples[i] = 0;
		}
		else {
			//_good_samples[i] = 1;
			_average_1 += pValues->values[i].temperature1;
			_good_count_1 ++;
		}
		if (fabs(pValues->values[i].temperature2-averageValues.temperature2) > INX_MAX_CELSIUS_DEVIATION) {
			//_good_samples[i] = 0;
		}
		else {
			//_good_samples[i] = 1;
			_average_2 += pValues->values[i].temperature2;
			_good_count_2 ++;
		}
	}
	if (_good_count_1 > 3*(INX_SAMPLEBUF_SIZE/4)  ) { // need 7 out of ten to use this value.
		averageValues.temperature1 = _average_1/_good_count_1;
		pValues->adc0.status = ADC_STATUS_OK;
	}
	else {
		pValues->adc0.status = ADC_STATUS_OUTLIERS;
	}
	if (_good_count_2 > 3*(INX_SAMPLEBUF_SIZE/4)  ) { // need 7 out of ten to use this value.
		averageValues.temperature2 = _average_2/_good_count_2;
		pValues->adc1.status = ADC_STATUS_OK;
	}
	else {
		pValues->adc1.status = ADC_STATUS_OUTLIERS;
	}
#endif
	//calculate pseudo variance (including outliers)
	for (int i=0; i < INX_SAMPLEBUF_SIZE;i++) {
		varianceValues.pid1 += (pValues->values[i].pid1-averageValues.pid1)*(pValues->values[i].pid1-averageValues.pid1);
		varianceValues.pid2 += (pValues->values[i].pid2-averageValues.pid2)*(pValues->values[i].pid2-averageValues.pid2);
		varianceValues.cpuTemp1 += (pValues->values[i].cpuTemp1-averageValues.cpuTemp1)*(pValues->values[i].cpuTemp1-averageValues.cpuTemp1);
		varianceValues.pid3 += (pValues->values[i].pid3-averageValues.pid3)*(pValues->values[i].pid3-averageValues.pid3);
	}
	varianceValues.pid1 /= (float)INX_SAMPLEBUF_SIZE;
	varianceValues.pid2 /= (float)INX_SAMPLEBUF_SIZE;
	varianceValues.cpuTemp1 /= (float)INX_SAMPLEBUF_SIZE;
	varianceValues.pid3 /= (float)INX_SAMPLEBUF_SIZE;

	//copy our new values in to our structure
	gValues.current=averageValues;
#ifdef INX_HEATROD_VERBOSE_DEBUG
	globalADCValuesPrint(pValues);
#endif
}

//not used for now but might point to some shared data structure
#define ADC_TASK_DATA_POINTER NULL
//no idea what this should be but copied httpd size for now
#define ADC_TASK_STACK_SIZE 3000
//be slightly more important than the networking so we don't miss data from the adc
#define ADC_TASK_PRIORITY CONFIG_MAIN_THREAD_PRIORITY

static void inxGPIOLogFault(const bool ok, volatile const dutyCycleState_t* pState){
	if(ok){
		
	}else{
		if(pState->on){
			errorLogsWrite(ERROR_LOGS_INTERNAL_RELAY_STUCK_OFF);
		}else{
			errorLogsWrite(ERROR_LOGS_INTERNAL_RELAY_STUCK_ON);
		}
	}
}
    
void inxGPIOFaultDetected(inxRelayCurrentState_t* pRelayErrorState,const bool heater1Ok, const bool heater2Ok,const bool heater3Ok,const bool thermalCOOk){
	SW_DIAG_printf("Hardware ERROR: inxGPIOFaultDetected\n");
	//indicate that the relay state should be sent back to head quarters
	pRelayErrorState->time = inxRTCGetSeconds();

	inxGPIOLogFault(heater1Ok,&dutyCycleState0);
	inxGPIOLogFault(heater2Ok,&dutyCycleState1);
	inxGPIOLogFault(heater3Ok,&dutyCycleState2);

	inxIecShutOffAllRelays(true);

	//make sure we don't do anything more
	gGpioConditionState.mode = INX_GPIO_CONDITION_DISABLED;
	errorLogsWrite(ERROR_LOGS_ADC_DISABLED);
	if(thermalCOOk==false){
		errorLogsWrite(ERROR_LOGS_THERMAL_CUT_OUT);
	}
}

void firmwareWriteSector(const bool doSwap,const uint32_t firmwareCurrentSector,u8_t* otaUpdateOneSectorBuffer,const uint32_t programSizeBytes,const uint32_t firmwareCrc){
	INX_PRINTF_OTA("firmwareWriteSecors gFlashBlockBase=%d gFlashTotalSize=%d gFlashSectorSize=%d doSwap=%d firmwareCurrentSecors=%d programSizeBytes=%d firmwareCrc=%d\r\n",gFlashBlockBase,gFlashTotalSize,gFlashSectorSize,doSwap,firmwareCurrentSector,programSizeBytes,firmwareCrc);
	WDOG_Refresh(wdog_base); //we have to kick the watch dog on entrance and exit as we don't want to be shut down during a write
	taskENTER_CRITICAL(); //this has to be here or we end restarting the board when we actually try to do the flash write
	flash_runtime_test_parameters_t runTimeParams;
	flash_configuration_parameters_t configurationParameters;
	flash_dcp_state_t dcpState;
	safety_common_t safetyCommon;
	safetyCommon.IEC60730B_flash_test_result = 0;

	uint32_t halfWayAddress=gFlashBlockBase + (gFlashTotalSize/2);
	uint32_t sectorAddress=halfWayAddress+(firmwareCurrentSector*gFlashSectorSize);

	#if CONFIG_OTA_DIAG
		flash_prot_state_t protection_state;
		status_t readStatus=FLASH_IsProtected(g_bootloaderContext.allFlashState,
                           sectorAddress,
                           gFlashSectorSize,
                           &protection_state);
		INX_PRINTF_OTA("Flash_IsProtected %d %d\r\n",readStatus,protection_state);
		flash_xacc_state_t access_state;
		readStatus=FLASH_IsExecuteOnly(g_bootloaderContext.allFlashState,
                             sectorAddress,
                             gFlashSectorSize,
                             &access_state);
		INX_PRINTF_OTA("Flash_IsExecuteOnly %d %d\r\n",readStatus,access_state);
	#endif

	INX_PRINTF_OTA("flash_mem_erase sectorAddress=%x\r\n",(void*)sectorAddress);
	status_t flashStatus=flash_mem_erase(sectorAddress,gFlashSectorSize);
	if(flashStatus!=kStatus_Success){
		INX_PRINTF_OTA("failed to write sector\n");
		taskEXIT_CRITICAL();
		WDOG_Refresh(wdog_base);
		return;
	}

	#if CONFIG_OTA_DIAG
		volatile uint32_t count=0;
		while(count<CONFIG_STARTUP_RELAY_CHECK_TICKS){
			count++;
		}
		UBaseType_t uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );
		INX_PRINTF_OTA("supervisor stack bytes free=%d\n",uxHighWaterMark*4);
		readStatus=FLASH_IsProtected(g_bootloaderContext.allFlashState,
                           sectorAddress,
                           gFlashSectorSize,
                           &protection_state);
		INX_PRINTF_OTA("Flash_IsProtected %d %d\r\n",readStatus,protection_state);
		readStatus=FLASH_IsExecuteOnly(g_bootloaderContext.allFlashState,
                             sectorAddress,
                             gFlashSectorSize,
                             &access_state);
		INX_PRINTF_OTA("Flash_IsExecuteOnly %d %d\r\n",readStatus,access_state);
	#endif

	INX_PRINTF_OTA("flash_mem_write sectorAddress=%x\r\n",(void*)sectorAddress);
	flashStatus=flash_mem_write(sectorAddress,gFlashSectorSize,otaUpdateOneSectorBuffer);
	if(flashStatus!=kStatus_Success){
		INX_PRINTF_OTA("failed to write sector\n");
		taskEXIT_CRITICAL();
		WDOG_Refresh(wdog_base);
		return;
	}
	INX_PRINTF_OTA("memset\n");
	memset(otaUpdateOneSectorBuffer,0,gFlashSectorSize);
	if(doSwap){
		HW_DIAG_printf("Doing Swap\n");
		//calculate crc for this new firmware
		SafetyFlashTestInit(0,halfWayAddress,halfWayAddress+programSizeBytes,&runTimeParams, &configurationParameters, &dcpState);
		SafetyFlashAfterResetTest(&safetyCommon, &configurationParameters, &dcpState);
		uint32_t crcValue=safetyCommon.IEC60730B_flash_test_result;
		if(crcValue==firmwareCrc || firmwareCrc==0){
			//copy our settings to the other half of flash
			HW_DIAG_printf("CRC Good\r\n");
			SW_DIAG_printf("saving CRC\r\n");
			saveSettingsToFlash(false,crcValue,programSizeBytes); //we plus one because our size does not start at zero, it starts at 1
			HW_DIAG_printf("Saved Settings\r\n");
			//try using the last sector of flash memory as our swap status area
			//this is written to both halves of the program flash
			uint32_t swapAddress=halfWayAddress-(CONFIG_FLASH_SECTOR_INDEX_FROM_END * gFlashSectorSize);
			flashStatus=flash_mem_erase(swapAddress,gFlashSectorSize);
			if(flashStatus!=kStatus_Success){
				HW_DIAG_printf("Failed to erase swapAddress but we think that is ok %p\n",(void*)swapAddress);

			}
			HW_DIAG_printf("Erased memory, kicking WDT\r\n");
			WDOG_Refresh(wdog_base); //we have to kick the watch dog on entrance and exit as we don't want to be shut down during a write
#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRi || INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
			inxGPIOFaultDetected(&gRelayErrorState,true,true,true,true);
#endif
			bootloader_reliable_update_as_requested(kReliableUpdateOption_Swap,swapAddress);
			HW_DIAG_printf("set swap, now reboot\n");
		}else{
			HW_DIAG_printf("bad crc for firmware 0x%X!=0x%X",crcValue,firmwareCrc);
		}
	}
	taskEXIT_CRITICAL();
	WDOG_Refresh(wdog_base);
}

uint16_t inxRTCReadValue(const inxRTCPartEnum_t part){
	uint16_t value = 0;
#if CONFIG_ENABLE_RTC
	if(gRTCDisabled || !gRTCInitDone){

	}else{
		rtc_datetime_t date;
		RTC_GetDatetime(RTC, &date);
		switch(part){
			case INX_RTC_PART_YEAR:
				value=date.year;
				break;
			case INX_RTC_PART_MONTH:
				value=(uint16_t)date.month;
				break;
			case INX_RTC_PART_DAY:
				value=(uint16_t)date.day;
				break;
			case INX_RTC_PART_HOUR:
				value=(uint16_t)date.hour;
				break;
			case INX_RTC_PART_MINUTE:
				value=(uint16_t)date.minute;
				break;
			case INX_RTC_PART_SECOND:
				value=(uint16_t)date.second;
		}
	}
#endif //CONFIG_ENABLE_RTC
	return value;
}

void inxRTCWriteSeconds(const uint32_t seconds){
	RTC_StopTimer(RTC);
	RTC->TSR=seconds;
	RTC_StartTimer(RTC);
}

status_t inxRTCWriteDateTime(const rtc_datetime_t date){
	status_t status=kStatus_Fail;
#if CONFIG_ENABLE_RTC
	if(gRTCDisabled || !gRTCInitDone){

	}else{
		gADCRTCLastGoodSeconds = RTC_ConvertDatetimeToSeconds(&date); 
		gADCRTCTicks=0;
		RTC_StopTimer(RTC);
		status=RTC_SetDatetime(RTC, &date);
		RTC_StartTimer(RTC);
		//reset any ramp stuff
		for(u8_t i=0;i<CONFIG_PID_CONTROLLERS_NUM;i++){
			inxSetPIDRampDesired(&gPIDs[i],gPIDs[i].rampDesiredTemp,true);
		}
	}
#endif //CONFIG_ENABLE_RTC
	return status;
}

void inxRTCWriteValue(const inxRTCPartEnum_t part,const uint16_t value){
#if CONFIG_ENABLE_RTC
	if(gRTCDisabled || !gRTCInitDone){

	}else{
		rtc_datetime_t date;
		RTC_GetDatetime(RTC,&date);

		switch(part){
			case INX_RTC_PART_YEAR:
				date.year=value;
				break;
			case INX_RTC_PART_MONTH:
				date.month=(uint8_t)value;
				break;
			case INX_RTC_PART_DAY:
				date.day=(uint8_t)value;
				break;
			case INX_RTC_PART_HOUR:
				date.hour=(uint8_t)value;
				break;
			case INX_RTC_PART_MINUTE:
				date.minute=(uint8_t)value;
				break;
			case INX_RTC_PART_SECOND:
				date.second=(uint8_t)value;
				break;
		}
		inxRTCWriteDateTime(date);
	}
#endif
}

void inxRTCGetDateTime(rtc_datetime_t* pDate){
	pDate->day=1;
	pDate->month=1;
	pDate->year=1970;
	pDate->hour=0;
#if CONFIG_ENABLE_RTC
	if(gRTCDisabled || !gRTCInitDone){

	}else{
		RTC_GetDatetime(RTC, pDate);
	}
#endif
}

static sys_thread_t gNetworkThread=NULL;
static void networkTask(void *arg);
static err_t networkThreadInit(){
	err_t result=ERR_OK;
	if(gNetworkThread==NULL){
		gNetworkThread = sys_thread_new("networkTaskName", networkTask, NULL, MQTT_TASK_STACK_SIZE, MQTT_TASK_PRIORITY);
		if(gNetworkThread==NULL){
			LWIP_PLATFORM_DIAG(("failed to create networking task"));
			result=ERR_MEM;
			printf("PBB 3690\r\n");
		}else{
			
		}
	}else{
		LWIP_PLATFORM_DIAG(("network thread already exists"));
	}
	return result;
}

static void networkThreadDestroy(){
	if(gNetworkThread==NULL){
		//LWIP_PLATFORM_DIAG(("network is null"));
	}else{
		vTaskDelete(gNetworkThread);
		gNetworkThread=NULL;
	}
}


typedef enum {
	INX_WATCH_DOG_KICK_ADC,
	INX_WATCH_DOG_KICK_SUPERVISOR
} inxWatchDogKickID_t;

//TODO can we have different modes of error handling depending on controller or network error?
//IEC 60730 H.11.12.7 3 Clock H.2.18.10.4
static void inxWatchDogKick(inxWatchDogKickID_t id){
#ifndef INX_HEATROD_IOT_DIAGNOSTIC_BUILD
	bool ok=false;
	switch(id){
		case INX_WATCH_DOG_KICK_ADC:
			gWatchDogState.adcTicks++;
			if(gWatchDogState.supervisorTicks != gWatchDogState.supervisorPreviousTicks){
				gWatchDogState.supervisorPreviousTicks = gWatchDogState.supervisorTicks;
				ok=true;
			}else{
				LWIP_PLATFORM_DIAG(("inxWatchDogKick: bad supervisorTicks %d==%d",gWatchDogState.supervisorTicks,gWatchDogState.supervisorPreviousTicks));
			}
			//only refresh the watch dog if no problems
			//LWIP_PLATFORM_DIAG(("inxWatchDogKick: timer=%d",GetTimerOutputValue(wdog_base)));
			WDOG_Refresh(wdog_base);
			break;
		case INX_WATCH_DOG_KICK_SUPERVISOR:
			gWatchDogState.supervisorTicks++;
			if(gWatchDogState.adcTicks != gWatchDogState.adcPreviousTicks){
				gWatchDogState.adcPreviousTicks = gWatchDogState.adcTicks;
				ok=true;
			}else{
				LWIP_PLATFORM_DIAG(("inxWatchDogKick: bad adcTicks %d==%d",gWatchDogState.adcTicks,gWatchDogState.adcPreviousTicks));
			}
			break;
		default:
			LWIP_PLATFORM_DIAG(("inxWatchDogKick: unknown id=%d",id));
			break;
	}

	if(ok){

	}else{
		LWIP_PLATFORM_DIAG(("inxWatchDogKick: initiating restart"));
		inxIecFaultShutdown();
	}
#endif
}

static uint32_t gLwipStoppedTicks=0;
static bool gLwipInitDone=false;
static void supervisorTaskNetworkMonitoring(){
	err_t result=ERR_OK;
	switch(gSupervisorNetworkMonitoringState){
		case SUPERVISOR_NETWORK_MONITORING_INIT:
			if(gLwipInitDone==true){
				result=networkThreadInit();
				if(result==ERR_OK){
					gSupervisorNetworkMonitoringState=SUPERVISOR_NETWORK_MONITORING_OK;
				}else{
					LWIP_PLATFORM_DIAG(("failed to start network thread"));
				}
			}else{
				//LWIP_PLATFORM_DIAG(("waiting for lwip"));
			}

		break;
		case SUPERVISOR_NETWORK_MONITORING_OK:
			if(gLwipStopped==true){
				//LWIP_PLATFORM_DIAG(("lwip has been stopped for %d",gLwipStoppedTicks));
				gLwipStoppedTicks++;
				if(gLwipStoppedTicks>CONFIG_SUPERVISOR_LWIP_STOPPED_WAIT_TICKS){
					LWIP_PLATFORM_DIAG(("lwip dead, stopping our own network thread"));
					networkThreadDestroy();
					gSupervisorNetworkMonitoringState=SUPERVISOR_NETWORK_MONITORING_STOPPED;
				}
			}
			break;
		case SUPERVISOR_NETWORK_MONITORING_STOPPED:
			//TODO figure out how to recover without doing a hard reset
			break;
	}

}

static float applyGlobalCalibrations(float temperature1){
	//apply temperature 1 calibrations
	temperature1 = (temperature1*(gCal2+1)) + gCal1;
	//add hardware fudge for HRi, +0.15 degrees per relay that is on, only in EMR mode.
	if(dutyCycleState0.on){
		temperature1=temperature1 + gCal3;
	}
	if(dutyCycleState1.on){
		temperature1=temperature1 + gCal3;
	}
	if(dutyCycleState2.on){
		temperature1=temperature1 + gCal3;
	}
	return temperature1;
}

static u16_t gRTCTicksSinceLastCheck=0;
static u16_t gRTCCheckTicksToWait=0;
static void supervisorTask(void *arg){
	LWIP_UNUSED_ARG(arg);
	bool dutyLogFull;
	adcValuesQueueMessage_t message;
	while(1){
		inxWatchDogKick(INX_WATCH_DOG_KICK_SUPERVISOR);
		dutyLogFull=false;
		//wait for a message from the adcs for a second
		BaseType_t received=xQueueReceive(gAdcValuesQueue,&message,gLPTMRInterruptMilliseconds*4);
#if CONFIG_ENABLE_RTC
		//LWIP_PLATFORM_DIAG(("supervisor time=%d",RTC->TSR));
		if(received==pdTRUE){

			if(gControllerOutputMode==INX_CONTROLLER_OUTPUT_MODE_DISABLED){
				//do nothing
			}else{
				//update modbus
			}

			//handle duty logging
			dutyLogFull=dutyCycleStateLog(&dutyCycleHistory0,message);
			if(dutyLogFull){
				inxRTCGetDateTime(&dutyCycleHistory0.historyFinishDate);
				bool success=dutyCycleHistoryCopy(&dutyCycleHistory0,&gMqttDutyCycleHistory0);
				if(success){
				}else{
					LWIP_PLATFORM_DIAG(("failed to copy duty cycle state 0"));
				}
			}
		}else{
			LWIP_PLATFORM_DIAG(("no message from adc"));
		}

		//also check to see if the RTC is stuck
		if(gRTCTicksSinceLastCheck<gRTCCheckTicksToWait){
			gRTCTicksSinceLastCheck++;
		}else{
			INX_PRINTF_SCHEDULE("gRTCTicksSinceLastCheck=%d adc=%d rtc=%d prev=%d gRTCStuck=%d gRTCInitDone=%d\r\n",gRTCTicksSinceLastCheck,inxRTCADCGetSeconds(),inxRTCGetSeconds(),gRTCPreviousSeconds,gRTCStuck,gRTCInitDone);
			gRTCTicksSinceLastCheck=0;
			if(gRTCStuck==false && gRTCInitDone==true && inxRTCGetSeconds()==gRTCPreviousSeconds){
				gRTCStuck=true;
				HW_DIAG_printf("RTC IS Stuck at %d\n",gRTCPreviousSeconds);
				errorLogsWrite(ERROR_LOGS_RTC_STUCK);
			}else{ // We can check for recovery if this was a glitch in the timing of the supervisor thrread. 
				// RTC can be slow to come up on some devices it appears ....
				if (gRTCInitDone==true && gRTCStuck == true) {  // only try if init is good
					uint32_t current_rtc = inxRTCGetSecondsAlways(); // get what ever is there to ignore status
					SW_DIAG_printf("current_rtc=%d gRTCPreviousSeconds=%d gRTCConseqGood=%d\r\n",current_rtc,gRTCPreviousSeconds,gRTCConseqGood);
					if (current_rtc > gRTCPreviousSeconds && ((current_rtc < gRTCPreviousSeconds+120) || gRTCConseqGood == 0)) {
						gRTCConseqGood++; // assume two minutes is better than nothing
						HW_DIAG_printf("RTC Is unsticking with %d\n",current_rtc);
					}else{
						gRTCConseqGood = 0;
					}
					if (gRTCConseqGood > 5 ) {
						gRTCStuck=false;
						gRTCConseqGood = 0;
						HW_DIAG_printf("RTC Is UNstuck with %d\n",current_rtc);
						errorLogsClearSingleError(ERROR_LOGS_RTC_STUCK);
						errorLogsWrite(ERROR_LOGS_RTC_RECOVERED);
					}
					gRTCPreviousSeconds =current_rtc;
				}else{
					errorLogsClearSingleError(ERROR_LOGS_RTC_STUCK_IN_SCHEDULE_MODE);
					gRTCPreviousSeconds=inxRTCGetSeconds(); // use the fancy one normally to pick up any status
				}
			}
		}
#endif //CONFIG_ENABLE_RTC
		//monitoring ADC
		if(gADCValuesReady==true || dutyLogFull==true){
			//LWIP_PLATFORM_DIAG(("gADCValuesReady=%d dutyLogFull=%d",gADCValuesReady,dutyLogFull));
			gADCValuesReady=false;
			globalADCValues_t values=readGlobalADCValues();
			inxRTCGetDateTime(&values.finishDate);
			if(dutyLogFull){
			}
			//we use a bool flag because we should only change gUserTemperatureAlarm when a change of state occurs, not every loop
			bool ok=true;
			for(u8_t i=0;i<CONFIG_PID_CONTROLLERS_NUM;i++){
				if(	gPIDs[i].sensors!=INX_CONTROLLER_SENSORS_NONE && 
					(gPIDs[i].currentValue < gPIDs[i].lowerAlarmTemp || gPIDs[i].currentValue > gPIDs[i].upperAlarmTemp)){
					//trigger the user defined alarm behaviour, currently just indicate MQTT thread should send something
					ok=false;
					if(gUserTemperatureAlarmTriggered){
						//do nothing, already triggered
					}else{
						gUserTemperatureAlarmTriggered=true;
						gUserAlarmsAdcValues=gPIDs[i].currentValue;
						errorLogsWrite(ERROR_LOGS_USER_ALARM_TEMPERATURE);
					}
				}else{
					
				}
			}
			if(ok){
				gUserTemperatureAlarmTriggered=false;
				errorLogsClearSingleError(ERROR_LOGS_USER_ALARM_TEMPERATURE);
			}
		}

		//check alarms
		if(gADCValuesUnsafePreviousValue==gADCValuesUnsafe){
			//no state change here so ignore everything
		}else{
			gADCValuesUnsafePreviousValue=gADCValuesUnsafe; //so we don't log it repeatedly, only on state change
			if(gADCValuesUnsafe==true){
				errorLogsWrite(ERROR_LOGS_ADC_VALUES_UNSAFE);
			}else{
				//state going from unsafe to safe
				errorLogsClearSingleError(ERROR_LOGS_ADC_VALUES_UNSAFE);
			}
		}

		supervisorTaskNetworkMonitoring();

		//TODO set local hardware alarm actions
		//TODO post alarms to network services
		//TODO update display
		//TODO reading user controls (e.g buttons)
		//TODO bachnet
		//TODO modbus IP

		//else printf("[RTC error - staus=%s]\n",(TimeState.rtcState==INX_TIME_STATE_RTC_UNSET)?"INSET":TimeState.rtcState==INX_TIME_STATE_RTC_DRIFT?"DRIFT":"OTHER");
		// we need to set a system-wide state variable with this information as in docs.

		//vTaskDelay(1000); //we now use waiting for a duty message instead
		//UBaseType_t uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );
		//printf("PBB supervisor stack bytes free=%d\n",uxHighWaterMark*4);
	}
	vTaskDelete(NULL);
}

void vApplicationStackOverflowHook( TaskHandle_t xTask,
                                    signed char *pcTaskName ){
	SW_DIAG_printf("stack overflow %s\n",pcTaskName);
}

#if HTTPSRV_CFG_WEBSOCKET_ENABLED
static void buildWebSocketStatusPayload(char payload[512]){
	rtc_datetime_t date;
	inxRTCGetDateTime(&date);

	//lock everything, copy the values, then unlock everything
#if CONFIG_MQTT_ENABLED
	MQTT_State_t mqttState=mqttGetGlobalState();
#else
	MQTT_State_t mqttState=MQTT_STATE_INIT;
#endif
	//const int result=
	snprintf(payload,512,"{\"timeStamp\":\"%d-%0*d-%0*dT%0*d:%0*d:%0*dZ\","
		"\"orgName\":\"%s\","
		"\"pid1\":%2.1f,"
		"\"pid2\":%2.1f,"
		"\"pid3\":%2.1f,"
		"\"desiredTemp1\":%2.1f,"
		"\"desiredTemp2\":%2.1f,"
		"\"desiredTemp3\":%2.1f,"
		"\"power1\":%d,"
		"\"power2\":%d,"
		"\"power3\":%d,"
		"\"dsp1\":%2.1f,\"dsp2\":%2.1f,\"dsp3\":%2.1f,"
		"\"e1power\":%d,\"e2power\":%d,\"e3power\":%d,\"mqttState\":%d,"
		"\"lat1\":%2.1f,\"uat1\":%2.1f,\"lat2\":%2.1f,\"uat2\":%2.1f,\"lat3\":%2.1f,\"uat3\":%2.1f}",
		  date.year,2,date.month,2,date.day,2,date.hour,2,date.minute,2,date.second,
		  gOrganisationName,
		  gPIDs[0].currentValue,
		  gPIDs[1].currentValue,
		  gPIDs[2].currentValue,
		  gPIDs[0].desiredTemp,
		  gPIDs[1].desiredTemp,
		  gPIDs[2].desiredTemp,
		  gPIDs[0].powerPercent,
		  gPIDs[1].powerPercent,
		  gPIDs[2].powerPercent,
		  gPIDs[0].rampDesiredTemp,gPIDs[1].rampDesiredTemp,gPIDs[2].rampDesiredTemp,
		  dutyCycleState0.powerPercent,
		  dutyCycleState1.powerPercent,
		  dutyCycleState2.powerPercent,
		  mqttState,
		  gPIDs[0].lowerAlarmTemp,gPIDs[0].upperAlarmTemp,gPIDs[1].lowerAlarmTemp,gPIDs[1].upperAlarmTemp,gPIDs[2].lowerAlarmTemp,gPIDs[2].upperAlarmTemp);
}

static uint32_t gWebSocketTicksSinceLastSentData=0;
static void networkTaskHandleWebSocket(){
	if(gWebSocketHandle>0){
		gWebSocketTicksSinceLastSentData++;
		if(gWebSocketTicksSinceLastSentData>=CONFIG_WEB_SOCKET_DATA_TICKS_SECONDS){
			//reset the ticks
			gWebSocketTicksSinceLastSentData=0;

			//build a JSON payload
			buildWebSocketStatusPayload(gWebSocketData);

			//WS_send does not create it's own copy of the data so we must give it a safe buffer
#if DEBUG_WS
			 LWIP_PLATFORM_DIAG(("gWebSocketHandle=%d payload=%s",gWebSocketHandle,gWebSocketData));
#endif
			 gWebSocketContext.handle=gWebSocketHandle;
			 gWebSocketContext.data.data_ptr=(uint8_t*)gWebSocketData;
			 gWebSocketContext.data.length=strlen(gWebSocketData);
			 gWebSocketContext.data.type=WS_DATA_TEXT;
			 gWebSocketContext.fin_flag=true;
#if DEBUG_WS
			 LWIP_PLATFORM_DIAG(("WS_send start"));
#endif
			 WS_send(&gWebSocketContext);
#if DEBUG_WS
			 LWIP_PLATFORM_DIAG(("WS_send end"));
#endif
		}
	}else{
		gWebSocketTicksSinceLastSentData=CONFIG_WEB_SOCKET_DATA_TICKS_SECONDS;
	}
}
#endif
#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRi || INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
#if CONFIG_GPIO_BUTTONS_INTERRUPT
#else
#endif //CONFIG_GPIO_BUTTONS_INTERRUPT
#endif //INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRi || INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc

extern inxModbusState_t gModbusState;

/*
 * Thread that runs the network and async local display driver
 * */
signed char* EhsL_buff;
static void networkTask(void *arg){
	LWIP_UNUSED_ARG(arg);
	#if INX_HEATROD_EHS
		//inxEHSCopyDefaultToFlash();
		//inxClearFiles();
		EhsL_buff=(signed char*)inxSODLGetReadPointer();
		EhsMain(NULL,NULL);
		EhsHMetaSetHWID(clientId);
		//printf("PBB priority=%d\n",NVIC_GetPriority(FTM0_IRQn));
	//NVIC_SetPriority(FTM0_IRQn,5U);
	//NVIC_SetPriority(FTM1_IRQn,6U);
		ftm_config_t ftmInfo;
	FTM_GetDefaultConfig(&ftmInfo);

    /* Divide FTM clock by 4 */
    ftmInfo.prescale = kFTM_Prescale_Divide_1;

    /* Initialize FTM module */
    FTM_Init(BOARD_FTM_BASEADDR, &ftmInfo);
	FTM_SetTimerPeriod(BOARD_FTM_BASEADDR,USEC_TO_COUNT(1U,FTM_SOURCE_CLOCK));
	NVIC_SetPriority(FTM0_IRQn,8U); //set to 1 after the UART
    FTM_EnableInterrupts(BOARD_FTM_BASEADDR, kFTM_TimeOverflowInterruptEnable);
    EnableIRQ(BOARD_FTM_IRQ_NUM);
    FTM_StartTimer(BOARD_FTM_BASEADDR, kFTM_SystemClock);


	FTM_Init(FTM1, &ftmInfo);
	FTM_SetTimerPeriod(FTM1,USEC_TO_COUNT(1000U,FTM_SOURCE_CLOCK));
	NVIC_SetPriority(FTM1_IRQn,8U); //set to 1 after the UART
	FTM_EnableInterrupts(FTM1,kFTM_TimeOverflowInterruptEnable);
	EnableIRQ(FTM1_IRQn);
	FTM_StartTimer(FTM1,kFTM_SystemClock);

		UBaseType_t uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );
		printf("PBB 4061 %d\n",uxHighWaterMark*4);
	#endif //INX_HEATROD_EHS
	err_t err=ERR_OK;
#if CONFIG_MDNS_ENABLED
	mdns_resp_init();
#endif

	while(1){
		//LWIP_PLATFORM_DIAG(("2155 about to get available memory"));
		//uint32_t freeHeap=GetAvailableMemory();
		//LWIP_DEBUGF(DEBUG_NETWORK,("gNetworkState=%d %s",gNetworkState,clientId));
		if(gSettingsUpdatedByUser==true){
			gSettingsUpdatedByUser=false;
			gSendSettings=true;
			saveSettingsToFlash(true,gCrcValue,gProgramSizeBytes);
		}else if(gDoFactoryReset==true){
			loadSettingsFromFlash(true);
			saveSettingsToFlash(true,gCrcValue,gProgramSizeBytes);
			gDoFactoryReset=false;
			inxIecRebootDevice();
		}else if(gDoReset==true){
			gDoReset=false;
			err=errorLogsClear();
			if(err==ERR_OK){
				//don't have to save if no errors were cleared
			}else{
				//assume we cleared some errors so should save the settings again
				saveSettingsToFlash(true,gCrcValue,gProgramSizeBytes);
			}
			inxIecRebootDevice();
		}else if(gErrorLogsUpdated==true){
			gErrorLogsUpdated=false;
			saveSettingsToFlash(true,gCrcValue,gProgramSizeBytes);
		}else if(EhsPermanentStorageCopyToBuffer(gSettingsToWrite,sizeof(gSettingsToWrite))){
			inxEHSPermanentStorageWrite();
		}
		//SW_DIAG_printf("gNetworkState=%d\r\n",gNetworkState);
#if CONFIG_ENET_ENABLED
printf("PBB gNetworkState=%d\n",gNetworkState);
		switch(gNetworkState){
			case NETWORK_STATE_INIT:
				err=stack_init();
				//HW_DIAG_printf("Network State - Init \n");
				//ignore errors from this method as our networking layer doesn't report errors accurately
				gNetworkState=NETWORK_STATE_WAIT_FOR_PHY;
				inxLedNetworkModeSet(INX_LED_NETWORK_MODE_DISABLED);
				break;
			case NETWORK_STATE_WAIT_FOR_PHY:
				err=inxNetworkTaskWaitForPhy();
				//HW_DIAG_printf("Network State - Wait for Phy - done\n");
				if(err==ERR_OK){
					gNetworkState=NETWORK_STATE_START_DHCP;
				}else if(err==ERR_INPROGRESS){

				}else{
					gNetworkState=NETWORK_STATE_DO_DISCONNECT;
				}
				break;
			case NETWORK_STATE_START_DHCP:
				err=inxNetworkTaskStartDHCP();
				if(err==ERR_OK){
					gNetworkState=NETWORK_STATE_WAITING_FOR_DHCP;
				}else{
					gNetworkState=NETWORK_STATE_DO_DISCONNECT;
				}
				//HW_DIAG_printf("Network State - Set to wait for DHCP\n");
				break;
			case NETWORK_STATE_WAITING_FOR_DHCP:
				err=inxNetworkTaskWaitForDHCP();
				if(err==ERR_OK){
					#if CONFIG_MQTT_ENABLED
						mqttSetGlobalState(MQTT_STATE_INIT);
					#endif
					gNetworkState=NETWORK_STATE_CONNECTED;
					//HW_DIAG_printf("Network State - DHCP done\n");
					inxLedNetworkModeSet(INX_LED_NETWORK_MODE_LOCAL);
#if CONFIG_NETWORK_TEST_DHCP_MEM_LEAK == 1
					gNetworkState=NETWORK_STATE_DO_DISCONNECT;
#endif //CONFIG_NETWORK_TEST_DHCP_MEM_LEAK
				}else if(err==ERR_INPROGRESS){
					//HW_DIAG_printf("Network State - DHCP in progress\n");

				}else{
					//HW_DIAG_printf("Network State - DHCP timedout\n");
					//dhcp timed out, disconnect again
					gNetworkState=NETWORK_STATE_DO_DISCONNECT;
				}
				break;
			case NETWORK_STATE_CONNECTED:
				//HW_DIAG_printf("Network State - Connected - starting MQTT main loop\n");
#if HTTPSRV_CFG_WEBSOCKET_ENABLED
				networkTaskHandleWebSocket();
#endif
#if CONFIG_MQTT_ENABLED
				mqttMainLoop();
#endif
				break;
			case NETWORK_STATE_DO_DISCONNECT:
				//HW_DIAG_printf("Network State - Disconnecting\n");
				printf("PBB 4077\n");
				if(gHttpsrvHandle!=0){
					HTTPSRV_release(gHttpsrvHandle);
					gHttpsrvHandle=0;
			  	}
				  printf("PBB 4082\n");
		      	dhcp_release(&fsl_netif0);
				  printf("PBB 4084\n");
		      	dhcp_stop(&fsl_netif0);
				  printf("PBB 4086\n");
		      	dhcp_cleanup(&fsl_netif0);
				  printf("PBB 4088\n");
#if CONFIG_MDNS_ENABLED
		      	mdns_resp_remove_netif(&fsl_netif0);
#endif
		      	gNetworkState=NETWORK_STATE_INIT;
				  printf("PBB 4088\n");
				break;
		}
#endif //CONFIG_ENET_ENABLED
#if CONFIG_MODBUS_ENABLED
		switch(gModbusState){
			case INX_MODBUS_STATE_INIT:
				eMBInit( MB_RTU, 1, 0, BOARD_DEBUG_UART_BAUDRATE, MB_PAR_NONE );
				eMBEnable();
				gModbusState=INX_MODBUS_STATE_RUN;
				printf("PBB 4089 modbus enabled\n");
				break;
			case INX_MODBUS_STATE_STOP:
				eMBDisable();
				eMBClose();
				gModbusState = INX_MODBUS_STATE_STOPPED;
				break;
			case INX_MODBUS_STATE_STOPPED:
			case INX_MODBUS_STATE_RUN:
				//do nothing for run
				break;
		}

		#if INX_HEATROD_EHS
			//gEhsTick=false;
			Ehs_ConsoleCommand_Type localCommand=Command;
			if (localCommand == EHS_CONTINUE ||	localCommand ==	EHS_RELOAD_EHS_FROM_FILE || localCommand == EHS_PAUSE) {
				#ifndef EHS_THREADED_EHS
					localCommand=EhsProcessCommands(); //@todo we seem to be in this mode - check the EHS_THREADED_EHS flag is set properly.
				#else
					localCommand=EhsPoleCommands(); // This is for implementations where the console process runs in a seperate thread for RTOS version
				#endif
				if (EhsHRequestEHSInterrupt()) {
					localCommand = EhsCheckInternalCommands(localCommand);
				}
			}
			switch(localCommand){
				case EHS_EXIT_EHS:
				case EHS_RELOAD_EHS_FROM_FILE:
					gEhsTick=false;
					EhsCloseAppThreadsAndWaitForTearDown(); // This also calls all the object's destructors. //@todo rename this to DestroyComponents
					if (!EhsResetStaticModules()) { //@todo should the following be before this?
						
					}
					#if CONFIG_MQTT_ENABLED
						mqttSetGlobalState(MQTT_STATE_DO_DISCONNECT);
					#endif
					//save the new sodl
					//inxWriteSODLBuffer();
					//always go back to start of buffer ready for next SODL
					inxSODLFlashSectorIndex=0;
					EhsL_buff=(signed char*)inxSODLGetReadPointer(); //reset the file pointer becaues EHS messes with it
					//EhsAddDynamicModules(); We may only want to do this after reading SODL for module depends?
					EhsApplicationReset(); // Initialise the kernel application tables and reset the memory manager
					if (SetupApplication()) {// command processor starts the app set up here.
						Ehs_AppStart();
					} else {
						EhsApplicationReset(); // Initialise the kernel application tables and reset the memory manager
						inxEHSCopyDefaultToFlash();
						EhsL_buff=(signed char*)inxSODLGetReadPointer();
						if(SetupApplication()){
							Ehs_AppStart();
						}else{
							
						}
					}
					break;
				default:
					break;
			}
			gEhsTick=true;
			//printf("PBB 6726");
		#endif //INX_HEATROD_EHS
		for(uint8_t i=0;i<CONFIG_NETWORK_TASK_POLL_DELAY_MAX_TICKS;i++){
			eMBPoll();
#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRi || INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
#if CONFIG_GPIO_BUTTONS_INTERRUPT
#else
			inxButtonsPoll();
			inxLedTick(gProvisioned);
			inxDisplayPoll(false);
#endif //CONFIG_GPIO_BUTTONS_INTERRUPT
#endif //INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRi || INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
			vTaskDelay(CONFIG_NETWORK_TASK_POLL_DELAY);
		}
		
#else //CONFIG_MODBUS_ENABLED
		vTaskDelay(1000); //simulate the modbus poll delay
#endif //CONFIG_MODBUS_ENABLED

		//UBaseType_t uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );
		//printf("PBB network stack bytes free=%d\n",uxHighWaterMark*4);
	}
	vTaskDelete(NULL);
}

static void supervisorThreadInit(){
	sys_thread_t threadInfo;
	threadInfo = sys_thread_new("supervisorTaskName", supervisorTask, NULL, SUPERVISOR_TASK_STACK_SIZE, SUPERVISOR_TASK_PRIORITY);
	if(threadInfo==NULL){
		SW_DIAG_printf("Failed to create supervisor thread\n");
	}
}

static void lwipNetworkInitCallback(void* arg){
	LWIP_UNUSED_ARG(arg);
	gLwipInitDone=true;
}

/*!
 * @brief The main function containing server thread.
 */
static void main_thread(void *arg)
{
    LWIP_UNUSED_ARG(arg);
    supervisorThreadInit();
    gLwipStopped=false;
    tcpip_init(lwipNetworkInitCallback, NULL); //this is done here because it can only be done once
    vTaskDelete(NULL);
}

void BOARD_SetRtcClockSource(void)
{
    /* Enable the RTC 32KHz oscillator */
    RTC->CR |= RTC_CR_OSCE_MASK;
}

/* todo exctend this to have +/- control values and user configurable values */
//IEC 60730 H.11.12.7 7.2.1 Analog A/D and D/A converters (plausibility) H.2.18.13
//IEC 60730 H.11.12.7 7.2.2 Analog multiplexer (ignored because we don't have one)
static bool pidValuesSafe(inxPID_t* pPID){
	bool safe=false;
	if(pPID->currentValue>=INX_SAFE_TEMPERATURE_LOWER_BOUNDS && pPID->currentValue<=INX_SAFE_TEMPERATURE_UPPER_BOUNDS){
		safe=true;
	}
	return safe;
}
static uint32_t adcrTemp25 = 0; //set when ADCs are calibrated
static uint32_t adcr100m = 0; //set when ADCs are calibrated

/*!
 * @brief ADC stop conversion
 */
static void ADC16_PauseConversion(ADC_Type *base)
{
    adc16_channel_config_t adcChnConfig;

    adcChnConfig.channelNumber = 31U; /*!< AD31 channel */
    adcChnConfig.enableInterruptOnConversionCompleted = false;
#if defined(FSL_FEATURE_ADC16_HAS_DIFF_MODE) && FSL_FEATURE_ADC16_HAS_DIFF_MODE
    adcChnConfig.enableDifferentialConversion = false;
#endif
    ADC16_SetChannelConfig(base, CONFIG_ADC_CHANNEL_GROUP0, &adcChnConfig);
}

/*!
 * @brief calibrate parameters: VDD and ADCR_TEMP25
 */
static void ADC16_CalibrateParams(ADC_Type *base,unsigned char do_bandgap_tempcal)
{
    uint32_t bandgapValue = 0; /*! ADC value of BANDGAP */
    uint32_t vdd = 0;          /*! VDD in mV */

    adc16_config_t adcUserConfig;
    adc16_channel_config_t adcChnConfig;
    pmc_bandgap_buffer_config_t pmcBandgapConfig;

    pmcBandgapConfig.enable = true;

#if (defined(FSL_FEATURE_PMC_HAS_BGEN) && FSL_FEATURE_PMC_HAS_BGEN)
    pmcBandgapConfig.enableInLowPowerMode = false;
#endif
#if (defined(FSL_FEATURE_PMC_HAS_BGBDS) && FSL_FEATURE_PMC_HAS_BGBDS)
    pmcBandgapConfig.drive = kPmcBandgapBufferDriveLow;
#endif
    /* Enable BANDGAP reference voltage */
    PMC_ConfigureBandgapBuffer(PMC, &pmcBandgapConfig);

    /*
    * Initialization ADC for
    * 16bit resolution, interrupt mode, hw trigger disabled.
    * normal convert speed, VREFH/L as reference,
    * disable continuous convert mode
    */
    /*
     * adcUserConfig.referenceVoltageSource = kADC16_ReferenceVoltageSourceVref;
     * adcUserConfig.clockSource = kADC16_ClockSourceAsynchronousClock;
     * adcUserConfig.enableAsynchronousClock = true;
     * adcUserConfig.clockDivider = kADC16_ClockDivider8;
     * adcUserConfig.resolution = kADC16_ResolutionSE12Bit;
     * adcUserConfig.longSampleMode = kADC16_LongSampleDisabled;
     * adcUserConfig.enableHighSpeed = false;
     * adcUserConfig.enableLowPower = false;
     * adcUserConfig.enableContinuousConversion = false;
     */
    ADC16_GetDefaultConfig(&adcUserConfig);
#if defined(FSL_FEATURE_ADC16_MAX_RESOLUTION) && (FSL_FEATURE_ADC16_MAX_RESOLUTION >= 16U)
    //adcUserConfig.resolution = kADC16_Resolution16Bit;
//#warning "TODO why isn't this set to 16 bit?"
#else
    adcUserConfig.resolution = kADC16_ResolutionSE12Bit;
#endif
	//we force 12 bit resolution for now
	//16 bit breaks the cpu temperature and doesn't seem to
	//make anything better
	adcUserConfig.resolution = kADC16_ResolutionSE12Bit;
    adcUserConfig.enableHighSpeed = false;
    adcUserConfig.clockDivider = kADC16_ClockDivider8;


    adcUserConfig.enableContinuousConversion = false;
    adcUserConfig.clockSource = kADC16_ClockSourceAsynchronousClock;
    adcUserConfig.enableLowPower = 1;
    adcUserConfig.longSampleMode = kADC16_LongSampleCycle24;
#undef BOARD_ADC_USE_ALT_VREF
#ifdef BOARD_ADC_USE_ALT_VREF
    adcUserConfig.referenceVoltageSource = kADC16_ReferenceVoltageSourceValt;
#else
    adcUserConfig.referenceVoltageSource = kADC16_ReferenceVoltageSourceVref;
#endif
    ADC16_Init(base, &adcUserConfig);

#if defined(FSL_FEATURE_ADC16_HAS_HW_AVERAGE) && FSL_FEATURE_ADC16_HAS_HW_AVERAGE
    /* Use hardware average to increase stability of the measurement  */
    ADC16_SetHardwareAverage(base, kADC16_HardwareAverageCount32);
#endif /* FSL_FEATURE_ADC16_HAS_HW_AVERAGE */

#if defined(FSL_FEATURE_ADC16_HAS_CALIBRATION) && FSL_FEATURE_ADC16_HAS_CALIBRATION
    /* Auto calibration */
    ADC16_DoAutoCalibration(base);
#endif

    adcChnConfig.channelNumber = CONFIG_ADC_BAND_GAP_CHANNEL;
#if defined(FSL_FEATURE_ADC16_HAS_DIFF_MODE) && FSL_FEATURE_ADC16_HAS_DIFF_MODE
    adcChnConfig.enableDifferentialConversion = false;
#endif

    adcChnConfig.enableInterruptOnConversionCompleted = false;
    ADC16_SetChannelConfig(base, CONFIG_ADC_CHANNEL_GROUP0, &adcChnConfig);

    /* Wait for the conversion to be done */
    while (!ADC16_GetChannelStatusFlags(base, CONFIG_ADC_CHANNEL_GROUP0))
    {
    }

    /* Get current ADC BANDGAP value */
    bandgapValue = ADC16_GetChannelConversionValue(base, CONFIG_ADC_CHANNEL_GROUP0);

    ADC16_PauseConversion(base);
if (do_bandgap_tempcal) {
    /* Get VDD value measured in mV: VDD = (ADCR_VDD x V_BG) / ADCR_BG */
    vdd = ADCR_VDD * V_BG / bandgapValue;
    /* Calibrate ADCR_TEMP25: ADCR_TEMP25 = ADCR_VDD x V_TEMP25 / VDD */
    adcrTemp25 = ADCR_VDD * V_TEMP25 / vdd;
    /* ADCR_100M = ADCR_VDD x M x 100 / VDD */
    adcr100m = (ADCR_VDD * INX_M) / (vdd * 10);
}

    /* Disable BANDGAP reference voltage */
    pmcBandgapConfig.enable = false;
    PMC_ConfigureBandgapBuffer(PMC, &pmcBandgapConfig);
}

/* converts 12 bit adc value to temperature for pt100 sensor using processor board pt100 input */
float convert_pt100_to_temperature_v2(const uint32_t raw_value,const float offset,const float scale) {
#define PT100_RBIAS (4700.0f*1.00f)
#define PT100_RREF (100.0f*1.00f)
#define PT100_RF (2200.0f*1.00f)
#define PT100_GAIN 23.0f
#define PT100_TEMPCOEF	0.385f
//#define ADC_FSD 0xFFFF - in 16 bit mode
#define ADC_FSD 0x0FFF
	float voltage     = ((float)(raw_value)/(float)ADC_FSD)*3.3f; // 12 bit ADC value with 3.3v reference
	float pt100_resistance = (-PT100_RREF*PT100_RBIAS*voltage)/(PT100_RREF*voltage-3.3f*PT100_RF-3.3f*PT100_RREF);
	float temperature = ((pt100_resistance - (PT100_RREF))/(PT100_TEMPCOEF+scale)) + offset;
	/* cal1 is offfset in degC after adjusting for gain.
	 * cal2 is gain adjustment +/- from nominal value */
	return temperature;
}

float convert_pt100_3W_to_temperature(const uint32_t raw_value,const float offset,const float scale) {

	// i = 3/4700 = 0.64mA
	// V = I(100+tempC*0.375)
	// tempC = (V/0.00064-100)/0.375

	float temperature = offset + (((float)raw_value/4096.0f)*3.30f-0.84074f)/(0.003277778f+scale/1000.0f);

	return temperature;
}

static float convert10VToTemperature(const uint32 in,const float offset,const float scale){
	float temp=(float)(((float)(in*100))/4096.0);
	float adjusted=(temp*(scale+1))+offset;
	return adjusted;
}

static float convert20MAToTemperature(const int32 in,const float offset,const float scale){
	float temp=(float)(((float)((in-819)*100))/3277.0);
	float adjusted=(temp*(scale+1))+offset;
	return adjusted;
}

static float convertModbusToTemperature(const uint32 in){
	return in;
}

/* Prexision opa mp gain (R1/R2) * 2
 * Vo (U6) Type K
1000/4.7 *2 * 0.0406 = 17.1277mV/C (0-190C range)
470/4.7 *2 * 0.0406 = 8.12mV/C (0-400C range)
412/4.7 *2 * 0.0406 = 7.118mV/C (0-463C range)
 */

#ifdef INX_HEATROD_SAMPLES_1_2

#define INX_HEATROD_HRC_THERMOCOUPLE_ADC_VREF_MV 3300.0f
#define INX_HEATROD_HRC_THERMOCOUPLE_MV_PER_C 17.1277f
#else
#define INX_HEATROD_HRC_THERMOCOUPLE_ADC_VREF_MV 3300.0f
#define INX_HEATROD_HRC_THERMOCOUPLE_MV_PER_C 7.118f
#endif
#define INX_HEATROD_HRC_THERMOCOUPLE_AVG_OPAMP_OFFSET_MV 0  //(INX_HEATROD_HRC_THERMOCOUPLE_MV_PER_C*10)
// 1000/4.7 *2 * 0.0406 = 17.1277mV/C
static float convertThermocoupleToTemperature(const uint32 in,const float offset,const float scale){
	float temp = (((float)in/4096.0f)*INX_HEATROD_HRC_THERMOCOUPLE_ADC_VREF_MV-INX_HEATROD_HRC_THERMOCOUPLE_AVG_OPAMP_OFFSET_MV)/INX_HEATROD_HRC_THERMOCOUPLE_MV_PER_C;
	float adjusted = (temp*(scale+1))+offset;
	//printf ("ADC= %d, Thermocouple Temp (10mV/C) = %f\r\n",in, temperature);
	return adjusted;
}


#ifdef INX_HEATROD_IOT_DIAGNOSTIC_BUILD
	int show_adc_values = -1; // Don't show anything to start with results for a bit then stop spamming the console
	char relOn = 0; // flag to switch on relays to check calibration under relay coil load
	int hw_diagnostic_test_errors = 0;
#endif

const adcRawValues_t* mainGetRawSensorValues(){
	return &gADCRawValues;
}

float getConvertedValue(const inxControllerSensors_t sensors,const adcRawValues_t* pRawValues){
	float value=0.0;
	switch(sensors){
		case INX_CONTROLLER_SENSORS_10V:
			value = convert10VToTemperature(pRawValues->zeroTenV,gNoResetSettings.zeroTenVOffset,gNoResetSettings.zeroTenVScale);
			break;
		case INX_CONTROLLER_SENSORS_20MA:
			value = convert20MAToTemperature(pRawValues->fourTwentyMa,gNoResetSettings.fourTwentyMAOffset,gNoResetSettings.fourTwentyMAScale);
			break;
		case INX_CONTROLLER_SENSORS_MODBUS:
			value = convertModbusToTemperature(0.0);
			break;
		case INX_CONTROLLER_SENSORS_THERMOCOUPLE:
			value = convertThermocoupleToTemperature(pRawValues->thermocouple1,gNoResetSettings.thermoCoupleOffset,gNoResetSettings.thermoCoupleScale);
			break;
		case INX_CONTROLLER_SENSORS_PT100_CHANNEL1:
			#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
				value = convert_pt100_3W_to_temperature(pRawValues->pt100Channel1,gNoResetSettings.pt1001Offset,gNoResetSettings.pt1001Scale);
			#else
				value = convert_pt100_to_temperature_v2(pRawValues->pt100Channel1,gNoResetSettings.pt1001Offset,gNoResetSettings.pt1001Scale);
			#endif
			break;
		case INX_CONTROLLER_SENSORS_PT100_CHANNEL2:
			value = convert_pt100_to_temperature_v2(pRawValues->pt100Channel2,gNoResetSettings.pt1002Offset,gNoResetSettings.pt1002Scale);
			break;
		default:
			break;
	}
	return value;
}

/*
 * This is called when all the ADC values are in
 * Converts the raw values to temperature values for use on control and monitoring
 */
static void convertADCValuesToRealValues(adcRawValues_t* pRawValues,volatile dutyCycleState_t* pDuty1State,adcConvertedValues_t* pConverted){

	//@todo this function or probably the oine that calls it should select which input is being read (PT100 - ch1, ch2, 0-10v, 4-20mA, or MODBUS input value)
#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_FRDMK64EVAL
	float R100; // PT100 resistance
	#define OPAMPGAIN 12.5f
	#define SOURCERESISTANCE 2200.0f
	float volt_value; //ADC voltage
	//convert temperature1
	volt_value = (pRawValues->pt100Channel1*3.3f)/4096.0f;
	R100 = (volt_value * SOURCERESISTANCE) / (12.5*3.3f-(float)volt_value);
	pConverted->pid1 = 2.56f*(R100-100.0f);

	//convert temperature2
	volt_value = (pRawValues->pt100Channel2*3.3f)/4096.0f;
	R100 = (volt_value * SOURCERESISTANCE) / (12.5*3.3f-(float)volt_value);
	pConverted->pid2 = 2.56f*(R100-100.0f);
#else // for all production versions.

//we only apply global calibrations to pid1
pConverted->pid1=applyGlobalCalibrations(getConvertedValue(gPIDs[0].sensors,pRawValues));
pConverted->pid2=getConvertedValue(gPIDs[1].sensors,pRawValues);
pConverted->pid3=getConvertedValue(gPIDs[2].sensors,pRawValues);

#ifdef DEPRICATED
	#define CALIBRATION_VALUE 1.3f
	float R100; // PT100 resistance
	float temperature=(float)pRawValues->temperature1;
	R100=(temperature/4096.0f)*100.0f;
	pConverted->temperature1 = R100;
	temperature=(float)pRawValues->temperature2;
	R100=(temperature/4096.0f)*100.0f;
	pConverted->temperature2 = R100 * CALIBRATION_VALUE;
#endif
#endif


#if DEBUG_ADC_CONVERT_VALUES
	LWIP_PLATFORM_DIAG(("temp1 %d becomes %3.1f",pRawValues->temperature1,pConverted->temperature1));
	LWIP_PLATFORM_DIAG(("temp2 %d becomes %3.1f",pRawValues->temperature2,pConverted->temperature2));
#endif

	//now convert cpuTemp1
	pConverted->cpuTemp1 = (int32_t)(CONFIG_ADC_CPU_TEMP_STANDARD_TEMP - ((int32_t)pRawValues->cpuTemp1 - (int32_t)adcrTemp25) * 100 / (int32_t)adcr100m);
	//LWIP_PLATFORM_DIAG(("cpuTemp1 raw=%d converted=%d",pRawValues->cpuTemp1,pConverted->cpuTemp1));

	/* Diagnostics */
#ifdef INX_HEATROD_IOT_DIAGNOSTIC_BUILD
//#error "DIAGNOSTIC BUILD IS ON!"
	float first_temperature_without_control_config;
	float thermocouple_without_control_config;
	float diag_0_10V = convert10VToTemperature(pRawValues->zeroTenV,gNoResetSettings.zeroTenVOffset,gNoResetSettings.zeroTenVScale); // 0C - 100C range
	float diag_4_20mA= convert20MAToTemperature(pRawValues->fourTwentyMa,gNoResetSettings.fourTwentyMAOffset,gNoResetSettings.fourTwentyMAScale); // 0C - 100C  range.
	//float diag_modbus_temperature = convertModbusToTemperature(0.0); // todo we sgould be reading this properly.


#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
	first_temperature_without_control_config = convertPT1003WChannel2ToTemperature(pRawValues->pt100Channel1);
	thermocouple_without_control_config = convertThermocoupleToTemperature(pRawValues->thermocouple1);
#else
	first_temperature_without_control_config = convert_pt100_to_temperature_v2(pRawValues->pt100Channel1,gNoResetSettings.pt1001Offset,gNoResetSettings.pt1001Scale);
	thermocouple_without_control_config = -9999.9f;
#endif

	if (show_adc_values >= 0 ) {
			if (show_adc_values < (INX_HW_DIAG_ADC_TESTS_TODO/2)) {
				if (show_adc_values == INX_HW_DIAG_ADC_TESTS_TODO/2-1 ) {
					relOn = 1;
					HW_DIAG_printf("Switching on 3 Internals Relays:\n");
					GPIO_PortClear(CONFIG_GPIO_CONTROL_BASE_OUT, 1U << CONFIG_GPIO_CONTROL_INTERNAL_RELAY_1);
					GPIO_PortClear(CONFIG_GPIO_CONTROL_BASE_OUT, 1U << CONFIG_GPIO_CONTROL_INTERNAL_RELAY_2);
					GPIO_PortClear(CONFIG_GPIO_CONTROL_BASE_OUT, 1U << CONFIG_GPIO_CONTROL_INTERNAL_RELAY_3);
				}
			}
			show_adc_values--;
			//HW_DIAG_printf (/*"|%d|"*/"T1(%4d:%5.1fC)|T2(%4d:%5.1fC)|TCpl(%4d:%5.1fC)|10v(%4d:%5.1fC)|20mA(%4d:%5.1fC)|cpu(%3.1fC)|\r\n",
			HW_DIAG_printf ("|[%s]|T1=%5.1fC|T2=%5.1fC|Tcpl=%5.1fC(Raw=%d)|10v=%5.1fC|20mA=%5.1fC|cpu=%3.1fC|Time=%d|\r\n",
					relOn?"REL= ON":"REL=OFF",
					//show_adc_values+1,
					//pRawValues->pt100Channel1,
					first_temperature_without_control_config,
					//pRawValues->pt100Channel2,
					pConverted->temperature2,
					//pRawValues->thermocouple1,
					thermocouple_without_control_config,
					pRawValues->thermocouple1,
					//pRawValues->zeroTenV,
					diag_0_10V,
					//pRawValues->fourTwentyMa,
					diag_4_20mA,
					(float)(pConverted->cpuTemp1),
					inxRTCGetSeconds()
			);

			if (show_adc_values == -1) {
				//count up errors on last posted results only
				if (first_temperature_without_control_config > 20.0f || first_temperature_without_control_config < -30.0f ) {
					HW_DIAG_printf("HW DIAG: PT100 Sensor 1  Range Error Detected - RAW values = %d\n",pRawValues->pt100Channel1);
					hw_diagnostic_test_errors++;
				}
				if (pConverted->temperature2 > 20.0f || pConverted->temperature2 < -30.0f ) {
					HW_DIAG_printf("HW DIAG: PT100 Sensor 2  Range Error Detected - RAW values = %d\n",pRawValues->pt100Channel2);
					hw_diagnostic_test_errors++;
				}
#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
				if (thermocouple_without_control_config > 30.0f || thermocouple_without_control_config < 10.0f ) {
					HW_DIAG_printf("HW DIAG: Thermo couple range Error detected  - RAW values = %d\n",pRawValues->thermocouple1);
					hw_diagnostic_test_errors++;
				}
#endif
				if (diag_0_10V < 10.0f || diag_0_10V > 20.0f ) {
					HW_DIAG_printf("HW DIAG: 0-10v Sensor Range Error Detected - RAW values = %d\n",pRawValues->zeroTenV);
					hw_diagnostic_test_errors++;
				}
				if (diag_4_20mA < -17.0f || diag_4_20mA > -5.0f ) {
					HW_DIAG_printf("HW DIAG: 4-20mA Sensor Range Error Detected - RAW values = %d\n",pRawValues->fourTwentyMa);
					hw_diagnostic_test_errors++;
				}

				HW_DIAG_printf("Switching off 3 Internals Relays...\n");
				GPIO_PortSet(CONFIG_GPIO_CONTROL_BASE_OUT, 1U << CONFIG_GPIO_CONTROL_INTERNAL_RELAY_1);
				GPIO_PortSet(CONFIG_GPIO_CONTROL_BASE_OUT, 1U << CONFIG_GPIO_CONTROL_INTERNAL_RELAY_2);
				GPIO_PortSet(CONFIG_GPIO_CONTROL_BASE_OUT, 1U << CONFIG_GPIO_CONTROL_INTERNAL_RELAY_3);
				relOn = 0; // just in case we might loop this test one day
				HW_DIAG_printf("______________________________________\n");
				if (hw_diagnostic_test_errors < 1) {
					HW_DIAG_printf("All Hardware Diagnostic Test Complete! \n*** NO ERRORS DETECTED *** (Assumed visual Displays inspected)\n");
				}
				else {
					HW_DIAG_printf("All Hardware Diagnostic Test Complete \n*** ERRORS FOUND = %d ***\n",hw_diagnostic_test_errors);
				}
				for (int i = 0 ; i < 20;i++) {
					inxDisplaySet(0x0F,0X0F,0x0F,hw_diagnostic_test_errors>9?9:hw_diagnostic_test_errors,false);
					inxDiagnosticWaitLongTime(10);
					inxDisplaySet(3,3,3,hw_diagnostic_test_errors>9?9:hw_diagnostic_test_errors,false);
					inxDiagnosticWaitLongTime(1);
				}
				HW_DIAG_printf("______________________________________\n");
			}
	}
#endif
}

/*static pidConditionStateDiff_t adcConditionGetDiff(const pidConditionState_t* pState){
	uint32_t latestValuesIndex=pState->valuesIndex;
	if(latestValuesIndex==0){
		//wrap around to the end of the buffer
		latestValuesIndex=CONFIG_PID_CONDITION_MONITORING_WINDOW_SAMPLES-1;
	}else{
		//write ahead is always one ahead of the read head
		latestValuesIndex--;
	}
	uint32_t earliestValuesIndex = (latestValuesIndex + 1); //we can do this because the value in front of the latestValue is the next one to be overwritten, therefore the oldest
	if(earliestValuesIndex>=CONFIG_PID_CONDITION_MONITORING_WINDOW_SAMPLES){
		earliestValuesIndex=0;
	}

	const float latestValues=pState->values[latestValuesIndex];
	const float earliestValues=pState->values[earliestValuesIndex];
	pidConditionStateDiff_t diffs={
		latestValues - earliestValues,
		earliestValues
	};
	return diffs;
}*/


static void adcConditionMonitoring(volatile pidConditionState_t* pState,const adcConvertedValues_t* pLatestValues,volatile dutyCycleState_t* pDutyCycleState){
	//start with just calculating the rate of change over our condition monitoring window
#define INX_IGNORE_SENSOR_ERRORS
#ifdef INX_IGNORE_SENSOR_ERRORS
	pState->mode=INX_PID_CONDITION_OK;
#else
	//take a copy of our latest values
	memcpy(&pState->values[pState->valuesIndex],pLatestValues,sizeof(adcConvertedValues_t));
	//move our write head
	pState->valuesIndex++;
	if(pState->valuesIndex>=CONFIG_ADC_CONDITION_MONITORING_WINDOW_SAMPLES){
		pState->valuesIndex=0;
	}

	switch(pState->mode){
		case INX_ADC_CONDITION_INIT:
			//the first time we reach 0 is the first time we have a full rolling buffer
			if(pState->valuesIndex==0){
				pState->mode=INX_ADC_CONDITION_OK;
			}
			break;
		case INX_ADC_CONDITION_OK: ; //this semi colon is here we can have a declaration in a statement
			//we can calculate our rolling differential
			adcConditionStateDiff_t diffs=adcConditionGetDiff(pState);
			LWIP_DEBUGF(CONFIG_ADC_CONDITION_MONITORING_DEBUG,("adcconmon:tempDiff=%.2f",diffs.tempDiff));

			if(diffs.tempDiff>CONFIG_ADC_CONDITION_MONITORING_MAX_TEMP_DIFF || diffs.tempDiff<CONFIG_ADC_CONDITION_MONITORING_MIN_TEMP_DIFF){
				pDutyCycleState->conditionBad=true;
				dutyCycleStateOffForced(pDutyCycleState);
				//log the temperature of when this happened
				pState->temp1AtBad=diffs.earliestTemp;
				pState->mode=INX_ADC_CONDITION_BAD;
				pState->samplesSinceBad=0;
				LWIP_DEBUGF(CONFIG_ADC_CONDITION_MONITORING_DEBUG,("adcconmon:disabled power temp1AtBad=%.2f",pState->temp1AtBad));
			}else if(pState->retries>0){
				//monitor if we can reset our retries or not
				pState->samplesSinceBad++;
				if(pState->samplesSinceBad>=CONFIG_ADC_CONDITION_MONITORING_RETRY_SAMPLES){
					pState->samplesSinceBad=0;
					pState->retries=0;
					LWIP_DEBUGF(CONFIG_ADC_CONDITION_MONITORING_DEBUG,("adcconmon:retries=%d",pState->retries));
				}
			}
			break;
		case INX_ADC_CONDITION_BAD:
			//count for our allotted back off time then check to see if safe to turn power back on
			pState->samplesSinceBad++;
			//TODO more intelligence for when to turn the heater back on
			if(pState->samplesSinceBad>=CONFIG_ADC_CONDITION_MONITORING_RETRY_SAMPLES){
				pState->retries++;
				if(pState->retries>CONFIG_ADC_CONDITION_MONITORING_RETRY_LIMIT){
					pState->mode=INX_ADC_CONDITION_DISABLED;
					SW_DIAG_printf("adcconmon:disabled power permanently\n");
					errorLogsWrite(ERROR_LOGS_ADC_DISABLED);
				}else{
					pDutyCycleState->conditionBad=false;
					pState->samplesSinceBad=0;
					pState->mode=INX_ADC_CONDITION_OK;
					SW_DIAG_printf(CONFIG_ADC_CONDITION_MONITORING_DEBUG,"adcconmon:enabling power retries=%d",pState->retries);
				}
			}
			break;
		case INX_ADC_CONDITION_DISABLED:
			//they will need to reset us
			break;
	}
#endif
}

//we assume this method is only ever called by the adc interrupt handler thread
volatile static bool gAdc0Done=0;
volatile static bool gAdc1Done=0;
static inxIecInterruptData_t gInxIecInterruptData={0,0};
//static uint32_t gADCTicksSinceLastSelfTest=CONFIG_IEC_PERIODIC_TEST_INTERVAL_SECONDS; //do a test immediately
static volatile bool gADCBuffersReady=false;

uint32_t inxTimerSecondsToTicks(const uint32_t seconds){
	return (seconds*1000/(gLPTMRInterruptMilliseconds*CONFIG_LPTMR_NUM_READS));
}

uint32_t inxTimerTicksToSeconds(const uint32_t ticks){
	return (ticks*gLPTMRInterruptMilliseconds*CONFIG_LPTMR_NUM_READS)/1000;
}

static void inxPIDRampRun(inxPID_t* pPID){
	const uint32_t rtcSeconds=inxRTCGetSeconds();
	if(rtcSeconds>=pPID->rampSecondsAtNextIncrement){
		float newDesired=pPID->rampDesiredTemp+1;
		//deal with if we have reached desired temp or if we have instant ramp
		if(newDesired>pPID->desiredTemp || pPID->rampSecondsPerDegree==0){
			newDesired=pPID->desiredTemp;
		}
		inxSetPIDRampDesired(pPID,newDesired,false);
	}
}

static void inxTimerRun(const uint8_t i,bool* pTimerBusy){
	if(inxRTCGetSeconds()>=gPIDs[i].timerSecondsAtFinish || gRTCStuck){
		gPIDs[i].timerSecondsRemaining=0;
		gPIDs[i].timerState=INX_TIMER_STATE_FINISHED;
	}else{
		gPIDs[i].timerSecondsRemaining=gPIDs[i].timerSecondsAtFinish-inxRTCGetSeconds();
	}
	*pTimerBusy=true;
}

static void inxTimerFinish(const uint8_t i){
	gPIDs[i].timerSecondsRemaining=0;
	updateDesiredTemp(&gPIDs[i],0.0);
	gPIDs[i].timerState=INX_TIMER_STATE_READY;
}

static void inxTimerStart(const uint8_t i,bool* pTimerBusy){
	gPIDs[i].timerSecondsRemaining=gPIDs[i].timerDurationSeconds;
	gPIDs[i].timerSecondsAtFinish=inxRTCGetSeconds()+gPIDs[i].timerDurationSeconds;
	updateDesiredTemp(&gPIDs[i],gPIDs[i].manualSetPoint);
	*pTimerBusy=true;
	switch(i){
		case 0:
			gButtonState=INX_BUTTON_STATE_DISPLAY_TIMER;
			break;
		default:
			break;
	}
	gPIDs[i].timerState=INX_TIMER_STATE_RUNNING;
}
typedef double ehs_float;
bool EhsADCReadEvent(const uint8_t channel,const ehs_float value);
static void inxADCEHSRead(){
	for(uint8_t i=INX_CONTROLLER_SENSORS_PT100_CHANNEL1;i<=INX_CONTROLLER_SENSORS_THERMOCOUPLE;i++){
		EhsADCReadEvent(i,getConvertedValue(i,&gADCRawValues));
	}
}

static void adcSampleProcessing(){
	//LWIP_PLATFORM_DIAG(("adcSampleProcessing %d %d %d %d",gAdc0Done,gAdc1Done,gInxIecInterruptData.doingTest,gInxIecInterruptData.interruptCount));
	if(gAdc0Done && gAdc1Done){
		gAdc0Done=0;
		gAdc1Done=0;

		//if we are in diagnostic mode then just clear the flag and stop
		if(gInxIecInterruptData.doingTest){
			gInxIecInterruptData.interruptCount++;
			return;
		}
		inxWatchDogKick(INX_WATCH_DOG_KICK_ADC);

		//convert the ADC values to actual temperatures and voltages
		convertADCValuesToRealValues(&gADCRawValues,&dutyCycleState0,&gValues.values[nValueIndex]);
		inxADCEHSRead();
		adcConditionMonitoring(&gAdcConditionState,&gValues.values[nValueIndex],&dutyCycleState0);

		//post a message with this information
		adcValuesQueueMessage_t message;
		message.numElements=gAdcCurrentElementCount;
		message.duty1On=dutyCycleState0.on;
		message.duty2On=dutyCycleState1.on;
		message.duty3On=dutyCycleState2.on;
		message.log=false;
		gADCSamplesCount++;
		if(gADCSamplesCount>=gADCSamplesPerSecond){
			gADCSamplesCount=0;
			message.log=true;
		}
		BaseType_t success=xQueueSendToBackFromISR(gAdcValuesQueue,&message,NULL);
		if(success!=pdPASS){
			SW_DIAG_printf("adcconmon: failed to put duty message on queue\n");
		}

		//handle any PID timers
		bool timerBusy=false;
		for(u8_t i=0;i<CONFIG_PID_CONTROLLERS_NUM;i++){
			inxPIDRampRun(&gPIDs[i]);
			switch(gPIDs[i].scheduleMode){
				case SCHEDULE_MODE_TIMER:
					switch(gPIDs[i].timerState){
						case INX_TIMER_STATE_START:
							inxTimerStart(i,&timerBusy);
						case INX_TIMER_STATE_RUNNING:
							inxTimerRun(i,&timerBusy);
							break;
						case INX_TIMER_STATE_FINISHED:
							inxTimerFinish(i);
							break;
						default:
							break;
					}
					break;
				case SCHEDULE_MODE_TIMER_SET_POINT:
					switch(gPIDs[i].timerState){
						case INX_TIMER_STATE_START:
							inxTimerStart(i,&timerBusy);
							break;
						case INX_TIMER_STATE_GOING_TO_SET_POINT:
							//if we are at the set point or over then start the timer
							if(gPIDs[i].currentValue>=gPIDs[i].desiredTemp){
								gPIDs[i].timerState=INX_TIMER_STATE_START;
							}
							break;
						case INX_TIMER_STATE_RUNNING:
							inxTimerRun(i,&timerBusy);
							break;
						case INX_TIMER_STATE_FINISHED:
							inxTimerFinish(i);
							break;
						default:
							break;
					}
					break;
				default:
					break;
			}
		}
		if(timerBusy){
			
		}else{
			switch(gButtonState){
				//if the user is looking at the temperature show them the finished time
				case INX_BUTTON_STATE_DISPLAY_TIMER_TEMPERATURE:
					gButtonState=INX_BUTTON_STATE_DISPLAY_TIMER;
					break;
				default:
					break;
			}
		}

		if(		(gADCBuffersReady==true) //so that we only force a cycle update when we have fully booted up
			&& 	(gControllerOutputUpdated==true || gDynamicSetPointUpdated==true)){
			//this forces next nValueIndex increment to trigger power recalculation
			nValueIndex = (INX_SAMPLEBUF_SIZE-1);
			//samples and duty log must be kept in sync or we get stale values back in to devman
			dutyCycleHistory0.powerHistoryHead = CONFIG_DUTY_CYCLE_HISTORY_LENGTH-1;
			if(gDynamicSetPointUpdated==true){
				//if power required > 0 and relay is off then we need to force
				//reset of cycle so power comes on
				for(u8_t i=0;i<CONFIG_PID_CONTROLLERS_NUM;i++){
					if(gPIDs[i].powerPercent>0){
						volatile dutyCycleState_t* pDutyCycleState=&dutyCycleState0;
						switch(i){
							case 1:
								pDutyCycleState=&dutyCycleState1;
								break;
							case 2:
								pDutyCycleState=&dutyCycleState2;
								break;
						}
						if(pDutyCycleState->on==false){
							gControllerOutputUpdated=true;
						}
					}
				}
			}
			if(gControllerOutputUpdated){
				//our controller output mode was changed so reset the tick count to force a quick update of elements and power
				//force everything off since we don't know what output mode we'll be using when the power comes back on
				inxIecShutOffAllRelays(false);
				dutyCycleState0.numInterrupts=dutyCycleState0.cycleInterruptLength-2;
				dutyCycleState1.numInterrupts=dutyCycleState1.cycleInterruptLength-2;
				dutyCycleState2.numInterrupts=dutyCycleState2.cycleInterruptLength-2;
				updateLPTMRInterruptMilliseconds(false);
			}
		}else if(gADCBuffersReady==false){
			memcpy(&gValues.current,&gValues.values[nValueIndex],sizeof(gValues.current));
		}

		//increment our buffer index
		nValueIndex++;
		if (nValueIndex >= INX_SAMPLEBUF_SIZE) {
			nValueIndex = 0;
			gControllerOutputUpdated=false;
			gDynamicSetPointUpdated=false;
			calculateAverageAndVariance(&gValues);
			writeGlobalADCValues(&gValues);
			gADCValuesReady = true;
			//assume all elements should be off
			dutyCycleStateSetPowerPercent(&dutyCycleState0,0,0);
			dutyCycleStateSetPowerPercent(&dutyCycleState1,0,0);
			dutyCycleStateSetPowerPercent(&dutyCycleState2,0,0);
			for(u8_t i=0;i<CONFIG_PID_CONTROLLERS_NUM;i++){
				//we must do this after calculatingAverageAndVariance so that pid values are accurate
				if(gADCBuffersReady==false){
					//this is the first full set of data so update ramp starting point
					inxSetPIDRampDesired(&gPIDs[i],gPIDs[i].currentValue+1,false);
				}
				if(pidValuesSafe(&gPIDs[i])){
					//recalculate required power
					gADCValuesUnsafe=false;
				}else{
					//shut off the power
					//printf("##==== Unsafe values = %f\n",gValues.adc0.temperature);
					gADCValuesUnsafe=true;
					dutyCycleStateSetPowerPercent(&dutyCycleState0,0,0);
					dutyCycleStateSetPowerPercent(&dutyCycleState1,0,0);
					dutyCycleStateSetPowerPercent(&dutyCycleState2,0,0);
					inxIecShutOffAllRelays(false);
				}
			}
			gADCBuffersReady=true;
		}else {
			if(gADCBuffersReady){
				//calculate new averages and update our global
				calculateAverageAndVariance(&gValues);
			}
		}

		//uint32_t ticksToWait=(CONFIG_MAX_MILLISECONDS_BETWEEN_SAVES/(gLPTMRInterruptMilliseconds*CONFIG_LPTMR_NUM_READS));
		//printf("gTicks=%d ticksToWait=%d\r\n",gTicksSinceSettingsSave,ticksToWait);
		gTicksSinceSettingsSave++;
		if(gTicksSinceSettingsSave >= (CONFIG_MAX_MILLISECONDS_BETWEEN_SAVES/(gLPTMRInterruptMilliseconds*CONFIG_LPTMR_NUM_READS))){
			gTicksSinceSettingsSave=0;
			//so that we send back the relay state
			gGenerateDiagnosticReport=true;
			gSettingsUpdatedByUser=true;
		}

#if INX_HEATROD_HEALTHCHECKTESTS == 1
		//IEC 60730 count time between periodic self tests
		if(message.log==true){
			//only tick every second, which is when we are instructed to log
			gADCTicksSinceLastSelfTest++;
			if(gADCTicksSinceLastSelfTest>=CONFIG_IEC_PERIODIC_TEST_INTERVAL_SECONDS){
				gADCTicksSinceLastSelfTest=0;
				ENET_EnableSleepMode(ENET,true);
				inxIecPeriodicTest(gCrcValue,gProgramSizeBytes,&adc0ChnConfig,&adc1ChnConfig);
				ENET_EnableSleepMode(ENET,false);
			}
		}
#endif
		//GPIO_PortToggle(GPIOB, 1u << CONFIG_GPIO_OUTPUT2);
	}
}

void ADC0_IRQHandler(void)
{
	uint32_t adcRTCTicks=gADCRTCTicks+1;
	if(adcRTCTicks>65536){
		//avoid wrap around
		gADCRTCLastGoodSeconds=inxRTCADCGetSeconds();
		adcRTCTicks=0;
	}
	gADCRTCTicks=adcRTCTicks;
	//TODO the adc handlers should do nothing until the supervisor thread is ready to receive messages
	/*uint32_t diffSeconds=RTC->TSR-gLastRtcSeconds;
	if(diffSeconds>=30){
		gLastRtcSeconds=RTC->TSR;
		GPIO_PortToggle(GPIOB, 1u << CONFIG_GPIO_OUTPUT1);
	}*/
#ifdef INX_TOGGLE_JITTER_PIN
	GPIO_PortToggle(GPIOB, 1u << CONFIG_GPIO_OUTPUT1);
#endif
	//LWIP_PLATFORM_DIAG(("adc0"));
	/* Read to clear COCO flag. */
	if (ADC16_GetChannelStatusFlags(CONFIG_ADC_BASE_0, CONFIG_ADC_CHANNEL_GROUP0))
	{
#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
		if(adc0ChnConfig.channelNumber == CONFIG_ADC0_DM0_3WIREPT100_CHANNEL && adc0ChnConfig.enableDifferentialConversion==false){
			gADCRawValues.pt100Channel1 = ADC16_GetChannelConversionValue(CONFIG_ADC_BASE_0, CONFIG_ADC_CHANNEL_GROUP0);
			adc0ChnConfig.enableDifferentialConversion=false;
			adc0ChnConfig.channelNumber = CONFIG_ADC0_DP1_420MA;
		}
#else
		if(adc0ChnConfig.channelNumber == CONFIG_ADC0_DP0_PT100_CHANNEL1 && adc0ChnConfig.enableDifferentialConversion==false){
			gADCRawValues.pt100Channel1 = ADC16_GetChannelConversionValue(CONFIG_ADC_BASE_0, CONFIG_ADC_CHANNEL_GROUP0);
			adc0ChnConfig.enableDifferentialConversion=false;
			adc0ChnConfig.channelNumber = CONFIG_ADC0_DP1_420MA;
		}
#endif
		else if(adc0ChnConfig.channelNumber == CONFIG_ADC0_DP1_420MA && adc0ChnConfig.enableDifferentialConversion==false){
			gADCRawValues.fourTwentyMa = ADC16_GetChannelConversionValue(CONFIG_ADC_BASE_0, CONFIG_ADC_CHANNEL_GROUP0);
			adc0ChnConfig.enableDifferentialConversion=false;
			adc0ChnConfig.channelNumber = CONFIG_ADC0_DM1_THERMOCOUPLE1_CHANNEL;
		}
		else{ // Not checking it's set to thermocouple, but will assume so...
			gADCRawValues.thermocouple1 = ADC16_GetChannelConversionValue(CONFIG_ADC_BASE_0, CONFIG_ADC_CHANNEL_GROUP0);
			adc0ChnConfig.enableDifferentialConversion=false;
#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
			adc0ChnConfig.channelNumber = CONFIG_ADC0_DM0_3WIREPT100_CHANNEL;
#else
			adc0ChnConfig.channelNumber = CONFIG_ADC0_DP0_PT100_CHANNEL1;
#endif
			gAdc0Done=1;
			adcSampleProcessing();
		}
		ADC16_SetChannelConfig(ADC0,  CONFIG_ADC_CHANNEL_GROUP0, &adc0ChnConfig);
	}else{
		//LWIP_PLATFORM_DIAG(("adc0 failed conversion"));
	}
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}

//TODO how do we know which ADC channel to read for which hardware and for which control sensor?

static uint32_t gFalseAlarmTicks=0;
void ADC1_IRQHandler(void)
{
	//GPIO_PortToggle(GPIOB, 1u << CONFIG_GPIO_OUTPUT2);
	//Read to clear COCO flag.
	//LWIP_PLATFORM_DIAG(("adc1"));
	if (ADC16_GetChannelStatusFlags(CONFIG_ADC_BASE_1, CONFIG_ADC_CHANNEL_GROUP0))
	{
		//ADC16_GetChannelConversionValue(CONFIG_ADC_BASE_1, DEMO_ADC_CHANNEL_GROUP0);
		if(adc1ChnConfig.channelNumber==CONFIG_ADC1_DP0_PT100_CHANNEL2){
			gADCRawValues.pt100Channel2 = ADC16_GetChannelConversionValue(CONFIG_ADC_BASE_1, CONFIG_ADC_CHANNEL_GROUP0);
			adc1ChnConfig.channelNumber=CONFIG_ADC1_DP1_010V;
		}else if(adc1ChnConfig.channelNumber==CONFIG_ADC1_DP1_010V){
			gADCRawValues.zeroTenV = ADC16_GetChannelConversionValue(CONFIG_ADC_BASE_1, CONFIG_ADC_CHANNEL_GROUP0);
			adc1ChnConfig.channelNumber=CONFIG_ADC_CPU_TEMP_CHANNEL;
		}else{
			gADCRawValues.cpuTemp1=ADC16_GetChannelConversionValue(CONFIG_ADC_BASE_1, CONFIG_ADC_CHANNEL_GROUP0);
			adc1ChnConfig.channelNumber=CONFIG_ADC1_DP0_PT100_CHANNEL2;
			gAdc1Done=1;
			adcSampleProcessing();
		}
		ADC16_SetChannelConfig(ADC1,  CONFIG_ADC_CHANNEL_GROUP0, &adc1ChnConfig);
	}else{
		//PBB TODO not sure why this code is here, should be the error handler for adc1
		/*gADCRawValues.duty2 = ADC16_GetChannelConversionValue(CONFIG_ADC_BASE_1, CONFIG_ADC_CHANNEL_GROUP1);
		adc1ChnConfig.channelNumber=CONFIG_ADC_USER_CHANNEL0;
		gAdc1Done=1; //TODO are these thread safe?
		//LWIP_PLATFORM_DIAG(("adc1 irq %d %d %d",RTC->TSR,values[nValueIndex][1],values[nValueIndex][3]));
		adcSampleProcessing();*/
		//LWIP_PLATFORM_DIAG(("adc1 failed conversion"));
	}
#ifndef INX_HEATROD_IOT_DIAGNOSTIC_BUILD
	//IEC 60730 H.11.12.7 7.1 I/O Periphery H.2.18.13
	inxIecGPIOState_t expectedState = INX_IEC_GPIO_OFF;
	//verify that our expected state matches our real state
	if(dutyCycleState0.on){
		expectedState = INX_IEC_GPIO_OFF;
	}else{
		expectedState = INX_IEC_GPIO_ON;
	}
	bool heater1Ok=inxIecGPIOCheck(&gRelayErrorState,INX_IEC_GPIO_HEATER_ONE_TEST,expectedState,dutyCycleState0.ticksSinceChange,gMinTicksSinceHeaterChange);

	if(dutyCycleState1.on){
		expectedState = INX_IEC_GPIO_OFF;
	}else{
		expectedState = INX_IEC_GPIO_ON;
	}
	//override for when relay 2 does not have correct gpio readings
	switch(gPIDs[0].elementMode){
		case ELEMENT_MODE_SINGLE_TWO_POLE:
			expectedState = INX_IEC_GPIO_ON;
			break;
		default:
			break;
	}
	bool heater2Ok=inxIecGPIOCheck(&gRelayErrorState,INX_IEC_GPIO_HEATER_TWO_TEST,expectedState,dutyCycleState1.ticksSinceChange,gMinTicksSinceHeaterChange);

	if(dutyCycleState2.on){
		expectedState = INX_IEC_GPIO_OFF;
	}else{
		expectedState = INX_IEC_GPIO_ON;
	}
	bool heater3Ok=inxIecGPIOCheck(&gRelayErrorState,INX_IEC_GPIO_HEATER_THREE_TEST,expectedState,dutyCycleState2.ticksSinceChange,gMinTicksSinceHeaterChange);
	gFalseAlarmTicks++;
	if(gFalseAlarmTicks>100){
		gFalseAlarmTicks=0;
	}
	//read the thermal CO so we update the global state in case we are asked for a report
	//thermal CO should follow relay 1 unless it has tripped in which case it should always be off
	expectedState=INX_IEC_GPIO_ON;
	if(dutyCycleState0.on){
		expectedState=INX_IEC_GPIO_OFF;
	}

//in 3 phase 415 mode we need to check the TCO while relay 3 is off
//need to modify the duty state so that relays 2 and 3 turn off gMinTicks before relay 1 so that we can test relay 1
//on it's own
//then the only problem is in 100% duty mode relay 3 is never off
//therefore we need to intermittently knock relay 2 and 3 off so we can test the thermal CO
//best way of doing that is with a separate counter on the duty state that counts how long a relay has been on continuously
//if it gets above the limit for that relay then it tweaks down the ticks needed by gMinTicks

	bool thermalCOOk=inxIecGPIOCheck(&gRelayErrorState,INX_IEC_GPIO_THERMAL_CO_TEST,expectedState,0,0);
	pwmcalc_printf("-->IQ:");
	switch(gGpioConditionState.mode){
		case INX_GPIO_CONDITION_OK:
			//pwmcalc_printf("COND=GOOD -");
			if(heater1Ok && heater2Ok && heater3Ok && thermalCOOk){
				pwmcalc_printf("3heaters good\n");
				//dutyCycleStateTick(&dutyCycleState0);
				//dutyCycleStateTick(&dutyCycleState1);
				//dutyCycleStateTick(&dutyCycleState2);
			}else{
				//bad state
				pwmcalc_printf("STATE=BAD,");
				inxGPIOFaultDetected(&gRelayErrorState,heater1Ok,heater2Ok,heater3Ok,thermalCOOk); // what does this actually do?
			}
			break;
		case INX_GPIO_CONDITION_DISABLED:
			pwmcalc_printf("COND=DISABLED,");
			if(gFalseAlarmTicks==0){
				//indicate that the relay state should be sent back to head quarters
				gRelayErrorState.time = inxRTCGetSeconds();
			}
			break;
		case INX_GPIO_CONDITION_DIAGNOSTICS:
			pwmcalc_printf("COND=DIAGNOSTICS,");
			//LWIP_PLATFORM_DIAG(("Not doing anything till diagnostics are over"));
			break;
		default:
			LWIP_PLATFORM_DIAG(("Should never get here"));
			pwmcalc_printf("COND=WANK,");
			inxGPIOFaultDetected(&gRelayErrorState,false,false,false,false);
			break;
	}
	pwmcalc_printf("IQ-->\n");
#endif // DIAGNOSTIC BUILD DONT INCLUDE
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}

void BOARD_ConfigTriggerSource(void)
{
    /* Configure SIM for ADC hw trigger source selection */
	//trigger adc0 with the low power timer and trigger adc1 with the low power timer
    SIM->SOPT7 |= (0x8eu | 0x8e00u);
    //trigger adc1 with the low power timer
	//SIM->SOPT7 |= ;
}
/* Enable the trigger source of LPTimer */
void LPTMR_InitTriggerSourceOfAdc(LPTMR_Type *base)
{
    lptmr_config_t lptmrUserConfig;

    /*
     * lptmrUserConfig.timerMode = kLPTMR_TimerModeTimeCounter;
     * lptmrUserConfig.pinSelect = kLPTMR_PinSelectInput_0;
     * lptmrUserConfig.pinPolarity = kLPTMR_PinPolarityActiveHigh;
     * lptmrUserConfig.enableFreeRunning = false;
     * lptmrUserConfig.bypassPrescaler = true;
     * lptmrUserConfig.prescalerClockSource = kLPTMR_PrescalerClock_1;
     * lptmrUserConfig.value = kLPTMR_Prescale_Glitch_0;
     */
    LPTMR_GetDefaultConfig(&lptmrUserConfig);
    /* Init LPTimer driver */
    LPTMR_Init(base, &lptmrUserConfig);

    /* Set the LPTimer period */
    if(gLPTMRInterruptMilliseconds==0){
    	gLPTMRInterruptMilliseconds=CONFIG_LPTMR_DEFAULT_INTERRUPT_MILLISECONDS;
    }
    LPTMR_SetTimerPeriod(base, gLPTMRInterruptMilliseconds);

    /* Start the LPTimer */
    LPTMR_StartTimer(base);

    /* Configure SIM for ADC hw trigger source selection */
    BOARD_ConfigTriggerSource();
}


/*!
 * @brief Initialize the ADCx for Hardware trigger.
 *
 * We don't use this mode for HRx
 */
bool ADC16_InitHardwareTrigger(ADC_Type *base,adc16_channel_config_t* adcChnConfig)
{
    adc16_config_t adcUserConfig;

    //start with default config
    ADC16_GetDefaultConfig(&adcUserConfig);
    ADC16_Init(base, &adcUserConfig);

    //calibrate
    ADC16_DoAutoCalibration(base);
    //then apply our config options
    adcUserConfig.clockSource = kADC16_ClockSourceAsynchronousClock;

	#if ((defined BOARD_ADC_USE_ALT_VREF) && BOARD_ADC_USE_ALT_VREF)
		adcUserConfig.referenceVoltageSource = kADC16_ReferenceVoltageSourceValt;
	#endif
	ADC16_Init(base, &adcUserConfig);
    ADC16_EnableHardwareTrigger(base, true);
    ADC16_SetHardwareAverage(base,kADC16_HardwareAverageCount32);
    /* set the first conversion channel number in the sequence */
#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
    adcChnConfig->channelNumber = CONFIG_ADC0_DM0_3WIREPT100_CHANNEL;
#else
    adcChnConfig->channelNumber = CONFIG_ADC0_DP0_PT100_CHANNEL1;
#endif
#if defined(FSL_FEATURE_ADC16_HAS_DIFF_MODE) && FSL_FEATURE_ADC16_HAS_DIFF_MODE
    adcChnConfig->enableDifferentialConversion = false;
#endif
    adcChnConfig->enableInterruptOnConversionCompleted = true;

    /* Configure channel 0 */
    ADC16_SetChannelConfig(base,  CONFIG_ADC_CHANNEL_GROUP0, adcChnConfig);


    return true;
}

/*static bool testFlashWrite(){
	bool success=false;
	status_t result;
	uint32_t testData[4];
	result=writeToFlash(&testData,sizeof(testData),0);
	if (kStatus_FTFx_Success == result){
		success=true;
	}
	return success;
}*/

//we only get about 256 clock cycles here before the cpu resets
//no printf works, breakpoints don't work, basically all you can do is write values to memory addresses
//and read them back on the next boot
void WDOG_EWM_IRQHandler(void)
{
	inxIecShutOffAllRelays(true);
	return;
}

/*
const char clientProvisioningCert[] = "-----BEGIN CERTIFICATE-----\n"
"MIIDRzCCAi8CAQEwDQYJKoZIhvcNAQELBQAwUTELMAkGA1UEBhMCVUsxGDAWBgNV\n"
"BAgMD1NvdXRoIFlvcmtzaGlyZTESMBAGA1UEBwwJU2hlZmZpZWxkMRQwEgYDVQQK\n"
"DAtpbnggbGltaXRlZDAeFw0xOTEyMTAwMDQ1MzBaFw0yOTEyMDcwMDQ1MzBaMIGB\n"
"MQswCQYDVQQGEwJVSzEYMBYGA1UECAwPU291dGggWW9ya3NoaXJlMRIwEAYDVQQH\n"
"DAlTaGVmZmllbGQxFDASBgNVBAoMC2lueCBsaW1pdGVkMQwwCgYDVQQLDANpbngx\n"
"IDAeBgNVBAMMF2RldnBvcnQuaW54LXN5c3RlbXMuY29tMIIBIjANBgkqhkiG9w0B\n"
"AQEFAAOCAQ8AMIIBCgKCAQEAw3bb/7NGzSvUCkXyYpMbM27U3vQCRCeU/rkY13Sp\n"
"RB2MVxJa6Z0q0C4b+5bmU9FPvEn9OfI0iadsoEN/7czvCBC0YSRHofdWg+i9b/nN\n"
"dyXVaboiPYk0rK8RuopcNMIrakiAKCUJP7CriTMMspP2SEDyoyCy1u93fllfD27D\n"
"XIHQ6slJrago1dhiHNKZD/Fj6WI3NcJY5Jx1jDyDKAQMBcXxYSnhvLs9t9H+nrNx\n"
"zEP0J9vaFfyzmFolhOagyTSlTzgy3Y+XpXrmmMEjVoWtAJve71Ltkbxn12Y8v2Ot\n"
"wghSbOlz2+5GpwLbkXskbUSxmXEgJ9hoh4SHoyDMLmjHiwIDAQABMA0GCSqGSIb3\n"
"DQEBCwUAA4IBAQCri4kPm20iqnDcLrb9qyWFDgr7VFSOLavOut/uVfJmTfJj1ptB\n"
"pgCKh9bKRafGzy2qPtro3xF4ZsK8/eACZ/4MDHgqy+5Hd7maWS4eMJqz1Dvd8GHo\n"
"gOxBb7A4ylyIoEq3RvvcpEN6NvERMLaHwOHW4DIHXrHf+5ILbqNYggHvp6/FUbJW\n"
"fsCR6eUd0UDsDPk6uPEVh+AcJqqo7WSPUUlL6xqRe/NTuJaV+ptqLLbn16cZvwI5\n"
"3d1+SwYXbR1YolNNpCNFGeBlx7iozkf+4xm0MzdvXf5AkXPG4LYqSNjg7VFRnvMJ\n"
"t+/+1WyuVMw6JdY7IVJp75t7xzByz5wOXz3R\n"
"-----END CERTIFICATE-----\n";
*/

#define X509_VALID_PROVISIONING_CERTIFICATE 0

static mbedtls_x509_crt cacert;

static void initProvisioningCertificate(){
	//init ca certificate
	mbedtls_x509_crt_init( &cacert );
	//int ret=
	mbedtls_x509_crt_parse( &cacert, (const unsigned char *)mbedtls_provisioning_ca_crt, strlen(mbedtls_provisioning_ca_crt)+1 );
	SW_DIAG_printf("TLS Provisioing Certificate OK\n",ret);
}
/*
static bool extractCertificatePassword(mbedtls_x509_buf* v3_ext){
	if(v3_ext != NULL){
		char* delimiter = "\f ";
		char* data = strstr((char*)v3_ext->p, delimiter);
		if(data != NULL){
			data = (data + strlen(delimiter)); // skip the delimiter characters
			char* end = strstr(data, "0\r");
			int len = (int)(end - data);
			if(len > 0 && len <= CONFIG_CERTIFICATE_PASSWORD_LENGTH){
				memset( gCertificatePassword, '\0', CONFIG_CERTIFICATE_PASSWORD_LENGTH );
				for(int i = 0; i < len; i++){
					gCertificatePassword[i] = data[i];
				}
				LWIP_PLATFORM_DIAG(("Certificate password decoded successfully (%s)",
									gCertificatePassword));
				return true;
			}
		}
	}
	return false;
}
*/
// TODO - Extract password from a temporary certificate storage 'ORG_UNIT_NAME' instead of v3 extension
#define CERTIFICATE_PASSWORD_OID "2.5.4.3"		// COMMON NAME
#define CERTIFICATE_ORGANISATION_OID "2.5.4.11" // ORG UNIT NAME
#define CERTIFICATE_SUBJECT_BUFFER_LENGTH (64)
static char gSubjectName[CERTIFICATE_SUBJECT_BUFFER_LENGTH];

static bool extractCertificateDetails(mbedtls_x509_crt* cert){
	if(cert == NULL){
		return false;
	}
	int expectedSubjectsCount = 2;
	mbedtls_x509_name* subject = &cert->subject;
	while(subject!=NULL){
		mbedtls_oid_get_numeric_string(gSubjectName, CERTIFICATE_SUBJECT_BUFFER_LENGTH, &subject->oid);
		subject->val.p[subject->val.len] = '\0';
		//LWIP_PLATFORM_DIAG(("%s=%s",gSubjectName,subject->val.p));

		if(strcmp(CERTIFICATE_PASSWORD_OID, gSubjectName) == 0 && subject->val.len <= CONFIG_CERTIFICATE_PASSWORD_LENGTH){
			memcpy(gCertificatePassword,subject->val.p,subject->val.len);
			//LWIP_PLATFORM_DIAG(("Certificate password decoded successfully (%s)",gCertificatePassword));
			expectedSubjectsCount--;
		}else if(strcmp(CERTIFICATE_ORGANISATION_OID, gSubjectName) == 0){
			gCertificateOrganisationID = atol((const char*)subject->val.p);
			//LWIP_PLATFORM_DIAG(("Certificate organisation decoded successfully (%lld)",gCertificateOrganisationID));
			expectedSubjectsCount--;
		}
		if(!expectedSubjectsCount){
			break;
		}
		subject=subject->next;
	}
	gCertificateExpieryDate = cert->valid_to;
	return !expectedSubjectsCount;
}

static bool isCertificateExpired(mbedtls_x509_time valid_to){
	// @TODO - implement check for the expired date
	return false;
}

// Returns true when the certificate is valid
static bool handleProvisioningCertificate(const char* clientCert, bool bExtractDetails)
{
	bool success = false;

	mbedtls_x509_crl* ca_crl=NULL; //not sure what this does
	//init peer cert
	mbedtls_x509_crt peercert;
	mbedtls_x509_crt_init(&peercert);

	int ret=mbedtls_x509_crt_parse(&peercert, (const unsigned char *)clientCert, strlen(clientCert)+1 );
	
	uint32_t result=0;
	ret=mbedtls_x509_crt_verify_with_profile(
			&peercert,
			&cacert,
			ca_crl,
			&mbedtls_x509_crt_profile_default,
			NULL,
		    &result,
			NULL,
			NULL);
	//LWIP_PLATFORM_DIAG(("verify certificate ret=%d,result=%d",ret,result));
	success = (result == X509_VALID_PROVISIONING_CERTIFICATE) &&
			  (ret == X509_VALID_PROVISIONING_CERTIFICATE);
	success = success && !isCertificateExpired(peercert.valid_to);
	success = success && (!bExtractDetails || extractCertificateDetails(&peercert));
	//success = success && (!bExtractPassword || extractCertificatePassword(&peercert.v3_ext));

	mbedtls_x509_crt_free( &peercert );

	return success;
}

#define MD5_DIGEST_SIZE (16)
char hexdigest[2*MD5_DIGEST_SIZE + 1];
unsigned char digest[MD5_DIGEST_SIZE];

// Returns true if the data matches certificate password which is
// encrypted using md5.
static bool compareCertificatePassword(char* data)
{
	// encrypt the data
	mbedtls_md5((const unsigned char *)data, strlen(data), digest);
	// convert to hex
	for(int i = 0; i < MD5_DIGEST_SIZE; ++i){
		sprintf(&hexdigest[i*2], "%02x", (unsigned int)digest[i]);
	}
	//LWIP_PLATFORM_DIAG(("data=%s",data));
	//LWIP_PLATFORM_DIAG(("hexdigest=%s",hexdigest));

	return strncmp(hexdigest, gCertificatePassword, sizeof(gCertificatePassword)) == 0;
}

static uint32_t getSystemErrorState(){
	uint32_t errorState = ERROR_STATE_CLEAR;
	for(errorLogsError_t i=0;i<ERROR_LOGS_DISPLAY_BOUNDARY;i++){
		if(gErrorLogs.errors[i].unixTime>0){
			errorState |= ERROR_STATE_MAP[i];
		}
	}
	return errorState;
}

void mainFactoryReset(){
	gDoFactoryReset=true; //let the network thread do it, safer
}

void mainReset(){
	gDoReset=true; //let the network thread do it, safer
}

#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRi || INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
void PORTC_IRQHandler(void)
{
	const uint32_t pins = GPIO_PortGetInterruptFlags(GPIOC);
	//pins tells us which pins were actually triggered

    // Clear external interrupt flag.
	if(pins & (1U << CONFIG_GPIO_BUTTON_SET_PIN)){
		GPIO_PortClearInterruptFlags(CONFIG_GPIO_BUTTON_SET_BASE, 1U << CONFIG_GPIO_BUTTON_SET_PIN);
	}
	if(pins & (1U << CONFIG_GPIO_BUTTON_UP_PIN)){
		GPIO_PortClearInterruptFlags(CONFIG_GPIO_BUTTON_UP_BASE, 1U << CONFIG_GPIO_BUTTON_UP_PIN);
	}
    if(pins & (1U << CONFIG_GPIO_BUTTON_DOWN_PIN)){
    	GPIO_PortClearInterruptFlags(CONFIG_GPIO_BUTTON_DOWN_BASE, 1U << CONFIG_GPIO_BUTTON_DOWN_PIN);
    }

/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
  exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}
#endif //INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRi || INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc

static unsigned char gMbedTLSBuf[50000];
static uint32_t pcTestPattern=0; //PBB this has to be outside of the stack or the program counter test breaks randomly


void inxRTCInit(){
#if CONFIG_ENABLE_RTC
	if(gRTCDisabled){

	}else{
		//setup RTC
		rtc_config_t rtcConfig;
		RTC_GetDefaultConfig(&rtcConfig);
		//PBB this triggers a fault when VBAT is not powered properly.
		// We should not run this if the WDT has expired more than once.
		RTC_Init(RTC, &rtcConfig);
		BOARD_SetRtcClockSource();

		/* Set a start date time and start RTC */
		rtc_datetime_t date;
		date.year = 1970U;
		date.month = 1U;
		date.day = 1U;
		date.hour = 0U;
		date.minute = 0U;
		date.second = 0U;
		//this flag must be set before the write is done or the write will be ignore
		gRTCInitDone=true;
		inxRTCWriteDateTime(date);
	}
#endif //CONFIG_ENABLE_RTC
}

void updateLPTMRInterruptMilliseconds(const bool ignoreDutyCycle){
	if(ignoreDutyCycle){

	}else{
		float lengthSeconds=dutyCycleGetLengthSeconds();
		//make sure we get at least one sample per second, no slower
		if(lengthSeconds>CONFIG_SAMPLES_PER_DUTY_CYCLE){
			lengthSeconds=CONFIG_SAMPLES_PER_DUTY_CYCLE;
		}
		gADCSamplesPerSecond=CONFIG_SAMPLES_PER_DUTY_CYCLE/lengthSeconds;
		//update our RTC base because the interrupt timings are going to change
		gADCRTCLastGoodSeconds=inxRTCADCGetSeconds();
		gADCRTCTicks=0;
		gLPTMRInterruptMilliseconds=(lengthSeconds*1000) / (CONFIG_LPTMR_NUM_READS*CONFIG_SAMPLES_PER_DUTY_CYCLE);
		gRTCCheckTicksToWait=(CONFIG_RTC_SECONDS_BETWEEN_CHECKS*1000)/(gLPTMRInterruptMilliseconds*CONFIG_LPTMR_NUM_READS);
		gMinTicksSinceHeaterChange=CONFIG_EMR_RESPONSE_TIME_MILLISECONDS/gLPTMRInterruptMilliseconds;
		updateDutyCycleLengths();
	}
	LPTMR_InitTriggerSourceOfAdc(CONFIG_LPTMR_BASE);
}

status_t inxInitFlash(){
	//INX_BOOT_DIAG_PRINT(("flash,"));
	//initialise flash storage
	ftfx_security_state_t securityStatus = kFTFx_SecurityStateNotSecure; /* Return protection status */
	/* Return code from each flash driver function */

	//Setup flash driver structure for device and initialize variables.
	status_t result = FLASH_Init(g_bootloaderContext.allFlashState);
	INX_PRINTF_OTA("FLASH_init result=%d\r\n",result);
	if (kStatus_FTFx_Success != result){
		INX_BOOT_DIAG_PRINT(("1\n"));
	}else{
		INX_BOOT_DIAG_PRINT(("0\n"));
	}

	INX_BOOT_DIAG_PRINT(("flashCache,"));
	//Setup flash cache driver structure for device and initialize variables.
	result = FTFx_CACHE_Init(g_bootloaderContext.allFlashCacheState);
	INX_PRINTF_OTA("FTFx_CACHE_Init result=%d\r\n",result);
	if (kStatus_FTFx_Success != result){
		INX_BOOT_DIAG_PRINT(("1\n"));
	}else{
		INX_BOOT_DIAG_PRINT(("0\n"));
	}

	// Get flash properties
	FLASH_GetProperty(g_bootloaderContext.allFlashState, kFLASH_PropertyPflash0BlockBaseAddr, &gFlashBlockBase);
	FLASH_GetProperty(g_bootloaderContext.allFlashState, kFLASH_PropertyPflash0TotalSize, &gFlashTotalSize);
	FLASH_GetProperty(g_bootloaderContext.allFlashState, kFLASH_PropertyPflash0SectorSize, &gFlashSectorSize);
	if(sizeof(gSettingsToWrite)>gFlashSectorSize){
		//LWIP_PLATFORM_DIAG(("error: our settings structure can not be bigger than a single flash sector settings size=%d sector size=%d",sizeof(gSettingsToWrite),gFlashSectorSize));
		return -1;
	}

	//print welcome message
	//LWIP_PLATFORM_DIAG(("Program Flash Base:\t(0x%x)", gFlashBlockBase));
	//LWIP_PLATFORM_DIAG(("Total Program Flash Size:\t%d KB, Hex: (0x%x)", (gFlashTotalSize / 1024), gFlashTotalSize));
	//LWIP_PLATFORM_DIAG(("Program Flash Sector Size:\t%d KB, Hex: (0x%x) ", (gFlashSectorSize / 1024), gFlashSectorSize));

	/* Check security status. */
	result = FLASH_GetSecurityState(g_bootloaderContext.allFlashState, &securityStatus);
	if (kStatus_FTFx_Success != result)
	{
		//LWIP_PLATFORM_DIAG(("Failed to get flash security status"));
	}
	/* Print security status. */
	switch (securityStatus)
	{
		case kFTFx_SecurityStateNotSecure:
			//LWIP_PLATFORM_DIAG(("Flash is UNSECURE!"));
			break;
		case kFTFx_SecurityStateBackdoorEnabled:
			//LWIP_PLATFORM_DIAG(("Flash is SECURE, BACKDOOR is ENABLED!"));
			break;
		case kFTFx_SecurityStateBackdoorDisabled:
			//LWIP_PLATFORM_DIAG(("Flash is SECURE, BACKDOOR is DISABLED!"));
			break;
		default:
			break;
	}


	//set the location of our flash storage
#if !defined(INX_DONT_PARTITION_FLASH) && defined(FSL_FEATURE_FLASH_HAS_PFLASH_BLOCK_SWAP) && FSL_FEATURE_FLASH_HAS_PFLASH_BLOCK_SWAP
	//set our settings storage area to be just before the swap information sector
	//the last sector from the end is used for swap information
	uint32_t midWayPoint=(gFlashBlockBase + (gFlashTotalSize/2));
	gSettingsStorageFirstSectorAddress = midWayPoint - ((CONFIG_FLASH_SECTOR_INDEX_FROM_END + CONFIG_NUMBER_OF_FLASH_SECTORS_FOR_SETTINGS) * gFlashSectorSize);
	//TODO we should compare the size of our permStorage_t to make sure 1 pflashSectorSize will be enough
#else
	gSettingsStorageFirstSectorAddress = gFlashBlockBase + (gFlashTotalSize - ((CONFIG_FLASH_SECTOR_INDEX_FROM_END + CONFIG_NUMBER_OF_FLASH_SECTORS_FOR_SETTINGS) * gFlashSectorSize));
#endif
	INX_PRINTF_OTA("gSettingsStorageFirstSectorAddress=%x gFlashBlockBase=%x gFlashTotalSize=%d gFlashSectorSize=%d\r\n",gSettingsStorageFirstSectorAddress,gFlashBlockBase,gFlashTotalSize,gFlashSectorSize);
	return result;
}

void inxHeapInit(){
	//do an initial malloc so we have a marker we can check for bss overwriting start of heap
    gpInxIecHeapStart=malloc(sizeof(uint32_t));
    if(gpInxIecHeapStart==NULL){
    	//LWIP_PLATFORM_DIAG(("no heap"));
    }else{
    	*gpInxIecHeapStart=INX_FLASH_VALIDATION_CODE;
		//LWIP_PLATFORM_DIAG(("heap start=0x%X",gpInxIecHeapStart));
    }
    //need to place a marker at the end of the heap, we do this by allocating all of the heap except the last four bytes
    //then alloc that bit and set the marker, then free the rest
    uint32_t count=0;
    void* p = NULL;
    while(p==NULL){
    	p=malloc(CONFIG_HEAP_SIZE_BYTES-(sizeof(uint32_t)*count));
    	if(p==NULL){}else{
    		free(p);
    		gpInxIecHeapEnd=p+CONFIG_HEAP_SIZE_BYTES-(sizeof(uint32_t)*(count+1));
		}
    	count++;
    }
    *gpInxIecHeapEnd=INX_FLASH_VALIDATION_CODE;
    //LWIP_PLATFORM_DIAG(("heap end=0x%X",gpInxIecHeapEnd));
}


/* assumees the ADCs have already been configured */
int inxBootDiagnosticSensorTests(){
	int retval = 0;
	if(inxIecEnableADCs(&adc0ChnConfig,&adc1ChnConfig,false)){
		INX_BOOT_DIAG_PRINT(("0\n"));
		//wait for them to do a full set of reads
		while(nValueIndex==0){}
		//disable them again
		inxIecDisableADCs();
		nValueIndex=0;
		//print the results
		INX_BOOT_DIAG_PRINT(("pt100channel1,%d\n",gADCRawValues.pt100Channel1));
		INX_BOOT_DIAG_PRINT(("pt100channel2,%d\n",gADCRawValues.pt100Channel2));
		INX_BOOT_DIAG_PRINT(("cpuTemp1,%d\n",gADCRawValues.cpuTemp1));
		INX_BOOT_DIAG_PRINT(("zeroTenV,%d\n",gADCRawValues.zeroTenV));
		INX_BOOT_DIAG_PRINT(("fourTwentyMa,%d\n",gADCRawValues.fourTwentyMa));
		INX_BOOT_DIAG_PRINT(("thermocouple1,%d\n",gADCRawValues.thermocouple1));
	}else{
		retval = 1;
		INX_BOOT_DIAG_PRINT(("1\n"));
	}
	// Lets not rerun this too often - we never see this debug anyway
	//INX_BOOT_DIAG_PRINT(("rtc,"));
	//inxRTCInit();
	//INX_BOOT_DIAG_PRINT(("0\n"));
	return retval;
}

/*!
 * @brief Main function.
 */
int main(void)
{
	//memset((void*)0x2001D800,12,0x12000);
	/*for(uint32_t i=1;i<0xfff;i++){
		u8_t* pointer=(u8_t*)(pointerToTopOfHeat-i);
		*pointer=(u8_t)12;
		if(pointer==(u8_t*)(0x2F800-10)){
			printf("PBB 6176 %d\n",*pointer);
		}
		if((i%100)==0){
			printf("PBB 6174 %x\n",pointerToTopOfHeat-i);
		}
	}*/
	printf("PBB 6175 %d\n",*((uint8_t*)(0x2002F800-10)));
	//status_t result;
	/*uint32_t rtosPriority=( ( ( uint32_t ) configKERNEL_INTERRUPT_PRIORITY ) << 24UL );
	(void)rtosPriority;*/
	/* todo - we should move all the PIN config to a single file. possibly different functions for different PURPOSES IF NECESSARY*/
#undef INX_CHECK_PRODUCTION_BUILDS_RESTART_ON_BOOT_AS_EXPECTED
#ifdef INX_CHECK_PRODUCTION_BUILDS_RESTART_ON_BOOT_AS_EXPECTED
	SW_DIAG_printf("XXXXXXXXXXXXXXXXXXXXXXX->RESTARTING<-XXXXXXXXXXXXXXXXX\n");
#endif
	//initialise GPIO pins
 	gpio_pin_config_t gpioPinConfigDefault0 = {
		kGPIO_DigitalOutput, 0,
 	};
	gpio_pin_config_t gpioPinConfigDefault1 = {
		kGPIO_DigitalOutput, 1,
	};

#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRi
	SW_DIAG_printf("Starting HeatRod HRi Controller..\r\n");
#elif INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
	//This will only work in Debug life-style config to avoid code and IO overhaeds/risks in production systems.
	SW_DIAG_printf("Starting HeatRod HRc Controller..\r\n");
#else
	SW_DIAG_printf("Starting HeatRod FRDM Demo Controller...\n");
#endif
	/* todo  these need to use function specific macros e.g. RSD485 enable, control out 1, etc.*/
	/* todo these configs need to be made board specific*/
	/* todo allt= this init should ho in a board (or board family) code module/functions. */
    GPIO_PinInit(GPIOB, CONFIG_GPIO_RS485_ENABLE_DISABLE, &gpioPinConfigDefault0); // RS485 flow control

#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRi || INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
    /* Do some kind of config that is different to pin mix config */
	/* todo move this to a pin config init file  - i think it is already duplicztged theree- GPIO init is a mess*/
	GPIO_PinInit(CONFIG_GPIO_PROCBOARDLEDS_BASE,CONFIG_GPIO_PROCBOARDLED1_RED_PIN,&gpioPinConfigDefault0);
	GPIO_PinInit(CONFIG_GPIO_PROCBOARDLEDS_BASE,CONFIG_GPIO_PROCBOARDLED1_GRN_PIN,&gpioPinConfigDefault0);
	GPIO_PinInit(CONFIG_GPIO_PROCBOARDLEDS_BASE,CONFIG_GPIO_PROCBOARDLED2_RED_PIN,&gpioPinConfigDefault0);
	GPIO_PinInit(CONFIG_GPIO_PROCBOARDLEDS_BASE,CONFIG_GPIO_PROCBOARDLED2_GRN_PIN,&gpioPinConfigDefault0);
	/* Display */
	GPIO_PinInit(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,CONFIG_GPIO_NUMERICAL_DISPLAY_A,&gpioPinConfigDefault0);
	GPIO_PinInit(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,CONFIG_GPIO_NUMERICAL_DISPLAY_B,&gpioPinConfigDefault0);
	GPIO_PinInit(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,CONFIG_GPIO_NUMERICAL_DISPLAY_C,&gpioPinConfigDefault0);
	GPIO_PinInit(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,CONFIG_GPIO_NUMERICAL_DISPLAY_D,&gpioPinConfigDefault0);
	GPIO_PinInit(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,CONFIG_GPIO_NUMERICAL_DISPLAY_CHAR1,&gpioPinConfigDefault0);
	GPIO_PinInit(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,CONFIG_GPIO_NUMERICAL_DISPLAY_CHAR2,&gpioPinConfigDefault0);
	GPIO_PinInit(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,CONFIG_GPIO_NUMERICAL_DISPLAY_CHAR3,&gpioPinConfigDefault0);
	GPIO_PinInit(CONFIG_GPIO_NUMERICAL_DISPLAY_BASE,CONFIG_GPIO_NUMERICAL_DISPLAY_CHAR4,&gpioPinConfigDefault0);

#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
	GPIO_PinInit(CONFIG_GPIO_NEGATIVE_DISPLAY1_PIN_BASE,CONFIG_GPIO_NEGATIVE_DISPLAY1_PIN,&gpioPinConfigDefault0); // note we might support this on the Hri one day.
	GPIO_PinInit(CONFIG_GPIO_NUMERICAL_DISPLAY2_BASE,CONFIG_GPIO_NUMERICAL_DISPLAY2_CHAR1,&gpioPinConfigDefault0);
	GPIO_PinInit(CONFIG_GPIO_NUMERICAL_DISPLAY2_BASE,CONFIG_GPIO_NUMERICAL_DISPLAY2_CHAR2,&gpioPinConfigDefault0);
	GPIO_PinInit(CONFIG_GPIO_NUMERICAL_DISPLAY2_BASE,CONFIG_GPIO_NUMERICAL_DISPLAY2_CHAR3,&gpioPinConfigDefault0);
	GPIO_PinInit(CONFIG_GPIO_NUMERICAL_DISPLAY2_BASE,CONFIG_GPIO_NUMERICAL_DISPLAY2_CHAR4,&gpioPinConfigDefault0);
	GPIO_PinInit(CONFIG_GPIO_NEGATIVE_DISPLAY2_PIN_BASE,CONFIG_GPIO_NEGATIVE_DISPLAY2_PIN,&gpioPinConfigDefault0);
#endif


#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRi || INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
	/* Control Stuff */
		GPIO_PinInit(CONFIG_GPIO_CONTROL_BASE_OUT,CONFIG_GPIO_CONTROL_INTERNAL_RELAY_1,&gpioPinConfigDefault1);
		GPIO_PinInit(CONFIG_GPIO_CONTROL_BASE_OUT,CONFIG_GPIO_CONTROL_INTERNAL_RELAY_2,&gpioPinConfigDefault1);
		GPIO_PinInit(CONFIG_GPIO_CONTROL_BASE_OUT,CONFIG_GPIO_CONTROL_INTERNAL_RELAY_3,&gpioPinConfigDefault1);
		GPIO_PinInit(CONFIG_GPIO_CONTROL_BASE_OUT,CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_1,&gpioPinConfigDefault1);
		GPIO_PinInit(CONFIG_GPIO_CONTROL_BASE_OUT,CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_2,&gpioPinConfigDefault1);
		GPIO_PinInit(CONFIG_GPIO_CONTROL_BASE_OUT,CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_3,&gpioPinConfigDefault1);
#endif

#else


#endif //INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRi


#if CONFIG_TLK106
	//this is the initialisation for the phy reset, it should be held high for the tlk106
	GPIO_PinInit(CONFIG_GPIO_PHY_RESET_BASE, CONFIG_GPIO_PHY_RESET, &gpioPinConfigDefault1);
#endif //CONFIG_TLK106

	SYSMPU_Type *base = SYSMPU;
    BOARD_InitPins();
	printf("PBB 6267\n");
#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRi  || INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
	gpio_pin_config_t sw_config = {
		kGPIO_DigitalInput, 0,
	};
	GPIO_PinInit(CONFIG_GPIO_BUTTON_SET_BASE, CONFIG_GPIO_BUTTON_SET_PIN, &sw_config);
	GPIO_PinInit(CONFIG_GPIO_BUTTON_UP_BASE, CONFIG_GPIO_BUTTON_UP_PIN, &sw_config);
	GPIO_PinInit(CONFIG_GPIO_BUTTON_DOWN_BASE, CONFIG_GPIO_BUTTON_DOWN_PIN, &sw_config);
#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
	GPIO_PinInit(CONFIG_GPIO_BUTTON_MODE_BASE, CONFIG_GPIO_BUTTON_MODE_PIN, &sw_config);
#endif

#if CONFIG_GPIO_BUTTONS_INTERRUPT
    // input button init
    //this has to be after BOARD_InitPins
	PORT_SetPinInterruptConfig(CONFIG_GPIO_BUTTON_SET_PORT, CONFIG_GPIO_BUTTON_SET_PIN, kPORT_InterruptFallingEdge);
	PORT_SetPinInterruptConfig(CONFIG_GPIO_BUTTON_UP_PORT, CONFIG_GPIO_BUTTON_UP_PIN, kPORT_InterruptFallingEdge);
	PORT_SetPinInterruptConfig(CONFIG_GPIO_BUTTON_DOWN_PORT, CONFIG_GPIO_BUTTON_DOWN_PIN, kPORT_InterruptFallingEdge);
	EnableIRQ(PORTC_IRQn);
#endif //CONFIG_GPIO_BUTTONS_INTERRUPT

//#define INX_HEATROD_SKIP_DIPLAY_TEST
#ifndef INX_HEATROD_SKIP_DIPLAY_TEST
#ifdef INX_HEATROD_IOT_DIAGNOSTIC_BUILD
#undef INX_HEATROD_IOT_DIAGNOSTIC_BUILD_WITH_BUTTONS
#ifdef INX_HEATROD_IOT_DIAGNOSTIC_BUILD_WITH_BUTTONS
	inxButtonsHWDiagnosticTest();
#endif
	diagnostic_hw_test_display();

#endif
#endif

/* set normal run Display test digits */
#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
	inxDisplaySet(8,8,8,8,false);
	inxDisplay2Set(8,8,8,8,false);
#else
	inxDisplaySet(8,8,8,8,false);
#endif

#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRi
	GPIO_PinInit(CONFIG_GPIO_SENSE_CONRELAY_BASE, CONFIG_GPIO_SENSE_CONRELAY_1, &sw_config);
	GPIO_PinInit(CONFIG_GPIO_SENSE_CONRELAY_BASE, CONFIG_GPIO_SENSE_CONRELAY_2, &sw_config);
	GPIO_PinInit(CONFIG_GPIO_SENSE_CONRELAY_BASE, CONFIG_GPIO_SENSE_CONRELAY_3, &sw_config);

	/* Fail over sensing */
	GPIO_PinInit(CONFIG_GPIO_SENSE_CO_BASE, CONFIG_GPIO_SENSE_THERM_CO_1, &sw_config);
	GPIO_PinInit(CONFIG_GPIO_SENSE_CO_BASE, CONFIG_GPIO_SENSE_EXT_CO_1  , &sw_config);
#endif

#endif //INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRi || INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc

#if INX_HEATROD_IOT_BOARD_VARIANT != INX_HR_FRDMK64EVAL
	inxButtonsDetectDiagnosticBoot(); // check boot options and run relevant rest or run-time diagnostic modes.
#endif

	printf("PBB 6324\n");
	HW_DIAG_printf("CPU Clock & Phy connection Tests:\n__________________________________\n");
	
#if INX_HEATROD_HEALTHCHECKTESTS == 1
    //INX_HEATROD_STARTUP_CHECK
    //inxIecUartTests();
	SW_DIAG_printf("SW DIAG: Checking reset register ... ");
    memset(&g_sSafetyCommon,0,sizeof(g_sSafetyCommon));
    //PBB this test must be done before any ethernet init or clock frequency changes or it will fail on the special registers test
    //IEC 60730 H.11.12.7 Section 1.1 CPU Registers H.2.16.5
	SafetyCpuAfterResetTest(&g_sSafetyCommon);
	SW_DIAG_printf("OK\n");
#endif //INX_HEATROD_HEALTHCHECKTESTS == 1

#if CONFIG_PHY_ENABLE_CLOCK_OUTPUT == 0
    //Set the CPU clock to run from the 50Mhx clock input lines via a shared clock with the phy
	HW_DIAG_printf("HW DIAG: Checking 50Mhz Clock ... ");
    BOARD_BootClockRUN();
	HW_DIAG_printf("OK\n");
#endif //CONFIG_PHY_ENABLE_CLOCK_OUTPUT == 0

	//Seems this just inits the UART as a debug console which we don't want
    //BOARD_InitDebugConsole();
    //inxHeapInit();

	HW_DIAG_printf("HW DIAG: Checking PHY Initialisation ... ");
#if CONFIG_TLK106
#ifndef SKIP_ETHERNET_SETUP
    //we need to do this to set up the PHY for sleep mode during periodic tests
    //also we need this to enable the 50Mhz clock from the TLK106 chip
	fsl_enet_config0.phyAddress=CONFIG_SMI_PHY_ADDR; //this is the current SMI address of our tlk106 chip
	fsl_netif0.state=&fsl_enet_config0;
	err_t eth0err=ethernetif0_init(&fsl_netif0);
	while(eth0err==ERR_INPROGRESS){
		eth0err=inxNetworkTaskWaitForPhy();
	}
	if(eth0err!=ERR_OK){
	//	LWIP_PLATFORM_DIAG(("eth0err is not OK (%d)",eth0err));
		HW_DIAG_printf("ERROR: Network PHY didn't start\n");
		return -1;
	}
#endif
	//wait a while for the 50Mhz signal from the PHY chip to get in to the xtal input
	volatile uint32_t count=0;
	while(count<320000){
		count++;
	}
#else
	//we have to do this on boot so that we can put the PHY in to sleep  mode before doing periodic tests
	fsl_netif0.state=&fsl_enet_config0;
	err_t eth0err=ethernetif0_init(&fsl_netif0);
	while(eth0err==ERR_INPROGRESS){
		eth0err=inxNetworkTaskWaitForPhy();
	}
	if(eth0err!=ERR_OK){
		HW_DIAG_printf("ERROR: Failed to initialise (NOT TLK106 dirivitive)\n");
		return -1;
	}
#endif
	HW_DIAG_printf("OK\n"); // todo (!!) is this for the debugger? Can we loose this?
#if CONFIG_PHY_ENABLE_CLOCK_OUTPUT == 1
    //Set the CPU cloick to run from the Phy's (now configured) clock output - otherwise this is done earlier
    HW_DIAG_printf("HW_DIAG: Checking external 50MHz clock input...");
	BOARD_BootClockRUN();
    HW_DIAG_printf("OK\n______________________________________\n");
#endif

	//this is a heap allocated queue, should probably be replaced with a statically allocated queue
	gAdcValuesQueue = xQueueCreate( CONFIG_ADC_VALUES_QUEUE_LENGTH,sizeof(adcValuesQueueMessage_t));
	/* pxQueueBuffer was not NULL so xQueue should not be NULL. */
	configASSERT( gAdcValuesQueue );

	//enable ADCs and calibrate them both, using ADC1 for temperature sesning
	ADC16_CalibrateParams(ADC0,0); //calibrate the ADC0 - Ch1 temperature, 0-10v
	ADC16_CalibrateParams(ADC1,1); //calibrate the ADC1 - Ch2 temperature, 4-20mA, cpu temp
	if(gDiagnosticBoot == true){
		inxBootDiagnosticSensorTests();
	}

	//IEC 60730 H.11.12.7 Section 1.3 CPU program counter H.2.16.5
	SafetyPcTest(&g_sSafetyCommon,(uint32_t)&pcTestPattern);

    //Disable SYSMPU. what ever this is?
    base->CESR &= ~SYSMPU_CESR_VLD_MASK;
	printf("PBB 6406\n");
    HW_DIAG_printf("HW DIAG: Initialising flash ...");
	inxInitFlash();
	#if CONFIG_OTA_DIAG
		/*status_t readStatus=flash_mem_read(0x7E000,gFlashSectorSize,gOTAUpdateOneSectorBuffer);
		INX_PRINTF_OTA("flash_mem_read %d\r\n",readStatus);
		readStatus=flash_mem_erase(0x7E000,gFlashSectorSize);
		INX_PRINTF_OTA("flash_mem_erase %d\r\n",readStatus);
		readStatus=flash_mem_read(0x7E000,gFlashSectorSize,gOTAUpdateOneSectorBuffer);
		INX_PRINTF_OTA("flash_mem_read %d\r\n",readStatus);

		memset(gOTAUpdateOneSectorBuffer,0,gFlashSectorSize);
		readStatus=flash_mem_erase(0x80000,gFlashSectorSize);
		INX_PRINTF_OTA("flash_mem_erase %d\r\n",readStatus);
		readStatus=flash_mem_write(0x80000,gFlashSectorSize,gOTAUpdateOneSectorBuffer);
		
		uint32_t swapAddress=(gFlashTotalSize/2)-(CONFIG_FLASH_SECTOR_INDEX_FROM_END * gFlashSectorSize);
		//bootloader_reliable_update_as_requested(kReliableUpdateOption_Swap,swapAddress);
		readStatus=flash_mem_read(swapAddress,gFlashSectorSize,gOTAUpdateOneSectorBuffer);
		INX_PRINTF_OTA("flash_mem_read %d\r\n",readStatus);
		for(uint32_t i=0;i<gFlashSectorSize;i++){
			u8_t* address=(u8_t*)(0x80000 + i);
			u8_t value=*address;
			if(value!=0xff){
				INX_PRINTF_OTA("%x %x,",address,value);
			}
		}
		
		readStatus=flash_mem_read(0xe0000,gFlashSectorSize,gOTAUpdateOneSectorBuffer);
		INX_PRINTF_OTA("flash_mem_read %d\r\n",readStatus);
		readStatus=flash_mem_erase(0xe0000,gFlashSectorSize);
		INX_PRINTF_OTA("flash_mem_erase %d\r\n",readStatus);
		readStatus=flash_mem_read(0xe0000,gFlashSectorSize,gOTAUpdateOneSectorBuffer);
		INX_PRINTF_OTA("flash_mem_read %d\r\n",readStatus);

		readStatus=flash_mem_read(0x80000,gFlashSectorSize,gOTAUpdateOneSectorBuffer);
		INX_PRINTF_OTA("flash_mem_read %d\r\n",readStatus);
		readStatus=flash_mem_erase(0x80000,gFlashSectorSize);
		INX_PRINTF_OTA("flash_mem_erase %d\r\n",readStatus);
		readStatus=flash_mem_read(0x80000,gFlashSectorSize,gOTAUpdateOneSectorBuffer);
		INX_PRINTF_OTA("flash_mem_read %d\r\n",readStatus);*/
		
		//firmwareWriteSector(false,0,gOTAUpdateOneSectorBuffer,0,0);
	#endif
	HW_DIAG_printf(" Finished\n______________________________________\n");
    getCpuId(clientCpuId);
    HW_DIAG_printf("HW DIAG: Device UIDs:\n__________________________________\n");
    HW_DIAG_printf("Firmware version="CONFIG_FIRMWARE_VERSION"\nClientCpuId=%s\n",clientCpuId);

	snprintf(gHashedClientId,HASH_CLIENT_ID_MAX_LENGTH,"%lld",string_hash(clientCpuId, HASH_MOD_ID, HASH_SEED_ID)); 	// create unique 10-digit device id
	//HW_DIAG_printf("Hashed Client ID =%s\n",gHashedClientId);
	snprintf(gDevicePin,HASH_PIN_MAX_LENGTH,"%lld",string_hash(gHashedClientId, HASH_MOD_PIN, HASH_SEED_PIN));		// create unique 5-digit device pin
	HW_DIAG_printf("Device Pin=%s\n",gDevicePin);
	updateDisplayId();
	HW_DIAG_printf("ClientId=%s\n",clientId);
	HW_DIAG_printf("______________________________________\n");
	//PBB this needs to be 1 at the most or timing issues will break most connections
	mbedtls_debug_set_threshold(0);
	//PBB this is a chunk of memory since we don't have a functioning heap for mbedtls to use
	mbedtls_memory_buffer_alloc_init( gMbedTLSBuf, sizeof(gMbedTLSBuf) );
	initProvisioningCertificate();
#if INX_HEATROD_BOOT_LED_TEST
	inxLedBootTest();

#endif
#ifdef INX_HEATROD_IOT_DIAGNOSTIC_BUILD
	inxLedTest();
	//inxLedBootTest();
#endif

#if CONFIG_MDNS_ENABLED
	strcpy(gMdnsName, CONFIG_MDNS_HOSTNAME);
	strcat(gMdnsName, clientId);
#endif

	//PBB DON'T ALLOCATE GLOBAL BUFFERS ON THE MAIN STACK

	 //todo aren't we supposed to read this back from flash first and not do anything if it's set in flash?
#if	INX_HEATROD_HEALTHCHECKTESTS == 1
	gGpioConditionState.mode = INX_GPIO_CONDITION_DIAGNOSTICS;
#else
#ifdef INX_HEATROD_IOT_DIAGNOSTIC_BUILD
	gGpioConditionState.mode = INX_GPIO_CONDITION_DISABLED; // stop any other interrupts chaning the relays in Diagnostic mode
#else
	gGpioConditionState.mode = INX_GPIO_CONDITION_OK;
#endif
#endif

#if HTTPSRV_CFG_MBEDTLS_ENABLE
	memset(&gHttpdTlsParams,0,sizeof(gHttpdTlsParams));
#endif

	loadSettingsFromFlash(false);
	switch(gPIDs[0].scheduleMode){
		case SCHEDULE_MODE_TIMER_SET_POINT:
		case SCHEDULE_MODE_TIMER:
			gButtonState=INX_BUTTON_STATE_DISPLAY_TIMER;
			break;
		default:
			break;
	}
	//inxDisplaySetFromTemperature(gSettingsBuffer.manualSetPoint,1); // todo not good here, but will work for now.

	//todo move this into a function in the safety module
	//todo also include a build optionthat doesn't test the WDT every boot (Fast boot mode?)
#if	INX_HEATROD_HEALTHCHECKTESTS == 1
	SW_DIAG_printf("Health check tests:\n__________________________________\nSW DIAG: Starting/re-entering watchdog timer test...");
	SafetyWatchdogTest(&gsSafetyWdTest);
	wdog_test_config_t test_config;
	//clear the watch dog reset count if we have booted normally
	volatile uint16_t wdog_reset_count = WDOG_GetResetCount(wdog_base);
	//LWIP_PLATFORM_DIAG(("before clear wdog_reset_count=%d",wdog_reset_count));
	if (!(RCM_GetPreviousResetSources(rcm_base) & kRCM_SourceWdog))
	{
		WDOG_ClearResetCount(wdog_base);
	}
	wdog_reset_count = WDOG_GetResetCount(wdog_base);
	//LWIP_PLATFORM_DIAG(("after clear wdog_reset_count=%d",wdog_reset_count));
	switch(gSafetyMode){
		case INX_SAFETY_MODE_DISABLE_SLOW_START_CHECKS:
		case INX_SAFETY_MODE_DISABLE_ALL_HARDWARE_CHECKS:
			if (wdog_reset_count == 0){
				wdog_reset_count = 1; //If we don't test the watchdog we need to set this for the reset status tests
			}
			break;
		default:
			#if INX_HEATROD_HEALTHCHECKTESTS == 1
				#if INX_HEATROD_WATCHDOGTEST == 1
					//LWIP_PLATFORM_DIAG(("wdog_reset_count=%d",wdog_reset_count));
					if(wdog_reset_count == 0){
						//inxIecSetNormalWatchDog();
						//LWIP_PLATFORM_DIAG(("triggering watch dog test"));
						WDOG_SetTestModeConfig(wdog_base, &test_config);
						inxIecWaitForWatchDogUpdate(wdog_base);

						//wait for timeout reset
						while (1)
						{
						}
						//todo - We should timeout this wait and go to a safe failmode to report
						// the issue rather than hang here.

					}
				#else //INX_HEATROD_WATCHDOGTEST == 1
					if (wdog_reset_count == 0) wdog_reset_count = 1; /* If we don't test the watchdog we need to set this for the reset status tests */
				#endif //INX_HEATROD_WATCHDOGTEST == 1
			#endif //INX_HEATROD_HEALTHCHECKTESTS == 1
			break;
	}
	SW_DIAG_printf("Finished\n");
#endif

	// todo - move this into a function in the swafety or flash module
	// todo and decide when we actually need to run it - not clear what the crc check means at the moment.

#if CONFIG_DO_FLASH_TEST == 1
	//if we have no CRC value then copy ourselves in to the other half and initiate reboot
	if(gCrcValue==0){
		for(uint32_t currentSector=0;currentSector<CONFIG_MAX_PROGRAM_SIZE_SECTORS;currentSector++){
			bool doSwap = currentSector==(CONFIG_MAX_PROGRAM_SIZE_SECTORS-1);
			uint32_t programSizeBytes = (currentSector+1) * gFlashSectorSize;
			memcpy(gOTAUpdateOneSectorBuffer,(void*)0+(currentSector*gFlashSectorSize),gFlashSectorSize);
			firmwareWriteSector(doSwap,currentSector,gOTAUpdateOneSectorBuffer,programSizeBytes,0);
		}
	}
#else
	SW_DIAG_printf("Skipping flash re-write to other buffer");
#endif
	
	//calculate our lptmr interrupt frequency
	updateLPTMRInterruptMilliseconds(false);
	//initialise our duty cycle states
	memset(&dutyCycleHistory0,0,sizeof(dutyCycleHistory0));
	dutyCycleState0.powerOnThresholdPercent=0;
	dutyCycleState0.internalGPIOBase=CONFIG_HEATER_ONE_GPIO_BASE;
	dutyCycleState0.internalGPIOPin=CONFIG_GPIO_CONTROL_INTERNAL_RELAY_1;// todo - this can be dynamically switched to external(auxillary outputs too. should read from the config at start up case oo)
	dutyCycleState0.externalGPIOBase=CONFIG_HEATER_ONE_GPIO_BASE;
	dutyCycleState0.externalGPIOPin=CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_1;
	dutyCycleStateSetPowerPercent(&dutyCycleState0,0,0);
	dutyCycleStateResetCycle(&dutyCycleState0);

	dutyCycleState1.powerOnThresholdPercent=0;
	dutyCycleState1.internalGPIOBase=CONFIG_HEATER_TWO_GPIO_BASE;
	dutyCycleState1.internalGPIOPin=CONFIG_GPIO_CONTROL_INTERNAL_RELAY_2;
	dutyCycleState1.externalGPIOBase=CONFIG_HEATER_TWO_GPIO_BASE;
	dutyCycleState1.externalGPIOPin=CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_2;
	dutyCycleStateSetPowerPercent(&dutyCycleState1,0,0);
	dutyCycleStateResetCycle(&dutyCycleState1);

	dutyCycleState2.powerOnThresholdPercent=0;
	dutyCycleState2.internalGPIOBase=CONFIG_HEATER_THREE_GPIO_BASE;
	dutyCycleState2.internalGPIOPin=CONFIG_GPIO_CONTROL_INTERNAL_RELAY_3;
	dutyCycleState2.externalGPIOBase=CONFIG_HEATER_THREE_GPIO_BASE;
	dutyCycleState2.externalGPIOPin=CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_3;
	dutyCycleStateSetPowerPercent(&dutyCycleState2,0,0);
	dutyCycleStateResetCycle(&dutyCycleState2);

#if INX_HEATROD_HEALTHCHECKTESTS == 1
	HW_DIAG_printf("HW DIAG: Starting Health Check (boot number %d)\n",wdog_reset_count);
	switch(wdog_reset_count){
		case 1:
			//LWIP_PLATFORM_DIAG(("setting up the watch dog for regular operation"));
			inxIecSetNormalWatchDog();
			//enable the RTC because we use it as part of our diagnostics
			HW_DIAG_printf("HW DIAG:inxRTCInit...\r\n");
			inxRTCInit();
			HW_DIAG_printf("HW DIAG:inxRTCInit done\r\n");
			//make this only run if our flash settings say we haven't ran before
			if(gSettingsBuffer.diagnosticsDone){

			}else{
				HW_DIAG_printf("Entering IEC Diagnostics...");
				inxIecDiagnostic(&gInxIecInterruptData,&adc0ChnConfig,&adc1ChnConfig,gCrcValue,gProgramSizeBytes);
				HW_DIAG_printf("Done!\n");
				//write some updated settings if we do run this so we don't run it in future
				gDiagnosticsDone = true;
				saveSettingsToFlash(true,gCrcValue,gProgramSizeBytes);
				HW_DIAG_printf("saved setting to flash... possibly...\n");
			}
			switch(gControllerOutputMode){
				case INX_CONTROLLER_OUTPUT_MODE_INTERNAL_RELAY:
					result=inxIecInternalRelayCheck(&gStartupRelayErrorState);
					if(result==kStatus_Success){
						//all is fine enable the output
						gWatchDogState.deviceFailed = false;
					}else{
						//all is not fine, disable the gpio output and raise an error
						gWatchDogState.deviceFailed = true;
					}
					break;
				case INX_CONTROLLER_OUTPUT_MODE_EXTERNAL_RELAY:
					//assume we can't test external relays
					gWatchDogState.deviceFailed = false;
					break;
				default:
					break;
			}
			break;
		case 2:
			HW_DIAG_printf("we encountered an error so run diagnostic, if we pass then resume operation\n");
			inxIecDiagnostic(&gInxIecInterruptData,&adc0ChnConfig,&adc1ChnConfig,gCrcValue,gProgramSizeBytes);
			WDOG_ClearResetCount(wdog_base);
			//now the diagnostics have passed then do a restart and let normal operation take over
			inxIecRebootDevice();
			break;
		case 3:
			HW_DIAG_printf("error occurred again during diagnostic try clearing the settings and doing diagnostics again\n");
			loadSettingsFromFlash(true); // todo - review if we really want to do this.Would be very bad potentially if the factory defaults can lead to tings switching on when they were'nt before
			errorLogsWrite(ERROR_LOGS_FORCED_FACTORY_RESET);
			saveSettingsToFlash(true,gCrcValue,gProgramSizeBytes);
			inxIecDiagnostic(&gInxIecInterruptData,&adc0ChnConfig,&adc1ChnConfig,gCrcValue,gProgramSizeBytes);
			WDOG_ClearResetCount(wdog_base);
			//now the diagnostics have passed then do a restart and let normal operation take over
			inxIecRebootDevice();
			break;
		case 4:
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////(("error occurred again after factory reset, disabling RTC"));
			HW_DIAG_printf("Rebooting with factory default flash\n");
			loadSettingsFromFlash(false);
			errorLogsWrite(ERROR_LOGS_RTC_DISABLED);
			gRTCDisabled=true;
			inxIecDiagnostic(&gInxIecInterruptData,&adc0ChnConfig,&adc1ChnConfig,gCrcValue,gProgramSizeBytes);
			//assume if we got here it is safe to disable the RTC
			saveSettingsToFlash(true,gCrcValue,gProgramSizeBytes);
			WDOG_ClearResetCount(wdog_base);
			//now the diagnostics have passed then do a restart and let normal operation take over
			inxIecRebootDevice();
			break;
		default:
			/* todo review if these ignores are valid - wouldn't these be caused by a software issue, so we would never ignore?*/
			HW_DIAG_printf("wdog_reset_count=%d not starting up\n",wdog_reset_count);
	#if CONFIG_IGNORE_FATAL_FAULTS_AND_REPORT
			SW_DIAG_printf("ignoring bad watch dog number and reporting error\n");
			inxIecSetNormalWatchDog();
			errorLogsWrite(ERROR_LOGS_WATCH_DOG_RESETS);
	#else //CONFIG_IGNORE_FATAL_FAULTS_AND_REPORT
			while(1){}
	#endif //CONFIG_IGNORE_FATAL_FAULTS_AND_REPORT
			break;
	}

#else //INX_HEATROD_HEALTHCHECKTESTS == 1
	HW_DIAG_printf("HW DIAG: Checking RTC...");
	inxRTCInit(); //we should always init RTC if no health check tests
	//errorLogsClearSingleError(ERROR_LOGS_INTERNAL_RELAY_STUCK_ON);
    HW_DIAG_printf("OK\n");
	//HW_DIAG_printf("RTC IS NOW  Initied and says %d\n",inxRTCGetSecondsAlways());
#endif //INX_HEATROD_HEALTHCHECKTESTS == 1

#if MQTT_USE_TLS || CONFIG_USE_DNS || CONFIG_USE_DHCP
    /* initialize random number generator */
    RNGA_Init(RNG); /* init random number generator */
    RNGA_Seed(RNG, SIM->UIDL); /* use device unique ID as seed for the RNG */
#endif
	printf("6702\n");
#if CONFIG_MQTT_ENABLED
	mqttSetTopicNames(clientId);
#endif

	//allow the ADCs to start doing GPIO
	//todo - shouldn't we check the flash first?
	// This just seems to wipe out the boot testing and we did it earlier..??? :
	gGpioConditionState.mode = INX_GPIO_CONDITION_OK;

	/* Set to allow entering vlps mode */
	//SMC_SetPowerModeProtection(SMC, kSMC_AllowPowerModeVlp);

	//clear status flags
	gLwipStopped=false;

	if(inxIecEnableADCs(&adc0ChnConfig,&adc1ChnConfig,false)){}else{
		SW_DIAG_printf("SW DIAG: ERROR - Failed to enable ADCs, Continuing to allow diagnostics\n");
#ifdef INX_HEATROD_IOT_DIAGNOSTIC_BUILD
		hw_diagnostic_test_errors++;
#endif
		//while(1){}
		//todo we should set a flag here to go into a recoverable (no output mode) state to report this over network
	}

	//PBB wobble in ADC interrupt was caused by debug output
	/*NVIC_SetPriority(LPTMR0_IRQn,0u);
	NVIC_SetPriority(ENET_1588_Timer_IRQn,6U);
	NVIC_SetPriority(ENET_Transmit_IRQn,6U);
	NVIC_SetPriority(ENET_Receive_IRQn,6U);
	NVIC_SetPriority(ENET_Error_IRQn,6U);
	NVIC_SetPriority(RNG_IRQn,5U);*/
//todo (!!) - what is this for?
	IP4_ADDR(&gfsl_netif0_ipaddr, 192U, 168U, 2U, 61U);
	IP4_ADDR(&gfsl_netif0_netmask, 255U, 255U, 255U, 0U);
	IP4_ADDR(&gfsl_netif0_gw, 192U, 168U, 2U, 1U);

	HW_DIAG_printf("\nRelay Outputs...\n__________________________________\n");
#ifdef INX_HEATROD_IOT_DIAGNOSTIC_BUILD

	int sense;
	/*  Relay  1 Check  Sense check */
	HW_DIAG_printf("RELAY_1 POWERED ON  -> ");
	GPIO_PortClear(CONFIG_GPIO_CONTROL_BASE_OUT, 1U << CONFIG_GPIO_CONTROL_INTERNAL_RELAY_1);
	inxDiagnosticWaitLongTime(1);
#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRi
	sense = GPIO_PinRead(CONFIG_GPIO_SENSE_CONRELAY_BASE,CONFIG_GPIO_SENSE_CONRELAY_1);
	HW_DIAG_printf("Sensed %s [%s]\n",sense?"OFF":"ON",!sense?" OK":"ERROR");if (sense) hw_diagnostic_test_errors++;
	/* Check others are off */
	sense = GPIO_PinRead(CONFIG_GPIO_SENSE_CONRELAY_BASE,CONFIG_GPIO_SENSE_CONRELAY_2);
	if (!sense) {hw_diagnostic_test_errors++;HW_DIAG_printf("ERROR! - RELAY_2 ALSO POWERED ON\n");}
	sense = GPIO_PinRead(CONFIG_GPIO_SENSE_CONRELAY_BASE,CONFIG_GPIO_SENSE_CONRELAY_3);
	if (!sense) {hw_diagnostic_test_errors++;HW_DIAG_printf("ERROR! - RELAY_3 ALSO POWERED ON\n");}
	sense = GPIO_PinRead(CONFIG_GPIO_SENSE_CO_BASE,CONFIG_GPIO_SENSE_THERM_CO_1);
	if (sense) {hw_diagnostic_test_errors++;HW_DIAG_printf("ERROR! - Thermal Coutout is not connected\n");}
#endif

	HW_DIAG_printf("RELAY_1 POWERED OFF -> ");
	GPIO_PortSet(CONFIG_GPIO_CONTROL_BASE_OUT, 1U << CONFIG_GPIO_CONTROL_INTERNAL_RELAY_1);
#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRi
	inxDiagnosticWaitLongTime(1);
	sense = GPIO_PinRead(CONFIG_GPIO_SENSE_CONRELAY_BASE,CONFIG_GPIO_SENSE_CONRELAY_1);
	HW_DIAG_printf("Sensed %s [%s]\n",sense?"OFF":"ON",sense?" OK":"ERROR");if (!sense) hw_diagnostic_test_errors++;
	sense = GPIO_PinRead(CONFIG_GPIO_SENSE_CONRELAY_BASE,CONFIG_GPIO_SENSE_CONRELAY_2);

	HW_DIAG_printf("RELAY_2 POWERED ON  -> ");
	GPIO_PortClear(CONFIG_GPIO_CONTROL_BASE_OUT, 1U << CONFIG_GPIO_CONTROL_INTERNAL_RELAY_2);
	inxDiagnosticWaitLongTime(1);
	sense = GPIO_PinRead(CONFIG_GPIO_SENSE_CONRELAY_BASE,CONFIG_GPIO_SENSE_CONRELAY_2);
	HW_DIAG_printf("Sensed %s [%s]\n",sense?"OFF":"ON",!sense?" OK":"ERROR");if (sense) hw_diagnostic_test_errors++;
	sense = GPIO_PinRead(CONFIG_GPIO_SENSE_CO_BASE,CONFIG_GPIO_SENSE_THERM_CO_1);
	if (!sense) {hw_diagnostic_test_errors++;HW_DIAG_printf("ERROR! - Thermal Coutout is sensed ON\n");}
	/* Others Relays*/
	sense = GPIO_PinRead(CONFIG_GPIO_SENSE_CONRELAY_BASE,CONFIG_GPIO_SENSE_CONRELAY_1);
	if (!sense) {hw_diagnostic_test_errors++;HW_DIAG_printf("ERROR! - RELAY 1 ALSO POWERED ON\n");}
	sense = GPIO_PinRead(CONFIG_GPIO_SENSE_CONRELAY_BASE,CONFIG_GPIO_SENSE_CONRELAY_3);
	if (!sense) {hw_diagnostic_test_errors++;HW_DIAG_printf("ERROR! - RELAY 3 ALSO POWERED ON\n");}

	HW_DIAG_printf("RELAY_2 POWERED OFF -> ");
	GPIO_PortSet(CONFIG_GPIO_CONTROL_BASE_OUT, 1U << CONFIG_GPIO_CONTROL_INTERNAL_RELAY_2);
	inxDiagnosticWaitLongTime(1);
	sense = GPIO_PinRead(CONFIG_GPIO_SENSE_CONRELAY_BASE,CONFIG_GPIO_SENSE_CONRELAY_1);
	HW_DIAG_printf("Sensed %s [%s]\n",sense?"OFF":"ON",sense?" OK":"ERROR");if (!sense) hw_diagnostic_test_errors++;

	HW_DIAG_printf("RELAY_3 POWERED  ON -> ");
	GPIO_PortClear(CONFIG_GPIO_CONTROL_BASE_OUT, 1U << CONFIG_GPIO_CONTROL_INTERNAL_RELAY_3);
	inxDiagnosticWaitLongTime(1);
	sense = GPIO_PinRead(CONFIG_GPIO_SENSE_CONRELAY_BASE,CONFIG_GPIO_SENSE_CONRELAY_3);
	HW_DIAG_printf("Sensed %s [%s]\n",sense?"OFF":"ON",!sense?" OK":"ERROR");if (sense) hw_diagnostic_test_errors++;
	sense = GPIO_PinRead(CONFIG_GPIO_SENSE_CONRELAY_BASE,CONFIG_GPIO_SENSE_CONRELAY_1);
	if (!sense) {hw_diagnostic_test_errors++;HW_DIAG_printf("ERROR! - RELAY 1 ALSO POWERED ON\n");}
	sense = GPIO_PinRead(CONFIG_GPIO_SENSE_CONRELAY_BASE,CONFIG_GPIO_SENSE_CONRELAY_2);
	if (!sense) {hw_diagnostic_test_errors++;HW_DIAG_printf("ERROR! - RELAY 2 ALSO POWERED ON\n");}

	HW_DIAG_printf("RELAY_3 POWERED OFF -> ");
	GPIO_PortSet(CONFIG_GPIO_CONTROL_BASE_OUT, 1U << CONFIG_GPIO_CONTROL_INTERNAL_RELAY_3);
	inxDiagnosticWaitLongTime(1);
	sense = GPIO_PinRead(CONFIG_GPIO_SENSE_CONRELAY_BASE,CONFIG_GPIO_SENSE_CONRELAY_3);
	HW_DIAG_printf("Sensed %s [%s]\n",sense?"OFF":"ON",sense?" OK":"ERROR"); if (!sense) hw_diagnostic_test_errors++;
#endif
/* Checking the external SSRs with an LED flash test - no other way of confirming with current test jigs */
	HW_DIAG_printf("___________\nEXTERNAL SSR Fast Switch test\nIteration:");
	for (int i = 0 ; i < 15 ; i++) {
		HW_DIAG_printf("%d,",i);
		GPIO_PortClear(CONFIG_GPIO_CONTROL_BASE_OUT, 1U << CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_1);
		inxDiagnosticWaitLongTime(1);
		//HW_DIAG_printf("EXTERNAL SSR1 OFF\n");
		GPIO_PortSet(CONFIG_GPIO_CONTROL_BASE_OUT, 1U << CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_1);
		inxDiagnosticWaitLongTime(1);
		//HW_DIAG_printf("EXTERNAL SSR2 ON\n");
		GPIO_PortClear(CONFIG_GPIO_CONTROL_BASE_OUT, 1U << CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_2);
		inxDiagnosticWaitLongTime(1);
		//HW_DIAG_printf("EXTERNAL SSR2 OFF\n");
		GPIO_PortSet(CONFIG_GPIO_CONTROL_BASE_OUT, 1U << CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_2);
		inxDiagnosticWaitLongTime(1);
		//HW_DIAG_printf("EXTERNAL SSR3 ON\n");
		GPIO_PortClear(CONFIG_GPIO_CONTROL_BASE_OUT, 1U << CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_3);
		inxDiagnosticWaitLongTime(1);
		//HW_DIAG_printf("EXTERNAL SSR3 OFF\n");
		GPIO_PortSet(CONFIG_GPIO_CONTROL_BASE_OUT, 1U << CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_3);
		inxDiagnosticWaitLongTime(1);
	}
	/*
#define CONFIG_GPIO_SENSE_CO_BASE GPIOB
#define CONFIG_GPIO_SENSE_CO_PORT PORTB
#define CONFIG_GPIO_SENSE_THERM_CO_1 (20U)
#define CONFIG_GPIO_SENSE_THERM_CO_2 (21U)
#define CONFIG_GPIO_SENSE_THERM_CO_3 (22U)
#define CONFIG_GPIO_SENSE_EXT_CO_1 (23U)

#define CONFIG_GPIO_SENSE_CONRELAY_PORT PORTC
#define CONFIG_GPIO_SENSE_CONRELAY_BASE	GPIOC

#define CONFIG_GPIO_SENSE_CONRELAY_1 (2U)
#define CONFIG_GPIO_SENSE_CONRELAY_2 (1U)
#define CONFIG_GPIO_SENSE_CONRELAY_3 (0U)
*/

	HW_DIAG_printf("Finished\n______________________________________\n");

#endif
	HW_DIAG_printf("Finished\n______________________________________\n");

	//HW_DIAG_printf("RTC 9 IS NOW %d\n",inxRTCGetSecondsAlways());

#ifdef INX_HEATROD_IOT_DIAGNOSTIC_BUILD
	/* Starting Logging ADC values */
	show_adc_values = INX_HW_DIAG_ADC_TESTS_TODO; // Don't show anything to start with results for a bit then stop spamming the console
	HW_DIAG_printf("Checking ADCs (%d samples)...\n__________________________________\n",INX_HW_DIAG_ADC_TESTS_TODO);
	/* This will leave diagnostic console output running and tidy up the console when it's finished. */
#endif
	printf("PBB 6844\n");
	//create main thread in RTOS
    if (sys_thread_new("main", main_thread, NULL, DEFAULT_THREAD_STACKSIZE, CONFIG_MAIN_THREAD_PRIORITY) == NULL){
        LWIP_PLATFORM_DIAG(("main(): Task creation failed."));
    }else{
    	/* run RTOS */
		vTaskStartScheduler();
    }

    /* should not reach this statement */
    for (;;)
        ;
}

static bool gFTM2Started=false;
static void inxPWMDeinit(){
	if(gFTM2Started){
		FTM_StopTimer(FTM2);
		gFTM2Started=false;
	}
}

static void inxPWMInit(const uint32_t hz){
	inxPWMDeinit();
	ftm_config_t ftmInfo;
	FTM_GetDefaultConfig(&ftmInfo);
	ftmInfo.prescale=kFTM_Prescale_Divide_128;
	if(hz>100){
		ftmInfo.prescale=kFTM_Prescale_Divide_1;
	}
	FTM_Init(FTM2, &ftmInfo);
}

void inxPWMConfig(const uint32_t periodHz,const uint8_t ehsPin){
	ftm_chnl_pwm_signal_param_t ftmParam[2];
    ftmParam[0].level = CONFIG_PWM_OUTPUT_MODE;
    ftmParam[0].dutyCyclePercent = 0;
    ftmParam[0].firstEdgeDelayPercent = 0U;
	ftmParam[0].chnlNumber = kFTM_Chnl_0;
	ftmParam[1].level = CONFIG_PWM_OUTPUT_MODE;
    ftmParam[1].dutyCyclePercent = 0;
    ftmParam[1].firstEdgeDelayPercent = 0U;
	ftmParam[1].chnlNumber = kFTM_Chnl_1;
	inxPWMInit(periodHz);
	uint8_t gpioPin=CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_2;
	PORT_Type* port=PORTB;
	port_mux_t mux=kPORT_PinDisabledOrAnalog;
	switch(ehsPin){
		case 0:
			gpioPin=CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_2;
			mux=kPORT_MuxAlt3;
			break;
		case 1:
			gpioPin=CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_3;
			mux=kPORT_MuxAlt3;
			break;
		default:
			break;
	}
	PORT_SetPinMux(port, gpioPin, mux);
	status_t status=FTM_SetupPwm(FTM2, ftmParam, 2U, kFTM_CenterAlignedPwm, periodHz, CLOCK_GetFreq(kCLOCK_McgFixedFreqClk));
	if(status==kStatus_Success){

	}else{
		//TODO actually report configuration errors
	}
}

void inxPWMEnable(const bool enable){
	if(enable && gFTM2Started==false){
		FTM_StartTimer(FTM2, kFTM_FixedClock);
		gFTM2Started=true;
	}else if(enable==false && gFTM2Started==true){
		FTM_StopTimer(FTM2);
		gFTM2Started=false;
	}
}

static ftm_chnl_t inxPWMPinToChannel(const uint8 pin){
	ftm_chnl_t ftmChannel=kFTM_Chnl_0;
	switch(pin){
		case 1:
			ftmChannel=kFTM_Chnl_1;
			break;
	}
	return ftmChannel;
}

void inxPWMDuty(const uint8_t pin,const uint8_t powerPercent){
	const ftm_chnl_t ftmChannel=inxPWMPinToChannel(pin);
	FTM_UpdatePwmDutycycle(FTM2, ftmChannel, kFTM_CenterAlignedPwm, powerPercent);
	FTM_SetSoftwareTrigger(FTM2, true);
}

void BOARD_FTM_HANDLER(void)
{
	gFTMTicks++;
	//if((gFTMTicks%1000000)==1){
	//	printf("PBB %d\n",gFTMTicks);
	//}
	FTM_ClearStatusFlags(BOARD_FTM_BASEADDR, kFTM_TimeOverflowFlag);
    __DSB();
}

void FTM1_IRQHandler(void)
{
	if(gEhsTick){
		Command=EhsMainLoop(NULL,NULL);
		if (Command == EHS_CONTINUE ||	Command ==	EHS_RELOAD_EHS_FROM_FILE || Command == EHS_PAUSE) {
			//if((gFTMTicks%1000)==1){
		//		printf("PBB\n");
	//		}
			EhsTgtTimer_tick();
		}
	}
	FTM_ClearStatusFlags(FTM1, kFTM_TimeOverflowFlag);
    __DSB();
}
#endif // LWIP_SOCKET
