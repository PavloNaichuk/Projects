#pragma once

#include <QAbstractTableModel>
#include <QTableView>
#include <memory>
#include "CompositeComponent.h"
#include "Component.h"


class CategoryWidget;
class TestWidget;

struct Components
{
    QString name;
    QString type;
    QString data;

};

class TableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    TableModel(std::unique_ptr<CompositeComponent> compositeComponent);
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    bool insertRows(int row, int count, const QModelIndex& index = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

private:
    QTableView* mTableView;
    std::unique_ptr<CompositeComponent> mCompositeComponent;
    Component* mComponent;
    CategoryWidget* mCategoryWidget;
    TestWidget* mTestWidget;
    QList<Components> mComponents;
};
