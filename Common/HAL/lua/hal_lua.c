/** @file hal_lua.c
 * Implementation of the lua support
 *
 * @author: inx limited
 * @version: $Revision: 1491 $
 * @date: $Date$
 * 
 * Copyright (c) inx limited, 2007. All rights reserved.
 */

#include "hal_lua.h"
#include "ehs_types.h"
#include "hal-api.h"
#include <lua.h>

/* set up the environement variables for LUA to find outs lbraries */
ehs_bool EhsHLuaInit()
{
	ehs_char buf[EHS_MAXPATHLENGTH];
	EhsStrcpy(buf,EhsHMetaGetInstPath());
	//EhsTConvertPathFormatToOS(buf); @todo must implement this!
#ifdef EHS_WIN32_RUNENV
	EhsStrcat(buf,"csdir/lua/scripts/?.lua;;");
#else
	EhsStrcat(buf,"csdir\\lua/scripts\\?.lua;;");
#endif
	setenv("LUA_PATH",buf); //buffer is copied by setenv

	EhsStrcpy(buf,EhsHMetaGetInstPath());
#ifdef EHS_WIN32_RUNENV
	EhsStrcat(buf,"csdir\\lua\\lib\\?.dll;;");
#else
	EhsStrcat(buf,"csdir/lua/lib/?.so;;");
#endif
	setenv("LUA_CPATH",buf);
}

void LuaReportErrors(lua_State *L, int status)
{
  	if ( status!=0 ) {
    	printf("-- %s\n",lua_tostring(L, -1));
    	lua_pop(L, 1); // remove error message
    	lua_gc(L, LUA_GCCOLLECT, 0);
  	}
}

