#---------------------------------------------------------------
# Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk
# inxware ERT configuration file for linux_arm64_lvgl_raspberrypi_debian13-sferalabs
# @author: inx limited

# Raspberry Pi CM4/CM5 with LVGL, Debian 13, and Sfera Labs Strato Pi Max industrial I/O.
# Based on linux_arm64_lvgl_raspberrypi_debian13 with GPIO/ADC/UART overridden to
# the Sfera Labs sysfs HAL targeting the Strato Pi Max (most capable Sfera Labs platform).
#
# Strato Pi Max hardware:
#   - Up to 4 expansion slots, each with 7-ch digital I/O, quad RS-422/485, analog I/O
#   - pin_id encoding: (slot << 8) | channel  (slot 1-4, ch 1-7)
#   - ADC channel encoding: upper nibble = slot, lower nibble = channel within slot
#   - Dual SD, watchdog, UPS power management, USB control, accelerometer


#################################################################################################################
# Baseline Configuration Includes
# Inherit the full RPi debian13 base (LVGL, OpenCV, ML, etc.)
#################################################################################################################

include ./target/platform/linux_arm64_lvgl_raspberrypi_debian13/config.mk


#################################################################################################################
# Architecture, OS, Toolchain and Dependencies
# (Inherited from parent — no changes needed)
#################################################################################################################


#################################################################################################################
# Feature Configuration
# Override peripheral HAL selections to Sfera Labs sysfs backend
#################################################################################################################

#----- Peripheral Features — GPIO -----
# Override the parent's stubbed GPIO with the Sfera Labs sysfs implementation.
# No wiringpi or pigpio library dependency — pure POSIX file I/O on sysfs.
EHS_PERIPHERALS_GPIO_SUPPORT=sferalabs

# PWM is not available via Sfera Labs sysfs; stubbed provides no-op stubs.
EHS_PERIPHERALS_PWM_SUPPORT=stubbed

#----- Peripheral Features — Analog I/O -----
# Strato Pi Max expansion boards provide 24-bit ADC and analog output via sysfs.
EHS_PERIPHERALS_ADC_DAC_SUPPORT=sferalabs

# Strato Pi Max slot capacity (4 slots × up to 4 channels each)
# Upper nibble encodes slot (1-4), lower nibble encodes channel within slot (1-4)
DEFS+=EHS_TARGET_ADC_UNIT_NUMBER=1
DEFS+=EHS_TARGET_ADC_CHANNEL_NUMBER=4

#----- Peripheral Features — Serial / RS-485 / RS-422 -----
# Replaces the always-included UART stub with the Sfera Labs termios implementation.
# TgtUart_RS485Setup() writes to the Strato Pi Max sysfs to enable RS-485 TX-enable control.
EHS_PERIPHERALS_UART_SUPPORT=sferalabs

# Two serial ports: ttyAMA1 (primary RS-485/422), ttyAMA2 (secondary)
DEFS+=EHS_TARGET_UART_COUNT=2
DEFS+=SFERALABS_UART0_DEV=\"/dev/ttyAMA1\"
DEFS+=SFERALABS_UART1_DEV=\"/dev/ttyAMA2\"

#----- Sfera Labs Board Selection -----
# Strato Pi Max is the most capable Sfera Labs platform:
#   - Expandable to 4 I/O expansion board slots
#   - Per-slot: 7-ch industrial digital I/O, quad RS-422, 24-bit ADC/DAC, LTE (M.2)
#   - Base board: watchdog, UPS, dual SD, USB control, accelerometer, secure element
#DEFS+=EHS_SFERALABS_BOARD_STRATOPIMAX
DEFS+=EHS_SFERALABS_BOARD_IONOPI


#################################################################################################################
# Application and Packaging
#################################################################################################################

EHS_DEFAULT_APP=tutorials/hello_world


#################################################################################################################
# Sfera Labs Peripheral HAL Support
# Enable the new peripheral HAL blocks for the Strato Pi Max (and other Sfera Labs boards where available).
#################################################################################################################

EHS_WATCHDOG_SUPPORT=sferalabs
EHS_UPS_SUPPORT=sferalabs
EHS_BUZZER_SUPPORT=sferalabs
EHS_SD_SELECT_SUPPORT=sferalabs
EHS_USB_POWER_SUPPORT=sferalabs
EHS_PERIPHERALS_ACCEL_GYRO_SUPPORT=sferalabs
EHS_RS485_CONFIG_SUPPORT=sferalabs


# LoRaWAN modem support (WIO-E5 via UART/serial)
EHS_LORAWAN_SUPPORT=wio_e5

################################### END OF CONFIGURATION ###################################################
