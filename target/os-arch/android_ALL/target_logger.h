/** @file target_logger.h
 * In this file, android target specific logger is defined.
 *
 * @author: inx limited
 *
 */

#ifndef EHS_TARGET_LOGGER_H
#define EHS_TARGET_LOGGER_H

#include <android/log.h>

#define  LOG_TAG    "NativeEHS"
#define  LOGI(...)  ((void)__android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__))
#define  LOGW(...)  ((void)__android_log_print(ANDROID_LOG_WARN,LOG_TAG__VA_ARGS__))
#define  LOGE(...)  ((void)__android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__))
#ifndef NDEBUG
#define  LOGV(...)  ((void)__android_log_print(ANDROID_LOG_VERBOSE,LOG_TAG,__VA_ARGS__))
#else
#define  LOGV(...)  ((void)0)
#endif


#endif
