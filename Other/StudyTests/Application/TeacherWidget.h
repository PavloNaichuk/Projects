#ifndef TEACHERWIDGET_H
#define TEACHERWIDGET_H
#include "TableModel.h"


#include <QWidget>
class QTableView;
class QPushButton;

class CategoryWidget;
class TestWidget;
class Component;

class TeacherWidget : public QWidget
{
    Q_OBJECT

public:
    TeacherWidget(QWidget* parent = 0);
    ~TeacherWidget();

signals:
    void back();
    void open();
    void remove();
    void addCategory();
    void addTest();
    void sorted();


public slots:
    void backClicked();
    void contexMenuClicked(const QPoint pos);
    void contexMenuComponent(const QPoint pos);

    void openClicked();
    void removeClicked();
    void categoryClicked();
    void testClicked();
    void sortedClicked();


private:
    QPushButton* mBackButton;
    TableModel* mTableModel;
    QTableView* mTableView;
    Component* mComponent;
    CategoryWidget* mCategoryWidget;
    TestWidget* mTestWidget;


};

#endif // TEACHERWIDGET_H
