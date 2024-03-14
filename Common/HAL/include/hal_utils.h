/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/



#ifndef _HAL_UTILS_H
#define _HAL_UTILS_H

#include "hal-api.h"

/* @TODO - this is impemented in EHS-kernel parse_sodl.c, move its implementation to HAL */

const char* ReadParmFile(const char* szInputLine, char* szParmsParams);


#endif /* _HAL_UTILS_H */
