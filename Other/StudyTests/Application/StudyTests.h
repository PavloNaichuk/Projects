#ifndef STUDYTESTS_H
#define STUDYTESTS_H

#include <QObject>

class MainWidget;
class StudentEnterMenuWidget;
class TeacherEnterMenuWidget;
class StudentWidget;
class TeacherWidget;
class AddTestWidget;

class StudyTests : public QObject
{
    Q_OBJECT

public:
    ~StudyTests();
    int runGame();

private slots:
    void startProgramStudent();
    void startProgramTeacher();
    void exitProgram();
    void studentEnter();
    void teacherEnter();
    void addTest();
    void editTest();
    void removeTest();
    void addTestEnter();
    void editTestEnter();
    void removeTestEnter();

private:
    MainWidget* mMainWidget;
    StudentEnterMenuWidget* mStudentEnterMenuWidget;
    TeacherEnterMenuWidget* mTeacherEnterMenuWidget;
    StudentWidget* mStudentWidget;
    TeacherWidget* mTeacherWidget;
    AddTestWidget* mAddTestWidget;
};

#endif // STUDYTESTS_H
