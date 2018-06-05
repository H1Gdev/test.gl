//--------------------------------------------------------------------------------------
// File: util.cpp
// Desc:
//
// Author:      QUALCOMM
//
//               Copyright (c) 2017 QUALCOMM Technologies, Inc.
//                         All Rights Reserved.
//                      QUALCOMM Proprietary/GTDR
//--------------------------------------------------------------------------------------

#include "util/util.h"

#include "CL/cl.h"

#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <CL/cl_ext_qcom.h>

/************************
 * Forward declarations *
 ************************/

/**
 * \brief Writes a given value to the output stream in little-endian byte order
 *
 * @param UIntType - Template param. For correctness, must be an unsigned integral type.
 * @param out - The stream to write to.
 * @param val - The value to write.
 */
template <typename UIntType>
static void write_le(std::ostream &out, UIntType val);

/**
 * \brief Reads a value from a little-endian byte order input stream.
 *
 * @param UIntType - For correctness, must be an unsigned integral type
 * @param in - The stream to read from.
 * @return The value read.
 */
template <typename UIntType>
static UIntType read_le(std::istream &in);

template <typename UIntType>
static void write_le(std::ostream &out, UIntType val)
{
    unsigned char byte = 0;
    for (uint32_t i = 0; i < sizeof(UIntType); ++i)
    {
        byte = (val & (0xFF << (i * 8))) >> (i * 8);
        out.put(*reinterpret_cast<char *>(&byte));
    }
}

template <typename UIntType>
static UIntType read_le(std::istream &in)
{
    UIntType val = 0;
    unsigned char byte = 0;
    for (uint32_t i = 0; i < sizeof(UIntType); ++i)
    {
        in.get(*reinterpret_cast<char *>(&byte));
        val |= byte << (i * 8);
    }
    return val;
}

static void read_and_check_header(std::istream &in, uint32_t &width, uint32_t &height, uint32_t desired_data_type,
                                  uint32_t desired_order, const std::string &data_type_name,
                                  const std::string &order_name)
{
    width              = read_le<uint32_t>(in);
    height             = read_le<uint32_t>(in);
    uint32_t data_type = read_le<uint32_t>(in);
    uint32_t order     = read_le<uint32_t>(in);
    if (order != desired_order || data_type != desired_data_type)
    {
        std::cerr << "Expected " << order_name << " and " << data_type_name << " channel order and data types\n";
        std::exit(EXIT_FAILURE);
    }
}

// Macros are great for stringification
#define GET_HEADER(strm, w, h, desired_dt, desired_ord) \
    read_and_check_header(strm, w, h, desired_dt, desired_ord, #desired_dt, #desired_ord)

nv12_image_t load_nv12_image_data(const std::string &filename)
{
    std::ifstream fin(filename, std::ios::binary);
    if (!fin)
    {
        std::cerr << "Can't open " << filename << " for reading\n";
        std::exit(EXIT_FAILURE);
    }

    nv12_image_t result;
    GET_HEADER(fin, result.y_width, result.y_height, CL_UNORM_INT8, CL_QCOM_NV12);

    const size_t y_plane_len = result.y_width * result.y_height;
    result.y_plane.resize(y_plane_len);
    fin.read(reinterpret_cast<char *>(result.y_plane.data()), y_plane_len);

    result.uv_plane.resize(y_plane_len / 2);
    fin.read(reinterpret_cast<char *>(result.uv_plane.data()), y_plane_len / 2);

    return result;
}

tp10_image_t load_tp10_image_data(const std::string &filename)
{
    std::ifstream fin(filename, std::ios::binary);
    if (!fin)
    {
        std::cerr << "Can't open " << filename << " for reading\n";
        std::exit(EXIT_FAILURE);
    }

    tp10_image_t result;
    GET_HEADER(fin, result.y_width, result.y_height, CL_QCOM_UNORM_INT10, CL_QCOM_TP10);

    const size_t y_plane_len = result.y_width * result.y_height / 3 * 4;
    result.y_plane.resize(y_plane_len);
    for (size_t i = 0; i < result.y_width * result.y_height / 3; ++i)
    {
        const uint32_t packed_vals = read_le<uint32_t>(fin);
        std::memcpy(result.y_plane.data() + i * 4, &packed_vals, sizeof(packed_vals));
    }

    result.uv_plane.resize(y_plane_len / 2);
    for (size_t i = 0; i < result.y_width * result.y_height / 6; ++i)
    {
        const uint32_t packed_vals = read_le<uint32_t>(fin);
        std::memcpy(result.uv_plane.data() + i * 4, &packed_vals, sizeof(packed_vals));
    }
    return result;
}

p010_image_t load_p010_image_data(const std::string &filename)
{
    std::ifstream fin(filename, std::ios::binary);
    if (!fin)
    {
        std::cerr << "Can't open " << filename << " for reading\n";
        std::exit(EXIT_FAILURE);
    }

    p010_image_t result;
    GET_HEADER(fin, result.y_width, result.y_height, CL_QCOM_UNORM_INT10, CL_QCOM_P010);

    const size_t y_plane_len = result.y_width * result.y_height * 2;
    result.y_plane.resize(y_plane_len);
    for (size_t i = 0; i < result.y_width * result.y_height; ++i)
    {
        const uint16_t val = read_le<uint16_t>(fin);
        std::memcpy(result.y_plane.data() + i * 2, &val, sizeof(val));
    }

    result.uv_plane.resize(y_plane_len / 2);
    for (size_t i = 0; i < result.y_width * result.y_height / 4; ++i)
    {
        const uint16_t u_val = read_le<uint16_t>(fin);
        const uint16_t v_val = read_le<uint16_t>(fin);
        std::memcpy(result.uv_plane.data() + i * 4,     &u_val, sizeof(u_val));
        std::memcpy(result.uv_plane.data() + i * 4 + 2, &v_val, sizeof(v_val));
    }

    return result;
}

static void save_yuv_file_internal(const std::string &filename, const yuv_image_t &image, uint32_t data_type, uint32_t order,
                                   uint32_t channel_bytes)
{
    std::ofstream fout(filename, std::ios::binary);
    if (!fout)
    {
        std::cerr << "Can't open " << filename << " for writing.\n";
        std::exit(EXIT_FAILURE);
    }

    write_le<uint32_t>(fout, image.y_width);
    write_le<uint32_t>(fout, image.y_height);
    write_le<uint32_t>(fout, data_type);
    write_le<uint32_t>(fout, order);

    for (size_t i = 0; i < image.y_plane.size() / channel_bytes; ++i)
    {
        switch (channel_bytes)
        {
            case 1:
            {
                write_le<uint8_t>(fout, image.y_plane[i]);
                break;
            }
            case 2:
            {
                const uint16_t val = *reinterpret_cast<const uint16_t *>(image.y_plane.data() + i * channel_bytes);
                write_le<uint16_t>(fout, val);
                break;
            }
            case 4:
            {
                const uint32_t val = *reinterpret_cast<const uint32_t *>(image.y_plane.data() + i * channel_bytes);
                write_le<uint32_t>(fout, val);
                break;
            }
            default:
            {
                std::cerr << "Error, can't write " << channel_bytes << " bytes at a time.\n";
                std::exit(EXIT_FAILURE);
            }
        }
    }

    for (size_t i = 0; i < image.uv_plane.size() / channel_bytes; ++i)
    {
        switch (channel_bytes)
        {
            case 1:
            {
                write_le<uint8_t>(fout, image.uv_plane[i]);
                break;
            }
            case 2:
            {
                const uint16_t val = *reinterpret_cast<const uint16_t *>(image.uv_plane.data() + i * channel_bytes);
                write_le<uint16_t>(fout, val);
                break;
            }
            case 4:
            {
                const uint32_t val = *reinterpret_cast<const uint32_t *>(image.uv_plane.data() + i * channel_bytes);
                write_le<uint32_t>(fout, val);
                break;
            }
            default:
            {
                std::cerr << "Error, can't write " << channel_bytes << " bytes at a time.\n";
                std::exit(EXIT_FAILURE);
            }
        }
    }
}

void save_nv12_image_data(const std::string &filename, const nv12_image_t &image)
{
    save_yuv_file_internal(filename, image, CL_UNORM_INT8, CL_QCOM_NV12, 1);
}

void save_tp10_image_data(const std::string &filename, const tp10_image_t &image)
{
    save_yuv_file_internal(filename, image, CL_QCOM_UNORM_INT10, CL_QCOM_TP10, 4);
}

void save_p010_image_data(const std::string &filename, const p010_image_t &image)
{
    save_yuv_file_internal(filename, image, CL_QCOM_UNORM_INT10, CL_QCOM_P010, 2);
}

// Returns smallest y such that y % r == 0 and y >= x
size_t work_units(size_t x, size_t r)
{
    return (x + r - 1) / r;
}
