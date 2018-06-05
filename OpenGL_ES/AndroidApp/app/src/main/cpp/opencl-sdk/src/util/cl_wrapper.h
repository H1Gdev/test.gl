//--------------------------------------------------------------------------------------
// File: cl_wrapper.h
// Desc:
//
// Author:      QUALCOMM
//
//               Copyright (c) 2017 QUALCOMM Technologies, Inc.
//                         All Rights Reserved.
//                      QUALCOMM Proprietary/GTDR
//--------------------------------------------------------------------------------------

#ifndef SDK_EXAMPLES_CL_WRAPPER_H
#define SDK_EXAMPLES_CL_WRAPPER_H
#include <string>
#include <vector>
#include <utility>

#include <CL/cl.h>
#include <CL/cl_ext_qcom.h>

#ifdef USES_ANDROID_CMAKE
#include <ion.h>
#include <msm_ion.h>
#else
#include <linux/ion.h>
#ifndef EXECUTE_IN_JNI
#include <linux/msm_ion.h>
#endif
#endif

#include "util.h"

/**
 * \brief A wrapper around OpenCL setup/teardown code.
 *
 * All objects exposed are owned by the wrapper, and are cleaned up when it is destroyed.
 */
class cl_wrapper {
public:
    /**
     * \brief Sets up OpenCL.
     */
    cl_wrapper();

    /**
     * \brief Frees associated OpenCL objects, including the results of make_kernel, make_program, and make_ion_buffer.
     */
    ~cl_wrapper();

    /**
     * \brief Gets the cl_context associated with the wrapper for using in OpenCL functions.
     * @return
     */
    cl_context          get_context() const;

    /**
    * \brief Gets the cl_command_queue associated with the wrapper for using in OpenCL functions.
    * @return
    */
    cl_command_queue    get_command_queue() const;

    /**
     * \brief Makes a cl_kernel from the given program.
     *
     * @param kernel_name
     * @param program
     * @return
     */
    cl_kernel           make_kernel(const std::string &kernel_name, cl_program program);

    /**
     * Makes a cl_program (whose lifetime is managed by cl_wrapper) from the given source code strings.
     *
     * @param program_source - The source code strings.
     * @param program_source_len - The length of program_source
     * @return
     */
    cl_program          make_program(const char **program_source, cl_uint program_source_len);

    /**
     * \brief Makes an ion buffer that can be used for a YUV 4:2:0 image.
     *
     * @param img_format [in] - The image format
     * @param img_desc [in] - The image description
     * @return
     */
    cl_mem_ion_host_ptr make_ion_buffer_for_yuv_image(const cl_image_format &img_format, const cl_image_desc &img_desc);

    /**
     * \brief Makes an ion buffer that can be used for a compressed image.
     *
     * @param img_format [in] - The image format
     * @param img_desc [in] - The image description
     * @return
     */
    cl_mem_ion_host_ptr make_ion_buffer_for_compressed_image(cl_image_format img_format, const cl_image_desc &img_desc);

    /**
     * \brief Makes an ion buffer of the specified size.
     *
     * @param size [in] - Desired buffer size
     * @return
     */
    cl_mem_ion_host_ptr make_ion_buffer(size_t size);

    /**
     * \brief Checks if the wrapped device supports the desired extension via clGetDeviceInfo
     *
     * @param desired_extension
     * @return true if the desired_extension is supported, otherwise false
     */
    bool                check_extension_support(const std::string &desired_extension) const;

    /**
     * \brief Gets the required row pitch for the given image. Must be considered when accessing the underlying ion buffer.
     *
     * @param img_format [in] - The image format
     * @param img_desc [in] - The image description
     * @return the image row pitch
     */
    size_t              get_ion_image_row_pitch(const cl_image_format &img_format, const cl_image_desc &img_desc) const;

private:
    cl_device_id m_device;
    cl_context m_context;
    cl_command_queue m_cmd_queue;
    std::vector<cl_program> m_programs;
    std::vector<cl_kernel> m_kernels;
    std::vector<std::pair<void*, size_t>> m_ion_host_ptrs;
    std::vector<ion_fd_data> m_fd_data;
    std::vector<ion_handle_data> m_handle_data;
    int m_ion_device_fd;
};


#endif //SDK_EXAMPLES_CL_CONTEXT_WRAPPER_H
