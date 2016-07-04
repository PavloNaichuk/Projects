#include "CardBoardWidget.h"
#include <QPainter>
#include <QDesktopWidget>
#include <QApplication>
#include <QMouseEvent>

#include <QDebug>

CardBoardWidget::CardBoardWidget(QWidget* parent)
    : QWidget(parent)
    , mCardBoard(nullptr)
{
    setWindowTitle("Memory Puzzle");
    setGeometry(QRect(0, 0, Width, Height));

    QDesktopWidget* screenGeometry = QApplication::desktop();
    int x = (screenGeometry->width() - width()) / 2;
    int y = (screenGeometry->height() - height()) / 2;
    move(x, y);
}

const CardBoard* CardBoardWidget::getCardBoard() const
{
    return mCardBoard;
}

void CardBoardWidget::setCardBoard(CardBoard* cardBoard)
{
    mCardBoard = cardBoard;
}

void CardBoardWidget::paintEvent(QPaintEvent* paintEvent)
{
    QPainter painter(this);
    painter.setFont(QFont("Arial", 50));

    for (int row = 0; row < CardBoard::kNumRows; ++row)
    {
        for (int col = 0; col < CardBoard::kNumCols; ++col)
        {
            const Card& card = mCardBoard->GetCard(row, col);
            if (card.isOpened())
            {
                painter.fillRect(card.getRect(), Qt::white);
                painter.drawText(card.getRect(), Qt::AlignCenter, QString(card.getValue()));
            }
            else
            {
                painter.fillRect(card.getRect(), card.getColor());
            }
        }
    }
    painter.end();
}

void CardBoardWidget::resizeEvent(QResizeEvent* event)
{
    mCardBoard->resizeBoard(width(), height());
}

void CardBoardWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        int cellWidth = Width / CardBoard::kNumCols;
        int cellHeight = Height / CardBoard::kNumRows;

        int col = floor(event->x() / cellWidth);
        int row = floor(event->y() / cellHeight);

        Card& card = mCardBoard->GetCard(row, col);
        card.setOpened(true);

        update();
    }
}

