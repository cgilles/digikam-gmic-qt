/* ============================================================
 *
 * This file is a part of digiKam project
 * https://www.digikam.org
 *
 * Date        : 2019-11-28
 * Description : digiKam Batch Queue Manager plugin for GmicQt.
 *
 * SPDX-FileCopyrightText: 2019-2025 by Gilles Caulier <caulier dot gilles at gmail dot com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ============================================================ */

// Qt includes

#include <QObject>
#include <QApplication>
#include <QDataStream>
#include <QTextStream>
#include <QStringList>
#include <QList>
#include <QUrl>

// digiKam includes

#include "digikam_debug.h"
#include "dinfointerface.h"
#include "previewloadthread.h"

// Local includes

#include "Common.h"
#include "Host/GmicQtHost.h"
#include "gmicqtimageconverter.h"

namespace DigikamGenericGmicQtPlugin
{

extern DInfoInterface* s_infoIface;
extern QUrl            s_currentAlbumUrl;
extern QList<QUrl>     s_urlList;

} // namespace DigikamBqmGmicQtPlugin

using namespace DigikamGenericGmicQtPlugin;
using namespace DigikamGmicQtPluginCommon;

/**
 * GMic-Qt plugin functions
 * See documentation from GmicQtHost.h for details.
 */

namespace GmicQtHost
{

const QString ApplicationName          = QLatin1String("digiKam");
const char* const ApplicationShortname = GMIC_QT_XSTRINGIFY(GMIC_HOST);
const bool DarkThemeIsDefault          = false;

void getLayersExtent(int* width,
                     int* height,
                     GmicQt::InputMode mode)
{
    qCDebug(DIGIKAM_DPLUGIN_GENERIC_LOG) << "Calling GmicQt getLayersExtent(): InputMode="
                                         << (int)mode;

    QList<QUrl> list = s_infoIface->currentSelectedItems();

    if (!list.isEmpty())
    {
        DImg img = PreviewLoadThread::loadFastSynchronously(list.first().toLocalFile(), 1024);
        *width   = img.width();
        *height  = img.height();
    }
    else
    {
        *width  = 0;
        *height = 0;
    }

    qCDebug(DIGIKAM_DPLUGIN_GENERIC_LOG) << "W=" << *width;
    qCDebug(DIGIKAM_DPLUGIN_GENERIC_LOG) << "H=" << *height;
}

void getCroppedImages(cimg_library::CImgList<gmic_pixel_type>& images,
                      cimg_library::CImgList<char>& imageNames,
                      double x,
                      double y,
                      double width,
                      double height,
                      GmicQt::InputMode mode)
{
    qCDebug(DIGIKAM_DPLUGIN_GENERIC_LOG) << "Calling GmicQt getCroppedImages()";

    s_urlList = s_infoIface->currentSelectedItems();

    if (s_urlList.isEmpty())
    {
        s_urlList = s_infoIface->currentAlbumItems();
    }

    if ((mode == GmicQt::InputMode::NoInput) || s_urlList.isEmpty())
    {
        images.assign();
        imageNames.assign();

        return;
    }

    images.assign(s_urlList.size());
    imageNames.assign(s_urlList.size());
    s_currentAlbumUrl = s_urlList[0];

    for (int i = 0 ; i < s_urlList.size() ; ++i)
    {
        DImg input_image       = PreviewLoadThread::loadFastSynchronously(s_urlList[i].toLocalFile(), 1024);
        const bool entireImage = (
                                  (x      < 0.0) &&
                                  (y      < 0.0) &&
                                  (width  < 0.0) &&
                                  (height < 0.0)
                                 );

        if (entireImage)
        {
            x      = 0.0;
            y      = 0.0;
            width  = 1.0;
            height = 1.0;
        }

        QString name  = QString::fromUtf8("pos(0,0),name(%1)").arg(QFileInfo(s_urlList[i].toLocalFile()).baseName());
        QByteArray ba = name.toUtf8();
        gmic_image<char>::string(ba.constData()).move_to(imageNames[i]);

        const int ix = static_cast<int>(entireImage ? 0
                                                    : std::floor(x * input_image.width()));

        const int iy = static_cast<int>(entireImage ? 0
                                                    : std::floor(y * input_image.height()));

        const int iw = entireImage ? input_image.width()
                                   : std::min(
                                              static_cast<int>(input_image.width()  - ix),
                                              static_cast<int>(1 + std::ceil(width  * input_image.width()))
                                             );

        const int ih = entireImage ? input_image.height()
                                   : std::min(
                                              static_cast<int>(input_image.height() - iy),
                                              static_cast<int>(1 + std::ceil(height * input_image.height()))
                                             );

        GMicQtImageConverter::convertDImgtoCImg(input_image.copy(ix, iy, iw, ih), images[i]);
    }
}

void applyColorProfile(cimg_library::CImg<gmic_pixel_type>& images) // cppcheck-suppress constParameterReference

{
    qCDebug(DIGIKAM_DPLUGIN_GENERIC_LOG) << "Calling GmicQt applyColorProfile()";

    Q_UNUSED(images);
}

void showMessage(const char* message)
{
    qCDebug(DIGIKAM_DPLUGIN_GENERIC_LOG) << "Calling GmicQt showMessage()";
    qCDebug(DIGIKAM_DPLUGIN_GENERIC_LOG) << "G'MIC-Qt:" << message;
}

void outputImages(cimg_library::CImgList<gmic_pixel_type>& images,  // cppcheck-suppress constParameterReference
                  const cimg_library::CImgList<char>& imageNames,
                  GmicQt::OutputMode mode)
{
    Q_UNUSED(images);
    Q_UNUSED(imageNames);
    Q_UNUSED(mode);
}

} // namespace GmicQtHost
