/***************************************************************
 * Copyright (C) 2025 inx limited, UK - All Rights Reserved.
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license
 * with this file. If not, please visit:
 *  <https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

#ifndef QT_MAIN_INTEGRATION_H
#define QT_MAIN_INTEGRATION_H

#include "globals.h"

EHS_GLOBAL ehs_bool EhsTV_initQt(int argc, char ** argv);

EHS_GLOBAL void EhsTV_registerTickCallback(void);

EHS_GLOBAL int EhsTV_runQt(void);

#endif /* QT_MAIN_INTEGRATION_H */
