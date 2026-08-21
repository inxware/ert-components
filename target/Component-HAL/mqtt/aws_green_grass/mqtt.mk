DEFS+=EHS_MQTT_CLIENT_FB_THREAD=1

ifeq ($(EHS_GNU_OS),mingw32posix)
LIB += aws-c-common 
LIB += aws-c-cal 
LIB += aws-c-io 
LIB += aws-c-compression 
LIB += aws-c-http 
LIB += aws-c-mqtt
else
LIB += dl
# Link the AWS SDK and BoringSSL/AWS-LC static libraries using absolute paths inside a
# linker archive group.  Absolute paths guarantee the middleware libraries are used
# (avoiding any -L search-path ambiguity with system OpenSSL), and --start-group /
# --end-group makes the linker iterate over the group until all cross-archive symbol
# references are resolved, regardless of the order the symbols are first encountered.
#
# --exclude-libs hides every symbol from these archives out of the executable's
# .dynsym.  Without it AWS-LC's CRYPTO_*/SSL_* end up exported with default
# visibility; the system libssl.so.3 (pulled in by libcurl) then resolves its
# crypto callbacks back into the executable's AWS-LC copies — incompatible
# struct layouts, crash in pthread_rwlock_wrlock with a garbage pointer.
LNKFLAGS += -Wl,--start-group \
    $(EHS_COMPONENT_SUPPORT_LIBS)libaws-c-mqtt.a \
    $(EHS_COMPONENT_SUPPORT_LIBS)libaws-c-http.a \
    $(EHS_COMPONENT_SUPPORT_LIBS)libaws-c-compression.a \
    $(EHS_COMPONENT_SUPPORT_LIBS)libaws-c-io.a \
    $(EHS_COMPONENT_SUPPORT_LIBS)libaws-c-cal.a \
    $(EHS_COMPONENT_SUPPORT_LIBS)libs2n.a \
    $(EHS_COMPONENT_SUPPORT_LIBS)libssl.a \
    $(EHS_COMPONENT_SUPPORT_LIBS)libcrypto.a \
    $(EHS_COMPONENT_SUPPORT_LIBS)libaws-c-common.a \
    -Wl,--end-group \
    -Wl,--exclude-libs,libaws-c-mqtt.a:libaws-c-http.a:libaws-c-compression.a:libaws-c-io.a:libaws-c-cal.a:libs2n.a:libssl.a:libcrypto.a:libaws-c-common.a

endif

OBJECTS += mqtt.$(OBJ)
