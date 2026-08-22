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

# Accelerometer and Gyroscope (combined IMU) — HAL selected via EHS_PERIPHERALS_ACCEL_GYRO_SUPPORT
# Both the simple accelerometer FB and the combined accel_gyro FB share the same HAL, so both
# are guarded by the same variable.  os-arch configs default this to 'stubbed'.

ifdef EHS_PERIPHERALS_ACCEL_GYRO_SUPPORT
ifneq ($(EHS_PERIPHERALS_ACCEL_GYRO_SUPPORT),none)
	OBJECTS += inx-accel_gyro.$(OBJ)
	DEFS += EHS_PERIPHERALS_ACCEL_GYRO_SUPPORT
endif
endif

# Components moved from user/ directory
# NOTE: ADC unit/channel counts are NOT configured here - they belong to the ADC/DAC
# HAL and are handled by target/Component-HAL/adc_dac/adc_dac_common.mk.  A target may
# have GPIO without ADC (or the reverse), so the two must never share a guard.
ifdef EHS_PERIPHERALS_ADC_DAC_SUPPORT
ifneq ($(EHS_PERIPHERALS_ADC_DAC_SUPPORT),none)
ifneq ($(EHS_PERIPHERALS_ADC_DAC_SUPPORT),)
# EHS_TARGET_ADC_UNIT_NUMBER=0 means "this board variant has no ADC units at all".
# adc_dac_common.mk builds no HAL backend in that case, so the FBs that call it must
# be left out too or the link fails with undefined references.
ifneq ($(EHS_TARGET_ADC_UNIT_NUMBER),0)
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
# Modbus is UART-based, so enable the UART blocks unless the platform already
# chose a value. Was 'ifndef HS_UART_SUPPORT' — that name exists nowhere, so the
# branch always fired and overrode a platform's deliberate EHS_UART_SUPPORT=none.
ifndef EHS_UART_SUPPORT
 EHS_UART_SUPPORT=yes
endif
endif
endif
endif

# The inx-uart blocks are written against the Component-HAL UART contract, not
# just the TgtUart_* entry points: they reference gTargetUartPin*, gUARTBaudRate,
# gUARTParity, gUARTStopBits, gUARTHWCTRL, Common_UART_onReceive and
# TgtUART_CONFIG_ERROR, all of which come from target/Component-HAL/uart/<impl>.
# So the blocks need EHS_PERIPHERALS_UART_SUPPORT (which selects that backend),
# not merely EHS_UART_SUPPORT (which some os-arch trees use for their own UART
# HAL). Every target that builds these blocks today sets both. Requiring only
# the latter compiled the blocks with no backend and failed on undefined symbols.
ifdef EHS_UART_SUPPORT
ifneq ($(EHS_UART_SUPPORT),none)
ifneq ($(EHS_UART_SUPPORT),)
ifneq ($(filter-out none,$(EHS_PERIPHERALS_UART_SUPPORT)),)
	OBJECTS += inx-uart.$(OBJ)
	OBJECTS += inx-uart_config.$(OBJ)
	DEFS += EHS_UART_SUPPORT
endif
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


ifdef EHS_RS485_CONFIG_SUPPORT
ifneq ($(EHS_RS485_CONFIG_SUPPORT),none)
	OBJECTS  += rs485_config.$(OBJ)
	DEFS     += EHS_RS485_CONFIG_SUPPORT
endif
endif
