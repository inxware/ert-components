#Add config for unity3d build type.

#Currently stubbed versions of standard EHS GUI operations:

#We don't render graphics with the internal BSD fonts for this option
EHS_DONT_USE_BASIC_FONTS=true

OBJECTS += target_viewport.$(OBJ)
OBJECTS += targetgfx_init.$(OBJ)

#Set this to allow UI's built in keyboard handler work.
EHS_PERIPHERALS_GUI_KEYBOARD=yes

DEFS += EHS_UNITY3D_WIDGETS
OBJECTS += inx-unity2.$(OBJ)
