#include "StudyTests.h"
#include "MainWidget.h"
#include "StudentWidget.h"
#include "TeacherWidget.h"
#include "StartProgramMenuWidget.h"
#include "StudentEnterMenuWidget.h"
#include "TeacherEnterMenuWidget.h"
#include "CategoryWidget.h"
#include "TestWidget.h"

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
   // mMainWidget->setStudent();
    mStudentWidget = new StudentWidget();
    mStudentWidget->show();
     mMainWidget->close();
}
void StudyTests::teacherEnter()
{
    //mMainWidget->setTeacher();
    mTeacherWidget = new TeacherWidget();
    mTeacherWidget->show();
    mMainWidget->close();

}

void StudyTests::addCategory()
{
    mCategoryWidget = new CategoryWidget();
    mCategoryWidget->show();
    mMainWidget->close();
}



void StudyTests::addTest()
{
    mTestWidget = new TestWidget();
    mTestWidget->show();
    mMainWidget->close();
}

StudyTests::~StudyTests()
{
    delete mMainWidget;
}
