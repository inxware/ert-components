#---------------------------------------------------------------
# Copyright (C) 2008-2025 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk
# @author: inx limited

# Usage Description
# This is for Raspberry Pis - it is aimed at being used with inxware demo
# using the Axelera AI Metis NPU for hardware-accelerated inference.

# ─────────────────────────────────────────────────────────────────────────────
# Axelera AI SDK — reference URLs
#
# Main product / Voyager SDK page:
#   https://axelera.ai/ai-software/voyager-sdk
# GitHub (Voyager SDK — open source, ARM64 configs under cfg/):
#   https://github.com/axelera-ai-hub/voyager-sdk
# Community / developer forum:
#   https://community.axelera.ai/
#
# Public APT repository (no auth, packages for arm64 and amd64):
#   https://software.axelera.ai/artifactory/axelera-apt-source/
# APT repo GPG key:
#   https://software.axelera.ai/artifactory/api/security/keypair/axelera/public
# Axelera PyPI index (Python SDK wheels):
#   https://software.axelera.ai/artifactory/api/pypi/axelera-pypi/simple
#
# Key APT packages (current SDK version = 1.6.0):
#   axelera-runtime-1.6.0  — runtime libs, headers, pkg-config
#                             installs to /opt/axelera/runtime-1.6.0-1/
#   axelera-device-1.6.0   — device firmware blobs
#                             installs to /opt/axelera/device-1.6.0-1/
#   metis-dkms             — PCIe kernel driver (NOT needed in build container)
#
# Distribution strings:  ubuntu22 main  /  ubuntu24 main
# (ubuntu24 used for this Debian 12 image — same libc/ABI base)
#
# ─── Docker strategy: OPTION B — extend demo RPI image (IMPLEMENTED) ─────────
# The Dockerfile for this target extends inxware/inx-debian12-clang-raspi-arm64
# (the base demo RPI cross-build image) and adds the Axelera APT repo, then
# installs axelera-runtime:arm64 + axelera-device:arm64 for cross-linking.
# This is cleaner than the Hailo approach (no temp/.deb copy needed) because
# Axelera's APT repo is fully public and unauthenticated.
# ─────────────────────────────────────────────────────────────────────────────

include ./target/platform/linux_arm64_lvgl_raspberrypi_demo/config.mk

# Machine Learning & Machine Vision Support — Axelera Metis NPU
# EHS_ML_SUPPORT=yes enables the full ML stack.
# EHS_ML_HARDWARE_ACCELERATION=axelera selects the axruntime backend
# (target/Component-HAL/ml/engine/axelera/).
# EHS_ML_IE_IMAGE_SUPPORT is intentionally unset — no TFLite CPU fallback;
# the Metis NPU handles all inference.
EHS_ML_SUPPORT=yes
EHS_ML_HARDWARE_ACCELERATION=axelera
EHS_ML_IE_IMAGE_SUPPORT=none
SDL_FULLSCREEN=no
EHS_DEFAULT_APP=hardware-demos/rpi-axelera

################################### END OF TOOLBOX CONFIGURATION ###################################################
################################################################################################################
# Define any OS install scripts that should be run on first install
################################################################################################################
