#!/bin/bash

# @TODO - This should be an optional things only if a config.mk file speciies aspecific target. I would imagine now we don't hack the device's exitinf scriots and use rc.d/ we have much mor lattitude for the same thing to work on all ANdroids of a given version
check_platform(){
	echo "=========== Checking android platform ============="
	VERSION=$( $ADB shell 'getprop ro.product.model' )
	if [ "$VERSION" == "ROC-RK3566-PC" ]; then # !!!!!!!!!!!!!!!! CHANGE back to - rk356x_rock_3c_r
		echo "Target device android platform ($VERSION) matches."
	else
		echo "Target device android platform ($VERSION) doesn't match configuration (ROC-RK3566-PC)."
		exit 1
	fi
}
