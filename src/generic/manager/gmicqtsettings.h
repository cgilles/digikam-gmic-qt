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

#pragma once

// Qt includes

#include <QtGlobal>
#include <QList>
#include <QSettings>
#include <QString>
#include <QStringList>
#include <QUrl>
#include <QMap>

namespace DigikamGenericGmicQtPlugin
{

class GmicQtSettings
{
public:

    GmicQtSettings()  = default;
    ~GmicQtSettings() = default;

    /**
     * Read and write settings in config file between sessions.
     */
    void  readSettings(const QSettings& cnf);
    void  writeSettings(QSettings& cnf);

public:

    QList<QUrl> inputImages;      ///< Selected items to process.
    QUrl        targetUrl;
    QString     templateFName;
    int         format;
};

} // namespace DigikamGenericGmicQtPlugin
