#ifndef SELECTFIRSTCARDSTATE_H
#define SELECTFIRSTCARDSTATE_H

#include "GameState.h"

class GameLogic;

class SelectFirstCardState : public GameState
{
public:
    SelectFirstCardState(GameLogic* gameLogic);

    void enter();
    void update();
    void exit();
    void handleMousePress(int x, int y);

private:
    GameLogic* mGameLogic;
};

#endif // SELECTFIRSTCARDSTATE_H
