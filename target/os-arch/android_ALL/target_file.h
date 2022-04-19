/** @file target_file.h
 * The target-specific declarations required to support the HAL for file services
 * are defined here. This file should only be included by hal_file.h
 *
 * @author: inx limited
 * @version: $Revision: 2662 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 *
 * Copyright (c) inx limited, 2006. All rights reserved.
 */

#include "../gnu_ALL/target_file.h"
#include <jni.h>
#include <android_native_app_glue.h>

void EhsTInitFileSystem(struct android_app* app); // copies over assets etc to real files
