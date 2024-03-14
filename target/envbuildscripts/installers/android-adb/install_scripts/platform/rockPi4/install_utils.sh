#!/bin/bash

check_platform(){
	echo "=========== Checking android platform ============="
	VERSION=$( $ADB shell 'getprop ro.product.model' )
	if [ "$VERSION" == "rk3399" ]; then
		echo "Target device android platform ($VERSION) matches."
	else
		echo "Target device android platform ($VERSION) doesn't match configuration (rk3399."
		exit 1
	fi
}
