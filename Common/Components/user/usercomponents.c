//ICB START INCLUDES MACRO -- DO NOT ALTER
#include "blockref_table.h"
#ifndef EHS_SKIP_GNULIBRARIES
#include "inx-ehs_controller.h"
#include "inx-inx-netsocket.h"
#include "inx-json_stream.h"
#include "inx-application_info_getter.h"
#include "inx-JSONObjectFunctionBlock.h"
#include "inx-application_run.h"
#include "inx-xml_stream.h"
#include "inx-devman_interface.h"
#include "inx-webkit.h"
#include "inx-netsocketrsrvr.h"
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
EhsBlockRefType EhsBlockRefTable_usercomponents[]={
#ifndef EHS_SKIP_GNULIBRARIES
EHS_BLOCKREF_ENTRY(INXWARE_FB_ehs_controller ,ehs_controller),
EHS_BLOCKREF_ENTRY(INXWARE_FB_netSocket ,netSocket),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_json_stream_parser ,json_stream_parser),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_application_info_getter ,application_info_getter),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_JSONObject ,JSONObject),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_xml_stream_parser ,xml_stream_parser),
EHS_BLOCKREF_ENTRY(INXWARE_FB_application_run ,application_run),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_netsocketsrvr ,netsocketsrvr),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_DevmanInterface ,DevmanInterface),
#ifdef EHS_UNITY3D_WIDGETS
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_unity2,unity2),
#endif
#endif //ifndef EHS_SKIP_GNULIBRARIES
#ifdef EHS_MQTT_SUPPORT
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_mqtt_client ,mqtt_client),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_mqtt_subscribe ,mqtt_subscribe),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_mqtt_publish ,mqtt_publish),
#endif
#ifdef EHS_NXP_SUPPORT
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_adc_read ,adc_read),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_modbus_config ,modbus_config),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_modbus_read ,modbus_read),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_modbus_write ,modbus_write),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_ndc ,ndc),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_rtc,rtc),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_pwm,pwm),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_uart_config,uart_config),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_permanent_storage_int,permanent_storage_int),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_permanent_storage_bool,permanent_storage_bool),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_permanent_storage_real,permanent_storage_real),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_permanent_storage_string,permanent_storage_string),
#endif
#ifdef EHS_DEVMAN_SUPPORT
EHS_BLOCKREF_ENTRY(INXWARE_FB_DevmanInterface ,DevmanInterface),
#endif
#ifdef EHS_WEBKIT_SUPPORT
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_html_java ,html_java),
#endif
{0}
};
//ICB END BLOCK REF TABLE MACRO -- DO NOT ALTER
