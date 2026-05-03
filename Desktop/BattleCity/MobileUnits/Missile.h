#pragma once

#include "Core/MobileUnit.h"
#include "CollisionDetection/CollisionDetector.h"
#include <QRectF>
#include <QVector2D>

class LevelMap;
class GameUnitModel;

class Missile : public MobileUnit
{
    Q_OBJECT

    Q_PROPERTY(const QRectF& bounds READ bounds)
    Q_PROPERTY(float rotationAngle READ rotationAngle)

public:
    Missile(LevelMap& levelMap, GameUnitModel& gameUnitModel, const QRectF& bounds, const QVector2D& heading);

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

signals:
    void damaged(Missile* pMissile);

private:
    QRectF mBounds;
    QVector2D mHeading;
    int mHealth;
    CollisionDetector mCollisionDetector;
};
