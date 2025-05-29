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
        ImageEditor = 0,
        BQM,
        Showfoto,
        Generic,
        Unknow
    };

public:

    explicit GmicQtWidget(
                          DPlugin* const tool,
                          QWidget* const parent,
                          QString* const filterName
                         );
    ~GmicQtWidget()                     override;

    void saveParameters();
    void setFilterSelectionMode();
    void hideButtons();
    void setHostType(HostType type);
    void copyGmicCommand();

public:

    static GmicQtWidget* createWidget(DPlugin* const tool,
                                      HostType type,
                                      const QString& command = QString());

    static void backupApplicationProperties();
    static void restoreApplicationProperties();

private Q_SLOTS:

    void slotOkClicked();

private:

    class Private;
    Private* const d = nullptr;
};

// -------------------------------------------------------------------------

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

    static QString execWindow(DPlugin* const tool,
                              HostType type,
                              const QString& command = QString());

protected:

    void showEvent(QShowEvent* event)   override;
    void closeEvent(QCloseEvent* event) override;
};

} // namespace DigikamGmicQtPluginCommon
