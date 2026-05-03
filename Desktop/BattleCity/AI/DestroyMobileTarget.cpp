#include "DestroyMobileTarget.h"
#include "MobileUnits/Tank.h"
#include "Core/Config.h"
#include <QtGlobal>

DestroyMobileTarget::DestroyMobileTarget(const MobileUnit& target, LevelMap& levelMap)
    : mTarget(target)
    , mPathFinder(levelMap)
{
    mPathValidationTime.start();
}

void DestroyMobileTarget::activate(Tank& tank)
{
    validatePathToTarget(tank);
}

void DestroyMobileTarget::update(Tank& tank)
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
                tank.fire();

            return;
        }
        else
        {
            validatePathToTarget(tank);
        }
    }
    if (tank.state() == MobileUnitStateType::STOPPED)
    {
        tank.followPath();
        return;
    }
    if (tank.state() == MobileUnitStateType::FOLLOWING_PATH)
    {
        if (mPathToTarget.finished() || (mPathValidationTime.elapsed() > Config::TANK_PATH_VALIDATION_IN_MS))
        {
            validatePathToTarget(tank);
        }
        return;
    }
}

float DestroyMobileTarget::evaluate(Tank& tank) const
{
    QPointF tankTopLeft = tank.bounds().topLeft();
    QPointF targetTopLeft = mTarget.bounds().topLeft();

    return QVector2D(tankTopLeft - targetTopLeft).lengthSquared();
}

void DestroyMobileTarget::validatePathToTarget(Tank& tank)
{
    mPathToTarget = mPathFinder.findPath(tank, mTarget);
    tank.setPath(&mPathToTarget);

    mPathValidationTime.restart();
}
