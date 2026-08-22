/***************************************************************
 * Copyright (C) 2008-2025 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file targetos_init.h
 * OS initialisation interface for XMOS xcore.ai FreeRTOS targets.
 *
 * @author: inx limited
 */

#ifndef EHS_TARGET_OS_INIT_H
#define EHS_TARGET_OS_INIT_H

#include "globals.h"
#include "hal-api.h"

ehs_bool EhsTOsSys_UpdateEnvironment(EhsMetaDataType *pEhsMetaData, ehs_uint8 what);
void EhsTOsSys_init(void);
void EhsTOsSys_term(void);
void EhsTOsApp_init(void);
void EhsTOsApp_term(void);
void EhsTOsApp_reset(void);
void EhsBinSearchPath(void);

#endif /* EHS_TARGET_OS_INIT_H */
