#include <GLES3/gl31.h>
//#include <GLES3/gl3ext.h>   // In OpenGL ES 3 Extension, nothing is defined.

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

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

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_h1gdev_androidapp_MainActivity_stringFromAAsset(
        JNIEnv *env,
        jobject /* this */,
        jobject assetManager) {
    AAssetManager* aAssetManager = AAssetManager_fromJava(env, assetManager);
    // filename is file path from assets directory.
    const char* filename = "asset.txt";
    AAsset* aAsset = AAssetManager_open(aAssetManager, filename, AASSET_MODE_RANDOM);
    size_t length = static_cast<size_t>(AAsset_getLength(aAsset));
    char buf[length + 1];
    buf[length] = '\0';
    AAsset_read(aAsset, buf, length);
    std::string hello = buf;
    AAsset_close(aAsset);
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_h1gdev_androidapp_MainActivity_outputGLInfo(
        JNIEnv *env,
        jobject /* this */) {
    LOGI("GL_VENDOR is %s", glGetString(GL_VENDOR));
    LOGI("GL_RENDERER is %s", glGetString(GL_RENDERER));
    LOGI("GL_VERSION is %s", glGetString(GL_VERSION));
    LOGI("GL_SHADING_LANGUAGE_VERSION is %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

    LOGI("GL_EXTENSIONS is");
    GLint extensions = 0;
    glGetIntegerv(GL_NUM_EXTENSIONS, &extensions);
    for (GLint i = 0; i < extensions; ++i) {
        LOGI("[%3d]%s", i, glGetStringi(GL_EXTENSIONS, i));
    }
}
