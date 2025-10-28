########################################################################
#  Default depemdnencies and feature support for Linux amr64 targets.
########################################################################

# Default to stubbing this for targets that don't declare a choice
ifneq ($(EHS_PERIPHERALS_GPIO_SUPPORT),none)
ifndef EHS_PERIPHERALS_GPIO_SUPPORT
   EHS_PERIPHERALS_GPIO_SUPPORT=stubbed
endif
endif

ifneq ($(EHS_PERIPHERAL_DEVICE_SUPPORT),none)
ifndef EHS_PERIPHERAL_DEVICE_SUPPORT
   EHS_PERIPHERAL_DEVICE_SUPPORT=all
endif
endif

ifneq ($(EHS_IMAGEPROCESSING_APRILTAG_SUPPORT),none)
ifndef	EHS_IMAGEPROCESSING_APRILTAG_SUPPORT
	EHS_IMAGEPROCESSING_APRILTAG_SUPPORT=enabled
endif
endif