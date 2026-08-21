#!/bin/bash
mkdir INX
cd INX
echo You will need to install the MinGW toolchain on this machine to build the Windows variants:
sudo apt-get install gcc-mingw32 mingw32-binutils


#test -f SystemTests/CI/validate_all.sh || svn co svn://server/LucidSource/branches/INX/SystemTests
#test -f EHS || svn co svn://server/LucidSource/branches/INX/EHS
#cd SystemTests/CI && ./start_CI.sh



