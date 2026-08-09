/* ============================================================
 *
 * This file is a part of digiKam project
 * https://www.digikam.org
 *
 * Date        : 2019-11-28
 * Description : digiKam Batch Queue Manager plugin for GmicQt.
 *
 * SPDX-FileCopyrightText: 2019-2026 by Gilles Caulier <caulier dot gilles at gmail dot com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ============================================================ */

#include "gmicfiltermodel.h"

// Qt includes

#include <QBuffer>

// digiKam includes

#include "digikam_debug.h"
#include "ditemtooltip.h"

// Local includes

#include "gmicfiltermngr.h"
#include "gmicfilternode.h"
#include "gmicqtcommon.h"

using namespace Digikam;
using namespace DigikamGmicQtPluginCommon;

namespace DigikamBqmGmicQtPlugin
{

class Q_DECL_HIDDEN GmicFilterModel::Private
{
public:

    Private() = default;

public:

    GmicFilterManager* manager   = nullptr;
    bool               endMacro  = false;
};

// -----------------------------------------------------------------


GmicFilterModel::GmicFilterModel(GmicFilterManager* const mngr, QObject* const parent)
    : QAbstractItemModel(parent),
      d                 (new Private)
{
    d->manager = mngr;

    connect(d->manager, SIGNAL(signalEntryAdded(GmicFilterNode*)),
            this, SLOT(slotEntryAdded(GmicFilterNode*)));

    connect(d->manager, SIGNAL(signalEntryRemoved(GmicFilterNode*,int,GmicFilterNode*)),
            this, SLOT(slotEntryRemoved(GmicFilterNode*,int,GmicFilterNode*)));

    connect(d->manager, SIGNAL(signalEntryChanged(GmicFilterNode*)),
            this, SLOT(slotEntryChanged(GmicFilterNode*)));
}

GmicFilterModel::~GmicFilterModel()
{
    delete d;
}

GmicFilterManager* GmicFilterModel::manager() const
{
    return d->manager;
}

QModelIndex GmicFilterModel::index(GmicFilterNode* nd) const
{
    const GmicFilterNode* const item = nd->parent();

    if (!item)
    {
        return QModelIndex();
    }

    return createIndex(item->children().indexOf(nd), 0, nd);
}

void GmicFilterModel::slotEntryAdded(GmicFilterNode* item)
{
    Q_ASSERT(item && item->parent());

    int row                     = item->parent()->children().indexOf(item);
    GmicFilterNode* const pitem = item->parent();

    // item was already added so remove before beginInsertRows is called

    pitem->remove(item);
    beginInsertRows(index(pitem), row, row);
    pitem->add(item, row);
    endInsertRows();
}

void GmicFilterModel::slotEntryRemoved(GmicFilterNode* prnt, int row, GmicFilterNode* item)
{
    // item was already removed, re-add so beginRemoveRows works

    prnt->add(item, row);
    beginRemoveRows(index(prnt), row, row);
    prnt->remove(item);
    endRemoveRows();
}

void GmicFilterModel::slotEntryChanged(GmicFilterNode* item)
{
    QModelIndex idx = index(item);

    Q_EMIT dataChanged(idx, idx);
}

bool GmicFilterModel::removeRows(int row, int count, const QModelIndex& prnt)
{
    if ((row < 0) || (count <= 0) || ((row + count) > rowCount(prnt)))
    {
        return false;
    }

    const GmicFilterNode* const fnode = node(prnt);

    for (int i = (row + count - 1) ; i >= row ; --i)
    {
        GmicFilterNode* const item = fnode->children().at(i);
        d->manager->removeEntry(item);
    }

    if (d->endMacro)
    {
        d->manager->undoRedoStack()->endMacro();
        d->endMacro = false;
    }

    return true;
}

QVariant GmicFilterModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if ((orientation == Qt::Horizontal) && (role == Qt::DisplayRole))
    {
        switch (section)
        {
            case 0:
            {
                return QObject::tr("Title");
            }

            case 1:
            {
                return QObject::tr("Comment");
            }
        }
    }

    return QAbstractItemModel::headerData(section, orientation, role);
}

QVariant GmicFilterModel::data(const QModelIndex& ind, int role) const
{
    if (!ind.isValid() || (ind.model() != this))
    {
        return QVariant();
    }

    const GmicFilterNode* const commandNode = node(ind);

    switch (role)
    {
        case Qt::ToolTipRole:
        {
            if (commandNode->type() == GmicFilterNode::Item)
            {
                DToolTipStyleSheet cnt;
                QString tip    = cnt.tipHeader;
                QString header = commandNode->title;

                tip += cnt.headBeg + header + cnt.headEnd;

                tip += cnt.cellBeg +  QObject::tr("G'MIC items:") + cnt.cellMid;
                tip += QString::number(commandNode->commands.count()) + cnt.cellEnd;

                tip += cnt.cellBeg +  QObject::tr("Chained Filters:") + cnt.cellMid;
                const QStringList keys = commandNode->commands.keys();
                tip += cnt.breakString(keys.join(QLatin1String(", "))) + cnt.cellEnd;

                tip += cnt.cellBeg +  QObject::tr("Description:") + cnt.cellMid;
                tip += cnt.breakString(commandNode->desc) + cnt.cellEnd;

                tip += cnt.tipFooter;

                return tip;
            }

            return QVariant();
        }

        case Qt::DisplayRole:
        {
            if (commandNode->type() == GmicFilterNode::Separator)
            {
                // NOTE: separator is draw in the delegate implmeentation.

                return QVariant();
            }

            switch (ind.column())
            {
                case 0:
                {
                    return commandNode->title;
                }

                case 1:
                {
                    return commandNode->desc;
                }
            }

            break;
        }

        case GmicFilterModel::CommandRole:
        {
            return commandNode->commands;
        }

        case GmicFilterModel::DateAddedRole:
        {
            return commandNode->dateAdded;
        }

        case GmicFilterModel::TypeRole:
        {
            return commandNode->type();
        }

        case GmicFilterModel::SeparatorRole:
        {
            return (commandNode->type() == GmicFilterNode::Separator);
        }

        case Qt::DecorationRole:
        {
            if (ind.column() == 0)
            {
                if      (commandNode->type() == GmicFilterNode::Item)
                {
                    return QIcon::fromTheme(QLatin1String("process-working-symbolic"));
                }
                else if (commandNode->type() == GmicFilterNode::RootFolder)
                {
                    return s_gmicQtPluginIcon();
                }
                else if (commandNode->type() == GmicFilterNode::Separator)
                {
                    return QIcon();
                }
                else
                {
                    return QIcon::fromTheme(QLatin1String("folder"));
                }
            }
        }
    }

    return QVariant();
}

int GmicFilterModel::columnCount(const QModelIndex& prnt) const
{
    return ((prnt.column() > 0) ? 0 : 2);
}

int GmicFilterModel::rowCount(const QModelIndex& prnt) const
{
    if (prnt.column() > 0)
    {
        return 0;
    }

    if (!prnt.isValid())
    {
        return d->manager->commands()->children().count();
    }

    const GmicFilterNode* const item = static_cast<GmicFilterNode*>(prnt.internalPointer());

    return item->children().count();
}

QModelIndex GmicFilterModel::index(int row, int column, const QModelIndex& prnt) const
{
    if (
        (row    <  0)                ||
        (column <  0)                ||
        (row    >= rowCount(prnt))   ||
        (column >= columnCount(prnt))
       )
    {
        return QModelIndex();
    }

    // get the parent node

    const GmicFilterNode* const parentNode = node(prnt);

    return createIndex(row, column, parentNode->children().at(row));
}

QModelIndex GmicFilterModel::parent(const QModelIndex& ind) const
{
    if (!ind.isValid())
    {
        return QModelIndex();
    }

    const GmicFilterNode* const itemNode = node(ind);
    GmicFilterNode* const parentNode     = (itemNode ? itemNode->parent() : nullptr);

    if (!parentNode || (parentNode == d->manager->commands()))
    {
        return QModelIndex();
    }

    // get the parent's row

    const GmicFilterNode* const grandParentNode = parentNode->parent();
    int parentRow                               = grandParentNode->children().indexOf(parentNode);

    Q_ASSERT(parentRow >= 0);

    return createIndex(parentRow, 0, parentNode);
}

bool GmicFilterModel::hasChildren(const QModelIndex& prnt) const
{
    if (!prnt.isValid())
    {
        return true;
    }

    const GmicFilterNode* const parentNode = node(prnt);

    return (
            (parentNode->type() == GmicFilterNode::Folder) ||
            (parentNode->type() == GmicFilterNode::RootFolder)
           );
}

Qt::ItemFlags GmicFilterModel::flags(const QModelIndex& ind) const
{
    if (!ind.isValid())
    {
        return Qt::NoItemFlags;
    }

    Qt::ItemFlags flags                     = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    const GmicFilterNode* const commandNode = node(ind);

    if (commandNode->type() != GmicFilterNode::RootFolder)
    {
        flags |= Qt::ItemIsDragEnabled;
    }

    if (hasChildren(ind))
    {
        flags |= Qt::ItemIsDropEnabled;
    }

    return flags;
}

Qt::DropActions GmicFilterModel::supportedDropActions() const
{
    return (Qt::CopyAction | Qt::MoveAction);
}

QStringList GmicFilterModel::mimeTypes() const
{
    QStringList types;
    types << QLatin1String("application/gmicfilters.xml");

    return types;
}

QMimeData* GmicFilterModel::mimeData(const QModelIndexList& indexes) const
{
    QMimeData* const mimeData = new QMimeData();
    QByteArray ba;
    QDataStream stream(&ba, QIODevice::WriteOnly);

    for (QModelIndex id : std::as_const(indexes))
    {
        if ((id.column() != 0) || !id.isValid())
        {
            continue;
        }

        QByteArray encodedData;
        QBuffer buffer(&encodedData);
        buffer.open(QBuffer::ReadWrite);
        GmicXmlWriter writer;
        const GmicFilterNode* const parentNode = node(id);
        writer.write(&buffer, parentNode, QString());
        stream << encodedData;
    }

    mimeData->setData(QLatin1String("application/gmicfilters.xml"), ba);

    return mimeData;
}

bool GmicFilterModel::dropMimeData(const QMimeData* mdata,
                                   Qt::DropAction action,
                                   int row, int column,
                                   const QModelIndex& prnt)
{
    if (action == Qt::IgnoreAction)
    {
        return true;
    }

    if (!mdata->hasFormat(QLatin1String("application/gmicfilters.xml")) || (column > 0))
    {
        return false;
    }

    QByteArray ba = mdata->data(QLatin1String("application/gmicfilters.xml"));
    QDataStream stream(&ba, QIODevice::ReadOnly);

    if (stream.atEnd())
    {
        return false;
    }

    QUndoStack* const undoStack = d->manager->undoRedoStack();
    undoStack->beginMacro(QLatin1String("Move Filters"));

    while (!stream.atEnd())
    {
        QByteArray encodedData;
        stream >> encodedData;
        QBuffer buffer(&encodedData);
        buffer.open(QBuffer::ReadOnly);

        GmicXmlReader reader;
        QString currentPath;    // Not used.

        GmicFilterNode* const rootNode  = reader.read(&buffer, false, currentPath);

        QList<GmicFilterNode*> children = rootNode->children();

        for (int i = 0 ; i < children.count() ; ++i)
        {
            GmicFilterNode* const commandNode = children.at(i);
            rootNode->remove(commandNode);
            row                               = qMax(0, row);
            GmicFilterNode* const parentNode  = node(prnt);
            d->manager->addEntry(parentNode, commandNode, row);
            d->endMacro                       = true;
        }

        delete rootNode;
    }

    return true;
}

bool GmicFilterModel::setData(const QModelIndex& ind, const QVariant& value, int role)
{
    if (!ind.isValid())
    {
        return false;
    }

    GmicFilterNode* const item = node(ind);

    switch (role)
    {
        case Qt::DisplayRole:
        {
            if (ind.column() == 0)
            {
                d->manager->setTitle(item, value.toString());
                break;
            }

            if (ind.column() == 1)
            {
                d->manager->setComment(item, value.toString());
                break;
            }

            return false;
        }

        case GmicFilterModel::CommandRole:
        {
            d->manager->setCommand(item, value.toString());
            break;
        }

        default:
        {
            return false;
        }
    }

    return true;
}

GmicFilterNode* GmicFilterModel::node(const QModelIndex& ind) const
{
    GmicFilterNode* const itemNode = static_cast<GmicFilterNode*>(ind.internalPointer());

    if (!itemNode)
    {
        return d->manager->commands();
    }

    return itemNode;
}

} // namespace DigikamBqmGmicQtPlugin
