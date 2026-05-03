#pragma once

class Tank;

class TankAI
{
public:
    virtual ~TankAI() {}
    virtual void update(Tank& tank) = 0;
};
