#include "SelectSecondCardState.h"
#include "SelectFirstCardState.h"
#include "WaitBeforeClosingState.h"
#include <cassert>
#include "GameLogic.h"
#include "CardBoard.h"

SelectSecondCardState::SelectSecondCardState(GameLogic* gameLogic)
    : mGameLogic(gameLogic)
{
}

void SelectSecondCardState::enter()
{
    mGameLogic->setSelectedSecondCard(nullptr);
}

void SelectSecondCardState::update()
{
    Card* secondCard = mGameLogic->getSelectedSecondCard();
    if (secondCard != nullptr)
    {
        Card* firstCard = mGameLogic->getSelectedFirstCard();
        assert(firstCard != nullptr);

        secondCard->setOpened(true);
        if (firstCard->getValue() == secondCard->getValue())
        {
            mGameLogic->setCurrentState(mGameLogic->getSelectFirstCardState());
        }
        else
        {
            mGameLogic->setCurrentState(mGameLogic->getWaitBeforeClosingState());
        }
    }
}

void SelectSecondCardState::exit()
{
     mGameLogic->setRedrawBoard(true);
}

void SelectSecondCardState::handleMousePress(int x, int y)
{
    int cellWidth = mGameLogic->getCardBoardWidth() / CardBoard::kNumCols;
    int cellHeight = mGameLogic->getCardBoardHeight() / CardBoard::kNumRows;

    int col = floor(x / cellWidth);
    int row = floor(y / cellHeight);

    CardBoard* cardBoard = mGameLogic->getCardBoard();

    Card* secondCard = &cardBoard->GetCard(row, col);
    if (secondCard != mGameLogic->getSelectedFirstCard())
        mGameLogic->setSelectedSecondCard(secondCard);
}
