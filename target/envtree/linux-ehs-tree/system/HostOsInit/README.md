# Using the HostOSInit Directory
The HostOSInit system is for self-applied OS patches on first boot to complete the installation process of a new target.
The purpose for this may include
- Patching the OS for an application specific requirement
- applying serialisation etc. that may need device specific information extracted at runtime

This directory should contain bash scripts and can use intial numbering scehemes to ensure a specific order of applications.

One all the scripts have been run they will not be re-run again and the device will be rebooted.

The install scripts are not deleted by default (though a final script to do this could be created)

The scripts will only be run on first boot of a new OS and __will be repeated for any update scripts__. 
There is no mechanism curently to avoid repeatition of very first boot and subsequent updated other than 
creating a new platform for updates to initial installs.

e.g. ./target/platform/<YOUR PLATFORM>/config.mk
can select OS initi scripts from here using the following directives

    HOST_OS_CONFIG_SCRIPTS+= \
    0200-snd-add-csound-volume \


## Todo
- Consider if the Android configuration install scripts can be brought into this structure. Android currently has it's own methods.

