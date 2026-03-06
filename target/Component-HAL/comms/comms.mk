
# This is done globally now
#ifdef  EHS_COMMS_API_SUPPORT
#	ifneq  ($(EHS_COMMS_API_SUPPORT),stubbed)
#		include $(EHS_TARGET_COMMS_API_PATH)/../tcp_server_common/tcp.mk
#	else
#		This is needed to keep the Kernel happy. It may write to stdout if debug tty is available.
#		OBJECTS += stubbed_console.$(OBJ)
#	endif
#endif

ifdef EHS_COMPONENT_NETWORKING_SUPPORT
ifneq ($EHS_COMPONENT_NETWORKING_SUPPORT,none)
LIB += crypto
endif
endif