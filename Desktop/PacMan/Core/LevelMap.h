#pragma once

#include "Config.h"
#include "Math/Vector2f.h"

enum class MapItemType
{
	NONE,
	WALL,
	POINT_PILL,
	POWER_UP,
	PEN_DOOR
};

struct MapTile
{
	static const int InvalidIndex = -1;

	MapTile(int row = InvalidIndex, int col = InvalidIndex, MapItemType mapItemType = MapItemType::NONE)
		: m_Row(row)
		, m_Col(col)
		, m_MapItemType(mapItemType)
	{}

	int m_Row;
	int m_Col;
	MapItemType m_MapItemType;
};

class LevelMap
{
public:
	LevelMap(const char* pathToLevelMap);

	LevelMap(const LevelMap&) = delete;
	LevelMap& operator= (const LevelMap&) = delete;
	
	const MapTile& GetTile(int tileRow, int tileCol) const;

	const MapTile& GetTeleportTile1() const;
	const MapTile& GetTeleportTile2() const;
	
	const MapTile& GetPacManSpawnTile() const;
	const MapTile& GetBlinkySpawnTile() const;
	const MapTile& GetPinkySpawnTile() const;
	const MapTile& GetInkySpawnTile() const;
	const MapTile& GetClydeSpawnTile() const;

private:
	MapTile m_MapTiles[Config::NUM_MAP_TILE_ROWS][Config::NUM_MAP_TILE_COLS];

	MapTile m_TeleportTile1;
	MapTile m_TeleportTile2;

	MapTile m_PacManSpawnTile;
	MapTile m_BlinkySpawnTile;
	MapTile m_PinkySpawnTile;
	MapTile m_InkySpawnTile;
	MapTile m_ClydeSpawnTile;
};

bool IsValid(const MapTile& mapTile);
bool IsTraversableForGhost(const MapTile& mapTile);
bool IsTraversableForPacMan(const MapTile& mapTile);

const Vector2f CalcMapTileCenter(int tileRow, int tileCol);
void CalcMapTileRowAndColumn(const Vector2f& point, int& tileRow, int& tileCol);