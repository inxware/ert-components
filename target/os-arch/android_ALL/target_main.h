/*
 * target_main.h
 *
 *  Created on: 24 Jul 2014
 *      Author: pdrezet
 */

#ifndef TARGET_MAIN_H_
#define TARGET_MAIN_H_

#include <EGL/egl.h>
#include <GLES/gl.h>
#include <android/log.h>
#include <android_native_app_glue.h>

//~todo move this to the target_Main.h if there is such a thing
struct engine {
   struct android_app* app;
   EGLDisplay display;
   EGLSurface surface;
   EGLContext context;
   int32_t width;
   int32_t height;
};

#endif /* TARGET_MAIN_H_ */
