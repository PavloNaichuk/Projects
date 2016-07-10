#include "SelectFirstCardState.h"
#include "SelectSecondCardState.h"
#include "GameLogic.h"
#include "CardBoard.h"

SelectFirstCardState::SelectFirstCardState(GameLogic* gameLogic)
    : mGameLogic(gameLogic)
{
}

void SelectFirstCardState::enter()
{
    mGameLogic->setSelectedFirstCard(nullptr);
}

void SelectFirstCardState::update()
{
    if (mGameLogic->getSelectedFirstCard() != nullptr)
    {
        mGameLogic->getSelectedFirstCard()->setOpened(true);
        mGameLogic->setCurrentState(mGameLogic->getSelectSecondCardState());
    }
}

void SelectFirstCardState::exit()
{
     mGameLogic->setRedrawBoard(true);
}

void SelectFirstCardState::handleMousePress(int x, int y)
{
    int cellWidth = mGameLogic->getCardBoardWidth() / CardBoard::kNumCols;
    int cellHeight = mGameLogic->getCardBoardHeight() / CardBoard::kNumRows;

    int col = floor(x / cellWidth);
    int row = floor(y / cellHeight);

    CardBoard* cardBoard = mGameLogic->getCardBoard(); 
    mGameLogic->setSelectedFirstCard(&cardBoard->GetCard(row, col));
}

