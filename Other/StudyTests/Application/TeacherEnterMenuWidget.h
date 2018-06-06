#ifndef TEACHERENTERWIDGET_H
#define TEACHERENTERWIDGET_H
#include <QDialog>

class QPushButton;
class QLineEdit;

class TeacherEnterMenuWidget : public QDialog
{
    Q_OBJECT

public:
    TeacherEnterMenuWidget(QDialog* parent = 0);
    ~TeacherEnterMenuWidget();

signals:
    void teacherEnter();
public slots:
    void teacherEnterClicked();

private:
    QLineEdit* mTeacherLoginEdit;
    QLineEdit* mTeacherPaswordEdit;
    QPushButton* mEnterButton;
};
#endif // TEACHERENTERWIDGET_H
