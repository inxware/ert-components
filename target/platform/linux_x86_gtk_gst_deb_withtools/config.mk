#
# config.mk - Configuration properties of the current platform
# 
# Called by ../../../Makefile
#

# @author: Pierre Drezet
# @version: $Revision: 43 $
# @date: $Date: 2012-3-1 $
# 
# Copyright (c) inx limited, 2008. All rights reserved.
#

#DEBUG OPTIONS
#EHS_DEBUGALL=true
ifdef EHS_DEBUGALL
DEFS += EHS_RUNTIME_LOGGER_ENABLED
DEFS += EHS_DEBUG_AV
export EHS_DEBUG=yes
endif

#overrides the deb only variant var
export SYSTEM_VARIANT=debian_tools
DEFS += EHS_GST_010

include ./target/platform/linux_x86_gtk_gst/config.mk



