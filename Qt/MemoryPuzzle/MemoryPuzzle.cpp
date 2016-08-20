#include "MemoryPuzzle.h"
#include "LevelGenerator.h"
#include "GameMenuWidget.h"
#include "LevelCompletedWidget.h"
#include "MainWidget.h"
#include "CardBoard.h"
#include "GameLogic.h"

#include <QtDebug>
#include <memory>

int MemoryPuzzle::runGame()
{

    mCards = LevelGenerator::create();
    mCardBoard = new CardBoard(mCards);

    mGameLogic = new GameLogic();
    mMainWidget = new MainWidget(mGameLogic);
    connect(mMainWidget, SIGNAL(gameMenuRequested()), this, SLOT(openGameMenu()));
    connect(mGameLogic, SIGNAL(levelCompleted()), this, SLOT(levelCompleted()));

    mMainWidget->setCardBoard(mCardBoard);
    mMainWidget->show();

    mGameLogic->startGame();
    return 0;
}

void MemoryPuzzle::levelCompleted()
{
    LevelCompletedWidget* levelCompletedWidget = new LevelCompletedWidget();
    connect(levelCompletedWidget, SIGNAL(nextLevel()), this, SLOT(loadNextLevel()));
    connect(levelCompletedWidget, SIGNAL(replayLevel()), this, SLOT(replayLevel()));
    connect(levelCompletedWidget, SIGNAL(exitGame()), this, SLOT(exitGame()));
    levelCompletedWidget->show();
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
    mGameLogic->replayLevel();
    delete mCardBoard;
    mCardBoard = new CardBoard(mCards);
    mMainWidget->setCardBoard(mCardBoard);
}

void MemoryPuzzle::exitGame()
{
    mMainWidget->close();
}

void MemoryPuzzle::resumeGame()
{
    mGameLogic->resume();
}

void MemoryPuzzle::openGameMenu()
{
    GameMenuWidget* gameMemu = new GameMenuWidget();
    connect(gameMemu, SIGNAL(exitGame()), this, SLOT(exitGame()));
    connect(gameMemu, SIGNAL(resume()), this, SLOT(resumeGame()));
    mGameLogic->pause();
    gameMemu->show();
}
