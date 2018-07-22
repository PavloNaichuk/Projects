#ifndef ADDTESTWIDGET_H
#define ADDTESTWIDGET_H

#include <QWidget>

class QPushButton;
class QLineEdit;

class AddTestWidget : public QWidget
{
    Q_OBJECT

public:
    AddTestWidget(QWidget *parent = 0);
    ~AddTestWidget();

signals:
    void addTestEnter();
public slots:
    void addTestEnterClicked();

private:
    QPushButton* mAddTestEnterButton;
};


#endif // ADDTESTWIDGET_H
