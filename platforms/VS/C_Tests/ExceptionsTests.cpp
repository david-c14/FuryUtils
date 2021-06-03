#include "pch.h"
#include "CppUnitTest.h"
#include "utils.hpp"

#include "../../../include/FuryUtils.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Exceptions_Tests
{
	TEST_CLASS(Exceptions_Tests)
	{
	public:
		TEST_METHOD(When_GetExceptionCode_is_called_Then_the_correct_value_is_returned) {
			Assert::AreEqual(0, GetExceptionCode(), L"Incorrect error code");
 		}

		TEST_METHOD(When_GetExceptionString_is_called_Then_the_correct_value_is_returned) {
			Assert::AreEqual("", GetExceptionString(), L"Incorrect error message");
		}
	};
}
