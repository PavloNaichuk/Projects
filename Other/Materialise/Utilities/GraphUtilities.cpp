#include "stdafx.h"
#include "GraphUtilities.h"

Graph createGraph(const VertexBuffer& vertexBuffer, const IndexBuffer& indexBuffer)
{
	Graph graph(vertexBuffer.size());

    auto indexIt = indexBuffer.cbegin();
	auto indexItEnd = indexBuffer.cend();

	while (indexIt != indexItEnd)
	{
		VertexIndex index1 = *indexIt++;
		VertexIndex index2 = *indexIt++;
		VertexIndex index3 = *indexIt++;

		Edge edge12(index1, index2, distance(vertexBuffer[index1], vertexBuffer[index2]));
		Edge edge23(index2, index3, distance(vertexBuffer[index2], vertexBuffer[index3]));
		Edge edge13(index1, index3, distance(vertexBuffer[index1], vertexBuffer[index3]));
		
		graph.addEdge(edge12);
		graph.addEdge(edge23);
		graph.addEdge(edge13);
	}

	return graph;
}
