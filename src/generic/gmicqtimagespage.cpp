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

#include "mailimagespage.h"

// Qt includes

#include <QIcon>
#include <QPixmap>
#include <QLabel>

// KDE includes

#include <klocalizedstring.h>

// Local includes

#include "mailwizard.h"
#include "ditemslist.h"
#include "dlayoutbox.h"

namespace DigikamGenericGmicQtPlugin
{

class Q_DECL_HIDDEN GmicQtImagesPage::Private
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

    DItemsList*     imageList   = nullptr;
    GmicQtWizard*     wizard      = nullptr;
    DInfoInterface* iface       = nullptr;
};

GmicQtImagesPage::GmicQtImagesPage(QWizard* const dialog, const QString& title)
    : DWizardPage(dialog, title),
      d          (new Private(dialog))
{
    DVBox* const vbox  = new DVBox(this);
    QLabel* const desc = new QLabel(vbox);
    desc->setText(i18n("<p>This view list all items to export by mail.</p>"));

    d->imageList       = new DItemsList(vbox);
    d->imageList->setObjectName(QLatin1String("GmicQtImages ImagesList"));
    d->imageList->setControlButtonsPlacement(DItemsList::ControlButtonsBelow);

    setPageWidget(vbox);
    setLeftBottomPix(QIcon::fromTheme(QLatin1String("image-stack")));

    connect(d->imageList, SIGNAL(signalImageListChanged()),
            this, SIGNAL(completeChanged()));
}

GmicQtImagesPage::~GmicQtImagesPage()
{
    delete d;
}

void GmicQtImagesPage::setItemsList(const QList<QUrl>& urls)
{
    d->imageList->slotAddImages(urls);
}

void GmicQtImagesPage::initializePage()
{
    d->imageList->setIface(d->iface);
    d->imageList->listView()->clear();

    if (d->wizard->settings()->selMode == GmicQtSettings::IMAGES)
    {
        d->imageList->loadImagesFromCurrentSelection();
    }
    else
    {
        setItemsList(d->wizard->settings()->inputImages);
    }
}

bool GmicQtImagesPage::validatePage()
{
    if (d->imageList->imageUrls().isEmpty())
    {
        return false;
    }

    d->wizard->settings()->inputImages = d->imageList->imageUrls();

    return true;
}

bool GmicQtImagesPage::isComplete() const
{
    return (!d->imageList->imageUrls().isEmpty());
}

} // namespace DigikamGenericGmicQtPlugin

#include "moc_gmicqtimagespage.cpp"
