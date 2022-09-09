/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

//BEGIN_INCLUDE(all)


#include <jni.h>
#include <errno.h>

#include <EGL/egl.h>
#include <GLES/gl.h>

#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>

#include "target_file.h"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))

GLfloat vertices[] = {0,0,0, 0,1,0, 1,0,0, 0,1,0, 1,1,0, 1,0,0,
                      -1,0,0, 0,0,0, -1,-1,0, -1,-1,0, 0,-1,0, 0,0,0,
                      0,0,0, -1,0,0, -1,1,0, 0,0,0, -1,1,0, 0,1,0,
                      0,0,0, 1,0,0, 1,-1,0, 0,0,0, 0,-1,0, 1,-1,0
                      };

GLfloat texCoord[] = {0,0, 0,1, 1,1, 1,0};

unsigned char redChar[4];
unsigned char greenChar[4];
unsigned char blueChar[4];

int textures[4];

struct engine
{
    struct android_app* app;
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
    int32_t width;
    int32_t height;
};


void generateTextures(int n, int *tex)
{

    redChar[0]=255;
    greenChar[0]=0;
    blueChar[0]=0;
    redChar[1]=0;
    greenChar[1]=255;
    blueChar[1]=0;
    redChar[2]=0;
    greenChar[2]=0;
    blueChar[2]=255;
    redChar[3]=255;
    greenChar[3]=255;
    blueChar[3]=0;

    char bitmap[64][64][4];
    char *array = &bitmap[0][0][0];

    ////generate IDs
    glGenTextures(n, tex);

    int i = 0;
    for(i=0; i<n; i++)
    {

        ///load bitmap into memory
        int j=0;
        int k=0;
        for(j=0; j<64; j++)
        {
            for(k=0; k<64; k++)
            {
                bitmap[j][k][0] = redChar[i];
                bitmap[j][k][1] = greenChar[i];
                bitmap[j][k][2] = blueChar[i];
                bitmap[j][k][3] = 255;
            }
        }

        ///bind bitmap
        glBindTexture(GL_TEXTURE_2D, tex[i]);

        ///set up texture parameters
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        ///create textures
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, array);

    }
}
/*
void render(int n, int *tex, GLfloat *texCoordinates, GLfloat *vertexCoordinates){

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    int i=0;

    for(i=0; i<n; i++){
    	glBindTexture(GL_TEXTURE_2D, tex[i]);
    	glEnableClientState(GL_VERTEX_ARRAY);
    	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    	glVertexPointer(3, GL_FLOAT, 0, vertexCoordinates);
    	glTexCoordPointer(2, GL_FLOAT, 0, texCoordinates);
    	glDrawArrays(GL_TRIANGLES, i*6, 6);
    	glDisableClientState(GL_VERTEX_ARRAY);;
    	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
}
*/

static int engine_init_display(struct engine* engine)
{


    const EGLint attribs[] =
    {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_BLUE_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_RED_SIZE, 8,
        EGL_NONE
    };

    EGLint w, h, dummy, format;
    EGLint numConfigs;
    EGLConfig config;
    EGLSurface surface;
    EGLContext context;

    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(display, 0, 0);
    eglChooseConfig(display, attribs, &config, 1, &numConfigs);
    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);
    ANativeWindow_setBuffersGeometry(engine->app->window, 0, 0, format);
    surface = eglCreateWindowSurface(display, config, engine->app->window, NULL);
    context = eglCreateContext(display, config, NULL, NULL);

    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE)
    {
        LOGW("Unable to eglMakeCurrent");
        return -1;
    }

    eglQuerySurface(display, surface, EGL_WIDTH, &w);
    eglQuerySurface(display, surface, EGL_HEIGHT, &h);

    engine->display = display;
    engine->context = context;
    engine->surface = surface;
    engine->width = w;
    engine->height = h;

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    generateTextures(4, textures);

    return 0;
}



static void engine_draw_frame(struct engine* engine)
{
    if (engine->display == NULL)
    {
        return;
    }

    //render(3, textures, texCoord, vertices);
    eglSwapBuffers(engine->display, engine->surface);
}


/**
 * Process the next main command.
 */
static void engine_handle_cmd(struct android_app* app, int32_t cmd)
{
    struct engine* engine = (struct engine*)app->userData;
    switch (cmd)
    {
    case APP_CMD_INIT_WINDOW:
        // The window is being shown, get it ready.
        if (engine->app->window != NULL)
        {
            engine_init_display(engine);
            engine_draw_frame(engine);
        }
        break;
    }
}

void android_main(struct android_app* state)
{
    struct engine engine;

    // Make sure glue isn't stripped.
    app_dummy();
    EhsTInitFileSystem(state);

    memset(&engine, 0, sizeof(engine));
    (struct engine)state->userData = &engine;
    state->onAppCmd = engine_handle_cmd;
    //state->onInputEvent = engine_handle_input;
    engine.app = state;



    while (1)
    {
        // Read all pending events.
        int ident;
        int events;
        struct android_poll_source* source;

        // If not animating, we will block forever waiting for events.
        // If animating, we loop until all events are read, then continue
        // to draw the next frame of animation.
        while ((ident=ALooper_pollAll(-1, NULL, &events,
                                      (void**)&source)) >= 0)
        {
            // Process this event.
            if (source != NULL)
            {
                source->process(state, source);
            }
        }
    }
}
