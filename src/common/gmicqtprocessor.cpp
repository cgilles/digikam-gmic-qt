/* ============================================================
 *
 * This file is a part of digiKam project
 * https://www.digikam.org
 *
 * Date        : 2019-11-28
 * Description : GmicQt Command Processor.
 *
 * SPDX-FileCopyrightText: 2019-2025 by Gilles Caulier <caulier dot gilles at gmail dot com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ============================================================ */

#include "gmicqtprocessor.h"

// digiKam includes

#include "digikam_debug.h"
#include "ditemtooltip.h"

// Local includes

#include "GmicQt.h"
#include "Common.h"
#include "FilterThread.h"
#include "GmicStdlib.h"
#include "Misc.h"
#include "Updater.h"
#include "gmicqtimageconverter.h"

namespace gmic_library
{
template <typename T> struct gmic_list;
}

using namespace GmicQt;

namespace DigikamGmicQtPluginCommon
{

class Q_DECL_HIDDEN GmicQtProcessor::Private
{
public:

    Private()
        : gmicImages(new gmic_library::gmic_list<gmic_pixel_type>)
    {
    }

public:

    FilterThread*                   filterThread = nullptr;
    gmic_library::gmic_list<float>* gmicImages   = nullptr;

    QTimer                          timer;
    QString                         filterName;

    QString                         command;
    bool                            completed    = false;
    bool                            cancel       = false;

    DImg                            inImage;
    QStringList                     inFiles;
    DImg                            outImage;
};

GmicQtProcessor::GmicQtProcessor(QObject* const parent)
    : QObject(parent),
      d      (new Private)
{
    GmicStdLib::Array = Updater::getInstance()->buildFullStdlib();
    d->gmicImages->assign();
}

GmicQtProcessor::~GmicQtProcessor()
{
    delete d->gmicImages;
    delete d;
}

void GmicQtProcessor::setInputImage(const DImg& inImage)
{
    d->inImage = inImage;
}

void GmicQtProcessor::setInputFiles(const QStringList& inFiles)
{
    d->inFiles = inFiles;
}

bool GmicQtProcessor::setProcessingCommand(const QString& command)
{
    if (command.isEmpty())
    {
        qCWarning(DIGIKAM_DPLUGIN_LOG) << "The G'MIC command is empty.";

        return false;
    }
    else
    {
        d->command    = command;
        d->filterName = QString::fromLatin1("Custom command (%1)").arg(elided(d->command, 35));
    }

    return true;
}

void GmicQtProcessor::startProcessingImage()
{
    gmic_list<char> imageNames;

    gmic_library::gmic_list<float>& images = *d->gmicImages;
    images.assign(1);
    imageNames.assign(1);

    QString name  = QString::fromUtf8("pos(0,0),name(%1)").arg(d->inImage.originalFilePath());
    QByteArray ba = name.toUtf8();
    gmic_image<char>::string(ba.constData()).move_to(imageNames[0]);

    qCDebug(DIGIKAM_DPLUGIN_LOG) << "Processing image size" << d->inImage.size();

    GMicQtImageConverter::convertDImgtoCImg(
                                            d->inImage.copy(
                                                            0, 0,
                                                            d->inImage.width(),
                                                            d->inImage.height()
                                                           ),
                                            images[0]
                                           );

    qCDebug(DIGIKAM_DPLUGIN_LOG) << QString::fromUtf8("G'MIC: %1").arg(d->command);

    QString env = QString::fromLatin1("_input_layers=%1").arg((int)DefaultInputMode);
    env        += QString::fromLatin1(" _output_mode=%1").arg((int)DefaultOutputMode);
    env        += QString::fromLatin1(" _output_messages=%1").arg((int)OutputMessageMode::VerboseConsole);

    d->filterThread = new FilterThread(this,
                                       QLatin1String("skip 0"),
                                       d->command,
                                       env);

    d->filterThread->swapImages(images);
    d->filterThread->setImageNames(imageNames);

    d->completed = false;

    connect(d->filterThread, &FilterThread::finished,
            this, &GmicQtProcessor::slotProcessingFinished);

    d->timer.setInterval(250);

    connect(&d->timer, &QTimer::timeout,
            this, &GmicQtProcessor::slotSendProgressInformation);

    d->timer.start();
    d->filterThread->start();
}

void GmicQtProcessor::startProcessingFiles()
{
    gmic_list<char> imageNames;

    qCDebug(DIGIKAM_DPLUGIN_LOG) << "Processing images as layers:" << d->inFiles.size();

    gmic_library::gmic_list<float>& images = *d->gmicImages;
    images.assign(d->inFiles.size());
    imageNames.assign(d->inFiles.size());

    for (int i = 0 ; (i < d->inFiles.size()) && !d->cancel ; ++i)
    {
        QString name  = QString::fromUtf8("pos(0,0),name(%1)").arg(d->inFiles[i]);
        QByteArray ba = name.toUtf8();
        gmic_image<char>::string(ba.constData()).move_to(imageNames[i]);

        Q_EMIT signalProgressInfo(tr("Converting image %1").arg(QFileInfo(d->inFiles[i]).fileName()));

        bool b = d->inImage.load(d->inFiles[i]);

        if (b)
        {
            GMicQtImageConverter::convertDImgtoCImg(
                                                    d->inImage.copy(
                                                                    0, 0,
                                                                    d->inImage.width(),
                                                                    d->inImage.height()
                                                                   ),
                                                    images[i]
                                                   );
        }
        else
        {
            qCCritical(DIGIKAM_DPLUGIN_LOG) << "Error while loading" << d->inFiles[i];
        }
    }

    Q_EMIT signalProgressInfo(tr("Running G'MIC filter %1").arg(DToolTipStyleSheet().elidedText(d->command, Qt::ElideRight)));

    QString env = QString::fromLatin1("_input_layers=%1").arg((int)DefaultInputMode);
    env        += QString::fromLatin1(" _output_mode=%1").arg((int)DefaultOutputMode);
    env        += QString::fromLatin1(" _output_messages=%1").arg((int)OutputMessageMode::VerboseConsole);

    d->filterThread = new FilterThread(this,
                                       QLatin1String("skip 0"),
                                       d->command,
                                       env);

    d->filterThread->swapImages(images);
    d->filterThread->setImageNames(imageNames);

    d->completed = false;

    connect(d->filterThread, &FilterThread::finished,
            this, &GmicQtProcessor::slotProcessingFinished);

    d->timer.setInterval(250);

    connect(&d->timer, &QTimer::timeout,
            this, &GmicQtProcessor::slotSendProgressInformation);

    d->timer.start();
    d->filterThread->start();
}

void GmicQtProcessor::slotSendProgressInformation()
{
    if (d->filterThread)
    {
        Q_EMIT signalProgress(d->filterThread->progress());
    }
}

void GmicQtProcessor::slotProcessingFinished()
{
    d->timer.stop();
    QString errorMessage;
    QStringList status = d->filterThread->gmicStatus();

    qCDebug(DIGIKAM_DPLUGIN_LOG) << "G'MIC Filter status" << status;

    if (d->filterThread->failed())
    {
        qCWarning(DIGIKAM_DPLUGIN_LOG) << "G'MIC Filter execution failed!";

        errorMessage = d->filterThread->errorMessage();

        if (errorMessage.isEmpty())
        {
            errorMessage = QLatin1String("G'MIC Filter execution failed without error message.");
        }

        qCDebug(DIGIKAM_DPLUGIN_LOG) << errorMessage;
        d->completed = false;
    }
    else
    {
        gmic_list<gmic_pixel_type> images = d->filterThread->images();

        if (!d->filterThread->aborted())
        {
            GMicQtImageConverter::convertCImgtoDImg(
                                                    images[0],
                                                    d->outImage,
                                                    d->inImage.sixteenBit()
                                                   );

            qCDebug(DIGIKAM_DPLUGIN_LOG) << "G'MIC Filter execution completed!";

            d->completed = true;
        }
        else
        {
            qCWarning(DIGIKAM_DPLUGIN_LOG) << "G'MIC Filter execution aborted...";
        }
    }

    d->filterThread->deleteLater();
    d->filterThread = nullptr;

    Q_EMIT signalDone(errorMessage);
}

void GmicQtProcessor::cancel()
{
    d->cancel = true;

    if (d->filterThread)
    {
        d->filterThread->abortGmic();
    }
}

DImg GmicQtProcessor::outputImage() const
{
    return d->outImage;
}

QString GmicQtProcessor::processingCommand() const
{
    return d->command;
}

QString GmicQtProcessor::filterName() const
{
    return d->filterName;
}

bool GmicQtProcessor::processingComplete() const
{
    return d->completed;
}

} // namespace DigikamGmicQtPluginCommon

#include "moc_gmicqtprocessor.cpp"
