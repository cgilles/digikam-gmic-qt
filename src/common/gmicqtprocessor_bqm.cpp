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

#include "gmicqtprocessor_p.h"

namespace DigikamGmicQtPluginCommon
{

void GmicQtProcessor::setInputImage(const DImg& inImage)
{
    d->inImage = inImage;
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
    if (d->cancel)
    {
        d->completed = false;
        Q_EMIT signalDone(tr("G'MIC Filter execution aborted!"));
        return;
    }

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

DImg GmicQtProcessor::outputImage() const
{
    DImg outImage;

    if (d->completed && !d->cancel)
    {
        GMicQtImageConverter::convertCImgtoDImg(
                                                d->outImages[0],
                                                outImage,
                                                d->inImage.sixteenBit()
                                               );
    }

    return outImage;
}

} // namespace DigikamGmicQtPluginCommon
