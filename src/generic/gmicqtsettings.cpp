/* ============================================================
 *
 * This file is a part of digiKam project
 * https://www.digikam.org
 *
 * Date        : 2025-04-21
 * Description : digiKam generic GmicQt plugin supporting layers mode.
 *
 * SPDX-FileCopyrightText: 2025 by Gilles Caulier <caulier dot gilles at gmail dot com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ============================================================ */

#include "gmicqtsettings.h"

namespace DigikamGenericGmicQtPlugin
{

void GmicQtSettings::readSettings()
{
    QSettings config;
    config.beginGroup(QLatin1String("GmicQt Generic Plugin"));
    selMode     = (Selection)config.value(QLatin1String("SelMode"),       (int)IMAGES);
    imageFormat = (ImageFormat)config.value(QLatin1String("ImageFormat"), (int)JPEG);
    config.endGroup();
}

void GmicQtSettings::writeSettings()
{
    QSettings config;
    config.beginGroup(QLatin1String("GmicQt Generic Plugin"));
    config.setValue(QLatin1String("SelMode"),     (int)selMode);
    config.setValue(QLatin1String("ImageFormat"), (int)imageFormat);
    config.endGroup();
}

QString GmicQtSettings::format() const
{
    if (imageFormat == JPEG)
    {
        return QLatin1String("JPEG");
    }

    return QLatin1String("PNG");
}

QMap<GmicQtSettings::ImageFormat, QString> GmicQtSettings::imageFormatNames()
{
    QMap<ImageFormat, QString> frms;

    frms[JPEG] = i18nc("Image format: JPEG", "Jpeg");
    frms[PNG]  = i18nc("Image format: PNG",  "Png");

    return frms;
}

} // namespace DigikamGenericGmicQtPlugin
