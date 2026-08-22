#---------------------------------------------------------------
# Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk
# inxware ERT configuration for linux_armv7l_clang_debian10-no-certs
#
# Community-release variant of linux_armv7l_clang_debian10.
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

include ./target/platform/linux_armv7l_clang_debian10/config.mk

DEVMAN_SERVER_DOMAIN=

################################### END OF CONFIGURATION ###################################################

# Cleared explicitly, not left to the parent. A -no-certs variant that only
# clears DEVMAN_SERVER_DOMAIN still asks for a CA bundle if its parent sets one,
# and targetenv then fails with "you have specified
# DEVMAN_SERVER_CERTS_FULL_CA_BUNDLE, but a bundle does not exist" — the bundle
# lives in the private DevmanSecurity repo, so it is never present in public CI.
DEVMAN_SERVER_CERTS_FULL_CA_BUNDLE=
DEVMAN_SERVER_CERTS_CLIENT_AUTH_REQUIRED=
