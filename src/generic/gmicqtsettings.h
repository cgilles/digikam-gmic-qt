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

#pragma once

// Qt includes

#include <QtGlobal>
#include <QList>
#include <QString>
#include <QStringList>
#include <QUrl>
#include <QMap>

namespace DigikamGenericGmicQtPlugin
{

class GmicQtSettings
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

    enum ImageFormat
    {
        JPEG = 0,
        PNG
    };

public:

    GmicQtSettings()  = default;
    ~GmicQtSettings() = default;

    /**
     * Read and write settings in config file between sessions.
     */
    void  readSettings(const KConfigGroup& group);
    void  writeSettings(KConfigGroup& group);

    QString format()           const;

    /**
     * Helper methods to fill combobox from GUI.
     */
    static QMap<ImageFormat, QString> imageFormatNames();

public:

    Selection                 selMode           = IMAGES;    ///< Items selection mode

    QList<QUrl>               inputImages;      ///< Selected items to send.

    ImageFormat               imageFormat       = JPEG;

    QMap<QUrl, QUrl>          itemsList;        ///< Map of original item and attached item (can be resized).
};

} // namespace DigikamGenericGmicQtPlugin
