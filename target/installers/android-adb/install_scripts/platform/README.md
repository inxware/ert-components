Android ehs-tree install scripts per platform
==============================================

This directory contains OS patches that are used to change target-specific OS config, scripts etc.

These scripts will setup things like MAC IDs and also contain OS initrc scripts that should be installed on particular devices.

Todo
-----
Consider moving this to the more general OS patching system (as described in current commits).
Should init.rc belong in target/envtree/android-ehs-tree/root-dir/ (or at least a specific version - like we have for linux)
In fact do we need the init.rc at all - it is only to insert a single call to the supervisor as the first line after the boo label.

 