
VPATH+=: $(EHS_COMMON_COMPONENTS_PATH)/user
OBJECTS += usercomponents.$(OBJ)

OBJECTS += inx-ehs_controller.$(OBJ)

ifdef EHS_NETWORKING_SUPPORT
OBJECTS += inx-application_run.$(OBJ)
OBJECTS += inx-inx-netsocket.$(OBJ)
OBJECTS += inx-application_info_getter.$(OBJ)
OBJECTS += inx-devman_interface.$(OBJ)
OBJECTS += inx-netsocketrsrvr.$(OBJ)
endif

ifndef EHS_SKIP_GNULIBRARIES
include $(EHS_COMMON_HAL_PATH)/json/json.mk
OBJECTS += inx-json_stream.$(OBJ)
OBJECTS += inx-xml_stream.$(OBJ)
OBJECTS += inx-JSONObjectFunctionBlock.$(OBJ)

#@todo this is for the xml stream parser - should be out somewhere more sensible..
ifeq ($(EHS_GNU_OS),$(filter $(EHS_GNU_OS),mingw32msvc w64-mingw32))
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

ifdef EHS_WEBKIT_SUPPORT
OBJECTS += inx-webkit.$(OBJ)
endif
ifdef EHS_UNITY3D_WIDGETS
OBJECTS += inx-unity2.$(OBJ)
endif
ifdef EHS_MQTT_SUPPORT
	ifdef EHS_LWIP
	else
		LIB+=dl
		LIB+=:libaws-c-common.a :libssl.a :libcrypto.a :libaws-c-compression.a :libaws-c-mqtt.a :libaws-c-io.a :libs2n.a :libaws-c-http.a :libaws-c-cal.a
		#aws-c-compression crypto s2n ssl aws-c-io aws-c-http
		#LIB+=aws-c-mqtt 
	endif
OBJECTS += inx-mqtt_client.$(OBJ)
OBJECTS += inx-mqtt_publish.$(OBJ)
OBJECTS += inx-mqtt_subscribe.$(OBJ)
endif
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
endif
