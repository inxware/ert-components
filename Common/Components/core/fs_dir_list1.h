/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/**
 * @file dtv_pvr_list.h
 * This declares the PVR functions for the DTV toolkit. See
 * target-specific components directory for the implementation of this.
 *
 * @author: inx limited
 *
 */

#ifndef _EHS_FILESYSTEMDIR_LIST_H
#define _EHS_FILESYSTEMDIR_LIST_H

#include "ehs_fb_types.h"


#define EHS_FB_NAME_FileSystemDirList1 "FileSystemDirList1"
#define EHS_FB_ID_FileSystemDirList1 0x4186


/* declare functions provided by this function block */
EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(FileSystemDirList1);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(FileSystemDirList1);
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(FileSystemDirList1);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(FileSystemDirSet1);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(FileSystemDirGet1);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(FileSystemDirRelease1);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(FileSystemDirList1)

#endif /* */

