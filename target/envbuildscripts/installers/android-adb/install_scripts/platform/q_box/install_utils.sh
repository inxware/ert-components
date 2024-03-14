#!/bin/bash

check_platform(){
	echo "=========== Checking android platform ============="
	VERSION=$( $ADB shell 'getprop ro.product.model' )
	if [ "$VERSION" == "MBOX" ]; then
		echo "Target device android platform ($VERSION) matches."
	else
		echo "Target device android platform ($VERSION) doesn't match configuration (MBOX)."
		exit 1
	fi
}
