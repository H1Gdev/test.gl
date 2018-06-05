//--------------------------------------------------------------------------------------
// File: util.h
// Desc:
//
// Author:      QUALCOMM
//
//               Copyright (c) 2017 QUALCOMM Technologies, Inc.
//                         All Rights Reserved.
//                      QUALCOMM Proprietary/GTDR
//--------------------------------------------------------------------------------------

#ifndef SDK_EXAMPLES_UTIL_H
#define SDK_EXAMPLES_UTIL_H

#include <string>
#include <vector>
#include <CL/cl.h>

/**
 * \brief yuv_image_t represents the "raw bytes" + width and height of YUV image with two planes.
 *        this encompasses e.g. NV12, TP10, P010.
 */
struct yuv_image_t
{
    uint32_t y_width;
    uint32_t y_height;
    std::vector<unsigned char> y_plane;
    std::vector<unsigned char> uv_plane;

    virtual ~yuv_image_t() = default;
};

struct nv12_image_t : public yuv_image_t {};

struct tp10_image_t : public yuv_image_t {};

struct p010_image_t : public yuv_image_t {};

/**
 * \brief Loads an 8-bit NV12 image from image data at filename
 *
 * @param filename
 * @return
 */
nv12_image_t load_nv12_image_data(const std::string &filename);

/**
 * \brief Saves 8-bit NV12 image to the given filename
 *
 * @param filename
 * @param image
 */
void save_nv12_image_data(const std::string &filename, const nv12_image_t &image);

/**
 * \brief Loads a TP10 image from image data at filename
 *
 * @param filename
 * @return
 */
tp10_image_t load_tp10_image_data(const std::string &filename);

/**
 * \brief Saves TP10 image to the given filename
 *
 * @param filename
 * @param image
 */
void save_tp10_image_data(const std::string &filename, const tp10_image_t &image);

/**
 * \brief Loads a p010 image from image data at filename
 *
 * @param filename
 * @return
 */
p010_image_t load_p010_image_data(const std::string &filename);

/**
 * \brief Saves p010 image to the given filename
 *
 * @param filename
 * @param image
 */
void save_p010_image_data(const std::string &filename, const p010_image_t &image);

// Returns smallest y such that y % r == 0 and y >= x
size_t work_units(size_t x, size_t r);

#endif //SDK_EXAMPLES_UTIL_H
