#ifndef TEACHERWIDGET_H
#define TEACHERWIDGET_H
#include "TableModel.h"

#include <QWidget>
class QTableView;
class QPushButton;

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


public slots:
    void backClicked();
    void contexMenuClicked(const QPoint pos);
    void openClicked();
    void removeClicked();

private:
    QPushButton* mBackButton;
    TableModel* mTableModel;
    QTableView* mTableView;
};

#endif // TEACHERWIDGET_H
