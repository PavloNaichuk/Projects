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
       const Component* component = mCompositeComponent->child(row);
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
    if(type  == CATEGORY)
    {
        const int row = rowCount();
        beginInsertRows(QModelIndex(), row, row);

        std::unique_ptr<Category> category(new Category("Нова категорія"));
        mCompositeComponent->addChild(std::move(category));
        //this->insertRows(this->rowCount(), 1);

        endInsertRows();
    }

    if(type == TEST)
    {
        const int row = rowCount();
        beginInsertRows(QModelIndex(), row, row);
        std::unique_ptr<Test> test(new Test("Новий  тест"));
        mCompositeComponent->addChild(std::move(test));
        //this->insertRows(this->rowCount(), 1);
        endInsertRows();
    }
}

bool TableModel::addComponent(Type type)
{
    if(type  == CATEGORY)
    {
        return true;
    }

    if(type == TEST)
    {
        return false;
    }
}
