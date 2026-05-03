#include "HelpWidget.h"

HelpWidget::HelpWidget(QMessageBox* parent)
    : QMessageBox(parent)
{
    setWindowTitle("Help");
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    setModal(true);
    setText("A 4x4 board is full of overturned cards. There is a pair for each card. "
            "The player flips over two cards. "
            "If they match, then they stay overturned. Otherwise they flip back. "
            "The player needs to overturn all the cards in the fewest moves to win.");
}

HelpWidget::~HelpWidget()
{
}

