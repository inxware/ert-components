#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk 
# inxware ERT configuration file for linux_android_arm_p64_a6_player-adnoc
# @author: inx limited
# This target siimply builds a standard linux version of EHS with android toolchain.
# It does not manage any events or resources such as graphics, so is unlikely to be useful
# other than to test compilation of standard code. It would run an exe using sysecec I expect.

# @author: inx limited


include ./target/platform/linux_android_arm_p64_a6_player-ambifier/config.mk

################################### END OF TOOLBOX CONFIGURATION ###################################################

#DEVMAN_SERVER_DOMAIN=ADDNOC-Server URL TO GO HERE
#DEVMAN_SERVER_PROTOCOL=https

include ./target/devman-configs/adnoc-green.local.mk
