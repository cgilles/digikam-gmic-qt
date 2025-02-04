/* ============================================================
 *
 * This file is a part of digiKam project
 * https://www.digikam.org
 *
 * Date        : 2019-11-28
 * Description : digiKam Batch Queue Manager plugin for GmicQt.
 *
 * SPDX-FileCopyrightText: 2019-2025 by Gilles Caulier <caulier dot gilles at gmail dot com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ============================================================ */

#include "gmicfilterchain_p.h"

namespace DigikamBqmGmicQtPlugin
{

GmicFilterChainViewItem::GmicFilterChainViewItem(GmicFilterChainView* const view,
                                                 int index,
                                                 const QString& title,
                                                 const QString& command)
    : QTreeWidgetItem(view),
      d              (new Private)
{
    setIndex(index);
    setTitle(title);
    setCommand(command);
    setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

    d->view = view;
}

GmicFilterChainViewItem::~GmicFilterChainViewItem()
{
    delete d;
}

void GmicFilterChainViewItem::setCommand(const QString& command)
{
    d->command = command;
    setText(GmicFilterChainView::Command, d->command);
}

QString GmicFilterChainViewItem::command() const
{
    return d->command;
}

void GmicFilterChainViewItem::setTitle(const QString& title)
{
    d->title = title;
    setText(GmicFilterChainView::Title, d->title);
}

QString GmicFilterChainViewItem::title() const
{
    return d->title;
}

void GmicFilterChainViewItem::setIndex(int index)
{
    d->index = index;
    setText(GmicFilterChainView::Index, QString::fromLatin1("%1").arg(d->index + 1));
}

int GmicFilterChainViewItem::index() const
{
    return d->index;
}

bool GmicFilterChainViewItem::operator<(const QTreeWidgetItem& other) const
{
    const GmicFilterChainViewItem& item = dynamic_cast<const GmicFilterChainViewItem&>(other);

    return (index() < item.index());
}

} // namespace DigikamBqmGmicQtPlugin
