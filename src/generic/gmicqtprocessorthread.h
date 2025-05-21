/* ============================================================
 *
 * This file is a part of digiKam project
 * https://www.digikam.org
 *
 * Date        : 2025-05-17
 * Description : a G'MIC processor thread
 *
 * SPDX-FileCopyrightText: 2025 by Gilles Caulier <caulier dot gilles at gmail dot com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ============================================================ */

#pragma once

// Qt includes

#include <QThread>
#include <QUrl>
#include <QString>
#include <QStringList>

// Local includes

#include "gmicqtprocessor.h"

using namespace DigikamGmicQtPluginCommon;

namespace DigikamGenericGmicQtPlugin
{

class GmicQtProcessorThread : public QThread
{
    Q_OBJECT

public:

    explicit GmicQtProcessorThread(QObject* const parent);
    ~GmicQtProcessorThread() override;

    void setSettings(const QStringList& inputPaths,
                     const QString& command,
                     const QString& outputPath,
                     const QString& outputFormat);

    void cancel();

Q_SIGNALS:

    void signalComplete(const QString& error);
    void signalProgressInfo(const QString& info);
    void signalUpdateHostApp(const QUrl& url);

private:

    void run() override;

public:

    // Declared as public due to GmicQtProcessorThreadObserver class.
    class Private;

private:

    Private* const d = nullptr;
};

} // namespace DigikamGenericGmicQtPlugin
