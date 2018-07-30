#ifndef STUDYTESTS_H
#define STUDYTESTS_H

#include <QObject>

class MainWidget;
class StudentEnterMenuWidget;
class TeacherEnterMenuWidget;
class StudentWidget;
class TeacherWidget;
class CategoryWidget;
class TestWidget;

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
    void addCategory();
    void addTest();


private:
    MainWidget* mMainWidget;
    StudentEnterMenuWidget* mStudentEnterMenuWidget;
    TeacherEnterMenuWidget* mTeacherEnterMenuWidget;
    StudentWidget* mStudentWidget;
    TeacherWidget* mTeacherWidget;
    CategoryWidget* mCategoryWidget;
    TestWidget* mTestWidget;
};

#endif // STUDYTESTS_H
