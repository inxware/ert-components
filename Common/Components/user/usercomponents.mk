#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
#---------------------------------------------------------------#
VPATH+=: $(EHS_COMMON_COMPONENTS_PATH)/user
OBJECTS += usercomponents.$(OBJ)
OBJECTS += inx-ehs_controller.$(OBJ)
OBJECTS += inx-application_run.$(OBJ)
ifdef EHS_NETWORKING_SUPPORT
#Socker only support needed:
OBJECTS += inx-inx-netsocket.$(OBJ)
OBJECTS += inx-netsocketrsrvr.$(OBJ)
#full tcpip stack needed e.g. http
ifdef EHS_DEVMAN_SUPPORT
OBJECTS += inx-devman_interface.$(OBJ)
endif
endif
ifndef EHS_SKIP_APPLICATION_INFO_GETTER
OBJECTS += inx-application_info_getter.$(OBJ)
endif
ifndef EHS_SKIP_GNULIBRARIES
include $(EHS_COMMON_HAL_PATH)/json/json.mk
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
# Lets find out why it breaks the NXP build: DEFS += EHS_MQTT_SUPPORT #PBB 2022-10-25 commented out because it breaks the NXP build
    ifeq ($(EHS_MQTT_SUPPORT),lwip)
			#e.g this one should be EHS_MQTT_SUPPORT=<some technology e.g. lwip,...
			DEFS += EHS_MQTT_SUPPORT_LWIP
	else ifeq ($(EHS_MQTT_SUPPORT),esp32)
			#e.g this one should be EHS_MQTT_SUPPORT=<some technology e.g. lwip,...
			DEFS += EHS_MQTT_SUPPORT_ESP32
			LIB += mqtt
			LIB += mbedtls
#			LIB += libesp-tls
    else ifeq ($(EHS_MQTT_SUPPORT),greengrass)
		DEFS += EHS_MQTT_SUPPORT_AWS_GREENGRASS
		ifeq ($(EHS_GNU_OS),mingw32posix)
			LIB += aws-c-common aws-c-cal aws-c-io aws-c-compression aws-c-http aws-c-mqtt
		else
			LIB += dl 	
			LIB += :libaws-c-mqtt.a :libaws-c-http.a :libaws-c-compression.a :libaws-c-io.a :libaws-c-cal.a :libs2n.a :libssl.a :libcrypto.a :libaws-c-common.a
			DEFS += EHS_MQTT_SUPPORT_AWS_GREENGRASS
		endif
	else
# this shouldn't happen!
	endif
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
OBJECTS += inx-modbus_config.$(OBJ)
OBJECTS += inx-modbus_read.$(OBJ)
OBJECTS += inx-modbus_write.$(OBJ)
OBJECTS += inx-uart_config.$(OBJ)
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
endif
ifdef EHS_LORAWAN_SUPPORT
OBJECTS += inx-lorawan.$(OBJ)
endif
ifdef EHS_WIFI_SUPPORT
OBJECTS += inx-wifi_station.$(OBJ)
endif
ifdef EHS_AUDIO_INPUT_LEVEL_SUPPORT
OBJECTS += inx-audio_input_level.$(OBJ)
endif
ifdef EHS_UART_SUPPORT
DEFS += EHS_UART_SUPPORT
OBJECTS += inx-uart.$(OBJ)
endif
OBJECTS += inx-hex2string.$(OBJ)
OBJECTS += inx-Int2HexString.$(OBJ)
ifdef EHS_SCHEDULER_SUPPORT
OBJECTS += inx-scheduler.$(OBJ)
DEFS += EHS_SCHEDULER_SUPPORT
endif
ifdef EHS_PID_SUPPORT
OBJECTS += inx-PID.$(OBJ)
DEFS += EHS_PID_SUPPORT
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
EHS_TOOLBOX_HASHES:=$(EHS_TOOLBOX_HASHES)"0xd12a0e92,"