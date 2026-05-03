#pragma once

#include "Core/MobileUnit.h"
#include "CollisionDetection/CollisionDetector.h"
#include <memory>
#include <QRectF>
#include <QVector2D>
#include <QTime>

struct MapTile;
class LevelMap;
class GameUnitModel;
class MobileUnitState;
class MobileUnitStates;
class TankAI;

class Tank : public MobileUnit
{
    Q_OBJECT

    Q_PROPERTY(const QRectF& bounds READ bounds)
    Q_PROPERTY(float rotationAngle READ rotationAngle)

public:
    Tank(GameUnitType type, std::shared_ptr<TankAI> tankAI, LevelMap& levelMap,
         GameUnitModel& gameUnitModel, MobileUnitStates& mobileUnitStates,
         const QRectF& bounds, const QVector2D& heading, int health, float maxSpeed);

    void update() override;

    GameUnitType type() const override;
    MobileUnitStateType state() const override;

    const QRectF& bounds() const override;
    void setBounds(const QRectF& bounds) override;

    const QVector2D& heading() const override;
    void setHeading(const QVector2D& heading) override;

    bool destroyable() const override;
    void handleDamage(int healthDamage) override;
    int health() const override;

    float maxSpeed() const override;
    float rotationAngle() const override;

    Path* path() override;
    void setPath(Path* pPath) override;

    bool readyToFire() const;
    void fire();

    void moveForward();
    void stop();

    void followPath();

    const QVector2D calcHeading(const QPointF& target, float* pDistToTarget = nullptr) const;
    bool hasLineOfSight(const QPointF& target, float allowedRadius) const;
    bool hasPathObstructed(const QVector2D& heading, float testDist, GameUnit** ppFoundGameUnit) const;

signals:
    void damaged(Tank* pTank);
    void fired(Tank* pTank);

private:
    GameUnitType mType;
    MobileUnitStates& mMobileUnitStates;
    QRectF mBounds;
    QVector2D mHeading;
    std::shared_ptr<TankAI> mTankAI;
    MobileUnitState* mCurrState;
    CollisionDetector mCollisionDetector;
    int mHealth;
    float mMaxSpeed;
    QTime mWeaponReloadTime;
    Path* mpPath;
};
