#
# deps.mk - dependencies for the current makefile
# 
# Automatically Generated. DO NOT EDIT
# 
# Created by makedeps.sh on Sun 17 Apr 23:35:18 BST 2022


dtv_remote2.h : ehs_fb_types.h

gpio_in.h : ehs_fb_types.h inx-component.h

gpio_out.h : ehs_fb_types.h inx-component.h

guileds.h : ehs_fb_types.h

guitextdisplay.h : ehs_fb_types.h

keypress.h : ehs_fb_types.h callback_queue.h

dtv_remote.$(OBJ) : target.h dtv_remote.h dtv_remote1.h ehs_fb_types.h dtv_remote2.h globals.h fid.h callback_queue.h keypress.h

gpio_in.$(OBJ) : target.h inx-parameters.h gpio_in.h target_gpio.h

gpio_out.$(OBJ) : target.h inx-parameters.h gpio_out.h target_gpio.h

guileds.$(OBJ) : target.h guileds.h guiparams.h globals.h setCompletes.h app_data.h hal_string.h messages.h

guitextdisplay.$(OBJ) : target.h guitextdisplay.h guiparams.h globals.h widget.h font.h setCompletes.h app_data.h hal_string.h hal_viewport.h messages.h

keypress.$(OBJ) : target.h keypress.h globals.h setCompletes.h inx-parameters.h inx-component.h

peripherals_components.$(OBJ) : blockref_table.h dtv_remote2.h keypress.h gpio_out.h gpio_in.h