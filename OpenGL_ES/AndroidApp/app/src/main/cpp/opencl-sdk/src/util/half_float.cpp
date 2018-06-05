//--------------------------------------------------------------------------------------
// File: half_float.cpp
// Desc:
//
// Author:      QUALCOMM
//
//               Copyright (c) 2017 QUALCOMM Technologies, Inc.
//                         All Rights Reserved.
//                      QUALCOMM Proprietary/GTDR
//--------------------------------------------------------------------------------------

#include "half_float.h"
#include <cmath>

cl_half to_half(float f)
{
    static const struct
    {
        unsigned int bit_size       = 16;                                                 // total number of bits in the representation
        unsigned int num_frac_bits  = 10;                                                 // number of fractional (mantissa) bits
        unsigned int num_exp_bits   = 5;                                                  // number of (biased) exponent bits
        unsigned int sign_bit       = 15;                                                 // position of the sign bit
        unsigned int sign_mask      = 1 << 15;                                            // mask to extract sign bit
        unsigned int frac_mask      = (1 << 10) - 1;                                      // mask to extract the fractional (mantissa) bits
        unsigned int exp_mask       = ((1 << 5) - 1) << 10;                               // mask to extract the exponent bits
        int          exp_bias       = (1 << (5 - 1)) - 1;                                 // bias for the exponent
        unsigned int e_max          = (1 << (5 - 1)) - 1;                                 // max value for the exponent
        int          e_min          = -((1 << (5 - 1)) - 1) + 1;                          // min value for the exponent
        unsigned int max_normal     = ((((1 << (5 - 1)) - 1) + 127) << 23) | 0x7FE000;    // max value that can be represented by the 16 bit float
        unsigned int min_normal     = ((-((1 << (5 - 1)) - 1) + 1) + 127) << 23;          // min value that can be represented by the 16 bit float
        unsigned int bias_diff      = ((unsigned int)(((1 << (5 - 1)) - 1) - 127) << 23); // difference in bias between the float16 and float32 exponent
        unsigned int frac_bits_diff = 23 - 10;                                            // difference in number of fractional bits between float16/float32
    } float16_params;

    static const struct
    {
        unsigned int abs_value_mask    = 0x7FFFFFFF; // ANDing with this value gives the abs value
        unsigned int sign_bit_mask     = 0x80000000; // ANDing with this value gives the sign
        unsigned int e_max             = 127;        // max value for the exponent
        unsigned int num_mantissa_bits = 23;         // 23 bit mantissa on single precision floats
        unsigned int mantissa_mask     = 0x007FFFFF; // 23 bit mantissa on single precision floats
    } float32_params;

    const union
    {
        float f;
        unsigned int bits;
    } value = {f};

    const unsigned int f_abs_bits = value.bits & float32_params.abs_value_mask;
    const bool         is_neg     = value.bits & float32_params.sign_bit_mask;
    const unsigned int sign       = (value.bits & float32_params.sign_bit_mask) >> (float16_params.num_frac_bits + float16_params.num_exp_bits + 1);
    cl_half            half       = 0;

    if (std::isnan(value.f))
    {
        half = float16_params.exp_mask | float16_params.frac_mask;
    }
    else if (std::isinf(value.f))
    {
        half = is_neg ? float16_params.sign_mask | float16_params.exp_mask : float16_params.exp_mask;
    }
    else if (f_abs_bits > float16_params.max_normal)
    {
        // Clamp to max float 16 value
        half = sign | (((1 << float16_params.num_exp_bits) - 1) << float16_params.num_frac_bits) | float16_params.frac_mask;
    }
    else if (f_abs_bits < float16_params.min_normal)
    {
        const unsigned int frac_bits    = (f_abs_bits & float32_params.mantissa_mask) | (1 << float32_params.num_mantissa_bits);
        const int          nshift       = float16_params.e_min + float32_params.e_max - (f_abs_bits >> float32_params.num_mantissa_bits);
        const unsigned int shifted_bits = nshift < 24 ? frac_bits >> nshift : 0;
        half                            = sign | (shifted_bits >> float16_params.frac_bits_diff);
    }
    else
    {
        half = sign | ((f_abs_bits + float16_params.bias_diff) >> float16_params.frac_bits_diff);
    }
    return half;
}