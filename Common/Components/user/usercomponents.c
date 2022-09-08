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
#include "inx-adc_read.h"
#include "inx-rtc.h"
#include "inx-pwm.h"
#include "inx-numeric_display_char.h"
#include "inx-modbus_read.h"
#include "inx-modbus_write.h"
#include "inx-modbus_config.h"
#include "inx-uart_config.h"
#include "inx-permanent_storage.h"
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
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_xml_stream_parser,INXWARE_FB_ID_xml_stream_parser ,xml_stream_parser),
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
#ifdef EHS_NXP_SUPPORT
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_adc_read,INXWARE_FB_ID_adc_read ,adc_read),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_modbus_config,INXWARE_FB_ID_modbus_config ,modbus_config),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_modbus_read,INXWARE_FB_ID_modbus_read ,modbus_read),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_modbus_write,INXWARE_FB_ID_modbus_write ,modbus_write),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_ndc,INXWARE_FB_ID_modbus_write ,ndc),
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
    {0}
};
//ICB END BLOCK REF TABLE MACRO -- DO NOT ALTER
