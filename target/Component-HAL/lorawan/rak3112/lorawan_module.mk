# LoRaWAN module: RAK3112 (ESP32-S3 + SX1262)
#
# Wraps SX126x-Arduino (beegee-tokyo) LoRaWAN stack on top of arduino-esp32
# — both are built by ert-contrib-middleware/inx_build_scripts/
# build-esp32s3-n8r8-freertos-ehs-idf-5-1-x.sh and installed into
# $(EHS_COMPONENT_SUPPORT_INCLUDE)/{arduino,sx126x-arduino}/ plus
# $(EHS_COMPONENT_SUPPORT_LIBS)/{libarduino.a,libsx126x-arduino.a}.
#
# Only valid on n8r8 (the OS variant that carries the Arduino build);
# ordinary esp32s3 variants don't have these archives.

ifneq ($(EHS_OS),esp32s3_freertos)
$(error LoRaWAN rak3112 requires EHS_OS=esp32s3_freertos)
endif
ifeq ($(COMPONENT_VARIANT),)
$(error LoRaWAN rak3112 requires COMPONENT_VARIANT=n8r8 (or compatible variant shipping libarduino.a))
endif

# C++ source; the build rules handle .cpp → .$(OBJ) the same way mv/opencv does.
OBJECTS  += lorawan-rak3112.$(OBJ)
CPPFLAGS += -std=gnu++17

# Upstream Arduino headers use `String`, `millis()`, `delay()` etc. which need
# the ESP32 Arduino core on the include path. SX126x-Arduino's own headers
# live next door — both trees were extracted by the contrib build script.
INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)/arduino/
INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)/arduino/variants/esp32s3/
INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)/arduino/libraries/SPI/
INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)/arduino/libraries/Ticker/
INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)/sx126x-arduino/

# Archives are extracted as -larduino and -lsx126x-arduino; the order matters
# because sx126x-arduino references Arduino core symbols (Serial, SPI, Ticker).
LIB += sx126x-arduino
LIB += arduino
