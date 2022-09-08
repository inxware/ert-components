/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

/* @file target_main.h
 *
 * @author: inx limited
 *
 */
#ifndef TARGET_MAIN_H_
#define TARGET_MAIN_H_

#include <EGL/egl.h>
#include <GLES/gl.h>
#include <android/log.h>
#include <android_native_app_glue.h>

//~todo move this to the target_Main.h if there is such a thing
struct engine
{
    struct android_app* app;
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
    int32_t width;
    int32_t height;
};

#endif /* TARGET_MAIN_H_ */
