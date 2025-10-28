#Arduino default dependency and feature configurations

#For now we will stubb all file systems on ardino by default.
ifneq ($(EHS_FILESYSTEM_SUPPORT),none)
ifndef EHS_FILESYSTEM_SUPPORT
	EHS_FILESYSTEM_SUPPORT=stubbed
endif
endif
