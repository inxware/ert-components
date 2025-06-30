DEFS+=EHS_MQTT_CLIENT_FB_THREAD=1

ifeq ($(EHS_GNU_OS),mingw32posix)
LIB += aws-c-common aws-c-cal aws-c-io aws-c-compression aws-c-http aws-c-mqtt
else
LIB += dl 	
LIB += :libaws-c-mqtt.a :libaws-c-http.a :libaws-c-compression.a :libaws-c-io.a :libaws-c-cal.a :libs2n.a :libssl.a :libcrypto.a :libaws-c-common.a
endif

OBJECTS += mqtt.$(OBJ)