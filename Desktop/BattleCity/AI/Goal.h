#pragma once

class Tank;

class Goal
{
public:
    virtual ~Goal() {}

    virtual void activate(Tank& tank) = 0;
    virtual void update(Tank& tank) = 0;
    virtual float evaluate(Tank& tank) const = 0;
};
