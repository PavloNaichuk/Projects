#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{		
	TEST_CLASS(GraphTest)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			int a = 5;
			a = a;
		}

	};
}