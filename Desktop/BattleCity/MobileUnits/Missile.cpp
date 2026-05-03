#include "Missile.h"
#include "Core/Config.h"
#include "Core/Utilities.h"
#include <QtGlobal>

Missile::Missile(LevelMap& levelMap, GameUnitModel& gameUnitModel, const QRectF& bounds, const QVector2D& heading)
    : mBounds(bounds)
    , mHeading(heading)
    , mHealth(Config::MISSILE_FULL_HEALTH)
    , mCollisionDetector(levelMap, gameUnitModel)
{
}

void Missile::update()
{
    float desiredMoveDist = maxSpeed();

    Collision collision;
    mCollisionDetector.detectNearestCollisionForMobileUnit(*this, mHeading, desiredMoveDist, &collision);

    float allowedMoveDist = desiredMoveDist - collision.mPenetration;
    QVector2D moveOffset = allowedMoveDist * mHeading;

    mBounds.translate(moveOffset.toPointF());

    if (collision.found())
    {
        if (collision.mpGameUnit != nullptr)
            collision.mpGameUnit->handleDamage(Config::MISSILE_HEALTH_DAMAGE);

        handleDamage(Config::MISSILE_FULL_HEALTH);
    }
}

GameUnitType Missile::type() const
{
    return GameUnitType::MISSILE;
}

MobileUnitStateType Missile::state() const
{
    return MobileUnitStateType::MOVING;
}

const QRectF& Missile::bounds() const
{
    return mBounds;
}

void Missile::setBounds(const QRectF& bounds)
{
    mBounds = bounds;
}

float Missile::rotationAngle() const
{
    return extractRotationAngle(mHeading);
}

const QVector2D& Missile::heading() const
{
    return mHeading;
}

void Missile::setHeading(const QVector2D& heading)
{
    mHeading = heading;
}

bool Missile::destroyable() const
{
    return true;
}

void Missile::handleDamage(int healthDamage)
{
    mHealth -= healthDamage;
    emit damaged(this);
}

float Missile::maxSpeed() const
{
    return Config::MISSILE_SPEED;
}

int Missile::health() const
{
    return mHealth;
}

Path* Missile::path()
{
    Q_ASSERT(false && "Can be implemented if needed");
    return nullptr;
}

void Missile::setPath(Path* pPath)
{
    Q_ASSERT(false && "Can be implemented if needed");
    Q_UNUSED(pPath);
}
