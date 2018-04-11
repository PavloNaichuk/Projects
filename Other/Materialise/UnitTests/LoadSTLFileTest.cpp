#include "stdafx.h"
#include "CppUnitTest.h"
#include <fstream> 
#include "../Utilities/STLFile.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(STLLoaderTest)
	{
	public:
		TEST_METHOD(TestSTLLoader)
		{
			auto result = loadSTLFile("D:\\flat.stl");
			Assert::IsTrue(result.second);
		}

		TEST_METHOD(TestSTLLoaderFalse)
		{
			auto result = loadSTLFile("D:\\flat1.stl");
			Assert::IsTrue(result.second);
		}

		TEST_METHOD(TestSTLLoaderTriangle)
		{
			Triangle triangle1;
			triangle1.mNormal = Vector(0.0f, 0.0f, 1.0f);
			triangle1.mVertex1 = Vector(1.0f, 0.0f, 0.0f);
			triangle1.mVertex2 = Vector(0.5f, 0.5f, 0.0f);
			triangle1.mVertex3 = Vector(0.0f, 0.0f, 0.0f);

			Triangle triangle2;
			triangle2.mNormal = Vector(0.0f, 0.0f, 1.0f);
			triangle2.mVertex1 = Vector(0.0f, 0.0f, 0.0f);
			triangle2.mVertex2 = Vector(0.5f, 0.5f, 0.0f);
			triangle2.mVertex3 = Vector(0.0f, 1.0f, 0.0f);

			Triangle triangle3;
			triangle3.mNormal = Vector(0.0f, 0.0f, 1.0f);
			triangle3.mVertex1 = Vector(1.0f, 0.0f, 0.0f);
			triangle3.mVertex2 = Vector(1.5f, 1.0f, 0.0f);
			triangle3.mVertex3 = Vector(0.0f, 1.0f, 0.0f);

			auto result = loadSTLFile("D:\\flat.stl");

			Assert::IsTrue(AreEqual(triangle1, result.first[0]));
			Assert::IsTrue(AreEqual(triangle2, result.first[1]));
			Assert::IsTrue(AreEqual(triangle3, result.first[2]));
		}
	};
}
