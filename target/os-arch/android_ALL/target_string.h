#ifndef TARGET_STRING_ANDROID
#define TARGET_STRING_ANDROID
/** @file target_string.h
 * The target-specific declarations required to support the HAL for string services
 * are defined here. This file should only be included by hal_string.h
 *
 * @author: inx limited
 *
 */

#include "../gnu_ALL/target_string.h"

//@todo hack for missing function needed  by libarchive

size_t mbstowcs(wchar_t *dest, const char *src, size_t n) ;
#endif
