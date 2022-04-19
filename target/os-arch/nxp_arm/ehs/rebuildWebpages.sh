#!/bin/bash
perl ./lwip/src/apps/httpsrv/mkfs/mkfs.pl ./lwip/src/apps/httpsrv/mkfs/webpage
mv ./httpsrv_fs_data.c ./lwip/src/apps/httpsrv/httpsrv_fs_data.c
