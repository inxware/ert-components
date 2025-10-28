

DEFS += EHS_PID_SUPPORT

ifeq ($(EHS_PID_SUPPORT),stubbed)
	OBJECTS += $(EHS_TARGET_COMPONENT_HAL_PATH)/controller/pid/inx-PID_stub.$(OBJ)
else ifeq ($(EHS_PID_SUPPORT),gnu)

## For GNU we don't have a target specific ADC hardware configuration so set some default values for the compilation 
##  EHS_THERMOCOUPLE_AMP_GAIN_DIV_MULTIPLIER_CUSTOM has the bitlength of EHS_THERMOCOUPLE_GAIN_SHIFT_BITS
##  The calculation result involving the multiplier would be shifted right (or divided by one shifted left by) the shift bits.
	DEFS += EHS_THERMOCOUPLE_AMP_GAIN_DIV_MULTIPLIER_CUSTOM=748
	DEFS += EHS_THERMOCOUPLE_GAIN_SHIFT_BITS=17
## Maximum ADC reading in 10-bit fixed point. This is only used to check the thermocouple parameters during build time
	DEFS += EHS_THERMOCOUPLE_ADC_MAX_FP=1843200
	DEFS += EHS_PT100_AMP_GAIN_SCALE=7652
	DEFS += EHS_PT100_AMP_GAIN_DIVIDER=137

	OBJECTS += $(EHS_TARGET_COMPONENT_HAL_PATH)/controller/pid/inx-PID_gnu.$(OBJ)
#The following is a dependency of the gnu version.
	OBJECTS += $(EHS_TARGET_COMPONENT_HAL_PATH)/controller/pid/inx-PID_isr.$(OBJ)

else ifeq ($(EHS_PID_SUPPORT),esp32)
#VPATH += $(EHS_TARGET_COMPONENT_HAL_PATH)/controller
	INC_DIRS += $(EHS_TARGET_COMPONENT_HAL_PATH)/controller
	OBJECTS += $(EHS_TARGET_COMPONENT_HAL_PATH)/controller/pid/inx-PID_isr.$(OBJ)
	OBJECTS += target_pid.$(OBJ)
endif
