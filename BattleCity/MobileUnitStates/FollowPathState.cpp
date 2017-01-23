#include "FollowPathState.h"
#include "Core/MobileUnit.h"
#include "Core/Utilities.h"
#include "Core/LevelMap.h"
#include "Navigation/Path.h"

FollowPathState::FollowPathState(LevelMap& levelMap, GameUnitModel& gameUnitModel)
    : mCollisionDetector(levelMap, gameUnitModel)
{
}

MobileUnitStateType FollowPathState::type() const
{
    return MobileUnitStateType::FOLLOWING_PATH;
}

void FollowPathState::update(MobileUnit& gameUnit)
{
    Path* pPath = gameUnit.path();
    if (pPath->finished())
        return;

    QRectF gameUnitBounds = gameUnit.bounds();
    QPointF gameUnitCenter = gameUnitBounds.center();

    const MapTile* pMapTile = pPath->currentMapTile();
    QPointF checkPoint = calcMapTileBottomRightLocation(pMapTile->mRow, pMapTile->mCol);

    QVector2D moveDir(checkPoint - gameUnitCenter);
    float distToCheckPoint = moveDir.length();

    if (distToCheckPoint > kEpsilon)
    {
        gameUnit.setHeading(moveDir.normalized());

        float desiredMoveDist = gameUnit.maxSpeed();
        desiredMoveDist = std::min(desiredMoveDist, distToCheckPoint);

        Collision collision;
        mCollisionDetector.detectNearestCollisionForMobileUnit(gameUnit, gameUnit.heading(), desiredMoveDist, &collision, CheckVersusMobileUnits);

        float allowedMoveDist = desiredMoveDist - collision.mPenetration;
        QVector2D moveOffset = allowedMoveDist * gameUnit.heading();

        QRectF gameUnitBounds = gameUnit.bounds();
        gameUnitBounds.translate(moveOffset.toPointF());

        gameUnit.setBounds(gameUnitBounds);
    }
    else
    {
        pPath->setNextMapTile();
    }
}
