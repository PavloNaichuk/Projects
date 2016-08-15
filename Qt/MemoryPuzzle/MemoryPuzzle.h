#ifndef MEMORYPUZZLE_H
#define MEMORYPUZZLE_H

#include "LevelCompletedWidget.h"
#include "MainWidget.h"
#include "CardBoard.h"
#include "GameLogic.h"

class QKeyEvent;
class GameMenuWidget;
class QTime;

class MemoryPuzzle : public QObject
{
    Q_OBJECT

public:
    int runGame();

private slots:
    void levelCompleted();
    void loadNextLevel();
    void replayLevel();
    void exitGame();
    void openGameMenu();

private:
    MainWidget* mMainWidget;
    GameLogic* mGameLogic;
    CardBoard* mCardBoard;
    LevelCompletedWidget* mLevelCompletedWidget;
    GameMenuWidget* mGameMenu;
    Card* mCards;
};

#endif // MEMORYPUZZLE_H
