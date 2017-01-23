#pragma once

#include "MobileUnitStates/MobileUnitStateTypes.h"

class MobileUnit;

class MobileUnitState
{
public:
    virtual ~MobileUnitState() {}

    virtual MobileUnitStateType type() const = 0;
    virtual void update(MobileUnit& gameUnit) = 0;
};
