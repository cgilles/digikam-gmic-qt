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

#include "gmicqtwizard.h"

// Local includes

#include "dwizardpage.h"
#include "digikam_debug.h"
#include "gmicqtintropage.h"
#include "gmicqtimagespage.h"
#include "gmicqtsettingspage.h"
#include "gmicqtfilterspage.h"
#include "gmicqtfinalpage.h"

namespace DigikamGenericGmicQtPlugin
{

class Q_DECL_HIDDEN GmicQtWizard::Private
{
public:

    explicit Private() = default;

public:

    DInfoInterface*     iface        = nullptr;
    GmicQtSettings*     settings     = nullptr;
    GmicQtIntroPage*    introPage    = nullptr;
    GmicQtImagesPage*   imagesPage   = nullptr;
    GmicQtSettingsPage* settingsPage = nullptr;
    GmicQtFiltersPage*  filtersPage  = nullptr;
    GmicQtFinalPage*    finalPage    = nullptr;
};

GmicQtWizard::GmicQtWizard(QWidget* const parent, DInfoInterface* const iface)
    : DWizardDlg(parent, QLatin1String("GmicQtWizard Dialog")),
      d         (new Private)
{
    setWindowTitle(tr("G'MIC-Qt (layers mode)"));
    setOption(QWizard::NoCancelButtonOnLastPage);
    setModal(true);

    d->iface             = iface;
    d->settings          = new GmicQtSettings;
    d->settings->readSettings();

    d->introPage         = new GmicQtIntroPage(this,    tr("Welcome to G'MIC-Qt Layers Mode Tool"));
    d->imagesPage        = new GmicQtImagesPage(this,   tr("Stacked Images List"));
    d->settingsPage      = new GmicQtSettingsPage(this, tr("Processing Settings"));
    d->filtersPage       = new GmicQtFiltersPage(this); // NOTE: no title here.
    d->finalPage         = new GmicQtFinalPage(this,    tr("Processing G'MIC Filter"));
}

GmicQtWizard::~GmicQtWizard()
{
    d->settings->writeSettings();

    delete d;
}

DInfoInterface* GmicQtWizard::iface() const
{
    return d->iface;
}

GmicQtSettings* GmicQtWizard::settings() const
{
    return d->settings;
}

bool GmicQtWizard::validateCurrentPage()
{
    if (!DWizardDlg::validateCurrentPage())
    {
        return false;
    }

    return true;
}

} // namespace DigikamGenericGmicQtPlugin

#include "moc_gmicqtwizard.cpp"
