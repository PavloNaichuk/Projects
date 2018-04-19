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
		TEST_METHOD(TestLoad)
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
			triangle3.mVertex2 = Vector(1.0f, 1.0f, 0.0f);
			triangle3.mVertex3 = Vector(0.0f, 1.0f, 0.0f);

			auto result = loadSTLFile("../UnitTests/TestFiles/TestLoad.stl");
			Assert::IsTrue(result.second);
			Assert::AreEqual(result.first.size(), (decltype(result.first.size()))3);

			Assert::IsTrue(AreEqual(triangle1, result.first[0]));
			Assert::IsTrue(AreEqual(triangle2, result.first[1]));
			Assert::IsTrue(AreEqual(triangle3, result.first[2]));
		}

		TEST_METHOD(TestLoad_MissingNormalCoord)
		{
			auto result = loadSTLFile("../UnitTests/TestFiles/TestLoad_MissingNormalCoord.stl");
			Assert::IsFalse(result.second);
			Assert::IsTrue(result.first.empty());
		}

		TEST_METHOD(TestLoad_InvalidNormalFormat)
		{
			auto result = loadSTLFile("../UnitTests/TestFiles/TestLoad_InvalidNormalFormat.stl");
			Assert::IsFalse(result.second);
			Assert::IsTrue(result.first.empty());
		}

		TEST_METHOD(TestLoad_MissingNormal)
		{
			auto result = loadSTLFile("../UnitTests/TestFiles/TestLoad_MissingNormal.stl");
			Assert::IsFalse(result.second);
			Assert::IsTrue(result.first.empty());
		}

		TEST_METHOD(TestLoad_InvalidVertexFormat)
		{
			auto result = loadSTLFile("../UnitTests/TestFiles/TestLoad_InvalidVertexFormat.stl");
			Assert::IsFalse(result.second);
			Assert::IsTrue(result.first.empty());
		}

		TEST_METHOD(TestLoad_MissingVertex)
		{
			auto result = loadSTLFile("../UnitTests/TestFiles/TestLoad_MissingVertex.stl");
			Assert::IsFalse(result.second);
			Assert::IsTrue(result.first.empty());
		}

		TEST_METHOD(TestLoad_MissingVertexCoord)
		{
			auto result = loadSTLFile("../UnitTests/TestFiles/TestLoad_MissingVertexCoord.stl");
			Assert::IsFalse(result.second);
			Assert::IsTrue(result.first.empty());
		}

		TEST_METHOD(TestLoad_EmptyFile)
		{
			auto result = loadSTLFile("../UnitTests/TestFiles/TestLoad_EmptyFile.stl");
			Assert::IsFalse(result.second);
			Assert::IsTrue(result.first.empty());
		}

		TEST_METHOD(TestLoad_InvalidSolidAttribute)
		{
			auto result = loadSTLFile("../UnitTests/TestFiles/TestLoad_InvalidSolidAttribute.stl");
			Assert::IsFalse(result.second);
			Assert::IsTrue(result.first.empty());
		}

		TEST_METHOD(TestLoad_InvalidFacetAttribute)
		{
			auto result = loadSTLFile("../UnitTests/TestFiles/TestLoad_InvalidFacetAttribute.stl");
			Assert::IsFalse(result.second);
			Assert::IsTrue(result.first.empty());
		}

		TEST_METHOD(TestLoad_InvalidNormalAttribute)
		{
			auto result = loadSTLFile("../UnitTests/TestFiles/TestLoad_InvalidNormalAttribute.stl");
			Assert::IsFalse(result.second);
			Assert::IsTrue(result.first.empty());
		}

		TEST_METHOD(TestLoad_InvalidOuterAttribute)
		{
			auto result = loadSTLFile("../UnitTests/TestFiles/TestLoad_InvalidOuterAttribute.stl");
			Assert::IsFalse(result.second);
			Assert::IsTrue(result.first.empty());
		}

		TEST_METHOD(TestLoad_InvalidVertexAttribute)
		{
			auto result = loadSTLFile("../UnitTests/TestFiles/TestLoad_InvalidVertexAttribute.stl");
			Assert::IsFalse(result.second);
			Assert::IsTrue(result.first.empty());
		}

		TEST_METHOD(TestLoad_InvalidEndloopAttribute)
		{
			auto result = loadSTLFile("../UnitTests/TestFiles/TestLoad_InvalidEndloopAttribute.stl");
			Assert::IsFalse(result.second);
			Assert::IsTrue(result.first.empty());
		}

		TEST_METHOD(TestLoad_InvalidEndfacetAttribute)
		{
			auto result = loadSTLFile("../UnitTests/TestFiles/TestLoad_InvalidEndfacetAttribute.stl");
			Assert::IsFalse(result.second);
			Assert::IsTrue(result.first.empty());
		}
		
		TEST_METHOD(TestLoad_InvaludEndsolidAttribute)
		{
			auto result = loadSTLFile("../UnitTests/TestFiles/TestLoad_InvaludEndsolidAttribute.stl");
			Assert::IsFalse(result.second);
			Assert::IsTrue(result.first.empty());
		}

		TEST_METHOD(TestLoad_InvalidPath)
		{
			auto result = loadSTLFile("../UnitTests/TestFiles/TestLoad.std");
			Assert::IsFalse(result.second);
			Assert::IsTrue(result.first.empty());
		}
	};
}
