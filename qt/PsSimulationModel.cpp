#include "PsSimulationModel.h"
#include "model/SimulationObject.h"

class ModelContainer : public SimulationObject
{
public:
    ModelContainer(SimulationObject& _root)
    : SimulationObject("ModelContainerRoot", SO_SOC, NULL)
    , root(_root)
    {
        // TODO: reset parent of this child to 0 when the ModelContainer disappears
        _root.setParent(this);
    }

    virtual int parentIndex()
    {
        return 0;
    }

    virtual SimulationObject* getChild(int index)
    {
        if (index!=0)
        {
            return NULL;
        }
        return &root;
    }

    virtual int childCount()
    {
        return 1;
    }

    virtual void accept(SimulationObjectVisitor& visitor)
    {
        // pass
    }

private:
    SimulationObject& root;
};

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
        // Security check: In our model we only have one column. If we are
        // asked for a column >0 then something is wrong and we return 0 to say
        // there are no children for this item
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
    root = new ModelContainer(*_root);
}

#include "PsSimulationModel.moc"
