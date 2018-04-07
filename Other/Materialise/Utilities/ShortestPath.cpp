#include "stdafx.h"
#include "ShortestPath.h"
#include "Graph.h"

std::pair<Path, bool> findShortestPath(const Graph& graph, VertexIndex startIndex, VertexIndex endIndex)
{
	std::vector<float> distances(graph.numVertices(), std::numeric_limits<float>::max());
	std::vector<VertexIndex> parents(graph.numVertices(), graph.numVertices());
	std::vector<std::uint8_t> visited(graph.numVertices(), 0);

	bool pathFound = false;
	distances[startIndex] = 0.0f;
	
	while (true)
	{
		VertexIndex currentIndex = graph.numVertices();
		float minDist = std::numeric_limits<float>::max();
		for (VertexIndex index = 0; index < graph.numVertices(); ++index)
		{
			if ((visited[index] == 0) && (distances[index] < minDist))
			{
				currentIndex = index;
				minDist = distances[index];
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
			float newDist = distances[currentIndex] + adjacent.mWeight;
			if (newDist < distances[adjacent.mIndex])
			{
				distances[adjacent.mIndex] = newDist;
				parents[adjacent.mIndex] = currentIndex;
			}
		}
		visited[currentIndex] = 1;
	}

	Path path;
	if (pathFound)
	{
		for (VertexIndex index = endIndex; true; index = parents[index])
		{
			path.push_back(index);
			if (index == startIndex)
				break;
		}
		std::reverse(path.begin(), path.end());
	}
	return std::make_pair(path, pathFound);
}
