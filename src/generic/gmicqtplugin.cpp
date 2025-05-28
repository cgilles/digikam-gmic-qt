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
#include "gmicqtwizard.h"
#include "gmicqtsettingsdlg.h"
#include "gmicqtprocessordlg.h"
#include "gmic.h"

using namespace GmicQt;
using namespace DigikamGmicQtPluginCommon;

namespace DigikamGenericGmicQtPlugin
{

QList<QUrl> s_urlList;

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
    return QLatin1String("image_editor");
}

QString GmicQtPlugin::handbookChapter() const
{
    return QLatin1String("enhancement_tools");
}

QString GmicQtPlugin::handbookReference() const
{
    return QLatin1String("enhance-gmicqt-layers");
}

QList<DPluginAuthor> GmicQtPlugin::authors() const
{
    return s_gmicQtPluginAuthors();
}

void GmicQtPlugin::setup(QObject* const parent)
{
    DPluginAction* const ac = new DPluginAction(parent);
    ac->setIcon(icon());
    ac->setText(tr("G'MIC-Qt (layers)..."));
    ac->setObjectName(QLatin1String("GmicQt"));
    ac->setActionCategory(DPluginAction::GenericTool);

    connect(ac, SIGNAL(triggered(bool)),
//            this, SLOT(slotGmicQt()));
            this, SLOT(slotGmicQtWizard()));

    addAction(ac);
}

void GmicQtPlugin::slotGmicQtWizard()
{
    if (!reactivateToolDialog(m_toolDlg))
    {
        delete m_toolDlg;
        m_toolDlg = new GmicQtWizard(nullptr, infoIface(sender()));
        m_toolDlg->setPlugin(this);
        m_toolDlg->show();
    }
}

void GmicQtPlugin::slotGmicQt()
{
    DInfoInterface* const iface   = infoIface(sender());

    GmicQtSettingsDlg* const gdlg = new GmicQtSettingsDlg(this, iface);
    gdlg->show();

    QEventLoop loop;

    connect(gdlg, &QDialog::finished,
            [&loop, gdlg](int result)
        {
            loop.quit();

            if (result != QDialog::Accepted)
            {
                delete gdlg;
                return;
            }
        }
    );

    loop.exec();

    s_urlList        = gdlg->imageUrls();
    QUrl newURL      = iface->uploadUrl();
    QString fileName = gdlg->outputTemplate();
    int     format   = gdlg->outputFormat();

    delete gdlg;

    QClipboard* const clipboard  = QGuiApplication::clipboard();
    clipboard->clear();

    QString fname = GmicQtWindow::execWindow(
                                             this,                      // Plugin instance.
                                             GmicQtWindow::Generic      // Host type.
                                            );

    if (!clipboard->text().isEmpty() && !fname.isEmpty())
    {
        QStringList paths;

        for (const QUrl& url : std::as_const(s_urlList))
        {
            paths.append(url.toLocalFile());
        }

        GmicQtProcessorDlg* const pdlg = new GmicQtProcessorDlg(this);

        connect(pdlg, SIGNAL(signalUpdateHostApp(QUrl)),
                iface, SLOT(slotMetadataChangedForUrl(QUrl)));

        pdlg->setSettings(paths, clipboard->text(), newURL.toLocalFile(), fileName, format);
        pdlg->show();

        QEventLoop loop2;

        connect(pdlg, &QDialog::finished,
                &loop2, &QEventLoop::quit);

        loop2.exec();

        delete pdlg;
    }
    else
    {
        qCDebug(DIGIKAM_DPLUGIN_GENERIC_LOG) << "G'MIC Generic tool aborted";
    }
}

} // namespace DigikamGenericGmicQtPlugin

#include "moc_gmicqtplugin.cpp"
