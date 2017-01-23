#pragma once

#include <QVector>

struct MapTile;

class Path
{
public:
    Path();

    const MapTile* currentMapTile() const;
    void setNextMapTile();

    bool finished() const;

    void addMapTile(MapTile* pMapTile);
    void removeMapTile(MapTile* pMapTile);

    bool empty() const;
    void reverse();
    void clear();

private:
    QVector<MapTile*> mMapTiles;
    int mCurrentTileIndex;
};
