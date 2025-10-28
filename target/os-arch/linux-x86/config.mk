########################################################################
#  Default dependencies and feature support for Linux x86 targets.
########################################################################

# Default to stubbing this for targets that don't declare
ifneq ($(EHS_PERIPHERALS_GPIO_SUPPORT),none)
ifndef EHS_PERIPHERALS_GPIO_SUPPORT
   EHS_PERIPHERALS_GPIO_SUPPORT=stubbed
endif
endif

ifneq ($(EHS_IMAGEPROCESSING_APRILTAG_SUPPORT),none)
ifndef	EHS_IMAGEPROCESSING_APRILTAG_SUPPORT
	EHS_IMAGEPROCESSING_APRILTAG_SUPPORT=enabled
endif
endif