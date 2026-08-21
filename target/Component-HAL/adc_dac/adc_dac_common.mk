

ifneq ($(EHS_TARGET_ADC_UNIT_NUMBER),0)
# Some family options may not have ADC Units and other will, tis can be signalled by EHS_TARGET_ADC_UNIT_NUMBER=0
# Warning this means DACS code will not be built if set to 0!!!
# (Common/Components/peripherals/components.mk applies the same test so the ADC/DAC
#  function blocks are dropped too - otherwise the link fails on missing HAL symbols.)

# ------------------------------------------------------------------------------------
# ADC geometry - EHS_TARGET_ADC_UNIT_NUMBER / EHS_TARGET_ADC_CHANNEL_NUMBER
#
# These are properties of the ADC hardware ONLY.  They must never be defaulted from,
# or guarded by, GPIO/UART/PWM support - a board can have any one without the others.
#
# Each backend's target_adcdac.h carries its own #ifndef fallback sized for that part
# (stubbed 1x1, SPI_A6_LTC241X 1x16, ESP32S3_IDF 2x10, arduino 1x4, sferalabs 1x8).
# So we only push a -D when the platform config.mk actually declares a value; staying
# silent lets the backend default apply instead of overwriting it with a wrong one.
#
# There is no valid "0 channels" - every backend header #errors on <= 0 and has no
# EHS_TARGET_ADC_CHANNEL_DEFAULT() expansion for 0.  A board with no analog input is
# expressed as EHS_PERIPHERALS_ADC_DAC_SUPPORT=none, or EHS_TARGET_ADC_UNIT_NUMBER=0.
# ------------------------------------------------------------------------------------
ifdef EHS_TARGET_ADC_UNIT_NUMBER
	DEFS+=EHS_TARGET_ADC_UNIT_NUMBER=$(EHS_TARGET_ADC_UNIT_NUMBER)
endif
ifdef EHS_TARGET_ADC_CHANNEL_NUMBER
	DEFS+=EHS_TARGET_ADC_CHANNEL_NUMBER=$(EHS_TARGET_ADC_CHANNEL_NUMBER)
endif

EHS_COMMON_ADC_DAC_HAL_PATH=$(EHS_TARGET_COMPONENT_HAL_PATH)/adc_dac
EHS_TARGET_ADC_DAC_HAL_PATH=$(EHS_COMMON_ADC_DAC_HAL_PATH)/$(EHS_PERIPHERALS_ADC_DAC_SUPPORT)
INC_DIRS+=$(EHS_COMMON_ADC_DAC_HAL_PATH)
INC_DIRS+=$(EHS_TARGET_ADC_DAC_HAL_PATH)
VPATH+=$(EHS_COMMON_ADC_DAC_HAL_PATH)
VPATH+=$(EHS_TARGET_ADC_DAC_HAL_PATH)

ifneq (,$(wildcard $(EHS_TARGET_ADC_DAC_HAL_PATH)/target_adcdac.mk))
include $(EHS_TARGET_ADC_DAC_HAL_PATH)/target_adcdac.mk
endif

OBJECTS+=target_adcdac.$(OBJ)

ifeq ($(EHS_PERIPHERALS_ADC_DAC_SUPPORT),stubbed)
	DEFS+=EHS_PERIPHERALS_ADC_DAC_SUPPORT=EHS_PERIPHERALS_ADCDAC_TYPE_STUBBED
else ifeq ($(EHS_PERIPHERALS_ADC_DAC_SUPPORT),SPI_A6_LTC241X)
	DEFS+=EHS_PERIPHERALS_ADC_DAC_SUPPORT=EHS_PERIPHERALS_ADCDAC_TYPE_SPI_A6_LTC241X
else ifeq ($(EHS_PERIPHERALS_ADC_DAC_SUPPORT),NXP_K64)
	DEFS+=EHS_PERIPHERALS_ADC_DAC_SUPPORT=EHS_PERIPHERALS_ADCDAC_TYPE_NXP_K64
else ifeq ($(EHS_PERIPHERALS_ADC_DAC_SUPPORT),ESP32_IDF)
#Not currently supported - but shouldn't get here?:	OBJECTS += target_adcdac.$(OBJ)
#note for todo 2024 we sometimes use enumerated hash defsand sometimes use the EHS_XXXXX_SUPPORT__VALUE method for DEFS - both can be useful, but need to decide which is the better overall (prolly enums) 
#	DEFS+=EHS_PERIPHERALS_ADC_DAC_SUPPORT__NONE
	DEFS+=EHS_PERIPHERALS_ADC_DAC_SUPPORT=EHS_PERIPHERALS_ADCDAC_TYPE_ESP32_IDF
else ifeq ($(EHS_PERIPHERALS_ADC_DAC_SUPPORT),ESP32S3_IDF)
	DEFS+=EHS_PERIPHERALS_ADC_DAC_SUPPORT=EHS_PERIPHERALS_ADCDAC_TYPE_ESP32S3_IDF
else ifeq ($(EHS_PERIPHERALS_ADC_DAC_SUPPORT),arduino)
	DEFS+=EHS_PERIPHERALS_ADC_DAC_SUPPORT=EHS_PERIPHERALS_ADCDAC_TYPE_ARDUINO
else ifeq ($(EHS_PERIPHERALS_ADC_DAC_SUPPORT),sferalabs)
	DEFS+=EHS_PERIPHERALS_ADC_DAC_SUPPORT=EHS_PERIPHERALS_ADCDAC_TYPE_SFERALABS
# The NXP os-arch prbably hardwires ADC support in already included code.
else
# DONT SET DEFS+=EHS_PERIPHERALS_ADC_DAC_SUPPORT=EHS_PERIPHERALS_ADCDAC_TYPE_UNKNOWN
# No objects added - might leave a hole in the toolbox and cause a build error.
endif

endif
