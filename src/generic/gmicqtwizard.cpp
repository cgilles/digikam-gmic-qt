/* ============================================================
 *
 * This file is a part of digiKam project
 * https://www.digikam.org
 *
 * Date        : 2025-04-21
 * Description : digiKam generic GmicQt plugin supporting layer mode.
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

// KDE includes

#include <klocalizedstring.h>
#include <ksharedconfig.h>
#include <kconfiggroup.h>

// Local includes

#include "dwizardpage.h"
#include "digikam_debug.h"
#include "mailintropage.h"
#include "mailalbumspage.h"
#include "mailimagespage.h"
#include "mailsettingspage.h"
#include "mailfinalpage.h"

namespace DigikamGenericGmicQtPlugin
{

class Q_DECL_HIDDEN MailWizard::Private
{
public:

    explicit Private() = default;

    DInfoInterface*     iface           = nullptr;
    MailIntroPage*      introPage       = nullptr;
    MailAlbumsPage*     albumsPage      = nullptr;
    MailImagesPage*     imagesPage      = nullptr;
    MailSettingsPage*   settingsPage    = nullptr;
    MailFinalPage*      finalPage       = nullptr;
    MailSettings*       settings        = nullptr;
};

MailWizard::MailWizard(QWidget* const parent, DInfoInterface* const iface)
    : DWizardDlg(parent, QLatin1String("Email Dialog")),
      d         (new Private)
{
    setWindowTitle(i18nc("@title:window", "Send by Email"));
    setOption(QWizard::NoCancelButtonOnLastPage);
    setModal(true);

    d->iface             = iface;
    d->settings          = new MailSettings;

    KSharedConfigPtr config = KSharedConfig::openConfig();
    KConfigGroup group      = config->group(QLatin1String("Email Dialog Settings"));
    d->settings->readSettings(group);

    d->introPage         = new MailIntroPage(this,    i18n("Welcome to Email Tool"));
    d->albumsPage        = new MailAlbumsPage(this,   i18n("Albums Selection"));
    d->imagesPage        = new MailImagesPage(this,   i18n("Images List"));
    d->settingsPage      = new MailSettingsPage(this, i18n("Email Settings"));
    d->finalPage         = new MailFinalPage(this,    i18n("Export by Email"));
}

MailWizard::~MailWizard()
{
    KSharedConfigPtr config = KSharedConfig::openConfig();
    KConfigGroup group      = config->group(QLatin1String("Email Dialog Settings"));
    d->settings->writeSettings(group);

    delete d;
}

void MailWizard::setItemsList(const QList<QUrl>& urls)
{
    d->imagesPage->setItemsList(urls);
}

DInfoInterface* MailWizard::iface() const
{
    return d->iface;
}

MailSettings* MailWizard::settings() const
{
    return d->settings;
}

bool MailWizard::validateCurrentPage()
{
    if (!DWizardDlg::validateCurrentPage())
    {
        return false;
    }

    return true;
}

int MailWizard::nextId() const
{
    if (d->settings->selMode == MailSettings::ALBUMS)
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
