#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CTests
{
	TEST_CLASS(CTests)
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
