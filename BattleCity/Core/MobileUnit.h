#pragma once

#include "GameUnit.h"
#include "MobileUnitStates/MobileUnitStateTypes.h"

class Path;
class QVector2D;

class MobileUnit : public GameUnit
{
public:
    virtual void update() = 0;

    virtual GameUnitType type() const = 0;
    virtual MobileUnitStateType state() const = 0;

    virtual const QRectF& bounds() const = 0;
    virtual void setBounds(const QRectF& bounds) = 0;

    virtual const QVector2D& heading() const = 0;
    virtual void setHeading(const QVector2D& heading) = 0;

    virtual bool destroyable() const = 0;
    virtual void handleDamage(int healthDamage) = 0;
    virtual int health() const = 0;

    virtual float maxSpeed() const = 0;
    virtual float rotationAngle() const = 0;

    virtual Path* path() = 0;
    virtual void setPath(Path* pPath) = 0;
};
