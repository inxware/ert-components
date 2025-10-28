/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/** @file mv_hello.cpp
 * C++ implementation for machine vision hello world function.
 *
 * @author: inx limited
 *
 */

#include <iostream>

#warning "Compiling mv_hello.cpp - C++ file integration test"

extern "C" {
    void hello_world(void);
}

void hello_world(void)
{
    std::cout << "Hello World from MV Component HAL!" << std::endl;
}

