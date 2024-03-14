#!/bin/bash

check_platform(){
	echo "=========== Checking android platform ============="
	VERSION=$( $ADB shell 'getprop ro.product.model' )
	if [ "$VERSION" == "QUAD-CORE H6 petrel-p1" ]; then
		echo "Target device android platform ($VERSION) matches."
	else
		echo "Target device android platform ($VERSION) doesn't match configuration (QUAD-CORE H6 petrel-p1)."
		exit 1
	fi
}
