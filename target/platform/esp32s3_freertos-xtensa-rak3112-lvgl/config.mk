#---------------------------------------------------------------
# Copyright (C) 2026, inx limited, UK.
# All Rights Reserved.  LGPLv3 — see https://www.gnu.org/licenses/lgpl-3.0.txt
#---------------------------------------------------------------
#
# RAK3112 (ESP32-S3 + SX1262 LoRa) with an LVGL display.
#
# Identical to esp32s3_freertos-xtensa-rak3112 except that the GUI is enabled.
# The plain variant stays headless: it is the shipping LoRa configuration and
# should not grow a display it may not have wired up.
#
# Memory is not the constraint. RAK3112 is an n8r8 part (8 MB flash, 8 MB PSRAM),
# where the base LVGL targets run on n8r2 with a quarter of the PSRAM.
#
# Display driver: ft81x, matching esp32s3_freertos-xtensa-base and the existing
# LoRa-plus-display demo (esp32s3_freertos-xtensa-demo-loradisp). It is a single
# line to change if the attached panel differs -- graphics.mk also accepts
# gc9a01, st7701, st7789, ili9341/9481/9486/9488, ssd1306 and sh1107.

include ./target/platform/esp32s3_freertos-xtensa-rak3112/config.mk

#----- GUI Features -----
EHS_GUI_SUPPORT=lvgl
EHS_LVGL_DISPLAY_DRIVER=ft81x
# Touch must be set too, not just display. graphics.mk adds
# lvgl_touch/tp_spi.o unconditionally whenever LVGL is on, and that file needs
# CONFIG_LV_TOUCH_SPI_CS, which only exists once a touch driver is selected --
# so display-only does not build. FT81x is a combined display+touch controller,
# so the pair is correct here rather than a workaround; both esp32s3 LVGL base
# targets set them identically.
EHS_LVGL_TOUCH_DRIVER=ft81x

# The porting guide notes LVGL targets may need 'yes' here: in-place teardown
# can crash the render thread on app reload. Inherited as 'no' from the plain
# variant; set to 'yes' if a reload hangs on real hardware.
# EHS_TARGET_APPLOAD_RESTARTING_REBOOT=yes

EHS_APPLAND_INST_DEPLOY_NAME=Xtensa-ESP32-S3-RAK3112-LVGL
EHS_APPLAND_INST_OS_NAME=xtensa-esp32-s3-rak3112-lvgl
