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

#include "gmicqtwizard.h"

// Qt includes

#include <QCheckBox>
#include <QLabel>
#include <QMenu>
#include <QApplication>
#include <QComboBox>
#include <QListWidget>
#include <QTextBrowser>

// Local includes

#include "dwizardpage.h"
#include "gmicqtintropage.h"
#include "gmicqtalbumspage.h"
#include "gmicqtimagespage.h"
#include "gmicqtsettings.h"

namespace DigikamGenericGmicQtPlugin
{

class Q_DECL_HIDDEN GmicQtWizard::Private
{
public:

    explicit Private() = default;

public:

    DInfoInterface*     iface           = nullptr;
    GmicQtIntroPage*    introPage       = nullptr;
    GmicQtAlbumsPage*   albumsPage      = nullptr;
    GmicQtImagesPage*   imagesPage      = nullptr;
    GmicQtSettings*     settings        = nullptr;
};

GmicQtWizard::GmicQtWizard(QWidget* const parent, DInfoInterface* const iface)
    : DWizardDlg(parent, QLatin1String("GmicQt Wizard")),
      d         (new Private)
{
    setWindowTitle(tr("G'MIC-Qt Wizard"));
    setOption(QWizard::NoCancelButtonOnLastPage);
    setModal(true);

    d->iface      = iface;
    d->settings   = new GmicQtSettings;

    d->introPage  = new GmicQtIntroPage(this,  tr("Welcome to G'MIC-Qt Tool (Layers Mode)"));
    d->albumsPage = new GmicQtAlbumsPage(this, tr("Albums Selection"));
    d->imagesPage = new GmicQtImagesPage(this, tr("Images List"));
}

GmicQtWizard::~GmicQtWizard()
{
    delete d;
}

void GmicQtWizard::setItemsList(const QList<QUrl>& urls)
{
    d->imagesPage->setItemsList(urls);
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

int GmicQtWizard::nextId() const
{
    if (d->settings->selMode == GmicQtSettings::ALBUMS)
    {
        if (currentPage() == d->introPage)
        {
            return d->albumsPage->id();
        }
    }
    else
    {
        if (currentPage() == d->introPage)
        {
            return d->imagesPage->id();
        }
    }

    return DWizardDlg::nextId();
}

} // namespace DigikamGenericGmicQtPlugin

#include "moc_gmicqtwizard.cpp"
