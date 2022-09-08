/** @file target_file.h
 * The target-specific declarations required to support the HAL for file services
 * are defined here. This file should only be included by hal_file.h
 *
 * @author: inx limited
 *
 */

#include "../gnu_ALL/target_file.h"
#include <jni.h>
#include <android_native_app_glue.h>

void EhsTInitFileSystem(struct android_app* app); // copies over assets etc to real files
