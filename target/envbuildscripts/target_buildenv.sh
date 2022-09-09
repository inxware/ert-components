#!/bin/bash
####################################################################################################
# This file script will create docker image for the target platform and give you an interactive 
# shell to check out interactive building etc. It mounts the host's file system where the ert repos are kept.  
####################################################################################################

./target/envbuildscripts/target_buildenv_run_command.sh sh -c "/bin/bash"
