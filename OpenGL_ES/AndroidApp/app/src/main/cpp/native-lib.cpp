#include <GLES3/gl31.h>

#include <jni.h>
#include <string>

#define LOG_MSG_FLAG
//#undef LOG_MSG_FLAG
#define LOG_TAG "Native-Lib"
#include "log.h"

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_h1gdev_androidapp_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
