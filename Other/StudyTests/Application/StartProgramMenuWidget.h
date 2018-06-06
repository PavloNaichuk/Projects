#ifndef STARTPROGRAMMENUWIDGET_H
#define STARTPROGRAMMENUWIDGET_H

#include <QDialog>

class QPushButton;

class StartProgramMenuWidget : public QDialog
{
    Q_OBJECT

public:
    StartProgramMenuWidget(QDialog* parent = 0);
    ~StartProgramMenuWidget();

signals:
    void startProgramStudent();
    void startProgramTeacher();
    void exitProgram();

public slots:
    void startProgramStudentClicked();
    void startProgramTeacherClicked();
    void exitProgramClicked();

private:
    QPushButton* mStartProgramStudentButton;
    QPushButton* mStartProgramTeacherButton;
    QPushButton* mExitProgramButton;

};

#endif // STARTPROGRAMMENUWIDGET_H
