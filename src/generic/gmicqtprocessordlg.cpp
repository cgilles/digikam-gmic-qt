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

#include "gmicqtprocessordlg.h"

// Qt includes

#include <QPushButton>
#include <QProgressBar>
#include <QEventLoop>

// digiKam includes

#include "dimg.h"
#include "digikam_debug.h"

namespace DigikamGenericGmicQtPlugin
{

GmicQtProcessorThread::GmicQtProcessorThread(QObject* const parent)
    : QThread(parent)
{
}

GmicQtProcessorThread::~GmicQtProcessorThread()
{
    wait();
}

void GmicQtProcessorThread::setSettings(const QStringList& inputPaths,
                                        const QString& command,
                                        const QString& outputPath,
                                        const QString& outputFormat)
{
    qCDebug(DIGIKAM_DPLUGIN_GENERIC_LOG) << "G'MIC command      :" << command;
    qCDebug(DIGIKAM_DPLUGIN_GENERIC_LOG) << "Images to Process  :" << inputPaths;
    qCDebug(DIGIKAM_DPLUGIN_GENERIC_LOG) << "Ouput image file   :" << outputPath;
    qCDebug(DIGIKAM_DPLUGIN_GENERIC_LOG) << "Ouput image format :" << outputFormat;

    m_inputPaths   = inputPaths;
    m_command      = command;
    m_outputPath   = outputPath;
    m_outputFormat = outputFormat;
}

void GmicQtProcessorThread::run()
{
    m_proc = new GmicQtProcessor();

    connect(m_proc, SIGNAL(signalProgressInfo(QString)),
            this, SIGNAL(signalProgressInfo(QString)));

    m_proc->setInputFiles(m_inputPaths);

    if (!m_proc->setProcessingCommand(m_command))
    {
        qCDebug(DIGIKAM_DPLUGIN_GENERIC_LOG) << "GmicGenericTool: cannot setup G'MIC filter!";
    }
    else
    {
        QEventLoop loop;

        connect(m_proc, SIGNAL(signalDone(QString)),
                &loop, SLOT(quit()));

        m_proc->startProcessingFiles();

        qCDebug(DIGIKAM_DPLUGIN_GENERIC_LOG) << "GmicGenericTool: started G'MIC filter...";

        loop.exec();

        bool b = m_proc->processingComplete();
        qCDebug(DIGIKAM_DPLUGIN_GENERIC_LOG) << "GmicGenericTool: G'MIC filter completed:" << b;

        if (b)
        {
            Q_EMIT signalProgressInfo(tr("Save data into\n%1").arg(m_outputPath));

            b = m_proc->outputImage().save(m_outputPath, m_outputFormat);
            qCDebug(DIGIKAM_DPLUGIN_GENERIC_LOG) << "GmicGenericTool: G'MIC save data completed:" << b;
        }
    }

    delete m_proc;

    Q_EMIT signalComplete();
}

// ----------------------------------------------------------------------------------

class Q_DECL_HIDDEN GmicQtProcessorDlg::Private
{
public:

    Private() = default;

public:

    GmicQtProcessorThread* thread = nullptr;
};

GmicQtProcessorDlg::GmicQtProcessorDlg(QWidget* const parent)
    : QProgressDialog(parent, Qt::FramelessWindowHint),
      d              (new Private)
{
    QString title = tr("Processing G'MIC filter. Please wait...");
    setLabelText(title);
    setCancelButton(nullptr);
    setMinimumDuration(0);
    setModal(true);
    setAutoClose(false);

    setMinimum(0);
    setMaximum(0);
    setValue(0);

    d->thread = new GmicQtProcessorThread(this);

    connect(d->thread, SIGNAL(signalComplete()),
            this, SLOT(slotComplete()));

    connect(d->thread, &GmicQtProcessorThread::signalProgressInfo,
            [=](const QString& info)
        {
            setLabelText(QString::fromUtf8("%1\n%2").arg(title).arg(info));
        }
    );
}

GmicQtProcessorDlg::~GmicQtProcessorDlg()
{
    delete d;
}

void GmicQtProcessorDlg::setSettings(const QStringList& inputPaths,
                                     const QString& command,
                                     const QString& outputPath,
                                     const QString& outputFormat)
{
    d->thread->setSettings(inputPaths, command, outputPath, outputFormat);
    d->thread->start();
}

void GmicQtProcessorDlg::slotComplete()
{
    qCDebug(DIGIKAM_DPLUGIN_GENERIC_LOG) << "Thread is complete";
    accept();
}

} // namespace DigikamGenericGmicQtPlugin

#include "moc_gmicqtprocessordlg.cpp"
