#pragma once

#include "Core.h"

struct Edge
{
	Edge(VertexIndex startIndex, VertexIndex endIndex, float weight);

	VertexIndex mStartIndex;
	VertexIndex mEndIndex;
	float mWeight;
};

struct AdjacentVertex
{
	AdjacentVertex(VertexIndex index, float weight);

	VertexIndex mIndex;
	float mWeight;
};

class Graph
{
public:
	using AdjacencyList = std::vector<AdjacentVertex>;
	Graph(VertexIndex numVertices);

	VertexIndex numVertices() const;
	const AdjacencyList& adjacencyList(VertexIndex index) const;

	bool hasEdge(const Edge& edge) const;
	bool addEdge(const Edge& edge);
	void removeEdge(const Edge& edge);

private:
	using VertexList = std::vector<AdjacencyList>;
	VertexList mVertexList;
};

void print(const Graph& graph);
