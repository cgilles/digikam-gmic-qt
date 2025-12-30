/* ============================================================
 *
 * This file is a part of digiKam project
 * https://www.digikam.org
 *
 * Date        : 2019-11-28
 * Description : common implementations for the digiKam plugin for GmicQt.
 *
 * SPDX-FileCopyrightText: 2019-2026 by Gilles Caulier <caulier dot gilles at gmail dot com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ============================================================ */

#pragma once

// Qt includes

#include <QString>
#include <QList>
#include <QIcon>
#include <QPushButton>

// digiKam includes

#include "dplugin.h"
#include "filteraction.h"

using namespace Digikam;

namespace DigikamGmicQtPluginCommon
{

/**
 * @return the G'MIC plugin description and details.
 * @param title is used as top level header title of the description.
 */
QString              s_gmicQtPluginDetails(const QString& title);

/**
 * @return the G'MIC plugin authors list.
 */
QList<DPluginAuthor> s_gmicQtPluginAuthors();

/**
 * @return the G'MIC plugin icon.
 */
QIcon                s_gmicQtPluginIcon();

/**
 * @return the digiKam image versioning container populated with the G'MIC filter properties.
 * @param gmicCommand the G'MIC filter command.
 * @param filterPath the G'MIC filter path from the GmicQt filters hierarchy.
 * @param inMode the G'MIC filter input mode.
 * @param outMode the G'MIC filter output mode.
 * @param filterName the G'MIC filter name.
 */
FilterAction         s_gmicQtFilterAction(const QString& gmicCommand,
                                          const QString& filterPath,
                                          int            inMode,
                                          int            outMode,
                                          const QString& filterName);

/**
 * @brief Populate the dialog Help Button with about and documentation support.
 * @param parent the parent widget instance.
 * @param tool the digiKam plugin instance.
 * @param help the Help push button instance.
 */
void s_gmicQtPluginPopulateHelpButton(QWidget* const parent,
                                      DPlugin* const tool,
                                      QPushButton* const help);

} // namespace DigikamGmicQtPluginCommon
