/** @file target_process.c
 * description
 *
 * @author: inx limited
 * @version: $Revision: 2731 $
 * @date: $Date$
 *
 * Copyright (c) inx limited, 2007. All rights reserved.
 */

#include "../gnu_ALL/target_process.c"

//@todo this shouldn't be here...

size_t mbstowcs(wchar_t *dest, const char *src, size_t n) {
EhsStrcpy(dest, src); //@todo - just to do something...
}
