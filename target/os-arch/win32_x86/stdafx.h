/* 
 stdafx.h : include file for standard system include files,
 or project specific include files that are used frequently, but
 are changed infrequently
 */


#pragma once

#ifndef WINVER				/* Allow use of features specific to Windows XP or later. */
#define WINVER 0x0501		
#endif

#ifndef _WIN32_WINNT		/* Allow use of features specific to Windows XP or later */
#define _WIN32_WINNT 0x0501	
#endif						

#ifndef _WIN32_WINDOWS		/* Allow use of features specific to Windows 98 or later. */
#define _WIN32_WINDOWS 0x0410 
#endif

#ifndef _WIN32_IE			/* Allow use of features specific to IE 6.0 or later. */
#define _WIN32_IE 0x0600	
#endif

#define WIN32_LEAN_AND_MEAN		/* Exclude rarely-used stuff from Windows headers */

/* Windows Header Files: */
#include <windows.h>
#include <commctrl.h>
#include <winsock2.h>

/* C RunTime Header Files */
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
