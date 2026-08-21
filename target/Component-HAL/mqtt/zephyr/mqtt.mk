#---------------------------------------------------------------
# Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
# LGPLv3 license — see https://www.gnu.org/licenses/lgpl-3.0.txt
#---------------------------------------------------------------

# MQTT HAL over Zephyr's in-tree socket MQTT library.
#
# Applies to any Zephyr target, not just nRF91: the library talks BSD sockets,
# so it works over the nRF91's offloaded modem sockets and over Zephyr's native
# stack on an nRF70 Wi-Fi board without a line of difference here.

OBJECTS += mqtt.$(OBJ)

# Make the mqtt_client function block own the client thread.
#
# Without this the loop is never entered: EhsMqttClientLoop is only called from
# inx-mqtt_client.c's EHS_FB_START_THREAD, which is compiled in under this
# define (the ESP backends leave it off because an IDF task calls the loop
# instead). Same setting as the paho and aws_green_grass backends.
#
# COSTS A DYNAMIC THREAD SLOT: the FB calls EHS_FB_START_THREAD, which draws
# from target_process.c's pool of EHS_ZEPHYR_MAX_DYNAMIC_THREADS stacks. That
# pool is statically allocated BSS, so a platform trimmed to the exact number
# of threads it spawns must be given one more before enabling MQTT.
DEFS += EHS_MQTT_CLIENT_FB_THREAD=1

########################################################################################################
## Zephyr Kconfig
########################################################################################################
# ERT_ZEPHYR_KCONFIG is consumed when the zephyr_cmake_gen recipe runs, so
# appending from here — parsed via component-hal.mk during make's read phase —
# lands in the generated prj.conf. Same mechanism as wifi_nrf70.mk.
#
# MQTT_LIB 'select's NET_SOCKETS, so the platform does not have to. It does NOT
# depend on NET_NATIVE, which is what makes it usable on the cellular variant
# where Zephyr's own TCP/IP stack is compiled out entirely.
ERT_ZEPHYR_KCONFIG += CONFIG_MQTT_LIB=y

# Broker keepalive in SECONDS. mqtt_live() sends PINGREQ on this schedule and
# the backend's 100 ms loop tick drives it, so any value well above that tick
# works. Kept at the Zephyr default; a metered cellular link might justify
# raising it, since each PINGREQ/PINGRESP is billable traffic. Override from a
# platform config.mk with  ERT_MQTT_KEEPALIVE = 300.
ERT_MQTT_KEEPALIVE ?= 60
ERT_ZEPHYR_KCONFIG += CONFIG_MQTT_KEEPALIVE=$(ERT_MQTT_KEEPALIVE)

# TLS off. The backend refuses a TLS connect request rather than downgrading it
# (see the TLS note in mqtt.c): CONFIG_MQTT_LIB_TLS pulls mbedTLS in, and the
# nRF91 modem's native TLS uses security tags rather than the PEM filenames the
# mqtt_client function block's parameters carry, so enabling this symbol alone
# would not produce a working mTLS client anyway.
ERT_ZEPHYR_KCONFIG += CONFIG_MQTT_LIB_TLS=n
ERT_ZEPHYR_KCONFIG += CONFIG_MQTT_LIB_WEBSOCKET=n

# Fresh session on every connect. The alternative (persistent session) has the
# broker queue QoS 1 messages while the device is away and deliver the backlog
# on reconnect — which on a device that reboots into a new SODL means a burst
# of messages addressed to an application that no longer exists.
ERT_ZEPHYR_KCONFIG += CONFIG_MQTT_CLEAN_SESSION=y
