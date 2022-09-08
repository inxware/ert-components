
#include <jni.h>
#include "hal-api.h"

/* exposed JNI functions */
/*
JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_setIpAddress(JNIEnv *env,jobject obj,jstring ipaddress);
JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_setMacAddress(JNIEnv *env,jobject obj,jstring macaddress);
*/
/*
 * EHS_GLOBAL void EhsHMetaUpdateStatic();
EHS_GLOBAL void EhsHMetaUpdateDynamic();
EHS_GLOBAL const ehs_char* EhsHMetaGetInstPath();
EHS_GLOBAL const ehs_char* EhsHMetaGetUserPath();
EHS_GLOBAL const ehs_char* EhsHMetaAppLiveDefaultDir();
EHS_GLOBAL const ehs_char* EhsHSetMetaAppLiveDefaultDir(ehs_uint8 which);
EHS_GLOBAL const ehs_char* EhsHMetaGetHWID();
EHS_GLOBAL const ehs_char* EhsHMetaGetIPAddr();
EHS_GLOBAL const ehs_char* EhsHMetaGetEHSVersion();
EHS_GLOBAL const ehs_uint32 EhsHMetaGetRAMAvail();
EHS_GLOBAL const ehs_uint32 EhsHMetaGetRAMTotal();
EHS_GLOBAL const ehs_uint32 EhsHMetaGetRAMUsedEHS();
EHS_GLOBAL const ehs_uint32 EhsHMetaGetStorAvail();
EHS_GLOBAL const ehs_uint32 EhsHMetaGetStorUsed();
EHS_GLOBAL const ehs_uint32 EhsHMetaGetStorTotal();
EHS_GLOBAL const ehs_uint16 EhsHMetaGetCPUUsage();
EHS_GLOBAL const ehs_uint32 EhsHMetaGetSysAvail();
EHS_GLOBAL const ehs_uint32 EhsHMetaGetSysTotal();
EHS_GLOBAL const ehs_uint32 EhsHMetaGetSysUsed();
EHS_GLOBAL const ehs_char* EhsHMetaGetVersion();
EHS_GLOBAL const ehs_char* EhsHMetaGetBuildDate();
EHS_GLOBAL const ehs_char* EhsHMetaGetTargetVariant();
EHS_GLOBAL const ehs_char* EhsHMetaGetEHSStartDate();
EHS_GLOBAL const ehs_char * EhsHMetaGetOSVersion();
EHS_GLOBAL const ehs_char* EhsHMetaGetMiscInfo();
 */
/*
JNIEXPORT void JNICALL Java_com_android_gl2jni_SysInfo_setIpAddress(JNIEnv *env, jobject obj, jstring value) {
	jboolean iscopy;
	EhsHMetaSetIPAddr((*env)->GetStringUTFChars(env, value, &iscopy));

}
*/
JNIEXPORT void JNICALL Java_com_inx_ehs_utils_EhsJNI_jniSetId(JNIEnv *env, jobject obj, jstring value, jstring path)
{
    jboolean iscopy;
    EhsHMetaSetHWID((*env)->GetStringUTFChars(env, value, &iscopy));
    EhsHMetaSetInstPath((*env)->GetStringUTFChars(env, path, &iscopy));
    EhsHOsSys_UpdateEnvironment();
    //EhsTInitFileSystem(env->envapp??? can't ge the app so this is done in java instead);
}
