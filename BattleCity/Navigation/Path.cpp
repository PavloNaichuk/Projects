#include "Path.h"
#include <algorithm>
#include <QtGlobal>

Path::Path()
    : mCurrentTileIndex(0)
{
}

const MapTile* Path::currentMapTile() const
{
    return mMapTiles[mCurrentTileIndex];
}

void Path::setNextMapTile()
{
    Q_ASSERT(!finished());
    ++mCurrentTileIndex;
}

bool Path::finished() const
{
    return (mCurrentTileIndex == mMapTiles.size());
}

void Path::addMapTile(MapTile* pMapTile)
{
    Q_ASSERT(!mMapTiles.contains(pMapTile));
    mMapTiles.append(pMapTile);
}

void Path::removeMapTile(MapTile* pMapTile)
{
    Q_ASSERT(mMapTiles.contains(pMapTile));
    mMapTiles.removeOne(pMapTile);
}

bool Path::empty() const
{
    return mMapTiles.empty();
}

void Path::clear()
{
    mCurrentTileIndex = 0;
    mMapTiles.clear();
}

void Path::reverse()
{
    mCurrentTileIndex = 0;
    std::reverse(mMapTiles.begin(), mMapTiles.end());
}
