# QNX Neutrino default feature configuration — applies to all QNX architectures.
# Architecture-specific config.mk files (e.g. qnx-arm64/config.mk) may
# further override these with ?= assignments.

# Default to HTTP networking on all QNX targets
EHS_NETWORKING_SUPPORT       ?= http
EHS_PERIPHERALS_UART_SUPPORT ?= qnx

# Default network features
EHS_COMPONENTS_NETWORK_URL_GET ?= enabled
