#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

#!/bin/bash

# Fairly naive calculation of dependencies.
# all lines that begin with #include " (possibly with leading spaces)
# are extracted - even if they're in #ifdef blocks
# #include <...> is ignored, because this is assumed to be system
# files that don't change.

for dir in $*
do
# change to the directory where the work is to take place
pushd $dir

echo "Generating dependencies for $dir"

# header files first
egrep -H "^[ 	]*# *include" *.h |sed 's/[ 	]*# *include *//' | grep -v ":<" | sed 's/ .*$//' | sed 's/"/ /g' | sed 's/:/ /' > $$.1
last=
echo "#" > $$.2
echo "# deps.mk - dependencies for the current makefile" >> $$.2
echo "# " >> $$.2
echo "# Automatically Generated. DO NOT EDIT" >> $$.2
echo "# " >> $$.2

cat $$.1 | while read line
do
set `echo $line`
parent=$1
child=$2
if [ "$last" != "$parent" ]
then
	echo >> $$.2
	echo  >> $$.2
	echo -n $parent :  >> $$.2
	last=$parent
fi
echo -n " $child" >> $$.2
done

# now for the .c files
egrep -H "^[ 	]*# *include" *.c |sed 's/[ 	]*# *include *//' | grep -v ":<" | sed 's/ .*$//' | sed 's/"/ /g' | sed 's/:/ /' | sed 's/\.c/\.$(OBJ)/' > $$.3
last=
cat $$.3 | while read line
do
set `echo $line`
parent=$1
child=$2
if [ "$last" != "$parent" ]
then
	echo >> $$.2
	echo  >> $$.2
	echo -n $parent :  >> $$.2
	last=$parent
fi
echo -n " $child" >> $$.2
done
#u2d $$.2
mv $$.2 deps.mk
rm $$.1 $$.3

popd
done
