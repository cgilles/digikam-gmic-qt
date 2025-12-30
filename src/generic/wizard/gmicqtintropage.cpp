/* ============================================================
 *
 * This file is a part of digiKam project
 * https://www.digikam.org
 *
 * Date        : 2025-04-21
 * Description : digiKam generic GmicQt plugin supporting layers mode.
 *
 * SPDX-FileCopyrightText: 2025-2026 by Gilles Caulier <caulier dot gilles at gmail dot com>
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
    QWidget* const vbox = new QWidget(this);
    QLabel* const desc  = new QLabel(vbox);

    desc->setWordWrap(true);
    desc->setOpenExternalLinks(true);

    QString str         = QLatin1String("<qt>");

    str.append(tr("<p><h1><b>Welcome to G'MIC Layers Tool</b></h1></p>"
                  "<p>This assistant will guide you to select and apply G'MIC filter "
                  "to a selection of images taken from your collection.</p>"
                  "<p>The items will be interpreted as separated input layers "
                  "to be composed or assembled together as a single or new output layers.</p>"));

    str.append(tr("<p><a href='https://gmic.eu/'>G'MIC</a> (GREYC’s Magic for Image Computing) is a full-featured open-source framework for image processing. "
                  "It provides filters for:</p>"
                  "<p><ul>"
                  "<li>Changing colors.</li>"
                  "<li>Creating patterns.</li>"
                  "<li>Adding artistic touches.</li>"
                  "<li>Repairing inconsistencies in pictures.</li>"
                  "<li>Rendering shapes.</li>"
                  "<li>And much more...</li>"
                  "</ul></p>"));

    str.append(tr("<p><u>Note:</u> this tool do not process items one by one in batch. Please use the Batch Queue Manager G'MIC Processor instead. "
                  "See the <a href='https://docs.digikam.org/en/batch_queue/base_tools.html#g-mic-processor'>online documentation</a> for much details.</p>"));

    str.append(tr("<p><b>Credits:</b></p>"));
    str.append(QString::fromUtf8("<a href='https://www.greyc.fr'>GREYC</a> | "
                                 "<a href='https://www.cnrs.fr'>CNRS</a> | "
                                 "<a href='https://www.unicaen.fr'>Normandy University</a> | "
                                 "<a href='https://www.ensicaen.fr'>Ensicaen</a>"));


    str.append(QLatin1String("</qt>"));

    desc->setText(str);

    QLabel* const logo      = new QLabel(vbox);
    logo->setPixmap(QPixmap(QLatin1String(":resources/logos.png")));

    QVBoxLayout* const vlay = new QVBoxLayout(vbox);
    vlay->addWidget(desc);
    vlay->addWidget(logo);

    setPageWidget(vbox);
    setLeftBottomPix(s_gmicQtPluginIcon().pixmap(128).scaledToWidth(128));
}

} // namespace DigikamGenericGmicQtPlugin

#include "moc_gmicqtintropage.cpp"
