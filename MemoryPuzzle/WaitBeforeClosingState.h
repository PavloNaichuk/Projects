#ifndef WAIT_BEFORE_CLOSING_STATE_H
#define WAIT_BEFORE_CLOSING_STATE_H

#include "GameState.h"
#include <QTime>

class GameLogic;

class WaitBeforeClosingState : public GameState
{
public:
    WaitBeforeClosingState(GameLogic* gameLogic);

    void enter();
    void update();
    void exit();
    void handleMousePress(int x, int y);

private:
    GameLogic* mGameLogic;
    QTime mTime;
};

#endif // WAIT_BEFORE_CLOSING_STATE_H
