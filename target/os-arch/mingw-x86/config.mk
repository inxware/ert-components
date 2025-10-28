
#Default dependencies and feature support for MinGW x86 targets.

ifneq ($(EHS_COMMS_API_SUPPORT),none)
ifndef  EHS_COMMS_API_SUPPORT
   EHS_COMMS_API_SUPPORT=winsock
endif
endif

# Default network features for Windows targets
ifneq ($(EHS_COMPONENTS_NETWORK_URL_GET),none)
ifndef EHS_COMPONENTS_NETWORK_URL_GET
	EHS_COMPONENTS_NETWORK_URL_GET=enabled
endif
endif

ifneq ($(EHS_IMAGEPROCESSING_APRILTAG_SUPPORT),none)
ifndef	EHS_IMAGEPROCESSING_APRILTAG_SUPPORT
	EHS_IMAGEPROCESSING_APRILTAG_SUPPORT=enabled
endif
endif