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

#pragma once

// Qt includes

#include <QtGlobal>
#include <QList>
#include <QString>
#include <QStringList>
#include <QUrl>
#include <QMap>

class KConfigGroup;

namespace DigikamGenericSendByMailPlugin
{

class MailSettings
{

public:

    /**
     * Images selection mode
     */
    enum Selection
    {
        IMAGES = 0,
        ALBUMS
    };

    enum MailClient
    {
        BALSA = 0,
        CLAWSMAIL,
        EVOLUTION,
        KMAIL,
        NETSCAPE,       ///< Messenger (https://en.wikipedia.org/wiki/Netscape_Messenger_9)
        OUTLOOK,
        SYLPHEED,
        THUNDERBIRD
    };

    enum ImageFormat
    {
        JPEG = 0,
        PNG
    };

public:

    MailSettings()  = default;
    ~MailSettings() = default;

    /**
     * Read and write settings in config file between sessions.
     */
    void  readSettings(const KConfigGroup& group);
    void  writeSettings(KConfigGroup& group);

    QString format()           const;

    /**
     * Return the attachment limit in bytes
     */
    qint64  attachementLimit() const;

    void setMailUrl(const QUrl& orgUrl, const QUrl& emailUrl);
    QUrl mailUrl(const QUrl& orgUrl) const;

    /**
     * Helper methods to fill combobox from GUI.
     */
    static QMap<MailClient,  QString> mailClientNames();
    static QMap<ImageFormat, QString> imageFormatNames();

public:

    Selection                 selMode           = IMAGES;    ///< Items selection mode

    QList<QUrl>               inputImages;      ///< Selected items to send.

    bool                      addFileProperties = false;
    bool                      imagesChangeProp  = false;

    bool                      removeMetadata    = false;

    int                       imageCompression  = 75;

    qint64                    attLimitInMbytes  = 17;

    QString                   tempPath;

    MailClient                mailProgram       = THUNDERBIRD;

    int                       imageSize         = 1024;

    ImageFormat               imageFormat       = JPEG;

    QMap<QUrl, QUrl>          itemsList;        ///< Map of original item and attached item (can be resized).

    QMap<MailClient, QString> binPaths;         ///< Map of paths for all mail clients.
};

} // namespace DigikamGenericSendByMailPlugin
