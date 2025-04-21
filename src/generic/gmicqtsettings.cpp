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

void GmicQtSettings::readSettings(const KConfigGroup& group)
{
    selMode           = (Selection)group.readEntry("SelMode",
                        (int)IMAGES);
    imageFormat       = (ImageFormat)group.readEntry("ImageFormat",
                        (int)JPEG);
}

void GmicQtSettings::writeSettings(KConfigGroup& group)
{
    group.writeEntry("SelMode",            (int)selMode);
    group.writeEntry("ImageFormat",        (int)imageFormat);
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
