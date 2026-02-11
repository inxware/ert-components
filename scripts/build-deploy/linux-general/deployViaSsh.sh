#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

DO_EXE=1

if [ "$1" == "-console" ]; then
	WiTHCONSOLE=1
	TARGETADDR=${2}
elif [ "$1" == "-noexe" ]; then
	DO_EXE=0
	TARGETADDR=${2}

else
	TARGETADDR=${1}
fi

source TARGET.cfg

#make targetenv_deb UPLOAD="ambifier@www.ambifier.com:/home/ambifier/deb-repo"
if [ "$DO_EXE" == "1" ]; then

   ssh root@${TARGETADDR} service run_ehs stop
   sleep 2
   ssh root@${TARGETADDR} killall run_ehs.sh ; killall ehs.exe
fi

if [ 1 = 0 ]; then
echo "WARNING!!!!! Hack to send the appp is active"
ssh  root@${TARGETADDR} "mkdir -p /opt/ehs/appdata/default/" || exit
scp ../TARGET_TREES/ehs_env-${TARGET}/appdata/default/* root@${TARGETADDR}:/opt/ehs/appdata/default/ || exit
fi

echo "copying ehs.exe ..." 
scp ../TARGET_TREES/ehs_env-${TARGET}/bin/ehs.exe root@${TARGETADDR}:/opt/ehs/bin/ || exit
echo "Done"

#Hack this to install the debian package
if [ 1 = 0 ]; then
if [ -f  ../TARGET_TREES/ehs_deb-${TARGET}/*.deb ]; then
# thenls ../TARGET_TREES/ehs_deb-${TARGET}/*.deb  ]; then
echo "copying debian_package ..." 
scp ../TARGET_TREES/ehs_deb-${TARGET}/*.deb root@${TARGETADDR}:/opt/ehs/ || exit
echo "Done"
fi
fi

if [ "$DO_EXE" = "1" ]; then
  if [ "$WITHCONSOLE" = "1" ]; then
    ssh root@${TARGETADDR} "cd /opt/ehs/bin && ./run_ehs.sh NO_RESTART LIB_HOST"
  else
#    ssh root@${TARGETADDR} "service run_ehs start"
    ssh root@${TARGETADDR} "cd /opt/ehs/bin && export DISPLAY=:0; export LD_LIBRARY_PATH=/opt/ehs/bin/cslib && ./ehs.exe"
  fi
fi
