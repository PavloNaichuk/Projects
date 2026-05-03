#include "MobileUnitStates.h"
#include "MobileUnitStates/StopState.h"
#include "MobileUnitStates/MoveState.h"
#include "MobileUnitStates/FollowPathState.h"

MobileUnitStates::MobileUnitStates(LevelMap& levelMap, GameUnitModel& gameUnitModel)
    : mStopState(new StopState())
    , mMoveState(new MoveState(levelMap, gameUnitModel))
    , mFollowPathState(new FollowPathState(levelMap, gameUnitModel))
{
}
