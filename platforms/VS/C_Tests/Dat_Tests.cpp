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

		TEST_METHOD(Given_a_valid_dat_When_entry_point_is_called_Then_the_number_of_entries_is_returned)
		{
			std::vector<uint8_t> inputFile = utils::ReadFile("basic.dat");
			dat_p dat = Dat_create(inputFile.data(), inputFile.size());
			try {
				int count = Dat_entryCount(dat);
				Assert::AreEqual(2, count, L"Invalid entry count should have been returned");
			}
			catch (...) {
			}
			Dat_destroy(dat);
		}



		TEST_METHOD(Given_a_valid_dat_When_next_is_called_Then_the_correct_header_is_returned) {
			// Move definitions of DatHeader and DatEntry into new header.
			// Include in export header
			// Do not void out using typedef
			// Remove headerSize method and c# checks.
		}
	};
}
