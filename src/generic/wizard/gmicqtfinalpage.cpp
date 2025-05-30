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

#include "gmicqtfinalpage.h"

// Qt includes

#include <QIcon>
#include <QVBoxLayout>
#include <QUrl>
#include <QApplication>
#include <QStyle>
#include <QTimer>

// Digikam includes

#include "digikam_debug.h"
#include "dprogresswdg.h"
#include "dhistoryview.h"

// Local includes

#include "gmicqtwizard.h"
#include "gmicqtprocessorthread.h"
#include "gmicqtcommon.h"

using namespace DigikamGmicQtPluginCommon;

namespace DigikamGenericGmicQtPlugin
{

class Q_DECL_HIDDEN GmicQtFinalPage::Private
{
public:

    explicit Private(QWizard* const dialog)
    {
        wizard = dynamic_cast<GmicQtWizard*>(dialog);

        if (wizard)
        {
            iface    = wizard->iface();
            settings = wizard->settings();
        }
    }

    DHistoryView*           progressView  = nullptr;
    DProgressWdg*           progressBar   = nullptr;
    bool                    complete      = false;
    GmicQtProcessorThread*  processor     = nullptr;
    GmicQtWizard*           wizard        = nullptr;
    GmicQtSettings*         settings      = nullptr;
    DInfoInterface*         iface         = nullptr;
};

GmicQtFinalPage::GmicQtFinalPage(QWizard* const dialog, const QString& title)
    : DWizardPage(dialog, title),
      d          (new Private(dialog))
{
    QWidget* const vbox     = new QWidget(this);
    d->progressView         = new DHistoryView(vbox);
    d->progressBar          = new DProgressWdg(vbox);

    QVBoxLayout* const vlay = new QVBoxLayout(vbox);
    vlay->addWidget(d->progressView);
    vlay->addWidget(d->progressBar);
    vlay->setStretchFactor(d->progressBar, 10);
    vlay->setContentsMargins(QMargins());
    vlay->setSpacing(layoutSpacing());

    setPageWidget(vbox);
    setLeftBottomPix(QIcon::fromTheme(QLatin1String("run-build")));

    connect(d->progressBar, SIGNAL(signalProgressCanceled()),
            this, SLOT(slotCancel()));
}

GmicQtFinalPage::~GmicQtFinalPage()
{
    slotCancel();

    delete d;
}

void GmicQtFinalPage::slotCancel()
{
    if (d->processor)
    {
        d->processor->cancel();
    }
}

void GmicQtFinalPage::initializePage()
{
    d->complete = false;

    Q_EMIT completeChanged();

    QTimer::singleShot(0, this, SLOT(slotProcess()));
}

void GmicQtFinalPage::slotProcess()
{
    if (!d->wizard)
    {
        d->progressView->addEntry(tr("Internal Error"),
                                  DHistoryView::ErrorEntry);
        return;
    }

    d->progressView->clear();
    d->progressBar->reset();
    d->progressBar->setMinimum(0);
    d->progressBar->setValue(0);
    d->progressBar->setMaximum(d->settings->inputImages.size() + 2);
    d->progressBar->progressScheduled(tr("G'MIC-Qt (layers)"), true, true);
    d->progressBar->progressThumbnailChanged(s_gmicQtPluginIcon().pixmap(22, 22));

    delete d->processor;

    d->processor = new GmicQtProcessorThread(this);

    connect(d->processor, SIGNAL(signalComplete(QString)),
            this, SLOT(slotComplete(QString)));

    connect(d->processor, SIGNAL(signalProgressInfo(QString)),
            this, SLOT(slotProgressInfo(QString)));

    connect(d->processor, SIGNAL(signalUpdateHostApp(QUrl)),
            d->iface, SLOT(slotMetadataChangedForUrl(QUrl)));

    d->processor->setSettings(d->settings);
    d->processor->start();
}

void GmicQtFinalPage::cleanupPage()
{
    slotCancel();
}

void GmicQtFinalPage::slotProgressInfo(const QString& info)
{
    d->progressView->addEntry(info, DHistoryView::ProgressEntry);
    d->progressBar->setValue(d->progressBar->value() + 1);
}

void GmicQtFinalPage::slotComplete(const QString& error)
{
    d->progressBar->setValue(d->progressBar->maximum());
    d->progressBar->progressCompleted();

    d->complete = true;

    if (error.isEmpty())
    {
        d->progressView->addEntry(tr("G'MIC filter is done"), DHistoryView::SuccessEntry);
    }
    else
    {
        d->progressView->addEntry(tr("Error while processing G'MIC filter"), DHistoryView::ErrorEntry);
        d->progressView->addEntry(error, DHistoryView::ErrorEntry);
    }

    Q_EMIT completeChanged();
}

bool GmicQtFinalPage::isComplete() const
{
    return d->complete;
}

} // namespace DigikamGenericGmicQtPlugin

#include "moc_gmicqtfinalpage.cpp"
