#pragma once

#include <QPointF>
#include <QRectF>
#include <QVector2D>
#include <functional>
#include <array>
#include <vector>

class LevelMap;
class GameUnitModel;
class GameUnit;
class MobileUnit;
class StationaryUnit;

enum CollisionCheckFlags
{
    CheckVersusMapBorder = 1 << 0,
    CheckVersusStatationaryUnits = 1 << 1,
    CheckVersusMobileUnits = 1 << 2,
    CheckVersusAll = CheckVersusMapBorder | CheckVersusStatationaryUnits | CheckVersusMobileUnits
};

struct Collision
{
    Collision();
    Collision(GameUnit* pGameUnit, float penetration);

    bool found() const;

    GameUnit* mpGameUnit;
    float mPenetration;
};

class CollisionDetector
{
public:
    CollisionDetector(LevelMap& levelMap, GameUnitModel& gameUnitModel);

    void detectNearestCollisionForMobileUnit(const MobileUnit& mobileUnitOfInterest,
        const QVector2D& heading, float testDist, Collision* pCollision, int checkFlags = CheckVersusAll) const;

private:
    void populateBorderCollisionChecks();
    void populateStaticUnitCollisionChecks();
    void populateMobileUnitCollisionChecks();
    void populateFeelersGenerators();

    std::vector<MobileUnit*> fetchAllMobileUnits() const;

private:
    LevelMap& mLevelMap;
    GameUnitModel& mGameUnitModel;
    const QRectF mLevelMapBorders;

    using MapBorderCollisionCheck = std::function<float (const QRectF&, const QPointF&)>;
    using StaticUnitCollisionCheck = std::function<float (const QRectF&, const QPointF&)>;
    using MobileUnitCollisionCheck = std::function<float (const QRectF&, const QPointF&, const QPointF&)>;
    using FeelersGenerator = std::function<std::vector<QPointF> (const MobileUnit&)>;

    enum { kCollisionChecksCapacity = 5 };

    std::array<MapBorderCollisionCheck, kCollisionChecksCapacity> mMapBorderCollisionChecks;
    std::array<StaticUnitCollisionCheck, kCollisionChecksCapacity> mStaticUnitCollisionChecks;
    std::array<MobileUnitCollisionCheck, kCollisionChecksCapacity> mMobileUnitCollisionChecks;
    std::array<FeelersGenerator, kCollisionChecksCapacity> mFeelersGenerators;
};
