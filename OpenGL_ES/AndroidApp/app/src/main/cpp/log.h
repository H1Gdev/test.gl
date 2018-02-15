#ifndef LOG_H_
#define LOG_H_

#include <stdio.h>	// NULL
#include <android/log.h>

// [ログ出力]

#ifndef LOG_TAG
#define LOG_TAG NULL
#endif // LOG_TAG

#ifdef LOG_MSG_FLAG
// logライブラリが必要
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#else // LOG_MSG_FLAG
#define LOGV(...)
#define LOGD(...)
#define LOGI(...)
#define LOGW(...)
#define LOGE(...)
#endif // LOG_MSG_FLAG

#endif // LOG_H_
