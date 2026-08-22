#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk 
# inxware ERT configuration file for linux_android_arm_p64_h6_player-ambifier
# @author: inx limited
# This target siimply builds a standard linux version of EHS with android toolchain.
# It does not manage any events or resources such as graphics, so is unlikely to be useful
# other than to test compilation of standard code. It would run an exe using sysecec I expect.

include ./target/platform/linux_android_arm_radxa_rock_3c_player-adnoc-brown/config.mk

################################### END OF TOOLBOX CONFIGURATION ###################################################

include ./target/devman-configs/devman-tsa.inx-systems.net.mk

# make sure we configure 'player' as a product
EHS_PRODUCT_NAME=player

# skip extra devman server settings
DEVMAN_SERVER_DOMAIN_1=
DEVMAN_SERVER_DOMAIN_2=

# default Lucid app
EHS_DEFAULT_APP=customer-apps/RetailMusic/RetailAudio-v0.1.0

# don't set up ntp server for this traget
NETWORK_NTP_SERVER=

# enable this to log verbose curl
#DEFS += EHS_ENABLE_CURL_VERBOSE