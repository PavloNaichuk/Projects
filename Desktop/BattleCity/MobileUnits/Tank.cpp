#include "Tank.h"
#include "Core/Config.h"
#include "Core/Utilities.h"
#include "MobileUnitStates/MobileUnitState.h"
#include "MobileUnitStates/MobileUnitStates.h"
#include "AI/TankAI.h"
#include <QtGlobal>

Tank::Tank(GameUnitType type, std::shared_ptr<TankAI> tankAI, LevelMap& levelMap,
     GameUnitModel& gameUnitModel, MobileUnitStates& mobileUnitStates,
     const QRectF& bounds, const QVector2D& heading, int health, float maxSpeed)
    : mType(type)
    , mMobileUnitStates(mobileUnitStates)
    , mBounds(bounds)
    , mHeading(heading)
    , mTankAI(tankAI)
    , mCurrState(mMobileUnitStates.stopState())
    , mCollisionDetector(levelMap, gameUnitModel)
    , mHealth(health)
    , mMaxSpeed(maxSpeed)
    , mpPath(nullptr)
{
    mWeaponReloadTime.start();
    mWeaponReloadTime = mWeaponReloadTime.addMSecs(Config::TANK_WEAPON_RELOAD_IN_MS);
}

void Tank::update()
{
    mTankAI->update(*this);
    mCurrState->update(*this);
}

GameUnitType Tank::type() const
{
    return mType;
}

MobileUnitStateType Tank::state() const
{
    return mCurrState->type();
}

const QRectF& Tank::bounds() const
{
    return mBounds;
}

void Tank::setBounds(const QRectF& bounds)
{
    mBounds = bounds;
}

float Tank::rotationAngle() const
{
    return extractRotationAngle(mHeading);
}

const QVector2D& Tank::heading() const
{
    return mHeading;
}

void Tank::setHeading(const QVector2D& heading)
{
    mHeading = heading;
}

bool Tank::destroyable() const
{
    return true;
}

void Tank::handleDamage(int healthDamage)
{
    mHealth -= healthDamage;
    emit damaged(this);
}

float Tank::maxSpeed() const
{
    return mMaxSpeed;
}

int Tank::health() const
{
    return mHealth;
}

bool Tank::readyToFire() const
{
    return (mWeaponReloadTime.elapsed() > Config::TANK_WEAPON_RELOAD_IN_MS);
}

void Tank::fire()
{
    Q_ASSERT(readyToFire());

    emit fired(this);
    mWeaponReloadTime.restart();
}

void Tank::moveForward()
{
    mCurrState = mMobileUnitStates.moveState();
}

void Tank::stop()
{
    mCurrState = mMobileUnitStates.stopState();
}

Path* Tank::path()
{
    return mpPath;
}

void Tank::setPath(Path* pPath)
{
    mpPath = pPath;
}

void Tank::followPath()
{
    Q_ASSERT(mpPath != nullptr);
    mCurrState = mMobileUnitStates.followPathState();
}

const QVector2D Tank::calcHeading(const QPointF& target, float* pDistToTarget) const
{
    QVector2D fireDir(target - mBounds.center());

    float distOnX = std::abs(fireDir.x());
    float distOnY = std::abs(fireDir.y());

    if (distOnX > distOnY)
    {
        fireDir.setX(sign(fireDir.x()));
        fireDir.setY(0.0f);

        if (pDistToTarget != nullptr)
           *pDistToTarget = distOnX;
    }
    else
    {
        fireDir.setX(0.0f);
        fireDir.setY(sign(fireDir.y()));

        if (pDistToTarget != nullptr)
           *pDistToTarget = distOnY;
    }
    return fireDir;
}

bool Tank::hasLineOfSight(const QPointF& target, float allowedRadius) const
{
    const QPointF tankCenter = mBounds.center();
    return belongsToRange(tankCenter.x() - allowedRadius, target.x(), tankCenter.x() + allowedRadius) ||
        belongsToRange(tankCenter.y() - allowedRadius, target.y(), tankCenter.y() + allowedRadius);
}

bool Tank::hasPathObstructed(const QVector2D& heading, float testDist, GameUnit** ppFoundGameUnit) const
{
    int checkFlags = CheckVersusStatationaryUnits | CheckVersusMobileUnits;

    Collision collision;
    mCollisionDetector.detectNearestCollisionForMobileUnit(*this, heading, testDist, &collision, checkFlags);

    if (ppFoundGameUnit != nullptr)
       *ppFoundGameUnit = collision.mpGameUnit;

    return collision.found();
}
