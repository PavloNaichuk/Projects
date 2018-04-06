#include "stdafx.h"
#include "Graph.h"
#include <iostream>
#include <algorithm>
#include <cassert>
#include <functional>

Edge::Edge(VertexIndex startIndex, VertexIndex endIndex, float weight)
	: mStartIndex(startIndex)
	, mEndIndex(endIndex)
	, mWeight(weight)
{
}

AdjacentVertex::AdjacentVertex(VertexIndex index, float weight)
	: mIndex(index)
	, mWeight(weight)
{}

Graph::Graph(VertexIndex numVertices)
	: mVertexList(numVertices)
{
}

VertexIndex Graph::numVertices() const
{
	return mVertexList.size();
}

const Graph::AdjacencyList& Graph::adjacencyList(VertexIndex index) const
{
	assert(index < numVertices());
	return mVertexList[index];
}

bool Graph::addEdge(const Edge& edge)
{
	if (edge.mStartIndex == edge.mEndIndex)
		return false;

	if (!((edge.mStartIndex < numVertices()) && (edge.mEndIndex < numVertices())))
		return false;

	auto comp = [&edge](const AdjacentVertex& adjacentVertex)
	{
		return adjacentVertex.mIndex == edge.mEndIndex;
	};
	auto it = std::find_if(mVertexList[edge.mStartIndex].cbegin(), mVertexList[edge.mStartIndex].cend(), comp);
	if ((it != mVertexList[edge.mStartIndex].cend()))
		return false;

	mVertexList[edge.mStartIndex].emplace_back(edge.mEndIndex, edge.mWeight);
	mVertexList[edge.mEndIndex].emplace_back(edge.mStartIndex, edge.mWeight);

	return true;
}

void Graph::removeEdge(const Edge& edge)
{
	{
		AdjacencyList& adjacencyList = mVertexList[edge.mStartIndex];

		auto it = std::find_if(adjacencyList.cbegin(), adjacencyList.cend(),
			[&](const AdjacentVertex& vertex) { return vertex.mIndex == edge.mEndIndex; });

		adjacencyList.erase(it);
	}
	{
		AdjacencyList& adjacencyList = mVertexList[edge.mEndIndex];

		auto it = std::find_if(adjacencyList.cbegin(), adjacencyList.cend(),
			[&](const AdjacentVertex& vertex) { return vertex.mIndex == edge.mStartIndex; });

		adjacencyList.erase(it);
	}
}

void print(const Graph& graph)
{
	const VertexIndex numVertices = graph.numVertices();
	for (VertexIndex vertexIndex = 0; vertexIndex < numVertices; ++vertexIndex)
	{
		std::cout << "Vertex index: " << vertexIndex << "\n";

		for (const AdjacentVertex& adjacentVertex : graph.adjacencyList(vertexIndex))
			std::cout << "Adjacent index: " << adjacentVertex.mIndex << ", weight: " << adjacentVertex.mWeight << "\n";

		std::cout << "\n";
	}
}