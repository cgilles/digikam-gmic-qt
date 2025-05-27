/* ============================================================
 *
 * This file is a part of digiKam project
 * https://www.digikam.org
 *
 * Date        : 2025-05-17
 * Description : a G'MIC settings container
 *
 * SPDX-FileCopyrightText: 2025 by Gilles Caulier <caulier dot gilles at gmail dot com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ============================================================ */

#include "gmicqtsettings.h"

namespace DigikamGenericGmicQtPlugin
{

void GmicQtSettings::readSettings(const QSettings& cnf)
{
/*
    selMode           = (Selection)group.readEntry("SelMode",
                        (int)IMAGES);
    addFileProperties = group.readEntry("AddCommentsAndTags",
                        false);
    imagesChangeProp  = group.readEntry("ImagesChangeProp",
                        false);
    removeMetadata    = group.readEntry("RemoveMetadata",
                        false);
    attLimitInMbytes  = group.readEntry("AttLimitInMbytes",
                        17);
    imageCompression  = group.readEntry("ImageCompression",
                        75);
    mailProgram       = (MailClient)group.readEntry("MailProgram",
                        (int)THUNDERBIRD);
    imageSize         = group.readEntry("ImageSize",
                        1024);
    imageFormat       = (ImageFormat)group.readEntry("ImageFormat",
                        (int)JPEG);
*/
}

void GmicQtSettings::writeSettings(QSettings& cnf)
{
/*
    group.writeEntry("SelMode",            (int)selMode);
    group.writeEntry("AddCommentsAndTags", addFileProperties);
    group.writeEntry("ImagesChangeProp",   imagesChangeProp);
    group.writeEntry("RemoveMetadata",     removeMetadata);
    group.writeEntry("AttLimitInMbytes",   attLimitInMbytes);
    group.writeEntry("ImageCompression",   imageCompression);
    group.writeEntry("MailProgram",        (int)mailProgram);
    group.writeEntry("ImageSize",          imageSize);
    group.writeEntry("ImageFormat",        (int)imageFormat);
*/
}

} // namespace DigikamGenericGmicQtPlugin
