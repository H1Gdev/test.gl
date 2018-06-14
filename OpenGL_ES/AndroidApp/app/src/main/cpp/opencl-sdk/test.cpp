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


#define OPENCL_C_STYLE
#define OPENCL_CPP_STYLE

#ifdef OPENCL_C_STYLE
static void notifyContext(const char* errinfo, const void* private_info, size_t cb, void* user_data) {
    LOGE("notifyContext");
    LOGE("errinfo:%s", errinfo);
    LOGE("private_info:%p, cb:%zu", private_info, cb);
    LOGE("user_data:%p", user_data);
}

cl_int executeC() {
    cl_int ret = 0;

    cl_context context = 0;
    cl_command_queue command_queue = 0;
    cl_program program = 0;
    cl_kernel kernel = 0;
    cl_event event = 0;
    try {
        cl_int err;

        // Platform(s)
        cl_uint num_platforms = 0;
        err = clGetPlatformIDs(0, nullptr, &num_platforms);
        if (err != CL_SUCCESS) {
            throw err;
        } else {
            LOGD("num_platforms=%u", num_platforms);
        }
        cl_platform_id platforms[num_platforms];
        err = clGetPlatformIDs(num_platforms, platforms, nullptr);
        if (err != CL_SUCCESS) {
            throw err;
        }
        cl_platform_id platform = platforms[0];

        const cl_device_type device_type = CL_DEVICE_TYPE_GPU;
#if 1
        // Platform - Device(s)
        cl_uint num_devices = 0;
        err = clGetDeviceIDs(platform, device_type, 0, nullptr, &num_devices);
        if (err != CL_SUCCESS) {
            throw err;
        } else {
            LOGD("num_devices=%u", num_devices);
        }
        cl_device_id devices[num_devices];
        err = clGetDeviceIDs(platform, device_type, num_devices, devices, nullptr);
        if (err != CL_SUCCESS) {
            throw err;
        }
        cl_device_id device = devices[0];

        // Context - Device(s)
        context = clCreateContext(nullptr, 1, &device, notifyContext, nullptr, &err);
        if (err != CL_SUCCESS) {
            throw err;
        }
#else
        // Context - Device(s)
        context = clCreateContextFromType(nullptr, device_type, notify, nullptr, &err);
        if (err != CL_SUCCESS) {
            throw err;
        }

        size_t num_devices = 0;
        err = clGetContextInfo(context, CL_CONTEXT_DEVICES, 0, nullptr, &num_devices);
        if (err != CL_SUCCESS) {
            throw err;
        }
        cl_device_id devices[num_devices];
        err = clGetContextInfo(context, CL_CONTEXT_DEVICES, num_devices, devices, nullptr);
        if (err != CL_SUCCESS) {
            throw err;
        }
        cl_device_id device = devices[0];
#endif
        // (Context, Device) - Queue(s)
        command_queue = clCreateCommandQueue(context, device, 0, &err);
        if (err != CL_SUCCESS) {
            throw err;
        }

        // (Context, Device) - Program(s)
        {
            // @formatter:off
            const char* source =
                    // Kernel
                    "kernel void execute0(int n) {"
                    "}";
            // @formatter:on
            // Source, Binary, Built-in kernels, or IL.
            program = clCreateProgramWithSource(context, 1, &source, nullptr, &err);
            if (err != CL_SUCCESS) {
                throw err;
            }

#if 0
            const cl_device_id* device_list = nullptr;
            cl_uint num_program_devices = 0;
#else
            const cl_device_id* device_list = &device;
            cl_uint num_program_devices = 1;
#endif
            // @formatter:off
            const char* options =
                    " -Werror"
                    " -cl-std=CL2.0";
            // @formatter:on
            void (* pfn_notify)(cl_program, void*) = nullptr;   // wait for build to complete.
            err = clBuildProgram(program, num_program_devices, device_list, options, pfn_notify, nullptr);
            if (err != CL_SUCCESS) {
                size_t size = 0;
                {
                    err = clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_OPTIONS, 0, nullptr, &size);
                    if (err != CL_SUCCESS) {
                        throw err;
                    }
                    char value[size];
                    err = clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_OPTIONS, size, value, nullptr);
                    if (err != CL_SUCCESS) {
                        throw err;
                    }
                    LOGD("CL_PROGRAM_BUILD_OPTIONS=%s", value);
                }
                {
                    cl_build_status value;
                    err = clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_STATUS, sizeof(cl_build_status), &value, nullptr);
                    if (err != CL_SUCCESS) {
                        throw err;
                    }
                    LOGD("CL_PROGRAM_BUILD_STATUS=%d", value);
                }
                {
                    err = clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, nullptr, &size);
                    if (err != CL_SUCCESS) {
                        throw err;
                    }
                    char value[size];
                    err = clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, size, &value, nullptr);
                    if (err != CL_SUCCESS) {
                        throw err;
                    }
                    LOGE("CL_PROGRAM_BUILD_LOG=%s", value);
                }
                throw err;
            }
        }

        // Program - Kernel(s)
        const char* kernel_name = "execute0";
        kernel = clCreateKernel(program, kernel_name, &err);
        if (err != CL_SUCCESS) {
            throw err;
        }

        const cl_int arg0 = 100;
        err = clSetKernelArg(kernel, 0, sizeof(arg0), &arg0);
        if (err != CL_SUCCESS) {
            throw err;
        }
#if 1
        {
            size_t size;
            err = clGetKernelArgInfo(kernel, 0, CL_KERNEL_ARG_NAME, 0, nullptr, &size);
            if (err != CL_SUCCESS) {
                throw err;
            }
            char value[size];
            err = clGetKernelArgInfo(kernel, 0, CL_KERNEL_ARG_NAME, size, value, nullptr);
            if (err != CL_SUCCESS) {
                throw err;
            }
            LOGD("[0]CL_KERNEL_ARG_NAME:%s", value);
        }
#endif

        // work-group
        //  work-item
        const cl_uint dimension = 2;
        const size_t global_work_offset[] = {0, 0};
        const size_t global_work_size[] = {10, 10}; // work-groups
        const size_t local_work_size[] = {5, 5};    // work-items
        err = clEnqueueNDRangeKernel(
                command_queue,
                kernel,
                dimension,
                global_work_offset,
                global_work_size,
                local_work_size,
                0,
                nullptr,
                &event
        );
        if (err != CL_SUCCESS) {
            throw err;
        }

        err = clFlush(command_queue);
        if (err != CL_SUCCESS) {
            throw err;
        }

        // Event
        err = clWaitForEvents(1, &event);
        if (err != CL_SUCCESS) {
            throw err;
        }
#if 1
        {
            cl_int status;
            clGetEventInfo(event, CL_EVENT_COMMAND_EXECUTION_STATUS, sizeof(status), &status, nullptr);
            if (err != CL_SUCCESS) {
                throw err;
            } else if (status != CL_COMPLETE) {
                LOGE("status=%d", status);
                throw err;
            }
        }
#endif
        err = clReleaseEvent(event);
        if (err != CL_SUCCESS) {
            throw err;
        }
        event = 0;

        err = clFinish(command_queue);
        if (err != CL_SUCCESS) {
            throw err;
        }

        throw err;  // for finalize...
    } catch (cl_int e) {
        cl_int err;

        if (event != 0) {
            err = clReleaseEvent(event);
            if (err != CL_SUCCESS) {
                LOGE("clReleaseEvent()=%d", err);
            }
        }

        if (kernel != 0) {
            err = clReleaseKernel  (kernel);
            if (err != CL_SUCCESS) {
                LOGE("clReleaseKernel()=%d", err);
            }
        }
        if (program != 0) {
            err = clReleaseProgram (program);
            if (err != CL_SUCCESS) {
                LOGE("clReleaseProgram()=%d", err);
            }
        }
        if (command_queue != 0) {
            err = clReleaseCommandQueue(command_queue);
            if (err != CL_SUCCESS) {
                LOGE("clReleaseCommandQueue()=%d", err);
            }
        }
        if (context != 0) {
            err = clReleaseContext(context);
            if (err != CL_SUCCESS) {
                LOGE("clReleaseContext()=%d", err);
            }
        }

        ret = e;
    }
    return ret;
}
#endif
#ifdef OPENCL_CPP_STYLE
#define CL_HPP_ENABLE_EXCEPTIONS
#include <CL/cl2.hpp>

cl_int executeCpp() {
    cl_int ret = 0;

    try {
#if 0
        {
            // Provide getDefault() and setDefault() method.
            cl::Platform platform = cl::Platform::getDefault();
            LOGD("[Platform]%s", platform.getInfo<CL_PLATFORM_NAME>().c_str());
            cl::Device device = cl::Device::getDefault();
            LOGD("[Device]%s", device.getInfo<CL_DEVICE_NAME>().c_str());
            cl::Context context = cl::Context::getDefault();
            LOGD("[Context]Device:%s", context.getInfo<CL_CONTEXT_DEVICES>()[0].getInfo<CL_DEVICE_NAME>().c_str());
            cl::CommandQueue queue = cl::CommandQueue::getDefault();
            LOGD("[CommandQueue]Device:%s", queue.getInfo<CL_QUEUE_DEVICE>().getInfo<CL_DEVICE_NAME>().c_str());
        }
#endif

        // Platform(s)
        std::vector<cl::Platform> allPlatforms;
        cl::Platform::get(&allPlatforms);
        LOGD("num_platforms=%zu", allPlatforms.size());
        cl::Platform platform = allPlatforms[0];

        const cl_device_type device_type = CL_DEVICE_TYPE_GPU;
#if 1
        // Platform - Device(s)
        std::vector<cl::Device> devices;
        platform.getDevices(device_type, &devices);
        LOGD("num_devices=%zu", devices.size());
        cl::Device device = devices[0];

        // Context - Device(s)
        cl::Context context(device);
#else
        // Context - Device(s)
        cl::Context context(device_type);

        std::vector<cl::Device> devices = context.getInfo<CL_CONTEXT_DEVICES>();
        cl::Device device = devices[0];
#endif
        // (Context, Device) - Queue(s)
        cl::CommandQueue queue = cl::CommandQueue(context, device);

        // (Context, Device) - Program(s)
        cl::Program program;
        {
            // @formatter:off
            std::string source =
                    // Kernel
                    "kernel void execute0(int n) {"
                    "}";
            // @formatter:on
            // Source, Binary, Built-in kernels, or IL.
            program = cl::Program(context, source);

            // @formatter:off
            std::string options =
                    " -Werror"
                    " -cl-std=CL2.0";
            // @formatter:on
#if 1
            program.build(options.c_str());
#else
            program.build(std::vector<cl::Device>({device}), options.c_str());
#endif
        }

        // Program - Kernel(s)
        cl::Kernel kernel(program, "execute0");

        const cl_int arg0 = 100;
        kernel.setArg(0, arg0);
#if 1
        LOGD("[0]CL_KERNEL_ARG_NAME:%s", kernel.getArgInfo<CL_KERNEL_ARG_NAME>(0).c_str());
#endif

        // work-group
        //  work-item
        const cl::NDRange offset(0, 0);
        const cl::NDRange global(10, 10);   // work-groups
        const cl::NDRange local(5, 5);      // work-items
        cl::Event event;
        queue.enqueueNDRangeKernel(kernel, offset, global, local, nullptr, &event);

        queue.flush();

        // Event
        event.wait();
#if 1
        {
            cl_int status = event.getInfo<CL_EVENT_COMMAND_EXECUTION_STATUS>();
            if (status != CL_COMPLETE) {
                LOGE("status=%d", status);
            }
        }
#endif

        queue.finish();
    } catch (cl::BuildError& e) {
        LOGE("%s", e.what());
        for (const std::pair<cl::Device, std::string>& buildLog : e.getBuildLog()) {
            LOGE("%s:%s", buildLog.first.getInfo<CL_DEVICE_NAME>().c_str(), buildLog.second.c_str());
        }
        ret = e.err();
    } catch (cl::Error& e) {
        LOGE("%s", e.what());
        ret = e.err();
    }
    return ret;
}
#endif

int executeOpenCL() {
    int ret = 0;
#ifdef OPENCL_C_STYLE
    LOGD("Execute OpenCL C");
    ret |= (int)executeC();
#endif
#ifdef OPENCL_CPP_STYLE
    LOGD("Execute OpenCL C++");
    ret |= (int)executeCpp();
#endif
    return ret;
}
