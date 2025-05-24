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

GmicQtProcessor::GmicQtProcessor(QObject* const parent)
    : QObject(parent),
      d      (new Private)
{
    GmicStdLib::Array = Updater::getInstance()->buildFullStdlib();
    d->gmicImages->assign();
    d->observer       = new GmicQtProcessorObserver(d);
}

GmicQtProcessor::~GmicQtProcessor()
{
    delete d->gmicImages;
    delete d->observer;
    delete d;
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
            errorMessage = tr("G'MIC Filter execution failed without error message.");
        }

        qCDebug(DIGIKAM_DPLUGIN_LOG) << errorMessage;
        d->completed = false;
    }
    else
    {
        d->outImages = d->filterThread->images();

        qCDebug(DIGIKAM_DPLUGIN_LOG) << "G'MIC Filter generated output images:" << d->outImages.size();

        if (!d->filterThread->aborted())
        {
            qCDebug(DIGIKAM_DPLUGIN_LOG) << "G'MIC Filter execution completed.";

            d->completed = true;
        }
        else
        {
            errorMessage = tr("G'MIC Filter execution aborted!");
            qCDebug(DIGIKAM_DPLUGIN_LOG) << errorMessage;
            d->completed = false;
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
