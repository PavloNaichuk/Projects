#include "StartProgramMenuWidget.h"
#include <QHBoxLayout>
#include <QPushButton>

StartProgramMenuWidget::StartProgramMenuWidget(QDialog* parent)
    : QDialog(parent)
{
    setWindowTitle("Меню");
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    setModal(true);

    mStartProgramStudentButton = new QPushButton("&Зайти як учень");
    mStartProgramTeacherButton = new QPushButton("&Зайти як вчитель");
    mExitProgramButton = new QPushButton("&Вихід");

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(mStartProgramStudentButton);
    layout->addWidget(mStartProgramTeacherButton);
    layout->addWidget(mExitProgramButton);
    setLayout(layout);

    resize(250, 100);
    connect(mStartProgramStudentButton, SIGNAL(clicked()), this, SLOT(startProgramStudentClicked()));
    connect(mStartProgramTeacherButton, SIGNAL(clicked()), this, SLOT(startProgramTeacherClicked()));
    connect(mExitProgramButton, SIGNAL(clicked()), this, SLOT(exitProgramClicked()));
}

StartProgramMenuWidget::~StartProgramMenuWidget()
{
    delete mStartProgramStudentButton;
    delete mStartProgramTeacherButton;
    delete mExitProgramButton;
}

void StartProgramMenuWidget::startProgramStudentClicked()
{
    emit startProgramStudent();
    close();
}
void StartProgramMenuWidget::startProgramTeacherClicked()
{
    emit startProgramTeacher();
    close();
}

void StartProgramMenuWidget::exitProgramClicked()
{
    emit exitProgram();
    close();
}
