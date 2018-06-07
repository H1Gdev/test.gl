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

int executeOpenCL();

extern "C"
JNIEXPORT jint JNICALL
Java_com_example_h1gdev_androidapp_MainActivity_executeOpenCL(
        JNIEnv* /* env */,
        jobject /* this */) {
    LOGD("[S]%s()", __func__);
    jint ret = (jint)executeOpenCL();
    LOGD("[E]%s()=%d", __func__, ret);
    return ret;
}

//#define BASIC_COMPRESSED_IMAGE
#define BASIC_HELLO_WORLD
//#define BASIC_QCOM_BLOCK_MATCH_SAD
//#define BASIC_QCOM_BLOCK_MATCH_SSD
//#define BASIC_QCOM_BOX_FILTER_IMAGE
//#define BASIC_QCOM_CONVOLVE_IMAGE

// cl_qcom_ext_host_ptr
// cl_qcom_android_native_buffer_host_ptr -> cl_qcom_ext_host_ptr
// cl_qcom_ion_host_ptr                   -> cl_qcom_ext_host_ptr
// cl_qcom_ext_host_ptr_iocoherent        -> cl_qcom_ext_host_ptr
// cl_qcom_create_buffer_from_image       -> cl_qcom_ext_host_ptr
//
// cl_qcom_other_image           -> cl_qcom_android_native_buffer_host_ptr
//                               -> cl_qcom_ext_host_ptr
// cl_qcom_compressed_image      -> cl_qcom_android_native_buffer_host_ptr
//                               -> cl_qcom_ext_host_ptr
// cl_qcom_protected_context     -> cl_qcom_android_native_buffer_host_ptr
//                               -> cl_qcom_ext_host_ptr
//
// cl_qcom_extract_image_plane   -> cl_qcom_other_image
//                               -> cl_qcom_compressed_image
//
// cl_qcom_vector_image_ops      -> cl_qcom_extract_image_plane
// cl_qcom_accelerated_image_ops -> cl_qcom_extract_image_plane
//
// cl_qcom_perf_hint
// cl_qcom_priority_hint
// cl_qcom_subgroup_shuffle

#ifdef BASIC_COMPRESSED_IMAGE
int compressed_image_main(int argc, const char** argv);
#endif
#ifdef BASIC_HELLO_WORLD
int hello_world_main(int argc, const char** argv);
#endif
#ifdef BASIC_QCOM_BLOCK_MATCH_SAD
int qcom_block_match_sad_main(int argc, const char** argv);
#endif
#ifdef BASIC_QCOM_BLOCK_MATCH_SSD
int qcom_block_match_ssd_main(int argc, const char** argv);
#endif
#ifdef BASIC_QCOM_BOX_FILTER_IMAGE
int qcom_box_filter_image_main(int argc, const char** argv);
#endif
#ifdef BASIC_QCOM_CONVOLVE_IMAGE
int qcom_convolve_image_main(int argc, const char** argv);
#endif

extern "C"
JNIEXPORT jint JNICALL
Java_com_example_h1gdev_androidapp_MainActivity_executeOpenCLSdkExamples(
        JNIEnv* /* env */,
        jobject /* this */) {
    LOGD("[S]%s()", __func__);
    jint ret = 0;

    try {
#ifdef BASIC_COMPRESSED_IMAGE
        {
            const char* argv[] = {
                    "compressed_image",
                    "/sdcard/DCIM/Camera/CL_QCOM_NV12__CL_UNORM_INT8__CONSTANT.dat",
                    "/sdcard/DCIM/Camera/compressed_image.dat",
            };
            int argc = sizeof(argv) / sizeof(argv[0]);

            LOGD("[S]%s", argv[0]);
            int result = compressed_image_main(argc, argv);
            if (result != 0) {
                LOGE("%s:%d", argv[0], result);
                throw result;
            }
            LOGD("[E]%s()", argv[0]);
        }
#endif
#ifdef BASIC_HELLO_WORLD
        {
            const char* argv[] = {
                    "hello_world",
                    "/sdcard/DCIM/Camera/CL_QCOM_NV12__CL_UNORM_INT8__CONSTANT.dat",
                    "/sdcard/DCIM/Camera/hello_world.dat",
            };
            int argc = sizeof(argv) / sizeof(argv[0]);

            LOGD("[S]%s", argv[0]);
            int result = hello_world_main(argc, argv);
            if (result != 0) {
                LOGE("%s:%d", argv[0], result);
                throw result;
            }
            LOGD("[E]%s", argv[0]);
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

            LOGD("[S]%s", argv[0]);
            int result = qcom_block_match_sad_main(argc, argv);
            if (result != 0) {
                LOGE("%s:%d", argv[0], result);
                throw result;
            }
            LOGD("[E]%s", argv[0]);
        }
#endif
#ifdef BASIC_QCOM_BLOCK_MATCH_SSD
        {
            const char* argv[] = {
                    "qcom_block_match_ssd",
                    "/sdcard/DCIM/Camera/CL_QCOM_NV12__CL_UNORM_INT8__CONSTANT.dat",
                    "16",
                    "/sdcard/DCIM/Camera/qcom_block_match_ssd.dat",
            };
            int argc = sizeof(argv) / sizeof(argv[0]);

            LOGD("[S]%s", argv[0]);
            int result = qcom_block_match_ssd_main(argc, argv);
            if (result != 0) {
                LOGE("%s:%d", argv[0], result);
                throw result;
            }
            LOGD("[E]%s", argv[0]);
        }
#endif
#ifdef BASIC_QCOM_BOX_FILTER_IMAGE
        {
            const char* argv[] = {
                    "qcom_box_filter_image",
                    "/sdcard/DCIM/Camera/CL_QCOM_NV12__CL_UNORM_INT8__CONSTANT.dat",
                    "/sdcard/DCIM/Camera/qcom_box_filter_image.dat",
            };
            int argc = sizeof(argv) / sizeof(argv[0]);

            LOGD("[S]%s", argv[0]);
            int result = qcom_box_filter_image_main(argc, argv);
            if (result != 0) {
                LOGE("%s:%d", argv[0], result);
                throw result;
            }
            LOGD("[E]%s", argv[0]);
        }
#endif
#ifdef BASIC_QCOM_CONVOLVE_IMAGE
        {
            const char* argv[] = {
                    "qcom_convolve_image",
                    "/sdcard/DCIM/Camera/CL_QCOM_NV12__CL_UNORM_INT8__CONSTANT.dat",
                    "/sdcard/DCIM/Camera/qcom_convolve_image.dat",
            };
            int argc = sizeof(argv) / sizeof(argv[0]);

            LOGD("[S]%s", argv[0]);
            int result = qcom_convolve_image_main(argc, argv);
            if (result != 0) {
                LOGE("%s:%d", argv[0], result);
                throw result;
            }
            LOGD("[E]%s", argv[0]);
        }
#endif
    } catch (int e) {
        ret = (jint)e;
    }

    LOGD("[E]%s()=%d", __func__, ret);
    return ret;
}
