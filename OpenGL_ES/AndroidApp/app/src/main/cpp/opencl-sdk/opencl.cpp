#include <jni.h>

#define LOG_MSG_FLAG
//#undef LOG_MSG_FLAG
#define LOG_TAG "OpenCL"
#include "log.h"

int outputCLInfo();

extern "C"
JNIEXPORT jint JNICALL
Java_com_example_h1gdev_androidapp_MainActivity_outputCLInfo(
        JNIEnv* /* env */,
        jobject /* this */) {
    LOGD("[S]%s()", __func__);
    jint ret = (jint)outputCLInfo();
    LOGD("[E]%s()=%d", __func__, ret);
    return ret;
}

#define BASIC_HELLO_WORLD
//#define BASIC_QCOM_BLOCK_MATCH_SAD

#ifdef BASIC_HELLO_WORLD
int hello_world_main(int argc, const char** argv);
#endif
#ifdef BASIC_QCOM_BLOCK_MATCH_SAD
int qcom_block_match_sad_main(int argc, const char** argv);
#endif

extern "C"
JNIEXPORT jint JNICALL
Java_com_example_h1gdev_androidapp_MainActivity_executeOpenCLSdkExamples(
        JNIEnv* /* env */,
        jobject /* this */) {
    LOGD("[S]%s()", __func__);
    jint ret = 0;

    try {
#ifdef BASIC_HELLO_WORLD
        {
            const char* argv[] = {
                    "hello_world",
                    "/sdcard/DCIM/Camera/CL_QCOM_NV12__CL_UNORM_INT8__CONSTANT.dat",
                    "/sdcard/DCIM/Camera/hello_world.dat",
            };
            int argc = sizeof(argv) / sizeof(argv[0]);

            int result = hello_world_main(argc, argv);
            if (result != 0) {
                LOGE("%s:%d", argv[0], result);
                throw result;
            }
        }
#endif
#ifdef BASIC_QCOM_BLOCK_MATCH_SAD
        {
            const char* argv[] = {
                    "qcom_block_match_sad",
                    "/sdcard/DCIM/Camera/CL_QCOM_NV12__CL_UNORM_INT8__CONSTANT.dat",
                    "16",
                    "/sdcard/DCIM/Camera/qcom_block_match_sad.dat",
            };
            int argc = sizeof(argv) / sizeof(argv[0]);

            int result = qcom_block_match_sad_main(argc, argv);
            if (result != 0) {
                LOGE("%s:%d", argv[0], result);
                throw result;
            }
        }
#endif
    } catch (int e) {
        ret = (jint)e;
    }

    LOGD("[E]%s()=%d", __func__, ret);
    return ret;
}
