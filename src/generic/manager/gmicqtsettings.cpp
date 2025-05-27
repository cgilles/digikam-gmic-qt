/* ============================================================
 *
 * This file is a part of digiKam project
 * https://www.digikam.org
 *
 * Date        : 2007-11-07
 * Description : mail settings container.
 *
 * SPDX-FileCopyrightText: 2007-2025 by Gilles Caulier <caulier dot gilles at gmail dot com>
 * SPDX-FileCopyrightText: 2010      by Andi Clemens <andi dot clemens at googlemail dot com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ============================================================ */

#include "mailsettings.h"

// KDE includes

#include <klocalizedstring.h>
#include <kconfiggroup.h>

namespace DigikamGenericSendByMailPlugin
{

void MailSettings::readSettings(const KConfigGroup& group)
{
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
}

void MailSettings::writeSettings(KConfigGroup& group)
{
    group.writeEntry("SelMode",            (int)selMode);
    group.writeEntry("AddCommentsAndTags", addFileProperties);
    group.writeEntry("ImagesChangeProp",   imagesChangeProp);
    group.writeEntry("RemoveMetadata",     removeMetadata);
    group.writeEntry("AttLimitInMbytes",   attLimitInMbytes);
    group.writeEntry("ImageCompression",   imageCompression);
    group.writeEntry("MailProgram",        (int)mailProgram);
    group.writeEntry("ImageSize",          imageSize);
    group.writeEntry("ImageFormat",        (int)imageFormat);
}

QString MailSettings::format() const
{
    if (imageFormat == JPEG)
    {
        return QLatin1String("JPEG");
    }

    return QLatin1String("PNG");
}

void MailSettings::setMailUrl(const QUrl& orgUrl, const QUrl& emailUrl)
{
    itemsList.insert(orgUrl, emailUrl);
}

QUrl MailSettings::mailUrl(const QUrl& orgUrl) const
{
    if (itemsList.contains(orgUrl))
    {
        return itemsList.find(orgUrl).value();
    }

    return QUrl();
}

qint64 MailSettings::attachementLimit() const
{
    qint64 val = attLimitInMbytes * 1024 * 1024;

    return val;
}

QMap<MailSettings::MailClient, QString> MailSettings::mailClientNames()
{
    QMap<MailClient, QString> clients;

    clients[BALSA]         = i18nc("Mail client: BALSA",         "Balsa");
    clients[CLAWSMAIL]     = i18nc("Mail client: CLAWSMAIL",     "Clawsmail");
    clients[EVOLUTION]     = i18nc("Mail client: EVOLUTION",     "Evolution");
    clients[KMAIL]         = i18nc("Mail client: KMAIL",         "Kmail");
    clients[NETSCAPE]      = i18nc("Mail client: NETSCAPE",      "Netscape Messenger");
    clients[OUTLOOK]       = i18nc("Mail client: OUTLOOK",       "Outlook");
    clients[SYLPHEED]      = i18nc("Mail client: SYLPHEED",      "Sylpheed");
    clients[THUNDERBIRD]   = i18nc("Mail client: THUNDERBIRD",   "Thunderbird");

    return clients;
}

QMap<MailSettings::ImageFormat, QString> MailSettings::imageFormatNames()
{
    QMap<ImageFormat, QString> frms;

    frms[JPEG] = i18nc("Image format: JPEG", "Jpeg");
    frms[PNG]  = i18nc("Image format: PNG",  "Png");

    return frms;
}

} // namespace DigikamGenericSendByMailPlugin
