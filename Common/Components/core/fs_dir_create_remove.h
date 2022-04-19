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

#ifndef _EHS_FILESYSTEMDIR_CR_H
#define _EHS_FILESYSTEMDIR_CR_H

#include "ehs_fb_types.h"

#define EHS_FB_FILESYSTEM_DIRCREATEREMOVE_NAME "FileSystemDirCreateRemove"

/* declare functions provided by this function block */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(FileSystemDirCreateRemove);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(FileSystemDirCreateRemove);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(FileSystemDirCreateRemoveCreate);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(FileSystemDirCreateRemoveRemove);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(FileSystemDirCreateRemove)

#endif /* */

