#include <CL/cl.h>

#define LOG_MSG_FLAG
//#undef LOG_MSG_FLAG
#define LOG_TAG "OpenCL-Test"
#include "log.h"

int outputCLInfo() {
    int ret = 0;
    try {
        cl_uint num_platforms = 0;
        cl_int err = clGetPlatformIDs(0, nullptr, &num_platforms);
        if (err != CL_SUCCESS) {
            LOGE("clGetPlatformIDs()=%d", err);
            throw err;
        }
        LOGI("num_platforms=%d", num_platforms);
        cl_platform_id platforms[num_platforms];
        err = clGetPlatformIDs(num_platforms, platforms, nullptr);
        if (err != CL_SUCCESS) {
            LOGE("clGetPlatformIDs()=%d", err);
            throw err;
        }

        const cl_platform_info param_names[] = {
                CL_PLATFORM_NAME,
                CL_PLATFORM_VERSION,
                CL_PLATFORM_VENDOR,
                CL_PLATFORM_PROFILE,
                CL_PLATFORM_EXTENSIONS,
        };
        for (cl_uint i = 0; i < num_platforms; ++i) {
            for (cl_uint j = 0; j < sizeof(param_names) / sizeof(param_names[0]); ++j) {
                size_t size;
                err = clGetPlatformInfo(platforms[i], param_names[j], 0, nullptr, &size);
                if (err != CL_SUCCESS) {
                    LOGE("clGetPlatformInfo()=%d", err);
                    throw err;
                }
                char value[size];
                err = clGetPlatformInfo(platforms[i], param_names[j], size, value, nullptr);
                if (err != CL_SUCCESS) {
                    LOGE("clGetPlatformInfo()=%d", err);
                    throw err;
                }
                LOGI("[%u]%s", i, value);
            }

            cl_uint num_devices = 0;
            err = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, 0, nullptr, &num_devices);
            if (err != CL_SUCCESS) {
                LOGE("clGetDeviceIDs()=%d", err);
                throw err;
            }
            LOGI("[%u]num_devices=%d", i, num_devices);
            cl_device_id devices[num_devices];
            err = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_GPU, num_devices, devices, nullptr);
            if (err != CL_SUCCESS) {
                LOGE("clGetDeviceIDs()=%d", err);
                throw err;
            }
            for (cl_uint j = 0; j < num_devices; ++j) {
                LOGI("[%u][%u]GetDeviceInfo", i, j);
                cl_device_id device = devices[j];

                struct device_info;
                struct device_info {
                    cl_device_info info;
                    const char* name;

                    void (* fun)(uint8_t*, void*);  // 前方宣言してもdevice_info&を引数にできなかった...
                    const char* suffix;

                    static void output_string(uint8_t* value, void* data) {
                        device_info& di = *((device_info*) data);
                        const char* s = di.suffix ? di.suffix : "";
                        LOGI("%s: %s %s", di.name, value, s);
                    }

                    static void output_size_t(uint8_t* value, void* data) {
                        device_info& di = *((device_info*) data);
                        const char* s = di.suffix ? di.suffix : "";
                        LOGI("%s: %zu %s", di.name, *((size_t*) value), s);
                    }

                    static void output_cl_uint(uint8_t* value, void* data) {
                        device_info& di = *((device_info*) data);
                        const char* s = di.suffix ? di.suffix : "";
                        LOGI("%s: %u %s", di.name, *((cl_uint*) value), s);
                    }

                    static void output_cl_ulong(uint8_t* value, void* data) {
                        device_info& di = *((device_info*) data);
                        const char* s = di.suffix ? di.suffix : "";
                        LOGI("%s: %lu %s", di.name, *((cl_ulong*) value), s);
                    }
                };
                const device_info infoList[] = {
                        {CL_DEVICE_NAME, "CL_DEVICE_NAME", device_info::output_string, nullptr},
                        {CL_DEVICE_VENDOR, "CL_DEVICE_VENDOR", device_info::output_string, nullptr},
                        {CL_DRIVER_VERSION, "CL_DRIVER_VERSION", device_info::output_string, nullptr},
                        {CL_DEVICE_OPENCL_C_VERSION, "CL_DEVICE_OPENCL_C_VERSION", device_info::output_string, nullptr},
                        {CL_DEVICE_VERSION, "CL_DEVICE_VERSION", device_info::output_string, nullptr},
                        {CL_DEVICE_EXTENSIONS, "CL_DEVICE_EXTENSIONS", device_info::output_string, nullptr},
                        {CL_DEVICE_BUILT_IN_KERNELS, "CL_DEVICE_BUILT_IN_KERNELS", device_info::output_string, nullptr},
                        {CL_DEVICE_MAX_CLOCK_FREQUENCY, "CL_DEVICE_MAX_CLOCK_FREQUENCY", device_info::output_cl_uint, "MHz"},
                        {CL_DEVICE_ADDRESS_BITS, "CL_DEVICE_ADDRESS_BITS", device_info::output_cl_uint, "bit"},
                        {CL_DEVICE_MAX_COMPUTE_UNITS, "CL_DEVICE_MAX_COMPUTE_UNITS", device_info::output_cl_uint, nullptr},

                        {CL_DEVICE_MEM_BASE_ADDR_ALIGN, "CL_DEVICE_MEM_BASE_ADDR_ALIGN", device_info::output_cl_uint, "bit"},
                        {CL_DEVICE_GLOBAL_MEM_SIZE, "CL_DEVICE_GLOBAL_MEM_SIZE", device_info::output_cl_ulong, "byte"},
                        {CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, "CL_DEVICE_GLOBAL_MEM_CACHE_SIZE", device_info::output_cl_ulong, "byte"},
                        {CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE, "CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE", device_info::output_cl_uint, "byte"},
                        {CL_DEVICE_MAX_MEM_ALLOC_SIZE, "CL_DEVICE_MAX_MEM_ALLOC_SIZE", device_info::output_cl_ulong, "byte"},
                        {CL_DEVICE_LOCAL_MEM_SIZE, "CL_DEVICE_LOCAL_MEM_SIZE", device_info::output_cl_ulong, "byte"},

                        {CL_DEVICE_MAX_WORK_GROUP_SIZE, "CL_DEVICE_MAX_WORK_GROUP_SIZE", device_info::output_size_t, nullptr},
                        {CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, "CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS", device_info::output_size_t, nullptr},

                        {CL_DEVICE_MAX_PARAMETER_SIZE, "CL_DEVICE_MAX_PARAMETER_SIZE", device_info::output_size_t, "byte"},
                        {CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE, "CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE", device_info::output_size_t, "(?)"},
                        {CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE, "CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE", device_info::output_size_t, "(?)"},
                        {CL_DEVICE_IMAGE_MAX_BUFFER_SIZE, "CL_DEVICE_IMAGE_MAX_BUFFER_SIZE", device_info::output_size_t, "pixel"},
                        {CL_DEVICE_IMAGE_MAX_ARRAY_SIZE, "CL_DEVICE_IMAGE_MAX_ARRAY_SIZE", device_info::output_size_t, "image"},
                        {CL_DEVICE_PRINTF_BUFFER_SIZE, "CL_DEVICE_PRINTF_BUFFER_SIZE", device_info::output_size_t, "byte"},
                };
                for (device_info info : infoList) {
                    size_t size = 0;
                    err = clGetDeviceInfo(device, info.info, 0, nullptr, &size);
                    if (err != CL_SUCCESS) {
                        LOGE("clGetDeviceInfo()=%d", err);
                    }
                    uint8_t value[size];
                    err = clGetDeviceInfo(device, info.info, size, value, nullptr);
                    if (err != CL_SUCCESS) {
                        LOGE("clGetDeviceInfo()=%d", err);
                    }
                    info.fun(value, &info);
                }
            }
        }
    } catch (cl_int e) {
        ret = (int)e;
    }
    return ret;
}
