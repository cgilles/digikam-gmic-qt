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

namespace DigikamGenericSendByMailPlugin
{

class Q_DECL_HIDDEN MailImagesPage::Private
{
public:

    explicit Private(QWizard* const dialog)
    {
        wizard = dynamic_cast<MailWizard*>(dialog);

        if (wizard)
        {
            iface = wizard->iface();
        }
    }

    DItemsList*     imageList   = nullptr;
    MailWizard*     wizard      = nullptr;
    DInfoInterface* iface       = nullptr;
};

MailImagesPage::MailImagesPage(QWizard* const dialog, const QString& title)
    : DWizardPage(dialog, title),
      d          (new Private(dialog))
{
    DVBox* const vbox  = new DVBox(this);
    QLabel* const desc = new QLabel(vbox);
    desc->setText(i18n("<p>This view list all items to export by mail.</p>"));

    d->imageList       = new DItemsList(vbox);
    d->imageList->setObjectName(QLatin1String("MailImages ImagesList"));
    d->imageList->setControlButtonsPlacement(DItemsList::ControlButtonsBelow);

    setPageWidget(vbox);
    setLeftBottomPix(QIcon::fromTheme(QLatin1String("image-stack")));

    connect(d->imageList, SIGNAL(signalImageListChanged()),
            this, SIGNAL(completeChanged()));
}

MailImagesPage::~MailImagesPage()
{
    delete d;
}

void MailImagesPage::setItemsList(const QList<QUrl>& urls)
{
    d->imageList->slotAddImages(urls);
}

void MailImagesPage::initializePage()
{
    d->imageList->setIface(d->iface);
    d->imageList->listView()->clear();

    if (d->wizard->settings()->selMode == MailSettings::IMAGES)
    {
        d->imageList->loadImagesFromCurrentSelection();
    }
    else
    {
        setItemsList(d->wizard->settings()->inputImages);
    }
}

bool MailImagesPage::validatePage()
{
    if (d->imageList->imageUrls().isEmpty())
    {
        return false;
    }

    d->wizard->settings()->inputImages = d->imageList->imageUrls();

    return true;
}

bool MailImagesPage::isComplete() const
{
    return (!d->imageList->imageUrls().isEmpty());
}

} // namespace DigikamGenericSendByMailPlugin

#include "moc_mailimagespage.cpp"
