#include "LevelMap.h"
#include "Core/Utilities.h"
#include "Core/GameUnitModel.h"
#include "StationaryUnits/Wall.h"
#include "StationaryUnits/PlayerBase.h"
#include <QFile>
#include <QTextStream>
#include <QtGlobal>

namespace
{
    const int kInvalidPos = -1;
    const int kInvalidClearance = 0;
    const int kInvalidCost = std::numeric_limits<int>::max();
}

MapTile::MapTile()
    : mRow(kInvalidPos)
    , mCol(kInvalidPos)
    , mCost(kInvalidCost)
    , mClearance(kInvalidClearance)
    , mpStationaryUnit(nullptr)
{}

LevelMap::LevelMap()
    : mSizeInPixels(Config::TILE_NUM_COLS * Config::TILE_SCREEN_SIZE, Config::TILE_NUM_ROWS * Config::TILE_SCREEN_SIZE)
{
    for (int row = 0; row < Config::TILE_NUM_ROWS; ++row)
    {
        for (int col = 0; col < Config::TILE_NUM_COLS; ++col)
        {
            mMapTiles[row][col].mRow = row;
            mMapTiles[row][col].mCol = col;
        }
    }
}

MapTile& LevelMap::mapTileAt(int row, int col)
{
    Q_ASSERT(belongsToRange(0, row, Config::TILE_NUM_ROWS - 1));
    Q_ASSERT(belongsToRange(0, col, Config::TILE_NUM_COLS - 1));

    return mMapTiles[row][col];
}

const QSizeF& LevelMap::sizeInPixels() const
{
    return mSizeInPixels;
}

const QPointF& LevelMap::playerTankSpawnLocation() const
{
    return mPlayerSpawnLocation;
}

void LevelMap::setPlayerTankSpawnLocation(const QPointF& location)
{
    mPlayerSpawnLocation = location;
}

const QVector<QPointF>& LevelMap::enemyTankSpawnLocations() const
{
    return mEnemyTankSpawnLocations;
}

void LevelMap::addEnemyTankSpawnLocation(const QPointF& location)
{
    mEnemyTankSpawnLocations.append(location);
}

void LevelMap::removeEnemyTankSpawnLocation(const QPointF& location)
{
    Q_ASSERT(mEnemyTankSpawnLocations.contains(location));
    mEnemyTankSpawnLocations.removeOne(location);
}

void LevelMap::recomputeNavigationMetrics()
{
    Q_ASSERT(Config::TILE_NUM_ROWS == Config::TILE_NUM_COLS);
    for (int row = 0; row < Config::TILE_NUM_ROWS; ++row)
    {
        for (int col = 0; col < Config::TILE_NUM_COLS; ++col)
        {
            mMapTiles[row][col].mCost = calcCost(row, col);
            mMapTiles[row][col].mClearance = calcClearance(row, col);
        }
    }
}

void LevelMap::clear()
{
    for (int row = 0; row < Config::TILE_NUM_ROWS; ++row)
    {
        for (int col = 0; col < Config::TILE_NUM_COLS; ++col)
        {
            mMapTiles[row][col].mCost = kInvalidCost;
            mMapTiles[row][col].mClearance = kInvalidClearance;
            mMapTiles[row][col].mpStationaryUnit = nullptr;
        }
    }
    mEnemyTankSpawnLocations.clear();
}

int LevelMap::calcClearance(int row, int col) const
{
    int clearance = 0;
    while ((row + clearance < Config::TILE_NUM_ROWS) && (col + clearance < Config::TILE_NUM_COLS))
    {
        for (int r = row; r < (row + clearance + 1); ++r)
            if (isNonTraversableTile(r, col + clearance))
                return clearance;

        for (int c = col; c < (col + clearance); ++c)
            if (isNonTraversableTile(row + clearance, c))
                return clearance;

        ++clearance;
    }
    return clearance;
}

int LevelMap::calcCost(int row, int col) const
{
    const StationaryUnit* pStationaryUnit = mMapTiles[row][col].mpStationaryUnit;
    if (pStationaryUnit == nullptr)
        return 0;

    if (pStationaryUnit->destroyable())
        return 2;

    return 4;
}

bool LevelMap::isNonTraversableTile(int row, int col) const
{
    return (mMapTiles[row][col].mpStationaryUnit != nullptr);
}

void LevelMap::loadLevel(const QString& fileName, GameUnitModel& gameUnitModel)
{
    clear();
    gameUnitModel.clear();

    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly))
    {
        int row = 0;
        for (QTextStream stream(&file); !stream.atEnd(); ++row)
        {
            QString line = stream.readLine();
            Q_ASSERT(line.size() == Config::TILE_NUM_COLS);

            for (int col = 0; col < Config::TILE_NUM_COLS; ++col)
            {
                MapTile& mapTile = mMapTiles[row][col];
                switch (line[col].toLatin1())
                {
                    case 'T':
                    {
                        break;
                    }
                    case 'B':
                    {
                        QPointF topLeft = calcMapTileTopLeftLocation(row, col);
                        QPointF bottomRight = calcMapTileBottomRightLocation(row, col);

                        Wall* pWall = new Wall(GameUnitType::BRICK_WALL, QRectF(topLeft, bottomRight), Config::BRICK_WALL_FULL_HEALTH);
                        gameUnitModel.addBrickWall(pWall);

                        mapTile.mpStationaryUnit = pWall;
                        break;
                    }
                    case 'S':
                    {
                        QPointF topLeft = calcMapTileTopLeftLocation(row, col);
                        QPointF bottomRight = calcMapTileBottomRightLocation(row, col);

                        Wall* pWall = new Wall(GameUnitType::STEEL_WALL, QRectF(topLeft, bottomRight), Config::STEEL_WALL_FULL_HEALTH);
                        gameUnitModel.addSteelWall(pWall);

                        mapTile.mpStationaryUnit = pWall;
                        break;
                    }
                    case 'E':
                    {
                        QPointF tankCenter = calcMapTileBottomRightLocation(row, col);
                        addEnemyTankSpawnLocation(tankCenter);
                        break;
                    }
                    case 'P':
                    {
                        QPointF tankCenter = calcMapTileBottomRightLocation(row, col);
                        setPlayerTankSpawnLocation(tankCenter);
                        break;
                    }
                    case 'H':
                    {
                        const int sizeInTiles = 2;

                        QPointF topLeft = calcMapTileTopLeftLocation(row, col);
                        QPointF bottomRight = calcMapTileBottomRightLocation(row + sizeInTiles - 1, col + sizeInTiles - 1);

                        PlayerBase* pBase = new PlayerBase(QRectF(topLeft, bottomRight));
                        gameUnitModel.setPlayerBase(pBase);

                        for (int r = row; r < row + sizeInTiles; ++r)
                            for (int c = col; c < col + sizeInTiles; ++c)
                                mMapTiles[r][c].mpStationaryUnit = pBase;

                        break;
                    }
                }
            }
        }
        Q_ASSERT(row == Config::TILE_NUM_ROWS);
        recomputeNavigationMetrics();
    }
    else
    {
        Q_ASSERT(false);
    }
}
