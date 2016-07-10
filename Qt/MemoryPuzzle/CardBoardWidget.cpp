#include "CardBoardWidget.h"
#include "CardBoard.h"
#include <QPainter>
#include <QDesktopWidget>
#include <QApplication>
#include <QMouseEvent>
#include <QDebug>

#include "GameLogic.h"
#include "GameState.h"

CardBoardWidget::CardBoardWidget(QWidget* parent)
    : QWidget(parent)
    , mGameLogic(new GameLogic())
    , mTimer(new QTimer())
{
    setWindowTitle("Memory Puzzle");
    setGeometry(QRect(0, 0, Width, Height));

    QDesktopWidget* screenGeometry = QApplication::desktop();
    int x = (screenGeometry->width() - width()) / 2;
    int y = (screenGeometry->height() - height()) / 2;

    move(x, y);

    mGameLogic->setCardBoardWidth(Width);
    mGameLogic->setCardBoardHeight(Height);

    connect(mTimer, SIGNAL(timeout()), this, SLOT(updateGameLogic()));
    mTimer->start(100);
}

CardBoardWidget::~CardBoardWidget()
{
    mTimer->stop();
    delete mTimer;
    delete mGameLogic;
}

const CardBoard* CardBoardWidget::getCardBoard() const
{
    return mGameLogic->getCardBoard();
}

void CardBoardWidget::setCardBoard(CardBoard* cardBoard)
{
    mGameLogic->setCardBoard(cardBoard);
}

void CardBoardWidget::paintEvent(QPaintEvent* paintEvent)
{
    QPainter painter(this);
    painter.setFont(QFont("Arial", 50));

    CardBoard* cardBoard = mGameLogic->getCardBoard();
    for (int row = 0; row < CardBoard::kNumRows; ++row)
    {
        for (int col = 0; col < CardBoard::kNumCols; ++col)
        {
            const Card& card = cardBoard->GetCard(row, col);
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
    CardBoard* cardBoard = mGameLogic->getCardBoard();
    cardBoard->resizeBoard(width(), height());

    mGameLogic->setCardBoardWidth(width());
    mGameLogic->setCardBoardHeight(height());
}

void CardBoardWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        GameState* currentState = mGameLogic->getCurrentState();
        currentState->handleMousePress(event->x(), event->y());
    }
}

void CardBoardWidget::updateGameLogic()
{
    GameState* currentState = mGameLogic->getCurrentState();
    currentState->update();

    if (mGameLogic->redrawBoard())
    {
        update();
        mGameLogic->setRedrawBoard(false);
    }
}

