/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

#include "target_os_dtv.h"
#include "target_dtv.h"

#include "hal-api.h"

static JavaVM* vm = NULL;
static JNIEnv* env = NULL;
static jobject appClazz;
static jclass classEhsNativeActivity = NULL;
static jmethodID methodID_JNI_AV_Command;
static jmethodID methodID_JNI_AV_RegisterCallback;
static jmethodID methodID_JNI_AV_GetIntAttribute;
static int id_count = 1000;

ehs_bool EhsTDPlayback_initJNI(struct android_app* state)
{

    EHSH_LOG_INFO("Initialisation of EhsTDPlayback JNI.");
    vm = state->activity->vm;
    env = state->activity->env;
    (*vm)->AttachCurrentThread(vm, &env, NULL);

    appClazz = (*env)->NewGlobalRef(env, state->activity->clazz);
    jclass activityClass = (*env)->FindClass(env, "android/app/NativeActivity");
    jmethodID getClassLoader = (*env)->GetMethodID(env, activityClass, "getClassLoader", "()Ljava/lang/ClassLoader;");
    jobject cls = (*env)->CallObjectMethod(env, appClazz, getClassLoader);
    jclass classLoader = (*env)->FindClass(env, "java/lang/ClassLoader");
    jmethodID findClass = (*env)->GetMethodID(env, classLoader, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");
    jstring strClassName = (*env)->NewStringUTF(env, "com/inx/ehs/EhsNativeActivity");
    classEhsNativeActivity = (jclass)(*env)->NewGlobalRef(env, (jclass)(*env)->CallObjectMethod(env, cls, findClass, strClassName));
    methodID_JNI_AV_Command = (*env)->GetMethodID(env, classEhsNativeActivity, "JNI_AV_Command", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)I");
    methodID_JNI_AV_RegisterCallback = (*env)->GetMethodID(env, classEhsNativeActivity, "JNI_AV_RegisterCallback", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)I");
    methodID_JNI_AV_GetIntAttribute = (*env)->GetMethodID(env, classEhsNativeActivity, "JNI_AV_GetIntAttribute", "(Ljava/lang/String;Ljava/lang/String;)I");

    if (methodID_JNI_AV_Command)
    {
        EHSH_LOG_INFO("Created 'JNI_AV_Command' for JNI ().");
    }
    else
    {
        EHSH_LOG_ERROR("Failed to create 'JNI_AV_Command' for JNI.");
    }
    if (methodID_JNI_AV_RegisterCallback)
    {
        EHSH_LOG_INFO("Created 'JNI_AV_RegisterCallback' for JNI ().");
    }
    else
    {
        EHSH_LOG_ERROR("Failed to create 'JNI_AV_RegisterCallback' for JNI.");
    }
    if (methodID_JNI_AV_GetIntAttribute)
    {
        EHSH_LOG_INFO("Created 'JNI_AV_GetIntAttribute' for JNI ().");
    }
    else
    {
        EHSH_LOG_ERROR("Failed to create 'JNI_AV_GetIntAttribute' for JNI.");
    }
    (*vm)->DetachCurrentThread(vm);

    //EhsTDPlayback_setCommnadJNI("media", "load", "<path> initJNI");

    return EHS_TRUE;
}

ehs_bool EhsTDPlayback_JNI_AV_Command(const ehs_char* jniID, const ehs_char* jniCmdName, const ehs_char* jniData)
{

    if (methodID_JNI_AV_Command)
    {

        (*vm)->AttachCurrentThread(vm, &env, NULL);

        jstring _jniID = (*env)->NewStringUTF(env, jniID);
        jstring _jniCmdName = (*env)->NewStringUTF(env, jniCmdName);
        jstring _jniData = (*env)->NewStringUTF(env, jniData);

        jint ret = (*env)->CallIntMethod(env, appClazz, methodID_JNI_AV_Command, _jniID, _jniCmdName, _jniData);

        (*vm)->DetachCurrentThread(vm);
        return EHS_TRUE;
    }

    EHSH_LOG_ERROR("Unable to call JNI_AV_Command (%s), as EhsTDPlayback JNI is not initialised.", jniCmdName);
    return EHS_FALSE;
}

ehs_bool EhsTDPlayback_JNI_AV_RegisterCallback(const ehs_char* jniID, const ehs_char* jniCallbackName, const ehs_char* jniDataPtr)
{

    if (methodID_JNI_AV_RegisterCallback)
    {

        (*vm)->AttachCurrentThread(vm, &env, NULL);

        jstring _jniID = (*env)->NewStringUTF(env, jniID);
        jstring _jniCallbackName = (*env)->NewStringUTF(env, jniCallbackName);
        jstring _jniDataPtr = (*env)->NewStringUTF(env, jniDataPtr);

        jint ret = (*env)->CallIntMethod(env, appClazz, methodID_JNI_AV_RegisterCallback, _jniID, _jniCallbackName, _jniDataPtr);

        (*vm)->DetachCurrentThread(vm);
        return EHS_TRUE;
    }

    EHSH_LOG_ERROR("Unable to call JNI_AV_RegisterCallback (%s), as EhsTDPlayback JNI is not initialised.", jniCallbackName);
    return EHS_FALSE;
}

int EhsTDPlayback_JNI_AV_GetIntAttribute(const ehs_char* jniID, const ehs_char* jniAttribName)
{
    if (methodID_JNI_AV_GetIntAttribute)
    {

        (*vm)->AttachCurrentThread(vm, &env, NULL);

        jstring _jniID = (*env)->NewStringUTF(env, jniID);
        jstring _jniAttribName = (*env)->NewStringUTF(env, jniAttribName);

        jint ret = (*env)->CallIntMethod(env, appClazz, methodID_JNI_AV_GetIntAttribute, _jniID, _jniAttribName);

        (*vm)->DetachCurrentThread(vm);
        return ret;
    }

    EHSH_LOG_ERROR("Unable to call JNI_AV_GetIntAttribute (%s), as EhsTDPlayback JNI is not initialised.", jniAttribName);
    return -1;
}

void* EhsTDPlayback_init(EhsFbPvrPlayClass *pPvrPlay)
{
    //Initialise the structure for our target dependent dtv
    ehs_char full_plugin_path[EHS_SYS_MAXPATHLENGTH];
    struct EhsTDPlaybackStruct* pPlayback =(struct EhsTDPlaybackStruct*) EhsHMem_tempAlloc(sizeof(struct EhsTDPlaybackStruct));
    if (!pPlayback)
    {
        return NULL;
    }

    pPlayback->xPlaybackState = EHS_FB_STATE_NOT_LOADED; //@todo move to the holding class
    pPlayback->nPlaySpeed = 0; //@todo move to the holding class
    pPlayback->mediaFormat=EHS_FALSE;
    pPlayback->nVolume=50; //@todo move to the holding class //set a defailt value. persistent values are set by the player manager
    // need a unique id for the function block
    // @TODO - find a better way than a counter
    sprintf(pPlayback->id, "%d", id_count);
    EHSH_LOG_INFO("EHS JNI ID : %s.", pPlayback->id);
    EhsTDPlayback_android_register(pPlayback->id, pPvrPlay);
    id_count++;
    return pPlayback;
}

EHS_GLOBAL void EhsTDPlayback_closeWindow(EhsFbPvrPlayClass* pPvrPlay)
{

}

EHS_GLOBAL void EhsTDPlayback_setWindow(EhsFbPvrPlayClass* pPvrPlay)
{

}

EHS_GLOBAL void EhsTDPlayback_updateZorder(EhsFbPvrPlayClass* pPvr) {}
