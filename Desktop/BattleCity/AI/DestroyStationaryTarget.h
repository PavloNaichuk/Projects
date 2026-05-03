#pragma once

#include "Goal.h"
#include "Navigation/PathFinder.h"

class LevelMap;
class StationaryUnit;

class DestroyStationaryTarget : public Goal
{
public:
    DestroyStationaryTarget(const StationaryUnit& target, LevelMap& levelMap);

    void activate(Tank& tank) override;
    void update(Tank& tank) override;
    float evaluate(Tank& tank) const override;

private:
    const StationaryUnit& mTarget;
    PathFinder mPathFinder;
    Path mPathToTarget;
};
