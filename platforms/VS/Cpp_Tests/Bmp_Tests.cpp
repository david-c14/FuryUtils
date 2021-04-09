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
				Assert::AreEqual(Exceptions::ERROR_BMP_SHORT_HEADER, x._errorString.c_str());
			}
		}

		TEST_METHOD(Given_a_file_with_a_non_bmp_header_When_the_file_is_used_to_construct_a_bmp_Then_an_INVALID_FORMAT_exception_is_raised) {
			try {
				Bmp bmp(utils::ReadFile("ba-bm.bmp"));
				Assert::Fail(L"Exception not raised");
			}
			catch (Exceptions::Exception x) {
				Assert::AreEqual((int)Exceptions::INVALID_FORMAT, x._errorCode);
				Assert::AreEqual(Exceptions::ERROR_BMP_HEADER_MAGIC, x._errorString.c_str());
			}
		}

		TEST_METHOD(Given_a_file_with_a_length_greater_than_the_declared_filesize_When_the_file_is_used_to_construct_a_bmp_Then_an_BUFFER_OVERFLOW_exception_is_raised) {
			try {
				Bmp bmp(utils::ReadFile("badfilesize.bmp"));
				Assert::Fail(L"Exception not raised");
			}
			catch (Exceptions::Exception x) {
				Assert::AreEqual((int)Exceptions::BUFFER_OVERFLOW, x._errorCode);
				Assert::AreEqual(Exceptions::ERROR_BMP_SIZE_MISMATCH, x._errorString.c_str());
			}
		}

		TEST_METHOD(Given_a_file_with_a_length_less_than_the_declared_filesize_When_the_file_is_used_to_construct_a_bmp_Then_an_BUFFER_OVERFLOW_exception_is_raised) {
			try {
				Bmp bmp(utils::ReadFile("shortfile.bmp"));
				Assert::Fail(L"Exception not raised");
			}
			catch (Exceptions::Exception x) {
				Assert::AreEqual((int)Exceptions::BUFFER_OVERFLOW, x._errorCode);
				Assert::AreEqual(Exceptions::ERROR_BMP_SIZE_MISMATCH, x._errorString.c_str());
			}
		}

		TEST_METHOD(Given_a_file_with_a_length_less_than_the_declared_info_size_When_the_file_is_used_to_construct_a_bmp_Then_an_BUFFER_OVERFLOW_exception_is_raised) {
			try {
				Bmp bmp(utils::ReadFile("shortinfo.bmp"));
				Assert::Fail(L"Exception not raised");
			}
			catch (Exceptions::Exception x) {
				Assert::AreEqual((int)Exceptions::BUFFER_OVERFLOW, x._errorCode);
				Assert::AreEqual(Exceptions::ERROR_BMP_INFO_SIZE_MISMATCH, x._errorString.c_str());
			}
		}

		TEST_METHOD(Given_a_file_with_a_length_less_than_the_declared_palette_size_When_the_file_is_used_to_construct_a_bmp_Then_an_BUFFER_OVERFLOW_exception_is_raised) {
			try {
				Bmp bmp(utils::ReadFile("shortpal.bmp"));
				Assert::Fail(L"Exception not raised");
			}
			catch (Exceptions::Exception x) {
				Assert::AreEqual((int)Exceptions::BUFFER_OVERFLOW, x._errorCode);
				Assert::AreEqual(Exceptions::ERROR_BMP_IMAGE_SIZE_MISMATCH, x._errorString.c_str());
			}
		}

		TEST_METHOD(Given_a_file_with_an_unrecognized_info_size_When_the_file_is_used_to_construct_a_bmp_Then_an_INVALID_FORMAT_exception_is_raised) {
			try {
				Bmp bmp(utils::ReadFile("badheadersize.bmp"));
				Assert::Fail(L"Exception not raised");
			}
			catch (Exceptions::Exception x) {
				Assert::AreEqual((int)Exceptions::INVALID_FORMAT, x._errorCode);
				Assert::AreEqual(Exceptions::ERROR_BMP_UNSUPPORTED_VERSION, x._errorString.c_str());
			}
		}

		TEST_METHOD(Given_a_file_with_1_bpp_When_the_file_is_used_to_construct_a_bmp_Then_an_INVALID_FORMAT_exception_is_raised) {
			try {
				Bmp bmp(utils::ReadFile("pal1bg.bmp"));
				Assert::Fail(L"Exception not raised");
			}
			catch (Exceptions::Exception x) {
				Assert::AreEqual((int)Exceptions::INVALID_FORMAT, x._errorCode);
				Assert::AreEqual(Exceptions::ERROR_BMP_UNSUPPORTED_DEPTH, x._errorString.c_str());
			}
		}

		TEST_METHOD(Given_a_file_with_a_length_less_than_the_declared_image_size_When_the_file_is_used_to_construct_a_bmp_Then_an_BUFFER_OVERFLOW_exception_is_raised) {
			try {
				Bmp bmp(utils::ReadFile("shortimage.bmp"));
				Assert::Fail(L"Exception not raised");
			}
			catch (Exceptions::Exception x) {
				Assert::AreEqual((int)Exceptions::BUFFER_OVERFLOW, x._errorCode);
				Assert::AreEqual(Exceptions::ERROR_BMP_IMAGE_SIZE_MISMATCH, x._errorString.c_str());
			}
		}

		TEST_METHOD(Given_a_file_with_a_palette_size_greater_than_the_declared_bit_depth_When_the_file_is_used_to_construct_a_bmp_Then_an_INVALID_FORMAT_exception_is_raised) {
			try {
				Bmp bmp(utils::ReadFile("badpalettesize.bmp"));
				Assert::Fail(L"Exception not raised");
			}
			catch (Exceptions::Exception x) {
				Assert::AreEqual((int)Exceptions::INVALID_FORMAT, x._errorCode);
				Assert::AreEqual(Exceptions::ERROR_BMP_PALETTE_SIZE_MISMATCH, x._errorString.c_str());
			}
		}

		TEST_METHOD(Given_a_file_with_an_8bpp_bitmap_When_the_file_is_used_to_construct_a_bmp_Then_the_bmp_is_correct) {
			Bmp bmp(utils::ReadFile("pal8.bmp"));
			std::vector<uint8_t> expected = utils::ReadFile("pal8out.bmp");
			std::vector<uint8_t> actual;
			bmp.Buffer(actual);
			Assert::IsTrue((expected == actual), L"Copied bitmap is not as expected");
		}


	};
}
