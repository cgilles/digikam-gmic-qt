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

    /**
     * @brief A class to run the G'MIC filter command and process files (Generic) or one image instance (BQM).
     * Implementation inspired from G'MIC-Qt HeadlessProcessor.
     */
    explicit GmicQtProcessor(QObject* const parent = nullptr);
    ~GmicQtProcessor()                   override;

    /**
     * @brief Set the G'MIC command to process. More than one command can be chained using space separator.
     */
    bool setProcessingCommand(const QString& command);
    QString processingCommand()     const;

    /**
     * @return a literal name of the G'MIC command.
     */
    QString filterName()            const;

    /**
     * @brief Process one single image. Used by the BQM plugin.
     */
    void setInputImage(const DImg& inImage);
    void startProcessingImage();

    /**
     * @brief Process multiple files as layer. Used by the Generic plugin.
     */
    void setInputFiles(const QStringList& inFiles);
    void startProcessingFiles();

    /**
     * @return true if the G'MIC processing is completed.
     */
    bool processingComplete()       const;

    /**
     * @return the result image processed by the G'MIC command.
     */
    DImg outputImage()              const;

    /**
     * @brief Allow to cancel the current G'MIC command process.
     */
    void cancel();

Q_SIGNALS:

    /**
     * @brief Emitted when the G'MIC processing is complete.
     * @param errorMessage contains a literal string of the error occurred during Gmic processing, if any.
     */
    void signalDone(const QString& errorMessage);

    /**
     * @brief Emit a progress value during G'MIC processing.
     * This depend of the selected G'MIC filter implementation.
     * If no feedback is suported in the G'MIC filter, -1 is always sent and this feature is not suitable.
     */
    void signalProgress(float progress);

    /**
     * @brief Emit @param info as a detailled literal progress information.
     */
    void signalProgressInfo(const QString& info);

private Q_SLOTS:

    void slotSendProgressInformation();
    void slotProcessingFinished();

private:

    class Private;
    Private* const d = nullptr;
};

} // namespace DigikamGmicQtPluginCommon
