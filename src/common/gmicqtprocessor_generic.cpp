/* ============================================================
 *
 * This file is a part of digiKam project
 * https://www.digikam.org
 *
 * Date        : 2019-11-28
 * Description : GmicQt Command Processor.
 *
 * SPDX-FileCopyrightText: 2019-2026 by Gilles Caulier <caulier dot gilles at gmail dot com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ============================================================ */

#include "gmicqtprocessor_p.h"

namespace DigikamGmicQtPluginCommon
{

void GmicQtProcessor::setInputFiles(const QStringList& inFiles)
{
    d->inFiles = inFiles;
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

        bool b = d->inImage.load(d->inFiles[i], d->observer);

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
            d->completed = false;

            if (d->cancel)
            {
                Q_EMIT signalDone(tr("G'MIC Filter execution aborted!"));
            }
            else
            {
                Q_EMIT signalDone(tr("Error while loading %1").arg(d->inFiles[i]));
            }

            return;
        }
    }

    if (d->cancel)
    {
        d->completed = false;
        Q_EMIT signalDone(tr("G'MIC Filter execution aborted!"));
        return;
    }

    Q_EMIT signalProgressInfo(tr("Running G'MIC filter %1").arg(d->command));

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

gmic_library::gmic_list<gmic_pixel_type> GmicQtProcessor::outputImages() const
{
    return d->outImages;
}

} // namespace DigikamGmicQtPluginCommon
