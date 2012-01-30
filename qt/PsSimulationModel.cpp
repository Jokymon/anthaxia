#include "PsSimulationModel.h"
#include "model/SimulationObject.h"

PsSimulationModel::PsSimulationModel(QObject *parent)
: QAbstractItemModel(parent)
, root(0)
{ }

QVariant PsSimulationModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    SimulationObject* item = static_cast<SimulationObject*>(index.internalPointer());

    return QVariant( QString(item->getName().c_str()) );
}

QModelIndex PsSimulationModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
    {
        return QModelIndex();
    }

    SimulationObject* parentItem = 0;

    if (!parent.isValid())
    {
        parentItem = root;
    }
    else
    {
        parentItem = static_cast<SimulationObject*>(parent.internalPointer());
    }

    if (!parentItem)
    {
        return QModelIndex();
    }

    SimulationObject* childItem = parentItem->getChild(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex PsSimulationModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return QModelIndex();
    }

    SimulationObject* childItem = static_cast<SimulationObject*>(index.internalPointer());
    SimulationObject* parentItem = childItem->getParent();

    if (parentItem == root)
    {
        return QModelIndex();
    }

    return createIndex(parentItem->parentIndex(), 0, parentItem);
}

int PsSimulationModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

int PsSimulationModel::rowCount(const QModelIndex &parent) const
{
    SimulationObject* parentItem;
    if (parent.column() > 0)
    {
        return 0;
    }

    if (!parent.isValid())
    {
        parentItem = root;
    }
    else
    {
        parentItem = static_cast<SimulationObject*>(parent.internalPointer());
    }

    if (!parentItem)
    {
        return 0;
    }

    return parentItem->childCount();
}
    
Qt::ItemFlags PsSimulationModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

void PsSimulationModel::setRootObject(SimulationObject* _root)
{
    root = _root;
}

#include "PsSimulationModel.moc"
