#ifndef HRX_PID_CONFIG_INX_H_
#define HRX_PID_CONFIG_INX_H_

#include "ehs_types.h"
#include "hal_pid.h"

#warning "What is this for? Why does it have so much irrelevant stuff in it? Is for PID for all targets?"
// =================================================
// typedef for systems that doesn't support these

//todo2024 we need to get rid of these typedefs and change everything to use the ehs_ typedefs instead.
#ifndef EHS_LWIP
// typedefs that are not part of this build
typedef unsigned char uint8_t;
typedef char int8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;
typedef long int64_t;
typedef unsigned long uint64_t;
typedef unsigned long u64_t;
#ifndef EHS_MINGW
typedef uint64_t size_t;
#endif
#endif
typedef uint8_t bool;
typedef uint8_t u8_t;
typedef int8_t s8_t;
typedef uint16_t u16_t;
typedef uint32_t u32_t;
typedef int32_t status_t;
//////////////////////////////////////////////////////////
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// DUMY nxp types. Remove us once done !!!!!
typedef s8_t err_t;

typedef struct _rtc_datetime
{
    uint16_t year;  /*!< Range from 1970 to 2099.*/
    uint8_t month;  /*!< Range from 1 to 12.*/
    uint8_t day;    /*!< Range from 1 to 31 (depending on month).*/
    uint8_t hour;   /*!< Range from 0 to 23.*/
    uint8_t minute; /*!< Range from 0 to 59.*/
    uint8_t second; /*!< Range from 0 to 59.*/
} rtc_datetime_t;

typedef enum {
	INX_LED_HEATING_MODE_MANUAL,
	INX_LED_HEATING_MODE_SCHEDULE,
	INX_LED_HEATING_MODE_TIMER
} inxLedHeatingMode_t;

#define MAKE_STATUS(group, code) ((((group)*100) + (code)))

/*! @brief Status group numbers. */
enum _status_groups
{
    kStatusGroup_Generic = 0,                 /*!< Group number for generic status codes. */
};

enum _generic_status
{
    kStatus_Success = MAKE_STATUS(kStatusGroup_Generic, 0),
    kStatus_Fail = MAKE_STATUS(kStatusGroup_Generic, 1),
    kStatus_ReadOnly = MAKE_STATUS(kStatusGroup_Generic, 2),
    kStatus_OutOfRange = MAKE_STATUS(kStatusGroup_Generic, 3),
    kStatus_InvalidArgument = MAKE_STATUS(kStatusGroup_Generic, 4),
    kStatus_Timeout = MAKE_STATUS(kStatusGroup_Generic, 5),
    kStatus_NoTransferInProgress = MAKE_STATUS(kStatusGroup_Generic, 6),
};

#define GPIOB_BASE                               (0x400FF040u)
#define GPIOB 
#define LWIP_PLATFORM_DIAG printf

#define false 0
#define true 1


#define INX_DISABLE_HTTP_SRV 1

// =================================================

//we have to use volatile here or the compiler will optimise reads of these
//variables and that causes weird issues
typedef struct _inxIecInterruptData {
	volatile bool doingTest;
	volatile uint32_t interruptCount;
} inxIecInterruptData_t;

// ==================================================
// ==================================================
// ==================================================

#define CONFIG_LIFESTYLE INX_LIFESTYLE_HEATROD_PRODUCTION
#define INX_HEATROD_IOT_BOARD_VARIANT INX_HR_HRi
#define CONFIG_MQTT_LOCATION CONFIG_MQTT_LOCATION_HEATROD

// ==================================================

/*
 * Board variants available for NX_HEATROD_IOT_BOARD_VARIANT
 * */
#define INX_HR_HRD (1) // obsolete round HRd board
#define INX_HR_HRi (2) // Current HRi (Integrated HeatRod Device) - with full display board
#define INX_HR_HRc (3) // Current HRc (DIN panel mounted Heatrod Controller)
#define INX_HR_FRDMK64EVAL (4)
#define INX_HR_HRi_dv1 (5) //  HRi (Integrated HeatRod Device) - with no display board or original style display board.
#define INX_HR_HRx_COMMISSIONING_BOARD (6) // Initial software to load on to production units that carries out self text and diagnosticvs when connectedto a commissioning board. Special keys allow specific softweare to be downloaded from Devman when fully integrated with production switching boards.

//#define INX_HACK_NHc
//Board-sub-variants for INX_HEATROD_IOT_BOARD_SUBVARIANT e.g. different displays
#define INX_HR_HRi_DISPv1 (1) // displays with no negative and status LEDs are mounted on the processor board.
#define INX_HR_HRi_DISPv2 (2) // DEFAULT Displays with status LEDS wired same as HRDc (different to Processor mounted config!) - includes set button on FP - no negative.
#define INX_HR_HRi_DISPv3 (5) // potential future hrdi with a negative display and the set button on the CPU board
#define INX_HR_HRc_DISPv1 (3) // this is the first HRDc displays (made by inx where the neg bar doesn't work)
#define INX_HR_HRc_DISPv2 (4) // this is the latest (default) standard HRDc displays.

/* ************************************************* */
/* Change this to the required build!                */
#ifndef INX_HEATROD_IOT_BOARD_VARIANT
	#error You must set INX_HEATROD_IOT_BOARD_VARIANT in your platform config
#endif
/* ************************************************* */

#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRi
	#define CONFIG_MODEL "HR-I3-02-00"
    #define CONFIG_THERMAL_CUT_OUT_DISABLED 1
	#if !defined(INX_HEATROD_IOT_BOARD_SUBVARIANT)
		#define INX_HEATROD_IOT_BOARD_SUBVARIANT INX_HR_HRi_DISPv2
	#endif //if !defined(INX_HEATROD_IOT_BOARD_SUBVARIANT)
#elif INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
	#define CONFIG_MODEL "HR-C3-02-00"
	#if !defined(INX_HEATROD_IOT_BOARD_SUBVARIANT)
		#define INX_HEATROD_IOT_BOARD_SUBVARIANT INX_HR_HRc_DISPv2
	#endif //if !defined(INX_HEATROD_IOT_BOARD_SUBVARIANT)
#elif INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRx_COMMISSIONING_BOARD
	#define CONFIG_MODEL "HR-COMMISSIONING"
#elif INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRD
	#define CONFIG_MODEL "HR-HRD-ORIGINAL"
#elif INX_HEATROD_IOT_BOARD_VARIANT ==  INX_HR_FRDMK64EVAL
	#define CONFIG_MODEL "HR-NXP-FRDMEVALBAORD"
#else
	#error "You must choose a valid board variant"
#endif

/* End User Configurations */
#define CONFIG_MQTT_LOCATION_INX (0)
#define CONFIG_MQTT_LOCATION_HEATROD (1)
#define CONFIG_MQTT_LOCATION_PATRICK (2)
#define CONFIG_MQTT_LOCATION_TEST (3)
#define CONFIG_MQTT_LOCATION_INX_PREMIGRATION (100)

// WARNING DONT CHECK THIS LINE IN UNCOMMENTED!
//#define CONFIG_MQTT_LOCATION CONFIG_MQTT_LOCATION_INX

#ifndef CONFIG_MQTT_LOCATION
	#error You must set CONFIG_MQTT_LOCATION in your platform config
#endif


/*
 * lifestyle choices - i.e. features to include that are not implicit in the board type or end user configuration:
 * Safety features enabled
 * Debug options
 */

#define INX_LIFESTYLE_DEBUG (1)
#define INX_LIFESTYLE_HEATROD_DEBUG (2)
#define INX_LIFESTYLE_HARDWARE_DEBUG (3) // use this to set INX_HEATROD_IOT_DIAGNOSTIC_BUILD
// ONLY LET THIS BE SET WHEN TARGET IS INX_HR_HRx_COMMISSIONING_BOARD!!
#define INX_LIFESTYLE_COMMISSION (4)
#define INX_LIFESTYLE_DISPLAY_HARDWARE_TESTER (5)
#define INX_LIFESTYLE_HEATROD_PRODUCTION (1000)
#define INX_LIFESTYLE_EHS (2000)
/* some lifestyles re required by board variants */
#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRx_COMMISSIONING_BOARD
 	#if CONFIG_LIFESTYLE != INX_LIFESTYLE_DISPLAY_HARDWARE_TESTER
		#define CONFIG_LIFESTYLE INX_LIFESTYLE_COMMISSION // We should force this for this boar type build for all other cases.
	#endif
#else
 	#if CONFIG_LIFESTYLE == INX_LIFESTYLE_COMMISSION
		#error " Only use CONFIG_LIFESTYLE=INX_LIFESTYLE_COMMISSION for BOARD VARIANT == INX_HR_HRx_COMMISSIONING_BOARD"
	#endif
#endif

/* System Parameters */
#define INX_THERMOCOUPLE_OFFSET_CALIBRATION_TEMPERATURE 35.0f
#define INX_THERMOCOUPLE_SCALE_CALIBRATION_TEMPERATURE 0.0f

#define INX_PT100_3W_OFFSET_CALIBRATION_TEMPERATURE 0.0f // Currently UNUSED!!!
#define INX_PT100_3W_SCALE_CALIBRATION_TEMPERATURE 3.0f //  Consistent error found using reads 1C too low at 35C when offset calibrated for 0C.


/* WARNING! DONT CHECK THIS NEXT LINE IN INCOMMENTED
 It should be set by jenkins or in the IDE preprocessor's project settings
 //#define CONFIG_LIFESTYLE INX_LIFESTYLE_DEBUG
 */
#ifndef CONFIG_LIFESTYLE
	#error You must set CONFIG_LIFESTYLE in your platform config
#endif

#if CONFIG_LIFESTYLE == INX_LIFESTYLE_DEBUG
	#define CONFIG_IGNORE_FATAL_FAULTS_AND_REPORT (1)
	#define CONFIG_THERMAL_CUT_OUT_DISABLED (0)
	//#define CONFIG_MORE_LWIP_DEBUG
	#define CONFIG_ALWAYS_DIAGNOSTIC_BOOT (0)
	#define MQTT_APP_DEBUG_TRACE LWIP_DBG_OFF
	#define DEBUG_IEC LWIP_DBG_OFF
	#define CONFIG_FORCE_PROVISIONED (0)
	#define CONFIG_RTC_FAKE_BEING_STUCK (0)
	#define CONFIG_FIRMWARE_IGNORE_UPDATES (0)
	#define CONFIG_OTA_DIAG (0)
	#define CONFIG_SCHEDULE_DIAG (0)
	#define DEBUG_WS (0)
	#define INX_HTTP_HARDWARE_SESSION_LENGTH_SECONDS 120
	#define CONFIG_UART_DEBUG 1
	#define CONFIG_HIGH_PRIORITY_UART 0
#elif CONFIG_LIFESTYLE == INX_LIFESTYLE_HARDWARE_DEBUG
	#define INX_HEATROD_IOT_DIAGNOSTIC_BUILD 1
	#define CONFIG_IGNORE_FATAL_FAULTS_AND_REPORT (1)
#elif CONFIG_LIFESTYLE == INX_LIFESTYLE_HEATROD_DEBUG
	#define CONFIG_FIRMWARE_IGNORE_UPDATES (1)
	#define CONFIG_MORE_LWIP_DEBUG
	#define CONFIG_IGNORE_FATAL_FAULTS_AND_REPORT (1)
    #if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRi
	  #define CONFIG_ALLOW_SSR_TIMINGS_ON_INTERNAL (1)
    #else
	  #define CONFIG_ALLOW_SSR_TIMINGS_ON_INTERNAL (0)
    #endif
#elif CONFIG_LIFESTYLE == INX_LIFESTYLE_HEATROD_PRODUCTION
	#if CONFIG_IGNORE_FATAL_FAULTS_AND_REPORT == 1
		#error "You must not set CONFIG_IGNORE_FATAL_FAULTS_AND_REPORT for production builds"
	#endif
	#ifdef CONFIG_MORE_LWIP_DEBUG
		#error "You must not set set WLIP debug for production builds"
	#endif
	#ifdef INX_HEATROD_IOT_DIAGNOSTIC_BUILD
		#error "You must not set INX_HEATROD_IOT_DIAGNOSTIC_BUILD for production builds"
	#endif
	#ifdef MQTT_APP_DEBUG
		#error "You must not set MQTT_APP_DEBUG for production builds"
	#endif

//todo should the folowign #def really be for th commisioningboard hardwre type and not the lifestyle
#elif CONFIG_LIFESTYLE == INX_LIFESTYLE_COMMISSION || CONFIG_LIFESTYLE == INX_LIFESTYLE_DISPLAY_HARDWARE_TESTER
	//#define INX_HEATROD_IOT_DIAGNOSTIC_BUILD 1 - we don't want all the HW diagnostics for the commissioning build
	#define CONFIG_GPIO_SENSE_LED_BASE GPIOE
	#define CONFIG_GPIO_SENSE_LED_PORT PORTE
	#define CONFIG_GPIO_SENSE_LED_PIN (24U)
#else //elif CONFIG_LIFESTYLE == INX_LIFESTYLE_COMMISSION
	#warning "You should set a build lifestyle. DEFAULTING TO CONFIG_LIFESTYLE == INX_LIFESTYLE_HEATROD_PRODUCTION for production builds"
	//#error "You must set CONFIG_LIFESTYLE in your platform config"
    #define CONFIG_LIFESTYLE INX_LIFESTYLE_HEATROD_PRODUCTION
#endif //CONFIG_LIFESTYLE == INX_DEBUG

//#define CONFIG_FIRMWARE_VERSION "Mystery.pmld"
#ifndef CONFIG_FIRMWARE_VERSION
	//#warning "You need to set the CONFIG_FIRMWARE_VERSION to deploy versions."
	//#warning "Setting to MAKE_MYSTERY"
	#define CONFIG_FIRMWARE_VERSION "UNKNOWN_MAKE_VERSION"
#endif

#ifndef CONFIG_MODEL
	//manufacturer designator (2 digits) - model designator and PID num (2 digits, e.g i1, i2 etc or c1, c2 etc) - version (2 digits, e.g 00 or 01) - power connection designator (2 digits)
	//ALL CAPS
	#define CONFIG_MODEL "HR-X1-00-00"
#endif

#ifndef DEBUG_WS
	#define DEBUG_WS (0)
#endif

#ifndef CONFIG_HIGH_PRIORITY_UART
	#define CONFIG_HIGH_PRIORITY_UART 0
#endif //ifndef CONFIG_HIGH_PRIORITY_UART

#ifndef CONFIG_OTA_DIAG
	#define CONFIG_OTA_DIAG (0)
#endif //CONFIG_OTA_DIAG

#ifndef CONFIG_SCHEDULE_DIAG
	#define CONFIG_SCHEDULE_DIAG (0)
#endif //CONFIG_SCHEDULE_DIAG

#ifndef CONFIG_FIRMWARE_IGNORE_UPDATES
	#define CONFIG_FIRMWARE_IGNORE_UPDATES (0)
#endif

#define CONFIG_NO_RESET_VERSION_NUMBER (1)
#define INX_FLASH_VALIDATION_CODE (0xdeadbee3) //must be updated when the permStorage_t is changed

#ifndef CONFIG_IGNORE_FATAL_FAULTS_AND_REPORT
	#define CONFIG_IGNORE_FATAL_FAULTS_AND_REPORT (0)
#endif

#ifndef CONFIG_FORCE_PROVISIONED
	#define CONFIG_FORCE_PROVISIONED (0)
#endif

#ifndef CONFIG_RTC_SECONDS_BETWEEN_CHECKS
	#define CONFIG_RTC_SECONDS_BETWEEN_CHECKS (10)
#endif

#ifndef CONFIG_RTC_FAKE_BEING_STUCK
	#define CONFIG_RTC_FAKE_BEING_STUCK (0)
#endif

#ifndef CONFIG_ALWAYS_DIAGNOSTIC_BOOT
	#define CONFIG_ALWAYS_DIAGNOSTIC_BOOT (0)
#endif

#ifndef CONFIG_THERMAL_CUT_OUT_DISABLED
	#define CONFIG_THERMAL_CUT_OUT_DISABLED (0)
#endif

#ifndef CONFIG_ALLOW_SSR_TIMINGS_ON_INTERNAL
	#define CONFIG_ALLOW_SSR_TIMINGS_ON_INTERNAL (0)
#endif

#ifndef DEBUG_IEC
	#define DEBUG_IEC LWIP_DBG_OFF
#endif

#ifndef INX_HEATROD_WATCHDOGTEST
	#define INX_HEATROD_WATCHDOGTEST (1U) //causes an initial watch dog reboot on start up, part of IEC
#endif

#define CONFIG_PWM_OUTPUT_MODE kFTM_LowTrue
#ifndef INX_HTTP_SESSION_LENGTH_SECONDS
	#define INX_HTTP_SESSION_LENGTH_SECONDS (8*60*60U)
#endif //INX_HTTP_SESSION_LENGTH_SECONDS

#ifndef INX_HTTP_HARDWARE_SESSION_LENGTH_SECONDS
	#define INX_HTTP_HARDWARE_SESSION_LENGTH_SECONDS (1*60*60U)
#endif //INX_HTTP_HARDWARE_SESSION_LENGTH_SECONDS

#define INX_HTTP_PATH_POST_SESSION_START "post_session_start"

/*
 * Calibration System Parameters
 */
//#define INX_CONTROLLER_CALIBRATION_THERMOCOUPLE_TEMPERATURE 20.0f
/*
 * System Software Parameters
 * todo - move this type of stuff to dedicated detailed config file, so we don't break tings accidently fidding around in here
 */
#ifndef WDOG_WCT_INSTRUCITON_COUNT
	#define WDOG_WCT_INSTRUCITON_COUNT (256U) //how long we wait for the watch dog to update after a settings change
#endif

#ifndef CONFIG_PHY_DO_LOOPBACK
	#define CONFIG_PHY_DO_LOOPBACK 0U
#endif

#ifndef CONFIG_MQTT_TEST_TLS_MEM_LEAK
	#define CONFIG_MQTT_TEST_TLS_MEM_LEAK (0U)
#endif

#ifndef CONFIG_NETWORK_TEST_DHCP_MEM_LEAK
	#define CONFIG_NETWORK_TEST_DHCP_MEM_LEAK (0U)
#endif

#ifndef CONFIG_DO_FLASH_TEST
	#define CONFIG_DO_FLASH_TEST (0) //enables the CRC check for the program flash, IEC //IEC TODO!!! check this sould be enabled
#endif
#ifndef CONFIG_IEC_RAM_CHUNKS
	#define CONFIG_IEC_RAM_CHUNKS 8 //the number of chunks to split the RAM in for testing
#endif
//this test caused intermittent on some devices so we made it optional
#ifndef CONFIG_IEC_CHECK_RTC_DURING_ADC_CHECK
	#define CONFIG_IEC_CHECK_RTC_DURING_ADC_CHECK (0)
#endif

#ifndef CONFIG_IEC_PERIODIC_TEST_INTERVAL_SECONDS
	#define CONFIG_IEC_PERIODIC_TEST_INTERVAL_SECONDS (60U) //how many seconds before running the periodic self test IEC
#endif

#ifndef CONFIG_IEC_INTERRUPT_TEST_LENGTH_SECONDS
	#define CONFIG_IEC_INTERRUPT_TEST_LENGTH_SECONDS (10U)
#endif

//todo2024 - this is vague is it used? what thread for if so?
#ifndef CONFIG_STACK_SIZE
	#define CONFIG_STACK_SIZE 0x800 //must match up with the linker settings
#endif
#ifndef CONFIG_HEAP_SIZE_BYTES
	#define CONFIG_HEAP_SIZE_BYTES 0x12000 //must match up with the linker settings
#endif

#ifndef DEMO_RING_BUFFER_SIZE
	#define DEMO_RING_BUFFER_SIZE 16 //uart ring buffer size for when we are testing and demoing
#endif

#ifndef MQTT_TASK_STACK_SIZE
	#define MQTT_TASK_STACK_SIZE 1800
#endif
#ifndef SUPERVISOR_TASK_STACK_SIZE
	#define SUPERVISOR_TASK_STACK_SIZE 1200
#endif

#ifndef MQTT_TASK_PRIORITY
	//be slightly more important than the networking so we don't miss data from the adc
	#define MQTT_TASK_PRIORITY CONFIG_MAIN_THREAD_PRIORITY
#endif
#ifndef SUPERVISOR_TASK_PRIORITY
	//be slightly more important than the networking so we don't miss data from the adc
	#define SUPERVISOR_TASK_PRIORITY MQTT_TASK_PRIORITY + 1
#endif

/* default MAC address configuration. gets partially overridden with cpuid */
#ifndef CONFIG_MAC_ADDR
	#define CONFIG_MAC_ADDR {0x02, 0x12, 0x13, 0x10, 0x15, 0x11}
#endif
/* Address of PHY interface. */
#define CONFIG_ENET0_PHY_ADDRESS BOARD_ENET0_PHY_ADDRESS
/* System clock name. */
#define CONFIG_ENET0_CLOCK_NAME kCLOCK_CoreSysClk
#ifndef CONFIG_MDNS_ENABLED
	#define CONFIG_MDNS_ENABLED (0)
#endif
#define CONFIG_MDNS_HOSTNAME "mDnsInxIoT "
#ifndef CONFIG_WEB_SOCKET_MESSAGE_ENABLED
	#define CONFIG_WEB_SOCKET_MESSAGE_ENABLED (1U)
#endif

#define CONFIG_ORGANISATION_NAME_LENGTH (128U)
#define CONFIG_NETWORK_TASK_POLL_DELAY 5
#define CONFIG_NETWORK_TASK_POLL_DELAY_MAX_TICKS (1000/CONFIG_NETWORK_TASK_POLL_DELAY)
#define CONFIG_BUTTONS_TIMEOUT_SECONDS 20
#define CONFIG_BUTTONS_TIMEOUT_TICKS (CONFIG_NETWORK_TASK_POLL_DELAY_MAX_TICKS * CONFIG_BUTTONS_TIMEOUT_SECONDS)
#define CONFIG_BUTTONS_IDLE_SECONDS 1
#define CONFIG_BUTTONS_IDLE_TICKS (CONFIG_BUTTONS_IDLE_SECONDS * CONFIG_NETWORK_TASK_POLL_DELAY_MAX_TICKS)
#define CONFIG_BUTTONS_SET_POINT_SMALL_INCREMENT 0.1
#define CONFIG_BUTTONS_MIN_HOLD_TICKS CONFIG_NETWORK_TASK_POLL_DELAY_MAX_TICKS/20
#define CONFIG_BUTTONS_MAX_HOLD_TICKS CONFIG_NETWORK_TASK_POLL_DELAY_MAX_TICKS
#define CONFIG_BUTTONS_LONG_PRESS_TICKS CONFIG_BUTTONS_MAX_HOLD_TICKS*2
#define CONFIG_BUTTONS_TIMER_SMALL_INCREMENT 1
#define CONFIG_BUTTONS_TIMER_MIN 0
#define CONFIG_BUTTONS_TIMER_MAX 9999
#define CONFIG_BUTTONS_10V_THRESHOLD 40.0
#define CONFIG_LED_TIMER_FLASH_RATE_MILLISECONDS 333
#define CONFIG_LED_PROVISIONED_FLASH_RATE_MILLISECONDS 1000

#ifndef INX_EMR_MIN_SECONDS
	#define INX_EMR_MIN_SECONDS (1)
#endif //INX_EMR_MIN_SECONDS

#ifndef INX_EMR_MAX_SECONDS
	#define INX_EMR_MAX_SECONDS (20*60)
#endif //INX_EMR_MAX_SECONDS

#ifndef INX_SSR_MIN_SECONDS
	#define INX_SSR_MIN_SECONDS (0.001)
#endif //INX_SSR_MIN_SECONDS

//#define CONFIG_PWM_MAX_SECONDS (1.0f)
#define CONFIG_PWM_MAX_SECONDS (1.0f) //we use negative for now to disable this mode
#define CONFIG_PWM_OUTPUT_MODE kFTM_LowTrue

#ifndef INX_SSR_MAX_SECONDS
	#define INX_SSR_MAX_SECONDS (60)
#endif //INX_SSR_MAX_SECONDS

#define CONFIG_EMR_RESPONSE_TIME_MILLISECONDS (100U)
#define CONFIG_LPTMR_BASE LPTMR0
/* ! NXP  specific
// Low Power Timer interrupt time in milliseconds
// this 1000 / numADCReads
// we currently have 3 ADC reads, read1: ADC0 + ADC1 temperature, read2: ADC0+ADC1 duty, read3: AC1 cpu temp
#define CONFIG_LPTMR_NUM_READS (3U)
*/
// esp32s3 - we have a single interrupt that reads all ADCs
#define CONFIG_LPTMR_NUM_READS (1U)
// esp32s3 - ISR is 1000 HZ , make sure this gets updated when 'EHS_PID_ADC_FREQ_HZ' changes
#define CONFIG_SAMPLES_PER_DUTY_CYCLE (60U)

#define CONFIG_LPTMR_DEFAULT_INTERRUPT_MILLISECONDS (33U)
#define ADCR_VDD (4095U) /* Maximum value when use 12b resolution */
#define V_BG (1000U)      /* BANDGAP voltage in mV (trim to 1.0V) */
#define V_TEMP25 (716U)   /* Typical VTEMP25 in mV */
#define INX_M (1620U)         /* Typical slope: (mV x 1000)/oC */

#define CONFIG_WEB_SOCKET_DATA_TICKS_SECONDS (10) //how often we update our status on the web socket
#define CONFIG_CGI_DATA_LENGTH_MAX (1600) //how much data we can receive or send in a post request
#define CONFIG_PROVISIONING_CERTIFICATE_MAX_LENGTH CONFIG_CGI_DATA_LENGTH_MAX
#define CONFIG_CERTIFICATE_PASSWORD_LENGTH (33)
#define CONFIG_MQTT_MAX_TLS_TICKS (100) //used to timeout TLS handshakes
#define CONFIG_MQTT_MAX_TOPIC_LENGTH (128)
#define CONFIG_MQTT_MAX_CONNECTION_ATTEMPTS (10U) //number of mqtt connection attempts before we cycle the network stack
#define INX_MQTT_MAX_PAYLOAD_SIZE (512)
#define CONFIG_ERROR_MESSAGE_SIZE INX_MQTT_MAX_PAYLOAD_SIZE/4
#define CONFIG_FLASH_SECTOR_SIZE_BYTES (4096)
#define CONFIG_MAX_SCHEDULE_SIZE_BYTES CONFIG_FLASH_SECTOR_SIZE_BYTES
#define CONFIG_HOURS_IN_WEEK (7*24)
/*
 * System Features
 */
//#ifndef CONFIG_MQTT_ENABLED
//	#define CONFIG_MQTT_ENABLED (1)
//#endif
//#define CONFIG_DHCP_WAIT_TICKS (32U) //how long we wait for DHCP to work
//#define CONFIG_DNS_WAIT_TICKS (32U)
//#define LWIP_DHCP_PROVIDE_DNS_SERVERS 1 //so we can get DNS servers from a dhcp request

/* In case of the protected sectors at the end of the pFlash just select
the block from the end of pFlash to be used for operations
CONFIG_FLASH_SECTOR_INDEX_FROM_END = 1 means the last sector,
CONFIG_FLASH_SECTOR_INDEX_FROM_END = 2 means (the last sector - 1) ...
in case of FSL_FEATURE_FLASH_HAS_PFLASH_BLOCK_SWAP it is
CONFIG_FLASH_SECTOR_INDEX_FROM_END = 1 means the last 2 sectors with width of 2 sectors,
CONFIG_FLASH_SECTOR_INDEX_FROM_END = 2 means the last 4 sectors back
with width of 2 sectors ...
*/
#ifndef CONFIG_FLASH_SECTOR_INDEX_FROM_END
  #define CONFIG_FLASH_SECTOR_INDEX_FROM_END 1U
#endif

#ifndef CONFIG_NUMBER_OF_FLASH_SECTORS_FOR_SETTINGS
	#define CONFIG_NUMBER_OF_FLASH_SECTORS_FOR_SETTINGS 4
#endif

#ifndef HTTPSRV_CFG_MBEDTLS_ENABLE
	#define HTTPSRV_CFG_MBEDTLS_ENABLE 0 //makes the local webserver use TLS, experimental
#endif

#ifndef CONFIG_COMMISSION_NUM_FLASHES
	#define CONFIG_COMMISSION_NUM_FLASHES 5
#endif //ifndef CONFIG_COMMISSION_NUM_FLASHES

/*
 * Application default settings and ranges
 */

#define CONFIG_FIRMWARE_NAME_MAX_LENGTH 64
#define CONFIG_FIRMWARE_NAME_MAX_LENGTH_STRING "64"

#define CONFIG_DEFAULT_DUTY_CYCLE_LENGTH_EMR_SECONDS (5*60)
#define CONFIG_DEFAULT_DUTY_CYCLE_LENGTH_SSR_SECONDS (1U)
#define CONFIG_FIRMWARE_CHUNK_SIZE 512
#define CONFIG_FIRMWARE_CHUNK_SIZE_STRING "512"
#define BL_FEATURE_RELIABLE_UPDATE (1) //enables the code for doing a firmware update
#define INX_HEATROD_SAVE_SCHEDULE //allows us to save the schedule in our permanent storage

#define INX_SAFE_TEMPERATURE_LOWER_BOUNDS (-60.0)
#define INX_SAFE_TEMPERATURE_UPPER_BOUNDS (260.0)
#define INX_SAMPLEBUF_SIZE 10
#define CONFIG_MAIN_THREAD_PRIORITY TCPIP_THREAD_PRIO + 1
#define CONFIG_SUPERVISOR_LWIP_STOPPED_WAIT_TICKS (60)
#define CONFIG_LOCAL_NAME_LENGTH (101)
#define CONFIG_LOCATION_LENGTH (101)
#define CONFIG_TIME_ZONE_LENGTH (25)
#define CONFIG_MAX_TIME_DRIFT_SECONDS (60)
#define CONFIG_MAX_MILLISECONDS_BETWEEN_SAVES (24*60*60*1000) //we save roughly once a day so that we can track relay on counts
//#define CONFIG_MAX_MILLISECONDS_BETWEEN_SAVES (1*30*1000) //we save roughly once a day so that we can track relay on counts
#define CONFIG_TIME_DRIFT_TICKS (86400) //24 hours, how often we try to get an updated time
#define CONFIG_TIME_SUBSCRIBE_TICKS (180) //how long we wait for a time message after subscribing
#define CONFIG_TIME_SUBSCRIBE_RTC_DRIFTED_WAIT (8640000) //todo dheck this is OK with changes to sampling rates. Should be 10 days, if we haven't updated time after that scheduling stops
#define CONFIG_TIME_DEBUG LWIP_DBG_ON
#define CONFIG_ADC_VALUES_QUEUE_LENGTH (2)
#define CONFIG_PID_CONDITION_MONITORING_DEBUG LWIP_DBG_OFF
#define CONFIG_PID_CONDITION_MONITORING_WINDOW_SAMPLES (5)
#define CONFIG_PID_CONDITION_MONITORING_RETRY_SAMPLES CONFIG_PID_CONDITION_MONITORING_WINDOW_SAMPLES*2
#define CONFIG_PID_CONDITION_MONITORING_MAX_TEMP_DIFF (50.0) // This can b quite large when booting and the temperature read is high
#define CONFIG_PID_CONDITION_MONITORING_MIN_TEMP_DIFF CONFIG_PID_CONDITION_MONITORING_MAX_TEMP_DIFF*-1.0
#define CONFIG_PID_CONDITION_MONITORING_RETRY_LIMIT (2)
//#define CONFIG_PID_I_MAX 200 // set by port now
//changing this is dangerous, will need testing
#define CONFIG_PID_CONTROLLERS_NUM (3U)
#define CONFIG_PID_OUTPUTS_NUM (3U)
#define CONFIG_DUTY_CYCLES_NUM (6U) //3 internal, 3 external
#define CONFIG_DUTY_CYCLE_HISTORY_LENGTH (10U)

#define INX_CPU_ID_MAX_LENGTH (200U)
#define HASH_CLIENT_ID_MAX_LENGTH (11U)
#define HASH_PIN_MAX_LENGTH (6U)
#define DISPLAY_CLIENT_ID_MAX_LENGTH (HASH_CLIENT_ID_MAX_LENGTH + 3U)

/*
 * MODBUS Settings
 */
#define REG_INPUT_START 				1000 //first numerical address of our registers
#define REG_INPUT_NREGS 				8 //the amount of input registers we have
#define REG_HOLDING_START           	2000 //the first numerical address of our holding registers
#define REG_HOLDING_NREGS           	50 //the amount of holding registers we have
#define REG_COILS_START					0 //the first address of our coil registers
#define REG_COILS_NREGS					50 //the number of coil registers
#define REG_DISCRETE_COILS_START		100 //the first address of our discrete coils
#define REG_DISCRETE_NREGS				50 //the number of discrete
#define CONFIG_MODBUS_PID_COEFFICIENT_SCALE_FACTOR 1000 //how we scale our PID values to avoid transmitting floats
#define CONFIG_MODBUS_TEMPERATURE_INDEX (0U)
#define CONFIG_MODBUS_EXTERNAL_TEMPERATURE_INDEX (1U)
#define CONFIG_MODBUS_MCU_TEMPERATURE_INDEX (2U)
#define CONFIG_MODBUS_SET_POINT_INDEX (3U)
#define CONFIG_MODBUS_DUTY_OVERALL_INDEX (4U)
#define CONFIG_MODBUS_DUTY_RELAY1_INDEX (5U)
#define CONFIG_MODBUS_DUTY_RELAY2_INDEX (6U)
#define CONFIG_MODBUS_DUTY_RELAY3_INDEX (7U)
#define CONFIG_MODBUS_TIME_YEAR_INDEX (8U)
#define CONFIG_MODBUS_TIME_MONTH_INDEX (9U)
#define CONFIG_MODBUS_TIME_DAY_INDEX (10U)
#define CONFIG_MODBUS_TIME_HOUR_INDEX (11U)
#define CONFIG_MODBUS_TIME_MINUTE_INDEX (12U)
#define CONFIG_MODBUS_TIME_SECOND_INDEX (13U)
#define CONFIG_MODBUS_NETWORK_CONNECTION_MODE_INDEX (14U)
#define CONFIG_MODBUS_ERROR_STATE_INDEX (15U)
#define CONFIG_MODBUS_ALARM_STATE_INDEX (16U)
#define CONFIG_MODBUS_ALARM_TIMEOUT_INDEX (17U)
#define CONFIG_MODBUS_POWER_MODE (18U)
#define CONFIG_MODBUS_ELEMENT_MODE (19U)
#define CONFIG_MODBUS_AUX_OUTPUT (20U)
#define CONFIG_MODBUS_ELEMENT_TWO_STEPPED_THRESHOLD (21U)
#define CONFIG_MODBUS_ELEMENT_THREE_STEPPED_THRESHOLD (22U)
#define CONFIG_MODBUS_PID1_KP (23U)
#define CONFIG_MODBUS_PID1_KI (24U)
#define CONFIG_MODBUS_PID1_KD (25U)
#define CONFIG_MODBUS_DESIRED_TEMPERATURE_INDEX (26U)
#define CONFIG_MODBUS_C2_TEMPERATURE_INDEX (27U)
#define CONFIG_MODBUS_C2_MSP_INDEX (28U)
#define CONFIG_MODBUS_C2_DUTY_OVERALL_INDEX (29U)
#define CONFIG_MODBUS_C2_DUTY_RELAY1_INDEX (30U)
#define CONFIG_MODBUS_C2_DUTY_RELAY2_INDEX (31U)
#define CONFIG_MODBUS_C2_DUTY_RELAY3_INDEX (32U)
#define CONFIG_MODBUS_C2_PIDKP_INDEX (33U)
#define CONFIG_MODBUS_C2_PIDKI_INDEX (34U)
#define CONFIG_MODBUS_C2_PIDKD_INDEX (35U)
#define CONFIG_MODBUS_C2_DESIRED_TEMPERATURE_INDEX (36U)
#define CONFIG_MODBUS_C3_TEMPERATURE_INDEX (37U)
#define CONFIG_MODBUS_C3_MSP_INDEX (38U)
#define CONFIG_MODBUS_C3_DUTY_OVERALL_INDEX (39U)
#define CONFIG_MODBUS_C3_DUTY_RELAY1_INDEX (40U)
#define CONFIG_MODBUS_C3_DUTY_RELAY2_INDEX (41U)
#define CONFIG_MODBUS_C3_DUTY_RELAY3_INDEX (42U)
#define CONFIG_MODBUS_C3_PIDKP_INDEX (43U)
#define CONFIG_MODBUS_C3_PIDKI_INDEX (44U)
#define CONFIG_MODBUS_C3_PIDKD_INDEX (45U)
#define CONFIG_MODBUS_C3_DESIRED_TEMPERATURE_INDEX (46U)

/*
 * High-level TCPIP Networking Protocol Configuration
 *
 */
#if CONFIG_MQTT_LOCATION == CONFIG_MQTT_LOCATION_INX || CONFIG_MQTT_LOCATION == CONFIG_MQTT_LOCATION_INX_PREMIGRATION
	#define CONFIG_BROKER_HOST_NAME       "www.inx-systems.com"
#elif CONFIG_MQTT_LOCATION == CONFIG_MQTT_LOCATION_PATRICK
	#define CONFIG_BROKER_HOST_NAME "www.somedomain.com"
#elif CONFIG_MQTT_LOCATION == CONFIG_MQTT_LOCATION_TEST
	#define CONFIG_BROKER_HOST_NAME "mqtt2.inx-systems.net"
#else
	#define CONFIG_BROKER_HOST_NAME       "iot-vs1.nibe.se"
#endif
#define CONFIG_BROKER_HOST_IP       NULL
#define CONFIG_CLIENT_GROUP_ID        "group1" /* each client connected to the host has to use a unique ID */
#define CONFIG_CLIENT_USER_NAME       "inx"
#define CONFIG_CLIENT_USER_PASSWORD   "88dogcat99"
#define CONFIG_TOPIC_NAME             "measurements/"CONFIG_CLIENT_GROUP_ID"/%s/datum"
#define CONFIG_SETTINGS_TOPIC_NAME    "settings/"CONFIG_CLIENT_GROUP_ID"/%s/updated"
#define CONFIG_LOGS_TOPIC_NAME        "logs/"CONFIG_CLIENT_GROUP_ID"/%s/error"
#define CONFIG_ALARMS_TOPIC_NAME	"alarms/"CONFIG_CLIENT_GROUP_ID"/%s/usertemperature"
#define CONFIG_TOPIC_NAME_DUTY_MEASUREMENTS "measurements/"CONFIG_CLIENT_GROUP_ID"/%s/duty"
#define CONFIG_MQTT_TOPIC_NAME_UPDATE_SETTINGS "update/settingsv2/%s"
#define CONFIG_MQTT_TOPIC_NAME_RESET "update/reset/%s"
#define CONFIG_MQTT_TIME_CHANNEL "inx/time"

/* K64 Pin mappings
 * todo move this to K64 mcu-specific config file
 */
#define DEMO_UART UART0
#define DEMO_UART_CLKSRC UART0_CLK_SRC
#define DEMO_UART_CLK_FREQ CLOCK_GetFreq(UART0_CLK_SRC)
#define DEMO_UART_IRQn UART0_RX_TX_IRQn
#define DEMO_UART_IRQHandler UART0_RX_TX_IRQHandler

#define CONFIG_ADC_CHANNEL_GROUP0 0U
#define CONFIG_ADC_CPU_TEMP_CHANNEL 26U
#define CONFIG_ADC_BAND_GAP_CHANNEL (27U)
#define CONFIG_ADC_CPU_TEMP_CHANNEL_GROUP 0U
#define CONFIG_ADC_CPU_TEMP_STANDARD_TEMP (25U)
#define CONFIG_ADC0_DP1_420MA 1U
#define CONFIG_ADC1_DP1_010V 1U
#define CONFIG_ADC0_DP0_PT100_CHANNEL1 0U
#define CONFIG_ADC1_DP0_PT100_CHANNEL2 0U

#define CONFIG_ADC0_DM1_THERMOCOUPLE1_CHANNEL 0x14
#define CONFIG_ADC0_DM0_3WIREPT100_CHANNEL 0x13

/* from PKG-K64 Spreadsheet
 * HRC config:
[ADC0 - DP 0] 4 - 20ma
[ADC0 - DM 0]  [BoB-CON1 - pin 16 thermocouple [ ADC channel no. 10011B/0x13]
[ADC1] 0 - 10V
[ADC0 - DM1]  [BoB-CON1 - pin 14] three wire pt100  [ ADC channel no. 10100B/0x14]
[ADC1] pt100 channel2

see ADC channel numbers are defined in section 3.7.1.3.1
https://www.nxp.com/docs/pcn_attachments/16373_KV31_512R-reference-manual_Rev3_RC.pdf
for ADC channel number magic numbers
 */

#define CONFIG_ADC_BASE_0 ADC0
#define CONFIG_ADC_BASE_1 ADC1

#define CONFIG_GPIO_BASE_0 GPIOB

//heater configs
#define CONFIG_HEATER_ONE_GPIO_BASE GPIOB
#define CONFIG_HEATER_TWO_GPIO_BASE GPIOB
#define CONFIG_HEATER_THREE_GPIO_BASE GPIOB

/* various configuration settings */
#define CONFIG_USE_FREERTOS                     (0) /* 1: use FreeRTOS; 0: bare metal */
#define CONFIG_USE_DNS                          (1) /* 1: use DNS to get broker IP address; 0: do not use DNS, use fixed address instead */
#define CONFIG_USE_DHCP                         (1) /* 1: use DHCP for board address, netmask and gateway; 0: use fixed IP addresses */
#define CONFIG_USE_SNTP                         (0) /* 1: use SNTP to get RTC time; 0: do not use SNTP */
#define CONFIG_USE_SERVER_VERIFICATION          (1) //(1 && !CONFIG_USE_BROKER_HSLU && !CONFIG_USE_BROKER_AZURE) /* 1: verify server with certificate; 0: no server verification */
#define CONFIG_ENET_ENABLED (1U)

#if CONFIG_USE_SNTP
  #define SNTP_TIME_OFFSET_TIME_ZONE   -1  /* offset hours for time zone */
  #define SNTP_TIME_OFFSET_DLS         -1  /* offset for daylight saving time */
#endif

/* Client IP address configuration. */
#define configIP_ADDR0 192
#define configIP_ADDR1 168
#define configIP_ADDR2 0
#define configIP_ADDR3 75

/* Client Netmask configuration. */
#define configNET_MASK0 255
#define configNET_MASK1 255
#define configNET_MASK2 255
#define configNET_MASK3 0

/* Client Gateway address configuration. */
#define configGW_ADDR0 8
#define configGW_ADDR1 8
#define configGW_ADDR2 8
#define configGW_ADDR3 8

#define COFNIG_DNS_ADDR0 8
#define COFNIG_DNS_ADDR1 8
#define COFNIG_DNS_ADDR2 8
#define COFNIG_DNS_ADDR3 8

/* Boot and display settings todo mixture of low level hardware-specific and and high-level fuctional config in here to seperate out one day */

#define CONFIG_STARTUP_RELAY_CHECK_TICKS (120000000/8)

#define CONFIG_INX_DISPLAY_ERROR_NUM_TICKS_OFF_LIMIT 20
#define CONFIG_INX_DISPLAY_ERROR_NUM_TICKS_ON_LIMIT 100
#define CONFIG_INX_DISPLAY_NEGATIVE_NUM_TICKS_OFF_LIMIT 10
#define CONFIG_INX_DISPLAY_NEGATIVE_NUM_TICKS_ON_LIMIT 50

/* Production HRI config */
#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRi || INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc || INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRx_COMMISSIONING_BOARD
	#define INX_HEATROD_BOOT_LED_TEST (1U)
	#define CONFIG_RELAY_ON_TEMPERATURE_ADJUSTMENT (0.05)
	#define CONFIG_DISPLAY_NUM_CHARS 4
	#define CONFIG_FLEX_RAM_START_ADDRESS 0x14000000
	#define CONFIG_FLAX_RAM_SIZE 0x1000
	#define CONFIG_SRAM_UPPER_START_ADDRESS 0x20000000
	#define CONFIG_SRAM_UPPER_SIZE 0x30000
	//#define  INX_DEMO_DEVICE
	//we have to disable RTC if vbat is not wired up correctly
	#define CONFIG_MAX_PROGRAM_SIZE_SECTORS 125
	#define CONFIG_ENABLE_RTC (0)     // TODO - this will probably need enabling
	#define CONFIG_MODBUS_ENABLED (0) // TODO - this will probably need enabling
#if INX_HEATROD_IOT_BOARD_SUBVARIANT > INX_HR_HRi_DISPv2
	#define INX_FIXED_NEGATIVE_DISPLAY_HARDWARE
#else
	#undef INX_FIXED_NEGATIVE_DISPLAY_HARDWARE
#endif
	//#define SKIP_ETHERNET_SETUP
	#define CONFIG_TLK106 (1U) // note we will need to split this condition for different clock paths
	#define CONFIG_SMI_PHY_ADDR (30U)

	/* TODO - move all these pin configs to board specific hardware config header file */
	#define CONFIG_GPIO_PHY_RESET_BASE GPIOA
	#define CONFIG_GPIO_PHY_RESET 4U
	#define CONFIG_PHY_ENABLE_CLOCK_OUTPUT (0U)
	#define CONFIG_PHY_FORCE_POLARITY (0U)
	#define CONFIG_PHY_READ_MAX_ATTEMPTS (1000U)

/* Numerical Display 1*/
	#define CONFIG_GPIO_NUMERICAL_DISPLAY_PORT PORTC
	#define CONFIG_GPIO_NUMERICAL_DISPLAY_BASE GPIOC
	#define CONFIG_GPIO_NUMERICAL_DISPLAY_A (8U)
	#define CONFIG_GPIO_NUMERICAL_DISPLAY_B	(9U)
	#define CONFIG_GPIO_NUMERICAL_DISPLAY_C	(10U)
	#define CONFIG_GPIO_NUMERICAL_DISPLAY_D	(11U)
// PTC C :
	#define CONFIG_GPIO_NUMERICAL_DISPLAY_CHAR1	(14U)
	#define CONFIG_GPIO_NUMERICAL_DISPLAY_CHAR2	(15U)
	#define CONFIG_GPIO_NUMERICAL_DISPLAY_CHAR3	(16U)
	#define CONFIG_GPIO_NUMERICAL_DISPLAY_CHAR4	(17U)
	//#define CONFIG_GPIO_NUMERICAL_DISPLAY_DP1	(18U) - used patricks new ones for these
	//#define CONFIG_GPIO_NUMERICAL_DISPLAY_DP1	(3U)

/* LED GPIO */
#if INX_HEATROD_IOT_BOARD_VARIANT==INX_HR_HRc || INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRx_COMMISSIONING_BOARD
	#define CONFIG_GPIO_PROCBOARDLEDS_BASE GPIOE
	#define CONFIG_GPIO_PROCBOARDLEDS_PORT PORTE
	#define CONFIG_GPIO_PROCBOARDLED2_RED_PIN (2U)
	#define CONFIG_GPIO_PROCBOARDLED2_GRN_PIN (0U)
	#define CONFIG_GPIO_PROCBOARDLED1_RED_PIN (5U)
	#define CONFIG_GPIO_PROCBOARDLED1_GRN_PIN (4U)
#else //INX_HEATROD_IOT_BOARD_VARIANT==INX_HR_HRc || INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRx_COMMISSIONING_BOARD
	#if INX_HEATROD_IOT_BOARD_SUBVARIANT == INX_HR_HRi_DISPv1
		#define CONFIG_GPIO_PROCBOARDLEDS_BASE GPIOE
		#define CONFIG_GPIO_PROCBOARDLEDS_PORT PORTE
		#define CONFIG_GPIO_PROCBOARDLED1_RED_PIN (2U)
		#define CONFIG_GPIO_PROCBOARDLED1_GRN_PIN (0U)
		#define CONFIG_GPIO_PROCBOARDLED2_RED_PIN (5U)
		#define CONFIG_GPIO_PROCBOARDLED2_GRN_PIN (4U)
	#else
		#define CONFIG_GPIO_PROCBOARDLEDS_BASE GPIOE
		#define CONFIG_GPIO_PROCBOARDLEDS_PORT PORTE
		#define CONFIG_GPIO_PROCBOARDLED2_RED_PIN (2U)
		#define CONFIG_GPIO_PROCBOARDLED2_GRN_PIN (0U)
		#define CONFIG_GPIO_PROCBOARDLED1_RED_PIN (5U)
		#define CONFIG_GPIO_PROCBOARDLED1_GRN_PIN (4U)
	#endif //INX_HEATROD_IOT_BOARD_SUBVARIANT == INX_HR_HRi_DISPv1
#endif //INX_HEATROD_IOT_BOARD_VARIANT==INX_HR_HRc || INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRx_COMMISSIONING_BOARD

	#define CONFIG_GPIO_BUTTON_UP_BASE GPIOC
	#define CONFIG_GPIO_BUTTON_DOWN_BASE GPIOC

	#define CONFIG_GPIO_BUTTON_UP_PORT PORTC
	#define CONFIG_GPIO_BUTTON_DOWN_PORT PORTC
#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc || INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRx_COMMISSIONING_BOARD
#if  INX_HEATROD_IOT_BOARD_SUBVARIANT == INX_HR_HRi_DISPv2
	#define CONFIG_GPIO_BUTTON_SET_BASE GPIOC
	#define CONFIG_GPIO_BUTTON_SET_PORT PORTC
	#define CONFIG_GPIO_BUTTON_SET_PIN (18U) // alternative use of negative sign pin
	#define CONFIG_GPIO_BUTTON_MODE_BASE GPIOC
	#define CONFIG_GPIO_BUTTON_MODE_PORT PORTC
	#define CONFIG_GPIO_BUTTON_MODE_PIN (18U)
#else
	#define CONFIG_GPIO_BUTTON_MODE_BASE GPIOC
	#define CONFIG_GPIO_BUTTON_MODE_PORT PORTC
	#define CONFIG_GPIO_BUTTON_MODE_PIN (13U) // aka front panel spare gpio

	#define CONFIG_GPIO_BUTTON_SET_BASE GPIOE
	#define CONFIG_GPIO_BUTTON_SET_PORT PORTE
	#define CONFIG_GPIO_BUTTON_SET_PIN (25U)

#endif // #end not HRi_DISPv2

	#define CONFIG_GPIO_BUTTON_UP_PIN (4U)   // up and down buttons are reversed on HRc so we know when we have the wrong build variant (INtended -- honest)
	#define CONFIG_GPIO_BUTTON_DOWN_PIN (5U)

#else


#if  INX_HEATROD_IOT_BOARD_SUBVARIANT == INX_HR_HRi_DISPv1
/* use set and up/down buttons as connected when fitted to the HRD processor board */
#define CONFIG_GPIO_BUTTON_SET_PORT PORTC
	#define CONFIG_GPIO_BUTTON_SET_BASE GPIOC
	#define CONFIG_GPIO_BUTTON_SET_PIN (6U)
    #define CONFIG_GPIO_BUTTON_UP_PIN (5U)
	#define CONFIG_GPIO_BUTTON_DOWN_PIN (4U)
#else
/* Using the updown buttons via the display header */
/* Use negative config pin for V2 front panels with set button mounted remotely from processor */
	#define CONFIG_GPIO_BUTTON_SET_BASE GPIOC
	#define CONFIG_GPIO_BUTTON_SET_PORT PORTC
	#define CONFIG_GPIO_BUTTON_SET_PIN (18U) // presumably this is the negative sign pin
	#define CONFIG_GPIO_BUTTON_UP_PIN (4U)   // up and down buttons are reversed on HRc so we know when we have the wrong build variant (INtended -- honest)
	#define CONFIG_GPIO_BUTTON_DOWN_PIN (5U)

#endif

#endif

	#define CONFIG_GPIO_BUTTONS_INTERRUPT (0U)

	#define CONFIG_GPIO_BUTTON_UP_PORT PORTC
	#define CONFIG_GPIO_BUTTON_DOWN_PORT PORTC

	#define CONFIG_GPIO_RS485_ENABLE_DISABLE 2
#endif
// sub clauses for each HRx variant
#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRi || INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRx_COMMISSIONING_BOARD
#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRi
	#define INX_HEATROD_HEALTHCHECKTESTS (0) // TODO - we may want to enable this //indicates we want software IEC checks at start up (but will not run in diagnostic build mode)
	#define INX_HEATROD_STARTUP_CHECK (0) // TODO - we may want to enable this  //tests the relays on boot
#endif
	/* Control Outputs and Digitial inputs
	PTB3	Control Out 1 (J4.3)
	PTB9	Control Out 2 (J4.5)	(Control Out 1
	PTB10	Control Out 3 (J4.7)
	PTB11	Aux Control Out 1 (J4.4)
	PTB16	RS485 - R
	PTB17	RS485 - D
	PTB18	Aux Control Out 2 (J4.6)
	PTB19	Aux Control Out 3 (J14.8)
	PTB20	Thermal Cout check 1 (J4.13) -> 7 segment display 2 enable char 1
	PTB21	Thermal Cout check 2 (J4.15) -> 6  segment display 2 enable char 2
	PTB22	Thermal Cout check 3 (J4.17) -> 6  segment display 2 enable char 3 (note 3 and 4 I think aare swapped on all display board revisions).
	PTB23	External Cut Out Check 1 (J4.19) -> 6  segment display 2 enable char 4
	PTC1	Control Check 2 (J4.16)
	PTC2	Control Check 1 (J4.18)
	PTC0	Control Check 3 (J4.14)
	PTC3	External Cut Out Check 2 (J4.20)
	 */

	#define CONFIG_GPIO_CONTROL_BASE_OUT GPIOB
	#define CONFIG_GPIO_CONTROL_PORT_OUT PORTB
	#if EHS_PLATFORM_ESP32S3_HRDCV2 == 1 || EHS_PLATFORM_ESP32S3_HRDCV2 == 2
	#define CONFIG_GPIO_CONTROL_INTERNAL_RELAY_1 (34U)
	#define CONFIG_GPIO_CONTROL_INTERNAL_RELAY_2 (33U)
	#define CONFIG_GPIO_CONTROL_INTERNAL_RELAY_3 (47U)
	#define CONFIG_GPIO_INVERSE_INTERNAL_RELAY_1 (false)
	#define CONFIG_GPIO_INVERSE_INTERNAL_RELAY_2 (false)
	#define CONFIG_GPIO_INVERSE_INTERNAL_RELAY_3 (false)
	#define CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_1 (48U)
	#define CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_2 (35U)
	#define CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_3 (36U)
	#define CONFIG_GPIO_INVERSE_SSR_EXTERNAL_OUT_1 (true)
	#define CONFIG_GPIO_INVERSE_SSR_EXTERNAL_OUT_2 (true)
	#define CONFIG_GPIO_INVERSE_SSR_EXTERNAL_OUT_3 (true)
	#elif EHS_PLATFORM_ESP32S3_HRDCV2 == 3
	#define CONFIG_GPIO_CONTROL_INTERNAL_RELAY_1 (42U)
	#define CONFIG_GPIO_CONTROL_INTERNAL_RELAY_2 (41U)
	#define CONFIG_GPIO_CONTROL_INTERNAL_RELAY_3 (47U)
	#define CONFIG_GPIO_INVERSE_INTERNAL_RELAY_1 (false)
	#define CONFIG_GPIO_INVERSE_INTERNAL_RELAY_2 (false)
	#define CONFIG_GPIO_INVERSE_INTERNAL_RELAY_3 (false)
	#define CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_1 (48U)
	#define CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_2 (35U)
	#define CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_3 (36U)
	#define CONFIG_GPIO_INVERSE_SSR_EXTERNAL_OUT_1 (true)
	#define CONFIG_GPIO_INVERSE_SSR_EXTERNAL_OUT_2 (true)
	#define CONFIG_GPIO_INVERSE_SSR_EXTERNAL_OUT_3 (true)
	#else
		#warning "EHS_PLATFORM_ESP32S3_HRDCV2 is out of platform config range or not defined!"
	// It should be only for the stubbed variation
	#define CONFIG_GPIO_CONTROL_INTERNAL_RELAY_1 (0U)
	#define CONFIG_GPIO_CONTROL_INTERNAL_RELAY_2 (0U)
	#define CONFIG_GPIO_CONTROL_INTERNAL_RELAY_3 (0U)
	#define CONFIG_GPIO_INVERSE_INTERNAL_RELAY_1 (false)
	#define CONFIG_GPIO_INVERSE_INTERNAL_RELAY_2 (false)
	#define CONFIG_GPIO_INVERSE_INTERNAL_RELAY_3 (false)
	#define CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_1 (0U)
	#define CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_2 (0U)
	#define CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_3 (0U)
	#define CONFIG_GPIO_INVERSE_SSR_EXTERNAL_OUT_1 (true)
	#define CONFIG_GPIO_INVERSE_SSR_EXTERNAL_OUT_2 (true)
	#define CONFIG_GPIO_INVERSE_SSR_EXTERNAL_OUT_3 (true)
	#endif
	//// TODO - specify external ports
	// #define CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_1 (11U)
	// #define CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_2 (18U)
	// #define CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_3 (19U)

	/*  Main board LEDs _from pin mapping in googledocs
		PTE0 System Stat LED1 (Red) - Hardware Fault (on Processor board ?)
		(PTE1 USB Power sense  GPIO)
		PTE2 System Stat LED1 (Green) -  System OK (on Processor board ?)
		PTE3 System Stat LED1 (blue-NOTUSED) - System Alarm (on Processor board ?)
		PTE4 Network Stat LED (Red) -  not connected (on Processor board ?)
		PTE5 Network Stat LED (Green) - connected OK (on Processor board ?)
		PTE6 Network Stat LED (Blue-NOT_USED) -  connected -ReadOnly / (flashing) Devman access error  (on Processor board ?)
	*/

	//#define CONFIG_GPIO_NUMERICAL_DISPLAY_CHAR
#endif


/* These are Relay control that are HRDi specific and are tested by the commissioning board  */
#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRi || INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRx_COMMISSIONING_BOARD // commissioning board shares the
	#define CONFIG_GPIO_SENSE_CO_BASE GPIOB
	#define CONFIG_GPIO_SENSE_CO_PORT PORTB
	#define CONFIG_GPIO_SENSE_THERM_CO_1 (20U)
	#define CONFIG_GPIO_SENSE_THERM_CO_2 (21U)
	#define CONFIG_GPIO_SENSE_THERM_CO_3 (22U)
	#define CONFIG_GPIO_SENSE_EXT_CO_1 (23U)

	#define CONFIG_GPIO_SENSE_CONRELAY_PORT PORTC
	#define CONFIG_GPIO_SENSE_CONRELAY_BASE	GPIOC

#if (INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRx_COMMISSIONING_BOARD) // the pins are swapped to what you might expect on HRDi
	#define CONFIG_NUM_DISPLAYS 2
	#define CONFIG_GPIO_SENSE_CONRELAY_1 (0U)
	#define CONFIG_GPIO_SENSE_CONRELAY_2 (1U)
	#define CONFIG_GPIO_SENSE_CONRELAY_3 (2U)
#else
	#define CONFIG_NUM_DISPLAYS 1
	#define CONFIG_GPIO_SENSE_CONRELAY_1 (2U)
	#define CONFIG_GPIO_SENSE_CONRELAY_2 (1U)
	#define CONFIG_GPIO_SENSE_CONRELAY_3 (0U)
	//PBB 20210607 we no longer support negative pin on HRI boards
	#if INX_HEATROD_IOT_BOARD_SUBVARIANT==INX_HR_HRi_DISPv1 || INX_HEATROD_IOT_BOARD_SUBVARIANT==INX_HR_HRi_DISPv2
		#undef CONFIG_GPIO_NEGATIVE_DISPLAY1_PIN_PORT
		#undef CONFIG_GPIO_NEGATIVE_DISPLAY1_PIN_BASE
		#undef CONFIG_GPIO_NEGATIVE_DISPLAY1_PIN
	#else //if INX_HEATROD_IOT_BOARD_SUBVARIANT==INX_HR_HRi_DISPv1 || INX_HEATROD_IOT_BOARD_SUBVARIANT==INX_HR_HRi_DISPv2
		#define CONFIG_GPIO_NEGATIVE_DISPLAY1_PIN_PORT PORTC
		#define CONFIG_GPIO_NEGATIVE_DISPLAY1_PIN_BASE GPIOC
		#define CONFIG_GPIO_NEGATIVE_DISPLAY1_PIN 18
	#endif //else if INX_HEATROD_IOT_BOARD_SUBVARIANT==INX_HR_HRi_DISPv1 || INX_HEATROD_IOT_BOARD_SUBVARIANT==INX_HR_HRi_DISPv2
	
#endif

#define CONFIG_DUTY_CYCLES_AT_FULL_POWER_BEFORE_FORCING_RELAY_OFF_TO_CHECK_TCO (20U)

#endif

/* Numerical Display 2*/
// PTC B :
	#define CONFIG_GPIO_NUMERICAL_DISPLAY2_PORT PORTB
	#define CONFIG_GPIO_NUMERICAL_DISPLAY2_BASE GPIOB
	#define CONFIG_GPIO_NUMERICAL_DISPLAY2_CHAR1	(20U)
	#define CONFIG_GPIO_NUMERICAL_DISPLAY2_CHAR2	(21U)
	#define CONFIG_GPIO_NUMERICAL_DISPLAY2_CHAR3	(23U) // note this is a fix for a hardwre problem on the HRc display
	#define CONFIG_GPIO_NUMERICAL_DISPLAY2_CHAR4	(22U) // note this is a fix for a hardwre problem on the HRc display

//these two should match
	#define CONFIG_GPIO_NEGATIVE_DISPLAY2_PIN_PORT PORTC
	#define CONFIG_GPIO_NEGATIVE_DISPLAY2_PIN_BASE GPIOC
	#define CONFIG_GPIO_NEGATIVE_DISPLAY2_PIN 3
#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
	#define CONFIG_ALLOW_SSR_TIMINGS_ON_INTERNAL (0)
	#define INX_HEATROD_HEALTHCHECKTESTS (1U) //indicates we want IEC checks


	/* Warning duplicating these from HRi in case they change */
	#define CONFIG_GPIO_CONTROL_BASE_OUT GPIOB
	#define CONFIG_GPIO_CONTROL_PORT_OUT PORTB
	#define CONFIG_GPIO_CONTROL_INTERNAL_RELAY_1 (3U)
	#define CONFIG_GPIO_CONTROL_INTERNAL_RELAY_2 (9U)
	#define CONFIG_GPIO_CONTROL_INTERNAL_RELAY_3 (10U)
	#define CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_1 (11U)
	#define CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_2 (18U)
	#define CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_3 (19U)
#endif

#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
	#define CONFIG_NUM_DISPLAYS 2

  //these two should match
	#define CONFIG_GPIO_NEGATIVE_DISPLAY1_PIN_PORT PORTC
	#define CONFIG_GPIO_NEGATIVE_DISPLAY1_PIN_BASE GPIOC
	#define CONFIG_GPIO_NEGATIVE_DISPLAY1_PIN 18
#elif INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRx_COMMISSIONING_BOARD
	//these two should match
	#define CONFIG_GPIO_NEGATIVE_DISPLAY1_PIN_PORT PORTC
	#define CONFIG_GPIO_NEGATIVE_DISPLAY1_PIN_BASE GPIOC
	#define CONFIG_GPIO_NEGATIVE_DISPLAY1_PIN 18
#endif


/* PRODUCTION TEST BOARD GPIO */

#if CONFIG_LIFESTYLE == INX_LIFESTYLE_COMMISSION || CONFIG_LIFESTYLE == INX_LIFESTYLE_DISPLAY_HARDWARE_TESTER

	#define CONFIG_GPIO_DIAGNOSTIC_PROD_HIGH_IN_PIN_PORT PORTC
	#define CONFIG_GPIO_DIAGNOSTIC_PROD_HIGH_IN_PIN_BASE GPIOC
	#define CONFIG_GPIO_DIAGNOSTIC_PROD_HIGH_IN_PIN 8
/* We use the DISP 2 Minus out test test BUTTONS - TBC if we use the set negative putton or dupe it in GPIO as in the next*/
/* We need to make the config for DIAGNOSTIC BUILD to initialise the PGIO, which currently means build an HRDc variant.
 * we probably want another board variant which is the production test board vairant that does the config especially for this board soi we still have
 * diagnostic interactive builds for HRDi and HRDc switcher/display configs.*/
//	#define CONFIG_GPIO_NEGATIVE_DISPLAY2_PIN_PORT PORTC
//	#define CONFIG_GPIO_NEGATIVE_DISPLAY2_PIN_BASE GPIOC
//	#define CONFIG_GPIO_NEGATIVE_DISPLAY2_PIN 3


#endif

/* Round HRD processor board with VBAT patches and phy address bootstrap resistors for addr 0x31 */
#if  INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRD
	#define CONFIG_DISPLAY_NUM_CHARS 4
	#define CONFIG_STACK_SIZE 0x800
	#define CONFIG_FLEX_RAM_START_ADDRESS 0x14000000
	#define CONFIG_FLAX_RAM_SIZE 0x1000
	#define CONFIG_SRAM_UPPER_START_ADDRESS 0x20000000
	#define CONFIG_SRAM_UPPER_SIZE 0x30000
	#define CONFIG_MAX_PROGRAM_SIZE_SECTORS 125
	#define CONFIG_PHY_FORCE_POLARITY (1U)
	#define CONFIG_PHY_ENABLE_CLOCK_OUTPUT (1U)
	#define CONFIG_GPIO_PHY_RESET_BASE GPIOC
	#define CONFIG_GPIO_PHY_RESET 12U
	#define CONFIG_ENABLE_RTC (0)
	#define CONFIG_TLK106 (1U)
	#define INX_HEATROD_HEALTHCHECKTESTS (1U)

/* DIO Pin assignments */
	#define CONFIG_GPIO_CONTROL_INTERNAL_RELAY_1 (9U)
	#define CONFIG_GPIO_CONTROL_INTERNAL_RELAY_2 (23U)
	#define CONFIG_GPIO_CONTROL_INTERNAL_RELAY_3 (23U)
	#define CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_1 (9U)
	#define CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_2 (23U)
	#define CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_3 (23U)
	#define CONFIG_GPIO_RS485_ENABLE_DISABLE 2
#endif

#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_FRDMK64EVAL
	#define CONFIG_NUM_DISPLAYS 0
	#define CONFIG_DISPLAY_NUM_CHARS 0
	#define CONFIG_STACK_SIZE 0x800
	#define CONFIG_FLEX_RAM_START_ADDRESS 0x14000000
	#define CONFIG_FLAX_RAM_SIZE 0x1000
	#define CONFIG_SRAM_UPPER_START_ADDRESS 0x20000000
	#define CONFIG_SRAM_UPPER_SIZE 0x30000
	#define CONFIG_MAX_PROGRAM_SIZE_SECTORS 125
	#define CONFIG_ENABLE_RTC (1)
	#define CONFIG_MODBUS_ENABLED (0U)
	#define SKIP_ETHERNET_SETUP
	#define CONFIG_TLK106 (0U) // note we will need to split this condition for different clock paths
	#define INX_HEATROD_HEALTHCHECKTESTS (1U)
	#define CONFIG_DISABLE_PHY (0U)
	#define CONFIG_SMI_PHY_ADDR (0U)
	#define CONFIG_GPIO_PHY_RESET_BASE GPIOA
	#define CONFIG_GPIO_PHY_RESET 4U
	#define CONFIG_PHY_ENABLE_CLOCK_OUTPUT (0U)
	#define CONFIG_PHY_FORCE_POLARITY (0U)
	#define CONFIG_GPIO_LED1_RED_BASE GPIOE
	#define CONFIG_GPIO_LED1_RED_PIN 0U

	#define CONFIG_RELAY_ON_TEMPERATURE_ADJUSTMENT (0.05)
	#define CONFIG_GPIO_PROCBOARDLEDS_BASE GPIOE
	#define CONFIG_GPIO_PROCBOARDLED1_RED_PIN (2U)
	#define CONFIG_GPIO_PROCBOARDLED1_GRN_PIN (0U)
	#define CONFIG_GPIO_PROCBOARDLED2_RED_PIN (5U)
	#define CONFIG_GPIO_PROCBOARDLED2_GRN_PIN (4U)

	#define CONFIG_GPIO_CONTROL_BASE_OUT GPIOB
	#define CONFIG_GPIO_CONTROL_PORT_OUT PORTB
	#define CONFIG_GPIO_CONTROL_INTERNAL_RELAY_1 (9U)
	#define CONFIG_GPIO_CONTROL_INTERNAL_RELAY_2 (23U)
	#define CONFIG_GPIO_CONTROL_INTERNAL_RELAY_3 (23U)
	#define CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_1 (23U)
	#define CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_2 (23U)
	#define CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_3 (23U)
	#define CONFIG_GPIO_RS485_ENABLE_DISABLE 2

	#define CONFIG_GPIO_NUMERICAL_DISPLAY_CHAR1	(0U)
	#define CONFIG_GPIO_NUMERICAL_DISPLAY_CHAR2	(0U)
	#define CONFIG_GPIO_NUMERICAL_DISPLAY_CHAR3	(0U)
	#define CONFIG_GPIO_NUMERICAL_DISPLAY_CHAR4	(0U)

#endif


/*
 * CONFIG COMBINATION RULE ENFORCEMENT
 * */
#if defined(INX_HEATROD_IOT_DIAGNOSTIC_BUILD) || CONFIG_LIFESTYLE == INX_LIFESTYLE_COMMISSION || CONFIG_LIFESTYLE == INX_LIFESTYLE_DISPLAY_HARDWARE_TESTER
#if INX_HEATROD_HEALTHCHECKTESTS == 1
//#warning "HEALTHCHECK TESTS HAVE BEEN DISABLED IN DIAGNOSTIC MODE!"
#undef INX_HEATROD_HEALTHCHECKTESTS
#define INX_HEATROD_HEALTHCHECKTESTS  0
#endif
#endif

#if CONFIG_UART_DEBUG
	#define HW_DIAG_UART_printf(fmt,...) printf(fmt, ##__VA_ARGS__ )
#else
	#define HW_DIAG_UART_printf(fmt,...) {}
#endif //if CONFIG_UART_DEBUG

/*
 * Configure Diagnostic Mode & Debug verbosity and modules
 * */
// todo move the following into a debug utility header
#ifdef INX_HEATROD_IOT_DIAGNOSTIC_BUILD
	#define INX_HW_DIAG_ADC_TESTS_TODO (2000)
	#define INX_PRINTF_OTA(fmt,...) printf(fmt, ##__VA_ARGS__ )

	#define HW_DIAG_printf(fmt,...) printf(fmt, ##__VA_ARGS__)
	//#define HW_DIAG_printf(fmt,...) printf(fmt, ##__VA_ARGS__)

#elif CONFIG_LIFESTYLE == INX_LIFESTYLE_COMMISSION || CONFIG_LIFESTYLE == INX_LIFESTYLE_DISPLAY_HARDWARE_TESTER
	#define INX_HW_DIAG_ADC_TESTS_TODO (10)
	#define HW_DIAG_printf(fmt,...) printf(fmt, ##__VA_ARGS__ )
#elif CONFIG_LIFESTYLE == INX_LIFESTYLE_DEBUG
	#define HW_DIAG_printf(fmt,...) printf(fmt, ##__VA_ARGS__ )
#else // possibly a production build
//#define HW_DIAG_printf(fmt,...) printf(fmt, ##__VA_ARGS__ )
#define HW_DIAG_printf(fmt,...) {}
#endif

#if CONFIG_OTA_DIAG
	#define INX_PRINTF_OTA(fmt,...) printf(fmt, ##__VA_ARGS__ )
#else
#ifdef INX_PRINTF_OTA
#undef INX_PRINTF_OTA
#endif
	#define INX_PRINTF_OTA(fmt,...) {}
#endif //CONFIG_OTA_DIAG

#if CONFIG_SCHEDULE_DIAG
	#define INX_PRINTF_SCHEDULE(fmt,...) printf(fmt, ##__VA_ARGS__ )
#else
	#define INX_PRINTF_SCHEDULE(fmt,...) {}
#endif //CONFIG_SCHEDULE_DIAG

#if CONFIG_LIFESTYLE == INX_LIFESTYLE_DEBUG || CONFIG_LIFESTYLE == INX_LIFESTYLE_HEATROD_DEBUG
#define SW_DIAG_printf(fmt,...) printf(fmt, ##__VA_ARGS__ )
//#define SW_DIAG_printf(fmt,...) {}
#else
#define SW_DIAG_printf(fmt,...) {}
#endif

//todo - we probably want to unravel all the following from the many types of indirections of debug loggers and instead direct to a single implementation and Logging code format (e.g. see HW_DIAG_printf)
/* In order to get console output when using jlink you need to do "quick settings -> SDK debug console -> Semihost console" then do a rebuild
 */
#ifndef MQTT_APP_DEBUG
	#define MQTT_APP_DEBUG              LWIP_DBG_ON /*LWIP_DBG_OFF*/
#endif
#ifndef MQTT_APP_DEBUG_TRACE
	#define MQTT_APP_DEBUG_TRACE        (MQTT_APP_DEBUG | LWIP_DBG_TRACE)
#endif

/*
 * More weird Debugstuff
 */
#define DEBUG_NETWORK LWIP_DBG_OFF
#define DEBUG_SUPERVISOR LWIP_DBG_OFF
#define DEBUG_ADC_CONVERT_VALUES (0)
#define DEBUG_HTTP_SRV LWIP_DBG_OFF
#define DEBUG_MODBUS LWIP_DBG_ON
#define DEBUG_LED LWIP_DBG_OFF
#define DEBUG_BUTTONS LWIP_DBG_OFF

/* Modified to never work unless in comissioning mode */
#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRx_COMMISSIONING_BOARD
#define INX_BOOT_DIAG_PRINT(message) do {\
                               if (gDiagnosticBoot==true){\
                                 printf message;\
                               }\
                             } while(0)
#else
#define INX_BOOT_DIAG_PRINT(message) do {\
                               if (gDiagnosticBoot==true){\
                             } while(0)
#endif

// todo none of the following is actually config - it should be in proper code headers
// !!! - make sure "ERROR_STATE_MAP" is updated when this enum is changed
typedef enum {
	ERROR_LOGS_INTERNAL_RELAY_STUCK_ON,
	ERROR_LOGS_INTERNAL_RELAY_STUCK_OFF,
	ERROR_LOGS_THERMAL_CUT_OUT,
	ERROR_LOGS_ADC_DISABLED,
	ERROR_LOGS_ADC_VALUES_UNSAFE,
	ERROR_LOGS_WATCH_DOG_RESETS,
	ERROR_LOGS_RAM_BOUNDARIES,
	ERROR_LOGS_RAM_WALK,
	ERROR_LOGS_INTERRUPTS,
	ERROR_LOGS_GPIO,
	ERROR_LOGS_FORCED_FACTORY_RESET,
	ERROR_LOGS_RTC_DISABLED,
	ERROR_LOGS_PROGRAM_FLASH_CRC,
	ERROR_LOGS_RTC_STUCK_IN_SCHEDULE_MODE,
	ERROR_LOGS_RTC_DRIFTED_IN_SCHEDULE_MODE,
	ERROR_LOGS_RTC_DISABLED_IN_SCHEDULE_MODE,
	ERROR_LOGS_DISPLAY_BOUNDARY,
	ERROR_LOGS_USER_ALARM_TEMPERATURE,
	ERROR_LOGS_RTC_STUCK,
	ERROR_LOGS_RTC_RECOVERED,
	ERROR_LOGS_END
} errorLogsError_t;

typedef enum {
	INX_DUTY_CYCLE_INTERNAL_1_INDEX=0,
	INX_DUTY_CYCLE_INTERNAL_2_INDEX=1,
	INX_DUTY_CYCLE_INTERNAL_3_INDEX=2,
	INX_DUTY_CYCLE_EXTERNAL_1_INDEX=3,
	INX_DUTY_CYCLE_EXTERNAL_2_INDEX=4,
	INX_DUTY_CYCLE_EXTERNAL_3_INDEX=5,
	INX_DUTY_CYCLE_MAX_INDEX=6
} inxDutyCycleIndex_t;

typedef struct {
	uint32_t unixTime;
} inxError_t;

typedef struct _error_logs{
	bool locked;
	inxError_t errors[ERROR_LOGS_END];
} errorLogs_t;

typedef enum {
	INX_HTTP_ERROR_OK,
	INX_HTTP_ERROR
} inxHTTPError_t;

typedef enum {
	INX_HTTP_SESSION_STATE_NONE,
	INX_HTTP_SESSION_STATE_ACTIVE
} inxHTTPSessionState_t;

typedef enum {
	INX_TIMER_STATE_READY,
	INX_TIMER_STATE_START,
	INX_TIMER_STATE_RUNNING,
	INX_TIMER_STATE_FINISHED,
	INX_TIMER_STATE_GOING_TO_SET_POINT
} inxTimerState_t;

typedef enum {
	INX_SAFETY_MODE_EN60335,
	INX_SAFETY_MODE_ADDITIONAL_CHECKS,
	INX_SAFETY_MODE_DISABLE_SLOW_START_CHECKS,
	INX_SAFETY_MODE_DISABLE_ALL_HARDWARE_CHECKS,
	INX_SAFETY_MODE_DISABLE_RELAY_CHECKS
} inxSafetyMode_t;

typedef enum  {
	ADC_STATUS_OK,
	ADC_STATUS_OUTLIERS,
	ADC_STATUS_CONNECTION_ERROR
} adcStatusType ;

typedef enum {
	INX_CONTROLLER_OUTPUT_MODE_DISABLED,
	INX_CONTROLLER_OUTPUT_MODE_MODBUS_ONLY,
	INX_CONTROLLER_OUTPUT_MODE_INTERNAL_1,
	INX_CONTROLLER_OUTPUT_MODE_INTERNAL_2,
	INX_CONTROLLER_OUTPUT_MODE_INTERNAL_3,
	INX_CONTROLLER_OUTPUT_MODE_EXTERNAL_1,
	INX_CONTROLLER_OUTPUT_MODE_EXTERNAL_2,
	INX_CONTROLLER_OUTPUT_MODE_EXTERNAL_3,
	INX_CONTROLLER_OUTPUT_MODE_END
} inxControllerOutputMode_t;

typedef enum {
	INX_CONTROLLER_CONTACTOR_MODE_EMR,
	INX_CONTROLLER_CONTACTOR_MODE_SSR
} inxRelayType_t;

typedef struct _adcRawConnected {
	volatile bool bPt100Channel1;
	volatile bool bPt100Channel2;
	volatile bool bZeroTenV;
	volatile bool bFourTwentyMa;
	volatile bool bThermocouple1;
	volatile bool bThermocouple2;
} adcRawConnected_t;

typedef struct _adcRawValues{
	volatile uint32_t pt100Channel1;
	volatile uint32_t pt100Channel2;
	volatile uint32_t cpuTemp1;
	volatile uint32_t zeroTenV;
	volatile uint32_t fourTwentyMa;
	volatile uint32_t thermocouple1;
	volatile uint32_t thermocouple2;
	adcRawConnected_t connected;
} adcRawValues_t;

typedef struct _adcConvertedValues{
	int32_t pid1;
	int32_t pid2;
	int32_t pid3;
	int32_t cpuTemp1;
} adcConvertedValues_t;

typedef struct _globalADCValues{
	rtc_datetime_t finishDate;
	adcConvertedValues_t current;
	adcConvertedValues_t values[INX_SAMPLEBUF_SIZE];
} globalADCValues_t;

typedef struct _dutyCycleHistory
{
	uint32_t powerHistoryHead;
	rtc_datetime_t historyFinishDate;
} dutyCycleHistory_t;

typedef struct _threadSafeDutyCycleHistory
{
	dutyCycleHistory_t state;
	bool locked;
	bool ready;
} threadSafeDutyCycleHistory_t;

typedef struct {
	uint8_t element1Control;
	uint8_t element1Monitor;
	uint8_t element2Control;
	uint8_t element2Monitor;
	uint8_t element3Control;
	uint8_t element3Monitor;
	uint8_t thermalCO1Control;
	uint8_t thermalCO1Monitor;
	uint32_t time;
} inxRelayCurrentState_t;

// Stepped mode has been disabled for HRDVc2
typedef enum {
	ELEMENT_MODE_SINGLE = 0,
	ELEMENT_MODE_DOUBLE_TANDEM,
	ELEMENT_MODE_TRIPLE_TANDEM,
	ELEMENT_MODE_SINGLE_TWO_POLE
} ElementMode_t;

typedef struct {
	int32_t kp;
	int32_t ki;
	int32_t kd;
} inxPIDCoefficients_t;

typedef enum {
	SCHEDULE_MODE_DISABLED,
	SCHEDULE_MODE_MANUAL,
	SCHEDULE_MODE_ENABLED,
	SCHEDULE_MODE_TIMER,
	SCHEDULE_MODE_TIMER_SET_POINT,
	SCHEDULE_MODE_CALIBRATION
} Schedule_Mode_t;

/* these are the logical ADC channels (Not NXP ADC Channels) */
typedef enum {
	INX_CONTROLLER_SENSORS_NONE,
	INX_CONTROLLER_SENSORS_PT100_CHANNEL1,
	INX_CONTROLLER_SENSORS_PT100_CHANNEL2,
	INX_CONTROLLER_SENSORS_10V,
	INX_CONTROLLER_SENSORS_20MA,
	INX_CONTROLLER_SENSORS_MODBUS,
	INX_CONTROLLER_SENSORS_THERMOCOUPLE_CHANNEL1,
	INX_CONTROLLER_SENSORS_THERMOCOUPLE_CHANNEL2
} inxControllerSensors_t;

typedef enum {
	INX_CONTROLLER_TC_TYPE_NONE = 0,
	INX_CONTROLLER_TC_TYPE_J,
	INX_CONTROLLER_TC_TYPE_K,
	INX_CONTROLLER_TC_TYPE_N,
	INX_CONTROLLER_TC_TYPE_T,
	INX_CONTROLLER_TC_TYPE_R,
	INX_CONTROLLER_TC_TYPE_S,
	INX_CONTROLLER_TC_TYPE_NOTDEFINED
} inxThermoCoupleType_t;

typedef struct {
	int32_t p;
	int32_t i;
	int32_t d;
	inxPIDCoefficients_t coefficients;
	uint32_t powerPercent;
	int32_t desiredTemp;
	ElementMode_t elementMode;
	inxControllerSensors_t sensors;
	Schedule_Mode_t scheduleMode;
	//float manualSetPoint;
	int32_t currentValue;
	//float upperAlarmTemp;
	//float lowerAlarmTemp;
	//uint32_t timerDurationSeconds;
	//uint32_t timerSecondsAtFinish;
	//uint32_t timerSecondsRemaining;
	//inxTimerState_t timerState;
	//uint32_t rampSecondsPerDegree;
	//uint32_t rampSecondsAtNextIncrement;
	// float rampDesiredTemp;
	bool scheduleOutputEnabled;
	inxControllerOutputMode_t controllerOutputMode[CONFIG_PID_OUTPUTS_NUM];
	int32_t nPIDUpperLimit;
	int32_t nIMax;
	//float timerSetPoint;
	bool calibrate;
} inxPID_t;

typedef struct {
	inxThermoCoupleType_t tc_type[2];
} inxSensorType_t;

typedef enum {
	INX_TIME_STATE_SUBSCRIBING,
	INX_TIME_STATE_WAITING_FOR_TIME,
	INX_TIME_STATE_UNSUBSCRIBING,
	INX_TIME_STATE_WAITING_FOR_DRIFT
} INX_Time_State_Enum_t;

typedef enum {
	INX_TIME_STATE_RTC_DRIFTED,
	INX_TIME_STATE_RTC_OK,
	INX_TIME_STATE_RTC_UNSET,
	INX_TIME_STATE_RTC_BUSY
} INX_TIME_RTC_State_Enum_t;

typedef struct _INX_Time_State_t {
	INX_Time_State_Enum_t state;
	uint32_t waitTicks;
	INX_TIME_RTC_State_Enum_t rtcState;
	uint32_t rtcWaitTicks;
	INX_TIME_RTC_State_Enum_t stateBeforeBusy;
} INX_Time_State_t;

typedef enum {
	INX_MODBUS_STATE_INIT,
	INX_MODBUS_STATE_STOP,
	NX_MODBUS_STATE_ERROR,
	INX_MODBUS_STATE_RUN
} inxModbusState_t;

typedef enum {
	INX_DUTY_ELEMENT_ONE,
	INX_DUTY_ELEMENT_TWO,
	INX_DUTY_ELEMENT_THREE,
	INX_DUTY_ELEMENT_ALL
} inxDutyElement_t;

typedef enum {
	INX_RTC_PART_YEAR,
	INX_RTC_PART_MONTH,
	INX_RTC_PART_DAY,
	INX_RTC_PART_HOUR,
	INX_RTC_PART_MINUTE,
	INX_RTC_PART_SECOND
} inxRTCPartEnum_t;

typedef enum {
	RESET_MODE_NONE = 0,
	RESET_MODE_RESTART_DEVICE,
	RESET_MODE_CLEAR_ERRORS_AND_RESET,
	RESET_MODE_FACTORY_RESET
} ResetMode_t;

typedef enum {
	INX_POWER_MODE_THREE_PHASE_415, //delta
	INX_POWER_MODE_THREE_PHASE_240,
	INX_POWER_MODE_SINGLE_PHASE_240,
	INX_POWER_MODE_INDEPENDENT_CONNECTION
} inxPowerModeEnum_t;

typedef enum {
	INX_PID_CONDITION_INIT,
	INX_PID_CONDITION_OK,
	INX_PID_CONDITION_BAD,
	INX_PID_CONDITION_DISABLED
} pidConditionStateMode_t;

typedef struct _pidConditionStateDiff_t {
	adcConvertedValues_t diff;
	adcConvertedValues_t earliestValues;
} pidConditionStateDiff_t;

typedef struct _inxNoFactoryResetStorage {
	uint8_t versionNumber;
	float pt1001Offset;
	float pt1001Scale;
	float pt1002Offset;
	float pt1002Scale;
	float thermoCoupleOffset;
	float thermoCoupleScale;
	float zeroTenVOffset;
	float zeroTenVScale;
	float fourTwentyMAOffset;
	float fourTwentyMAScale;
	uint32_t internalRelay1OnCount;
	uint32_t internalRelay2OnCount;
	uint32_t internalRelay3OnCount;
	uint32_t externalRelay1OnCount;
	uint32_t externalRelay2OnCount;
	uint32_t externalRelay3OnCount;
	uint32_t writeId;
} inxNoFactoryResetStorage_t;

//storage written to flash is always done in 1024 byte blocks
#define INX_SETTINGS_BYTES_RESERVED_FOR_NON_RESET_STORAGE ((sizeof(inxNoFactoryResetStorage_t) / 512)+1)*512
#define BUILD_BUG_ON(condition) ((void)sizeof(char[1 - 2*!!(condition)]))

// =========================================================

typedef struct _dutyCycleState
{
	bool conditionBad;
	uint32_t cycleInterruptLength;
	void* ftmBase;
	bool ftmEnabled;
	//ftm_chnl_t ftmChannel;
	uint32_t numInterrupts;
	volatile bool on;
	bool inverse_logic;
	uint32_t powerOffInterruptCount;
	uint32_t powerPercent;
	uint32_t gpioPin;
	uint32_t ticksSinceChange;
	uint32_t onCount;
	inxRelayType_t relayType;
	float cycleLengthSeconds;
} dutyCycleState_t;

// =========================================================

typedef enum  {
	SENSOR_ID_PT100 = 1,
	SENSOR_ID_PT200,
	SENSOR_ID_10V,
	SENSOR_ID_20MA,
	SENSOR_ID_THERMOCOUPLE1,
	SENSOR_ID_THERMOCOUPLE2,
	SENSOR_ID_COUNT
} inxAdcSensorId_t ;

typedef struct _inxCalibration {
	int32_t pt100Offset;
	int32_t pt100Scale;
	int32_t pt200Offset;
	int32_t pt200Scale;
	int32_t zeroTenVOffset;
	int32_t zeroTenVScale;
	int32_t fourTwentyMAOffset;
	int32_t fourTwentyMAScale;
	int32_t thermocoupleOneOffset;
	int32_t thermocoupleOneScale;
	int32_t thermocoupleTwoOffset;
	int32_t thermocoupleTwoScale;
} inxCalibration_t;

typedef enum {
	INX_HDWR_CONF_UNKNOWN = 0,
	INX_HDWR_CONF_SENSOR_TYPE_TC_1,
	INX_HDWR_CONF_SENSOR_TYPE_TC_2,
	/* add next config */
	INX_HDWR_CONF_COUNT
} inxHardwareConfigType_t;

/* ert specific functions */

EHS_GLOBAL ehs_pid_ctrl_error_id inx_pid_ctrl_init();

EHS_GLOBAL ehs_pid_ctrl_error_id inx_pid_ctrl_channel_init(const ehs_pid_ctrl_type* pid_ctrl);

EHS_GLOBAL ehs_pid_ctrl_error_id inx_pid_ctrl_gpio_init();
EHS_GLOBAL ehs_pid_ctrl_error_id inx_pid_ctrl_gpio_destroy();

// setters
EHS_GLOBAL ehs_pid_ctrl_error_id inx_set_setpoint_value(const ehs_pid_ctrl_type* pid_ctrl);
EHS_GLOBAL ehs_pid_ctrl_error_id inx_set_pid_config(const ehs_pid_ctrl_type* pid_ctrl);
EHS_GLOBAL ehs_pid_ctrl_error_id inx_set_io_config(const ehs_pid_ctrl_type* pid_ctrl);

EHS_GLOBAL void inx_set_calibration_offset(ehs_sint32 id, float offset);
EHS_GLOBAL void inx_set_calibration_scale(ehs_sint32 id, float scale);

// getters
EHS_GLOBAL ehs_bool inx_get_pid_output_index_for_pin(const ehs_sint32 pin, ehs_sint32* pid_index, ehs_sint32* pid_out_index);
EHS_GLOBAL ehs_float inx_get_adc_converted_value(const ehs_pid_ctrl_type* pid_ctrl);

// run pid controller
EHS_GLOBAL void inx_run_adc_pid(adcRawValues_t* adc_raw);

#endif /* HRX_PID_CONFIG_INX_H_ */
