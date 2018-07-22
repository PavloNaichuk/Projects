#include "StudentWidget.h"


StudentWidget::StudentWidget(QWidget* parent)
    : QWidget(parent)
{
    setWindowTitle("Виконання тестів");
    setWindowModality(Qt::WindowModal);
    resize(800, 600);
}

StudentWidget::~StudentWidget()
{
}
