#include "StudyTests.h"
#include "MainWidget.h"
#include "StudentWidget.h"
#include "TeacherWidget.h"
#include "StartProgramMenuWidget.h"
#include "StudentEnterMenuWidget.h"
#include "TeacherEnterMenuWidget.h"

int StudyTests::runGame()
{
    mMainWidget = new MainWidget();
    mMainWidget->show();

    StartProgramMenuWidget startProgramMenu;
    connect(&startProgramMenu, SIGNAL(startProgramStudent()), this, SLOT(startProgramStudent()));
    connect(&startProgramMenu, SIGNAL(startProgramTeacher()), this, SLOT(startProgramTeacher()));
    connect(&startProgramMenu, SIGNAL(exitProgram()), this, SLOT(exitProgram()));
    startProgramMenu.exec();


    return 0;
}

void StudyTests::startProgramStudent()
{
    StudentEnterMenuWidget studentEnterMenu;
    connect(&studentEnterMenu, SIGNAL(studentEnter()), this, SLOT(studentEnter()));
    studentEnterMenu.exec();


}
void StudyTests::startProgramTeacher()
{
    TeacherEnterMenuWidget teacherEnterMenu;
    connect(&teacherEnterMenu, SIGNAL(teacherEnter()), this, SLOT(teacherEnter()));
    teacherEnterMenu.exec();
}

void StudyTests::exitProgram()
{
    mMainWidget->close();
}
void StudyTests::studentEnter()
{
   // mStudentEnterMenuWidget->enter();
}
void StudyTests::teacherEnter()
{
   // mStudentEnterMenuWidget->enter();
}

StudyTests::~StudyTests()
{
    delete mMainWidget;

}
