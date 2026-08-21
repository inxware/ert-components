
#Default dependencies and feature support for MinGW x86 targets.

EHS_COMMS_API_SUPPORT                ?= winsock

# Default network features for Windows targets
EHS_COMPONENTS_NETWORK_URL_GET       ?= enabled

EHS_IMAGEPROCESSING_APRILTAG_SUPPORT ?= enabled

EHS_PERIPHERALS_UART_SUPPORT         ?= win32

# New peripheral HAL defaults — stub on MinGW/Windows targets
EHS_WATCHDOG_SUPPORT      ?= stubbed
EHS_UPS_SUPPORT           ?= stubbed
EHS_BUZZER_SUPPORT        ?= stubbed
EHS_SD_SELECT_SUPPORT     ?= stubbed
EHS_USB_POWER_SUPPORT     ?= stubbed
EHS_PERIPHERALS_ACCEL_GYRO_SUPPORT ?= stubbed
EHS_RS485_CONFIG_SUPPORT  ?= stubbed

# EHS_GNU_OS must be mingw32posix for BOTH 32- and 64-bit Windows. It is the OS
# field of the GNU triplet — the canonical triplet for 64-bit Windows is
# x86_64-w64-mingw32 — so "mingw32" names the OS, not the word size, which
# EHS_GNU_ARCH carries (i686 or x86_64).
#
# Guarded because the failure is silent: several .mk files compare EHS_GNU_OS
# against this literal to select the Windows code paths (gnu_ALL/toolchain.mk,
# graphics/gtk, graphics/qt, comms/winsock, mqtt/aws_green_grass,
# usercomponents.mk). Any other value selects the Linux/X11 branch instead and
# the build dies much later, at link, on missing ws2_32 symbols.
#
# MSYS2 calls its 64-bit package repository "mingw64". That is an environment
# name, not the triplet field.
ifneq ($(EHS_GNU_OS),mingw32posix)
    $(error EHS_GNU_OS is '$(EHS_GNU_OS)' but must be 'mingw32posix' for all \
mingw targets, 32- and 64-bit alike. The word size belongs in EHS_GNU_ARCH \
(i686 or x86_64).)
endif

# Default packager for this os-arch. A platform config.mk may override it with a
# plain '=' -- this file is included after the platform's own config (see
# target/platform/platform.mk, "include $(EHS_TARGET_OS_HW_PATH)/config.mk"),
# so '?=' here yields to the platform. 'make targetenv_package' dispatches on it.
EHS_PACKAGER_TYPE ?= nsis
