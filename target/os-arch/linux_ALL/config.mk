#Linux default dependencies and feature configurations

#Default to http networking enabled for linux-based systems
EHS_NETWORKING_SUPPORT       ?= http
EHS_PERIPHERALS_UART_SUPPORT ?= stubbed

# Default network features for Linux targets
EHS_COMPONENTS_NETWORK_URL_GET ?= enabled
