#ifndef SELECTSECONDCARDSTATE_H
#define SELECTSECONDCARDSTATE_H

#include "GameState.h"

class GameLogic;

class SelectSecondCardState : public GameState
{
    Q_OBJECT

public:
    SelectSecondCardState(GameLogic* gameLogic);

    void enter();
    void update();
    void exit();
    void handleMousePress(int x, int y);

signals:
    void selectionChanged(bool success);

private:
    GameLogic* mGameLogic;
};

#endif // SELECTSECONDCARDSTATE_H
