#include "pch.h"
#include "CppUnitTest.h"
#include "utils.hpp"
#include "Exceptions.h"
#include "Dat.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Dat_Tests
{
	TEST_CLASS(Dat_Tests)
	{
	public:
		
		TEST_METHOD(Given_a_faulty_dat_When_created_Then_an_exception_is_raised)
		{
			std::vector<uint8_t> inputFile = utils::ReadFile("pal8out.bmp");
			dat_p dat = Dat_create(inputFile.data(), inputFile.size());
			Assert::IsNull(dat, L"Expected null return value");
			Assert::AreEqual(3, GetExceptionCode(), L"Incorrect error code");
			Assert::AreEqual("Attempt to read beyond end of buffer", GetExceptionString(), L"Incorrect error message");
		}
	};
}
