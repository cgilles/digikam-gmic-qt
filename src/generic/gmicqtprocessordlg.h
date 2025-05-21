/* ============================================================
 *
 * This file is a part of digiKam project
 * https://www.digikam.org
 *
 * Date        : 2025-05-17
 * Description : a G'MIC processor progress dialog
 *
 * SPDX-FileCopyrightText: 2025 by Gilles Caulier <caulier dot gilles at gmail dot com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ============================================================ */

#pragma once

// Qt includes

#include <QString>
#include <QStringList>
#include <QUrl>
#include <QDialog>
#include <QCloseEvent>

// digiKam includes

#include "dplugin.h"

using namespace Digikam;

namespace DigikamGenericGmicQtPlugin
{

class GmicQtProcessorDlg : public QDialog
{
    Q_OBJECT

public:

    explicit GmicQtProcessorDlg(DPlugin* const tool, QWidget* const parent = nullptr);
    ~GmicQtProcessorDlg()           override;

    void setSettings(const QStringList& inputPaths,
                     const QString& command,
                     const QString& outputPath,
                     const QString& outputFormat);

Q_SIGNALS:

    void signalUpdateHostApp(const QUrl& url);

private Q_SLOTS:

    void slotProgressInfo(const QString& info);
    void slotComplete(const QString& error);
    void slotCancel();

private:

    void closeEvent(QCloseEvent*)   override;

private:

    class Private;
    Private* const d = nullptr;
};

} // namespace DigikamGenericGmicQtPlugin
