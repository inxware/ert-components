#Linux default dependencies and feature configurations

#Default to http networking enabled for linux-based systems
ifneq ($(EHS_NETWORKING_SUPPORT),none)
ifndef EHS_NETWORKING_SUPPORT
	EHS_NETWORKING_SUPPORT=http
endif
endif

# Default network features for Linux targets
ifneq ($(EHS_COMPONENTS_NETWORK_URL_GET),none)
ifndef EHS_COMPONENTS_NETWORK_URL_GET
	EHS_COMPONENTS_NETWORK_URL_GET=enabled
endif
endif