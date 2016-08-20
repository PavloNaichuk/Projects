#ifndef MEMORYPUZZLE_H
#define MEMORYPUZZLE_H

#include <QObject>

class MainWidget;
class CardBoard;
class GameLogic;
class Card;
class QKeyEvent;
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
    void resumeGame();
    void openGameMenu();

private:
    MainWidget* mMainWidget;
    GameLogic* mGameLogic;
    CardBoard* mCardBoard;
    Card* mCards;
};

#endif // MEMORYPUZZLE_H
