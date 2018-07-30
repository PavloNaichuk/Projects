#include "TableModel.h"
#include <QStandardItem>
#include "Component.h"
#include "CategoryWidget.h"
#include "TestWidget.h"


TableModel::TableModel(std::unique_ptr<CompositeComponent> compositeComponent)
    : mCompositeComponent(std::move(compositeComponent))
{
}

int TableModel::rowCount(const QModelIndex& /*parent*/) const
{
   return (int)mCompositeComponent->numChildren();
}

int TableModel::columnCount(const QModelIndex& /*parent*/) const
{
    return 3;
}

QVariant TableModel::data(const QModelIndex& index, int role) const
{
    const int row = index.row();
    const int col = index.column();

    if (role == Qt::DisplayRole)
    {
        const auto& components = mComponents.at(index.row());

        if (index.column() == 0)
        {
            //const Component* component = mCompositeComponent->child(row);
            //return QString(component->name());
            return components.name;
        }
        else if (index.column() == 1)
            return components.type;
        else if (index.column() == 2)
            return components.data;
    }

    return QVariant();
}


bool TableModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (index.isValid() && role == Qt::EditRole)
    {
        int row = index.row();
        auto component = mComponents.value(row);

        if (index.column() == 0)
            component.name = value.toString();
        else if (index.column() == 1)
            component.type = value.toString();
        else if (index.column() == 2)
            component.data = value.toString();
        else
            return false;

        mComponents.replace(row, component);
        emit dataChanged(index, index);

        return true;
    }
    return false;
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            switch (section)
            {
                case 0:
                    return QString("Назва");
                case 1:
                    return QString("Тип");
                case 2:
                    return QString("Дата");
            }
        }
    }
    return QVariant();
}

bool TableModel::insertRows(int row, int count, const QModelIndex& index/*parent*/)
{
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), count, count + row - 1);

    for (int index = 0; index < row; ++index)
    {
            if( mComponent->type() == CATEGORY)
            {
                mComponents.insert(count, { QString("Нова категорія"), });
            }

            if( mComponent->type() == TEST)
            {
                mComponents.insert(count, { QString("Новий  тест"), });
            }

    }

        endInsertRows();
        return true;
}



bool TableModel::removeRows(int row, int count,const QModelIndex& /*parent*/)
{
    beginRemoveRows(QModelIndex(), row, row + count - 1);

    for(int index = 0; index < count; ++index)
        mCompositeComponent->removeChild(index);

    endRemoveRows();
    return true;
}

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}
