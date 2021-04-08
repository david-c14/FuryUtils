#include "pch.h"
#include "CppUnitTest.h"
#include <fstream>
#include <vector>
#include "../../../headers/header_Bmp.hpp"
#include "../../../headers/header_Exceptions.hpp"
#include "utils.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BmpTests
{
	TEST_CLASS(BmpTests)
	{
	public:

		TEST_METHOD(Given_a_file_less_than_18_bytes_in_length_When_the_file_is_used_to_construct_a_bmp_Then_an_INVALID_FORMAT_exception_is_raised) {
			try {
				Bmp bmp(utils::ReadFile("tooshort.bmp"));
				Assert::Fail(L"Exception not raised");
			}
			catch(Exceptions::Exception x) {
				Assert::AreEqual((int)Exceptions::INVALID_FORMAT, x._errorCode);
				Assert::AreEqual("Buffer is too short to contain a valid Bmp", x._errorString.c_str());
			}
		}

		TEST_METHOD(Given_a_file_with_a_non_bmp_header_When_the_file_is_used_to_construct_a_bmp_Then_an_INVALID_FORMAT_exception_is_raised) {
			try {
				Bmp bmp(utils::ReadFile("ba-bm.bmp"));
				Assert::Fail(L"Exception not raised");
			}
			catch (Exceptions::Exception x) {
				Assert::AreEqual((int)Exceptions::INVALID_FORMAT, x._errorCode);
				Assert::AreEqual("Second character of buffer is not 'M'", x._errorString.c_str());
			}

		}
	};
}
