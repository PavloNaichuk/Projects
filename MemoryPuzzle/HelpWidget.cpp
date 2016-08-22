#include "HelpWidget.h"

HelpWidget::HelpWidget(QMessageBox* parent)
    : QMessageBox(parent)
{
    setWindowTitle("Help");
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    setModal(true);
    setText("bla bla bla");
}

HelpWidget::~HelpWidget()
{
}

