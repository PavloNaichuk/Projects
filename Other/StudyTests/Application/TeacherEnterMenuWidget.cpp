#include "TeacherEnterMenuWidget.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QGroupBox>

TeacherEnterMenuWidget::TeacherEnterMenuWidget(QDialog* parent)
    : QDialog(parent)
{
    setWindowTitle("Вхід вчителя");
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    setModal(true);


    mEnterButton = new QPushButton("&Зайти");

    QGroupBox* loginGroup = new QGroupBox(tr("Введіть прізвище та ім'я"));
    mTeacherLoginEdit = new QLineEdit;
    mTeacherLoginEdit->setFocus();

    QGroupBox* paswordGroup = new QGroupBox(tr("Введіть пароль"));
    mTeacherPaswordEdit = new QLineEdit;
    mTeacherPaswordEdit->setFocus();

    mEnterButton = new QPushButton("&Зайти");


    QGridLayout* loginlayout = new QGridLayout;
    loginlayout->addWidget(mTeacherLoginEdit);
    loginGroup->setLayout(loginlayout);

    QGridLayout* paswordlayout = new QGridLayout;
    paswordlayout->addWidget(mTeacherPaswordEdit);
    paswordGroup->setLayout(paswordlayout);

    QGridLayout*layout = new QGridLayout;
    layout->addWidget(loginGroup, 0, 0);
    layout->addWidget(paswordGroup, 1, 0);
    layout->addWidget(mEnterButton, 2, 0);
    setLayout(layout);

    resize(250, 100);
    connect(mEnterButton, SIGNAL(clicked()), this, SLOT(teacherEnterClicked()));
}

TeacherEnterMenuWidget::~TeacherEnterMenuWidget()
{
    delete mEnterButton;
}

void TeacherEnterMenuWidget::teacherEnterClicked()
{
    emit teacherEnter();
    close();
}
