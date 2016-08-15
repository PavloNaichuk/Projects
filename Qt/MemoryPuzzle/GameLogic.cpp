#include "GameLogic.h"
#include "SelectFirstCardState.h"
#include "SelectSecondCardState.h"
#include "WaitBeforeClosingState.h"
#include "CardBoard.h"
#include <QTime>
#include <QTimer>
#include <QtDebug>

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
    , mNumOpenedCards(0)
    , mCurrentLevel(1)
    , mTime(new QTime)
    , mTimer(new QTimer)
    , mSinceStart(new QTime)
{
    mSelectFirstCardState = new SelectFirstCardState(this);
    mSelectSecondCardState = new SelectSecondCardState(this);
    mWaitBeforeClosingState = new WaitBeforeClosingState(this);
    mCurrentState = mSelectFirstCardState;

    connect(mSelectSecondCardState, SIGNAL(selectionChanged(bool)), this, SLOT(selectionCompleted(bool)));
    connect(mTimer, SIGNAL(timeout()), this, SLOT(timeUpdate()));
}

GameLogic::~GameLogic()
{
    delete mSelectFirstCardState;
    delete mSelectSecondCardState;
    delete mWaitBeforeClosingState;
    delete mTime;
    delete mTimer;
    delete mSinceStart;
}

CardBoard* GameLogic::getCardBoard()
{
    return mCardBoard;
}

void GameLogic::setCardBoard(CardBoard* cardBoard)
{
    mTime->restart();
    mCardBoard = cardBoard;
    mTotalScore = 0;
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

int GameLogic::getCurrentLevel() const
{
    return mCurrentLevel;
}
void GameLogic::setScore(int score)
{
    mTotalScore = score;
    emit scoreChanged(mTotalScore);
}

void GameLogic::replayLevel()
{
    setScore(0);
    startGame();
}

void GameLogic::nextLevel()
{
    setScore(0);
    ++mCurrentLevel;
    startGame();
}

int GameLogic::getElapsedTime() const
{
    int elapsedTimeSec = mTime->elapsed() / 1000;
    return elapsedTimeSec;
}

void GameLogic::startGame()
{
    mTimer->start(1000);
}

void GameLogic::selectionCompleted(bool success)
{
    ++mNumAttempts;
    if (success)
    {
        mTotalScore += calcScore();
        mNumOpenedCards += 2;

        emit scoreChanged(mTotalScore);
        if (mNumOpenedCards == CardBoard::kNumRows * CardBoard::kNumCols)
        {
            emit levelCompleted();
            mNumOpenedCards = 0;
        }
        mNumAttempts = 0;
    }
}
void GameLogic::timeUpdate()
{
    qDebug() << "Time:" << mTimer;
    emit timeChanged(getElapsedTime());
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
