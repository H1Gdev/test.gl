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
//#define CONVERSIONS_P010_TO_COMPRESSED_TP10
//#define CONVOLUTIONS_ACCELERATED_CONVOLUTION
//#define CONVOLUTIONS_CONVOLUTION
//#define VECTOR_IMAGE_OPS_COMPRESSED_NV12_VECTOR_IMAGE_OPS
//#define VECTOR_IMAGE_OPS_COMPRESSED_P010_VECTOR_IMAGE_OPS
//#define VECTOR_IMAGE_OPS_COMPRESSED_TP10_VECTOR_IMAGE_OPS
//#define VECTOR_IMAGE_OPS_NV12_VECTOR_IMAGE_OPS
//#define VECTOR_IMAGE_OPS_P010_VECTOR_IMAGE_OPS
//#define VECTOR_IMAGE_OPS_TP10_VECTOR_IMAGE_OPS

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
#ifdef CONVERSIONS_P010_TO_COMPRESSED_TP10
int p010_to_compressed_tp10_main(int argc, const char** argv);
#endif
#ifdef CONVOLUTIONS_ACCELERATED_CONVOLUTION
int accelerated_convolution_main(int argc, const char** argv);
#endif
#ifdef CONVOLUTIONS_CONVOLUTION
int convolution_main(int argc, const char** argv);
#endif
#ifdef VECTOR_IMAGE_OPS_COMPRESSED_NV12_VECTOR_IMAGE_OPS
int compressed_nv12_vector_image_ops_main(int argc, const char** argv);
#endif
#ifdef VECTOR_IMAGE_OPS_COMPRESSED_P010_VECTOR_IMAGE_OPS
int compressed_p010_vector_image_ops_main(int argc, const char** argv);
#endif
#ifdef VECTOR_IMAGE_OPS_COMPRESSED_TP10_VECTOR_IMAGE_OPS
int compressed_tp10_vector_image_ops_main(int argc, const char** argv);
#endif
#ifdef VECTOR_IMAGE_OPS_NV12_VECTOR_IMAGE_OPS
int nv12_vector_image_ops_main(int argc, const char** argv);
#endif
#ifdef VECTOR_IMAGE_OPS_P010_VECTOR_IMAGE_OPS
int p010_vector_image_ops_main(int argc, const char** argv);
#endif
#ifdef VECTOR_IMAGE_OPS_TP10_VECTOR_IMAGE_OPS
int tp10_vector_image_ops_main(int argc, const char** argv);
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
#ifdef CONVERSIONS_P010_TO_COMPRESSED_TP10
        {
            const char* argv[] = {
                    "p010_to_compressed_tp10",
                    "/sdcard/DCIM/Camera/CL_QCOM_NV12__CL_UNORM_INT8__CONSTANT.dat",
                    "/sdcard/DCIM/Camera/p010_to_compressed_tp10.dat",
            };
            int argc = sizeof(argv) / sizeof(argv[0]);

            LOGD("[S]%s", argv[0]);
            int result = p010_to_compressed_tp10_main(argc, argv);
            if (result != 0) {
                LOGE("%s:%d", argv[0], result);
                throw result;
            }
            LOGD("[E]%s", argv[0]);
        }
#endif
#ifdef CONVOLUTIONS_ACCELERATED_CONVOLUTION
        {
            const char* argv[] = {
                    "accelerated_convolution",
                    "/sdcard/DCIM/Camera/CL_QCOM_NV12__CL_UNORM_INT8__CONSTANT.dat",
                    "/sdcard/DCIM/Camera/accelerated_convolution.dat",
            };
            int argc = sizeof(argv) / sizeof(argv[0]);

            LOGD("[S]%s", argv[0]);
            int result = accelerated_convolution_main(argc, argv);
            if (result != 0) {
                LOGE("%s:%d", argv[0], result);
                throw result;
            }
            LOGD("[E]%s", argv[0]);
        }
#endif
#ifdef CONVOLUTIONS_CONVOLUTION
        {
            const char* argv[] = {
                    "convolution",
                    "/sdcard/DCIM/Camera/CL_QCOM_NV12__CL_UNORM_INT8__CONSTANT.dat",
                    "/sdcard/DCIM/Camera/convolution.dat",
            };
            int argc = sizeof(argv) / sizeof(argv[0]);

            LOGD("[S]%s", argv[0]);
            int result = convolution_main(argc, argv);
            if (result != 0) {
                LOGE("%s:%d", argv[0], result);
                throw result;
            }
            LOGD("[E]%s", argv[0]);
        }
#endif
#ifdef VECTOR_IMAGE_OPS_COMPRESSED_NV12_VECTOR_IMAGE_OPS
        {
            const char* argv[] = {
                    "compressed_nv12_vector_image_ops",
                    "/sdcard/DCIM/Camera/CL_QCOM_NV12__CL_UNORM_INT8__CONSTANT.dat",
            };
            int argc = sizeof(argv) / sizeof(argv[0]);

            LOGD("[S]%s", argv[0]);
            int result = compressed_nv12_vector_image_ops_main(argc, argv);
            if (result != 0) {
                LOGE("%s:%d", argv[0], result);
                throw result;
            }
            LOGD("[E]%s", argv[0]);
        }
#endif
#ifdef VECTOR_IMAGE_OPS_COMPRESSED_P010_VECTOR_IMAGE_OPS
        {
            const char* argv[] = {
                    "compressed_p010_vector_image_ops",
                    "/sdcard/DCIM/Camera/CL_QCOM_NV12__CL_UNORM_INT8__CONSTANT.dat",
            };
            int argc = sizeof(argv) / sizeof(argv[0]);

            LOGD("[S]%s", argv[0]);
            int result = compressed_p010_vector_image_ops_main(argc, argv);
            if (result != 0) {
                LOGE("%s:%d", argv[0], result);
                throw result;
            }
            LOGD("[E]%s", argv[0]);
        }
#endif
#ifdef VECTOR_IMAGE_OPS_COMPRESSED_TP10_VECTOR_IMAGE_OPS
        {
            const char* argv[] = {
                    "compressed_tp10_vector_image_ops",
                    "/sdcard/DCIM/Camera/CL_QCOM_NV12__CL_UNORM_INT8__CONSTANT.dat",
            };
            int argc = sizeof(argv) / sizeof(argv[0]);

            LOGD("[S]%s", argv[0]);
            int result = compressed_tp10_vector_image_ops_main(argc, argv);
            if (result != 0) {
                LOGE("%s:%d", argv[0], result);
                throw result;
            }
            LOGD("[E]%s", argv[0]);
        }
#endif
#ifdef VECTOR_IMAGE_OPS_NV12_VECTOR_IMAGE_OPS
        {
            const char* argv[] = {
                    "nv12_vector_image_ops",
                    "/sdcard/DCIM/Camera/CL_QCOM_NV12__CL_UNORM_INT8__CONSTANT.dat",
            };
            int argc = sizeof(argv) / sizeof(argv[0]);

            LOGD("[S]%s", argv[0]);
            int result = nv12_vector_image_ops_main(argc, argv);
            if (result != 0) {
                LOGE("%s:%d", argv[0], result);
                throw result;
            }
            LOGD("[E]%s", argv[0]);
        }
#endif
#ifdef VECTOR_IMAGE_OPS_P010_VECTOR_IMAGE_OPS
        {
            const char* argv[] = {
                    "p010_vector_image_ops",
                    "/sdcard/DCIM/Camera/CL_QCOM_P010__CL_QCOM_UNORM_INT10__CONSTANT.dat",
            };
            int argc = sizeof(argv) / sizeof(argv[0]);

            LOGD("[S]%s", argv[0]);
            int result = p010_vector_image_ops_main(argc, argv);
            if (result != 0) {
                LOGE("%s:%d", argv[0], result);
                throw result;
            }
            LOGD("[E]%s", argv[0]);
        }
#endif
#ifdef VECTOR_IMAGE_OPS_TP10_VECTOR_IMAGE_OPS
        {
            const char* argv[] = {
                    "tp10_vector_image_ops",
                    "/sdcard/DCIM/Camera/CL_QCOM_TP10__CL_QCOM_UNORM_INT10__CONSTANT.dat",
            };
            int argc = sizeof(argv) / sizeof(argv[0]);

            LOGD("[S]%s", argv[0]);
            int result = tp10_vector_image_ops_main(argc, argv);
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
