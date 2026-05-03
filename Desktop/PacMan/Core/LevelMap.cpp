#include "LevelMap.h"
#include <cassert>
#include <fstream>
#include <string>
#include <cmath>

LevelMap::LevelMap(const char* pathToLevelMap)
{
	std::ifstream fileStream(pathToLevelMap);
	if (fileStream)
	{
		for (int row = 0; row < Config::NUM_MAP_TILE_ROWS;)
		{
			std::string line;
			std::getline(fileStream, line);
			assert(Config::NUM_MAP_TILE_COLS == line.size());
			
			for (int col = 0; col < Config::NUM_MAP_TILE_COLS; ++col)
			{
				MapItemType mapItemType = MapItemType::NONE;
				switch (line[col])
				{
					case 'E': 
					{
						break;
					}
					case '1':
					{
						m_TeleportTile1 = MapTile(row, col, MapItemType::NONE);
						break;
					}
					case '2':
					{
						m_TeleportTile2 = MapTile(row, col, MapItemType::NONE);
						break;
					}
					case 'F':
					{
						mapItemType = MapItemType::POINT_PILL;
						break;
					}
					case 'G':
					{
						mapItemType = MapItemType::POWER_UP;
						break;
					}
					case 'W':
					{
						mapItemType = MapItemType::WALL;
						break;
					}
					case 'D':
					{
						mapItemType = MapItemType::PEN_DOOR;
						break;
					}
					case 'M':
					{
						m_PacManSpawnTile = MapTile(row, col, MapItemType::NONE);
						break;
					}
					case 'R':
					{
						m_BlinkySpawnTile = MapTile(row, col, MapItemType::NONE);
						break;
					}
					case 'P':
					{
						m_PinkySpawnTile = MapTile(row, col, MapItemType::NONE);
						break;
					}
					case 'B':
					{
						m_InkySpawnTile = MapTile(row, col, MapItemType::NONE);
						break;
					}
					case 'O':
					{
						m_ClydeSpawnTile = MapTile(row, col, MapItemType::NONE);
						break;
					}
					default:
					{
						assert(false && "Unknown format");
					}
				}
				
				m_MapTiles[row][col].m_Row = row;
				m_MapTiles[row][col].m_Col = col;
				m_MapTiles[row][col].m_MapItemType = mapItemType;
			}
			++row;
		}

		assert(IsValid(m_TeleportTile1));
		assert(IsValid(m_TeleportTile2));

		assert(IsValid(m_PacManSpawnTile));
		assert(IsValid(m_BlinkySpawnTile));
		assert(IsValid(m_PinkySpawnTile));
		assert(IsValid(m_InkySpawnTile));
		assert(IsValid(m_ClydeSpawnTile));
	}
	else
	{
		assert(false);
	}
}

const MapTile& LevelMap::GetTile(int tileRow, int tileCol) const
{
	return m_MapTiles[tileRow][tileCol];
}

const MapTile& LevelMap::GetTeleportTile1() const
{
	return m_TeleportTile1;
}

const MapTile& LevelMap::GetTeleportTile2() const
{
	return m_TeleportTile2;
}

const MapTile& LevelMap::GetPacManSpawnTile() const
{
	return m_PacManSpawnTile;
}

const MapTile& LevelMap::GetBlinkySpawnTile() const
{
	return m_BlinkySpawnTile;
}

const MapTile& LevelMap::GetPinkySpawnTile() const
{
	return m_PinkySpawnTile;
}

const MapTile& LevelMap::GetInkySpawnTile() const
{
	return m_InkySpawnTile;
}

const MapTile& LevelMap::GetClydeSpawnTile() const
{
	return m_ClydeSpawnTile;
}

bool IsValid(const MapTile& mapTile)
{
	return (mapTile.m_Row != MapTile::InvalidIndex) && (mapTile.m_Col != MapTile::InvalidIndex);
}

bool IsTraversableForGhost(const MapTile& mapTile)
{
	return ((mapTile.m_MapItemType == MapItemType::NONE) ||
		(mapTile.m_MapItemType == MapItemType::POINT_PILL) ||
		(mapTile.m_MapItemType == MapItemType::POWER_UP) ||
		(mapTile.m_MapItemType == MapItemType::PEN_DOOR));
}

bool IsTraversableForPacMan(const MapTile& mapTile)
{
	return ((mapTile.m_MapItemType == MapItemType::NONE) ||
		(mapTile.m_MapItemType == MapItemType::POINT_PILL) ||
		(mapTile.m_MapItemType == MapItemType::POWER_UP));
}

const Vector2f CalcMapTileCenter(int tileRow, int tileCol)
{
	return Vector2f((0.5f + float(tileCol)) * Config::MAP_TILE_WORLD_SIZE,
		-1.0f * (0.5f + float(tileRow)) * Config::MAP_TILE_WORLD_SIZE);
}

void CalcMapTileRowAndColumn(const Vector2f& point, int& tileRow, int& tileCol)
{
	tileRow = (int)std::floorf(-1.0f * point.m_Y / Config::MAP_TILE_WORLD_SIZE);
	tileCol = (int)std::floorf(point.m_X / Config::MAP_TILE_WORLD_SIZE);
}
