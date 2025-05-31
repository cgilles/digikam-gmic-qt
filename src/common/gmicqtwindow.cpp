/* ============================================================
 *
 * This file is a part of digiKam project
 * https://www.digikam.org
 *
 * Date        : 2019-11-28
 * Description : common implementations for the digiKam plugin for GmicQt.
 *
 * SPDX-FileCopyrightText: 2019-2025 by Gilles Caulier <caulier dot gilles at gmail dot com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ============================================================ */

#include "gmicqtwindow.h"

// Qt includes

#include <QApplication>
#include <QEventLoop>
#include <QSettings>

// digiKam includes

#include "digikam_debug.h"
#include "digikam_globals.h"

using namespace GmicQt;

namespace DigikamGmicQtPluginCommon
{

extern GmicQtWidget* s_mainWidget;
extern QString       s_filterName;

GmicQtWindow::GmicQtWindow(
                           DPlugin* const tool,
                           QWidget* const parent,
                           QString* const filterName
                          )
    : GmicQtWidget(tool, parent, filterName)
{
}

QString GmicQtWindow::execWindow(DPlugin* const tool,
                                 HostType type,
                                 const QString& command)
{
    GmicQtWidget::createWidget(tool, type, command);

    if (QSettings().value(QLatin1String("Config/MainWindowMaximized"), false).toBool())
    {
        s_mainWidget->showMaximized();  // krazy:exclude=qmethods
    }
    else
    {
        s_mainWidget->show();
    }

    // Make it destroy itself on close (signaling the event loop)

    s_mainWidget->setAttribute(Qt::WA_DeleteOnClose);

    // Wait than main widget is closed.

    QEventLoop loop;

    connect(s_mainWidget, SIGNAL(destroyed()),
            &loop, SLOT(quit()));

    loop.exec();

    return s_filterName;
}

void GmicQtWindow::showEvent(QShowEvent* event)
{
    GmicQtWidget::backupApplicationProperties();

    GmicQtWidget::showEvent(event);
}

void GmicQtWindow::closeEvent(QCloseEvent* event)
{
    GmicQtWidget::restoreApplicationProperties();

    GmicQtWidget::closeEvent(event);
}

} // namespace DigikamGmicQtPluginCommon

#include "moc_gmicqtwindow.cpp"
