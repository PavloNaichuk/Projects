#ifndef TESTWIDGET_H
#define TESTWIDGET_H
#include "Test.h"

#include <QWidget>

class QPushButton;

class TestWidget : public QWidget
{
    Q_OBJECT

public:
    TestWidget(QWidget* parent = 0);
    ~TestWidget();

signals:
    void save();
public slots:
    void saveClicked();

private:
    QPushButton* mSaveButton;
};


#endif // TESTWIDGET_H
