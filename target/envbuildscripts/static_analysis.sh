#!/bin/bash
cppcheck --inline-suppr -i Common/HAL/graphics/ -i target/os-arch/nxp-redlib-freertos-arm/ehs/lwip/ -i UnitTest/Parser/ -i UnitTest/CommandProcessor/ -I Common/KAPI/ -I Common/HAL/include/ -I target/os-arch/nxp-redlib-freertos-arm/ehs/lwip/src/include/ -I target/os-arch/nxp-redlib-freertos-arm/ehs/lwip/ .
