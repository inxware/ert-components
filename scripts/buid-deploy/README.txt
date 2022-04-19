To upload a hri build for the nibe server do from EHS root:
./scripts/buid-deploy/heatrod-hri-make-upload.sh

To upload a hri build for the inx server do from EHS root:
./scripts/buid-deploy/inx-hri-make-upload.sh

To upload a build to inx server for another variant do from EHS root:
INX_IOT_VARIANT=nxp_arm_some_variant ./scripts/build-deploy/iot-make-upload.sh