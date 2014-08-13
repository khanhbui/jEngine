#ifndef __jEngine__Log__
#define __jEngine__Log__

#define LOG_TAG "jEngine"

#define LOGGING_ON 5
#define LOG_LEVEL_DEBUG 5
#define LOG_LEVEL_INFO 4
#define LOG_LEVEL_WARN 3
#define LOG_LEVEL_ERROR 1

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_DEBUG
#endif

#ifdef IOS
#define  LOGD(...) if (LOGGING_ON && LOG_LEVEL >= LOG_LEVEL_DEBUG)  printf("[DEBUG]" __VA_ARGS__); printf("\n")
#define  LOGE(...) if (LOGGING_ON && LOG_LEVEL >= LOG_LEVEL_ERROR)  printf("[ERROR]" __VA_ARGS__)
#define  LOGI(...) if (LOGGING_ON && LOG_LEVEL >= LOG_LEVEL_INFO)  printf("[INFO]" __VA_ARGS__)
#define  LOGW(...) if (LOGGING_ON && LOG_LEVEL >= LOG_LEVEL_WARN)  printf("[WARN]" __VA_ARGS__)
#else
#ifdef ANDROID
#include <android/log.h>

#define  LOGD(...) if (LOGGING_ON && LOG_LEVEL >= LOG_LEVEL_DEBUG)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  LOGE(...) if (LOGGING_ON && LOG_LEVEL >= LOG_LEVEL_ERROR)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define  LOGI(...) if (LOGGING_ON && LOG_LEVEL >= LOG_LEVEL_INFO)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define  LOGW(...) if (LOGGING_ON && LOG_LEVEL >= LOG_LEVEL_WARN)  __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)

#endif //ANDROID

#endif //IOS else

#endif /* defined(__jEngine__Log__) */