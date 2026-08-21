#!/bin/bash
#---------------------------------------------------------------
# Copyright (C) 2010-2026, inx limited, UK.
# All Rights Reserved.
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license
# with this file. If not, please visit
# <https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------
#
# configure_host.sh - install the build tools this MACHINE needs.
#
# Host scope, not target scope: nothing here depends on which target is
# configured. Xcode Command Line Tools, Homebrew packages, apt packages, git-lfs,
# GNU make, curl, Docker - all properties of the build machine.
#
# Split out of prepdeps.sh (2026-08). It used to run on every 'make prepdeps',
# which meant a 37-platform regression sweep probed Homebrew and could invoke
# 'sudo apt' 37 times to do nothing. What remains in prepdeps.sh is the
# dependency-repo fetch and the per-target checks, which do need the
# make-exported environment (EHS_ARCH, TARGET); this script does not.
#
# Run once per machine:   make configure-host
# It is idempotent - every install is guarded by a presence check - so re-running
# is harmless, just slower than skipping it.

source ./scripts/build-function-library/colour.sh
TXT_FG=${TXT_FG_GREEN}

# ── Non-interactive mode ──────────────────────────────────────────────────────
#
# With INX_NONINTERACTIVE=1 (which CI sets) this script never sudo's, never
# invokes a package manager and never waits for input. It collects everything the
# machine is missing and exits non-zero at the end listing the exact commands to
# run.
#
# The reason is CI: an unattended sweep that hits 'sudo apt' or a 'read -n 1'
# stops dead with no output and no verdict - exactly how a leftover Vagrant
# prompt silently stalled the regression runner.
#
# Interactively (the default) behaviour is unchanged: it still installs for you.
INX_NONINTERACTIVE="${INX_NONINTERACTIVE:-0}"
[ "${CI:-}" = "true" ] && INX_NONINTERACTIVE=1

_MISSING_DESC=()
_MISSING_CMD=()

# needs_install <description> <command the developer should run>
#   interactive     -> 0, the caller does the install itself
#   non-interactive -> records it and returns 1, so the caller skips the install
needs_install() {
    if [ "${INX_NONINTERACTIVE}" = "1" ]; then
        _MISSING_DESC+=("$1")
        _MISSING_CMD+=("$2")
        return 1
    fi
    return 0
}

# Bail at the end with the full list, rather than on the first missing item, so
# one run tells the developer everything they need to do.
report_missing_and_exit() {
    [ "${#_MISSING_DESC[@]}" -eq 0 ] && return 0
    echo
    err "This machine is missing build tools required by ert-components."
    err "Running non-interactively (INX_NONINTERACTIVE=1) - nothing was installed."
    echo
    local i
    for i in "${!_MISSING_DESC[@]}"; do
        echo "${TXT_FG_WHITE}  - ${_MISSING_DESC[$i]}${TXT_RESET}"
        echo "      ${_MISSING_CMD[$i]}"
    done
    echo
    err "Install the above, or run 'make configure-host' interactively, then retry."
    exit 1
}

OS_TYPE="$(uname -s)"
CPU_ARCH="$(uname -m)"

if [ "$OS_TYPE" = "Darwin" ]; then
    # ---------------------------------------------------------------------------
    # macOS host — use Homebrew for package management.
    #
    # Confirmed working on: macOS Monterey 12.x, Intel x86_64, Apple CLT 14.0.
    #
    # Apple Silicon (arm64): the macos_x86_64_clang target produces an Intel
    # x86_64 binary. M-series Macs can run x86_64 binaries under Rosetta 2,
    # but this combination has NOT been tested. The Homebrew library paths
    # in target/os-arch/macos-x86_64/target.mk are hardcoded to the Intel
    # Homebrew prefix (/usr/local/opt/...). On Apple Silicon Homebrew uses
    # /opt/homebrew — those paths would need updating. (BEST GUESS — untested.)
    # ---------------------------------------------------------------------------
    if [ "$CPU_ARCH" = "arm64" ]; then
        warn "Apple Silicon (arm64) detected. The macos_x86_64_clang target"
        warn "produces an Intel x86_64 binary (untested on this architecture)."
        warn "Homebrew paths in target/os-arch/macos-x86_64/target.mk assume"
        warn "the Intel prefix /usr/local/opt — update them to /opt/homebrew/opt"
        warn "if library-not-found errors occur at link time."
    fi

    # Xcode Command Line Tools — provides clang, ar, make (3.81), and SDK stubs
    # for libz, libexpat, libcurl. All confirmed sufficient for macos_x86_64_clang.
    if ! xcode-select -p &>/dev/null; then
        if needs_install "Xcode Command Line Tools" "xcode-select --install"; then
            echo "Xcode Command Line Tools not found. Launching installer..."
            xcode-select --install
            echo "Re-run this script after the CLT installation completes."
            exit 1
        fi
    fi

    # Homebrew — required for the two non-system link dependencies below.
    if ! command -v brew &>/dev/null; then
        if needs_install "Homebrew" "see https://brew.sh"; then
            err "Homebrew not found. Install it from https://brew.sh then re-run this script."
            exit 1
        fi
        # Non-interactive: nothing below can work without brew, so stop here.
        report_missing_and_exit
    fi

    # git-lfs — not bundled with macOS or Xcode CLT.
    if ! git lfs env &>/dev/null; then
        if needs_install "git-lfs" "brew install git-lfs && git lfs install"; then
            echo "Installing git-lfs via Homebrew..."
            brew install git-lfs
            git lfs install
        fi
    fi

    # libarchive — Homebrew keg-only (not symlinked into /usr/local by default
    # to avoid shadowing the macOS system libarchive). The build system references
    # it via the explicit keg path /usr/local/opt/libarchive, hardcoded in
    # target/os-arch/macos-x86_64/target.mk. Confirmed working: v3.8.7.
    if ! brew list libarchive &>/dev/null; then
        if needs_install "libarchive (Homebrew)" "brew install libarchive"; then
            echo "Installing libarchive via Homebrew..."
            brew install libarchive
        fi
    fi

    # libidn2 — confirmed at /usr/local/lib/libidn2.dylib after install. Linked
    # via the standard /usr/local/lib search path. Confirmed working: v2.3.8.
    if ! brew list libidn2 &>/dev/null; then
        if needs_install "libidn2 (Homebrew)" "brew install libidn2"; then
            echo "Installing libidn2 via Homebrew..."
            brew install libidn2
        fi
    fi

    # sdl2 — required for macos_x86_64_clang_lvgl (LVGL display backend).
    # Keg-only; the build system references it via /usr/local/opt/sdl2.
    # Not needed for macos_x86_64_clang (no GUI).
    if ! brew list sdl2 &>/dev/null; then
        if needs_install "sdl2 (Homebrew)" "brew install sdl2"; then
            echo "Installing sdl2 via Homebrew..."
            brew install sdl2
        fi
    fi

    # qt — required for macos_x86_64_clang_qt (Qt6/QML display backend).
    # Keg-only; the build system references it via /usr/local/opt/qt.
    # Installs ~1.5 GB of Qt6 frameworks. Not needed for non-Qt targets.
    if ! brew list qt &>/dev/null; then
        if needs_install "qt (Homebrew)" "brew install qt"; then
            echo "Installing Qt6 via Homebrew (this may take several minutes)..."
            brew install qt
        fi
    fi

    # Docker Desktop — needed for cross-compilation targets (ESP32, ARM Linux,
    # etc.) but NOT required for the macos_x86_64_clang native host build.
    # 'brew install --cask docker' downloads Docker Desktop; still requires
    # launching Docker.app once to accept the licence and start the daemon.
    # (BEST GUESS — cask install flow not fully tested end-to-end on macOS.)
    if ! command -v docker &>/dev/null; then
        warn "Docker Desktop not found."
        warn "  For macos_x86_64_clang (native build): Docker is NOT required."
        warn "  For cross-compilation targets (ESP32, ARM Linux, ...): Docker IS required."
        warn "  Install options:"
        warn "    brew install --cask docker   (then launch Docker.app to finish setup)"
        warn "    OR: https://www.docker.com/products/docker-desktop/"
    fi

else
    # ---------------------------------------------------------------------------
    # Linux (Debian/Ubuntu) host — use apt.
    # ---------------------------------------------------------------------------

    # Check we have git lfs installed
    if ! git lfs env &>/dev/null; then
        if needs_install "git-lfs" "sudo apt -y install git-lfs"; then
            echo "Installing git lfs. You may be asked for your user password for sudo"
            sudo apt -y install git-lfs
        fi
    fi

    #Install GNU make and some basic host building tools.
    if ! command -v make &> /dev/null ; then
        if needs_install "GNU make (build-essential)" "sudo apt update && sudo apt -y install build-essential"; then
            echo "Installing build-essentials for GNU make. You may be asked for you user password for sudo"
            sudo apt update -qq
            sudo apt -y install build-essential
        fi
    fi

    #Check for curl we often want this and need it for the docker install
    if ! command -v curl &> /dev/null ; then
        if needs_install "curl" "sudo apt update && sudo apt -y install curl"; then
            echo "Installing curl. You may be asked for you user password for sudo"
            sudo apt update -qq
            sudo apt -y install curl
        fi
    fi

    # Check if we have docker installed and install it if we don't
    if ! command -v docker &> /dev/null ; then
      if needs_install "Docker Engine" "see https://docs.docker.com/engine/install/ (then: sudo usermod -aG docker \$USER and re-login)"; then
        echo "Docker not found. Installing. (You may be asked to enter your user password for sudo operations)"
        sudo apt update -qq
        sudo apt -y install apt-transport-https ca-certificates curl software-properties-common
        curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo gpg --dearmor -o /usr/share/keyrings/docker-archive-keyring.gpg
        echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/docker-archive-keyring.gpg] https://download.docker.com/linux/ubuntu $(lsb_release -cs) stable" | sudo tee /etc/apt/sources.list.d/docker.list > /dev/null
        sudo apt update -qq
        apt-cache policy docker-ce # check this is coming from the docker package manager not Ubuntu
        sudo apt -y install docker-ce
        sudo systemctl status docker
        sudo usermod -aG docker ${USER}
        # Interactive only - the enclosing branch cannot be reached when
        # INX_NONINTERACTIVE=1, because needs_install returns 1 above. A blocking
        # read here is what would otherwise hang an unattended CI run.
        read -n 1 -p "${TXT_FG_RED}Docker has been installed, which requires you to reboot your machine or re-login so that new user permissions can take affect for ${USER}"
      fi
    else
        echo "Docker already installed"
    fi

fi

# Check if we have docker-compose installed and install it if we don't
warn "We're not checking the python environment for esp32 tools... find this line and enable if you want to know how..."
# Removed 2026-08: a disabled 'if [ 1 = 0 ]' block that would have installed
# python3 / pip / pyserial and an esp32 venv under /opt. Dead from the day it was
# written; the esp32 python environment comes from the build Docker image.

# Non-interactive: bail here if anything was missing, listing all of it.
report_missing_and_exit

echo "${TXT_FG_BRIGHT_GREEN}Host build tools checked.${TXT_RESET}"
