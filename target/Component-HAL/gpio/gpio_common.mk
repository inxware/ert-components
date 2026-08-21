# inx limited 2024 Choose the selected hardware PWM technology or the stubbed high-evel handler
# @brief this script setsup some common and library specific abstraction of the GPIO and PWM subsystems

#todo 2025 - we probably don't really need any of these?
ifeq ($(EHS_PERIPHERALS_GPIO_SUPPORT),stubbed)
#	OBJECTS += gpio_hal_stubb.$(OBJ) - we don't need this because we have a specific stubbed target for GPIO.
	DEFS+= EHS_PERIPHERALS_GPIO_SUPPORT=EHS_PERIPHERALS_GPIO_TYPE_STUBBED
else ifeq ($(EHS_PERIPHERALS_GPIO_SUPPORT),gui)
	DEFS+= EHS_PERIPHERALS_GPIO_SUPPORT=EHS_PERIPHERALS_GPIO_TYPE_GUI
else ifeq ($(EHS_PERIPHERALS_GPIO_SUPPORT),NXP_K64)
	DEFS+= EHS_PERIPHERALS_GPIO_SUPPORT=EHS_PERIPHERALS_GPIO_TYPE_NXP_K64
else ifeq ($(EHS_PERIPHERALS_GPIO_SUPPORT),sysfs_linux_arm)
	DEFS+= EHS_PERIPHERALS_GPIO_SUPPORT=EHS_PERIPHERALS_GPIO_TYPE_SYSFS_LINUX_ARM
else ifeq ($(EHS_PERIPHERALS_GPIO_SUPPORT),wiringpi)
	DEFS+= EHS_PERIPHERALS_GPIO_SUPPORT=EHS_PERIPHERALS_GPIO_TYPE_WIRINGPI
else ifeq ($(EHS_PERIPHERALS_GPIO_SUPPORT),pigpio)
	DEFS+= EHS_PERIPHERALS_GPIO_SUPPORT=EHS_PERIPHERALS_GPIO_TYPE_PIGPIO
else ifeq ($(EHS_PERIPHERALS_GPIO_SUPPORT),ESP32_IDF)
	DEFS+= EHS_PERIPHERALS_GPIO_SUPPORT=EHS_PERIPHERALS_GPIO_TYPE_ESP32_IDF
else ifeq ($(EHS_PERIPHERALS_GPIO_SUPPORT),ESP32S3_IDF)
	DEFS+= EHS_PERIPHERALS_GPIO_SUPPORT=EHS_PERIPHERALS_GPIO_TYPE_ESP32S3_IDF
else ifeq ($(EHS_PERIPHERALS_GPIO_SUPPORT),arduino)
	DEFS+= EHS_PERIPHERALS_GPIO_SUPPORT=EHS_PERIPHERALS_GPIO_TYPE_ARDUINO
else ifeq ($(EHS_PERIPHERALS_GPIO_SUPPORT),sferalabs)
	DEFS+= EHS_PERIPHERALS_GPIO_SUPPORT=EHS_PERIPHERALS_GPIO_TYPE_SFERALABS
else ifeq ($(EHS_PERIPHERALS_GPIO_SUPPORT),zephyr)
	DEFS+= EHS_PERIPHERALS_GPIO_SUPPORT=EHS_PERIPHERALS_GPIO_TYPE_ZEPHYR
else
#This shold be probably fail as we don't really have an unknown option
# DONT SET 	DEFS+= EHS_PERIPHERALS_GPIO_SUPPORT=EHS_PERIPHERALS_GPIO_TYPE_UKNOWN
endif

EHS_COMMON_GPIO_HAL_PATH=$(EHS_TARGET_COMPONENT_HAL_PATH)/gpio
EHS_TARGET_GPIO_HAL_PATH=$(EHS_COMMON_GPIO_HAL_PATH)/$(EHS_PERIPHERALS_GPIO_SUPPORT)

# if we have a gpio subsystem .mk file use it. otherwise include the usual files below.
ifneq (,$(wildcard $(EHS_TARGET_GPIO_HAL_PATH)/target_gpio.mk))
include $(EHS_TARGET_GPIO_HAL_PATH)/target_gpio.mk
endif
INC_DIRS+=$(EHS_COMMON_GPIO_HAL_PATH)
INC_DIRS+=$(EHS_TARGET_GPIO_HAL_PATH)
VPATH+=$(EHS_COMMON_GPIO_HAL_PATH)
VPATH+=$(EHS_TARGET_GPIO_HAL_PATH)
# - only one file so no need for a VPATH?
#   OBJECTS+=$(EHS_COMMON_GPIO_HAL_PATH)/inx_gpio.$(OBJ)
OBJECTS+=inx_gpio.$(OBJ)
OBJECTS+=target_gpio.$(OBJ)



# The PWM stuff is implemented outside of the other GPIO, but will assume it is always part of a GPIO config and not an indeondent PWM technology to GPIO
# todo 2025 - we just ned to untangle all this stuff - we don't need a seperate PWM condif surely?
ifdef EHS_PERIPHERALS_PWM_SUPPORT
ifneq ($(EHS_PERIPHERALS_PWM_SUPPORT),none)
    OBJECTS+=inx_pwm.$(OBJ)
	OBJECTS+=target_pwm.$(OBJ)
	ifeq ($(EHS_PERIPHERALS_PWM_SUPPORT),stubbed)
		# Stubbed PWM: target_pwm.c lives in the stubbed GPIO directory
		EHS_TARGET_PWM_HAL_PATH=$(EHS_COMMON_GPIO_HAL_PATH)/stubbed
	else
		# When the PWM backend differs from the GPIO backend (e.g. GPIO=sysfs_linux_arm, PWM=wiringpi),
		# VPATH only contains the GPIO backend directory, so target_pwm.c from the PWM backend
		# directory cannot be found. Add the PWM backend directory explicitly so make can locate it.
		ifneq ($(EHS_PERIPHERALS_PWM_SUPPORT),$(EHS_PERIPHERALS_GPIO_SUPPORT))
			EHS_TARGET_PWM_HAL_PATH=$(EHS_COMMON_GPIO_HAL_PATH)/$(EHS_PERIPHERALS_PWM_SUPPORT)
		endif
	endif
	ifdef EHS_TARGET_PWM_HAL_PATH
		INC_DIRS+=$(EHS_TARGET_PWM_HAL_PATH)
		VPATH+=$(EHS_TARGET_PWM_HAL_PATH)
	endif
endif
endif





