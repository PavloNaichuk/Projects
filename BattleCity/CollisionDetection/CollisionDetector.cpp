#include "CollisionDetector.h"
#include "Core/Utilities.h"
#include "Core/LevelMap.h"
#include "Core/GameUnitModel.h"
#include "Core/StationaryUnit.h"
#include "Core/MobileUnit.h"
#include "MobileUnits/Missile.h"
#include "MobileUnits/Tank.h"

namespace
{
    constexpr int calcHash(int dirX, int dirY);
}

Collision::Collision()
    : Collision(nullptr, 0.0f)
{
}

Collision::Collision(GameUnit* pGameUnit, float penetration)
    : mpGameUnit(pGameUnit)
    , mPenetration(penetration)
{
}

bool Collision::found() const
{
    return (mPenetration > 0.0f);
}

CollisionDetector::CollisionDetector(LevelMap& levelMap, GameUnitModel& gameUnitModel)
    : mLevelMap(levelMap)
    , mGameUnitModel(gameUnitModel)
    , mLevelMapBorders(QPointF(0.0f, 0.0f), mLevelMap.sizeInPixels())
{
    populateBorderCollisionChecks();
    populateStaticUnitCollisionChecks();
    populateMobileUnitCollisionChecks();
    populateFeelersGenerators();
}

void CollisionDetector::detectNearestCollisionForMobileUnit(const MobileUnit& mobileUnitOfInterest,
    const QVector2D& heading, float testDist, Collision* pCollision, int checkFlags) const
{
    pCollision->mpGameUnit = nullptr;
    pCollision->mPenetration = 0.0f;

    const int feelerDirX = int(heading.x());
    const int feelerDirY = int(heading.y());
    const int funcHash = calcHash(feelerDirX, feelerDirY);

    auto feelersGenerator = mFeelersGenerators[funcHash];
    auto borderCollisionCheck = mMapBorderCollisionChecks[funcHash];
    auto staticUnitCollisionCheck = mStaticUnitCollisionChecks[funcHash];
    auto mobileUnitCollisionCheck = mMobileUnitCollisionChecks[funcHash];

    auto mobileUnits = fetchAllMobileUnits();

    for (const QPointF& feelerStart : feelersGenerator(mobileUnitOfInterest))
    {
        QPointF feelerEnd = feelerStart + testDist * heading;

        float droppedBorderPenetration = 0.0f;
        if ((checkFlags & CheckVersusMapBorder) != 0)
        {
            float borderPenetration = borderCollisionCheck(mLevelMapBorders, feelerEnd);
            if (borderPenetration > 0.0f)
            {
                droppedBorderPenetration = borderPenetration + Config::TILE_SCREEN_HALF_SIZE;

                float testDistWithinMap = testDist - droppedBorderPenetration;
                feelerEnd = feelerStart + testDistWithinMap * heading;

                if (borderPenetration > pCollision->mPenetration)
                {
                    pCollision->mpGameUnit = nullptr;
                    pCollision->mPenetration = borderPenetration;
                }
            }
        }
        if ((checkFlags & CheckVersusStatationaryUnits) != 0)
        {
            int startRow, startCol;
            calcMapTileRowAndCol(feelerStart, startRow, startCol);

            int endRow, endCol;
            calcMapTileRowAndCol(feelerEnd, endRow, endCol);

            const int numTiles = std::max(std::abs(endRow - startRow), std::abs(endCol - startCol));
            for (int i = 1; i <= numTiles; ++i)
            {
                MapTile& mapTile = mLevelMap.mapTileAt(startRow + i * feelerDirY, startCol + i * feelerDirX);

                StationaryUnit* pGameUnit = mapTile.mpStationaryUnit;
                if (pGameUnit != nullptr)
                {
                    float gameUnitPenetration = staticUnitCollisionCheck(pGameUnit->bounds(), feelerEnd);
                    Q_ASSERT(gameUnitPenetration > 0.0f);

                    gameUnitPenetration += droppedBorderPenetration;
                    if (gameUnitPenetration > pCollision->mPenetration)
                    {
                        pCollision->mpGameUnit = pGameUnit;
                        pCollision->mPenetration = gameUnitPenetration;
                    }
                    break;
                }
            }
        }
        if ((checkFlags & CheckVersusMobileUnits) != 0)
        {
            for (MobileUnit* pGameUnit : mobileUnits)
            {
                if (pGameUnit != &mobileUnitOfInterest)
                {
                    float gameUnitPenetration = mobileUnitCollisionCheck(pGameUnit->bounds(), feelerStart, feelerEnd);
                    if (gameUnitPenetration > 0.0f)
                    {
                        gameUnitPenetration += droppedBorderPenetration;
                        if (gameUnitPenetration > pCollision->mPenetration)
                        {
                            pCollision->mpGameUnit = pGameUnit;
                            pCollision->mPenetration = gameUnitPenetration;
                        }
                    }
                }
            }
        }
    }
}

std::vector<MobileUnit*> CollisionDetector::fetchAllMobileUnits() const
{
    std::vector<MobileUnit*> mobileUnits;
    mobileUnits.reserve(1 + mGameUnitModel.numEnemyTanks() + mGameUnitModel.numMissiles());

    mobileUnits.emplace_back(mGameUnitModel.playerTank());
    for (int i = 0; i < mGameUnitModel.numEnemyTanks(); ++i)
        mobileUnits.emplace_back(mGameUnitModel.enemyTankAt(i));
    for (int i = 0; i < mGameUnitModel.numMissiles(); ++i)
        mobileUnits.emplace_back(mGameUnitModel.missileAt(i));

    return mobileUnits;
}

void CollisionDetector::populateBorderCollisionChecks()
{
    mMapBorderCollisionChecks[calcHash(1, 0)] = [](const QRectF& rect, const QPointF& point)
    {
        float penetration = 0.0f;
        if (rect.right() <= point.x())
            penetration = 1.0f + point.x() - rect.right();

        return penetration;
    };
    mMapBorderCollisionChecks[calcHash(-1, 0)] = [](const QRectF& rect, const QPointF& point)
    {
        float penetration = 0.0f;
        if (point.x() <= rect.left())
            penetration = 1.0f + rect.left() - point.x();

        return penetration;
    };
    mMapBorderCollisionChecks[calcHash(0, -1)] = [](const QRectF& rect, const QPointF& point)
    {
        float penetration = 0.0f;
        if (point.y() <= rect.top())
            penetration = 1.0f + rect.top() - point.y();

        return penetration;
    };
    mMapBorderCollisionChecks[calcHash(0, 1)] = [](const QRectF& rect, const QPointF& point)
    {
        float penetration = 0.0f;
        if (rect.bottom() <= point.y())
            penetration = 1.0f + point.y() - rect.bottom();

        return penetration;
    };
}

void CollisionDetector::populateStaticUnitCollisionChecks()
{
    mStaticUnitCollisionChecks[calcHash(1, 0)] = [](const QRectF& rect, const QPointF& point)
    {
        float penetration = 0.0f;
        if (rect.left() <= point.x())
            penetration = 1.0f + point.x() - rect.left();

        return penetration;
    };
    mStaticUnitCollisionChecks[calcHash(-1, 0)] = [](const QRectF& rect, const QPointF& point)
    {
        float penetration = 0.0f;
        if (point.x() <= rect.right())
            penetration = 1.0f + rect.right() - point.x();

        return penetration;
    };
    mStaticUnitCollisionChecks[calcHash(0, -1)] = [](const QRectF& rect, const QPointF& point)
    {
        float penetration = 0.0f;
        if (point.y() <= rect.bottom())
            penetration = 1.0f + rect.bottom() - point.y();

        return penetration;
    };
    mStaticUnitCollisionChecks[calcHash(0, 1)] = [](const QRectF& rect, const QPointF& point)
    {
        float penetration = 0.0f;
        if (rect.top() <= point.y())
            penetration = 1.0f + point.y() - rect.top();

        return penetration;
    };
}

void CollisionDetector::populateMobileUnitCollisionChecks()
{
    mMobileUnitCollisionChecks[calcHash(1, 0)] = [](const QRectF& rect, const QPointF& feelerStart, const QPointF& feelerEnd)
    {
        float penetration = 0.0f;

        if (belongsToRange(feelerStart.x(), rect.left(), feelerEnd.x()) &&
            belongsToRange(rect.top(), feelerEnd.y(), rect.bottom()))
            penetration = 1.0f + feelerEnd.x() - rect.left();

        return penetration;
    };
    mMobileUnitCollisionChecks[calcHash(-1, 0)] = [](const QRectF& rect,  const QPointF& feelerStart, const QPointF& feelerEnd)
    {
        float penetration = 0.0f;

        if (belongsToRange(feelerEnd.x(), rect.right(), feelerStart.x()) &&
            belongsToRange(rect.top(), feelerEnd.y(), rect.bottom()))
            penetration = 1.0f + rect.right() - feelerEnd.x();

        return penetration;
    };
    mMobileUnitCollisionChecks[calcHash(0, -1)] = [](const QRectF& rect,  const QPointF& feelerStart, const QPointF& feelerEnd)
    {
        float penetration = 0.0f;

        if (belongsToRange(feelerEnd.y(), rect.bottom(), feelerStart.y()) &&
            belongsToRange(rect.left(), feelerEnd.x(), rect.right()))
            penetration = 1.0f + rect.bottom() - feelerEnd.y();

        return penetration;
    };
    mMobileUnitCollisionChecks[calcHash(0, 1)] = [](const QRectF& rect,  const QPointF& feelerStart, const QPointF& feelerEnd)
    {
        float penetration = 0.0f;

        if (belongsToRange(feelerStart.y(), rect.top(), feelerEnd.y()) &&
            belongsToRange(rect.left(), feelerEnd.x(), rect.right()))
            penetration = 1.0f + feelerEnd.y() - rect.top();

        return penetration;
    };
}

void CollisionDetector::populateFeelersGenerators()
{
    mFeelersGenerators[calcHash(1, 0)] = [](const MobileUnit& mobileUnit)
    {
        return std::vector<QPointF>{mobileUnit.bounds().topRight(), mobileUnit.bounds().bottomRight()};
    };
    mFeelersGenerators[calcHash(-1, 0)] = [](const MobileUnit& mobileUnit)
    {
        return std::vector<QPointF>{mobileUnit.bounds().topLeft(), mobileUnit.bounds().bottomLeft()};
    };
    mFeelersGenerators[calcHash(0, -1)] = [](const MobileUnit& mobileUnit)
    {
        return std::vector<QPointF>{mobileUnit.bounds().topLeft(), mobileUnit.bounds().topRight()};
    };
    mFeelersGenerators[calcHash(0, 1)] = [](const MobileUnit& mobileUnit)
    {
        return std::vector<QPointF>{mobileUnit.bounds().bottomLeft(), mobileUnit.bounds().bottomRight()};
    };
}

namespace
{
    constexpr int calcHash(int dirX, int dirY)
    {
        return (((1 + dirY) << 1) + dirX);
    }
}
