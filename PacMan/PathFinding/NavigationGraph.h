#pragma once

#include "Core/Config.h"
#include "Math/Vector2f.h"
#include <vector>
#include <tuple>

struct GraphNode
{
	static const int InvalidIndex = -1;

	GraphNode(int tileRow = InvalidIndex, int tileCol = InvalidIndex)
		: m_TileRow(tileRow)
		, m_TileCol(tileCol)
	{}
	int m_TileRow;
	int m_TileCol;
};

class NavigationGraph
{
public:
	using GraphNodeNeighbors = std::vector<GraphNode*>;

	NavigationGraph();
	NavigationGraph(const NavigationGraph&) = delete;
	NavigationGraph& operator= (const NavigationGraph&) = delete;
	
	const GraphNode& GetNode(int tileRow, int tileCol) const;
	const GraphNodeNeighbors& GetNodeNeighbors(int tileRow, int tileCol) const;

	void AddEdge(int startTileRow, int startTileCol, int endTileRow, int endTileCol);
	void RemoveEdge(int startTileRow, int startTileCol, int endTileRow, int endTileCol);
	
private:
	using GraphNodeData = std::tuple<GraphNode, GraphNodeNeighbors>;
	GraphNodeData m_GridGraph[Config::NUM_MAP_TILE_ROWS][Config::NUM_MAP_TILE_ROWS];
};