#include "pch.h"
#include "CppUnitTest.h"
#include <fstream>
#include <vector>
#include "../../../headers/header_Dat.hpp"
#include "../../../headers/header_Exceptions.hpp"
#include "utils.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Dat_Tests
{
	TEST_CLASS(Dat_Tests)
	{
	public:
		TEST_METHOD(Given_a_file_less_than_2_bytes_in_length_When_the_file_is_used_to_construct_a_dat_Then_an_IO_ERROR_exception_is_raised) {
			try {
				Dat dat(utils::ReadFile("tooshort.dat"));
				Assert::Fail(L"Exception not raised");
			}
			catch (Exceptions::Exception x) {
				Assert::AreEqual((int)Exceptions::BUFFER_OVERFLOW, x._errorCode);
				Assert::AreEqual(Exceptions::ERROR_IO_READ_BEYOND_BUFFER, x._errorString.c_str());
			}
		}

		TEST_METHOD(Given_a_file_containing_two_entries_When_the_file_is_used_to_construct_a_dat_Then_the_dat_will_report_two_entries) {
			Dat dat(utils::ReadFile("basic.dat"));
			int entryCount = dat.EntryCount();
			Assert::AreEqual(2, entryCount, L"Incorrect entry count");
		}

		TEST_METHOD(Given_a_dat_containing_two_entries_When_the_entry_headers_are_retrieved_in_order_Then_the_values_are_correct) {
			Dat dat(utils::ReadFile("basic.dat"));
			DatHeader *dh = dat.Next();
			Assert::AreEqual("pal8out.bmp", dh->FileName, L"First filename is incorrect");
			Assert::IsFalse(dh->IsNotCompressed, L"First entry is uncompressed");
			Assert::AreEqual(9270, (int)dh->UncompressedSize, L"First entry uncompressed size is incorrect");
			Assert::AreEqual(4767, (int)dh->CompressedSize, L"First entry compressed size is incorrect");
			dh = dat.Next();
			Assert::AreEqual("pal4out.bmp", dh->FileName, L"Second filename is incorrect");
			Assert::IsFalse(dh->IsNotCompressed, L"Second entry is uncompressed");
			Assert::AreEqual(4214, (int)dh->UncompressedSize, L"Second entry uncompressed size is incorrect");
			Assert::AreEqual(1698, (int)dh->CompressedSize, L"Second entry compressed size is incorrect");
		}

		TEST_METHOD(Given_a_dat_containing_two_entries_When_the_entry_headers_are_iterated_more_than_twice_Then_NULL_is_returned) {
			Dat dat(utils::ReadFile("basic.dat"));
			DatHeader *dh = dat.Next();
			Assert::IsNotNull(dh, L"First header is NULL");
			dh = dat.Next();
			Assert::IsNotNull(dh, L"Second header is NULL");
			dh = dat.Next();
			Assert::IsNull(dh, L"Third header is not NULL");
			dh = dat.Next();
			Assert::IsNull(dh, L"Subsequent header is not NULL");
		}

		TEST_METHOD(Given_a_dat_containing_two_entries_When_reset_is_called_Then_header_iteration_starts_over) {
			Dat dat(utils::ReadFile("basic.dat"));
			dat.Reset();
			DatHeader *dh = dat.Next();
			Assert::AreEqual("pal8out.bmp", dh->FileName, L"First filename is incorrect");
			dat.Reset();
			dh = dat.Next();
			Assert::AreEqual("pal8out.bmp", dh->FileName, L"First filename is incorrect after first reset");
			dh = dat.Next();
			Assert::AreEqual("pal4out.bmp", dh->FileName, L"Second filename is incorrect after first reset");
			dat.Reset();
			dh = dat.Next();
			Assert::AreEqual("pal8out.bmp", dh->FileName, L"First filename is incorrect after second reset");
			dh = dat.Next();
			Assert::AreEqual("pal4out.bmp", dh->FileName, L"Second filename is incorrect after second reset");
			dh = dat.Next();
			Assert::IsNull(dh, L"Third entry is not NULL after second reset");
			dat.Reset();
			dh = dat.Next();
			Assert::AreEqual("pal8out.bmp", dh->FileName, L"First filename is incorrect after third reset");

		}
	};
}
