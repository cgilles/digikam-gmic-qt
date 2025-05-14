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

// Libfftw includes

#ifdef cimg_use_fftw3
#   include <fftw3.h>
#endif

// digiKam includes

#include "dinfointerface.h"
#include "digikam_debug.h"

// Local includes

#include "gmicqtwindow.h"
#include "gmicqtcommon.h"
#include "gmic.h"

using namespace GmicQt;
using namespace DigikamGmicQtPluginCommon;

namespace DigikamGenericGmicQtPlugin
{

DInfoInterface* s_infoIface = nullptr;

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
    ac->setText(tr("G'MIC-Qt..."));
    ac->setObjectName(QLatin1String("GmicQt"));
    ac->setActionCategory(DPluginAction::GenericTool);

    connect(ac, SIGNAL(triggered(bool)),
            this, SLOT(slotGmicQt()));

    addAction(ac);
}

void GmicQtPlugin::slotGmicQt()
{
    s_infoIface = infoIface(sender());

    QList<QUrl> selection = s_infoIface->currentSelectedItems();

    if (selection.count() == 0)
    {
        qCDebug(DIGIKAM_DPLUGIN_GENERIC_LOG) << "No image selected...";
        return;
    }

    QClipboard* const clipboard = QGuiApplication::clipboard();
    clipboard->clear();

    GmicQtWindow::execWindow(
                             this,                      // Plugin instance.
                             GmicQtWindow::Generic      // Host type.
                            );

    if (!clipboard->text().isEmpty())
    {
        qCDebug(DIGIKAM_DPLUGIN_GENERIC_LOG) << "GMic selected command:" << clipboard->text();
    }
}

} // namespace DigikamGenericGmicQtPlugin

#include "moc_gmicqtplugin.cpp"
