#include "stdafx.h"
#include "ShortestPath.h"
#include "Graph.h"

std::pair<Path, bool> findShortestPath(const Graph& graph, VertexIndex startIndex, VertexIndex endIndex)
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
	bool pathFound = false;
	while (true)
	{
		VertexIndex currentIndex = graph.numVertices();
		float minDist = std::numeric_limits<float>::max();
		for (VertexIndex index = 0; index < graph.numVertices(); ++index)
		{
			if ((visited[index] == 0) && (distance[index] < minDist))
			{
				currentIndex = index;
				minDist = distance[index];
			}
		}

		if (currentIndex == endIndex)
		{
			pathFound = true;
			break;
		}
		if (currentIndex == graph.numVertices()) 
		{
			pathFound = false;
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
	}

	if (pathFound)
	{
		Path path;
		for (VertexIndex index = endIndex; true; index = parents[index])
		{
			path.push_back(index);
			if (index == startIndex)
				break;
		}
		std::reverse(path.begin(), path.end());
		return std::make_pair(std::move(path), true);
	}
	return std::make_pair(Path(), false);
}
