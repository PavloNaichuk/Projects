#ifndef STUDENTENTERMENUWIDGET_H
#define STUDENTENTERMENUWIDGET_H
#include <QDialog>

class QPushButton;
class QLineEdit;

class StudentEnterMenuWidget : public QDialog
{
    Q_OBJECT

public:
    StudentEnterMenuWidget(QDialog* parent = 0);
    ~StudentEnterMenuWidget();

signals:
    void studentEnter();
public slots:
    void studentEnterClicked();

private:
    QLineEdit* mStudentLoginEdit;
    QPushButton* mEnterButton;
};
#endif // STUDENTENTERMENUWIDGET_H
