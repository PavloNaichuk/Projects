#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Utilities/OptimizeTriangleData.h"
#include <algorithm>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(OptimizeTriangleDataTest)
	{
	public:
		TEST_METHOD(TestOptimizeTriangleData)
		{
			std::vector<Triangle> triangles(3);

			triangles[0].mNormal = Vector(0.0f, 0.0f, 1.0f);
			triangles[0].mVertex1 = Vector(1.0f, 0.0f, 0.0f);
			triangles[0].mVertex2 = Vector(0.5f, 0.5f, 0.0f);
			triangles[0].mVertex3 = Vector(0.0f, 0.0f, 0.0f);

			triangles[1].mNormal = Vector(0.0f, 0.0f, 1.0f);
			triangles[1].mVertex1 = Vector(0.0f, 0.0f, 0.0f);
			triangles[1].mVertex2 = Vector(0.5f, 0.5f, 0.0f);
			triangles[1].mVertex3 = Vector(0.0f, 1.0f, 0.0f);

			triangles[2].mNormal = Vector(0.0f, 0.0f, 1.0f);
			triangles[2].mVertex1 = Vector(1.0f, 0.0f, 0.0f);
			triangles[2].mVertex2 = Vector(1.0f, 1.0f, 0.0f);
			triangles[2].mVertex3 = Vector(0.0f, 1.0f, 0.0f);

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

			auto optimizedData = optimizeTriangleData(triangles);

			Assert::AreEqual(vertexBuffer.size(), optimizedData.first.size());
			for (decltype(vertexBuffer.size()) index = 0; index < vertexBuffer.size(); ++index)
				Assert::IsTrue(areEqual(vertexBuffer[index], optimizedData.first[index]));
		
			Assert::IsTrue(indexBuffer == optimizedData.second);
		}
	};
}