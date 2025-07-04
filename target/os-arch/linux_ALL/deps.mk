#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

#
# deps.mk - dependencies for the current makefile
# 
# Automatically Generated. DO NOT EDIT
# 


targetos_init.h : target.h hal-api.h

target_types.h : ../base_full/base_types.h

targetos_init.$(OBJ) : ifaddrs.h targetos_init.h callback_queue.h  hal_process.h ehs_types.h unistd.h sys/types.h netinet/in.h linux/if.h net/if.h sys/ioctl.h sys/types.h sys/socket.h arpa/inet.h sys/param.h sys/types.h sys/statvfs.h sys/statfs.h sys/time.h sys/resource.h sys/sysinfo.h unistd.h target_net.h

target_time.$(OBJ) : target.h timer.h messages.h
