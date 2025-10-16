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

#include <QCloseEvent>
#include <QShowEvent>

// Local includes

#include "gmicqtwidget.h"

using namespace Digikam;

namespace DigikamGmicQtPluginCommon
{

class GmicQtWindow : public GmicQtWidget
{
    Q_OBJECT

public:

    /**
     * @brief Standard constructor.
     * @param tool the current digiKam plugin instance.
     * @param filterName will host the name of the G'MIC filter selected in the widget.
     * @param parent the current parent widget.
     */
    explicit GmicQtWindow(
                          DPlugin* const tool,
                          QString* const filterName,
                          QWidget* const parent
                         );
    ~GmicQtWindow()                     override = default;

public:

    /**
     * @brief Create and run an instance of the GmicQtWindow.
     * @return the Gmic filter name selected in the GmicQtWidget.
     * @param tool the current digiKam plugin instance.
     * @param type See HostType enum for details.
     * @param command the G'MIC filter command to pre-select in the G'MIC widget.
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
