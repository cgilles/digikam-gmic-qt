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

#include "gmicqtfilterspage.h"

// Qt includes

#include <QWidget>

// Digikam includes

#include "digikam_debug.h"

// Local includes

#include "gmicqtwizard.h"
#include "gmicqtwindow.h"

using namespace DigikamGmicQtPluginCommon;

namespace DigikamGenericGmicQtPlugin
{

class Q_DECL_HIDDEN GmicQtFiltersPage::Private
{
public:

    explicit Private(QWizard* const dialog)
    {
        wizard = dynamic_cast<GmicQtWizard*>(dialog);

        if (wizard)
        {
            settings = wizard->settings();
            iface    = wizard->iface();
        }
    }

public:

    GmicQtWindow*   filtersView          = nullptr;
    GmicQtWizard*   wizard               = nullptr;
    DInfoInterface* iface                = nullptr;
    GmicQtSettings* settings             = nullptr;
};

GmicQtFiltersPage::GmicQtFiltersPage(QWizard* const dialog)
    : DWizardPage(dialog, QString()),
      d          (new Private(dialog))
{
    d->filtersView = GmicQtWindow::createWindow(nullptr, GmicQtWindow::Generic);
    d->filtersView->hideButtons();
    setShowLeftView(false);
    setPageWidget(d->filtersView);
}

GmicQtFiltersPage::~GmicQtFiltersPage()
{
    delete d;
}

void GmicQtFiltersPage::initializePage()
{
}

bool GmicQtFiltersPage::validatePage()
{

    return true;
}

} // namespace DigikamGenericGmicQtPlugin

#include "moc_gmicqtfilterspage.cpp"
