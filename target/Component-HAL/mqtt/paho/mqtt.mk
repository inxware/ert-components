# Paho MQTT HAL — Eclipse Paho MQTT C async client with TLS.
#
# Library: libpaho-mqtt3as (async + SSL/TLS)
# libpaho-mqtt-c-dev:arm64 is not available as a cross-arch apt package.
# It must be cross-compiled from source in the Docker build image.
# See the Dockerfile for the target platform for the build steps.
# Current targets using this HAL: linux_arm64_debian13_base (and derivatives), Jetson.
#
# The 'as' variant (MQTTAsync + SSL) is selected to support TLS/mTLS connections
# to AWS IoT Core and other TLS-only brokers.  Plain TCP brokers also work.

DEFS += EHS_MQTT_CLIENT_FB_THREAD=1

OBJECTS += mqtt.$(OBJ)

LIB += paho-mqtt3as
