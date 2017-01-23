#pragma once

#include "MobileUnitStates/MobileUnitState.h"
#include "MobileUnitStates/MobileUnitStateTypes.h"

class StopState : public MobileUnitState
{
public:
    MobileUnitStateType type() const
    {
        return MobileUnitStateType::STOPPED;
    }
    void update(MobileUnit& gameUnit) override
    {
    }
};
