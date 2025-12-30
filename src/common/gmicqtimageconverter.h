/* ============================================================
 *
 * This file is a part of digiKam project
 * https://www.digikam.org
 *
 * Date        : 2019-11-28
 * Description : common implementations for the digiKam plugin for GmicQt.
 *
 * SPDX-FileCopyrightText: 2019-2026 by Gilles Caulier <caulier dot gilles at gmail dot com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ============================================================ */

#pragma once

// digiKam includes

#include "dimg.h"

// Local includes

#include "gmic.h"

using namespace Digikam;

namespace DigikamGmicQtPluginCommon
{

class GMicQtImageConverter
{

public:

    /**
     * @brief Method to convert CImg to Digikam::DImg image data container.
     * @param in the input CImg image.
     * @param out the output DImg image.
     * @param sixteenBit the flag to convert in 16 bits color depth.
     */
    static void convertCImgtoDImg(const cimg_library::CImg<float>& in,
                                  DImg& out,
                                  bool sixteenBit);

    /**
     * @brief Method to convert Digikam::DImg to CImg image data container.
     * @param in the input DImg image.
     * @param out the output CImg image.
     */
    static void convertDImgtoCImg(const DImg& in,
                                  cimg_library::CImg<float>& out);

private:

    /**
     * @brief Helper method to bound float value from a CImg pixel to DImg unsigned char pixel for 8 bits color depth.
     */
    static unsigned char  float2ucharBounded(const float& in);

    /**
     * @brief Helper method to bound float value from a CImg pixel to DImg unsigned short pixel for 16 bits color depth.
     */
    static unsigned short float2ushortBounded(const float& in);

private:

    // Disable
    GMicQtImageConverter()  = delete;
    ~GMicQtImageConverter() = delete;
};

} // namespace DigikamGmicQtPluginCommon
