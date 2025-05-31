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

#include "MainWindow.h"
#include "GmicQt.h"

using namespace GmicQt;
using namespace Digikam;

namespace DigikamGmicQtPluginCommon
{

class GmicQtWidget : public MainWindow
{
    Q_OBJECT

public:

    enum HostType
    {
        ImageEditor = 0,    ///< Save the GmicQt settings in right configuration file for the digiKam Image Editor plugin.
        BQM,                ///< Save the GmicQt settings in right configuration file for the digiKam Batch Queue Manager plugin.
        Showfoto,           ///< Save the GmicQt settings in right configuration file for Showfoto plugin.
        Generic,            ///< Save the GmicQt settings in right configuration file for the digiKam Generic plugin.
        Unknow
    };

public:

    explicit GmicQtWidget(
                          DPlugin* const tool,
                          QWidget* const parent,
                          QString* const filterName
                         );
    ~GmicQtWidget()                     override;

    /**
     * @brief Call MainWindow::saveSettings() method.
     */
    void saveParameters();

    /**
     * @brief Change the Ok button text from the GmicQt instance as Select Filter.
     */
    void setFilterSelectionMode();

    /**
     * @brief Hide all buttons on the bottom of GmicQt widget, excepted the Settings button.
     */
    void hideButtons();

    /**
     * @brief Set the host type to use with the GmicQt widget instance for the read/write settings.
     * See HostType for details.
     */
    void setHostType(HostType type);

    /**
     * @brief Copy in the clipboard the Gmic filter commands selected in the current GmicQt instance.
     */
    void copyGmicCommand();

public:

    /**
     * @brief Call this method to create an instance of the GmicQt tool widget.
     * The widget deletion is delegate to the caller.
     */
    static GmicQtWidget* createWidget(DPlugin* const tool,
                                      HostType type,
                                      const QString& command = QString());

    /**
     * @brief These method are used to backup and restore the host QApplication properties before the GmicQt instance,
     * as application definitions are modified to read and write the tool configuration with QSettings.
     * These method must be called before and after to call @method createWidget(), else the host application settings
     * will be moved in the GmicQt configuration file (and vis-versa).
     */
    static void backupApplicationProperties();
    static void restoreApplicationProperties();

private Q_SLOTS:

    /**
     * @brief Called when the GmicQt Ok button is pressed.
     */
    void slotOkClicked();

private:

    class Private;
    Private* const d = nullptr;
};

} // namespace DigikamGmicQtPluginCommon
