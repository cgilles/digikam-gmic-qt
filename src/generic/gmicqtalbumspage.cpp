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

#include "mailalbumspage.h"

// Qt includes

#include <QIcon>
#include <QPixmap>

// Local includes

#include "gmicqtwizard.h"

namespace DigikamGenericGmicQtPlugin
{

class Q_DECL_HIDDEN GmicQtAlbumsPage::Private
{
public:

    explicit Private(QWizard* const dialog)
    {
        wizard = dynamic_cast<GmicQtWizard*>(dialog);

        if (wizard)
        {
            iface = wizard->iface();
        }
    }

    bool             albumSupport   = false;
    QWidget*         albumSelector  = nullptr;
    GmicQtWizard*    wizard         = nullptr;
    DInfoInterface*  iface          = nullptr;
};

GmicQtAlbumsPage::GmicQtAlbumsPage(QWizard* const dialog, const QString& title)
    : DWizardPage(dialog, title),
      d          (new Private(dialog))
{
    if (d->iface)
    {
        d->albumSelector = d->iface->albumChooser(this);

        connect(d->iface, SIGNAL(signalAlbumChooserSelectionChanged()),
                this, SIGNAL(completeChanged()));
    }
    else
    {
        d->albumSelector = new QWidget(this);
    }

    setPageWidget(d->albumSelector);
    setLeftBottomPix(QIcon::fromTheme(QLatin1String("folder-mail")));
}

GmicQtAlbumsPage::~GmicQtAlbumsPage()
{
    delete d;
}

bool GmicQtAlbumsPage::validatePage()
{
    if (!d->iface)
    {
        return false;
    }

    if (d->iface->albumChooserItems().isEmpty())
    {
        return false;
    }

    d->wizard->settings()->inputImages.clear();

    // update image list with album contents.

    const auto urls = d->iface->albumsItems(d->iface->albumChooserItems());

    for (const QUrl& url : urls)
    {
        d->wizard->settings()->inputImages << url;
    }

    return true;
}

bool GmicQtAlbumsPage::isComplete() const
{
    if (!d->iface)
    {
        return false;
    }

    return (!d->iface->albumChooserItems().isEmpty());
}

} // namespace DigikamGenericGmicQtPlugin

#include "moc_gmicqtalbumspage.cpp"
