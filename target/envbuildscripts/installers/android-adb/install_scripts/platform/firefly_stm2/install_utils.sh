#!/bin/bash

# @TODO - This should be common for all androids (copare them all) and instead we should a variable for "Station M2" set in the config.mk file
check_platform(){
	echo "=========== Checking android platform ============="
	VERSION=$( $ADB shell 'getprop ro.product.model' )
	if [ "$VERSION" == "Station M2" ]; then # !!!!!!!!!!!!!!!! CHANGE back to - rk356x_rock_3c_r
		echo "Target device android platform ($VERSION) matches."
	else
		echo "\033[0;31m ERROR! : Target device android platform ($VERSION) doesn't match configuration (ROC-RK3566-PC)."
		exit 1
	fi
}
