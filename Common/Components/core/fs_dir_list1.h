/** 
 * @file dtv_pvr_list.h
 * This declares the PVR functions for the DTV toolkit. See
 * target-specific components directory for the implementation of this. 
 *
 * @author: inx limited
 * @version: $Revision: 1087 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 * 
 * Copyright (c) inx limited, 2007. All rights reserved.
 */

#ifndef _EHS_FILESYSTEMDIR_LIST_H
#define _EHS_FILESYSTEMDIR_LIST_H

#include "ehs_fb_types.h"

#define EHS_FB_FILESYSTEM_DIRLIST_NAME "FileSystemDirList1"
#define EHS_FB_FILESYS_LIST_SET_NAME "set"
#define EHS_FB_FILESYS_LIST_GET_NAME "get"
#define EHS_FB_FILESYS_LIST_REL_NAME "rel"
//#define EHS_FB_FILESYS_LIST_RESET_NAME "reset"


/* declare functions provided by this function block */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(FileSystemDirList1);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(FileSystemDirList1);
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(FileSystemDirList1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(FileSystemDirSet1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(FileSystemDirGet1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(FileSystemDirRelease1);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(FileSystemDirList1)

#endif /* */

