# eRT's ./target/envtree Directory

This directory should contain miscellaneous assets and scripts that may be copied to
devices for runtime or possibly first install installation (via targetOS init folder).

The base directories are arranged by OS type (but ot strictly the same naming convention as ./target/os-arch).

The directroy ./target/envtreee/PLATFORM-SPECIFIC is for platform-specific runtime assets that are applied after the os-arch-specific ones.

Firstly a generic set of config and directories is copied to the staging directory (../TARGET_TREES/) after 

    make targetenv

copies from 

    ./target/envtree/Generic-ehs-tree/root-ehs_dir/root-ehs_dir/*

then OS-specific assets are copied from  is identified in the ./target/os-arch/

    ./target/envtree/${EHS_OS}-ehs-tree/root-ehs_dir/root-ehs_dir/*

Then if there is a devman scripts or configuration (e.g. OS-level connection) at 

    ./target/envtree/${EHS_OS}-ehs-tree/root-ehs_dir/devman/*

Finally there may be some first boot install scripts to be installed from 

    ./target/envtree/${EHS_OS}-ehs-tree/root-ehs_dir/system/HostIOSInit/

which would be run on firdt boot (see spefific README.md for each OS type)

# TODO
- Consider moving the android platform specific scripts that are copied to devices into the ./target/envtreee/PLATFORM-SPECIFIC folder. 
