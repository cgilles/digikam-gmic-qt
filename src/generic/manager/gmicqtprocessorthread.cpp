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
#include <QFileInfo>
#include <QScopedPointer>

// digiKam includes

#include "dimg.h"
#include "dmetadata.h"
#include "digikam_debug.h"
#include "dimgloaderobserver.h"

// Local includes

#include "gmicqtcommon.h"
#include "gmicqtimageconverter.h"

namespace DigikamGenericGmicQtPlugin
{

class GmicQtProcessorThreadObserver;

class Q_DECL_HIDDEN GmicQtProcessorThread::Private
{

public:

    Private() = default;

public:

    bool                            cancel       = false;
    GmicQtProcessor*                proc         = nullptr;
    GmicQtProcessorThreadObserver*  observer     = nullptr;
    QStringList                     inputPaths;
    QString                         command;
    QString                         outputPath;
    QString                         fileName;
    int                             outputFormat = DImg::JPEG;
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
    if (d->proc)
    {
        d->proc->cancel();
    }

    d->cancel = true;
}

void GmicQtProcessorThread::setSettings(const QStringList& inputPaths,
                                        const QString& command,
                                        const QString& outputPath,
                                        const QString& fileName,
                                        int   outputFormat)
{
    qCDebug(DIGIKAM_DPLUGIN_GENERIC_LOG) << "G'MIC command        :" << command;
    qCDebug(DIGIKAM_DPLUGIN_GENERIC_LOG) << "Images to Process    :" << inputPaths;
    qCDebug(DIGIKAM_DPLUGIN_GENERIC_LOG) << "Output image path    :" << outputPath;
    qCDebug(DIGIKAM_DPLUGIN_GENERIC_LOG) << "Output image filename:" << fileName;
    qCDebug(DIGIKAM_DPLUGIN_GENERIC_LOG) << "Output image format  :" << DImg::formatToMimeType((DImg::FORMAT)outputFormat);

    d->inputPaths   = inputPaths;
    d->command      = command;
    d->outputPath   = outputPath;
    d->fileName     = fileName;
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
        qCDebug(DIGIKAM_DPLUGIN_GENERIC_LOG) << "GmicGenericTool: preparing G'MIC filter...";

        QEventLoop* const loop = new QEventLoop;

        connect(d->proc, &GmicQtProcessor::signalDone,
                loop, &QEventLoop::quit);

        d->proc->startProcessingFiles();

        if (!d->cancel)
        {
            qCDebug(DIGIKAM_DPLUGIN_GENERIC_LOG) << "GmicGenericTool: started G'MIC filter...";

            loop->exec();

            if (d->proc->processingComplete())
            {
                qCDebug(DIGIKAM_DPLUGIN_GENERIC_LOG) << "GmicGenericTool: G'MIC filter completed";

                DImg    outImage;
                QString filesList;

                for (const QString& inpath : std::as_const(d->inputPaths))
                {
                    filesList.append(QFileInfo(inpath).fileName() + QLatin1String(" ; "));
                }

                filesList.truncate(filesList.length() - 3);

                gmic_library::gmic_list<gmic_pixel_type> outImages = d->proc->outputImages();

                for (int i = 0 ; (i < outImages.size()) && !d->cancel ; i++)
                {
                    QString outFilePath = QString::fromUtf8("%1%2_%3.%4")
                                              .arg(d->outputPath)
                                              .arg(d->fileName)
                                              .arg(i, 2, 10, QLatin1Char('0'))
                                              .arg(DImg::formatToMimeType((DImg::FORMAT)d->outputFormat));

                    Q_EMIT signalProgressInfo(tr("Save data into %1").arg(outFilePath));

                    GMicQtImageConverter::convertCImgtoDImg(
                                                            outImages[i],
                                                            outImage,
                                                            (d->outputFormat != DImg::JPEG)
                                                           );

                    if (
                        outImage.save(
                                      outFilePath,
                                      (DImg::FORMAT)d->outputFormat,
                                      d->observer
                                     )
                       )
                    {
                        qCDebug(DIGIKAM_DPLUGIN_GENERIC_LOG) << "GmicGenericTool: G'MIC save data completed";

                        // Restoring matadata

                        qCDebug(DIGIKAM_DPLUGIN_GENERIC_LOG) << "Copying GPS info...";

                        // Find first src image which contain geolocation and save it to target file.

                        double lat, lng, alt;
                        QScopedPointer<DMetadata> meta(new DMetadata);

                        for (const QString& inpath : std::as_const(d->inputPaths))
                        {
                            meta->load(inpath);

                            if (meta->getGPSInfo(alt, lat, lng))
                            {
                                qCDebug(DIGIKAM_DPLUGIN_GENERIC_LOG) << "GPS info found from"
                                                                     << inpath << "and saved in"
                                                                     << outFilePath;
                                meta->load(outFilePath);
                                meta->setGPSInfo(alt, lat, lng);
                                meta->applyChanges(true);
                                break;
                            }
                        }

                        // Restore usual and common metadata from first item.

                        meta->load(d->inputPaths.constFirst());
                        QByteArray iptc = meta->getIptc();
                        QByteArray xmp  = meta->getXmp();
                        QString make    = meta->getExifTagString("Exif.Image.Make");
                        QString model   = meta->getExifTagString("Exif.Image.Model");
                        QDateTime dt    = meta->getItemDateTime();

                        meta->load(outFilePath);
                        meta->setIptc(iptc);
                        meta->setXmp(xmp);
                        meta->setXmpTagString("Xmp.tiff.Make",  make);
                        meta->setXmpTagString("Xmp.tiff.Model", model);
                        meta->setImageDateTime(dt);
                        meta->setXmpTagString("Xmp.digiKam.GmicInputFiles", filesList);
                        meta->setXmpTagString("Xmp.digiKam.GmicCommand",    d->command);
                        meta->applyChanges(true);

                        Q_EMIT signalUpdateHostApp(QUrl::fromLocalFile(outFilePath));
                    }
                    else
                    {
                        error = tr("Cannot save G'MIC filter data!");
                    }
                }
            }
            else
            {
                qCDebug(DIGIKAM_DPLUGIN_GENERIC_LOG) << "GmicGenericTool: left G'MIC filter with error.";
                error = tr("Cannot process G'MIC filter!");
            }
        }
        else
        {
            error = tr("G'MIC filter cancelled!");
        }

        delete loop;
    }

    delete d->proc;
    d->proc = nullptr;

    Q_EMIT signalComplete(error);
}

} // namespace DigikamGenericGmicQtPlugin

#include "moc_gmicqtprocessorthread.cpp"
