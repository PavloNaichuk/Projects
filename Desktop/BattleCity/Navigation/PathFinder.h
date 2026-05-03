#pragma once

#include "Path.h"

class GameUnit;
class LevelMap;

class PathFinder
{
public:
    PathFinder(LevelMap& levelMap);
    Path findPath(const GameUnit& source, const GameUnit& target);

private:
    Path findShortestPath(int gameUnitTileSize, int startTileRow, int startTileCol, int endTileRow, int endTileCol);

private:
    LevelMap& mLevelMap;
};
