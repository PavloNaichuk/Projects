#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <QObject>

class GameState : public QObject
{
    Q_OBJECT

public:
    virtual void enter() = 0;
    virtual void update() = 0;
    virtual void exit() = 0;
    virtual void handleMousePress(int x, int y) = 0;
};

#endif // GAMESTATE_H
