#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

set -e
#make clean || : 
make all_docker || exit

make targetenv  || exit
make targetenv_version  || exit
#This may be necessary for the Android 6 and 9 versions:
#make targetenv_apk  || exit
#But this works on  Android 11:
make targetenv_apk_docker  || exit

make targetenv_android_dep_pack || exit



