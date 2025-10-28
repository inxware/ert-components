/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/** @file hal_lua.c
 * Implementation of the lua support
 *
 * @author: inx limited
 *
 */

#include "hal_lua.h"
#include "globals.h"
#include "hal-api.h"
#include <lua.h>

/* set up the environement variables for LUA to find lbraries */
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
    if ( status!=0 )
    {
        lua_pop(L, 1); // remove error message
        lua_gc(L, LUA_GCCOLLECT, 0);
    }
}

