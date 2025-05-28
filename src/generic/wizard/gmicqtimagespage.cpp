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

#include "gmicqtimagespage.h"

// Qt includes

#include <QIcon>
#include <QPixmap>
#include <QLabel>
#include <QList>
#include <QUrl>

// Local includes

#include "gmicqtwizard.h"
#include "ditemslist.h"

namespace DigikamGenericGmicQtPlugin
{

extern QList<QUrl> s_urlList;

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

public:

    DItemsList*     imageList   = nullptr;
    GmicQtWizard*   wizard      = nullptr;
    DInfoInterface* iface       = nullptr;
};

GmicQtImagesPage::GmicQtImagesPage(QWizard* const dialog, const QString& title)
    : DWizardPage(dialog, title),
      d          (new Private(dialog))
{
    QWidget* const vbox     = new QWidget(this);
    QLabel* const desc      = new QLabel(vbox);
    desc->setText(tr("<p>This view list all stacked items to process with the G'MIC filter.</p>"));

    d->imageList            = new DItemsList(vbox);
    d->imageList->setObjectName(QLatin1String("Gmic-Qt ImagesList"));
    d->imageList->setControlButtonsPlacement(DItemsList::ControlButtonsBelow);

    QVBoxLayout* const vlay = new QVBoxLayout(vbox);
    vlay->addWidget(desc);
    vlay->addWidget(d->imageList);

    setPageWidget(vbox);
    setLeftBottomPix(QIcon::fromTheme(QLatin1String("view-list-details")));

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
    d->imageList->loadImagesFromCurrentSelection();
}

bool GmicQtImagesPage::validatePage()
{
    if (d->imageList->imageUrls().isEmpty())
    {
        return false;
    }

    // To backup in the INI config file.

    d->wizard->settings()->inputImages = d->imageList->imageUrls();

    // To pass the list to G'MIC-Qt infra.

    s_urlList = d->wizard->settings()->inputImages;

    return true;
}

bool GmicQtImagesPage::isComplete() const
{
    return (!d->imageList->imageUrls().isEmpty());
}

} // namespace DigikamGenericGmicQtPlugin

#include "moc_gmicqtimagespage.cpp"
