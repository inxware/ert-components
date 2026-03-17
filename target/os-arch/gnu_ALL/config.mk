# Default dependency and feature support for GNU targets.

EHS_FILESYSTEM_SUPPORT       ?= posix
EHS_PERIPHERALS_UART_SUPPORT ?= stubbed

# Kept as ifneq/ifndef: block contains a second statement (DEFS +=) inside the ifndef
ifneq ($(EHS_COMMS_API_SUPPORT),none)
ifndef EHS_COMMS_API_SUPPORT
	EHS_COMMS_API_SUPPORT=bsdsockets
	DEFS += EHS_COMMS_API_SUPPORT
endif
endif

#Default to include system console
EHS_COMPONENTS_SYSTEMEXEC_SUPPORT ?= yes
