#
# deps.mk - dependencies for the current makefile
# 
# Automatically Generated. DO NOT EDIT
# 
# Created by makedeps.sh on Sun 17 Apr 23:35:17 BST 2022


targetos_init.h : target.h hal-api.h

target_types.h : ../base_full/base_types.h

targetos_init.$(OBJ) : ifaddrs.h targetos_init.h callback_queue.h  hal_process.h ehs_types.h unistd.h sys/types.h netinet/in.h linux/if.h net/if.h sys/ioctl.h sys/types.h sys/socket.h arpa/inet.h sys/param.h sys/types.h sys/statvfs.h sys/statfs.h sys/time.h sys/resource.h sys/sysinfo.h unistd.h target_net.h

target_time.$(OBJ) : target.h timer.h messages.h