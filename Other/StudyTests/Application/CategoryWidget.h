#ifndef CATEGORYWIDGET_H
#define CATEGORYWIDGET_H
#include "Category.h"
#include "Test.h"

#include <QWidget>

class QPushButton;

class CategoryWidget : public QWidget
{
    Q_OBJECT

public:
    CategoryWidget(QWidget* parent = 0);
    ~CategoryWidget();

signals:
    void save();
public slots:
    void saveClicked();

private:
    QPushButton* mSaveButton;
    Category* mCategory;
};

#endif // CATEGORYWIDGET_H
