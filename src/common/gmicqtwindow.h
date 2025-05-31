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

#pragma once

// Qt includes

#include <QString>
#include <QCloseEvent>
#include <QShowEvent>
#include <QWidget>

// digiKam includes

#include "dplugin.h"
#include "dinfointerface.h"

// Local includes

#include "gmicqtwidget.h"

using namespace Digikam;

namespace DigikamGmicQtPluginCommon
{

class GmicQtWindow : public GmicQtWidget
{
    Q_OBJECT

public:

    explicit GmicQtWindow(
                          DPlugin* const tool,
                          QWidget* const parent,
                          QString* const filterName
                         );
    ~GmicQtWindow()                     override = default;

public:

    /**
     * Create and run an instance of the GmicQtWindow.
     * Return the Gmic filter name selected in the GmicQtWidget.
     */
    static QString execWindow(DPlugin* const tool,
                              HostType type,
                              const QString& command = QString());

protected:

    /**
     * @brief Reimplemented to call GmicQtWidget::backupApplicationProperties() and GmicQtWidget::restoreApplicationProperties().
     * See these methods for details.
     */
    void showEvent(QShowEvent* event)   override;
    void closeEvent(QCloseEvent* event) override;
};

} // namespace DigikamGmicQtPluginCommon
