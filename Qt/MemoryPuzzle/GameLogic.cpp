#include "GameLogic.h"
#include "SelectFirstCardState.h"
#include "SelectSecondCardState.h"
#include "WaitBeforeClosingState.h"

GameLogic::GameLogic()
    : mCardBoard(nullptr)
    , mCardBoardWidth(0)
    , mCardBoardHeight(0)
    , mRedrawBoard(false)
    , mSelectedFirstCard(nullptr)
    , mSelectedSecondCard(nullptr)
    , mSelectFirstCardState(nullptr)
    , mSelectSecondCardState(nullptr)
    , mWaitBeforeClosingState(nullptr)
    , mCurrentState(nullptr)
    , mTotalScore(0)
    , mNumAttempts(0)
{
    mSelectFirstCardState = new SelectFirstCardState(this);
    mSelectSecondCardState = new SelectSecondCardState(this);
    mWaitBeforeClosingState = new WaitBeforeClosingState(this);
    mCurrentState = mSelectFirstCardState;

    connect(mSelectSecondCardState, SIGNAL(selectionChanged(bool)), this, SLOT(selectionCompleted(bool)));
}

GameLogic::~GameLogic()
{
    delete mSelectFirstCardState;
    delete mSelectSecondCardState;
    delete mWaitBeforeClosingState;
}

CardBoard* GameLogic::getCardBoard()
{
    return mCardBoard;
}

void GameLogic::setCardBoard(CardBoard* cardBoard)
{
    mCardBoard = cardBoard;
}

int GameLogic::getCardBoardWidth() const
{
    return mCardBoardWidth;
}

void GameLogic::setCardBoardWidth(int cardBoardWidth)
{
    mCardBoardWidth = cardBoardWidth;
}

int GameLogic::getCardBoardHeight() const
{
    return mCardBoardHeight;
}

void GameLogic::setCardBoardHeight(int cardBoardHeight)
{
    mCardBoardHeight = cardBoardHeight;
}

bool GameLogic::redrawBoard() const
{
    return mRedrawBoard;
}

void GameLogic::setRedrawBoard(bool redrawBoard)
{
    mRedrawBoard = redrawBoard;
}

Card* GameLogic::getSelectedFirstCard()
{
    return mSelectedFirstCard;
}

void GameLogic::setSelectedFirstCard(Card* card)
{
    mSelectedFirstCard = card;
}

Card* GameLogic::getSelectedSecondCard()
{
    return mSelectedSecondCard;
}

void GameLogic::setSelectedSecondCard(Card* card)
{
    mSelectedSecondCard = card;
}

GameState* GameLogic::getCurrentState()
{
    return mCurrentState;
}

void GameLogic::setCurrentState(GameState* nextState)
{
    mCurrentState->exit();
    mCurrentState = nextState;
    mCurrentState->enter();
}

SelectFirstCardState* GameLogic::getSelectFirstCardState()
{
    return mSelectFirstCardState;
}

SelectSecondCardState* GameLogic::getSelectSecondCardState()
{
    return mSelectSecondCardState;
}

WaitBeforeClosingState* GameLogic::getWaitBeforeClosingState()
{
    return mWaitBeforeClosingState;
}

int GameLogic::getTotalScore() const
{
    return mTotalScore;
}

void GameLogic::selectionCompleted(bool success)
{
    ++mNumAttempts;
    if (success)
    {
        mTotalScore += calcScore();

        emit scoreChanged();
        mNumAttempts = 0;
    }
}

int GameLogic::calcScore() const
{
    if (mNumAttempts == 1)
        return 5;

    if (mNumAttempts == 2)
        return 4;

    if (mNumAttempts == 3)
        return 3;

    if (mNumAttempts == 4)
        return 2;

    return 1;
}
