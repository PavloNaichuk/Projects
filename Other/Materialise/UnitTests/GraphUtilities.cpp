#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Utilities/Graph.h"
#include "../Utilities/GraphUtilities.h"
#include <algorithm>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(GraphUtilitiesTest)
	{
	public:
		TEST_METHOD(GraphUtilities)
		{
			const VertexBuffer vertexBuffer =
			{
				Vector(1.0f, 0.0f, 0.0f),
				Vector(0.5f, 0.5f, 0.0f),
				Vector(0.0f, 0.0f, 0.0f),
				Vector(0.0f, 1.0f, 0.0f),
				Vector(1.0f, 1.0f, 0.0f)
			};
			const IndexBuffer indexBuffer =
			{
				0, 1, 2,
				2, 1, 3,
				0, 4, 3
			};

			Graph graph = createGraph(vertexBuffer, indexBuffer);
            Assert::AreEqual(vertexBuffer.size(), graph.numVertices());

			
			const std::vector<AdjacentVertex>& adjacentList = graph.adjacencyList(2);
			Assert::IsTrue(adjacentList.size() == 3u);
			
			Assert::AreEqual(adjacentList[0].mIndex, 1u);
			Assert::AreEqual(adjacentList[1].mIndex, 0u);
			Assert::AreEqual(adjacentList[2].mIndex, 3u);
			
		
		}
	};
}