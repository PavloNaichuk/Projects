#ifndef GAMESTATE_H
#define GAMESTATE_H

class GameState
{
public:
    virtual ~GameState(){}
    virtual void enter() = 0;
    virtual void update() = 0;
    virtual void exit() = 0;
    virtual void handleMousePress(int x, int y) = 0;
};

#endif // GAMESTATE_H
