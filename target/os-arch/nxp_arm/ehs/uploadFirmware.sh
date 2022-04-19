#!/bin/bash
firmwareName=${1}
localFile=./Debug/frdmk64f_lwip_httpsrv_freertos.bin
strings ${localFile} |grep sdl_version
scp -P 8822 ${localFile} root@www.inx-systems.com:/home/inx-devman/bin/dldata-scripts/firmwares/${firmwareName}
