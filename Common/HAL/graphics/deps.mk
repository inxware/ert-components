#
# deps.mk - dependencies for the current makefile
# 
# Automatically Generated. DO NOT EDIT
# 
# Created by makedeps.sh on Sun 17 Apr 23:35:18 BST 2022


font.h :  target_config.h  graphics_types.h

freetype_font.h : stb_truetype.h stb_truetype.h

graphics.h :  target_config.h graphics_types.h targetgfx_init.h

graphics_types.h :  target_config.h

html.h : 

widget.h : hal_viewport.h target_config.h graphics.h widget_textbox.h widget_image.h widget_patch.h widget_viewport.h 

widget_image.h :   graphics.h widget_image_gif.h widget_image_png.h widget_image_jpg.h widget_image_svg.h

widget_image_jpg.h : hal_viewport.h

widget_image_png.h : hal_viewport.h

widget_image_svg.h : hal_viewport.h

widget_patch.h :   graphics.h

widget_textbox.h :   font.h html.h

widget_viewport.h :   graphics.h

font.$(OBJ) : font.h hal_logger.h hal_file.h hal_string.h hal_viewport.h  hal_mem.h

graphics.$(OBJ) : graphics.h hal-api.h png.h

html.$(OBJ) : html.h hal_mem.h hal_string.h 

widget.$(OBJ) : widget.h   hal_string.h hal_process.h

widget_image.$(OBJ) : widget.h  hal_string.h hal_process.h hal_mem.h

widget_image_gif.$(OBJ) :  widget.h hal_mem.h  hal_string.h hal_viewport.h target.h

widget_image_jpg.$(OBJ) :  widget.h target.h graphics.h  hal-api.h jpeg/jpeglib.h

widget_image_png.$(OBJ) :  widget.h target.h png/png.h graphics.h targetgfx_init.h  hal-api.h

widget_image_svg.$(OBJ) :  widget.h target.h graphics.h  hal_string.h hal_viewport.h svg/BFBasicTypes.h svg/BFSVGT.h

widget_patch.$(OBJ) : widget.h  hal_string.h hal_process.h hal_viewport.h

widget_textbox.$(OBJ) : widget.h  hal_string.h hal_process.h hal_viewport.h font.h

widget_viewport.$(OBJ) : widget.h  hal_viewport.h hal-api.h