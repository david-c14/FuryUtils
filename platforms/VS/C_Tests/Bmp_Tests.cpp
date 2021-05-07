#include "pch.h"
#include "CppUnitTest.h"
#include "utils.hpp"
#include "Exceptions.h"
#include "Bmp.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Bmp_Tests
{
	TEST_CLASS(Bmp_Tests)
	{
	public:

		TEST_METHOD(Given_a_faulty_bmp_When_created_Then_an_exception_is_raised) {
			std::vector<uint8_t> inputFile = utils::ReadFile("badrle.bmp");
			bmp_p bmp = Bmp_createFromBmp(inputFile.data(), inputFile.size());
			Assert::IsNull(bmp, L"Expected null return value");
			Assert::AreEqual(1, GetExceptionCode(), L"Incorrect error code");
			Assert::AreEqual("Compressed data contains an error", GetExceptionString(), L"Incorrect error message");
		}

		TEST_METHOD(Given_a_faulty_imm_When_created_Then_an_exception_is_raised) {
			std::vector<uint8_t> pixelFile = utils::ReadFile("tooshort.bmp");
			std::vector<uint8_t> paletteFile = utils::ReadFile("pal8out.pam");
			bmp_p bmp = Bmp_createFromImmAndPam(pixelFile.data(), pixelFile.size(), paletteFile.data(), paletteFile.size());
			Assert::IsNull(bmp, L"Expected null return value");
			Assert::AreEqual(1, GetExceptionCode(), L"Incorrect error code");
			Assert::AreEqual("Image buffer size is too short for valid Imm", GetExceptionString(), L"Incorrect error message");
		}
	};
}
