#---------------------------------------------------------------
# Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk
# inxware ERT configuration for linux_android_arm-no-certs
#
# Community-release variant of linux_android_arm.
#
# The parent takes device-management certificates from ../DevmanSecurity/, which
# is not part of the community release, so the parent builds but cannot complete
# `make targetenv`. This variant is identical except that it configures no devman
# server.
#
# How it works: the entire certificate section of
# target/envbuildscripts/targetenv.sh is inside SetDevmanServer(), gated on
# `[ "${DEVMAN_SERVER_DOMAIN}" != "" ]`. Clearing that one variable skips all of
# it. DEVMAN_SERVER_DOMAIN is only exported for the shell scripts
# (platform.mk:407) and is not in DEFS, so nothing changes in the binary.
#
# Devman support itself stays compiled in — the runtime simply has no server
# configured until one is supplied.

include ./target/platform/linux_android_arm/config.mk

DEVMAN_SERVER_DOMAIN=

# APK signing keys also live in ../DevmanSecurity/. targetenv_make_apk.sh:25
# stops cleanly when this is empty, so `make targetenv_apk` produces no package
# rather than an unsigned one. The executable and targetenv still build.
EHS_ANDROID_PACKAGE_SIGNING_PATH=

################################### END OF CONFIGURATION ###################################################
