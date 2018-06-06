#include "MainWidget.h"
#include "StudentWidget.h"
#include "TeacherWidget.h"

#include <QStackedWidget>
#include <QVBoxLayout>
#include <QLayout>

MainWidget::MainWidget(QWidget* parent)
    : QWidget(parent)
    , mStudentWidget (new StudentWidget(this))
    , mTeacherWidget(new TeacherWidget(this))
{
    setWindowTitle("Тести");
    setWindowModality(Qt::WindowModal);
    resize(800, 600);
}

MainWidget::~MainWidget()
{
    delete mStudentWidget;
    delete mTeacherWidget;
}
void MainWidget::setStudent()
{
    mStudentWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QVBoxLayout* vLayout = new QVBoxLayout;
    vLayout->addWidget(mStudentWidget);

    setLayout(vLayout);
}
void MainWidget::setTeacher()
{
    mTeacherWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QVBoxLayout* vLayout = new QVBoxLayout;
    vLayout->addWidget(mTeacherWidget);

    setLayout(vLayout);
}
