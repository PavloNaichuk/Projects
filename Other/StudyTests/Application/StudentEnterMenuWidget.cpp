#include "StudentEnterMenuWidget.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QGroupBox>

StudentEnterMenuWidget::StudentEnterMenuWidget(QDialog* parent)
    : QDialog(parent)
{
    setWindowTitle("Вхід учня");
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    setModal(true);

    QGroupBox* loginGroup = new QGroupBox(tr("Введіть прізвище та ім'я"));
    mStudentLoginEdit = new QLineEdit;
    mStudentLoginEdit->setFocus();
    mEnterButton = new QPushButton("&Зайти");

    QGridLayout *qlayout = new QGridLayout;
    qlayout->addWidget(mStudentLoginEdit, 0, 0);
    loginGroup->setLayout(qlayout);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(loginGroup, 0, 0);
    layout->addWidget(mEnterButton, 1, 0);
    setLayout(layout);

    resize(250, 100);
    connect(mEnterButton, SIGNAL(clicked()), this, SLOT(studentEnterClicked()));
}

StudentEnterMenuWidget::~StudentEnterMenuWidget()
{
    delete mEnterButton;
}

void StudentEnterMenuWidget::studentEnterClicked()
{
    emit studentEnter();
    close();
}
