#include "AddTestWidget.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QGroupBox>

AddTestWidget::AddTestWidget(QWidget* parent)
    : QWidget(parent)
{
    setWindowTitle("Додати тест");
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);





    resize(250, 100);
    connect(mAddTestEnterButton, SIGNAL(clicked()), this, SLOT(addTestEnterClicked()));
}

AddTestWidget::~AddTestWidget()
{
    delete mAddTestEnterButton;
}

void AddTestWidget::addTestEnterClicked()
{
    emit addTestEnter();
    close();
}
