#!sh
# P Drezet 2008
echo #######  Building win_x86 target
#Check that dependencies are present

if [ -n "$2" ] 
then # the target envi name is the same as the source from prepdeps.
SPECIFIC_TARGET=$2;
echo Creating Target path to specific PATH =$SPECIFIC_TARGET
else 
SPECIFIC_TARGET=$1;
echo Creating Target path same as source target path =$SPECIFIC_TARGET
fi

if test -e ../OTHER_DEPENDENCIES/VLC/$1; then printf "VLC present - OK\n"; 
else printf "VLC not present - Use make TARGET="$1" prepdeps\n"; exit;
fi


#this target uses the canonical directory structure - so make it first
./target/envbuildscripts/targetenv_directories.sh $1
svn --force export ./target/envtree/win32-ehs-tree/root-ehs_dir/ ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/
svn --force export ./target/envtree/linux-ehs-tree/devman/core-ehs_dir/ ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/
svn --force export ./target/envtree/win32-ehs-tree/devman/plugins/player-ehs_dir/ ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/


#@todo VLC windows doesn't like the windows canonical structure - so rename the appdata directory so it doesn't get used by EHS.
#mv  ../TARGET_TREES/ehs_env-$1/appdata ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/appdata_X

# Populate directory structure for the target
if test -e ehs_$1.exe; then 
	printf "ehs_"$1".exe present - OK\n"; 
	cp ehs_$1.exe ../TARGET_TREES/ehs_env-$1/bin/ehs.exe
else 
	echo ehs_$1.exe not presenting - please build using make TARGET=$1 - Exiting ... 
exit
fi


echo Copying VLC binaries to target tree at: ../TARGET_TREES/ehs_env-$1/bin/
svn --force export ../OTHER_DEPENDENCIES/VLC/$1/ ../TARGET_TREES/ehs_env-$1/bin/
#remove annoying dead dll that sometimes appears in vlc if it is in the repo.
if test -e ../TARGET_TREES/ehs_env-$1/bin/modules/libdtstospdif_plugin.dll; then
	rm ../TARGET_TREES/ehs_env-$1/bin/modules/libdtstospdif_plugin.dll
fi
echo Done

#the following will be depreicated when Lua is properly integrated

./target/envbuildscripts/add_lua.sh $1 $SPECIFIC_TARGET
# The following is not needed: ./target/envbuildscripts/add_lua_$1.sh $1 $SPECIFIC_TARGET

echo making all bin files executable
chmod -R a+rwx ../TARGET_TREES/ehs_env-$1/bin/
echo Done
