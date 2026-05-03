#pragma once

#include "Config.h"
#include <QPointF>
#include <QSizeF>
#include <QVector>
#include <QObject>

class StationaryUnit;
class GameUnitModel;

struct MapTile
{
    MapTile();

    int mRow, mCol;
    int mCost;
    int mClearance;
    StationaryUnit* mpStationaryUnit;
};

class LevelMap : public QObject
{
    Q_OBJECT
    Q_PROPERTY(const QSizeF& sizeInPixels READ sizeInPixels)

public:
    LevelMap();

    void loadLevel(const QString& fileName, GameUnitModel& gameUnitModel);

    const QSizeF& sizeInPixels() const;
    MapTile& mapTileAt(int row, int col);

    const QPointF& playerTankSpawnLocation() const;
    void setPlayerTankSpawnLocation(const QPointF& location);

    const QVector<QPointF>& enemyTankSpawnLocations() const;
    void addEnemyTankSpawnLocation(const QPointF& location);
    void removeEnemyTankSpawnLocation(const QPointF& location);

    void recomputeNavigationMetrics();
    void clear();

private:
    bool isNonTraversableTile(int row, int col) const;

    int calcClearance(int row, int col) const;
    int calcCost(int row, int col) const;

private:
    const QSizeF mSizeInPixels;
    MapTile mMapTiles[Config::TILE_NUM_ROWS][Config::TILE_NUM_COLS];
    QPointF mPlayerSpawnLocation;
    QVector<QPointF> mEnemyTankSpawnLocations;
};
