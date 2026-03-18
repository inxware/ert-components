# Paho MQTT HAL — Eclipse Paho MQTT C async client with TLS.
#
# Library: libpaho-mqtt3as (async + SSL/TLS)
# Available from apt on all supported Linux targets — no ert-contrib-middleware build required:
#
#   Ubuntu 22.04:   apt install libpaho-mqtt-c-dev
#   Debian 12/13:   apt install libpaho-mqtt-c-dev
#   Raspberry Pi OS: apt install libpaho-mqtt-c-dev
#
# The 'as' variant (MQTTAsync + SSL) is selected to support TLS/mTLS connections
# to AWS IoT Core and other TLS-only brokers.  Plain TCP brokers also work.

DEFS += EHS_MQTT_CLIENT_FB_THREAD=1

OBJECTS += mqtt.$(OBJ)

LIB += paho-mqtt3as
