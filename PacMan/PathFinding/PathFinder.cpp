#include "PathFinder.h"
#include "Path.h"
#include "NavigationGraph.h"
#include "Core/LevelMap.h"
#include "Core/Config.h"
#include <queue>
#include <unordered_map>
#include <algorithm>

namespace
{
	std::shared_ptr<Path> BuildPath(const std::unordered_map<const GraphNode*, const GraphNode*>& parentMap, const GraphNode* pEndNode);
}

PathFinder::PathFinder(std::shared_ptr<LevelMap> levelMap)
	: m_NavGraph(new NavigationGraph())
{
	BuildNavigationGraph(levelMap);
}

std::shared_ptr<Path> PathFinder::FindPath(const Vector2f& startPoint, const Vector2f& endPoint)
{
	int startTileRow, startTileCol;
	CalcMapTileRowAndColumn(startPoint, startTileRow, startTileCol);

	int endTileRow, endTileCol;
	CalcMapTileRowAndColumn(endPoint, endTileRow, endTileCol);

	return BreadthFirstSearch(startTileRow, startTileCol, endTileRow, endTileCol);
}

void PathFinder::BuildNavigationGraph(std::shared_ptr<LevelMap> levelMap)
{
	for (int row = 0; row < Config::NUM_MAP_TILE_ROWS; ++row)
	{
		for (int col = 0; col < Config::NUM_MAP_TILE_COLS; ++col)
		{
			const MapTile& startMapTile = levelMap->GetTile(row, col);	
			if (IsTraversableForGhost(startMapTile))
			{
				if (row > 0)
				{
					const MapTile& endMapTile = levelMap->GetTile(row - 1, col);
					if (IsTraversableForGhost(endMapTile))
						m_NavGraph->AddEdge(row, col, endMapTile.m_Row, endMapTile.m_Col);
				}
				if (row < Config::NUM_MAP_TILE_ROWS - 1)
				{
					const MapTile& endMapTile = levelMap->GetTile(row + 1, col);
					if (IsTraversableForGhost(endMapTile))
						m_NavGraph->AddEdge(row, col, endMapTile.m_Row, endMapTile.m_Col);
				}
				if (col > 0)
				{
					const MapTile& endMapTile = levelMap->GetTile(row, col - 1);
					if (IsTraversableForGhost(endMapTile))
						m_NavGraph->AddEdge(row, col, endMapTile.m_Row, endMapTile.m_Col);
				}
				if (col < Config::NUM_MAP_TILE_COLS - 1)
				{
					const MapTile& endMapTile = levelMap->GetTile(row, col + 1);
					if (IsTraversableForGhost(endMapTile))
						m_NavGraph->AddEdge(row, col, endMapTile.m_Row, endMapTile.m_Col);
				}
			}
		}
	}
	{
		const MapTile& teleportTile1 = levelMap->GetTeleportTile1();
		const MapTile& teleportTile2 = levelMap->GetTeleportTile2();

		m_NavGraph->AddEdge(teleportTile1.m_Row, teleportTile1.m_Col, teleportTile2.m_Row, teleportTile2.m_Col);
		m_NavGraph->AddEdge(teleportTile2.m_Row, teleportTile2.m_Col, teleportTile1.m_Row, teleportTile1.m_Col);
	}
}

std::shared_ptr<Path> PathFinder::BreadthFirstSearch(int startTileRow, int startTileCol, int endTileRow, int endTileCol)
{
	const GraphNode* pStartNode = &m_NavGraph->GetNode(startTileRow, startTileCol);
	const GraphNode* pEndNode = &m_NavGraph->GetNode(endTileRow, endTileCol);

	std::queue<const GraphNode*> nodeQueue;
	nodeQueue.push(pStartNode);

	std::unordered_map<const GraphNode*, const GraphNode*> parentMap;
	parentMap.emplace(pStartNode, nullptr);

	bool pathFound = false;
	for (; !nodeQueue.empty() && !pathFound; nodeQueue.pop())
	{
		const GraphNode* pCurrNode = nodeQueue.front();
		if (pCurrNode == pEndNode)
		{
			pathFound = true;
		}
		else
		{
			for (const GraphNode* pNeighbor : m_NavGraph->GetNodeNeighbors(pCurrNode->m_TileRow, pCurrNode->m_TileCol))
			{
				if (parentMap.find(pNeighbor) == parentMap.end())
				{
					nodeQueue.push(pNeighbor);
					parentMap.emplace(pNeighbor, pCurrNode);
				}
			}
		}
	}
	if (pathFound)
	{
		return BuildPath(parentMap, pEndNode);
	}		
	return nullptr;
}

namespace
{
	std::shared_ptr<Path> BuildPath(const std::unordered_map<const GraphNode*, const GraphNode*>& parentMap, const GraphNode* pEndNode)
	{
		std::vector<const GraphNode*> nodes;

		const GraphNode* pPrevNode = pEndNode;
		nodes.emplace_back(pPrevNode);

		const GraphNode* pCurrNode = parentMap.at(pPrevNode);
		if (pCurrNode != nullptr)
		{
			int prevRowDir = pCurrNode->m_TileRow - pPrevNode->m_TileRow;
			int prevColDir = pCurrNode->m_TileCol - pPrevNode->m_TileCol;

			pPrevNode = pCurrNode;
			pCurrNode = parentMap.at(pPrevNode);

			while (pCurrNode != nullptr)
			{
				int currRowDir = pCurrNode->m_TileRow - pPrevNode->m_TileRow;
				int currColDir = pCurrNode->m_TileCol - pPrevNode->m_TileCol;
				
				if ((prevRowDir != currRowDir) || (prevColDir != currColDir))
				{
					prevRowDir = currRowDir;
					prevColDir = currColDir;

					nodes.emplace_back(pPrevNode);
				}

				pPrevNode = pCurrNode;
				pCurrNode = parentMap.at(pPrevNode);
			}
			nodes.emplace_back(pPrevNode);
		}

		std::reverse(nodes.begin(), nodes.end());
		return std::make_shared<Path>(std::move(nodes));
	}
}