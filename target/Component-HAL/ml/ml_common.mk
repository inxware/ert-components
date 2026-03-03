# configure machine learning (ml) component hal

# Makefile Variable for ML source root path
EHS_TARGET_ML_ROOT_PATH=$(EHS_TARGET_COMPONENT_HAL_PATH)/ml

ifeq ($(EHS_ML_SUPPORT),stubbed)
# Machine Learning Stubbed support
## Add C macro definition for ML support
	DEFS += EHS_ML_SUPPORT
## TODO2026  we shouldn't need to define a stubbed preprocessor if we are just going to build the stubbed code?
	DEFS += EHS_ML_SUPPORT_STUBBED

## Add ML Common sources and headers
	VPATH+=$(EHS_TARGET_ML_ROOT_PATH)
	OBJECTS+=ml_common.$(OBJ)
	INC_DIRS+=$(EHS_TARGET_ML_ROOT_PATH)

## Include ML Stubbed source path
### Stubbed source is generic for all platforms
	include $(EHS_TARGET_COMPONENT_HAL_PATH)/ml/stubbed/ml_stubbed.mk

# The following should check for a specific type of ML support?
# There was a discussion about supporting multiple ML abstractions (e.g. CPU + NPU), in which case we probably 
# want a seperate EHSMLSUPPORT_2 or seomthing like that 
else ifeq ($(EHS_ML_SUPPORT),yes)
# General Machine Learning support
## Add C macro definition for ML support
	DEFS += EHS_ML_SUPPORT

## Add ML Common sources and headers
	VPATH+=$(EHS_TARGET_ML_ROOT_PATH)
	OBJECTS+=ml_common.$(OBJ)
	INC_DIRS+=$(EHS_TARGET_ML_ROOT_PATH)

## Makefile Variable for Framework and Model path
	EHS_TARGET_ML_FRAMEWORK_PATH=$(EHS_TARGET_ML_ROOT_PATH)/framework
	EHS_TARGET_ML_ENGINE_POSTPROCESSING_PATH=$(EHS_TARGET_ML_ROOT_PATH)/postprocessing/engine
	EHS_TARGET_ML_GENERAL_POSTPROCESSING_PATH=$(EHS_TARGET_ML_ROOT_PATH)/postprocessing/general
	EHS_TARGET_ML_MODEL_PATH=$(EHS_TARGET_ML_ROOT_PATH)/postprocessing/model

## Include Framework and Model makefiles
	include $(EHS_TARGET_ML_FRAMEWORK_PATH)/ml_framework.mk
	include $(EHS_TARGET_ML_ENGINE_POSTPROCESSING_PATH)/ml_postprocessing_engine.mk
	include $(EHS_TARGET_ML_GENERAL_POSTPROCESSING_PATH)/ml_postprocessing_general.mk
	include $(EHS_TARGET_ML_MODEL_PATH)/ml_model.mk

## Include ML Stubbed source path
### Stubbed source is generic for all platforms
	include $(EHS_TARGET_COMPONENT_HAL_PATH)/ml/stubbed/ml_stubbed.mk

#	CXX_INC_DIRS+=$(EHS_COMMON_HAL_PATH)/include
	CXX_INC_DIRS+=$(INC_DIRS)

else
# No Machine Learning support
## Do NOTHING as there should be NO ML support

endif #EHS_ML_SUPPORT
