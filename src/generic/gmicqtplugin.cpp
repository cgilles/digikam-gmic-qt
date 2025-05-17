/* ============================================================
 *
 * This file is a part of digiKam project
 * https://www.digikam.org
 *
 * Date        : 2025-04-21
 * Description : digiKam generic GmicQt plugin supporting layers mode.
 *
 * SPDX-FileCopyrightText: 2025 by Gilles Caulier <caulier dot gilles at gmail dot com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ============================================================ */

#include "gmicqtplugin.h"

// Qt includes

#include <QApplication>
#include <QTranslator>
#include <QPointer>
#include <QImage>
#include <QBuffer>
#include <QByteArray>
#include <QClipboard>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QImageWriter>

// Libfftw includes

#ifdef cimg_use_fftw3
#   include <fftw3.h>
#endif

// digiKam includes

#include "dinfointerface.h"
#include "digikam_debug.h"
#include "dfiledialog.h"
#include "dimg.h"

// Local includes

#include "gmicqtwindow.h"
#include "gmicqtcommon.h"
#include "gmicqtprocessordlg.h"
#include "gmic.h"

using namespace GmicQt;
using namespace DigikamGmicQtPluginCommon;

namespace DigikamGenericGmicQtPlugin
{

DInfoInterface* s_infoIface = nullptr;
QUrl            s_currentAlbumUrl;
QList<QUrl>     s_urlList;

GmicQtPlugin::GmicQtPlugin(QObject* const parent)
    : DPluginGeneric(parent)
{
}

void GmicQtPlugin::cleanUp()
{
}

QString GmicQtPlugin::name() const
{
    return QString::fromUtf8("GmicQt");
}

QString GmicQtPlugin::iid() const
{
    return QLatin1String(DPLUGIN_IID);
}

QIcon GmicQtPlugin::icon() const
{
    return s_gmicQtPluginIcon();
}

QString GmicQtPlugin::description() const
{
    return tr("A tool for G'MIC-Qt supporting layers mode");
}

QString GmicQtPlugin::details() const
{
    return s_gmicQtPluginDetails(tr("An Generic tool for G'MIC-Qt supporting layers mode "
                                    "by selecting more than one item from icon-view or from the stack."));
}

QString GmicQtPlugin::handbookSection() const
{
    return QLatin1String("post_processing");
}

QString GmicQtPlugin::handbookChapter() const
{
    return QLatin1String("gmicqt");
}

QList<DPluginAuthor> GmicQtPlugin::authors() const
{
    return s_gmicQtPluginAuthors();
}

void GmicQtPlugin::setup(QObject* const parent)
{
    DPluginAction* const ac = new DPluginAction(parent);
    ac->setIcon(icon());
    ac->setText(tr("G'MIC-Qt (layer)..."));
    ac->setObjectName(QLatin1String("GmicQt"));
    ac->setActionCategory(DPluginAction::GenericTool);

    connect(ac, SIGNAL(triggered(bool)),
            this, SLOT(slotGmicQt()));

    addAction(ac);
}

void GmicQtPlugin::slotGmicQt()
{
    s_infoIface = infoIface(sender());

    QClipboard* const clipboard = QGuiApplication::clipboard();
    clipboard->clear();

    QString fname = GmicQtWindow::execWindow(
                                             this,                      // Plugin instance.
                                             GmicQtWindow::Generic      // Host type.
                                            );

    if (!clipboard->text().isEmpty() && !fname.isEmpty())
    {
        QStringList writableMimetypes;
        QList<QByteArray> supported = QImageWriter::supportedMimeTypes();

        for (const QByteArray& mimeType : std::as_const(supported))
        {
            writableMimetypes.append(QLatin1String(mimeType));
        }

        // Put first class citizens at first place

        writableMimetypes.removeAll(QLatin1String("image/jpeg"));
        writableMimetypes.removeAll(QLatin1String("image/tiff"));
        writableMimetypes.removeAll(QLatin1String("image/png"));
        writableMimetypes.insert(0, QLatin1String("image/png"));
        writableMimetypes.insert(1, QLatin1String("image/jpeg"));
        writableMimetypes.insert(2, QLatin1String("image/tiff"));

        qCDebug(DIGIKAM_DPLUGIN_GENERIC_LOG) << "outputImages: Offered mimetypes: " << writableMimetypes;

        QLatin1String defaultMimeType("image/png");
        QLatin1String defaultFileName("image.png");

        QPointer<DFileDialog> imageFileSaveDialog = new DFileDialog(nullptr,
                                                                    QObject::tr("New Image File Name"),
                                                                    QFileInfo(s_currentAlbumUrl.toLocalFile()).filePath());
        imageFileSaveDialog->setAcceptMode(QFileDialog::AcceptSave);
        imageFileSaveDialog->setMimeTypeFilters(writableMimetypes);
        imageFileSaveDialog->selectMimeTypeFilter(defaultMimeType);
        imageFileSaveDialog->selectFile(defaultFileName);

        // Start dialog and check if canceled.

        imageFileSaveDialog->exec();

        if (!imageFileSaveDialog->hasAcceptedUrls())
        {
            delete imageFileSaveDialog;

            return;
        }

        QUrl newURL                  = imageFileSaveDialog->selectedUrls().first();
        QFileInfo fi(newURL.toLocalFile());

        // Parse name filter and extract file extension

        QString selectedFilterString = imageFileSaveDialog->selectedNameFilter();
        QLatin1String triggerString("*.");
        int triggerPos               = selectedFilterString.lastIndexOf(triggerString);
        QString format;

        if (triggerPos != -1)
        {
            format = selectedFilterString.mid(triggerPos + triggerString.size());
            format = format.left(format.size() - 1);
            format = format.toUpper();
        }

        // If name filter was selected, we guess image type using file extension.

        if (format.isEmpty())
        {
            format = fi.suffix().toUpper();

            QList<QByteArray> imgExtList = QImageWriter::supportedImageFormats();
            imgExtList << "TIF";
            imgExtList << "TIFF";
            imgExtList << "JPG";
            imgExtList << "JPE";

            if (!imgExtList.contains(format.toLatin1()) && !imgExtList.contains(format.toLower().toLatin1()))
            {
                QMessageBox::critical(nullptr, QObject::tr("Unsupported Format"),
                                      QObject::tr("The target image file format \"%1\" is not supported.").arg(format));

                qCWarning(DIGIKAM_DPLUGIN_GENERIC_LOG) << "target image file format " << format << " is not supported!";

                delete imageFileSaveDialog;

                return;
            }
        }

        if (!newURL.isValid())
        {
            QMessageBox::critical(nullptr, QObject::tr("Cannot Create File"),
                                  QObject::tr("Failed to create file\n\"%1\" to\n\"%2\".")
                                  .arg(newURL.fileName())
                                  .arg(QDir::toNativeSeparators(newURL.toLocalFile().section(QLatin1Char('/'), -2, -2))));

            qCWarning(DIGIKAM_DPLUGIN_GENERIC_LOG) << "target URL is not valid !";

            delete imageFileSaveDialog;

            return;
        }

        delete imageFileSaveDialog;

        QStringList paths;

        for (const QUrl& url : s_urlList)
        {
            paths.append(url.toLocalFile());
        }

        GmicQtProcessorDlg* const dlg = new GmicQtProcessorDlg;

        dlg->setSettings(paths, clipboard->text(), newURL.toLocalFile(), format);
        dlg->exec();

        delete dlg;
    }
    else
    {
        qCDebug(DIGIKAM_DPLUGIN_GENERIC_LOG) << "G'MIC Generic tool aborted";
    }
}

} // namespace DigikamGenericGmicQtPlugin

#include "moc_gmicqtplugin.cpp"
