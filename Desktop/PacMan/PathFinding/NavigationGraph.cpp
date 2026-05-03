#include "NavigationGraph.h"
#include <cassert>
#include <algorithm>

NavigationGraph::NavigationGraph()
{
	for (int row = 0; row < Config::NUM_MAP_TILE_ROWS; ++row)
	{
		for (int col = 0; col < Config::NUM_MAP_TILE_COLS; ++col)
		{
			GraphNode& graphNode = std::get<0>(m_GridGraph[row][col]);
			graphNode.m_TileRow = row;
			graphNode.m_TileCol = col;
		}
	}
}

const GraphNode& NavigationGraph::GetNode(int tileRow, int tileCol) const
{
	return std::get<0>(m_GridGraph[tileRow][tileCol]);
}

const NavigationGraph::GraphNodeNeighbors& NavigationGraph::GetNodeNeighbors(int tileRow, int tileCol) const
{
	return std::get<1>(m_GridGraph[tileRow][tileCol]);
}

void NavigationGraph::AddEdge(int startTileRow, int startTileCol, int endTileRow, int endTileCol)
{
	GraphNodeData& startNodeData = m_GridGraph[startTileRow][startTileCol];
	GraphNodeNeighbors& startNodeNeighbors = std::get<1>(startNodeData);

	GraphNodeData& endNodeData = m_GridGraph[endTileRow][endTileCol];
	GraphNode* pEndNode = &std::get<0>(endNodeData);

	assert(std::find(startNodeNeighbors.begin(), startNodeNeighbors.end(), pEndNode) == startNodeNeighbors.end());
	startNodeNeighbors.emplace_back(pEndNode);
}

void NavigationGraph::RemoveEdge(int startTileRow, int startTileCol, int endTileRow, int endTileCol)
{
	GraphNodeData& startNodeData = m_GridGraph[startTileRow][startTileCol];
	GraphNodeNeighbors& startNodeNeighbors = std::get<1>(startNodeData);

	GraphNodeData& endNodeData = m_GridGraph[endTileRow][endTileCol];
	GraphNode* pEndNode = &std::get<0>(endNodeData);

	auto endNodeIt = std::find(startNodeNeighbors.begin(), startNodeNeighbors.end(), pEndNode);
	assert(endNodeIt != startNodeNeighbors.end());
	startNodeNeighbors.erase(endNodeIt);
}
