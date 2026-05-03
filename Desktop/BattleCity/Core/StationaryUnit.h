#pragma once

#include "GameUnit.h"

class StationaryUnit : public GameUnit
{
public:
    virtual void update() = 0;
    virtual GameUnitType type() const = 0;

    virtual const QRectF& bounds() const = 0;
    virtual void setBounds(const QRectF& bounds) = 0;

    virtual bool destroyable() const = 0;
    virtual void handleDamage(int healthDamage) = 0;
    virtual int health() const = 0;
};
