#include <jni.h>

#define LOG_MSG_FLAG
//#undef LOG_MSG_FLAG
#define LOG_TAG "OpenCL"
#include "log.h"

extern "C"
JNIEXPORT jint JNICALL
Java_com_example_h1gdev_androidapp_MainActivity_outputCLInfo(
        JNIEnv* /* env */,
        jobject /* this */) {
    LOGD("[S]%s()", __func__);
    jint ret = 0;
    LOGD("[E]%s()=%d", __func__, ret);
    return ret;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_example_h1gdev_androidapp_MainActivity_executeOpenCLSdkExamples(
        JNIEnv* /* env */,
        jobject /* this */) {
    LOGD("[S]%s()", __func__);
    jint ret = 0;
    LOGD("[E]%s()=%d", __func__, ret);
    return ret;
}
