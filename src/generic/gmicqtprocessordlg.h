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

#include <QThread>
#include <QString>
#include <QStringList>
#include <QProgressDialog>

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

Q_SIGNALS:

    void signalComplete(const QString& error);
    void signalProgressInfo(const QString& info);

private:

    void run() override;

private:

    GmicQtProcessor* m_proc = nullptr;
    QStringList      m_inputPaths;
    QString          m_command;
    QString          m_outputPath;
    QString          m_outputFormat;
};

// ----------------------------------------------------------------------------------

class GmicQtProcessorDlg : public QProgressDialog
{
    Q_OBJECT

public:

    explicit GmicQtProcessorDlg(QWidget* const parent = nullptr);
    ~GmicQtProcessorDlg() override;

    void setSettings(const QStringList& inputPaths,
                     const QString& command,
                     const QString& outputPath,
                     const QString& outputFormat);

public Q_SLOTS:

    void slotProgressInfo(const QString& info);
    void slotComplete(const QString& error);

private:

    void setMessage(const QString& txt);

private:

    class Private;
    Private* const d = nullptr;
};

} // namespace DigikamGenericGmicQtPlugin
