#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Utilities/Graph.h"
#include "../Utilities/ShortestPath.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(ShortestPathTest)
	{
	public:
		TEST_METHOD(TestFindShortestPath_Found1)
		{
			Graph graph(10);
			graph.addEdge(Edge(0, 1, 1.0f));
			graph.addEdge(Edge(2, 3, 2.0f));
			graph.addEdge(Edge(4, 5, 1.0f));

			const Path path = {2, 3};
			auto result = findShortestPath(graph, 2, 3);

			Assert::IsTrue(result.second);
			Assert::IsTrue(path == result.first);
		}

		TEST_METHOD(TestFindShortestPath_Found2)
		{
			Graph graph(10);
			graph.addEdge(Edge(0, 4, 1.0f));
			graph.addEdge(Edge(0, 7, 2.0f));
			graph.addEdge(Edge(1, 4, 1.0f));
			graph.addEdge(Edge(1, 5, 2.0f));
			graph.addEdge(Edge(2, 5, 2.0f));
			graph.addEdge(Edge(3, 6, 1.0f));
			graph.addEdge(Edge(3, 8, 2.0f));
			graph.addEdge(Edge(4, 5, 3.0f));
			graph.addEdge(Edge(4, 6, 4.0f));
			graph.addEdge(Edge(4, 9, 4.0f));
			graph.addEdge(Edge(5, 6, 5.0f));
			graph.addEdge(Edge(6, 7, 2.0f));
			graph.addEdge(Edge(7, 8, 3.0f));

			const Path path = {1, 4, 0, 7, 8 };
			auto result = findShortestPath(graph, 1, 8);

			Assert::IsTrue(result.second);
			Assert::IsTrue(path == result.first);
		}

		TEST_METHOD(TestFindShortestPath_NotFound1)
		{
			Graph graph(10);
			graph.addEdge(Edge(0, 1, 1.0f));
			graph.addEdge(Edge(2, 3, 2.0f));
			graph.addEdge(Edge(4, 5, 1.0f));

			auto result = findShortestPath(graph, 2, 5);

			Assert::IsFalse(result.second);
			Assert::IsTrue(result.first.empty());
		}

		TEST_METHOD(TestFindShortestPath_NotFound2)
		{
			Graph graph(11);
			graph.addEdge(Edge(0, 4, 1.0f));
			graph.addEdge(Edge(0, 7, 2.0f));
			graph.addEdge(Edge(1, 4, 1.0f));
			graph.addEdge(Edge(1, 5, 2.0f));
			graph.addEdge(Edge(2, 5, 2.0f));
			graph.addEdge(Edge(3, 6, 1.0f));
			graph.addEdge(Edge(3, 8, 2.0f));
			graph.addEdge(Edge(4, 5, 3.0f));
			graph.addEdge(Edge(4, 6, 4.0f));
			graph.addEdge(Edge(4, 9, 4.0f));
			graph.addEdge(Edge(5, 6, 5.0f));
			graph.addEdge(Edge(6, 7, 2.0f));
			graph.addEdge(Edge(7, 8, 3.0f));

			auto result = findShortestPath(graph, 1, 10);

			Assert::IsFalse(result.second);
			Assert::IsTrue(result.first.empty());
		}

		TEST_METHOD(TestFindShortestPath_InvalidVertex)
		{
			Graph graph(10);
			graph.addEdge(Edge(0, 4, 1.0f));
			graph.addEdge(Edge(0, 7, 2.0f));
			graph.addEdge(Edge(1, 4, 1.0f));
			graph.addEdge(Edge(1, 5, 2.0f));
			graph.addEdge(Edge(2, 5, 2.0f));
			graph.addEdge(Edge(3, 6, 1.0f));
			graph.addEdge(Edge(3, 8, 2.0f));
			graph.addEdge(Edge(4, 5, 3.0f));
			graph.addEdge(Edge(4, 6, 4.0f));
			graph.addEdge(Edge(4, 9, 4.0f));
			graph.addEdge(Edge(5, 6, 5.0f));
			graph.addEdge(Edge(6, 7, 2.0f));
			graph.addEdge(Edge(7, 8, 3.0f));

			auto result = findShortestPath(graph, 8, 11);
			Assert::IsFalse(result.second);
		}

		TEST_METHOD(TestFindShortestPath_SameStartAndEnd)
		{
			Graph graph(10);
			graph.addEdge(Edge(0, 1, 1.0f));
			graph.addEdge(Edge(2, 3, 2.0f));
			graph.addEdge(Edge(1, 4, 1.0f));
			graph.addEdge(Edge(1, 5, 2.0f));
			graph.addEdge(Edge(2, 5, 2.0f));
			graph.addEdge(Edge(3, 6, 1.0f));
			graph.addEdge(Edge(3, 8, 2.0f));
			graph.addEdge(Edge(4, 5, 3.0f));
			graph.addEdge(Edge(4, 6, 4.0f));
			graph.addEdge(Edge(4, 9, 4.0f));
			graph.addEdge(Edge(5, 6, 5.0f));
			graph.addEdge(Edge(6, 7, 2.0f));
			graph.addEdge(Edge(7, 8, 3.0f));

			const Path path = {2};
			auto result = findShortestPath(graph, 2, 2);

			Assert::IsTrue(result.second);
			Assert::IsTrue(path == result.first);
		}
	};
}
