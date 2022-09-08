#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
#---------------------------------------------------------------#

#
# deps.mk - dependencies for the current makefile
# 
# Automatically Generated. DO NOT EDIT
# 


appget.h : ehs_fb_types.h

appinfo.h : ehs_fb_types.h

arraystring1.h : ehs_fb_types.h

arraystring.h : ehs_fb_types.h

buffer.h : ehs_fb_types.h

const1.h : ehs_fb_types.h

convertor.h : ehs_fb_types.h

demux.h : ehs_fb_types.h

ehs_if.h : ehs_fb_types.h

event_counter1.h : ehs_fb_types.h

file.h : ehs_fb_types.h globals.h

fs_dir_create_remove.h : ehs_fb_types.h

fs_dir_list1.h : ehs_fb_types.h

if.h : ehs_fb_types.h

logic.h : ehs_fb_types.h

mux.h : target_config.h ehs_fb_types.h

rtinfo.h : ehs_fb_types.h

appget.$(OBJ) : ehs_fb_types.h setCompletes.h callback_queue.h fid.h app_data.h hal_devapps.h target.h appget.h hal_string.h hal_file.h hal-api.h

appinfo.$(OBJ) : target.h appinfo.h app_data.h setCompletes.h hal-api.h hal_devman.h

arraystring1.$(OBJ) : globals.h target.h arraystring1.h setCompletes.h hal-api.h

arraystring.$(OBJ) : arraystring.h globals.h target.h setCompletes.h hal-api.h

buffer.$(OBJ) : target.h buffer.h globals.h hal_mem.h setCompletes.h hal-api.h

calc_i.$(OBJ) : ehs_fb_types.h calc_i.h utils.h target.h globals.h messages.h app_data.h hal-api.h

const1.$(OBJ) : target.h globals.h const1.h setCompletes.h hal-api.h

convertor.$(OBJ) : target.h convertor.h setCompletes.h globals.h hal-api.h

core_components.$(OBJ) : blockref_table.h arraystring1.h buffer.h calc_i.h const1.h convertor.h demux.h event_counter1.h file.h ehs_if.h logic.h mux.h operator1.h sfc.h state.h stringfn.h time_clock.h wall_clock.h trigger.h fs_dir_list1.h fs_dir_create_remove.h system_exec.h rtinfo.h appinfo.h appget.h

demux.$(OBJ) : target.h demux.h setCompletes.h ehs_comp_api2.h hal-api.h

ehs_if.$(OBJ) : setCompletes.h ehs_if.h hal-api.h

event_counter1.$(OBJ) : globals.h event_counter1.h fid.h target.h app_data.h hal-api.h

file.$(OBJ) : target.h file.h setCompletes.h hal-api.h app_data.h

fileFunctionsUtils.$(OBJ) : target.h ehs_fb_types.h file.h

fs_dir_create_remove.$(OBJ) : target.h fs_dir_create_remove.h globals.h setCompletes.h app_data.h hal-api.h

fs_dir_list1.$(OBJ) : target.h fs_dir_list1.h globals.h setCompletes.h app_data.h hal-api.h

if.$(OBJ) : setCompletes.h if.h

logic.$(OBJ) : logic.h setCompletes.h hal-api.h

mux.$(OBJ) : mux.h setCompletes.h hal-api.h

operator1.$(OBJ) : globals.h target.h operator1.h setCompletes.h app_data.h hal-api.h

rtinfo.$(OBJ) : target.h rtinfo.h setCompletes.h hal-api.h

SFCBarGroup1.$(OBJ) : globals.h sfc.h setCompletes.h hal-api.h

SFCBarGroup1Event.$(OBJ) : ehs_fb_types.h sfc.h setCompletes.h

sfc.$(OBJ) : globals.h sfc.h setCompletes.h hal-api.h

state.$(OBJ) : ehs_fb_types.h state.h setCompletes.h hal-api.h

stringfn.$(OBJ) : globals.h ehs_fb_types.h ehs_fb_types.h stringfn.h fid.h app_data.h hal-api.h

system_exec.$(OBJ) : globals.h ehs_fb_types.h system_exec.h setCompletes.h target.h fid.h app_data.h hal-api.h

time_clock.$(OBJ) : ehs_fb_types.h time_clock.h fid.h timer.h app_data.h hal-api.h

trigger.$(OBJ) : ehs_fb_types.h trigger.h setCompletes.h target.h hal-api.h

wall_clock.$(OBJ) : ehs_fb_types.h wall_clock.h app_data.h hal-api.h
