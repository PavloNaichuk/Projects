#ifndef MEMORYPUZZLE_H
#define MEMORYPUZZLE_H

#include <QObject>

class MainWidget;
class StudentEnterMenuWidget;

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

private:
    MainWidget* mMainWidget;
    StudentEnterMenuWidget* mStudentEnterMenuWidget;
};

#endif // MEMORYPUZZLE_H
