#ifndef TABLEMODEL_H
#define TABLEMODEL_H
#include <QAbstractTableModel>
#include <QTime>

class TableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    TableModel(QObject* parent);
    int rowCount(const QModelIndex& parent = QModelIndex()) const ;
    int columnCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
};
#endif // TABLEMODEL_H
