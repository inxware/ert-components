#
# deps.mk - dependencies for the current makefile
# 
# Automatically Generated. DO NOT EDIT
# 
# Created by makedeps.sh on Sun 17 Apr 23:35:18 BST 2022


ehs_types.h :  target_types.h  

globals.h : target_config.h ehs_types.h

hal-api.h : globals.h ehs_types.h hal.h hal_mem.h hal_logger.h hal_time.h hal_process.h hal_xml_minimal.h hal_file.h hal_string.h hal_appstorage.h callback_queue.h  hal_media.h messages.h inx-parameters.h hal_console.h hal_webkit.h hal_lua.h hal_network.h hal_url.h hal_devapps.h hal_devman.h hal_viewport.h

hal_console.h :  target_types.h target_console.h

hal_devapps.h : globals.h hal_url.h hal_devman.h hal_appstorage.h

hal_devman.h : target.h

hal_dtv.h : ehs_types.h target_dtv.h callback_queue.h hal-api.h

hal_file.h : hal.h target_file.h ehs_types.h

hal_gui.h : hal_viewport.h font.h targetgfx_init.h

hal.h : globals.h hal_process.h

hal_jsonlist.h : hal_linkedlist.h

hal_linkedlist.h : ehs_types.h

hal_logger.h : ehs_types.h hal_string.h target_config.h target_specific.h globals.h

hal_lua.h : ehs_types.h

hal_media.h : ehs_types.h

hal_mem.h : ehs_types.h target_mem.h

hal_network.h : target_tcp.h

hal-peripherals.h : target_textdisplay.h

hal_process.h : ehs_types.h ehs_fb_types.h target_process.h

hal_string.h : ehs_types.h target_string.h

hal_time.h : target_types.h target_time.h

hal_xml_minimal.h : ehs_types.h hal_string.h hal_mem.h

inx-component.h : target.h buffer.h globals.h ehs_comp_api2.h blockref_table.h target.h ehs_fb_types.h app_data.h setCompletes.h hal-api.h

target.h : target_config.h target_types.h target_specific.h hal-api.h ehs_types.h