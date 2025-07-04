/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/



#ifndef _HAL_UTILS_H
#define _HAL_UTILS_H

#include "hal-api.h"

/* @TODO - this is impemented in EHS-kernel parse_sodl.c, move its implementation to HAL */

const char* ReadParmFile(const char* szInputLine, char* szParmsParams);


#endif /* _HAL_UTILS_H */
