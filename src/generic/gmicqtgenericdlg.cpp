/* ============================================================
 *
 * This file is a part of digiKam project
 * https://www.digikam.org
 *
 * Date        : 2025-05-17
 * Description : a G'MIC Generic plugin dialog
 *
 * SPDX-FileCopyrightText: 2025 by Gilles Caulier <caulier dot gilles at gmail dot com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ============================================================ */

#include "gmicqtgenericdlg.h"

// Qt includes

#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QDialogButtonBox>

// digiKam includes

#include "digikam_debug.h"
#include "ditemslist.h"

// Local includes

#include "gmicqtcommon.h"
#include "gmicqtprocessorthread.h"

namespace DigikamGenericGmicQtPlugin
{

class Q_DECL_HIDDEN GmicQtGenericDlg::Private
{
public:

    Private() = default;

public:

    QDialogButtonBox* buttons   = nullptr;
    DItemsList*       imageList = nullptr;
};

GmicQtGenericDlg::GmicQtGenericDlg(DPlugin* const tool, DInfoInterface* const iface, QWidget* const parent)
    : QDialog(parent),
      d      (new Private)
{
    setWindowTitle(tr("G'MIC-Qt (layers)"));

    d->buttons = new QDialogButtonBox(QDialogButtonBox::Help | QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    s_gmicQtPluginPopulateHelpButton(this, tool, d->buttons->button(QDialogButtonBox::Help));

    QWidget* const page     = new QWidget(this);
    QGridLayout* const grid = new QGridLayout(page);

    // --------------------------------------------------------

    QLabel* const logo      = new QLabel(page);
    logo->setPixmap(s_gmicQtPluginIcon().pixmap(48, 48));

    // --------------------------------------------------------

    QLabel* const header    = new QLabel(page);
    header->setWordWrap(true);
    header->setText(tr("<p>This view list all stacked items processed by the G'MIC filter as layers.</p>"
                       "<p>The first one is on top of the list, the last one</p>"
                       "<p>on the bottom. You can adjust the order of each item.</p>"));

    // --------------------------------------------------------

    d->imageList       = new DItemsList(page);
    d->imageList->setObjectName(QLatin1String("GmicGeneric ImagesList"));
    d->imageList->setControlButtonsPlacement(DItemsList::ControlButtonsRight);
    d->imageList->setIface(iface);
    d->imageList->listView()->clear();
    d->imageList->loadImagesFromCurrentSelection();

    // --------------------------------------------------------

    grid->addWidget(logo,          0, 0, 1, 1);
    grid->addWidget(header,        0, 1, 1, 1);
    grid->addWidget(d->imageList,  1, 0, 1, 2);
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

    connect(d->buttons->button(QDialogButtonBox::Ok), SIGNAL(clicked()),
            this, SLOT(accept()));

    connect(d->buttons->button(QDialogButtonBox::Cancel), SIGNAL(clicked()),
            this, SLOT(reject()));
}

GmicQtGenericDlg::~GmicQtGenericDlg()
{
    delete d;
}

QList<QUrl> GmicQtGenericDlg::imageUrls() const
{
    return d->imageList->imageUrls();
}

} // namespace DigikamGenericGmicQtPlugin

#include "moc_gmicqtgenericdlg.cpp"
