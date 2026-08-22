/***************************************************************
 * Copyright (C) 2008-2025 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file target_mem.h
 * Target-specific memory allocation for XMOS xcore.ai FreeRTOS targets.
 *
 * @author: inx limited
 */

#ifndef EHST_MEM_H
#define EHST_MEM_H

#include <stdlib.h>

#define EhsTMem_alloc malloc
#define EhsTMem_free  free

#endif /* EHST_MEM_H */
