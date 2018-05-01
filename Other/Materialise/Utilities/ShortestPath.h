#pragma once

#include "Core.h"

class Graph;

struct Path 
{
	Path()
		: mDistance(-1.0f)
	{}
	Path(std::vector<VertexIndex> vertexIndices, float distance)
		: mVertexIndices(std::move(vertexIndices))
		, mDistance(distance)
	{}
	std::vector<VertexIndex> mVertexIndices;
	float mDistance;
};

std::pair<Path, bool> findShortestPath(const Graph& graph, VertexIndex startIndex, VertexIndex endIndex);