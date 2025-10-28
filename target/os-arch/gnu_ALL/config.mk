# Default dependency and feature support for GNU targets. 

ifneq ($(EHS_FILESYSTEM_SUPPORT),none)
ifndef EHS_FILESYSTEM_SUPPORT
	EHS_FILESYSTEM_SUPPORT=posix
endif
endif

ifneq ($(EHS_COMMS_API_SUPPORT),none)
ifndef EHS_COMMS_API_SUPPORT
	EHS_COMMS_API_SUPPORT=bsdsockets
    DEFS += EHS_COMMS_API_SUPPORT
endif
endif 

#Default to include system console
ifneq ($(EHS_COMPONENTS_SYSTEMEXEC_SUPPORT),none)
ifndef EHS_COMPONENTS_SYSTEMEXEC_SUPPORT
	EHS_COMPONENTS_SYSTEMEXEC_SUPPORT=yes
endif
endif
