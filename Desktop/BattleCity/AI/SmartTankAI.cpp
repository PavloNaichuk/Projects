#include "SmartTankAI.h"
#include "DestroyMobileTarget.h"
#include "DestroyStationaryTarget.h"
#include "Core/GameUnitModel.h"
#include "StationaryUnits/PlayerBase.h"
#include "MobileUnits/Tank.h"
#include <algorithm>

SmartTankAI::SmartTankAI(LevelMap& levelMap, GameUnitModel& gameUnitModel)
{
    const Tank& playerTank = *gameUnitModel.playerTank();
    const PlayerBase& playerBase = *gameUnitModel.playerBase();

    mGoals.emplace_back(std::make_shared<DestroyMobileTarget>(playerTank, levelMap));
    mGoals.emplace_back(std::make_shared<DestroyStationaryTarget>(playerBase, levelMap));
}

void SmartTankAI::update(Tank& tank)
{
    auto nextGoal = evaluateNextGoal(tank);
    if (mCurrentGoal != nextGoal)
    {
        mCurrentGoal = nextGoal;
        mCurrentGoal->activate(tank);
    }
    else
    {
        mCurrentGoal->update(tank);
    }
}

std::shared_ptr<Goal> SmartTankAI::evaluateNextGoal(Tank& tank)
{
    auto hasLowerCost = [&tank](std::shared_ptr<Goal> goal1, std::shared_ptr<Goal> goal2)
    {
        return (goal1->evaluate(tank) < goal2->evaluate(tank));
    };

    auto it = std::min_element(mGoals.begin(), mGoals.end(), hasLowerCost);
    Q_ASSERT(it != mGoals.end());

    return *it;
}
