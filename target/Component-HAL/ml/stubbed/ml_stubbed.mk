# configure machine learning (ml) component hal
#TODO-2026: Review whether this location is correct for the stubbed ml component. It may be more appropriate to place it in a different directory, such as "stubbed/ml" or "stubbed/components/ml", to better organize the code and avoid confusion with other stubbed components.
# Add stubbed source to VPATH
VPATH+=$(EHS_TARGET_ML_ROOT_PATH)/stubbed
INC_DIRS+=$(EHS_TARGET_ML_ROOT_PATH)/stubbed

OBJECTS+=stubbed_ml.$(OBJ) 