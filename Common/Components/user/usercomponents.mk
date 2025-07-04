#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#
VPATH+=: $(EHS_COMMON_COMPONENTS_PATH)/user
OBJECTS += usercomponents.$(OBJ)
OBJECTS += inx-ehs_controller.$(OBJ)
OBJECTS += inx-application_run.$(OBJ)#


# TODO2025 we need to use the fllowin construct for all entries here rather a million different ones...
#ifdef EHS_XXXXXX_SUPPORT
#ifneq ($(EHS_XXXXX_SUPPORT),none)
# ...
#endif
#endif


ifdef EHS_DEVMAN_SUPPORT
ifneq ($(EHS_DEVMAN_SUPPORT),none)
	DEFS += EHS_DEVMAN_SUPPORT
	OBJECTS += inx-devman_interface.$(OBJ)
endif
else
# EHS_DEVMAN_SUPPORT - is too generic, as it includes player, media etc. we need something
# more graniual for devman mon interface, which can be used by mcu via mqtt
ifeq ($(EHS_DEVMAN_MON_SUPPORT),mqtt)
	OBJECTS += inx-devman_interface.$(OBJ)
endif
endif

# @TODO - This all needs to go into the networking folder.
# EHS_NETWORKING_SUPPORT or EHS_COMPONENTS_NETWORK_TCPIP_SOCKET
ifneq ($(EHS_NETWORKING_SUPPORT)$(EHS_COMPONENTS_NETWORK_TCPIP_SOCKET),)
#Thsis actually seems to mean HTTP not networking...TODO2025 refactor this to say what it is.
#Socket only support needed:
	OBJECTS += inx-inx-netsocket.$(OBJ)
	OBJECTS += inx-netsocketrsrvr.$(OBJ)
	DEFS += EHS_COMPONENTS_NETWORK_TCPIP_SOCKET
else
endif

# @TODO - move to PERIPHERALS or other toolbox
ifeq ($(EHS_PERIPHERALS_BACKLIGHT_SUPPORT),)
EHS_PERIPHERALS_BACKLIGHT_SUPPORT=stubbed
endif
ifdef EHS_PERIPHERALS_BACKLIGHT_SUPPORT
# backlight is using stub if not implementd for target, so should be safe to just define it
DEFS += EHS_PERIPHERALS_BACKLIGHT_SUPPORT=1
endif

#TODO2024 - This doesn't look like itfollows our new pattern? (ifeq (....))
ifndef EHS_SKIP_APPLICATION_INFO_GETTER

#TODO2024 = this CMLS should be of the form EHS_LIBXML_SUPPORT=<xml2,none,...>
ifndef EHS_NO_LIBXML2_SUPPORT
OBJECTS += inx-application_info_getter.$(OBJ)
endif
endif

include $(EHS_COMMON_HAL_PATH)/json/json.mk

ifndef EHS_SKIP_GNULIBRARIES
OBJECTS += inx-json_stream.$(OBJ)
OBJECTS += inx-JSONObjectFunctionBlock.$(OBJ)
ifndef EHS_EXCLUDE_XML_PARSER
OBJECTS += inx-xml_stream.$(OBJ)
#@todo this is for the xml stream parser - should be out somewhere more sensible..
#ifeq ($(EHS_GNU_OS),$(filter $(EHS_GNU_OS),mingw32msvc w64-mingw32))
ifeq ($(EHS_GNU_OS),mingw32posix)
LIB+= expat
else
ifeq ($(SYSTEM_VARIANT),RASPBERRYPI)
LIB+= expatw
LIB+= ssl
else
ifdef EHS_ANDROID
LIB+=:libexpat.a
else
LIB+= expat
endif
endif
endif
else
DEFS += EHS_EXCLUDE_XML_PARSER=1
endif
endif
#todo2022 move these components into an appropriate toolbox (see grand plan document)
#todo2022 make all these #defs sane
ifdef EHS_WEBKIT_SUPPORT
OBJECTS += inx-webkit.$(OBJ)
endif
ifdef EHS_MQTT_SUPPORT
	DEFS += EHS_MQTT_SUPPORT
	OBJECTS += inx-mqtt_client.$(OBJ)
	OBJECTS += inx-mqtt_publish.$(OBJ)
	OBJECTS += inx-mqtt_subscribe.$(OBJ)
endif
# OBJECTS += inx-mqtt_client.$(OBJ)
# OBJECTS += inx-mqtt_publish.$(OBJ)
# OBJECTS += inx-mqtt_subscribe.$(OBJ)
#todo2022: These need moving to the peripherals toolbox and shoiuld be stubbed out for targets without the hardware that use this toolbox.
ifdef EHS_PERIPHERALS_ADC_DAC_SUPPORT
OBJECTS += inx-adc_read.$(OBJ)
endif
#todo - the following NXP specific blocks should be implemented for other targets a part of the peripherial (or other toolboxes).

ifdef EHS_NXP_SUPPORT
OBJECTS += inx-rtc.$(OBJ)
OBJECTS += inx-pwm.$(OBJ)
OBJECTS += inx-numeric_display_char.$(OBJ)
OBJECTS += inx-permanent_storage.$(OBJ)

else ifdef EHS_ESP32_SUPPORT
#OBJECTS += inx-rtc.$(OBJ)
#OBJECTS += inx-pwm.$(OBJ)
#OBJECTS += inx-numeric_display_char.$(OBJ)
#OBJECTS += inx-modbus_config.$(OBJ)
#OBJECTS += inx-modbus_read.$(OBJ)
#OBJECTS += inx-modbus_write.$(OBJ)
#OBJECTS += inx-uart_config.$(OBJ)
#OBJECTS += inx-permanent_storage.$(OBJ)
else


endif

ifdef EHS_LORAWAN_SUPPORT
OBJECTS += inx-lorawan.$(OBJ)
endif
ifeq ($(EHS_WIFI_SUPPORT),yes)
DEFS += EHS_WIFI_SUPPORT
OBJECTS += inx-wifi_station.$(OBJ)
endif
ifdef EHS_AUDIO_INPUT_LEVEL_SUPPORT
OBJECTS += inx-audio_input_level.$(OBJ)
endif
#ifdef EHS_UART_SUPPORT
#DEFS += EHS_UART_SUPPORT=1
OBJECTS += inx-uart.$(OBJ)
OBJECTS += inx-uart_config.$(OBJ)
#else
#DEFS += EHS_UART_SUPPORT=0
#endif
OBJECTS += inx-hex2string.$(OBJ)
OBJECTS += inx-Int2HexString.$(OBJ)
ifdef EHS_SCHEDULER_SUPPORT
OBJECTS += inx-scheduler.$(OBJ)
DEFS += EHS_SCHEDULER_SUPPORT
endif
ifdef EHS_PID_SUPPORT
OBJECTS += inx-PID.$(OBJ)
OBJECTS += inx-calibrate.$(OBJ)
OBJECTS += inx-pid_relay_config.$(OBJ)
OBJECTS += inx-pid_hdwr_config_int.$(OBJ)
DEFS += EHS_PID_SUPPORT=$(EHS_PID_SUPPORT)
endif
ifndef EHS_OTA_SUPPORT
EHS_OTA_SUPPORT=stubbed
endif
ifeq ($(EHS_OTA_SUPPORT),stubbed)
OBJECTS += ota_hal_stubb.$(OBJ)
endif
OBJECTS += inx-ota.$(OBJ)
OBJECTS += inx-ota_data_parser.$(OBJ)
OBJECTS += inx-reboot.$(OBJ)
OBJECTS += inx-cgi2json.$(OBJ)
OBJECTS += inx-stringdivader.$(OBJ)
#TODO Change this to stub version later
ifdef EHS_PERIPHERALS_BACKLIGHT_SUPPORT
OBJECTS += inx-display_backlight.$(OBJ)
endif
ifdef EHS_PERIPHERALS_ADC_DAC_SUPPORT
OBJECTS += inx-dac.$(OBJ)
endif
ifdef EHS_MODBUS_SUPPORT
DEFS+=EHS_MODBUS_SUPPORT
OBJECTS += inx-modbus_config.$(OBJ)
OBJECTS += inx-modbus_read.$(OBJ)
OBJECTS += inx-modbus_write.$(OBJ)
OBJECTS += inx-modbus_slave_register.$(OBJ)
endif
OBJECTS += inx-num_demux.$(OBJ)
OBJECTS += inx-num_mux.$(OBJ)
OBJECTS += inx-rtinfo2.$(OBJ)
OBJECTS += inx-map_int.$(OBJ)
OBJECTS += inx-indexed_mux_int.$(OBJ)
OBJECTS += inx-binary2decimal8.$(OBJ)
OBJECTS += inx-indexed_mux_str.$(OBJ)
OBJECTS += inx-sample2str.$(OBJ)
OBJECTS += inx-key_value.$(OBJ)
OBJECTS += inx-indexed_demux_int.$(OBJ)
OBJECTS += inx-unsigned2int.$(OBJ)
OBJECTS += inx-json_parser_int.$(OBJ)
OBJECTS += inx-json_parser_bool.$(OBJ)
OBJECTS += inx-json_parser_real.$(OBJ)
OBJECTS += inx-indexed_mux_bool.$(OBJ)
OBJECTS += inx-indexed_mux_real.$(OBJ)
OBJECTS += inx-indexed_demux_bool.$(OBJ)
OBJECTS += inx-indexed_demux_real.$(OBJ)
OBJECTS += inx-indexed_demux_str.$(OBJ)
OBJECTS += inx-SineInt.$(OBJ)
OBJECTS += inx-CosineInt.$(OBJ)
OBJECTS += inx-TanInt.$(OBJ)
OBJECTS += inx-json_parser_str.$(OBJ)
OBJECTS += inx-key_value_int.$(OBJ)
OBJECTS += inx-key_value_real.$(OBJ)
OBJECTS += inx-key_value_bool.$(OBJ)

ifdef EHS_PERIPHERALS_ADC_DAC_SUPPORT
OBJECTS += inx-adc_config.$(OBJ)
	ifeq ($(EHS_PERIPHERALS_ADC_CONTINUOUS_SUPPORT),none)
		DEFS += EHS_PERIPHERALS_ADC_CONTINUOUS_SUPPORT__NONE
	else
		OBJECTS += inx-adc_read_continuous.$(OBJ)
	endif
	OBJECTS += inx-adc_read_single.$(OBJ)
endif

ifdef EHS_COMPONENTS_CONSOLE_IO
	OBJECTS += inx-console_print.$(OBJ)
	DEFS += EHS_COMPONENTS_CONSOLE_IO
endif

ifdef EHS_PERIPHERALS_PWM_SUPPORT
ifneq ($(EHS_PERIPHERALS_PWM_SUPPORT),none)
	OBJECTS += inx-pwm_gen.$(OBJ)
endif
endif

EHS_TOOLBOX_HASHES:=$(EHS_TOOLBOX_HASHES)"0x583cfb49,"
