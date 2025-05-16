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

#pragma once

// Qt includes

#include <QObject>
#include <QString>

// digiKam includes

#include "dimg.h"

using namespace Digikam;

namespace DigikamGmicQtPluginCommon
{

class GmicQtProcessor : public QObject
{
    Q_OBJECT

public:

    explicit GmicQtProcessor(QObject* const parent = nullptr);
    ~GmicQtProcessor()                   override;

    /**
     * Set the Gmic command to process. More than one command can be chained using space separator.
     */
    bool setProcessingCommand(const QString& command);
    QString processingCommand()     const;

    /**
     * Return a litteral name of the Gmic command.
     */
    QString filterName()            const;

    /**
     * Process one single image. Used by the BQM plugin.
     */
    void setInputImage(const DImg& inImage);
    void startProcessingImage();

    /**
     * Process multiple files as layer. Used by the Generic plugin.
     */
    void setInputFiles(const QStringList& inFiles);
    void startProcessingFiles();

    /**
     * Return true if the Gmic processing is completed.
     */
    bool processingComplete()       const;

    /**
     * Return the result image processed by the Gmic command.
     */
    DImg outputImage()              const;

    /**
     * Allow to cancel the current Gmic command process.
     */
    void cancel();

Q_SIGNALS:

    /**
     * Emitted when the processing is complete.
     * errorMessage contains a litteral string of the error occured during Gmic processing, if any.
     */
    void signalDone(const QString& errorMessage);

    /**
     * Return a progress value during Gmic processing.
     */
    void signalProgress(float progress);

private Q_SLOTS:

    void slotSendProgressInformation();
    void slotProcessingFinished();

private:

    class Private;
    Private* const d = nullptr;
};

} // namespace DigikamGmicQtPluginCommon
