#pragma once

#include "Core/StationaryUnit.h"
#include <QRectF>

class Wall : public StationaryUnit
{
    Q_OBJECT
    Q_PROPERTY(const QRectF& bounds READ bounds)

public:
    Wall(GameUnitType type, const QRectF& bounds, int health);

    void update() override;
    GameUnitType type() const override;

    const QRectF& bounds() const override;
    void setBounds(const QRectF& bounds) override;

    bool destroyable() const override;
    void handleDamage(int healthDamage) override;
    int health() const override;

signals:
    void damaged(Wall* pBrickWall);

private:
    GameUnitType mType;
    QRectF mBounds;
    int mHealth;
};
