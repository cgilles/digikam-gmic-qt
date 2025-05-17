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
#include <QLabel>
#include <QImage>
#include <QByteArray>
#include <QBuffer>

// digiKam includes

#include "dimg.h"
#include "digikam_debug.h"
#include "ditemtooltip.h"

// Local includes

#include "gmicqtcommon.h"

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
    QString error;

    m_proc = new GmicQtProcessor();

    connect(m_proc, SIGNAL(signalProgressInfo(QString)),
            this, SIGNAL(signalProgressInfo(QString)));

    m_proc->setInputFiles(m_inputPaths);

    if (!m_proc->setProcessingCommand(m_command))
    {
        error = tr("Cannot setup G'MIC filter!");
    }
    else
    {
        QEventLoop loop;

        connect(m_proc, SIGNAL(signalDone(QString)),
                &loop, SLOT(quit()));

        m_proc->startProcessingFiles();

        qCDebug(DIGIKAM_DPLUGIN_GENERIC_LOG) << "GmicGenericTool: started G'MIC filter...";

        loop.exec();

        if (m_proc->processingComplete())
        {
            qCDebug(DIGIKAM_DPLUGIN_GENERIC_LOG) << "GmicGenericTool: G'MIC filter completed";

            Q_EMIT signalProgressInfo(tr("Save data into<br>%1").arg(m_outputPath));

            if (m_proc->outputImage().save(m_outputPath, m_outputFormat))
            {
                qCDebug(DIGIKAM_DPLUGIN_GENERIC_LOG) << "GmicGenericTool: G'MIC save data completed";
            }
            else
            {
                error = tr("Cannot save G'MIC filter data!");
            }
        }
        else
        {
            error = tr("Cannot process G'MIC filter!");
        }
    }

    delete m_proc;

    Q_EMIT signalComplete(error);
}

// ----------------------------------------------------------------------------------

class Q_DECL_HIDDEN GmicQtProcessorDlg::Private
{
public:

    Private() = default;

public:

    GmicQtProcessorThread* thread   = nullptr;
    QPushButton*           closeBtn = nullptr;
    QString                title    = tr("Processing G'MIC filter. Please wait...");
};

GmicQtProcessorDlg::GmicQtProcessorDlg(QWidget* const parent)
    : QProgressDialog(parent, Qt::FramelessWindowHint),
      d              (new Private)
{
    setMessage(d->title);

    d->closeBtn = new QPushButton(tr("Close"));
    setCancelButton(d->closeBtn);
    d->closeBtn->setVisible(false);
    setMinimumDuration(0);
    setModal(true);
    setAutoClose(false);

    setMinimum(0);
    setMaximum(0);
    setValue(0);

    d->thread = new GmicQtProcessorThread(this);

    connect(d->thread, SIGNAL(signalComplete(QString)),
            this, SLOT(slotComplete(QString)));

    connect(d->thread, SIGNAL(signalProgressInfo(QString)),
            this, SLOT(slotProgressInfo(QString)));
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

void GmicQtProcessorDlg::slotProgressInfo(const QString& info)
{
    setMessage(QString::fromUtf8("%1<br>%2").arg(d->title).arg(info));
}

void GmicQtProcessorDlg::slotComplete(const QString& error)
{
    d->closeBtn->setVisible(true);
    setMaximum(1);
    setValue(1);

    if (error.isEmpty())
    {
        setMessage(tr("G'MIC filter is done"));
    }
    else
    {
        setMessage(tr("Error while processing G'MIC filter:<br>%1").arg(error));
    }
}

void GmicQtProcessorDlg::setMessage(const QString& txt)
{
    QString rtxt;
    DToolTipStyleSheet cnt;

    QImage img = s_gmicQtPluginIcon().pixmap(48, 48).toImage();
    QByteArray byteArray;
    QBuffer    buffer(&byteArray);
    img.save(&buffer, "PNG");

    rtxt += QLatin1String("<qt><table>");
    rtxt += cnt.cellBeg +
            QString::fromLatin1("<img src=\"data:image/png;base64,%1\">").arg(QString::fromLatin1(byteArray.toBase64().data())) +
            cnt.cellMid +
            txt         +
            cnt.cellEnd;
    rtxt += QLatin1String("</table></center></qt>");

    setLabelText(rtxt);
}

} // namespace DigikamGenericGmicQtPlugin

#include "moc_gmicqtprocessordlg.cpp"
