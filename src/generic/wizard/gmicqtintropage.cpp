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

#include "gmicqtintropage.h"

// Qt includes

#include <QLabel>
#include <QPixmap>
#include <QIcon>
#include <QVBoxLayout>

// Local includes

#include "digikam_config.h"
#include "digikam_debug.h"
#include "gmicqtwizard.h"
#include "gmicqtcommon.h"

using namespace DigikamGmicQtPluginCommon;

namespace DigikamGenericGmicQtPlugin
{

GmicQtIntroPage::GmicQtIntroPage(QWizard* const dialog, const QString& title)
    : DWizardPage(dialog, title)
{
    QWidget* const vbox     = new QWidget(this);
    QLabel* const desc      = new QLabel(vbox);

    desc->setWordWrap(true);
    desc->setOpenExternalLinks(true);

    QString str            = QLatin1String("<qt>");
    str.append(tr("<p><h1><b>Welcome to G'MIC Layers Tool</b></h1></p>"
                    "<p>This assistant will guide you to select and apply G'MIC filter "
                    "to a selection of images taken from your collection.</p>"));

    str.append(QLatin1String("</qt>"));

    desc->setText(str);

    QVBoxLayout* const vlay = new QVBoxLayout(vbox);
    vlay->addWidget(desc);

    setPageWidget(vbox);
    setLeftBottomPix(s_gmicQtPluginIcon().pixmap(128).scaledToWidth(128));
}

} // namespace DigikamGenericGmicQtPlugin

#include "moc_gmicqtintropage.cpp"
