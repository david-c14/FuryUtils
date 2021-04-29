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
			std::vector<uint8_t> inputFile = utils::ReadFile("basic.dat");
			dat_p dat = Dat_create(inputFile.data(), inputFile.size());
			try {
				DatHeader header;
				bool result;
				result = Dat_next(dat, &header);
				Assert::IsTrue(result, L"Returned value is not true");
				Assert::AreEqual((uint32_t)4767, header.CompressedSize, L"Compressed size is not correct");
				Assert::AreEqual((uint32_t)9270, header.UncompressedSize, L"Uncompressed size is not correct");
				Assert::AreEqual("pal8out.bmp", header.FileName, L"Filename is not correct");
				Assert::AreEqual((uint8_t)false, header.IsNotCompressed, L"Compression flag is incorrect");

				result = Dat_next(dat, &header);
				Assert::IsTrue(result, L"Returned value is not true");
				Assert::AreEqual((uint32_t)1698, header.CompressedSize, L"Compressed size is not correct");
				Assert::AreEqual((uint32_t)4214, header.UncompressedSize, L"Uncompressed size is not correct");
				Assert::AreEqual("pal4out.bmp", header.FileName, L"Filename is not correct");
				Assert::AreEqual((uint8_t)false, header.IsNotCompressed, L"Compression flag is incorrect");

				Dat_reset(dat);
				result = Dat_next(dat, &header);
				Assert::IsTrue(result, L"Returned value is not true");
				Assert::AreEqual((uint32_t)4767, header.CompressedSize, L"Compressed size is not correct");
				Assert::AreEqual((uint32_t)9270, header.UncompressedSize, L"Uncompressed size is not correct");
				Assert::AreEqual("pal8out.bmp", header.FileName, L"Filename is not correct");
				Assert::AreEqual((uint8_t)false, header.IsNotCompressed, L"Compression flag is incorrect");

			}
			catch (...) {

			}
			Dat_destroy(dat);
		}

		TEST_METHOD(Given_a_valid_dat_When_next_is_called_past_the_last_entry_Then_false_is_returned_and_the_passed_header_is_unchanged) {
			std::vector<uint8_t> inputFile = utils::ReadFile("basic.dat");
			dat_p dat = Dat_create(inputFile.data(), inputFile.size());
			try {
				DatHeader header;
				bool result;
				result = Dat_next(dat, &header);
				result = Dat_next(dat, &header);
				header.CompressedSize = 20;
				header.UncompressedSize = 10;
				header.IsNotCompressed = 1;
				header.FileName[0] = 'T';
				header.FileName[1] = 0;
				result = Dat_next(dat, &header);
				Assert::IsFalse(result, L"Returned value is true");
				Assert::AreEqual((uint32_t)20, header.CompressedSize, L"Compressed size is not correct");
				Assert::AreEqual((uint32_t)10, header.UncompressedSize, L"Uncompressed size is not correct");
				Assert::AreEqual("T", header.FileName, L"Filename is not correct");
				Assert::AreEqual((uint8_t)true, header.IsNotCompressed, L"Compression flag is incorrect");
			}
			catch (...) {

			}
			Dat_destroy(dat);
		}

		TEST_METHOD(Given_a_valid_dat_When_header_is_called_Then_the_correct_header_is_returned) {
			std::vector<uint8_t> inputFile = utils::ReadFile("basic.dat");
			dat_p dat = Dat_create(inputFile.data(), inputFile.size());
			try {
				DatHeader header;
				bool result;
				result = Dat_header(dat, 1, &header);
				Assert::IsTrue(result, L"Returned value is not true");
				Assert::AreEqual((uint32_t)1698, header.CompressedSize, L"Compressed size is not correct");
				Assert::AreEqual((uint32_t)4214, header.UncompressedSize, L"Uncompressed size is not correct");
				Assert::AreEqual("pal4out.bmp", header.FileName, L"Filename is not correct");
				Assert::AreEqual((uint8_t)false, header.IsNotCompressed, L"Compression flag is incorrect");
			}
			catch (...) {

			}
			Dat_destroy(dat);
		}

		TEST_METHOD(Given_a_valid_dat_When_header_is_called_with_an_incorrect_index_Then_the_false_is_returned_and_the_header_is_unchanged) {
			std::vector<uint8_t> inputFile = utils::ReadFile("basic.dat");
			dat_p dat = Dat_create(inputFile.data(), inputFile.size());
			try {
				DatHeader header;
				bool result;
				header.CompressedSize = 20;
				header.UncompressedSize = 10;
				header.IsNotCompressed = 1;
				header.FileName[0] = 'T';
				header.FileName[1] = 0;
				result = Dat_header(dat, 2, &header);
				Assert::IsFalse(result, L"Returned value is true");
				Assert::AreEqual((uint32_t)20, header.CompressedSize, L"Compressed size is not correct");
				Assert::AreEqual((uint32_t)10, header.UncompressedSize, L"Uncompressed size is not correct");
				Assert::AreEqual("T", header.FileName, L"Filename is not correct");
				Assert::AreEqual((uint8_t)true, header.IsNotCompressed, L"Compression flag is incorrect");
				Assert::AreEqual(4, GetExceptionCode(), L"Incorrect error code");
				Assert::AreEqual("Index out of range", GetExceptionString(), L"Incorrect error message");
			}
			catch (...) {

			}
			Dat_destroy(dat);
		}

		TEST_METHOD(Given_a_valid_dat_When_entry_is_called_Then_the_correct_buffer_is_returned) {
			std::vector<uint8_t> inputFile = utils::ReadFile("basic.dat");
			std::vector<uint8_t> expected = utils::ReadFile("pal8out.bmp");
			dat_p dat = Dat_create(inputFile.data(), inputFile.size());
			try {
				std::vector<uint8_t> actualBuffer(9270);
				bool result = Dat_entry(dat, 0, actualBuffer.data(), actualBuffer.size());
				Assert::IsTrue(result, L"Return value is incorrect");
				Assert::AreEqual(expected.size(), actualBuffer.size(), L"Filesize is incorrect");
				int cmp = memcmp(expected.data(), actualBuffer.data(), expected.size());
				Assert::AreEqual(0, cmp, L"Comparison does not match");
			}
			catch (...) {

			}
			Dat_destroy(dat);
		}

		TEST_METHOD(Given_a_valid_dat_When_entry_is_called_with_an_invalid_index_Then_the_return_value_is_false_and_INDEX_OUT_OF_RANGE_is_raised) {
			std::vector<uint8_t> inputFile = utils::ReadFile("basic.dat");
			dat_p dat = Dat_create(inputFile.data(), inputFile.size());
			try {
				std::vector<uint8_t> actualBuffer(9270);
				bool result = Dat_entry(dat, 2, actualBuffer.data(), actualBuffer.size());
				Assert::IsFalse(result, L"Return value is incorrect");
				Assert::AreEqual(4, GetExceptionCode(), L"Incorrect error code");
				Assert::AreEqual("Index out of range", GetExceptionString(), L"Incorrect error message");
			}
			catch (...) {

			}
			Dat_destroy(dat);
		}

		TEST_METHOD(Given_a_valid_dat_When_entry_is_called_with_an_incorrectly_sized_buffer_Then_the_return_value_is_false_and_BUFFER_OVERFLOW_is_raised) {
			std::vector<uint8_t> inputFile = utils::ReadFile("basic.dat");
			dat_p dat = Dat_create(inputFile.data(), inputFile.size());
			try {
				std::vector<uint8_t> actualBuffer(9269);
				bool result = Dat_entry(dat, 0, actualBuffer.data(), actualBuffer.size());
				Assert::IsFalse(result, L"Return value is incorrect");
				Assert::AreEqual(3, GetExceptionCode(), L"Incorrect error code");
				Assert::AreEqual("Attempt to read beyond end of buffer", GetExceptionString(), L"Incorrect error message");
			}
			catch (...) {

			}
			Dat_destroy(dat);
		}
	};
}
