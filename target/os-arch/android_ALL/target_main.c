
/** @file 
 * target_main.c
 * This file represents the main entry point for the target. Typically
 * it also incldes an event handler, should the target require one.
 * 
 * @author: inx limited
 * @version: $Revision: 1272 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 * 
 * Copyright (c) inx limited, 2006. All rights reserved.
 */

/**
 * @page Verification Verification report
 * @section target_main
 * @anchor target_main
 * @subsection misra MISRA compliance:
 * test.c demonstrated MISRA compliant on 2007-10-12
 * Last modified on $Date$
 *
 * This file contained no derogations to the MISRA standard.
 *
 * Note it is necessary to replace <sys/types.h> with <types.h> to lint this file successfully.
 */

/**
 * Provides access to the target-specific declarations of header files
 */
#define EHS_TARGET_CODE

/*****************************************************************************/
/* Included files */
#include <sys/types.h>
#include <signal.h>

#include <unistd.h>

#include "target.h"
#include "console_server.h"
#include "globals.h"
#include "ehs_main.h"
#include "hal-api.h" // required for the meta data storage
//#include "keypress.h" /* todo kbdhit should be available in hal.h? */
#ifdef EHS_GUI_SUPPORT
#include "targetgfx_init.h"
#endif
#ifdef EHS_WEBKIT_SUPPORT
#include "target_webkit.h"
#endif

#if defined(EHS_MEDIA_SUPPORT) && !defined(EHS_UNITY3D_WIDGETS)
#include "target_os_dtv.h"
#endif

#include "target_main.h"
#include "target_file.h"
/* Androgid bits */
#include <jni.h>
#include <android_native_app_glue.h>

#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifdef EHS_GRAPHICS
#include "hal_viewport.h"
#endif


/* OpenGL1 stuff
//todo moce this to the relevant components
#ifdef EHS_ANDROID_9
#include <EGL/egl.h>
#include <android/sensor.h>
x#endif
#include <GLES/gl.h>
*/

//@todo - look at CPU runtime option testing at runtime (NDK hello-beon sample app).
//#define OPENGL1_1
#ifdef MOVEME

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif


/*****************************************************************************/
/* Declare macros and local typedefs used by this file */


#define EHSL_MODULE_ID EHSH_LOG_MODULE_HAL_FILE
/*****************************************************************************/
/* Declare prototypes of local functions */
/*****************************************************************************/
/* Variables defined with file-scope */


//#ifdef EHS_ANDROID9 //has native activity

//#else //use our jni entry point
//void * ehs_android_main(void * );
//#error shouldnt be here
//#endif

/*
 * Some test graphics to check our binaries are OK
 */

/*
 * Finished test graphics
 */


int EHS_ANDROID_width = EHS_CONFIG_DISPLAY_WIDTH; //default to the platform config value
int EHS_ANDROID_height = EHS_CONFIG_DISPLAY_HEIGHT;

/**
 * Handle the SIGTERM signal
 */
EHS_LOCAL void EhsTargetHandleTerm(int);

/*****************************************************************************/
/* Variables defined with global-scope */
/*****************************************************************************/
/* Function definitions */

/* Delete this - no longer used?
void* EhsL_server(void* pDummy) {
	EHSH_LOG_INFO("server started up\n");
	EhsSvcTcp_server(NULL);
}
*/

#define EHS_MOVE_IGNORE_VAL 5
// see https://wiki.linuxquestions.org/wiki/List_of_Keysyms_Recognised_by_Xmodmap for xmod map


/* This handles quite alot of things - some to do with app lifecycle - but mostly to do with IO  - todo contennts of this should be kept somwhere else */
int32_t engine_handle_input(struct android_app* app, AInputEvent* event) {
	int32_t action,source,x,y;
	int32_t key_val;
	int32_t eventType = AInputEvent_getType(event);
	static int32_t last_x = -1;
	static int32_t last_y = -1;

	// try this to caps: event.shiftKey
	switch(eventType){
	case AINPUT_EVENT_TYPE_KEY:
		LOGI("Inout Event Type : AMOTION_EVENT_ACTION_UP");

		action = AKeyEvent_getAction(event) & AMOTION_EVENT_ACTION_MASK;
		if (action ==  AMOTION_EVENT_ACTION_UP) {
			key_val = AKeyEvent_getKeyCode(event);
			switch (key_val ) {
			case AKEYCODE_BACK:
			/* what does this do?relaod app if the back UI button is pressed (not delete-back I hope, we need this?)*/
				EhsHFSMForceInternallyRequestedCommand(EHS_RELOAD_EHS_FROM_FILE); //@todo - any way to check for success?
				break;
			default:
				break;
			}
		}
		else if (action ==  AMOTION_EVENT_ACTION_DOWN) {

			key_val = AKeyEvent_getKeyCode(event);
#ifdef EHS_GUI_SUPPORT

			LOGI("Input Event Type : AMOTION_EVENT_ACTION_DOWN = %d",key_val);
			switch (key_val ) {
			case AKEYCODE_SOFT_LEFT:
			case AKEYCODE_DPAD_LEFT:
				#ifdef EHS_GUI_SUPPORT
					EhsGUIKbHit(0xff51); /* left */
				#endif
				break;
			case AKEYCODE_SOFT_RIGHT:
			case AKEYCODE_DPAD_RIGHT:
				#ifdef EHS_GUI_SUPPORT
					EhsGUIKbHit(0xff53); /* right */
				#endif
				break;
			case AKEYCODE_BACK:
			/* what does this do?relaod app if the back UI button is pressed (not delete-back I hope, we need this?)*/
				EhsHFSMForceInternallyRequestedCommand(EHS_RELOAD_EHS_FROM_FILE); //@todo - any way to check for success?
				break;
			case AKEYCODE_DPAD_UP:
			case AKEYCODE_VOLUME_UP:
				#ifdef EHS_GUI_SUPPORT
					EhsGUIKbHit(0xFF52); /* right */
				#endif
				break;
			case AKEYCODE_VOLUME_DOWN:
			case AKEYCODE_DPAD_DOWN:
				#ifdef EHS_GUI_SUPPORT
					EhsGUIKbHit(0xFF54); /* right */
				#endif
				break;
			case AKEYCODE_HOME:
			#ifdef EHS_GUI_SUPPORT
					EhsGUIKbHit(0xFF50); /* right */
				#endif
				break;
			case AKEYCODE_STAR:
				EhsGUIKbHit('*');
				break;
			case AKEYCODE_POUND:
				EhsGUIKbHit('#');
				break;
			case AKEYCODE_TAB:
				EhsGUIKbHit(0xFF09);
				break;
			case  AKEYCODE_SPACE:
				EhsGUIKbHit(' ');
				break;
			case  AKEYCODE_EXPLORER:
				break;

			case  AKEYCODE_ENTER:
				//EhsGUIKbHit(0xFF0A);// linefeed
				EhsGUIKbHit(0xFF0D);// return
				break;
			case  AKEYCODE_DEL:
			//case  AKEYCODE_BACK:
				EhsGUIKbHit(0xFF08);
				break;

			case  AKEYCODE_GRAVE:
				EhsGUIKbHit('`');
				break;
			case  AKEYCODE_MINUS:
				EhsGUIKbHit('-');
				break;
			case  AKEYCODE_EQUALS:
				EhsGUIKbHit('=');
				break;
			case  AKEYCODE_LEFT_BRACKET :
				EhsGUIKbHit('(');
				break;
			case  AKEYCODE_RIGHT_BRACKET :
				EhsGUIKbHit(')');
				break;
			case  AKEYCODE_BACKSLASH :
				EhsGUIKbHit('\\');
				break;
			case  AKEYCODE_SEMICOLON :
				EhsGUIKbHit(';');
				break;
			case   AKEYCODE_APOSTROPHE :
				EhsGUIKbHit('\'');
				break;
			case   AKEYCODE_SLASH :
				EhsGUIKbHit('/');
				break;
			case  AKEYCODE_AT :
				EhsGUIKbHit('@');
				break;
			case  AKEYCODE_COMMA :
				EhsGUIKbHit(',');
				break;
			case   AKEYCODE_PERIOD :
				EhsGUIKbHit('.');
				break;
/* dump these for now */
			case AKEYCODE_CALL:
			case AKEYCODE_ENDCALL:
			case AKEYCODE_DPAD_CENTER:
			case AKEYCODE_POWER:
			case AKEYCODE_CAMERA:
			case AKEYCODE_CLEAR:
				#ifdef EHS_GUI_SUPPORT
			// we hav no idea wa these mean in terms of GTK codes EHS uses		EhsGUIKbHit(key_val);
				#endif
				break;
			default:
				if((key_val >= AKEYCODE_F1 ) && ( key_val <= AKEYCODE_F12))
				{
					#ifdef EHS_GUI_SUPPORT
						EhsGUIKbHit(key_val-AKEYCODE_F1+0xFFBE);
					#endif
				}
				else if ((key_val >= AKEYCODE_A ) && (key_val <= AKEYCODE_Z )) {
					#ifdef EHS_GUI_SUPPORT
						EhsGUIKbHit(key_val-AKEYCODE_A+'a');
					#endif
				}
				else if ((key_val >= AKEYCODE_0 ) && ( key_val <= AKEYCODE_9)) {
					#ifdef EHS_GUI_SUPPORT
						EhsGUIKbHit(key_val-AKEYCODE_0+'0');
					#endif
				}
				
				break;
			}
#endif
		}
		else {
			// todo any keydown handling
		}

	case AINPUT_EVENT_TYPE_MOTION:
	#ifdef INX_FIXED_THIS_IT_SEEMS_TO_CRASH_PINE64_H6
		source = AInputEvent_getSource(event);

		switch(source){
		case 0x5002://Sony xperia returns this for some reason -  no break intended
		case AINPUT_SOURCE_MOUSE: // ROCK64 - mouse input
		case AINPUT_SOURCE_TOUCHSCREEN:
			x = AMotionEvent_getX(event, 0); // these seem to be bugget on H6. null pointer and/or multiply overflows detected in MotionEvent::getAxisValue
			y = AMotionEvent_getY(event, 0);
			action = AKeyEvent_getAction(event) & AMOTION_EVENT_ACTION_MASK;
			switch(action){

			case AMOTION_EVENT_ACTION_DOWN:
				#ifdef EHS_GUI_SUPPORT
				EhsT_android_event_button_press(x,y);
				#endif
				last_x = x;
				last_y = y;
				break;
			case AMOTION_EVENT_ACTION_UP:
				#ifdef EHS_GUI_SUPPORT
				EhsT_android_event_button_release(x,y);
				#endif
				break; //deliberate fall through
			case AMOTION_EVENT_ACTION_MOVE: /* todo - the following is generl touch screen and should be put in a common module */
				#ifdef EHS_GUI_SUPPORT
				if (eventType == AMOTION_EVENT_ACTION_MOVE) EhsT_android_event_motion_notify(x,y);
				#endif
				if (last_x > 0 && last_y > 0 && (abs(last_x-x) > EHS_MOVE_IGNORE_VAL || abs(last_y-y) > EHS_MOVE_IGNORE_VAL )) {
					#ifdef EHS_GUI_SUPPORT
					if (y < last_y) EhsGUIKbHit(0xff52); /* up - todo make enumeration for these - currently based on GTK */
					if (y > last_y) EhsGUIKbHit(0xff54); /* down */
					if (x < last_x) EhsGUIKbHit(0xff51); /* left */
					if (x > last_x) EhsGUIKbHit(0xff53); /* right */
					#endif
					last_x = x;
					last_y = y;
				}
				break;
			default:
				break;
		}/* end of action switch */
		default:
			break;
		}
	#endif
	default:
		break;
	}/* end of event type switch */
	return 0;
}

void    ShowSplashScreen(struct android_app* state) {
	JavaVM* vm  = state->activity->vm;

    JNIEnv* env = state->activity->env;

    (*vm)->AttachCurrentThread(vm,&env, NULL);

	jclass activityClass = (*env)->FindClass(env,"android/app/NativeActivity");
   jmethodID getClassLoader =  (*env)->GetMethodID(env,activityClass,"getClassLoader", "()Ljava/lang/ClassLoader;");
   jobject cls =  (*env)->CallObjectMethod(env,state->activity->clazz, getClassLoader);
   jclass classLoader =  (*env)->FindClass(env,"java/lang/ClassLoader");
   jmethodID findClass =  (*env)->GetMethodID(env,classLoader, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");
   if (findClass) LOGI("Yeah findClass");
   else LOGI("No findClass");
   //jstring strClassName =  (*env)->NewStringUTF(env,"com/example/native_activity/BuildConfig");
   jstring strClassName =  (*env)->NewStringUTF(env,"com/inx/ehs/EhsServiceIF");
   jclass classIWant = (jclass) (*env)->CallObjectMethod(env,cls, findClass, strClassName);
   if (classIWant) LOGI("Yeah classIwant");
   else LOGI("No ClassIwant");

   jmethodID myMethofID =  (*env)->GetMethodID(env,classIWant, "sayHello", "()I");
   if (myMethofID) {
      	LOGI("Yeah myMethofID");
      	jint ret = (*env)->CallIntMethod(env,state->activity->clazz/*object*/ /*cls*/, myMethofID);
      	//LOGI("RETURNED = %d",ret);
      }
         else LOGI("No myMethofID");
   (*vm)->DetachCurrentThread(vm);


}

/*
 * Required by Android - this is the target_loop_iteration() function for Android
 * Need to poll the Android event handler and process events
 * This function is supplied to the Main Loop to be called every loop iteration
 *
 * This loop must call the framework function to process the events.
 * It also tests for app lifecycle events such as puase and stop and returns values to the main EHS loop.
 * Other events processed by this function may result in callbacks to engine-handle command (e.g. to handle sensor events).
 */
Ehs_ConsoleCommand_Type process_android_events(struct android_app* state) {
	int ident;
	int events;

	static int showsplash = 1;
	struct android_poll_source* source=NULL;

    // loop until all events are read, then continue
	while ((ident=ALooper_pollAll(0, NULL, &events,(void**)&source)) >= 0) {
		// Process this event.
		if (source != NULL) {
			source->process(state, source); /* this can lead to call backs to things like engine_handle_cmd*/
		}
		switch (ident) { /* pick up exit events here - easy to handle by return value of this function rather than callbacks */

		 /* Returns ALOOPER_POLL_WAKE if the poll was awoken using wake() before
		 * the timeout expired and no callbacks were invoked and no other file
		 * descriptors were ready.
		 *
		 * Returns ALOOPER_POLL_CALLBACK if one or more callbacks were invoked.
		 *
		 * Returns ALOOPER_POLL_TIMEOUT if there was no data before the given
		 * timeout expired.
		 *
		 * Returns ALOOPER_POLL_ERROR if an error occurred.
		 */
		/*
		LOOPER_ID_MAIN
		Looper data ID of commands coming from the app's main thread, which is returned as an identifier from ALooper_pollOnce(). The data for this identifier is a pointer to an android_poll_source structure. These can be retrieved and processed with android_app_read_cmd() and android_app_exec_cmd().
		LOOPER_ID_INPUT
		Looper data ID of events coming from the AInputQueue of the application's window, which is returned as an identifier from ALooper_pollOnce(). The data for this identifier is a pointer to an android_poll_source structure. These can be read via the inputQueue object of android_app.
		LOOPER_ID_USER
		Start of user-defined ALooper identifiers.
		 */
		}

		EHSH_LOG_INFO("state->redrawNeeded  = %d",state->redrawNeeded);
		EHSH_LOG_INFO("state->activityState = %d",state->activityState);
		EHSH_LOG_INFO("state->destroyed     = %d",state->destroyed);

		if (state->destroyRequested != 0) {
			EHSH_LOG_ERROR("Destroy Requested!!!!!!!!!!!");
			return EHS_EXIT_EHS;
		}
		else return EHS_CONTINUE;

	}


	return EHS_CONTINUE;
}

ehs_bool EhsT_platform_ready = EHS_FALSE;
ehs_bool EhsT_isLostFocus = EHS_FALSE;

static void engine_handle_cmd(struct android_app* app, int32_t cmd) {
    struct engine* engine = (struct engine*)app->userData;
    switch (cmd) {

    case APP_CMD_INPUT_CHANGED:
    	EHSH_LOG_INFO("Handling ANDROID COMMAND - APP_CMD_INPUT_CHANGED");
    	/**
    	 * Command from main thread: the AInputQueue has changed.  Upon processing
    	 * this command, android_app->inputQueue will be updated to the new queue
    	 * (or NULL).
    	 */


    case APP_CMD_INIT_WINDOW:
    	EHSH_LOG_INFO("Handling ANDROID COMMAND - APP_CMD_INIT_WINDOW");
    	/**
    	 * Command from main thread: a new ANativeWindow is ready for use.  Upon
    	 * receiving this command, android_app->window will contain the new window
    	 * surface.
    	 */

    	  if (engine->app->window != NULL) {
    	                EHS_ANDROID_width = ANativeWindow_getWidth(engine->app->window);
    	                EHS_ANDROID_height = ANativeWindow_getHeight(engine->app->window);
    	                EHSH_LOG_INFO("Setting window size to %d x %d",EHS_ANDROID_width,EHS_ANDROID_height);
    	            }

    	if (engine->app->window != NULL && EhsT_platform_ready != EHS_TRUE) {
#ifdef EHS_GUI_SUPPORT
			EhsTGfxSys_initLate((void*)app->userData); /* Need to sort this */
#endif
			
#ifdef EHS_WEBKIT_SUPPORT
			InitialiseWebkitJNICalls(app);
#endif
    		EhsT_platform_ready=EHS_TRUE;
            EhsT_isLostFocus=EHS_FALSE;
    	}

    	/* if (showsplash) {
    			ShowSplashScreen(app);
    			//showsplash = 0;

    	//	} */
    	break;

    case APP_CMD_TERM_WINDOW:
    	EHSH_LOG_INFO("Handling ANDROID COMMAND - APP_CMD_TERM_WINDOW");
   /*EHS_RELOAD_EHS_FROM_FILE_DONTSTART,
	//EHS_RELOAD_EHS_FROM_EXISTINGFILE,
	EHS_EXIT_EHS,
	EHS_CONTINUE,
	EHS_RUN
	*/
#ifndef EHS_GUI_SUPPORT
    	EhsHFSMForceInternallyRequestedCommand(EHS_EXIT_EHS);
#endif
    	/**
    	 * Command from main thread: the existing ANativeWindow needs to be
    	 * terminated.  Upon receiving this command, android_app->window still
    	 * contains the existing window; after calling android_app_exec_cmd
    	 * it will be set to NULL.
    	 */
    	break;
    case   APP_CMD_WINDOW_RESIZED:
    	EHSH_LOG_INFO("Handling ANDROID COMMAND - APP_CMD_WINDOW_RESIZED");
    	/**
    	 * Command from main thread: the current ANativeWindow has been resized.
    	 * Please redraw with its new size.
    	 */
    	break;
    case   APP_CMD_WINDOW_REDRAW_NEEDED:
    	EHSH_LOG_INFO("Handling ANDROID COMMAND - APP_CMD_WINDOW_REDRAW_NEEDED");
    	/**
    	 * Command from main thread: the system needs that the current ANativeWindow
    	 * be redrawn.  You should redraw the window before handing this to
    	 * android_app_exec_cmd() in order to avoid transient drawing glitches.
    	 */
    	break;
    case   APP_CMD_CONTENT_RECT_CHANGED:
    	EHSH_LOG_INFO("Handling ANDROID COMMAND - APP_CMD_CONTENT_RECT_CHANGED");
    	/**
    	 * Command from main thread: the content area of the window has changed,
    	 * such as from the soft input window being shown or hidden.  You can
    	 * find the new content rect in android_app::contentRect.
    	 */
    	break;
    case    APP_CMD_GAINED_FOCUS:
    	EHSH_LOG_INFO("Handling ANDROID COMMAND - APP_CMD_GAINED_FOCUS");
#ifdef EHS_GUI_SUPPORT
        if (EhsT_isLostFocus && engine->app->window != NULL) {
            EhsTGfxSys_initLate((void*)app->userData);
        }
        EhsT_isLostFocus = EHS_FALSE;
#endif
    	/* todo we should repaint here  */
    	/**
    	 * Command from main thread: the app's activity window has gained
    	 * input focus.
    	 */
    	break;
    case    APP_CMD_LOST_FOCUS:
    	EHSH_LOG_INFO("Handling ANDROID COMMAND - APP_CMD_LOST_FOCUS");
        EhsT_isLostFocus = EHS_TRUE;
    	/**
    	 * Command from main thread: the app's activity window has lost
    	 * input focus.
    	 */
    	break;
    case   APP_CMD_CONFIG_CHANGED:
    	EHSH_LOG_INFO("Handling ANDROID COMMAND - APP_CMD_CONFIG_CHANGED");
    	/**
    	 * Command from main thread: the current device configuration has changed.
    	 */
    	break;
    case    APP_CMD_LOW_MEMORY:
    	EHSH_LOG_INFO("Handling ANDROID COMMAND -  APP_CMD_LOW_MEMORY");
    	/**
    	 * Command from main thread: the system is running low on memory.
    	 * Try to reduce your memory use.
    	 */
    	break;
    case    APP_CMD_START:
    	EHSH_LOG_INFO("Handling ANDROID COMMAND - APP_CMD_START");
    	/**
    	 * Command from main thread: the app's activity has been started.
    	 */
		// This is set up in java (at least for now) 
    	//EhsTInitFileSystem(app);
    	break;
    case    APP_CMD_RESUME:
    	EHSH_LOG_INFO("Handling ANDROID COMMAND - APP_CMD_RESUME"); /* this is called before init window is called */
    	//EhsHFSMSetInternallyRequestedCommand(EHS_RELOAD_EHS_FROM_FILE);
    	/* todo we should reload state here or just repaint the background threads stuff*/
    	/**
    	 * Command from main thread: the app's activity has been resumed.
    	 */
    	break;
    case    APP_CMD_SAVE_STATE:
    	EHSH_LOG_INFO("Handling ANDROID COMMAND - APP_CMD_SAVE_STATE");
    	/* todo we could save state here */
    	/**
    	 * Command from main thread: the app should generate a new saved state
    	 * for itself, to restore from later if needed.  If you have saved state,
    	 * allocate it with malloc and place it in android_app.savedState with
    	 * the size in android_app.savedStateSize.  The will be freed for you
    	 * later.
    	 */
    	break;
    case     APP_CMD_PAUSE:
    	EHSH_LOG_INFO("Handling ANDROID COMMAND - APP_CMD_PAUSE");
    	/* todo we could save state here - if not in CMD_SAVE_STATE*/
    	/**
    	 * Command from main thread: the app's activity has been paused.
    	 */
    	break;
    case    APP_CMD_STOP:
    	EHSH_LOG_INFO("Handling ANDROID COMMAND - APP_CMD_STOP");
    	/**
    	 * Command from main thread: the app's activity has been stopped.
    	 */
    	break;
    case    APP_CMD_DESTROY:
    	EHSH_LOG_INFO("Handling ANDROID COMMAND - APP_CMD_DESTROY");
    	/**
    	 * Command from main thread: the app's activity is being destroyed,
    	 * and waiting for the app thread to clean up and exit before proceeding.
    	 */
    	EhsHFSMForceInternallyRequestedCommand(EHS_EXIT_EHS);

    	break;
    default:
    	break;

    }
}
//static void * ehs_android_main(void (*target_loop_iteration)(void*), struct android_app* state );
void * ehs_android_main(Ehs_ConsoleCommand_Type (*target_loop_iteration)(void*),void * target_env_blob);

/* temp location */
void displayKeyboard(struct android_app* mApplication, JNIEnv* lJNIEnv) {
    jint lFlags = 0x00000002;//InputMethodManager.SHOW_FORCED;
//LOGI("Helloooooooooooooooooooo");
    // Retrieves NativeActivity.
    jobject lNativeActivity = mApplication->activity->clazz;
    jclass ClassNativeActivity = (*lJNIEnv)->GetObjectClass(lJNIEnv,lNativeActivity);

    // Retrieves Context.INPUT_METHOD_SERVICE.
    jclass ClassContext = (*lJNIEnv)->FindClass(lJNIEnv,"android/content/Context");
    jfieldID FieldINPUT_METHOD_SERVICE = (*lJNIEnv)->GetStaticFieldID(lJNIEnv,ClassContext, "INPUT_METHOD_SERVICE", "Ljava/lang/String;");
    jobject INPUT_METHOD_SERVICE = (*lJNIEnv)->GetStaticObjectField(lJNIEnv,ClassContext, FieldINPUT_METHOD_SERVICE);
    if (INPUT_METHOD_SERVICE) LOGI("Yeah INOPUT");
    else LOGI("meh INOPUT");

    // Runs getSystemService(Context.INPUT_METHOD_SERVICE).
    jclass ClassInputMethodManager = (*lJNIEnv)->FindClass(lJNIEnv,"android/view/inputmethod/InputMethodManager");
    jmethodID MethodGetSystemService = (*lJNIEnv)->GetMethodID(lJNIEnv,ClassNativeActivity, "getSystemService","(Ljava/lang/String;)Ljava/lang/Object;");
    jobject lInputMethodManager = (*lJNIEnv)->CallObjectMethod(lJNIEnv,lNativeActivity, MethodGetSystemService,INPUT_METHOD_SERVICE);



    /*
     *    InputMethodManager imm = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
                imm.showSoftInput(mEditTextStudy, InputMethodManager.SHOW_FORCED);
     */

if (lInputMethodManager) LOGI("yeah lInputMethodManager");
else LOGI( "MEh lInputMethodManager");

    // Runs getWindow().getDecorView().
    jmethodID MethodGetWindow = (*lJNIEnv)->GetMethodID(lJNIEnv,ClassNativeActivity, "getWindow","()Landroid/view/Window;");
    jobject lWindow = (*lJNIEnv)->CallObjectMethod(lJNIEnv,lNativeActivity,MethodGetWindow);
    jclass ClassWindow = (*lJNIEnv)->FindClass(lJNIEnv,"android/view/Window");
    jmethodID MethodGetDecorView = (*lJNIEnv)->GetMethodID(lJNIEnv,ClassWindow, "getDecorView", "()Landroid/view/View;");
    jobject lDecorView = (*lJNIEnv)->CallObjectMethod(lJNIEnv,lWindow,MethodGetDecorView);
    if (lDecorView) LOGI("yeah lDecorView");
    else LOGI( "MEh lDecorView");
    if (1) {
        // Runs lInputMethodManager.showSoftInput(...).
        jmethodID MethodShowSoftInput = (*lJNIEnv)->GetMethodID(lJNIEnv,ClassInputMethodManager, "showSoftInput","(Landroid/view/View;I)Z");
        if (MethodShowSoftInput) LOGI("yeah MethodShowSoftInput");
        else LOGI("NAhh MethodShowSoftInput");
        jboolean lResult = (*lJNIEnv)->CallBooleanMethod(lJNIEnv,lInputMethodManager, MethodShowSoftInput,lDecorView, lFlags);



        LOGI("RESULT = %d",(int)lResult);
    } else {
        // Runs lWindow.getViewToken()
        jclass ClassView = (*lJNIEnv)->FindClass(lJNIEnv,"android/view/View");
        jmethodID MethodGetWindowToken = (*lJNIEnv)->GetMethodID(lJNIEnv,ClassView, "getWindowToken", "()Landroid/os/IBinder;");
        jobject lBinder = (*lJNIEnv)->CallObjectMethod(lJNIEnv,lDecorView,MethodGetWindowToken);

        // lInputMethodManager.hideSoftInput(...).
        jmethodID MethodHideSoftInput = (*lJNIEnv)->GetMethodID(lJNIEnv,ClassInputMethodManager, "hideSoftInputFromWindow","(Landroid/os/IBinder;I)Z");
        jboolean lRes = (*lJNIEnv)->CallBooleanMethod(lJNIEnv,lInputMethodManager, MethodHideSoftInput,lBinder, lFlags);
    }
}




/* this is the entry point for the android environment */
void android_main(struct android_app* state) {
    struct engine engine;

    // Make sure glue isn't stripped.
    // app_dummy(); // no longer need

    memset(&engine, 0, sizeof(engine));
    state->userData = &engine;
    state->onAppCmd = engine_handle_cmd;
    state->onInputEvent = engine_handle_input;
    engine.app = state;
    /* now call our ehs bit */

   // while(1) {sleep(1);}

    JavaVM* vm  = state->activity->vm;
    JNIEnv* env = state->activity->env;
    const char* temp;

#ifdef ATTACH_THREAD_FORALLTIME
  int attached = 0;
    switch ((*vm)->GetEnv(vm,(void**)&env, JNI_VERSION_1_6))
      {
      case JNI_OK:
    	  LOGI("Invalid java version");
    	  attached = 1;
        break;
      case JNI_EDETACHED:
        if ( (*vm)->AttachCurrentThread(vm,&env, NULL)!=0)
        {
         LOGI("Could not attach current thread");
        }
        else attached = 1;
        break;
      case JNI_EVERSION:
        LOGI("Invalid java version");
        attached = 0;
      }

if (attached)

/*	jclass activityClass = (*env)->FindClass(env,"com.inx.ehs.EhsServiceIF");
	if (activityClass) {
			LOGI("Yeah -found activityClass");
			//jint x = (*env)->CallIntMethod(env,state->activity->clazz, getClassLoader2);
		}
		else LOGI("Naah -not found activityClass");

	jmethodID getClassLoader2 = (*env)->GetMethodID(env,activityClass,"sayHello", "()I");
	if (getClassLoader2) {
		LOGI("Yeah -found sayHello");
		jint x = (*env)->CallIntMethod(env,state->activity->clazz, getClassLoader2);
	}
	else LOGI("Naah -not found sayHello");

	 //state->activity->clazz->processCommand();

	 //while(1) {sleep(1);}
	  *
	  */

#ifdef DOSOMETHINGLATER

    //jclass activityClass = (*env)->FindClass(env,"android/app/NativeActivity");

    //jmethodID getClassLoader = (*env)->GetMethodID(env,activityClass,"getClassLoader", "()Ljava/lang/ClassLoader;");
    //jobject cls = (*env)->CallObjectMethod(env,state->activity->clazz, getClassLoader);
    //JniMethodInfo t;


    //jclass classLoader = (*env)->FindClass(env,"java/lang/ClassLoader");
    //jmethodID findClass = (*env)->GetMethodID(env,classLoader, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");
    //jmethodID contextMethod = (*env)->GetMethodID(env,activityClass, "getApplicationContext", "()Landroid/content/Context;");
    //jobject contextObj = (*env)->CallObjectMethod(env,state->activity->clazz, contextMethod);

    /*
    jclass class = (*env)->FindClass(env,"EhsServiceIF");
    if (class) {
    	jmethodID method = (*env)->GetMethodID(env,class, "sayHello", "()V");
    	if (method) {
    		LOGI(" Yesy!!!!");
    	}
    	else LOGI(" No find method :(");
    }
    else LOGI(" No find class :(");
*/

    //jobject oActivity = state->activity->clazz; //GetObjectClass

   jclass cls = (*jenv)->FindClass(jenv, "org/pjsip/pjsua/pjsua_appJNI");



    jclass cActivity = (*env)->GetObjectClass(env,oActivity);

    jmethodID contextMethod = (*env)->GetMethodID(env,cActivity /*activityClass*/, "loadURL", "(Ljava/lang/String;)V");

    //jmethodID contextMethod = (*env)->GetMethodID(env,cActivity /*activityClass*/, "getPackageName", "()Ljava/lang/String;");//"(Ljava/lang/String;)V");

    if (contextMethod) {
    	//(*env)->CallVoidMethod(env,state->activity->clazz, contextMethod,(*env)->NewStringUTF(env,"www.google.com"));//(appClassObj, mid, mEnv->NewStringUTF(url));

    	LOGI(" OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO...." );
    	(*env)->CallStaticVoidMethod(env,state->activity->clazz, contextMethod,(*env)->NewStringUTF(env,"www.google.com"));//(appClassObj, mid, mEnv->NewStringUTF(url));
    	LOGI("Yeah!!!!!!!!!!!!!");
    }

    else {
    	LOGI("COULDN'T get contextMethod Avtivity = %x , clazz = %x",cActivity,state->activity->clazz);
    }
#else





   // move code later??
#ifdef createobject
    jmethodID constructor =  (*env)->GetMethodID(env,classIWant, "<init>", "()V");
    if (!constructor) 	LOGI("No constructor");
    else 	LOGI("Yeah constructor");
    //jmethodID constructor = (*env)->GetMethodID(env, cls, "<init>", "(I)V");
    jobject object = (*env)->NewObject(env, classIWant, constructor);
    if (!object) LOGI("No object");
    else 	LOGI("yeah Object");
    jmethodID myMethodID =  (*env)->GetMethodID(env,classIWant, "sayHello", "()I");

    // jmethodID myMethofID =  (*env)->GetStaticMethodID(env,classIWant, "sayHello", "()I");


    if (myMethofID) {
    	LOGI("Yeah myMethofID");
    	jint ret = (*env)->CallIntMethod(env,object /*cls*/, myMethodID);
    	LOGI("RETURNED = %d",ret);
    }
       else LOGI("No myMethofID");
#endif
    //jthrowable exception = (*env)->ExceptionOccurred(env);
     //if (exception) {
      // (*env)->ExceptionDescribe(env);
       // This gives me: "java.lang.NoClassDefFoundError: [generic]".
       // Also, theSubClass is null, so the next line causes a segfault.
     //}
   // while (1) {sleep(1);}


#endif

   // displayKeyboard( state,env);

   //(*vm)->DetachCurrentThread(vm,&env, NULL); todo this should be done when pausing?
    /*  	}
    	else {
    		LOGI("Couldn't find findClass ");
    	}
    }
    else {
    	LOGI("Couldn't find class loader ");
    }*/


    //  jclass appClass = mEnv->GetObjectClass(appClassObj);
    //       jmethodID mid = mEnv->GetMethodID(appClass, "openURL", "(Ljava/lang/String;)V");
    //       mEnv->CallVoidMethod(appClassObj, mid, mEnv->NewStringUTF(url));

    /* #include <sys/system_properties.h>
#define ANDROID_OS_BUILD_MODEL               "ro.product.model"

char model_id[PROP_VALUE_MAX]; // PROP_VALUE_MAX from <sys/system_properties.h>.
int len;
len = __system_property_get(ANDROID_OS_BUILD_MODEL, model_id);

https://android.googlesource.com/platform/frameworks/base/+/refs/heads/master/core/java/android/os/Build.java

     */
    	//(*vm)->DetachCurrentThread(vm);
} /* is attached */
#endif
	EHSH_LOG_INFO("Launching android main process.");
#if defined(EHS_MEDIA_SUPPORT) && !defined(EHS_UNITY3D_WIDGETS)
    // Audio/Video initialization
    EhsTDPlayback_initJNI(state);
#endif
    ehs_android_main(&process_android_events,state);
}

/** 
 * Main entry point to the application.
 * @return Integer representing exit code of application.
 *
 */

/* Android Specific start up for generic ehs_main
 * e.g. gets argv command type parameters
 * */
void * ehs_android_main(Ehs_ConsoleCommand_Type (*target_loop_iteration)(void*),void * target_env_blob)
{
	pid_t pID;
	//EHSH_LOG_INFO("-------EHS:Android_main v54-------");
	ehs_char *argv[]={"/sdcard/bin/ehs.exe",""}; /* @todo this make the user app data in the root of the sdcard -might want something else here... */
	ehs_char buf[EHS_MAXPATHLENGTH];
	getcwd(buf,EHS_MAXPATHLENGTH); // Note this must be implemented for mingw
	EHSH_LOG_ERROR("Storing info");
	EhsHStoreArgInfo(1,argv,buf);  //@todo argv this is a bit clumsy - should extract the userdata path etc..
	//EHSH_LOG_INFO("EHS startup\n");

	/* set the exit functions */
	//signal(SIGTERM,EhsTargetHandleTerm);
	//signal(SIGINT, EhsTargetHandleTerm);
	EhsMain( target_loop_iteration,target_env_blob); /* Blocking - doesn't return in this version */
	//EHSH_LOG_INFO("EHS Exiting Android Thread...");
	EhsExit(0);
	return 0;
}



/**
 * Handle the SIGTERM signal
 */
void EhsTargetHandleTerm(int sig)
{
	/* Tod we should call some HAL tear down functions here libxml , libcurl etc.*/
	EhsExit(0);
}

/* poll/call this before starting EHS to check if the host app environment is ready to start */
ehs_bool EhsTPlatformReady(void (*target_loop_iteration)(void*),void * target_env_blob) {
#ifdef EHS_UNITY3D_WIDGETS
	return EHS_TRUE;
#else
	if (target_loop_iteration) {
		target_loop_iteration(target_env_blob); /* Do the target specific handling if there is such a thing */
	}
	//EHSH_LOG_INFO("Testing platform... ready = %d",EhsT_platform_ready);
	return EhsT_platform_ready;
#endif
}
