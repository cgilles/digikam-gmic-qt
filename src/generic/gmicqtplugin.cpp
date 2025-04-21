/* ============================================================
 *
 * This file is a part of digiKam project
 * https://www.digikam.org
 *
 * Date        : 2025-04-21
 * Description : digiKam generic GmicQt plugin supporting layer mode.
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
#include <QSettings>
#include <QEventLoop>
#include <QPointer>
#include <QImage>
#include <QBuffer>
#include <QByteArray>

// Libfftw includes

#ifdef cimg_use_fftw3
#   include <fftw3.h>
#endif

// Local includes

#include "gmicqtwizard.h"
#include "gmicqtcommon.h"
#include "gmicqtwindow.h"
#include "gmic.h"

using namespace GmicQt;
using namespace DigikamGmicQtPluginCommon;

namespace DigikamGenericGmicQtPlugin
{

GmicQtPlugin::GmicQtPlugin(QObject* const parent)
    : DPluginGeneric(parent)
{
}

void GmicQtPlugin::cleanUp()
{
//    delete m_toolDlg;
}

QString GmicQtPlugin::name() const
{
    return tr("GmicQt (layers mode)");
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
                                    "by selecting more than one item from icon-view or from albums."));
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
    ac->setText(tr("G'MIC-Qt (layers mode)..."));
    ac->setObjectName(QLatin1String("gmicqt"));
    ac->setActionCategory(DPluginAction::GenericTool);

    connect(ac, SIGNAL(triggered(bool)),
            this, SLOT(slotGmicQt()));

    addAction(ac);
}

void GmicQtPlugin::slotGmicQt()
{
/*
    if (!reactivateToolDialog(m_toolDlg))
    {
        delete m_toolDlg;

        m_toolDlg = new GmicQtWizard(nullptr, infoIface(sender()));
        m_toolDlg->setPlugin(this);
        m_toolDlg->show();
    }
*/
}

} // namespace DigikamGenericGmicQtPlugin

#include "moc_gmicqtplugin.cpp"
