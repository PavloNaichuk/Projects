#include "MemoryPuzzle.h"
#include "LevelGenerator.h"
#include "GameMenuWidget.h"
#include "StartGameMenuWidget.h"
#include "LevelCompletedWidget.h"
#include "HelpWidget.h"
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

    StartGameMenuWidget startGameMenu;
    connect(&startGameMenu, SIGNAL(startGame()), this, SLOT(startGame()));
    connect(&startGameMenu, SIGNAL(exitGame()), this, SLOT(exitGame()));
    connect(&startGameMenu, SIGNAL(showHelp()), this, SLOT(showHelp()));
    startGameMenu.exec();

    return 0;
}

void MemoryPuzzle::levelCompleted()
{
    LevelCompletedWidget levelCompletedWidget;
    connect(&levelCompletedWidget, SIGNAL(nextLevel()), this, SLOT(loadNextLevel()));
    connect(&levelCompletedWidget, SIGNAL(replayLevel()), this, SLOT(replayLevel()));
    connect(&levelCompletedWidget, SIGNAL(exitGame()), this, SLOT(exitGame()));
    levelCompletedWidget.exec();
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

void MemoryPuzzle::startGame()
{
    mGameLogic->startGame();
}

void MemoryPuzzle::openGameMenu()
{
    mGameLogic->pause();

    GameMenuWidget gameMemu;
    connect(&gameMemu, SIGNAL(resume()), this, SLOT(resumeGame()));
    connect(&gameMemu, SIGNAL(exitGame()), this, SLOT(exitGame()));
    connect(&gameMemu, SIGNAL(showHelp()), this, SLOT(showHelp()));
    gameMemu.exec();
}

void MemoryPuzzle::showHelp()
{
    HelpWidget helpWidget;
    helpWidget.exec();
}


