#pragma once

#include <memory>

class LevelMap;
class GameUnitModel;
class MobileUnitState;

class MobileUnitStates
{
public:
    MobileUnitStates(LevelMap& levelMap, GameUnitModel& gameUnitModel);

    MobileUnitState* stopState() { return mStopState.get(); }
    MobileUnitState* moveState() { return mMoveState.get(); }
    MobileUnitState* followPathState() { return mFollowPathState.get(); }

private:
    std::shared_ptr<MobileUnitState> mStopState;
    std::shared_ptr<MobileUnitState> mMoveState;
    std::shared_ptr<MobileUnitState> mFollowPathState;
};
