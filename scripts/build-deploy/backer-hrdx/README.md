Building the Binary
===================
To build a full nxp image you juat need to use 

	./configure nxp_<specific>
	make all_docker

Flashing the Image
==================
from the ert-components base simply use
	./scripts/build-deploy/backer-hrdx/flash_HRDC_eRT.sh

Just Build an image with a fiven env variable $INX_IOT_VARIANT
	./iot-make.sh 

Specific Building and Deploying Devman Servers
==============================================
To upload a hri build for the nibe server do from EHS root:
	./scripts/build-deploy/heatrod-hri-make-upload.sh

To upload a hri build for the inx server do from EHS root:
	./scripts/build-deploy/inx-hri-make-upload.sh

To upload a build to inx server for another variant do from EHS root:
	INX_IOT_VARIANT=nxp_arm_some_variant ./scripts/build-deploy/iot-make-upload.sh
