#include "TableModel.h"
#include <QStandardItem>
#include "Component.h"
#include "CategoryWidget.h"
#include "TestWidget.h"
#include <cassert>


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

    const Component* component = mCompositeComponent->child(row);
    if (role == Qt::DisplayRole)
    {
        if (col == 0)
            return QString(component->name());
    }
    return QVariant();
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

bool TableModel::removeRows(int row, int count,const QModelIndex& /*parent*/)
{
    beginRemoveRows(QModelIndex(), row, row + count - 1);

    for(int index = 0; index < count; ++index)
        mCompositeComponent->removeChild(index);

    endRemoveRows();
    return true;
}

void TableModel::insertComponent(Type type)
{
    const int row = rowCount();
    beginInsertRows(QModelIndex(), row, row);

    std::unique_ptr<Component> component;
    if (type == CATEGORY)
        component.reset(new Category("Нова категорія"));
    else if (type == TEST)
        component.reset(new Test("Новий  тест"));
    else
        assert(false && "Unknown type");

    mCompositeComponent->addChild(std::move(component));
    endInsertRows();
}

bool TableModel::openComponent(Type type)
{
    return type == CATEGORY;
}
