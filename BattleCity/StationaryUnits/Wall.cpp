#include "Wall.h"
#include "Core/Config.h"
#include <QtGlobal>

Wall::Wall(GameUnitType type, const QRectF& bounds, int health)
    : mType(type)
    , mBounds(bounds)
    , mHealth(health)
{
}

void Wall::update()
{
}

GameUnitType Wall::type() const
{
    return mType;
}

const QRectF& Wall::bounds() const
{
    return mBounds;
}

void Wall::setBounds(const QRectF& bounds)
{
    mBounds = bounds;
}

int Wall::health() const
{
    return mHealth;
}

bool Wall::destroyable() const
{
    return (mType == GameUnitType::BRICK_WALL);
}

void Wall::handleDamage(int healthDamage)
{
    if (destroyable())
    {
        mHealth -= healthDamage;
        emit damaged(this);
    }
}
