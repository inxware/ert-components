# Copyright (C) 2025 inx limited, UK - All Rights Reserved.
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license
# with this file. If not, please visit:
#  <https://www.gnu.org/licenses/lgpl-3.0.txt>
#
#---------------------------------------------------------------
# Dependencies for Qt graphics HAL
#---------------------------------------------------------------

stddefs.h : stlite.h

targetgfx_init.h : target.h

target_viewport.h : graphics.h

targetgfx_init.$(OBJ) : targetgfx_init.h  hal_viewport.h

target_viewport.$(OBJ) :  hal_string.h hal_viewport.h hal_process.h widget.h graphics.h hal-api.h

qt_main_integration.$(OBJ): qt_main_integration.c ertqt.h

ertqt.$(OBJ): ertqt.cpp \
              ertqt.h \
              ertqt_label.h \
              ertqt_text_field.h \
              ertqt_button.h \
              ertqt_checkbox.h
