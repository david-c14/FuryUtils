#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CppTests
{
	TEST_CLASS(CppTests)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			std::string bob = "Bob";
			std::string test = "Bob";
			Assert::AreEqual(bob, test);
		}
	};
}
