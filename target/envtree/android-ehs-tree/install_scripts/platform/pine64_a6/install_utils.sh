#!/bin/bash


check_platform(){
	echo "=========== Checking android platform ============="
	VERSION=$( $ADB shell 'getprop ro.product.model' )
	if [ "$VERSION" == "PINE A64" ]; then
		echo "Target device android platform ($VERSION) matches."
	else
		echo "Target device android platform ($VERSION) doesn't match configuration (pine64_a6)."
		exit 1
	fi
}