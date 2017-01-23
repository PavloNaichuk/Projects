#pragma once

#include "TankAI.h"
#include <vector>
#include <memory>

class Goal;
class LevelMap;
class GameUnitModel;

class SmartTankAI : public TankAI
{
public:
    SmartTankAI(LevelMap& levelMap, GameUnitModel& gameUnitModel);
    void update(Tank& tank) override;

private:
    std::shared_ptr<Goal> evaluateNextGoal(Tank& tank);

private:
    std::vector<std::shared_ptr<Goal>> mGoals;
    std::shared_ptr<Goal> mCurrentGoal;
};
