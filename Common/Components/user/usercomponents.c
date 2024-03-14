/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/


//ICB START INCLUDES MACRO -- DO NOT ALTER
#include "blockref_table.h"

#ifdef EHS_COMMS_API_SUPPORT
#include "inx-inx-netsocket.h"
#include "inx-netsocketrsrvr.h"
#endif

#ifdef EHS_DEVMAN_SUPPORT
#include "inx-devman_interface.h"
#endif

#ifndef EHS_SKIP_GNULIBRARIES
#include "inx-ehs_controller.h"
#include "inx-json_stream.h"
#include "inx-application_info_getter.h"
#include "inx-JSONObjectFunctionBlock.h"
#include "inx-application_run.h"
#include "inx-xml_stream.h"
#include "inx-webkit.h"
#endif

#include "inx-unity2.h"
#include "inx-mqtt_publish.h"
#include "inx-mqtt_subscribe.h"
#include "inx-mqtt_client.h"
#include "inx-rtc.h"
#include "inx-pwm.h"
#include "inx-numeric_display_char.h"
#include "inx-modbus_read.h"
#include "inx-modbus_write.h"
#include "inx-modbus_config.h"
#include "inx-uart_config.h"
#include "inx-permanent_storage.h"
// @TODO - move all adc/dac components from usercomponents to more siutable place e.g. peripherals
#ifdef EHS_PERIPHERALS_ADC_DAC_SUPPORT
#include "inx-adc_read.h"
#endif
#ifdef EHS_LORAWAN_SUPPORT
#include "inx-lorawan.h"
#endif
#ifdef EHS_WIFI_SUPPORT
#include "inx-wifi_station.h"
#endif
#ifdef EHS_AUDIO_INPUT_LEVEL_SUPPORT
#include "inx-audio_input_level.h"
#endif
#ifdef EHS_UART_SUPPORT
#include "inx-uart.h"
#endif
#include "inx-hex2string.h"
#include "inx-Int2HexString.h"
#include "inx-scheduler.h"
#ifdef EHS_PID_SUPPORT
#include "inx-PID.h"
#endif
#include "inx-ota.h"
#include "inx-ota_data_parser.h"
#include "inx-reboot.h"
//ICB END INCLUDES MACRO -- DO NOT ALTER


//ICB START BLOCK REF TABLE MACRO -- DO NOT ALTER
EhsBlockRefType EhsBlockRefTable_usercomponents[]=
{
#ifndef EHS_SKIP_GNULIBRARIES
EHS_BLOCKREF_ENTRY(INXWARE_FB_NAME_ehs_controller ,INXWARE_FB_ID_ehs_controller,ehs_controller),
#ifdef EHS_COMMS_API_SUPPORT
#ifdef EHS_NETWORKING_SUPPORT
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_netsocketsrvr,INXWARE_FB_ID_netsocketsrvr ,netsocketsrvr),
EHS_BLOCKREF_ENTRY(INXWARE_FB_NAME_netSocket,INXWARE_FB_ID_netSocket ,netSocket),
#endif
//#ifdef EHS_DEVMAN_SUPPORT
//EHS_BLOCKREF_ENTRY(INXWARE_FB_DevmanInterface ,DevmanInterface),
//#endif
#ifdef EHS_DEVMAN_SUPPORT
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_DevmanInterface,INXWARE_FB_ID_DevmanInterface ,DevmanInterface),
#endif
#endif
#ifndef EHS_SKIP_GNULIBRARIES
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_json_stream_parser,INXWARE_FB_ID_json_stream_parser ,json_stream_parser),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_JSONObject,INXWARE_FB_ID_JSONObject ,JSONObject),
#ifndef EHS_EXCLUDE_XML_PARSER
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_xml_stream_parser,INXWARE_FB_ID_xml_stream_parser ,xml_stream_parser),
#endif
#ifndef  EHS_NO_LIBXML2_SUPPORT
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_application_info_getter,INXWARE_FB_ID_application_info_getter ,application_info_getter),
#endif
#endif
EHS_BLOCKREF_ENTRY(INXWARE_FB_NAME_application_run,INXWARE_FB_ID_application_run ,application_run),
#ifdef EHS_UNITY3D_WIDGETS
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_unity2,INXWARE_FB_ID_unity2,unity2),
#endif
#endif //ifndef EHS_SKIP_GNULIBRARIES
#ifdef EHS_MQTT_SUPPORT
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_mqtt_client ,INXWARE_FB_ID_mqtt_client,mqtt_client),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_mqtt_subscribe,INXWARE_FB_ID_mqtt_subscribe ,mqtt_subscribe),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_mqtt_publish,INXWARE_FB_ID_mqtt_publish ,mqtt_publish),
#endif
#ifdef EHS_PERIPHERALS_ADC_DAC_SUPPORT
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_adc_read,INXWARE_FB_ID_adc_read ,adc_read),
#endif
#ifdef EHS_NXP_SUPPORT
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_modbus_config,INXWARE_FB_ID_modbus_config ,modbus_config),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_modbus_read,INXWARE_FB_ID_modbus_read ,modbus_read),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_modbus_write,INXWARE_FB_ID_modbus_write ,modbus_write),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_ndc,INXWARE_FB_ID_ndc ,ndc),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_rtc,INXWARE_FB_ID_rtc ,rtc),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_pwm,INXWARE_FB_ID_pwm ,pwm),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_uart_config ,INXWARE_FB_ID_uart_config,uart_config),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_permanent_storage_int,INXWARE_FB_ID_permanent_storage_int,permanent_storage_int),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_permanent_storage_bool,INXWARE_FB_ID_permanent_storage_bool,permanent_storage_bool),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_permanent_storage_real,INXWARE_FB_ID_permanent_storage_real,permanent_storage_real),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_permanent_storage_string,INXWARE_FB_ID_permanent_storage_string,permanent_storage_string),
#endif
#ifdef EHS_WEBKIT_SUPPORT
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_html_java,INXWARE_FB_ID_html_java ,html_java),
#endif
#ifdef EHS_LORAWAN_SUPPORT
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_lorawan,INXWARE_FB_ID_lorawan,lorawan),
#endif
#ifdef EHS_WIFI_SUPPORT
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_wifi_station,INXWARE_FB_ID_wifi_station,wifi_station),
#endif
#ifdef EHS_AUDIO_INPUT_LEVEL_SUPPORT
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_audio_input_level,INXWARE_FB_ID_audio_input_level,audio_input_level),
#endif
#ifdef EHS_UART_SUPPORT
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_UART,INXWARE_FB_ID_UART,UART),
#endif
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_HexString2String,INXWARE_FB_ID_HexString2String,HexString2String),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_Int2HexString,INXWARE_FB_ID_Int2HexString,Int2HexString),
#ifdef EHS_SCHEDULER_SUPPORT
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_DevmanScheduler,INXWARE_FB_ID_DevmanScheduler,DevmanScheduler),
#endif
#ifdef EHS_PID_SUPPORT
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_PID,INXWARE_FB_ID_PID,PID),
#endif
#ifdef EHS_OTA_SUPPORT
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_OTA,INXWARE_FB_ID_OTA,OTA),
#endif
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_ota_data_parser,INXWARE_FB_ID_ota_data_parser,ota_data_parser),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_Reboot,INXWARE_FB_ID_Reboot,Reboot),
{0}
};
//ICB END BLOCK REF TABLE MACRO -- DO NOT ALTER
