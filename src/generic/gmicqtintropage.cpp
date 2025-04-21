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
#include <QComboBox>
#include <QIcon>
#include <QGroupBox>
#include <QGridLayout>

// KDE includes

#include <klocalizedstring.h>

// Local includes

#include "digikam_debug.h"
#include "digikam_config.h"
#include "digikam_globals.h"
#include "dlayoutbox.h"
#include "mailwizard.h"
#include "mailsettings.h"
#include "dbinarysearch.h"
#include "balsabinary.h"
#include "clawsmailbinary.h"
#include "kmailbinary.h"
#include "evolutionbinary.h"
#include "netscapebinary.h"
#include "outlookbinary.h"
#include "sylpheedbinary.h"
#include "thunderbirdbinary.h"

namespace DigikamGenericGmicQtPlugin
{

class Q_DECL_HIDDEN GmicQtIntroPage::Private
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

    QComboBox*        imageGetOption    = nullptr;
    DHBox*            hbox              = nullptr;
    GmicQtWizard*       wizard            = nullptr;
    DInfoInterface*   iface             = nullptr;
    DBinarySearch*    binSearch         = nullptr;
    BalsaBinary       balsaBin;
    ClawsGmicQtBinary   clawsBin;
    EvolutionBinary   evoluBin;
    KmailBinary       kmailBin;
    NetscapeBinary    netscBin;
    OutlookBinary     outloBin;
    SylpheedBinary    sylphBin;
    ThunderbirdBinary thundBin;
};

GmicQtIntroPage::GmicQtIntroPage(QWizard* const dialog, const QString& title)
    : DWizardPage(dialog, title),
      d          (new Private(dialog))
{
    DVBox* const vbox  = new DVBox(this);
    QLabel* const desc = new QLabel(vbox);

    desc->setWordWrap(true);
    desc->setOpenExternalLinks(true);
    desc->setText(i18n("<qt>"
                       "<p><h1><b>Welcome to Email Tool</b></h1></p>"
                       "<p>This assistant will guide you to send "
                       "your items with a mail client application.</p>"
                       "<p>Before to export contents, you will be able to adjust attachments "
                       "properties accordingly with your mail service capabilities.</p>"
                       "</qt>"));

    // ComboBox for image selection method

    d->hbox                     = new DHBox(vbox);
    QLabel* const getImageLabel = new QLabel(i18n("&Choose image selection method:"), d->hbox);
    d->imageGetOption           = new QComboBox(d->hbox);
    d->imageGetOption->insertItem(GmicQtSettings::ALBUMS, i18n("Albums"));
    d->imageGetOption->insertItem(GmicQtSettings::IMAGES, i18n("Images"));
    getImageLabel->setBuddy(d->imageGetOption);

    // --------------------

    QGroupBox* const binaryBox      = new QGroupBox(vbox);
    QGridLayout* const binaryLayout = new QGridLayout;
    binaryBox->setLayout(binaryLayout);
    binaryBox->setTitle(i18nc("@title:group", "GmicQt client application Binaries"));
    d->binSearch = new DBinarySearch(binaryBox);
    d->binSearch->addBinary(d->balsaBin);
    d->binSearch->addBinary(d->clawsBin);
    d->binSearch->addBinary(d->kmailBin);
    d->binSearch->addBinary(d->evoluBin);
    d->binSearch->addBinary(d->netscBin);
    d->binSearch->addBinary(d->outloBin);
    d->binSearch->addBinary(d->sylphBin);
    d->binSearch->addBinary(d->thundBin);

#ifdef Q_OS_MACOS

    // Std Macports install

    d->binSearch->addDirectory(QLatin1String("/opt/local/bin"));

    // digiKam Bundle PKG install

    d->binSearch->addDirectory(macOSBundlePrefix() + QLatin1String("bin"));

#endif

#ifdef Q_OS_WIN

    // FIXME : adjust paths

    d->binSearch->addDirectory(QLatin1String("C:/Program Files/"));

    d->binSearch->addDirectory(QLatin1String("C:/Program Files (x86)/"));

#endif

    vbox->setStretchFactor(desc,      2);
    vbox->setStretchFactor(d->hbox,   1);
    vbox->setStretchFactor(binaryBox, 3);

    setPageWidget(vbox);
    setLeftBottomPix(QIcon::fromTheme(QLatin1String("mail-client")));

    connect(d->binSearch, SIGNAL(signalBinariesFound(bool)),
            this, SLOT(slotBinariesFound()));
}

GmicQtIntroPage::~GmicQtIntroPage()
{
    delete d;
}

void GmicQtIntroPage::initializePage()
{
    bool albumSupport = (d->iface && d->iface->supportAlbums());

    if (!albumSupport)
    {
        d->imageGetOption->setCurrentIndex(GmicQtSettings::IMAGES);
        d->hbox->setEnabled(false);
    }
    else
    {
        d->imageGetOption->setCurrentIndex(d->wizard->settings()->selMode);
    }

    d->binSearch->allBinariesFound();
    slotBinariesFound();
}

bool GmicQtIntroPage::validatePage()
{
    d->wizard->settings()->selMode = (GmicQtSettings::Selection)d->imageGetOption->currentIndex();

    return true;
}

void GmicQtIntroPage::slotBinariesFound()
{
    d->wizard->settings()->binPaths.insert(GmicQtSettings::BALSA, d->balsaBin.isValid() ?
                                           d->balsaBin.path() : QString());

    d->wizard->settings()->binPaths.insert(GmicQtSettings::CLAWSMAIL, d->clawsBin.isValid() ?
                                           d->clawsBin.path() : QString());

    d->wizard->settings()->binPaths.insert(GmicQtSettings::EVOLUTION, d->evoluBin.isValid() ?
                                           d->evoluBin.path() : QString());

    d->wizard->settings()->binPaths.insert(GmicQtSettings::KMAIL, d->kmailBin.isValid() ?
                                           d->kmailBin.path() : QString());

    d->wizard->settings()->binPaths.insert(GmicQtSettings::NETSCAPE, d->netscBin.isValid() ?
                                           d->netscBin.path() : QString());

    d->wizard->settings()->binPaths.insert(GmicQtSettings::OUTLOOK, d->outloBin.isValid() ?
                                           d->outloBin.path() : QString());

    d->wizard->settings()->binPaths.insert(GmicQtSettings::SYLPHEED, d->sylphBin.isValid() ?
                                           d->sylphBin.path() : QString());

    d->wizard->settings()->binPaths.insert(GmicQtSettings::THUNDERBIRD, d->thundBin.isValid() ?
                                           d->thundBin.path() : QString());

    Q_EMIT completeChanged();
}

bool GmicQtIntroPage::isComplete() const
{
    const auto vals = d->wizard->settings()->binPaths.values();
    QString val     = vals.join(QString());
    qCDebug(DIGIKAM_DPLUGIN_GENERIC_LOG) << val;

    return (!val.isEmpty());
}

} // namespace DigikamGenericGmicQtPlugin

#include "moc_gmicqtintropage.cpp"
