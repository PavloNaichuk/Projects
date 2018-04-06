#include "stdafx.h"
#include "ShortestPath.h"
#include "Graph.h"

std::pair<std::vector<VertexIndex>, bool> findShortestPath(const Graph& graph, VertexIndex startIndex, VertexIndex endIndex)
{
	std::vector<float> distance(graph.numVertices());
	std::vector<VertexIndex> parents(graph.numVertices());
	std::vector<std::uint8_t> visited(graph.numVertices());
	for (VertexIndex i = 0; i < graph.numVertices(); ++i)
	{
		distance[i] = std::numeric_limits<float>::max();
		parents[i] = graph.numVertices();
		visited[i] = 0;
	}

	distance[startIndex] = 0.0f;
	VertexIndex numToVisit = graph.numVertices();
	while (numToVisit > 0)
	{
		VertexIndex currentIndex = graph.numVertices();
		float minDist = std::numeric_limits<float>::max();
		for (VertexIndex i = 0; i < graph.numVertices(); ++i)
		{
			if ((visited[i] == 0) && (distance[i] < minDist))
			{
				currentIndex = i;
				minDist = distance[i];
			}
		}
		if (currentIndex == endIndex)
		{
			break;
		}

		for (const AdjacentVertex& adjacent : graph.adjacencyList(currentIndex))
		{
			float newDist = distance[currentIndex] + adjacent.mWeight;
			if (newDist < distance[adjacent.mIndex])
			{
				distance[adjacent.mIndex] = newDist;
				parents[adjacent.mIndex] = currentIndex;
			}
		}

		visited[currentIndex] = 1;
		--numToVisit;
	}

	if (numToVisit > 0)
	{
		std::vector<VertexIndex> path;
		for (VertexIndex i = endIndex; true; i = parents[i])
		{
			path.push_back(i);
			if (i == startIndex)
				break;
		}
		std::reverse(path.begin(), path.end());
		return std::make_pair(std::move(path), true);
	}
	return std::make_pair(std::vector<VertexIndex>(), false);
}
