/** @file target_mem.h
 * Declare target-specific memory functions.
 * 
 * @author: inx limited
 * @version: $Revision: 1236 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 * 
 * Copyright (c) inx limited, 2006. All rights reserved.
 */

#ifndef EHST_MEM_H
#define EHST_MEM_H

/*****************************************************************************/
/* Included files */

/*****************************************************************************/
/* Define macros  */

#define EhsTMem_alloc pvPortMalloc
#define EhsTMem_free vPortFree
//#define EhsTMem_alloc malloc
//#define EhsTMem_free free

/*****************************************************************************/
/* Define types */


/*****************************************************************************/
/* Declare global variables */


/*****************************************************************************/
/* Declare function prototypes  */

#endif /*EHST_MEM_H*/
