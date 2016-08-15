#include "MemoryPuzzle.h"

#include "MainWidget.h"
#include "LevelGenerator.h"
#include "GameMenuWidget.h"
#include <QtDebug>
#include <memory>

int MemoryPuzzle::runGame()
{
    mCards = LevelGenerator::create();
    mCardBoard = new CardBoard(mCards);

    mLevelCompletedWidget = new LevelCompletedWidget();

    connect(mLevelCompletedWidget, SIGNAL(nextLevel()), this, SLOT(loadNextLevel()));
    connect(mLevelCompletedWidget, SIGNAL(replayLevel()), this, SLOT(replayLevel()));
    connect(mLevelCompletedWidget, SIGNAL(exitGame()), this, SLOT(exitGame()));

    mGameLogic = new GameLogic();

    mMainWidget = new MainWidget(mGameLogic);
    connect(mMainWidget, SIGNAL(gameMenuRequested()), this, SLOT(openGameMenu()));
    connect(mGameLogic, SIGNAL(levelCompleted()), this, SLOT(levelCompleted()));
    mMainWidget->setCardBoard(mCardBoard);
    mMainWidget->show();

    mGameLogic->startGame();

    mGameMenu = new GameMenuWidget();
    connect(mGameMenu, SIGNAL(exitGame()), this, SLOT(exitGame()));

    return 0;
}

void MemoryPuzzle::levelCompleted()
{
    mLevelCompletedWidget->show();
}

void MemoryPuzzle::loadNextLevel()
{
    mGameLogic->nextLevel();
    delete mCards;
    delete mCardBoard;
    mCards = LevelGenerator::create();
    mCardBoard = new CardBoard(mCards);
    mMainWidget->setCardBoard(mCardBoard);
}


void MemoryPuzzle::replayLevel()
{
    delete mCardBoard;
    mCardBoard = new CardBoard(mCards);
    mMainWidget->setCardBoard(mCardBoard);
}

void MemoryPuzzle::exitGame()
{
    mMainWidget->close();
}

void MemoryPuzzle::openGameMenu()
{
    // gameMenu
    mGameMenu->show();
}



