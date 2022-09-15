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

OBJECTS += inx-application_info_getter.$(OBJ)

ifndef EHS_SKIP_GNULIBRARIES
include $(EHS_COMMON_HAL_PATH)/json/json.mk
OBJECTS += inx-json_stream.$(OBJ)
OBJECTS += inx-xml_stream.$(OBJ)
OBJECTS += inx-JSONObjectFunctionBlock.$(OBJ)

#@todo this is for the xml stream parser - should be out somewhere more sensible..
#ifeq ($(EHS_GNU_OS),$(filter $(EHS_GNU_OS),mingw32msvc w64-mingw32))
ifeq ($(EHS_GNU_OS),mingw32msvc)
LIB+= expat-1
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

endif
#todo2022 move these components into an appropriate toolbox (see grand plan document)
#todo2022 make all these #defs sane

ifdef EHS_WEBKIT_SUPPORT
OBJECTS += inx-webkit.$(OBJ)
endif
ifdef EHS_UNITY3D_WIDGETS
OBJECTS += inx-unity2.$(OBJ)
endif
ifdef EHS_MQTT_SUPPORT
	DEFS += EHS_MQTT_SUPPORT
    ifeq ($(EHS_MQTT_SUPPORT),lwip)
#e.g this one should be EHS_MQTT_SUPPORT=<some technology e.g. lwip,...
		DEFS += EHS_MQTT_SUPPORT_LWIP
    else
		LIB+=dl 
		LIB+=:libaws-c-common.a :libssl.a :libcrypto.a :libaws-c-compression.a :libaws-c-mqtt.a :libaws-c-io.a :libs2n.a :libaws-c-http.a :libaws-c-cal.a :libaws-c-http.a :libaws-c-io.a :libdecrepit.a
		LIB += crypto ## add the gnu crypto for curl

	endif
	OBJECTS += inx-mqtt_client.$(OBJ)
	OBJECTS += inx-mqtt_publish.$(OBJ)
	OBJECTS += inx-mqtt_subscribe.$(OBJ)
endif

#todo2022: The following should be keyed off the os-arch paramter and existing "SPECIFIC_TARGET" 
ifdef EHS_NXP_SUPPORT
OBJECTS += inx-adc_read.$(OBJ)
OBJECTS += inx-rtc.$(OBJ)
OBJECTS += inx-pwm.$(OBJ)
OBJECTS += inx-numeric_display_char.$(OBJ)
OBJECTS += inx-modbus_config.$(OBJ)
OBJECTS += inx-modbus_read.$(OBJ)
OBJECTS += inx-modbus_write.$(OBJ)
OBJECTS += inx-uart_config.$(OBJ)
OBJECTS += inx-permanent_storage.$(OBJ)
else ifdef EHS_ESP32_SUPPORT
OBJECTS += inx-adc_read.$(OBJ)
#OBJECTS += inx-rtc.$(OBJ)
#OBJECTS += inx-pwm.$(OBJ)
#OBJECTS += inx-numeric_display_char.$(OBJ)
#OBJECTS += inx-modbus_config.$(OBJ)
#OBJECTS += inx-modbus_read.$(OBJ)
#OBJECTS += inx-modbus_write.$(OBJ)
#OBJECTS += inx-uart_config.$(OBJ)
#OBJECTS += inx-permanent_storage.$(OBJ)

endif
