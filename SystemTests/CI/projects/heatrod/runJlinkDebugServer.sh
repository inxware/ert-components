#!/bin/bash
/opt/SEGGER/JLink_V646j/JLinkGDBServerCLExe -nosilent -swoport 2332 -select USB=611000000  -telnetport 2333 -singlerun -endian little -noir -speed auto   -port 2331    -vd -device MK64FN1M0xxx12 -if SWD -halt -reportuseraction
