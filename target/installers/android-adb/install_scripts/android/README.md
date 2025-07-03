Android OS configuration via ADB
=================================
Development host and install-only target scripts used to set up the Android OS and installing the Supervisor system.

Variations are required for different versions of Android

Runs on Development HOST:
install_utils.sh : Does things like creates MAC address files
install.sh       : Ibstalls some scripts on the target (and uses them to set the default volume)

Runs on the TARGET:
ehs_utils.sh     : Utility functions that are specific to OSs
ehs_scripts/*    : some more utilities that seem to be purpose specific

