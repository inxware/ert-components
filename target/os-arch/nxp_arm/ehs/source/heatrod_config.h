/*
 * config.h
 *
 *  Created on: 18.04.2017
 *      Author: Erich Styger
 */

#ifndef CONFIG_H_
#define CONFIG_H_
#include "ip4_addr.h"
#include "lwip/err.h" /* must be included first, as it has as well the ERR_OK enumeration, which conflicts with the Processor Expert ERR_OK define */
#include "fsl_rtc.h"
#include "fsl_adc16.h"

/*
 * Board variants available to build for
 * */
#define INX_HR_HRD (1) // obsolete round HRd board
#define INX_HR_HRi (2) // Current HRi (Integrated HeatRod Device)
#define INX_HR_HRc (3) // Current HRc (DIN panel mounted Heatrod Controller)
#define INX_HR_FRDMK64EVAL (4)
//#define INX_HACK_NHc

/* ************************************************* */
/* Change this to the required build!                */
#ifndef INX_HEATROD_IOT_BOARD_VARIANT
	#error You must set INX_HEATROD_IOT_BOARD_VARIANT in your platform config
#endif
/* ************************************************* */

#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRi
	#define CONFIG_MODEL "HR-I3-02-00"
#elif INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
	#define CONFIG_MODEL "HR-C3-02-00"
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
#define INX_LIFESTYLE_HEATROD_PRODUCTION (1000)
#define INX_LIFESTYLE_EHS (2000)

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
	#define CONFIG_SCHEDULE_DIAG (1)
	#define DEBUG_WS (0)
#elif CONFIG_LIFESTYLE == INX_LIFESTYLE_EHS
	#define INX_HEATROD_EHS (1)
	#define INX_HEATROD_HEALTHCHECKTESTS (0)
#elif CONFIG_LIFESTYLE == INX_LIFESTYLE_HARDWARE_DEBUG
	#define INX_HEATROD_IOT_DIAGNOSTIC_BUILD
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

#else //CONFIG_LIFESTYLE == INX_DEBUG
#warning "You should set a build lifestyle - use CONFIG_LIFESTYLE == INX_LIFESTYLE_HEATROD_PRODUCTION for production builds"
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
#define INX_FLASH_VALIDATION_CODE (0xdeadbee6) //must be updated when the permStorage_t is changed

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
	#define CONFIG_DO_FLASH_TEST (0) //enables the CRC check for the program flash, IEC
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

#ifndef CONFIG_PHY_MAX_AUTONEG_TICKS
	#define CONFIG_PHY_MAX_AUTONEG_TICKS (10U) //how long we wait for ethernet auto negotiation
#endif

#define CONFIG_WAIT_FOR_PHY_TICKS (CONFIG_PHY_MAX_AUTONEG_TICKS-1) //needs to be one less than the autoneg ticks so that our state machine works properly

#ifndef CONFIG_CLOCK_EXTAL0_ETHERNET
	#define CONFIG_CLOCK_EXTAL0_ETHERNET (1) //this needs to be on or ethernet doesn't work
#endif

#ifndef CONFIG_STACK_SIZE
	#define CONFIG_STACK_SIZE 0x800 //must match up with the linker settings
#endif
#ifndef CONFIG_HEAP_SIZE_BYTES
	#define CONFIG_HEAP_SIZE_BYTES 0x400 //must match up with the linker settings
#endif

#ifndef DEMO_RING_BUFFER_SIZE
	#define DEMO_RING_BUFFER_SIZE 16 //uart ring buffer size for when we are testing and demoing
#endif


#ifndef MQTT_TASK_STACK_SIZE
	#define MQTT_TASK_STACK_SIZE 1900
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
	#define CONFIG_MDNS_ENABLED (1)
#endif
#define CONFIG_MDNS_HOSTNAME "mDnsInxIoT "
#ifndef CONFIG_WEB_SOCKET_MESSAGE_ENABLED
	#define CONFIG_WEB_SOCKET_MESSAGE_ENABLED (1U)
#endif

#define CONFIG_ORGANISATION_NAME_LENGTH (256U)
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

#define CONFIG_EMR_RESPONSE_TIME_MILLISECONDS (100U)
#define CONFIG_LPTMR_BASE LPTMR0
// Low Power Timer interrupt time in milliseconds
// this 1000 / numADCReads
// we currently have 3 ADC reads, read1: ADC0 + ADC1 temperature, read2: ADC0+ADC1 duty, read3: AC1 cpu temp
#define CONFIG_LPTMR_NUM_READS (3U)
#define CONFIG_SAMPLES_PER_DUTY_CYCLE (10U)
#define CONFIG_LPTMR_DEFAULT_INTERRUPT_MILLISECONDS (333U)
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
#define CONFIG_MQTT_MAX_CONNECTION_ATTEMPTS (5U) //number of mqtt connection attempts before we cycle the network stack
#define INX_MQTT_MAX_PAYLOAD_SIZE (512)
#define CONFIG_ERROR_MESSAGE_SIZE INX_MQTT_MAX_PAYLOAD_SIZE/4
#define CONFIG_FLASH_SECTOR_SIZE_BYTES (4096)
#define CONFIG_MAX_SCHEDULE_SIZE_BYTES CONFIG_FLASH_SECTOR_SIZE_BYTES
#define CONFIG_HOURS_IN_WEEK (7*24)
/*
 * System Features
 */
#ifndef CONFIG_MQTT_ENABLED
	#define CONFIG_MQTT_ENABLED (1)
#endif
#define CONFIG_DHCP_WAIT_TICKS (64U) //how long we wait for DHCP to work

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
	#define CONFIG_NUMBER_OF_FLASH_SECTORS_FOR_SETTINGS 1
#endif

#ifndef HTTPSRV_CFG_MBEDTLS_ENABLE
	#define HTTPSRV_CFG_MBEDTLS_ENABLE 0 //makes the local webserver use TLS, experimental
#endif

/*
 * Application default settings and ranges
 */
#define CONFIG_DNS_WAIT_TICKS (32U)
#define LWIP_DHCP_PROVIDE_DNS_SERVERS 1 //so we can get DNS servers from a dhcp request
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
#define CONFIG_MAX_TIME_DRIFT_SECONDS (60)
#define CONFIG_MAX_MILLISECONDS_BETWEEN_SAVES (24*60*60*1000) //we save roughly once a day so that we can track relay on counts
//#define CONFIG_MAX_MILLISECONDS_BETWEEN_SAVES (1*30*1000) //we save roughly once a day so that we can track relay on counts
#define CONFIG_TIME_DRIFT_TICKS (86400) //24 hours, how often we try to get an updated time
#define CONFIG_TIME_SUBSCRIBE_TICKS (180) //how long we wait for a time message after subscribing
#define CONFIG_TIME_SUBSCRIBE_RTC_DRIFTED_WAIT (864000) //todo dheck this is OK with changes to sampling rates. Should be 10 days, if we haven't updated time after that scheduling stops
#define CONFIG_TIME_DEBUG LWIP_DBG_ON
#define CONFIG_ADC_VALUES_QUEUE_LENGTH (2)
#define CONFIG_PID_CONDITION_MONITORING_DEBUG LWIP_DBG_OFF
#define CONFIG_PID_CONDITION_MONITORING_WINDOW_SAMPLES (5)
#define CONFIG_PID_CONDITION_MONITORING_RETRY_SAMPLES CONFIG_PID_CONDITION_MONITORING_WINDOW_SAMPLES*2
#define CONFIG_PID_CONDITION_MONITORING_MAX_TEMP_DIFF (20.0)
#define CONFIG_PID_CONDITION_MONITORING_MIN_TEMP_DIFF CONFIG_PID_CONDITION_MONITORING_MAX_TEMP_DIFF*-1.0
#define CONFIG_PID_CONDITION_MONITORING_RETRY_LIMIT (2)
#define CONFIG_PID_I_MAX 10.0
//changing this is dangerous, will need testing
#define CONFIG_PID_CONTROLLERS_NUM (3U)
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

/*
 * High-level TCPIP Networking Protocol Configuration
 *
 */
#if CONFIG_MQTT_LOCATION == CONFIG_MQTT_LOCATION_INX || CONFIG_MQTT_LOCATION == CONFIG_MQTT_LOCATION_INX_PREMIGRATION
	#define CONFIG_BROKER_HOST_NAME       "www.inx-systems.com"
#elif CONFIG_MQTT_LOCATION == CONFIG_MQTT_LOCATION_PATRICK
	#define CONFIG_BROKER_HOST_NAME "mqtt-home.hatsoffsoftware.net"
#elif CONFIG_MQTT_LOCATION == CONFIG_MQTT_LOCATION_TEST
	#define CONFIG_BROKER_HOST_NAME "mqtt2.inx-systems.net"
#else
	#define CONFIG_BROKER_HOST_NAME       "iot-vs1.nibe.se"
#endif
#define CONFIG_BROKER_HOST_IP       NULL
#define CONFIG_CLIENT_GROUP_ID        "group1" /* each client connected to the host has to use a unique ID */
#define CONFIG_CLIENT_USER_NAME       "inx"
#define CONFIG_CLIENT_USER_PASSWORD   "88dogcat99"
#define CONFIG_SETTINGS_TOPIC_NAME    "settings/"CONFIG_CLIENT_GROUP_ID"/%s/updated"
#define CONFIG_ALARMS_TOPIC_NAME	"alarms/"CONFIG_CLIENT_GROUP_ID"/%s/usertemperature"
#define CONFIG_TOPIC_NAME_DUTY_MEASUREMENTS "measurements/"CONFIG_CLIENT_GROUP_ID"/%s/duty"
#define CONFIG_MQTT_TOPIC_NAME_UPDATE_SETTINGS "update/settingsv2/%s"
#define CONFIG_MQTT_TOPIC_NAME_SODLCHUNK_POST "sodlchunk/postv1/%s"
#define CONFIG_MQTT_TOPIC_NAME_SODLCHUNK_GET "sodlchunk/getv1/"CONFIG_CLIENT_GROUP_ID"/%s"
#define CONFIG_MQTT_TOPIC_NAME_RESET "update/reset/%s"
#define CONFIG_MQTT_TIME_CHANNEL "inx/time"
#define CONFIG_MQTT_TOPIC_NAME_UPDATE_ORGANISATION "update/organisationID/%s"
#define CONFIG_MQTT_TOPIC_NAME_FIRMWARE_STARTOTA "firmware/startota/%s"
#define CONFIG_MQTT_TOPIC_NAME_INFO_LOGS "info/logs/%s"
#define CONFIG_MQTT_TOPIC_NAME_FIRMWARECHUNK_GET "firmwarechunk/get/"CONFIG_CLIENT_GROUP_ID"/%s"
#define CONFIG_MQTT_TOPIC_NAME_FIRMWARECHUNK_POST "firmwarechunk/post/%s"
#define CONFIG_MQTT_TOPIC_NAME_CONNECTIVITY_DIDCONNECT "connectivity/"CONFIG_CLIENT_GROUP_ID"/%s/didconnect"

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
#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRi || INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
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
	#define CONFIG_ENABLE_RTC (1)
	#define CONFIG_MODBUS_ENABLED (1U)
	//#define SKIP_ETHERNET_SETUP
	#define CONFIG_TLK106 (1U) // note we will need to split this condition for different clock paths
	#define CONFIG_SMI_PHY_ADDR (30U)

	/* TODO - move all these pin configs to board specific hardware config header file */
	#define CONFIG_GPIO_PHY_RESET_BASE GPIOA
	#define CONFIG_GPIO_PHY_RESET 4U
	#define CONFIG_PHY_ENABLE_CLOCK_OUTPUT (0U)
	#define CONFIG_PHY_FORCE_POLARITY (0U)

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
#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
	#define CONFIG_GPIO_PROCBOARDLEDS_BASE GPIOE
	#define CONFIG_GPIO_PROCBOARDLED2_RED_PIN (2U)
	#define CONFIG_GPIO_PROCBOARDLED2_GRN_PIN (0U)
	#define CONFIG_GPIO_PROCBOARDLED1_RED_PIN (5U)
	#define CONFIG_GPIO_PROCBOARDLED1_GRN_PIN (4U)
#else

	#define CONFIG_GPIO_PROCBOARDLEDS_BASE GPIOE
	#define CONFIG_GPIO_PROCBOARDLED1_RED_PIN (2U)
	#define CONFIG_GPIO_PROCBOARDLED1_GRN_PIN (0U)
	#define CONFIG_GPIO_PROCBOARDLED2_RED_PIN (5U)
	#define CONFIG_GPIO_PROCBOARDLED2_GRN_PIN (4U)

#endif

	#define CONFIG_GPIO_BUTTON_UP_BASE GPIOC
	#define CONFIG_GPIO_BUTTON_DOWN_BASE GPIOC

	#define CONFIG_GPIO_BUTTON_UP_PORT PORTC
	#define CONFIG_GPIO_BUTTON_DOWN_PORT PORTC
#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
	#define CONFIG_GPIO_BUTTON_MODE_BASE GPIOC
	#define CONFIG_GPIO_BUTTON_MODE_PORT PORTC
	#define CONFIG_GPIO_BUTTON_MODE_PIN (13U) // aka front panel spare gpio
	#define CONFIG_GPIO_BUTTON_SET_BASE GPIOE
	#define CONFIG_GPIO_BUTTON_SET_PORT PORTE
	#define CONFIG_GPIO_BUTTON_SET_PIN (25U)
	#define CONFIG_GPIO_BUTTON_UP_PIN (4U)   // up and down buttons are reversed on HRc so we know when we have the wrong build variant (INtended -- honest)
	#define CONFIG_GPIO_BUTTON_DOWN_PIN (5U)

#else
    #define CONFIG_GPIO_BUTTON_UP_PIN (5U)
	#define CONFIG_GPIO_BUTTON_DOWN_PIN (4U)
	#define CONFIG_GPIO_BUTTON_SET_PORT PORTC
	#define CONFIG_GPIO_BUTTON_SET_BASE GPIOC
	#define CONFIG_GPIO_BUTTON_SET_PIN (6U)
#endif


	#define CONFIG_GPIO_BUTTONS_INTERRUPT (0U)

	#define CONFIG_GPIO_BUTTON_UP_PORT PORTC
	#define CONFIG_GPIO_BUTTON_DOWN_PORT PORTC

	#define CONFIG_GPIO_RS485_ENABLE_DISABLE 2
#endif
// sub clauses for each HRx variant
#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRi
	#define CONFIG_NUM_DISPLAYS 1
	#ifndef INX_HEATROD_HEALTHCHECKTESTS
		#define INX_HEATROD_HEALTHCHECKTESTS (1U) //indicates we want software IEC checks at start up (but will not run in diagnostic build mode)
	#endif
	#define INX_HEATROD_STARTUP_CHECK (1U) //tests the relays on boot

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
	#define CONFIG_GPIO_CONTROL_INTERNAL_RELAY_1 (3U)
	#define CONFIG_GPIO_CONTROL_INTERNAL_RELAY_2 (9U)
	#define CONFIG_GPIO_CONTROL_INTERNAL_RELAY_3 (10U)
	#define CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_1 (11U)
	#define CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_2 (18U)
	#define CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_3 (19U)

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
/* And some inputs that are probably HRi specific */
#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRi
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
	#define CONFIG_DUTY_CYCLES_AT_FULL_POWER_BEFORE_FORCING_RELAY_OFF_TO_CHECK_TCO (20U)
#elif INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
	#define CONFIG_GPIO_SENSE_CONRELAY_PORT PORTC
	#define CONFIG_GPIO_SENSE_CONRELAY_BASE	GPIOC
	#define CONFIG_GPIO_SENSE_CONRELAY_1 (2U)
	#define CONFIG_GPIO_SENSE_CONRELAY_2 (1U)
#endif


#if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
	#ifndef INX_HEATROD_HEALTHCHECKTESTS
		#define INX_HEATROD_HEALTHCHECKTESTS (1U) //indicates we want IEC checks
	#endif //ifndef INX_HEATROD_HEALTHCHECKTESTS
	#define CONFIG_NUM_DISPLAYS 2
/* Numerical Display 2*/
// PTC B :
	#define CONFIG_GPIO_NUMERICAL_DISPLAY2_PORT PORTB
	#define CONFIG_GPIO_NUMERICAL_DISPLAY2_BASE GPIOB
	#define CONFIG_GPIO_NUMERICAL_DISPLAY2_CHAR1	(20U)
	#define CONFIG_GPIO_NUMERICAL_DISPLAY2_CHAR2	(21U)
	#define CONFIG_GPIO_NUMERICAL_DISPLAY2_CHAR3	(23U) // note this is a fix for a hardwre problem on the HRc display
	#define CONFIG_GPIO_NUMERICAL_DISPLAY2_CHAR4	(22U) // note this is a fix for a hardwre problem on the HRc display

  //these two should match
	#define CONFIG_GPIO_NEGATIVE_DISPLAY1_PIN_PORT PORTC
	#define CONFIG_GPIO_NEGATIVE_DISPLAY1_PIN_BASE GPIOC
	#define CONFIG_GPIO_NEGATIVE_DISPLAY1_PIN 18
	//these two should match
	#define CONFIG_GPIO_NEGATIVE_DISPLAY2_PIN_PORT PORTC
	#define CONFIG_GPIO_NEGATIVE_DISPLAY2_PIN_BASE GPIOC
	#define CONFIG_GPIO_NEGATIVE_DISPLAY2_PIN 3

	/* Warning duplicating these from HRi in case they change */
	#define CONFIG_GPIO_CONTROL_BASE_OUT GPIOB
	#define CONFIG_GPIO_CONTROL_INTERNAL_RELAY_1 (3U)
	#define CONFIG_GPIO_CONTROL_INTERNAL_RELAY_2 (9U)
	#define CONFIG_GPIO_CONTROL_INTERNAL_RELAY_3 (10U)
	#define CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_1 (11U)
	#define CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_2 (18U)
	#define CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_3 (19U)

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

//#define CONFIG_GPIO_OUTPUT0 9U -- replaced by CONFIG_GPIO_CONTROL_INTERNAL_RELAY_X
//#define CONFIG_GPIO_OUTPUT1 23u
//#define CONFIG_GPIO_OUTPUT2 23u

#endif


/*
 * CONFIG COMBINATION RULE ENFORCEMENT
 * */
#ifdef INX_HEATROD_IOT_DIAGNOSTIC_BUILD
#if INX_HEATROD_HEALTHCHECKTESTS == 1
//#warning "HEALTHCHECK TESTS HAVE BEEN DISABLED IN DIAGNOSTIC MODE!"
#undef INX_HEATROD_HEALTHCHECKTESTS
#define INX_HEATROD_HEALTHCHECKTESTS  0
#endif
#endif

/*
 * Configure Diagnostic Mode & Debug verbosity and modules
 * */
// todo move the following into a debug utility header
#ifdef INX_HEATROD_IOT_DIAGNOSTIC_BUILD
#define INX_HW_DIAG_ADC_TESTS_TODO (10)
#define HW_DIAG_printf(fmt,...) printf(fmt, ##__VA_ARGS__ )
#else // possibly a production build
//#define HW_DIAG_printf(fmt,...) printf(fmt, ##__VA_ARGS__ )
#define HW_DIAG_printf(fmt,...) {}
#endif

#if CONFIG_OTA_DIAG
	#define INX_PRINTF_OTA(fmt,...) printf(fmt, ##__VA_ARGS__ )
#else
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
#define DEBUG_MODBUS LWIP_DBG_OFF
#define DEBUG_LED LWIP_DBG_OFF
#define DEBUG_BUTTONS LWIP_DBG_OFF


#define INX_BOOT_DIAG_PRINT(message) do { \
                               if (gDiagnosticBoot==true){ \
                                 printf message; \
                               } \
                             } while(0)


#if CONFIG_UART_DEBUG
	#define HW_DIAG_UART_printf(fmt,...) printf(fmt, ##__VA_ARGS__ )
#else
	#define HW_DIAG_UART_printf(fmt,...) {}
#endif //if CONFIG_UART_DEBUG

typedef enum {
	INX_UART_BAUD_RATE_OFF,
	INX_UART_BAUD_RATE_9600,
	INX_UART_BAUD_RATE_19200,
	INX_UART_BAUD_RATE_38400,
	INX_UART_BAUD_RATE_57600,
	INX_UART_BAUD_RATE_115200
} inxUARTBaudRate_t;

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

typedef struct {
	uint32_t unixTime;
} inxError_t;

typedef struct _error_logs{
	bool locked;
	inxError_t errors[ERROR_LOGS_END];
} errorLogs_t;

typedef enum {
	INX_TIMER_STATE_READY,
	INX_TIMER_STATE_START,
	INX_TIMER_STATE_RUNNING,
	INX_TIMER_STATE_FINISHED,
	INX_TIMER_STATE_GOING_TO_SET_POINT
} inxTimerState_t;

typedef enum {
	INX_LOCAL_UI_MODE_MOBILE,
	INX_LOCAL_UI_MODE_MINI_DASHBOARD,
	INX_LOCAL_UI_MODE_FULL_DASHBOARD,
	INX_LOCAL_UI_MODE_FULL_SETTINGS,
	INX_LOCAL_UI_MODE_DISABLE,
	INX_LOCAL_UI_MODE_COUNT // used only for UI Mode option count
} INX_Local_UI_Mode_t;

typedef enum {
	INX_SAFETY_MODE_EN60335,
	INX_SAFETY_MODE_ADDITIONAL_CHECKS,
	INX_SAFETY_MODE_DISABLE_SLOW_START_CHECKS,
	INX_SAFETY_MODE_DISABLE_ALL_HARDWARE_CHECKS
} inxSafetyMode_t;

typedef enum  {
	ADC_STATUS_OK,
	ADC_STATUS_OUTLIERS,
	ADC_STATUS_CONNECTION_ERROR
} adcStatusType ;

typedef enum {
	INX_CONTROLLER_OUTPUT_MODE_DISABLED, // this is the emergency shutdown flag. Anything that can set an output relays should check for this flag first.
	INX_CONTROLLER_OUTPUT_MODE_MODBUS_ONLY,
	INX_CONTROLLER_OUTPUT_MODE_EXTERNAL_RELAY,
	INX_CONTROLLER_OUTPUT_MODE_INTERNAL_RELAY //this is the default mode
} inxControllerOutputMode_t;

typedef enum {
	INX_CONTROLLER_CONTACTOR_MODE_EMR,
	INX_CONTROLLER_CONTACTOR_MODE_SSR
} inxControllerContactorMode_t;

typedef struct _adcRawValues{
	volatile uint32_t pt100Channel1;
	volatile uint32_t pt100Channel2;
	volatile uint32_t cpuTemp1;
	volatile uint32_t zeroTenV;
	volatile uint32_t fourTwentyMa;
	volatile uint32_t thermocouple1;
} adcRawValues_t;

typedef struct _adcConvertedValues{
	float pid1;
	float pid2;
	float pid3;
	float cpuTemp1;
} adcConvertedValues_t;

typedef struct _globalADCValues{
	rtc_datetime_t finishDate;
	adcConvertedValues_t current;
	adcConvertedValues_t values[INX_SAMPLEBUF_SIZE];
} globalADCValues_t;

typedef struct _dutyCycleState
{
	bool conditionBad;
	uint32_t cycleInterruptLength;
	GPIO_Type* internalGPIOBase;
	GPIO_Type* externalGPIOBase;
	uint32_t numInterrupts;
	volatile bool on;
	uint32_t powerOffInterruptCount;
	uint32_t powerPercent;
	uint32_t powerOnThresholdPercent;
	uint32_t internalGPIOPin;
	uint32_t externalGPIOPin;
	uint32_t ticksSinceChange;
	uint32_t externalOnCount;
	uint32_t internalOnCount;
} dutyCycleState_t;

typedef struct _adcValuesQueueMessage{
	uint8_t numElements;
	bool duty1On;
	bool duty2On;
	bool duty3On;
	bool log;
} adcValuesQueueMessage_t;

typedef struct _dutyCycleHistory
{
	uint32_t powerHistoryHead;
	rtc_datetime_t historyFinishDate;
	adcValuesQueueMessage_t powerHistory[CONFIG_DUTY_CYCLE_HISTORY_LENGTH];
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

typedef enum {
	ELEMENT_MODE_SINGLE,
	ELEMENT_MODE_DOUBLE_STEPPED,
	ELEMENT_MODE_DOUBLE_TANDEM,
	ELEMENT_MODE_TRIPLE_STEPPED,
	ELEMENT_MODE_TRIPLE_TANDEM,
	ELEMENT_MODE_SINGLE_TWO_POLE
} ElementMode_t;

typedef struct {
	float kp;
	float ki;
	float kd;
} inxPIDCoefficients_t;

typedef enum {
	SCHEDULE_MODE_DISABLED,
	SCHEDULE_MODE_MANUAL,
	SCHEDULE_MODE_ENABLED,
	SCHEDULE_MODE_TIMER,
	SCHEDULE_MODE_TIMER_SET_POINT
} Schedule_Mode_t;

/* these are the logical ADC channels (Not NXP ADC Channels) */
typedef enum {
	INX_CONTROLLER_SENSORS_NONE,
	INX_CONTROLLER_SENSORS_PT100_CHANNEL1,
	INX_CONTROLLER_SENSORS_PT100_CHANNEL2,
	INX_CONTROLLER_SENSORS_10V,
	INX_CONTROLLER_SENSORS_20MA,
	INX_CONTROLLER_SENSORS_MODBUS,
	INX_CONTROLLER_SENSORS_THERMOCOUPLE
} inxControllerSensors_t;

typedef struct {
	float p;
	float i;
	float d;
	inxPIDCoefficients_t coefficients;
	uint32_t powerPercent;
	float desiredTemp;
	ElementMode_t elementMode;
	inxControllerSensors_t sensors;
	Schedule_Mode_t scheduleMode;
	float manualSetPoint;
	float currentValue;
	float upperAlarmTemp;
	float lowerAlarmTemp;
	uint32_t timerDurationSeconds;
	uint32_t timerSecondsAtFinish;
	uint32_t timerSecondsRemaining;
	inxTimerState_t timerState;
	uint32_t rampSecondsPerDegree;
	uint32_t rampSecondsAtNextIncrement;
	float rampDesiredTemp;
} inxPID_t;

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
	INX_NETWORK_CONNECTION_MODE_OFF,
	INX_NETWORK_CONNECTION_MODE_READ,
	INX_NETWORK_CONNECTION_MODE_WRITE
} inxNetworkConnectionMode_t;

typedef enum {
	NETWORK_STATE_INIT,
	NETWORK_STATE_WAIT_FOR_PHY,
	NETWORK_STATE_START_DHCP,
	NETWORK_STATE_WAITING_FOR_DHCP,
	NETWORK_STATE_CONNECTED,
	NETWORK_STATE_DO_DISCONNECT
} NETWORK_State_t;

typedef enum {
	INX_MODBUS_STATE_STOPPED,
	INX_MODBUS_STATE_INIT,
	INX_MODBUS_STATE_STOP,
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
	INX_AUX_DISABLED,
	INX_AUX_CONTROL_1,
	INX_AUX_CONTROL_2,
	INX_AUX_CONTROL_3,
	INX_AUX_CONTROL_4,
	INX_AUX_CONTROL_5,
	INX_AUX_CONTROL_6
} inxAuxControlEnum_t;

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
	float tempDiff;
	float earliestTemp;
} pidConditionStateDiff_t;

typedef struct _pidConditionState_t {
	pidConditionStateMode_t mode;
	float temp1AtBad;
	uint32_t samplesSinceBad;
	uint32_t retries;
	uint32_t valuesIndex;
	float values[CONFIG_PID_CONDITION_MONITORING_WINDOW_SAMPLES];
} pidConditionState_t;

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

typedef struct _permStorage{
	uint32_t settingsCrcValue;
	size_t size;
	uint32_t validationCode;
	bool diagnosticsDone;
	bool provisioned;
	uint64_t organisationID;
	uint32_t crcValue;
	uint32_t programSizeBytes;
	bool networkDhcp;
	errorLogs_t errorLogs;
	char localName[CONFIG_LOCAL_NAME_LENGTH];
	char location[CONFIG_LOCATION_LENGTH];
	bool rtcDisabled;
	float cal1;
	float cal2;
	float cal3;
} permStorage_t;
typedef struct {
	char* filename;
	uint16_t sizeBytes;
} inxEHSFile_t;

void buildStatusPayload(char* payload);
void copyGlobalADCValues(globalADCValues_t* pDst,globalADCValues_t* pSrc);
uint32_t dutyCycleStateCalculatePowerPercentFromHistory(const dutyCycleHistory_t* pState,const uint8_t pidNum,const inxPID_t* pPID);
void firmwareWriteSector(const bool doSwap,const uint32_t firmwareCurrentSector,u8_t* otaUpdateOneSectorBuffer,const uint32_t programSizeBytes,const uint32_t firmwareCrc);
uint32_t mainGetFlashSectorSize();
void mainFactoryReset();
void mainReset();
float mainGetDesiredTemp();
char* mainGetGlobalClientId();
float mainGetLowerAlarmTemp();
float mainGetUpperAlarmTemp();
float mainGetElementRatingKW();
float mainGetPID1_KP();
float mainGetPID1_KI();
float mainGetPID1_KD();
char* mainGetLocalName();
char* mainGetLocation();
void mainUpdateSchedule(const u16_t crc,const u8_t* dataIn,const u8_t* auxDataIn);
void mainSetNetworkState(const NETWORK_State_t newState);
void updateLowerAlarmTemp(const bool byUser,inxPID_t* pPID,const float newDesiredTemp);
void updateUpperAlarmTemp(const bool byUser,inxPID_t* pPID,const float newDesiredTemp);
void updatePID1_KP(const bool byUser,inxPID_t* pPID,const float newValue);
void updatePID1_KI(const bool byUser,inxPID_t* pPID,const float newValue);
void updatePID1_KD(const bool byUser,inxPID_t* pPID,const float newValue);
void updateScheduleMode(const uint8_t pidNum,const bool byUser,inxPID_t* pPID,const Schedule_Mode_t newScheduleMode);
void updateDesiredTemp(inxPID_t* pPID,const float newDesiredTemp);
void updateManualSetPoint(const bool byUser,inxPID_t* pPID,const float newManualSetPoint);
bool ADC16_InitHardwareTrigger(ADC_Type *base,adc16_channel_config_t* adcChnConfig);
void LPTMR_InitTriggerSourceOfAdc(LPTMR_Type *base);
uint16_t inxRTCReadValue(const inxRTCPartEnum_t part);
void inxRTCWriteValue(const inxRTCPartEnum_t part,const uint16_t month);
void inxSetElementMode(const bool byUser,inxPID_t* pPID,const uint16_t mode);
void inxSetAuxControlOutput(const bool byUser,const uint16_t controlOutput);
void inxSetPowerMode(const bool byUser,const uint16_t mode);

/* Duty Relay Management & Control functions */
void dutyCycleStateOn(volatile dutyCycleState_t* pState);
void dutyCycleStateOffForced(volatile dutyCycleState_t* pState);
void inxDutyCyclePowerThresholdSet(const bool byUser,volatile dutyCycleState_t* pState,const uint16_t powerPercent);

/* Control Loop */
void inxSetPIDCoefficients(float* coefficient,const float value);
void inxDisplay2Set(const char char1,const char char2, const char char3, const char char4,const bool async);

void mainUpdateOrganisationID(const char* data);
void inxRTCInit();
void updateLPTMRInterruptMilliseconds(const bool ignoreDutyCyle);
void inxUpdateSettings(const char* buffer);
uint32_t inxRTCADCGetSeconds(); //gets the time based on the ADCs
uint32_t inxRTCGetSeconds(); // retrns err numbers if mode is not right
uint32_t inxEHSGetUSeconds();
uint32_t inxRTCGetSecondsAlways(); // returns what ever is in the RTC register
status_t inxRTCWriteDateTime(const rtc_datetime_t date);
void inxPWMConfig(const uint32_t periodHz,const uint8_t ehsPin);
void inxPWMEnable(const bool enable);
void inxPWMDuty(const uint8_t pin,const uint8_t powerPercent);
void inxRTCWriteSeconds(const uint32_t seconds);
void inxRTCSecondsToDate(const char* timezone,const uint32_t unixSeconds,uint16_t* pYear,uint8_t* pMonth,uint8_t* pDOM,uint8_t* pDOW,uint8_t* pHour,uint8_t* pMinute,uint8_t* pSecond);
void inxSetPIDTimerDuration(const bool byUser,inxPID_t* pPID,const uint32_t seconds,const bool trigger);
uint32_t inxTimerSecondsToTicks(const uint32_t seconds);
uint32_t inxTimerTicksToSeconds(const uint32_t ticks);
const adcRawValues_t* mainGetRawSensorValues();
float getConvertedValue(const inxControllerSensors_t sensors,const adcRawValues_t* pRawValues);
extern volatile bool gUserTemperatureAlarmTriggered;
extern volatile bool gMqttADCValuesLock;
extern volatile bool gMqttADCValuesReady;
extern globalADCValues_t gMqttADCValues;
extern threadSafeDutyCycleHistory_t gMqttDutyCycleHistory0;
bool inxGPIOOn(GPIO_Type *base, uint32_t mask);
bool inxGPIOOff(GPIO_Type *base, uint32_t mask);
void inxWriteSODLBuffer();
#define CONFIG_EHS_MAX_SODL_SECTORS 2
typedef struct {
    uint8_t fileData[CONFIG_FLASH_SECTOR_SIZE_BYTES];
} inxTSDL;
char* inxSODLGetReadPointer();
inxTSDL* inxSODLGetWritePointer();
uint32_t inxSODLGetSectorAddress(const u8_t sectorIndex);
bool inxEHSPermanentStorageGetValue(const uint16_t id,void* pValue);
err_enum_t inxNetworkTaskWaitForDNS(const char* host,ip4_addr_t* pAddress);
uint32_t inxEHSPermanentStorageAddress();
status_t writeToFlash(void* pMem,size_t size,uint32_t offset,const bool useLower,const uint32_t base);
inxEHSFile_t inxEHSFilesFind(const char* fname);
uint8_t* inxEHSFilesGetData(inxEHSFile_t file);
void inxClearFiles();
#endif /* CONFIG_H_ */
