# The ./target/envtree/PALTFORM-SPECIFIC Directory

This dierectory should contain runtime config and scripts that are specific 
to particular platforms and applications. This allows for more fine grained configuration 
beyond the basic os-arch level OS environment scripts provided in the other directories in this folder.

The contents of this directory are copied to the staging directory of the platform after

    make targetenv

If the __./target/platform/config.mk__ file contains a variable __$SYSTEM_VARIANT__ this will 
trigger copying more specific assets to the device after the more generic assers have been copied.

Any platform specific assets should be located as

    ./target/envtree/PLATFORM-SPECIFIC/${SYSTEM_VARIANT}/

and (also optionally) if there are any application specic configurations too these should be located at

    ./target/envtree/PLATFORM-SPECIFIC/${SYSTEM_VARIANT}-{EHS_PRODUCT_NAME}/

# TODO
- Consider using $PRODUCTNAME more widely across os-arches and not just for the android targets as this provides another layer of 
strucyred granualarity that might simply for the hacks (and have less requirement for scripted hacks)
