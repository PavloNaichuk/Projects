#pragma once

#include "Goal.h"
#include "Navigation/PathFinder.h"
#include <QTime>

class LevelMap;
class MobileUnit;

class DestroyMobileTarget : public Goal
{
public:
    DestroyMobileTarget(const MobileUnit& target, LevelMap& levelMap);

    void activate(Tank& tank) override;
    void update(Tank& tank) override;
    float evaluate(Tank& tank) const override;

private:
    void validatePathToTarget(Tank& tank);

private:
    const MobileUnit& mTarget;
    PathFinder mPathFinder;
    Path mPathToTarget;
    QTime mPathValidationTime;
};
