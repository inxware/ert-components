#!/bin/bash
# P Drezet 2008

SPECIFIC_TARGET=$2

if test -e ../OTHER_DEPENDENCIES/lua/target_binaries/$1; then printf "lua present - OK\n"; 
else printf "lua not present - Check "$1" binaries exist under ../OTHER_DEPENDENCIES/lua \n Exiting"; 
exit;
fi
echo Copying Lua binaries to target tree at: ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/
mkdir -p ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/csdir/lua/lib

svn --force export ../OTHER_DEPENDENCIES/lua/target_binaries/$1/bin/ ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/csdir
svn --force export ../OTHER_DEPENDENCIES/lua/target_binaries/$1/lua/5.1/ ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/csdir/lua/
svn --force export ../OTHER_DEPENDENCIES/lua/target_binaries/$1/lib/5.1/ ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/csdir/lua/lib
mv ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/csdir/lua.exe ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/lusys.exe 
echo Done
