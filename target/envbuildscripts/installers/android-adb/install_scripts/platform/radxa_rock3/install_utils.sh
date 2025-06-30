#!/bin/bash

# @TODO - This should be an optional things only if a config.mk file speciies aspecific target. I would imagine now we don't hack the device's exitinf scriots and use rc.d/ we have much mor lattitude for the same thing to work on all ANdroids of a given version
check_platform(){
	echo "=========== Checking android platform ============="
	VERSION=$( $ADB shell 'getprop ro.product.model' )
	if [ "$VERSION" == "rk356x_rock_3c_r" ]; then # !!!!!!!!!!!!!!!! CHANGE back to - rk356x_rock_3c_r
		echo "Target device android platform ($VERSION) matches."
	else
		echo "Target device android platform ($VERSION) doesn't match configuration (rk356x_rock_3c_r)."
		exit 1
	fi
}
