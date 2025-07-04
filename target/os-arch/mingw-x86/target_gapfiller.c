/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/*
 * Lots of bits and bob not implemented by mingw though the headers don't tell you!
 */

#include <stdlib.h>
#include <windows.h>


char setenv (char *Name, char * Value)
{
    SetEnvironmentVariable(Name,Value);
}
