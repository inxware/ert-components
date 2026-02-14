

EHS_TARGET_GRAPHICS_PATH=$(EHS_TARGET_COMPONENT_HAL_PATH)/graphics/$(EHS_GUI_SUPPORT)
DEFS+=EHS_GUI_SUPPORT


# include sourcecode path of this dir in build
include $(EHS_TARGET_GRAPHICS_PATH)/deps.mk
VPATH+=$(EHS_TARGET_GRAPHICS_PATH)
INC_DIRS+=$(EHS_TARGET_GRAPHICS_PATH)

# include sourcecode from components dir in build
include $(EHS_TARGET_GRAPHICS_PATH)/Components/deps.mk
VPATH+= $(EHS_TARGET_GRAPHICS_PATH)/Components
INC_DIRS += $(EHS_TARGET_GRAPHICS_PATH)/Components

include $(EHS_TARGET_GRAPHICS_PATH)/graphics.mk
include $(EHS_TARGET_GRAPHICS_PATH)/Components/components.mk

# After choosing the graphics library we wannt to make sure the rendeer mode has been set.

ifeq ($(EHS_GUI_SUPPORT),lvgl)
	EHS_RENDER_MODE=B
	EHS_DONT_USE_BASIC_FONTS=yes
	DEFS += EHS_GUI_SUPPORT_MODE_B
else ifeq ($(EHS_GUI_SUPPORT),qt)
    # QT doesn't need our widgets or rendering. Just the MODE B widget mapping.
	EHS_DONT_USE_BASIC_FONTS=yes
	EHS_RENDER_MODE=Q
	# Set this as QT rendering is a type of render mode B
	DEFS += EHS_GUI_SUPPORT_MODE_B
	# Set this for addition QT falvour of render MODE B     
	DEFS += EHS_GUI_SUPPORT_MODE_B_QT
else
# Default inx widgets rendered with GTK, FB, SDL, OpenGL, ...
	EHS_RENDER_MODE=A
	DEFS += EHS_GUI_SUPPORT_MODE_A
endif

ifdef EHS_DONT_USE_BASIC_FONTS
export EHS_DONT_USE_BASIC_FONTS
endif

ifdef EHS_RENDER_MODE
export EHS_RENDER_MODE
endif

