Scipts for updating device's OS via Devman
==========================================

These seem to be scripts supplied to Devman (with package updated) that can upgrade supervisor and 
other OS level patches that need a read/write system partition to apply. Patches are applied to root.

General approach is:
--------------------
source .../ehs_utils.sh : Use the devies pre-installed methods.
InstallApkUpdates()     : Calls Devices's InstallApp() Utility from the platform to install any APKs it finds in the unpacked directory path arguement.
InstallSupervisorUpdates() : remounts root and installs scripts (e.g. supervisor scripts)

The script calls the above after untarring the upate zip file into a temporary directory on the device
and then reboots the device with the device's specific reboot command. 


Todo
----
We don't need one of these for every target - they are all the same, except it
seems  a6 (Android 7 needs to remount /system and 9 remounts / 
 -> We should just have directories for Android versions here and choose based on ANDROID_VERSION 

