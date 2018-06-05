//--------------------------------------------------------------------------------------
// File: half_float.h
// Desc:
//
// Author:      QUALCOMM
//
//               Copyright (c) 2017 QUALCOMM Technologies, Inc.
//                         All Rights Reserved.
//                      QUALCOMM Proprietary/GTDR
//--------------------------------------------------------------------------------------

#ifndef SDK_EXAMPLES_HALF_FLOAT_H
#define SDK_EXAMPLES_HALF_FLOAT_H

#include <CL/cl.h>

/**
 * \brief Given a 32-bit float, converts it (potentially with some error due to loss of precision)
 * to a 16-bit half float for use with OpenCL.
 *
 * @param f [in] - The 32-bit float to convert
 * @return the equivalent 16-bit half float
 */
cl_half to_half(float f);

#endif //SDK_EXAMPLES_HALF_FLOAT_H
