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

#if defined(EHS_DEVMAN_SUPPORT) || (EHS_DEVMAN_MON_SUPPORT == EHS_DEVMAN_MON_MQTT)
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

#ifdef EHS_PERIPHERAL_DEVICE_SUPPORT
    #include "inx-numeric_display_char.h"
    #include "inx-modbus_read.h"
    #include "inx-modbus_write.h"
    #include "inx-modbus_config.h"
    #include "inx-uart_config.h"
    #include "inx-permanent_storage.h"
#ifdef EHS_PERIPHERALS_PWM_SUPPORT
    #include "inx-pwm_gen.h"
#endif

#ifdef EHS_PERIPHERALS_ADC_DAC_SUPPORT
    #include "inx-adc_read.h"
    #include "inx-adc_config.h"
    #include "inx-adc_read_continuous.h"
    #include "inx-adc_read_single.h"
#endif
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
//#ifdef EHS_UART_SUPPORT
#include "inx-uart.h"
//#endif
#include "inx-hex2string.h"
#include "inx-Int2HexString.h"
#include "inx-scheduler.h"

#ifdef EHS_PID_SUPPORT
#include "inx-PID.h"
#include "inx-calibrate.h"
#endif

#include "inx-ota.h"
#include "inx-ota_data_parser.h"
#include "inx-reboot.h"
#include "inx-cgi2json.h"
#include "inx-stringdivader.h"
#include "inx-display_backlight.h"
#include "inx-dac.h"
#include "inx-pid_relay_config.h"
#include "inx-num_demux.h"
#include "inx-num_mux.h"
#include "inx-rtinfo2.h"
#include "inx-modbus_slave_register.h"
#include "inx-map_int.h"
#include "inx-indexed_mux_int.h"
#include "inx-binary2decimal8.h"
#include "inx-indexed_mux_str.h"
#include "inx-sample2str.h"
#include "inx-key_value.h"
#include "inx-indexed_demux_int.h"
#include "inx-pid_hdwr_config_int.h"
#include "inx-unsigned2int.h"
#include "inx-json_parser_int.h"
#include "inx-json_parser_bool.h"
#include "inx-json_parser_real.h"
#include "inx-indexed_mux_bool.h"
#include "inx-indexed_mux_real.h"
#include "inx-indexed_demux_bool.h"
#include "inx-indexed_demux_real.h"
#include "inx-indexed_demux_str.h"
#include "inx-SineInt.h"
#include "inx-CosineInt.h"
#include "inx-TanInt.h"
#include "inx-json_parser_str.h"
#include "inx-key_value_int.h"
#include "inx-key_value_real.h"
#include "inx-key_value_bool.h"
#include "inx-console_print.h"
//ICB END INCLUDES MACRO -- DO NOT ALTER


//ICB START BLOCK REF TABLE MACRO -- DO NOT ALTER
extern const EhsBlockRefType EhsBlockRefTable_usercomponents[]=
{

EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_HexString2String,INXWARE_FB_ID_HexString2String,HexString2String),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_Int2HexString,INXWARE_FB_ID_Int2HexString,Int2HexString),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_ota_data_parser,INXWARE_FB_ID_ota_data_parser,ota_data_parser),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_Reboot,INXWARE_FB_ID_Reboot,Reboot),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_cgi2json,INXWARE_FB_ID_cgi2json,cgi2json),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_StringDivader,INXWARE_FB_ID_StringDivader,StringDivader),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_NumDemultiplexReal,INXWARE_FB_ID_NumDemultiplexReal,NumDemultiplexReal),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_NumMultiplexReal,INXWARE_FB_ID_NumMultiplexReal,NumMultiplexReal),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_RuntimeInfo2,INXWARE_FB_ID_RuntimeInfo2,RuntimeInfo2),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_MapInt,INXWARE_FB_ID_MapInt,MapInt),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_indexed_mux_int,INXWARE_FB_ID_indexed_mux_int,indexed_mux_int),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_bin2dec8,INXWARE_FB_ID_bin2dec8,bin2dec8),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_indexed_mux_str,INXWARE_FB_ID_indexed_mux_str,indexed_mux_str),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_sample2str,INXWARE_FB_ID_sample2str,sample2str),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_indexed_demux_int,INXWARE_FB_ID_indexed_demux_int,indexed_demux_int),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_Unsigned2Int,INXWARE_FB_ID_Unsigned2Int,Unsigned2Int),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_json_parser_int,INXWARE_FB_ID_json_parser_int,json_parser_int),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_json_parser_bool,INXWARE_FB_ID_json_parser_bool,json_parser_bool),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_json_parser_real,INXWARE_FB_ID_json_parser_real,json_parser_real),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_indexed_mux_bool,INXWARE_FB_ID_indexed_mux_bool,indexed_mux_bool),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_indexed_mux_real,INXWARE_FB_ID_indexed_mux_real,indexed_mux_real),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_indexed_demux_bool,INXWARE_FB_ID_indexed_demux_bool,indexed_demux_bool),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_indexed_demux_real,INXWARE_FB_ID_indexed_demux_real,indexed_demux_real),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_indexed_demux_str,INXWARE_FB_ID_indexed_demux_str,indexed_demux_str),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_SineInt,INXWARE_FB_ID_SineInt,SineInt),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_CosineInt,INXWARE_FB_ID_CosineInt,CosineInt),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_TanInt,INXWARE_FB_ID_TanInt,TanInt),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_json_parser_str,INXWARE_FB_ID_json_parser_str,json_parser_str),
#ifndef EHS_FILESYSTEM_SUPPORT__NONE
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_key_value,INXWARE_FB_ID_key_value,key_value),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_key_value_int,INXWARE_FB_ID_key_value_int,key_value_int),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_key_value_real,INXWARE_FB_ID_key_value_real,key_value_real),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_key_value_bool,INXWARE_FB_ID_key_value_bool,key_value_bool),
#endif
#ifndef EHS_SKIP_GNULIBRARIES
    EHS_BLOCKREF_ENTRY(INXWARE_FB_NAME_ehs_controller ,INXWARE_FB_ID_ehs_controller,ehs_controller),

    // @TODO - This all needs to go into the networking folder.
    #if defined(EHS_NETWORKING_SUPPORT) || defined(EHS_COMPONENTS_NETWORK_TCPIP_SOCKET)
    #ifdef EHS_COMMS_API_SUPPORT
        //Most of htese if would not be needed if this was in the networking toolbox.
        EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_netsocketsrvr,INXWARE_FB_ID_netsocketsrvr ,netsocketsrvr),
        EHS_BLOCKREF_ENTRY(INXWARE_FB_NAME_netSocket,INXWARE_FB_ID_netSocket ,netSocket),
    #endif // EHS_COMMS_API_SUPPORT
    #endif // EHS_NETWORKING_SUPPORT or EHS_COMPONENTS_NETWORK_TCPIP_SOCKET
    // @TODO - move this to a more appropriate place
    #if defined(EHS_DEVMAN_SUPPORT) || (EHS_DEVMAN_MON_SUPPORT == EHS_DEVMAN_MON_MQTT)
        EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_DevmanInterface, INXWARE_FB_ID_DevmanInterface, DevmanInterface),
    #endif

    EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_json_stream_parser,INXWARE_FB_ID_json_stream_parser ,json_stream_parser),
    EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_JSONObject,INXWARE_FB_ID_JSONObject ,JSONObject),
    #ifndef EHS_EXCLUDE_XML_PARSER
        EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_xml_stream_parser,INXWARE_FB_ID_xml_stream_parser ,xml_stream_parser),
    #endif
    #ifndef  EHS_NO_LIBXML2_SUPPORT
        EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_application_info_getter,INXWARE_FB_ID_application_info_getter ,application_info_getter),
    #endif
    EHS_BLOCKREF_ENTRY(INXWARE_FB_NAME_application_run,INXWARE_FB_ID_application_run ,application_run),
    #ifdef EHS_UNITY3D_WIDGETS
        EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_unity2,INXWARE_FB_ID_unity2,unity2),
    #endif

#ifdef EHS_MQTT_SUPPORT
    EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_mqtt_client ,INXWARE_FB_ID_mqtt_client,mqtt_client),
    EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_mqtt_subscribe,INXWARE_FB_ID_mqtt_subscribe ,mqtt_subscribe),
    EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_mqtt_publish,INXWARE_FB_ID_mqtt_publish ,mqtt_publish),
#endif


#ifdef EHS_WEBKIT_SUPPORT
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_html_java,INXWARE_FB_ID_html_java ,html_java),
#endif

#endif //ifndef EHS_SKIP_GNULIBRARIES

#ifdef EHS_LORAWAN_SUPPORT
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_lorawan,INXWARE_FB_ID_lorawan,lorawan),
#endif

#ifdef EHS_WIFI_SUPPORT
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_wifi_station,INXWARE_FB_ID_wifi_station,wifi_station),
#endif

#ifdef EHS_AUDIO_INPUT_LEVEL_SUPPORT
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_audio_input_level,INXWARE_FB_ID_audio_input_level,audio_input_level),
#endif

#ifdef EHS_PERIPHERALS_BACKLIGHT_SUPPORT
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_Backlight,INXWARE_FB_ID_Backlight,Backlight),
#endif

#ifdef EHS_MODBUS_SUPPORT
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_modbus_config,INXWARE_FB_ID_modbus_config,modbus_config),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_modbus_read,INXWARE_FB_ID_modbus_read ,modbus_read),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_modbus_write,INXWARE_FB_ID_modbus_write ,modbus_write),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_modbus_slave_register,INXWARE_FB_ID_modbus_slave_register,modbus_slave_register),
#endif

#ifdef EHS_SCHEDULER_SUPPORT
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_DevmanScheduler,INXWARE_FB_ID_DevmanScheduler,DevmanScheduler),
#endif
#ifdef EHS_PID_SUPPORT
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_PID,INXWARE_FB_ID_PID,PID),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_Calibrate,INXWARE_FB_ID_Calibrate,Calibrate),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_pid_relay_config,INXWARE_FB_ID_pid_relay_config,pid_relay_config),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_pid_hdwr_config_int,INXWARE_FB_ID_pid_hdwr_config_int,pid_hdwr_config_int),
#endif
#ifdef EHS_OTA_SUPPORT
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_OTA,INXWARE_FB_ID_OTA,OTA),
#endif


#ifdef EHS_COMPONENTS_CONSOLE_IO
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_console_print,INXWARE_FB_ID_console_print,console_print),
#endif

#ifdef EHS_PERIPHERALS_SUPPORT
    // @TODO - do not use any target specific implemenation here
    // fix how NXP target is structured !  DId we just create more junk here rather than fixing existing? 
    #ifdef EHS_NXP_SUPPORT
    // todo2025 if we just nuked all this because it said what day of the week it was written?
    EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_ndc,INXWARE_FB_ID_ndc ,ndc),
    EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_rtc,INXWARE_FB_ID_rtc ,rtc),
    EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_pwm,INXWARE_FB_ID_pwm ,pwm),
    EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_permanent_storage_int,INXWARE_FB_ID_permanent_storage_int,permanent_storage_int),
    EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_permanent_storage_bool,INXWARE_FB_ID_permanent_storage_bool,permanent_storage_bool),
    EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_permanent_storage_real,INXWARE_FB_ID_permanent_storage_real,permanent_storage_real),
    EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_permanent_storage_string,INXWARE_FB_ID_permanent_storage_string,permanent_storage_string),
    #endif

    #ifdef EHS_PERIPHERALS_ADC_DAC_SUPPORT
    EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_adc_read,INXWARE_FB_ID_adc_read ,adc_read),
    EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_adc_config,INXWARE_FB_ID_adc_config,adc_config),
    EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_adc_read_continuous,INXWARE_FB_ID_adc_read_continuous,adc_read_continuous),
    EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_adc_read_single,INXWARE_FB_ID_adc_read_single,adc_read_single),
    EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_dac,INXWARE_FB_ID_dac,dac),
    #endif

    #ifdef EHS_PERIPHERALS_PWM_SUPPORT
    EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_pwm,INXWARE_FB_ID_pwm ,pwm),// This is the NXP one perhaps?
    EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_pwm_gen,INXWARE_FB_ID_pwm_gen,pwm_gen),
    #endif // EHS_PERIPHERALS_PWM_SUPPORT
    #ifdef EHS_UART_SUPPORT
    EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_UART,INXWARE_FB_ID_UART,UART),
    EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_uart_config,INXWARE_FB_ID_uart_config,uart_config),
    #endif
#endif // EHS_PERIPHERALS_SUPPORT

{0}
};
//ICB END BLOCK REF TABLE MACRO -- DO NOT ALTER
