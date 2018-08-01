#pragma once

#include <QAbstractTableModel>
#include <memory>
#include "CompositeComponent.h"

class TableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    TableModel(std::unique_ptr<CompositeComponent> compositeComponent);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;

    void insertComponent(Type type);
    bool addComponent(Type type);

private:
    std::unique_ptr<CompositeComponent> mCompositeComponent;  
};
