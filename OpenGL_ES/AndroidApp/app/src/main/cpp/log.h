#ifndef LOG_H_
#define LOG_H_

#include <chrono>
#include <android/log.h>

// [ログ出力]

#ifndef LOG_TAG
#define LOG_TAG nullptr
#endif // LOG_TAG

#ifdef LOG_MSG_FLAG
// logライブラリが必要
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#define BEGIN_N(n) std::chrono::high_resolution_clock::time_point b_##n = std::chrono::high_resolution_clock::now()
#define END_N(n, m) std::chrono::high_resolution_clock::time_point e_##n = std::chrono::high_resolution_clock::now(); std::chrono::microseconds d_##n = std::chrono::duration_cast<std::chrono::microseconds>(e_##n - b_##n); LOGI("%s: %lld[us]", m, d_##n.count());
#else // LOG_MSG_FLAG
#define LOGV(...)
#define LOGD(...)
#define LOGI(...)
#define LOGW(...)
#define LOGE(...)

#define BEGIN_N(n)
#define END_N(n, m)
#endif // LOG_MSG_FLAG

#define BEGIN() BEGIN_N(0)
#define END(m) END_N(0, m)

#endif // LOG_H_
