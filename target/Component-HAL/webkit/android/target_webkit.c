
#include <jni.h>
#include <android_native_app_glue.h>

#include "target_webkit.h"
#include "hal-api.h"

static JavaVM* vm  = NULL;
static JNIEnv* env = NULL;
static jobject appClazz ;

static jclass classWebkitView = NULL;
static jmethodID methodIDsetViewParameters,methodIDCreateWebView ,methodIDRunFunction ,methodIDDestroy,methodIDsetFRameVisible;


ehs_bool InitialiseWebkitJNICalls(struct android_app* state) {
	 vm  = state->activity->vm;
	    	env = state->activity->env;
    (*vm)->AttachCurrentThread(vm,&env, NULL);

    	appClazz = (*env)->NewGlobalRef(env, state->activity->clazz);
   jclass activityClass = (*env)->FindClass(env,"android/app/NativeActivity");
   jmethodID getClassLoader =  (*env)->GetMethodID(env,activityClass,"getClassLoader", "()Ljava/lang/ClassLoader;");
   jobject cls =  (*env)->CallObjectMethod(env,appClazz, getClassLoader);
   jclass classLoader =  (*env)->FindClass(env,"java/lang/ClassLoader");
   jmethodID findClass =  (*env)->GetMethodID(env,classLoader, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");

   jstring strClassName =  (*env)->NewStringUTF(env,"com/inx/ehs/EhsServiceIF");
   classWebkitView = (jclass) (*env)->NewGlobalRef(env, (jclass) (*env)->CallObjectMethod(env,cls, findClass, strClassName));
    methodIDCreateWebView =  (*env)->GetMethodID(env,classWebkitView, "CreateWebView", "(Ljava/lang/String;Ljava/lang/String;ZLjava/lang/String;)I");
    if (methodIDCreateWebView) {
 	   LOGI("CreateWebView() - OK ");
    }
    else LOGI("CreateWebView() - NOK ");
   /* Create calls onto the following Java methods */
   // setViewParameters(final int x, final int y,final int width, final int height, final int alpha,final int  timeoutms );
   methodIDsetViewParameters =  (*env)->GetMethodID(env,classWebkitView, "setViewParameters", "(IIIIII)V");
   if (methodIDsetViewParameters) {
	   LOGI("setViewParameters() - OK ");

   }
   else LOGI("setViewParameters() - NOK ");


   //void RunFunction(String funcName, String jsonarg);
   methodIDRunFunction =  (*env)->GetMethodID(env,classWebkitView, "RunFunction", "(Ljava/lang/String;Ljava/lang/String;)I");
   if (methodIDCreateWebView) {
	   LOGI("RunFunction() - OK ");
	   //jint ret = (*env)->CallIntMethod(env,state->activity->clazz/*object*/ /*cls*/, myMethofID);
   }
   else LOGI("RunFunction() - NOK ");
   //void Destroy();
   methodIDDestroy =  (*env)->GetMethodID(env,classWebkitView, "Destroy", "()I");
   if (methodIDCreateWebView) {
	   LOGI("Destroy() - OK ");
	   //jint ret = (*env)->CallIntMethod(env,state->activity->clazz/*object*/ /*cls*/, myMethofID);
   }
   else LOGI("Destroy() - NOK ");

methodIDsetFRameVisible =  (*env)->GetMethodID(env,classWebkitView, "setFRameVisible", "(Z)I");
  if (methodIDsetFRameVisible) {
	   LOGI("setFRameVisible() - OK ");
	   //jint ret = (*env)->CallIntMethod(env,state->activity->clazz/*object*/ /*cls*/, myMethofID);
  }
  else LOGI("setFRameVisible() - NOK ");

 /* todo show hide */

      //jint ret = (*env)->CallIntMethod(env,state->activity->clazz/*object*/ /*cls*/, myMethofID);

   (*vm)->DetachCurrentThread(vm);
	return EHS_TRUE;
}

//java_lang_string_class = (jclass)env->NewGlobalRef(local);
//_CHECK_JAVA_EXCEPTION(env);
//env->DeleteLocalRef(local);

/* calls into Java land to open a webkit window and load data/URL */
/* todo - may want BG colour here too one day */
ehs_bool EhsTWebkitLoad(ehs_char * baseURL, ehs_char * URLorData,ehs_bool Data,int x, int y, int width, int height, int alpha, int timeout){

	(*vm)->AttachCurrentThread(vm,&env, NULL);

	jint 	 ret = 0;
	/* set the parameters in the Java-side object */
	(*env)->CallVoidMethod(env,appClazz, methodIDsetViewParameters,x,y,width,height,alpha,timeout);

	jstring _baseURL =  (*env)->NewStringUTF(env,baseURL);
	jstring _URLorData =  (*env)->NewStringUTF(env,URLorData);
	jboolean _Data=Data;
	jstring _Nowt = (*env)->NewStringUTF(env,"");
	/* Create the view using the Java-side object */
	ret += (*env)->CallIntMethod(env,appClazz, methodIDCreateWebView,_baseURL,_URLorData,_Data,_Nowt);

	(*vm)->DetachCurrentThread(vm);
	return EHS_TRUE;

}

ehs_bool  EhsTWebkitRunFunction(ehs_char * baseURL, ehs_char * URLorData,ehs_char * JSON){

	(*vm)->AttachCurrentThread(vm,&env, NULL);
		 jint ret = (*env)->CallIntMethod(env,appClazz, methodIDRunFunction,baseURL,URLorData,JSON);
		 (*vm)->DetachCurrentThread(vm);
		 return EHS_TRUE;
}
ehs_bool EhsTWebkitShow(ehs_bool show){
	return EHS_TRUE;
}
ehs_bool EhsTWebkitUnloadAll(){
	return EHS_TRUE;
}

ehs_bool EhsTWebkitDestroy(){
	(*vm)->AttachCurrentThread(vm,&env, NULL);
	jint ret = (*env)->CallIntMethod(env,appClazz, methodIDDestroy);
	(*vm)->DetachCurrentThread(vm);
	return EHS_TRUE;
}


