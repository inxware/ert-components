#!/bin/bash
rm -f out.txt
dt=$(date '+%d/%m/%Y %H:%M:%S');
echo "$dt" > out.txt


sudo screen -L -Logfile  out.txt /dev/ttyUSB0 115200