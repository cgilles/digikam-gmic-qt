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

#include "gmicqtprocessorthread.h"

// Qt includes

#include <QEventLoop>
#include <QImage>

// digiKam includes

#include "dimg.h"
#include "digikam_debug.h"
#include "dimgloaderobserver.h"

// Local includes

#include "gmicqtcommon.h"

namespace DigikamGenericGmicQtPlugin
{

class GmicQtProcessorThreadObserver;

class Q_DECL_HIDDEN GmicQtProcessorThread::Private
{

public:

    Private() = default;

public:

    bool                            cancel      = false;
    GmicQtProcessor*                proc        = nullptr;
    GmicQtProcessorThreadObserver*  observer    = nullptr;
    QStringList                     inputPaths;
    QString                         command;
    QString                         outputPath;
    QString                         outputFormat;
};

// ---

class Q_DECL_HIDDEN GmicQtProcessorThreadObserver : public DImgLoaderObserver
{
public:

    explicit GmicQtProcessorThreadObserver(GmicQtProcessorThread::Private* const priv)
        : DImgLoaderObserver(),
          d                 (priv)
    {
    }

    ~GmicQtProcessorThreadObserver() override = default;

    bool continueQuery() override
    {
        return (!d->cancel);
    }

private:

    GmicQtProcessorThread::Private* const d = nullptr;
};

// ---

GmicQtProcessorThread::GmicQtProcessorThread(QObject* const parent)
    : QThread(parent),
      d      (new Private)
{
    d->observer = new GmicQtProcessorThreadObserver(d);
}

GmicQtProcessorThread::~GmicQtProcessorThread()
{
    wait();

    delete d->observer;
    delete d;
}

void GmicQtProcessorThread::cancel()
{
    d->proc->cancel();
    d->cancel = true;
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

    d->inputPaths   = inputPaths;
    d->command      = command;
    d->outputPath   = outputPath;
    d->outputFormat = outputFormat;
}

void GmicQtProcessorThread::run()
{
    QString error;

    d->proc = new GmicQtProcessor();

    connect(d->proc, SIGNAL(signalProgressInfo(QString)),
            this, SIGNAL(signalProgressInfo(QString)));

    d->proc->setInputFiles(d->inputPaths);

    if (!d->proc->setProcessingCommand(d->command))
    {
        error = tr("Cannot setup G'MIC filter!");
    }
    else
    {
        QEventLoop loop;

        connect(d->proc, SIGNAL(signalDone(QString)),
                &loop, SLOT(quit()));

        d->proc->startProcessingFiles();

        qCDebug(DIGIKAM_DPLUGIN_GENERIC_LOG) << "GmicGenericTool: started G'MIC filter...";

        loop.exec();

        if (d->proc->processingComplete())
        {
            qCDebug(DIGIKAM_DPLUGIN_GENERIC_LOG) << "GmicGenericTool: G'MIC filter completed";

            Q_EMIT signalProgressInfo(tr("Save data into %1").arg(d->outputPath));

            if (d->proc->outputImage().save(d->outputPath, d->outputFormat, d->observer))
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

    delete d->proc;

    Q_EMIT signalComplete(error);
}

} // namespace DigikamGenericGmicQtPlugin

#include "moc_gmicqtprocessorthread.cpp"
