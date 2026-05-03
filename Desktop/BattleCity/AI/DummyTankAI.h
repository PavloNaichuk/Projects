#pragma once

#include "TankAI.h"
#include <QtGlobal>

class DummyTankAI : public TankAI
{
public:
    void update(Tank& tank)
    {
        Q_UNUSED(tank);
    }
};
