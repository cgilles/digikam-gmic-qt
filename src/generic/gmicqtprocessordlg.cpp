/* ============================================================
 *
 * This file is a part of digiKam project
 * https://www.digikam.org
 *
 * Date        : 2025-05-17
 * Description : a G'MIC processor progress dialog
 *
 * SPDX-FileCopyrightText: 2025 by Gilles Caulier <caulier dot gilles at gmail dot com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ============================================================ */

#include "gmicqtprocessordlg.h"

// Qt includes

#include <QPushButton>
#include <QProgressBar>
#include <QLabel>
#include <QImage>
#include <QGridLayout>
#include <QDialogButtonBox>

// digiKam includes

#include "dimg.h"
#include "digikam_debug.h"
#include "dhistoryview.h"
#include "dimgloaderobserver.h"

// Local includes

#include "gmicqtcommon.h"
#include "gmicqtprocessorthread.h"

namespace DigikamGenericGmicQtPlugin
{

class Q_DECL_HIDDEN GmicQtProcessorDlg::Private
{
public:

    Private() = default;

public:

    GmicQtProcessorThread* thread  = nullptr;
    DHistoryView*          history = nullptr;
    QDialogButtonBox*      buttons = nullptr;
    QProgressBar*          pbar    = nullptr;
};

GmicQtProcessorDlg::GmicQtProcessorDlg(DPlugin* const tool, QWidget* const parent)
    : QDialog(parent),
      d      (new Private)
{
    d->buttons = new QDialogButtonBox(QDialogButtonBox::Help | QDialogButtonBox::Cancel, this);
    d->buttons->button(QDialogButtonBox::Cancel)->setVisible(true);
    s_gmicQtPluginPopulateHelpButton(this, tool, d->buttons->button(QDialogButtonBox::Help));

    QWidget* const page     = new QWidget(this);
    QGridLayout* const grid = new QGridLayout(page);

    // --------------------------------------------------------

    QLabel* const logo      = new QLabel(page);
    logo->setPixmap(s_gmicQtPluginIcon().pixmap(48, 48));

    // --------------------------------------------------------

    QLabel* const header    = new QLabel(page);
    header->setWordWrap(true);
    header->setText(tr("Processing G'MIC Filter"));

    // --------------------------------------------------------

    d->history              = new DHistoryView(page);
    d->pbar                 = new QProgressBar(page);
    d->pbar->setMinimum(0);
    d->pbar->setMaximum(0);
    d->pbar->setValue(0);

    // --------------------------------------------------------

    grid->addWidget(logo,        0, 0, 1, 1);
    grid->addWidget(header,      0, 1, 1, 1);
    grid->addWidget(d->history,  1, 0, 1, 2);
    grid->addWidget(d->pbar,     2, 0, 1, 2);
    grid->setColumnStretch(1, 10);
    grid->setRowStretch(1, 10);
    grid->setContentsMargins(QMargins());
    grid->setSpacing(layoutSpacing());

    QVBoxLayout* const vbx = new QVBoxLayout(this);
    vbx->addWidget(page);
    vbx->addWidget(d->buttons);
    setLayout(vbx);

    resize(600, 400);

    // --------------------------------------------------------

    connect(d->buttons->button(QDialogButtonBox::Cancel), SIGNAL(clicked()),
            this, SLOT(accept()));

    connect(d->buttons->button(QDialogButtonBox::Cancel), SIGNAL(clicked()),
            this, SLOT(slotCancel()));

    connect(d->buttons->button(QDialogButtonBox::Help), SIGNAL(clicked()),
            this, SLOT(slotHelp()));

    d->thread = new GmicQtProcessorThread(this);

    connect(d->thread, SIGNAL(signalComplete(QString)),
            this, SLOT(slotComplete(QString)));

    connect(d->thread, SIGNAL(signalProgressInfo(QString)),
            this, SLOT(slotProgressInfo(QString)));
}

GmicQtProcessorDlg::~GmicQtProcessorDlg()
{
    delete d;
}

void GmicQtProcessorDlg::closeEvent(QCloseEvent* e)
{
    if (!e)
    {
        return;
    }

    slotCancel();
    e->accept();
}

void GmicQtProcessorDlg::slotCancel()
{
    d->thread->cancel();
}

void GmicQtProcessorDlg::setSettings(const QStringList& inputPaths,
                                     const QString& command,
                                     const QString& outputPath,
                                     const QString& outputFormat)
{
    d->thread->setSettings(inputPaths, command, outputPath, outputFormat);
    d->thread->start();
}

void GmicQtProcessorDlg::slotProgressInfo(const QString& info)
{
    d->history->addEntry(info, DHistoryView::ProgressEntry);
}

void GmicQtProcessorDlg::slotComplete(const QString& error)
{
    d->buttons->button(QDialogButtonBox::Cancel)->setText(tr("Close"));

    disconnect(d->buttons->button(QDialogButtonBox::Cancel), SIGNAL(clicked()));

    connect(d->buttons->button(QDialogButtonBox::Cancel), SIGNAL(clicked()),
            this, SLOT(accept()));

    d->pbar->setMaximum(1);
    d->pbar->setValue(1);

    if (error.isEmpty())
    {
        d->history->addEntry(tr("G'MIC filter is done"), DHistoryView::SuccessEntry);
    }
    else
    {
        d->history->addEntry(tr("Error while processing G'MIC filter"), DHistoryView::ErrorEntry);
        d->history->addEntry(error, DHistoryView::ErrorEntry);
    }
}

} // namespace DigikamGenericGmicQtPlugin

#include "moc_gmicqtprocessordlg.cpp"
