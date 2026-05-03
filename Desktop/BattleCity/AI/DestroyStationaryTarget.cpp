#include "DestroyStationaryTarget.h"
#include "Core/StationaryUnit.h"
#include "MobileUnits/Tank.h"
#include <QtGlobal>

DestroyStationaryTarget::DestroyStationaryTarget(const StationaryUnit& target, LevelMap& levelMap)
    : mTarget(target)
    , mPathFinder(levelMap)
{
}

void DestroyStationaryTarget::activate(Tank& tank)
{
    mPathToTarget = mPathFinder.findPath(tank, mTarget);
    tank.setPath(&mPathToTarget);
}

void DestroyStationaryTarget::update(Tank& tank)
{
    const QPointF targetCenter = mTarget.bounds().center();
    const float allowedRadius = 2.0f;
    if (tank.hasLineOfSight(targetCenter, allowedRadius))
    {
        float distToTarget = 0.0f;
        const QVector2D headingToTarget = tank.calcHeading(targetCenter, &distToTarget);

        GameUnit* pFoundGameUnit = nullptr;
        if (!tank.hasPathObstructed(headingToTarget, distToTarget, &pFoundGameUnit) || pFoundGameUnit->destroyable())
        {
            tank.stop();
            tank.setHeading(headingToTarget);

            if (tank.readyToFire())
            {
                tank.fire();
            }
            else
            {
                tank.followPath();
            }
            return;
        }
    }
    if (tank.state() == MobileUnitStateType::STOPPED)
    {
        if (!mPathToTarget.finished())
            tank.followPath();

        return;
    }
    if (tank.state() == MobileUnitStateType::FOLLOWING_PATH)
    {
        if (mPathToTarget.finished())
            tank.stop();

        return;
    }
}

float DestroyStationaryTarget::evaluate(Tank& tank) const
{
    QPointF tankTopLeft = tank.bounds().topLeft();
    QPointF targetTopLeft = mTarget.bounds().topLeft();

    return QVector2D(tankTopLeft - targetTopLeft).lengthSquared();
}
