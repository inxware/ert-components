#default Android dependency and feature configurations

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

# Default network features for Android targets
ifneq ($(EHS_COMPONENTS_NETWORK_URL_GET),none)
ifndef EHS_COMPONENTS_NETWORK_URL_GET
	EHS_COMPONENTS_NETWORK_URL_GET=enabled
endif
endif

#Default to include system console
ifneq ($(EHS_COMPONENTS_SYSTEMEXEC_SUPPORT),none)
ifndef EHS_COMPONENTS_SYSTEMEXEC_SUPPORT
	EHS_COMPONENTS_SYSTEMEXEC_SUPPORT=yes
endif
endif
