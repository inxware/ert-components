#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

#
# Makefile fragment to build the GUI component code for EHS.

# Called by ../components.mk
#

# @author: inx limited
#
#
# Predefined variables

#  OBJ - File extension for object files

# Expected variables

#  OBJECTS - list of object files added by this makefile
#  VPATH - where to look for source code

#TODO2025  - Please make these follow the template used in usercomponents.mk rather being slightly different everywhere

INC_DIRS+=: $(EHS_COMMON_COMPONENTS_PATH)/peripherals
#INC_DIRS+=$(EHS_COMMON_TOOLKIT_PNG_PATH)
VPATH+=: $(EHS_COMMON_COMPONENTS_PATH)/peripherals

include $(EHS_COMMON_COMPONENTS_PATH)/peripherals/deps.mk
#@todo need proper profile labels for this part 
OBJECTS += peripherals_components.$(OBJ)

#todo2022 we need to split the peripheraks toolbox as per the DCC docs where UI peripherals are seperted from MCU IO.
ifdef EHS_PERIPHERALS_RCU
	DEFS+= EHS_PERIPHERALS_RCU
	OBJECTS += dtv_remote.$(OBJ)
endif

ifdef EHS_PERIPHERALS_GUI_KEYBOARD
	DEFS+= EHS_PERIPHERALS_GUI_KEYBOARD
	OBJECTS += keypress.$(OBJ)
endif

ifdef EHS_PERIPHERALS_USBLCD
	INC_DIRS += $(EHS_TARGET_OS_HW_PATH)/textdisplay #@todo this should be moved when win support is available
	DEFS+= EHS_PERIPHERALS_USBLCD
	OBJECTS += guitextdisplay.$(OBJ)
endif

ifdef EHS_PERIPHERALS_LEDS
	DEFS+= EHS_PERIPHERALS_LEDS
	OBJECTS += guileds.$(OBJ)
endif


ifdef EHS_PERIPHERALS_GPIO_SUPPORT
ifneq ($(EHS_PERIPHERALS_GPIO_SUPPORT),none)
#$(info  $$EHS_PERIPHERALS_GPIO_SUPPORT=[$(EHS_PERIPHERALS_GPIO_SUPPORT)])
#if the hardware doesn't have real GPIO then stubb it if the peripheral toolbox is still needed
# include the commone GPIO components in the toolbox. 
	OBJECTS += gpio_out.$(OBJ)
	OBJECTS += gpio_in.$(OBJ)     
endif
endif


#Some platform like ARDUINO have specific RGV LED APIs we might want to use sometimes.
ifndef EHS_PERIPHERALS_LED_SUPPORT
#NOTE!!! For now we will stubb all platforms if they do not declare a preference so they all build and run any app.
EHS_PERIPHERALS_LED_SUPPORT=stubbed
endif

ifdef EHS_PERIPHERALS_LED_SUPPORT
ifneq ($(EHS_PERIPHERALS_LED_SUPPORT),none)
	OBJECTS += inx-led.$(OBJ)
#todo2025- the following should just be another HAL stub and we don't need to differentiate stubbed from implemented here...
	ifeq ($(EHS_PERIPHERALS_LED_SUPPORT),stubbed)
		OBJECTS += inx-led_hal_stubb.$(OBJ)
	endif
	DEFS += EHS_PERIPHERALS_LED_SUPPORT
endif
endif

#Acceleromter support
# temporarrilly  do this ...
ifndef EHS_PERIPHERALS_ACCEL_GYRO_SUPPORT
EHS_PERIPHERALS_ACCEL_GYRO_SUPPORT=stubbed
endif

ifdef EHS_PERIPHERALS_ACCEL_GYRO_SUPPORT
ifneq ($(EHS_PERIPHERALS_ACCEL_GYRO_SUPPORT),none)
	OBJECTS += inx-accel_gyro.$(OBJ)
#todo2025- the following should just be another HAL stub and we don't need to differentiate stubbed from implemented here...

	ifeq ($(EHS_PERIPHERALS_ACCEL_GYRO_SUPPORT),stubbed)
		OBJECTS += inx-accel_gyro_stubb.$(OBJ)
	endif
	DEFS += EHS_PERIPHERALS_ACCEL_GYRO_SUPPORT
endif
endif

# Components moved from user/ directory
ifdef EHS_PERIPHERALS_ADC_DAC_SUPPORT
ifneq ($(EHS_PERIPHERALS_ADC_DAC_SUPPORT),none)
ifneq ($(EHS_PERIPHERALS_ADC_DAC_SUPPORT),)
	OBJECTS += inx-adc_read.$(OBJ)
	OBJECTS += inx-adc_config.$(OBJ)
	OBJECTS += inx-dac.$(OBJ)
	ifdef EHS_PERIPHERALS_ADC_CONTINUOUS_SUPPORT
	ifneq ($(EHS_PERIPHERALS_ADC_CONTINUOUS_SUPPORT),none)
		OBJECTS += inx-adc_read_continuous.$(OBJ)
		DEFS += EHS_PERIPHERALS_ADC_CONTINUOUS_SUPPORT
	endif
	endif
	OBJECTS += inx-adc_read_single.$(OBJ)
endif
endif
endif

ifdef EHS_PERIPHERALS_PWM_SUPPORT
ifneq ($(EHS_PERIPHERALS_PWM_SUPPORT),none)
ifneq ($(EHS_PERIPHERALS_PWM_SUPPORT),)
#	OBJECTS += inx-pwm.$(OBJ)
	OBJECTS += inx-pwm_gen.$(OBJ)
	DEFS += EHS_PERIPHERALS_PWM_SUPPORT
endif
endif
endif


ifdef EHS_MODBUS_SUPPORT
ifneq ($(EHS_MODBUS_SUPPORT),none)
ifneq ($(EHS_MODBUS_SUPPORT),)
	DEFS += EHS_MODBUS_SUPPORT
	OBJECTS += inx-modbus_config.$(OBJ)
	OBJECTS += inx-modbus_read.$(OBJ)
	OBJECTS += inx-modbus_write.$(OBJ)
	OBJECTS += inx-modbus_slave_register.$(OBJ)
#make sure the UART is enabled (assumein MODBUS always has UART)
ifndef HS_UART_SUPPORT
 EHS_UART_SUPPORT=yes
endif
endif
endif
endif

ifdef EHS_UART_SUPPORT
ifneq ($(EHS_UART_SUPPORT),none)
ifneq ($(EHS_UART_SUPPORT),)
	OBJECTS += inx-uart.$(OBJ)
	OBJECTS += inx-uart_config.$(OBJ)
	DEFS += EHS_UART_SUPPORT
endif
endif
endif

# EHS_WATCHDOG_SUPPORT, EHS_UPS_SUPPORT etc. are set by the platform's
# os-arch/config.mk (or platform/config.mk).

ifdef EHS_WATCHDOG_SUPPORT
ifneq ($(EHS_WATCHDOG_SUPPORT),none)
	OBJECTS  += watchdog.$(OBJ)
	DEFS     += EHS_WATCHDOG_SUPPORT
endif
endif

ifdef EHS_UPS_SUPPORT
ifneq ($(EHS_UPS_SUPPORT),none)
	OBJECTS  += ups.$(OBJ)
	DEFS     += EHS_UPS_SUPPORT
endif
endif

ifdef EHS_BUZZER_SUPPORT
ifneq ($(EHS_BUZZER_SUPPORT),none)
	OBJECTS  += buzzer.$(OBJ)
	DEFS     += EHS_BUZZER_SUPPORT
endif
endif

ifdef EHS_SD_SELECT_SUPPORT
ifneq ($(EHS_SD_SELECT_SUPPORT),none)
	OBJECTS  += sd_select.$(OBJ)
	DEFS     += EHS_SD_SELECT_SUPPORT
endif
endif

ifdef EHS_USB_POWER_SUPPORT
ifneq ($(EHS_USB_POWER_SUPPORT),none)
	OBJECTS  += usb_power.$(OBJ)
	DEFS     += EHS_USB_POWER_SUPPORT
endif
endif

ifdef EHS_ACCELEROMETER_SUPPORT
ifneq ($(EHS_ACCELEROMETER_SUPPORT),none)
	OBJECTS  += accelerometer.$(OBJ)
	DEFS     += EHS_ACCELEROMETER_SUPPORT
endif
endif

ifdef EHS_RS485_CONFIG_SUPPORT
ifneq ($(EHS_RS485_CONFIG_SUPPORT),none)
	OBJECTS  += rs485_config.$(OBJ)
	DEFS     += EHS_RS485_CONFIG_SUPPORT
endif
endif
