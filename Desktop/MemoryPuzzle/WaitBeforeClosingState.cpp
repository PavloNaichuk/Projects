#include "WaitBeforeClosingState.h"
#include "SelectFirstCardState.h"
#include "GameLogic.h"
#include "Card.h"

WaitBeforeClosingState::WaitBeforeClosingState(GameLogic* gameLogic)
    : mGameLogic(gameLogic)
{
    mTime.start();
}

void WaitBeforeClosingState::enter()
{
    mTime.restart();
}

void WaitBeforeClosingState::update()
{
    if (mTime.elapsed() > 500)
    {
        mGameLogic->getSelectedFirstCard()->setState(Card::Closed);
        mGameLogic->setSelectedFirstCard(nullptr);

        mGameLogic->getSelectedSecondCard()->setState(Card::Closed);
        mGameLogic->setSelectedSecondCard(nullptr);

        mGameLogic->setCurrentState(mGameLogic->getSelectFirstCardState());
    }
}

void WaitBeforeClosingState::exit()
{
     mGameLogic->setRedrawBoard(true);
}

void WaitBeforeClosingState::handleMousePress(int /*x*/, int /*y*/)
{
}
