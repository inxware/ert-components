#default dependencies and feature support for AMD64 build targets targets.

ifneq ($(EHS_IMAGEPROCESSING_APRILTAG_SUPPORT),none)
ifndef	EHS_IMAGEPROCESSING_APRILTAG_SUPPORT
	EHS_IMAGEPROCESSING_APRILTAG_SUPPORT=enabled
endif
endif
