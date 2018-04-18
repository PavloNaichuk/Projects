#include "stdafx.h"
#include "OptimizeTriangleData.h"

std::pair<VertexBuffer, IndexBuffer> OptimizeTriangleData(const std::vector<Triangle>& triangles)
{
	VertexBuffer vertexBuffer;
	IndexBuffer indexBuffer;

	for (const Triangle& triangle : triangles)
	{
		const Vector* pFirstVertex = &triangle.mVertex1;
		const Vector* pLastVertex = pFirstVertex + 3;

		for (const Vector* pVertex = pFirstVertex; pVertex != pLastVertex; ++pVertex)
		{
			auto it = std::find_if(vertexBuffer.begin(), vertexBuffer.end(),
				[pVertex](const Vector& vertex) { return AreEqual(vertex, *pVertex); });

			if (it == vertexBuffer.end()) 
			{
				vertexBuffer.push_back(*pVertex);
				indexBuffer.push_back(VertexIndex(vertexBuffer.size() - 1));
			}
			else
				indexBuffer.push_back(VertexIndex(it - vertexBuffer.begin()));
		}
	}
	return std::make_pair(vertexBuffer, indexBuffer);
}