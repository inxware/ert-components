#!/bin/bash
#---------------------------------------------------------------
# Launch eRT from the Boot2Qt demo launcher (qtlauncher).
#
# qtlauncher is a Wayland COMPOSITOR - apps it launches are Wayland
# clients rendering into a surface inside it.  Two things follow:
#
# 1. QT_QPA_PLATFORM must be wayland, not the image-wide eglfs from
#    /etc/default/qt.  On eglfs the app takes DRM master directly (see
#    boot2qt-display-config.md) and fights the launcher for the display.
#
# 2. This must NOT go through appcontroller.  Without --launch,
#    appcontroller STOPS the currently running application first - and
#    here that is the launcher itself, which then dies with
#    "Could not queue DRM page flip on screen DSI1 (Permission denied)"
#    and takes the compositor socket down with it, so the app we just
#    started has nothing left to connect to.
#
#    /opt/ehs/bin/run_ehs.sh calls plain `appcontroller`, so it cannot be
#    reused here; the environment it sets up is replicated below instead.
#---------------------------------------------------------------

# Image-wide Qt defaults (QT_WAYLAND_HARDWARE_INTEGRATION etc).  appcontroller
# would normally apply these; it treats them as defaults only, never
# overriding what is already set - so the explicit values below still win.
set -a
. /etc/default/qt
set +a

export QT_QPA_PLATFORM=wayland

# qtlauncher normally passes these down to the app it starts.  Default them so
# the script also works when run by hand over ssh, and so a launcher that only
# sets one of the two still gets a usable pair.  The socket name is the
# compositor's, from /run/user/0/boot2qt-democompositor.
: "${XDG_RUNTIME_DIR:=/run/user/0}"
: "${WAYLAND_DISPLAY:=boot2qt-democompositor}"
export XDG_RUNTIME_DIR WAYLAND_DISPLAY

# The launcher warns this is compositor-side only; leaving it set makes every
# client log a spurious "not supported at client-side" line.
unset QT_IM_MODULE

# Mirrors run_ehs.sh
export QML_IMPORT_PATH=/opt/ehs/appdata/temp/imports/
export QT_FILE_SELECTORS=big
export INXWAREROOT=/opt/ehs/bin/../
export DEVMANCOREDIR="${INXWAREROOT}devman/core/"
export SYSDATA="${INXWAREROOT}sysdata/"

# run from a known place so $PWD-relative paths in the app resolve
cd /opt/ehs/bin || exit 1

exec ./ehs.exe
