/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/
 
#ifndef __CBUFFER_H__
#define __CBUFFER_H__

#include <types.h>

typedef struct CBUFFERst
{
    char	*buf;
    u_long	size;
    u_long	write_index;
    u_long	read_index;
    u_long	item_count;
} CBUFFER;

extern error_code	cbuffer_init(CBUFFER *buffer, u_long size);
extern void			cbuffer_destroy(CBUFFER *buffer);
extern error_code	cbuffer_extract(CBUFFER *buffer, char *items, u_long count);
extern error_code	cbuffer_peek(CBUFFER *buffer, char *items, u_long count);
extern error_code	cbuffer_insert(CBUFFER *buffer, const char *items, u_long count);
extern u_long		cbuffer_get_free(CBUFFER *buffer);
extern u_long 		cbuffer_get_used(CBUFFER *buffer);
extern u_long		cbuffer_get_size(CBUFFER *buffer);

#endif //__CBUFFER_H__

