#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Utilities/Graph.h"
#include "../Utilities/ShortestPath.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{		
	TEST_CLASS(GraphTest)
	{
	public:
		
		TEST_METHOD(TestNumVerties)
		{
			Graph graph1(0u);
			Assert::AreEqual(0u, graph1.numVertices());

			Graph graph2(10u);
			Assert::AreEqual(10u, graph2.numVertices());
		}
		
		TEST_METHOD(TestAddEdge_ValidEdge)
		{
			Graph graph1(10u);
			Graph graph2(15u);
			Edge edge(2, 11, 1.0f);

			Assert::IsFalse(graph1.addEdge(edge));
			Assert::IsTrue(graph2.addEdge(edge));
		}

		TEST_METHOD(TestAddEdge_TheSameStartAndEnd)
		{
			Graph graph(10u);
			Edge edge(4, 4, 1.0f);
			Assert::IsFalse(graph.addEdge(edge));
		}

		TEST_METHOD(TestAddEdge_DuplicateEdge)
		{
			Graph graph(10u);
			Edge edge(3, 6, 1.0f);
			Assert::IsTrue(graph.addEdge(edge));
			Assert::IsFalse(graph.addEdge(edge));
		}

		TEST_METHOD(TestRemoveEdge)
		{
			Graph graph(10u);
			Edge edge1(0, 1, 1.0f);
			Edge edge2(2, 5, 6.0f);
			Edge edge3(4, 5, 4.0f);
			Edge edge4(6, 7, 2.0f);

			graph.addEdge(edge1);
			graph.addEdge(edge2);
			graph.addEdge(edge3);
			graph.addEdge(edge4);
			
			Assert::IsTrue(graph.hasEdge(edge2));
			graph.removeEdge(edge2);
			Assert::IsFalse(graph.hasEdge(edge2));
		}
	};
}