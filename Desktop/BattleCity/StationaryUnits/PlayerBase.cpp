#include "PlayerBase.h"
#include "Core/Config.h"
#include <QtGlobal>

PlayerBase::PlayerBase(const QRectF& bounds)
    : mBounds(bounds)
    , mHealth(Config::PLAYER_BASE_FULL_HEALTH)
{
}

void PlayerBase::update()
{
}

GameUnitType PlayerBase::type() const
{
    return GameUnitType::PLAYER_BASE;
}

const QRectF& PlayerBase::bounds() const
{
    return mBounds;
}

void PlayerBase::setBounds(const QRectF& bounds)
{
    mBounds = bounds;
}

int PlayerBase::health() const
{
    return mHealth;
}

bool PlayerBase::destroyable() const
{
    return true;
}

void PlayerBase::handleDamage(int healthDamage)
{
    mHealth -= healthDamage;
    emit damaged(this);
}
