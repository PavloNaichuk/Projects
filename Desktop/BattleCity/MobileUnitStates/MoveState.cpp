#include "MoveState.h"
#include "Core/MobileUnit.h"

MoveState::MoveState(LevelMap& levelMap, GameUnitModel& gameUnitModel)
    : mCollisionDetector(levelMap, gameUnitModel)
{
}

MobileUnitStateType MoveState::type() const
{
    return MobileUnitStateType::MOVING;
}

void MoveState::update(MobileUnit& gameUnit)
{
    float desiredMoveDist = gameUnit.maxSpeed();

    Collision collision;
    mCollisionDetector.detectNearestCollisionForMobileUnit(gameUnit, gameUnit.heading(), desiredMoveDist, &collision);

    float allowedMoveDist = desiredMoveDist - collision.mPenetration;
    QVector2D moveOffset = allowedMoveDist * gameUnit.heading();

    QRectF bounds = gameUnit.bounds();
    bounds.translate(moveOffset.toPointF());

    gameUnit.setBounds(bounds);
}
