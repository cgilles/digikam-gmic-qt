/* ============================================================
 *
 * This file is a part of digiKam project
 * https://www.digikam.org
 *
 * Date        : 2019-11-28
 * Description : GmicQt Command Processor.
 *
 * SPDX-FileCopyrightText: 2019-2025 by Gilles Caulier <caulier dot gilles at gmail dot com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ============================================================ */

#pragma once

#include "gmicqtprocessor.h"

// digiKam includes

#include "digikam_debug.h"
#include "dimgloaderobserver.h"

// Local includes

#include "Common.h"
#include "FilterThread.h"
#include "Misc.h"
#include "Updater.h"
#include "gmicqtimageconverter.h"

using namespace GmicQt;
using namespace Digikam;

namespace DigikamGmicQtPluginCommon
{

class GmicQtProcessorObserver;

class Q_DECL_HIDDEN GmicQtProcessor::Private
{
public:

    Private()
        : gmicImages(new gmic_library::gmic_list<gmic_pixel_type>)
    {
    }

public:

    FilterThread*                             filterThread = nullptr;
    gmic_library::gmic_list<gmic_pixel_type>* gmicImages   = nullptr;
    GmicQtProcessorObserver*                  observer     = nullptr;

    QTimer                                    timer;
    QString                                   filterName;

    QString                                   command;
    bool                                      completed    = false;
    bool                                      cancel       = false;

    DImg                                      inImage;
    QStringList                               inFiles;
    gmic_library::gmic_list<gmic_pixel_type>  outImages;
};

// ---

class Q_DECL_HIDDEN GmicQtProcessorObserver : public DImgLoaderObserver
{
public:

    explicit GmicQtProcessorObserver(GmicQtProcessor::Private* const priv)
        : DImgLoaderObserver(),
          d                 (priv)
    {
    }

    ~GmicQtProcessorObserver() override = default;

    bool continueQuery() override
    {
        return (!d->cancel);
    }

private:

    GmicQtProcessor::Private* const d = nullptr;
};

} // namespace DigikamGmicQtPluginCommon
