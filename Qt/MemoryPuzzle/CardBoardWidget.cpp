#include "CardBoardWidget.h"
#include "CardBoard.h"
#include <QPainter>
#include <QDesktopWidget>
#include <QApplication>
#include <QMouseEvent>
#include <QDebug>

#include "GameLogic.h"
#include "GameState.h"
#include "ClosedCardRenderer.h"
#include "OpenedCardRenderer.h"
#include "SelectedCardRenderer.h"

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

    mCardRenderers[Card::Closed] = new ClosedCardRenderer();
    mCardRenderers[Card::Opened] = new OpenedCardRenderer();
    mCardRenderers[Card::Selected] = new SelectedCardRenderer(mCardRenderers[Card::Opened]);
}

CardBoardWidget::~CardBoardWidget()
{
    mTimer->stop();
    delete mTimer;
    delete mGameLogic;
    delete mCardRenderers[Card::Closed];
    delete mCardRenderers[Card::Selected];
    delete mCardRenderers[Card::Opened];
}

const CardBoard* CardBoardWidget::getCardBoard() const
{
    return mGameLogic->getCardBoard();
}

void CardBoardWidget::setCardBoard(CardBoard* cardBoard)
{
    mGameLogic->setCardBoard(cardBoard);
}

void CardBoardWidget::paintEvent(QPaintEvent* /*paintEvent*/)
{
    QPainter painter(this);
    painter.setFont(QFont("Arial", 50));
    painter.setPen(QPen(Qt::black, 4, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));

    CardBoard* cardBoard = mGameLogic->getCardBoard();
    for (int row = 0; row < CardBoard::kNumRows; ++row)
    {
        for (int col = 0; col < CardBoard::kNumCols; ++col)
        {
            const Card& card = cardBoard->GetCard(row, col);
            mCardRenderers[card.getState()]->Render(&painter, &card);
        }
    }
    painter.end();
}

void CardBoardWidget::resizeEvent(QResizeEvent* /*event*/)
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

