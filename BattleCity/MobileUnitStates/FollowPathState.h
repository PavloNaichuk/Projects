#pragma once

#include "MobileUnitStates/MobileUnitState.h"
#include "CollisionDetection/CollisionDetector.h"

class LevelMap;
class GameUnitModel;

class FollowPathState : public MobileUnitState
{
public:
    FollowPathState(LevelMap& levelMap, GameUnitModel& gameUnitModel);

    MobileUnitStateType type() const override;
    void update(MobileUnit& gameUnit) override;

private:
    CollisionDetector mCollisionDetector;
};
