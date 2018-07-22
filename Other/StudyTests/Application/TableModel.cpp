#include "TableModel.h"

TableModel::TableModel(QObject *parent)
    :QAbstractTableModel(parent)
{
}

int TableModel::rowCount(const QModelIndex& /*parent*/) const
{
   return 3;
}

int TableModel::columnCount(const QModelIndex& /*parent*/) const
{
    return 3;
}

QVariant TableModel::data(const QModelIndex& index, int role) const
{
    int row = index.row();
    int col = index.column();

    if (role == Qt::DisplayRole)
    {
        if (row == 1 && col == 2)
        {
            return QTime::currentTime().toString();
        }
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
