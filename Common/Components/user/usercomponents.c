/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/


//ICB START INCLUDES MACRO -- DO NOT ALTER
#include "blockref_table.h"

// Network socket components moved to networking/

#if defined(EHS_DEVMAN_SUPPORT) || (EHS_DEVMAN_SUPPORT == EHS_DEVMAN_MQTT)
#include "inx-devman_interface.h"
#endif

#ifndef EHS_SKIP_GNULIBRARIES
/* TODO Not sure what inx-ehs-controller.h GNU feATURES ARE??/*/
#include "inx-ehs_controller.h"
// JSON stream parser moved to core/
#include "inx-application_info_getter.h"
// JSON object function block moved to core/
#include "inx-application_run.h"
// XML stream parser moved to core/
// WebKit component moved to gui/
#endif

#include "inx-unity2.h"
// MQTT components moved to networking/
#include "inx-rtc.h"

#ifdef EHS_PERIPHERAL_DEVICE_SUPPORT
    #include "inx-numeric_display_char.h"
    #include "inx-permanent_storage.h"
#endif


#ifdef EHS_LORAWAN_SUPPORT
#include "inx-lorawan.h"
#endif

// WiFi components moved to networking/

#ifdef EHS_AUDIO_INPUT_LEVEL_SUPPORT
#include "inx-audio_input_level.h"
#endif
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
// DAC component moved to peripherals/
#include "inx-pid_relay_config.h"
#include "inx-num_demux.h"
#include "inx-num_mux.h"
#include "inx-rtinfo2.h"
// Modbus slave register component moved to peripherals/
#include "inx-map_int.h"
// Indexed mux components moved to core/
#include "inx-binary2decimal8.h"
#include "inx-sample2str.h"
#include "inx-key_value.h"
#include "inx-pid_hdwr_config_int.h"
#include "inx-unsigned2int.h"
// JSON parser components moved to core/
#include "inx-SineInt.h"
#include "inx-CosineInt.h"
#include "inx-TanInt.h"
#include "inx-key_value_int.h"
#include "inx-key_value_real.h"
#include "inx-key_value_bool.h"
#include "inx-console_print.h"
//ICB END INCLUDES MACRO -- DO NOT ALTER

//ICB START BLOCK REF TABLE MACRO -- DO NOT ALTER
EHS_C_CPP_EXPORT const EhsBlockRefType EhsBlockRefTable_usercomponents[]=
{
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_HexString2String,INXWARE_FB_ID_HexString2String,HexString2String),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_Int2HexString,INXWARE_FB_ID_Int2HexString,Int2HexString),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_ota_data_parser,INXWARE_FB_ID_ota_data_parser,ota_data_parser),
#ifdef EHS_OTA_SUPPORT
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_OTA,INXWARE_FB_ID_OTA,OTA),
#endif
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_Reboot,INXWARE_FB_ID_Reboot,Reboot),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_cgi2json,INXWARE_FB_ID_cgi2json,cgi2json),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_StringDivader,INXWARE_FB_ID_StringDivader,StringDivader),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_NumDemultiplexReal,INXWARE_FB_ID_NumDemultiplexReal,NumDemultiplexReal),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_NumMultiplexReal,INXWARE_FB_ID_NumMultiplexReal,NumMultiplexReal),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_RuntimeInfo2,INXWARE_FB_ID_RuntimeInfo2,RuntimeInfo2),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_MapInt,INXWARE_FB_ID_MapInt,MapInt),
// Indexed mux/demux components moved to core/
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_bin2dec8,INXWARE_FB_ID_bin2dec8,bin2dec8),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_sample2str,INXWARE_FB_ID_sample2str,sample2str),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_Unsigned2Int,INXWARE_FB_ID_Unsigned2Int,Unsigned2Int),
// JSON parser components moved to core/
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_SineInt,INXWARE_FB_ID_SineInt,SineInt),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_CosineInt,INXWARE_FB_ID_CosineInt,CosineInt),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_TanInt,INXWARE_FB_ID_TanInt,TanInt),
#ifdef EHS_FILESYSTEM_SUPPORT
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_key_value,INXWARE_FB_ID_key_value,key_value),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_key_value_int,INXWARE_FB_ID_key_value_int,key_value_int),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_key_value_real,INXWARE_FB_ID_key_value_real,key_value_real),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_key_value_bool,INXWARE_FB_ID_key_value_bool,key_value_bool),
#endif

#ifdef EHS_LORAWAN_SUPPORT
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_lorawan,INXWARE_FB_ID_lorawan,lorawan),
#endif

    // WiFi components moved to networking/

#ifdef EHS_AUDIO_INPUT_LEVEL_SUPPORT
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_audio_input_level,INXWARE_FB_ID_audio_input_level,audio_input_level),
#endif

#ifdef EHS_PERIPHERALS_BACKLIGHT_SUPPORT
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_Backlight,INXWARE_FB_ID_Backlight,Backlight),
#endif
#ifdef EHS_PID_SUPPORT
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_PID,INXWARE_FB_ID_PID,PID),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_Calibrate,INXWARE_FB_ID_Calibrate,Calibrate),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_pid_relay_config,INXWARE_FB_ID_pid_relay_config,pid_relay_config),
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_pid_hdwr_config_int,INXWARE_FB_ID_pid_hdwr_config_int,pid_hdwr_config_int),
#endif
/* Devman APIs todo - shouldn't be here !*/
#ifdef EHS_SCHEDULER_SUPPORT
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_DevmanScheduler,INXWARE_FB_ID_DevmanScheduler,DevmanScheduler),
#endif
#if defined(EHS_DEVMAN_SUPPORT) && (EHS_DEVMAN_SUPPORT != EHS_DEVMAN_NONE)
    EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_DevmanInterface, INXWARE_FB_ID_DevmanInterface, DevmanInterface),
#endif

#ifdef EHS_COMPONENTS_CONSOLE_IO
EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_console_print,INXWARE_FB_ID_console_print,console_print),
#endif

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
/* These compoents that need GNU dependencies that wont workon MCUs typically 
 Not sure why it's just destry functions.... possibly this is a hack for something else?*/
 #ifndef EHS_SKIP_GNULIBRARIES
    EHS_BLOCKREF_ENTRY(INXWARE_FB_NAME_ehs_controller ,INXWARE_FB_ID_ehs_controller,ehs_controller),
    #ifndef  EHS_NO_LIBXML2_SUPPORT
        EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_application_info_getter,INXWARE_FB_ID_application_info_getter ,application_info_getter),
    #endif
    EHS_BLOCKREF_ENTRY(INXWARE_FB_NAME_application_run,INXWARE_FB_ID_application_run ,application_run),
    #ifdef EHS_UNITY3D_WIDGETS
        EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_unity2,INXWARE_FB_ID_unity2,unity2),
    #endif
#endif //ifndef EHS_SKIP_GNULIBRARIES
{0}
};
//ICB END BLOCK REF TABLE MACRO -- DO NOT ALTER
