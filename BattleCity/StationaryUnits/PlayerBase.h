#pragma once

#include "Core/StationaryUnit.h"
#include <QRectF>

class PlayerBase : public StationaryUnit
{
    Q_OBJECT
    Q_PROPERTY(const QRectF& bounds READ bounds)

public:
    PlayerBase(const QRectF& bounds);

    void update() override;
    GameUnitType type() const override;

    const QRectF& bounds() const override;
    void setBounds(const QRectF& bounds) override;

    bool destroyable() const override;
    void handleDamage(int healthDamage) override;
    int health() const override;

signals:
    void damaged(PlayerBase* pPlayerBase);

private:
    QRectF mBounds;
    int mHealth;
};
