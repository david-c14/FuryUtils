#include "pch.h"
#include "CppUnitTest.h"
#include "utils.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#define ASSETS "../../../testassets/"
#define DEBUG "./"
#define CD ".\\"

namespace Imm_Tests
{
	TEST_CLASS(Imm_Tests)
	{
	public:
		
		TEST_METHOD(No_parameters_should_yield_usage_message) {
			utils::Util util("No_parameters_should_yield_usage_message");
			util.CopyFile(DEBUG "ImmFile.exe");

			std::string usage = "ImmFile usage:\n\n"
								"\tThis Message                : ImmFile -?\n"
								"\tConvert IMM/PAM to BMP      : ImmFile -ib immfile pamfile bmpfile\n"
								"\tConvert BMP to IMM/PAM      : ImmFile -bi bmpfile immfile pamfile\n\n";

			int retval = util.Run(CD "ImmFile > out.txt 2> err.txt");

			Assert::AreEqual(0, retval, L"Return value should be 0");
			Assert::IsTrue(util.IsEmpty("err.txt"), L"Error file should be empty");
			Assert::IsTrue(util.Content("out.txt", usage), L"Output file should contain usage");

			util.TestCleanup();
		}

		TEST_METHOD(Convert_a_bmp_to_imm_and_pam)
		{
			utils::Util util("Convert_a_bmp_to_imm_and_pam");
			util.CopyFile(ASSETS "pal8out.bmp");
			util.CopyFile(ASSETS "pal8out.imm");
			util.CopyFile(ASSETS "pal8out.pam");
			util.CopyFile(DEBUG "ImmFile.exe");
			
			int retval = util.Run(CD "ImmFile -bi pal8out.bmp out.imm out.pam > out.txt 2> err.txt");

			Assert::AreEqual(0, retval, L"Return value should be 0");
			Assert::IsTrue(util.Exists("out.imm"), L"Imm file should exist");
			Assert::IsTrue(util.Exists("out.pam"), L"Pam file should exist");
			Assert::IsTrue(util.Compare("pal8out.imm", "out.imm"), L"Imm files differ");
			Assert::IsTrue(util.Compare("pal8out.pam", "out.pam"), L"Pam files differ");
			Assert::IsTrue(util.IsEmpty("out.txt"), L"Output file should be empty");
			Assert::IsTrue(util.IsEmpty("err.txt"), L"Error file should be empty");

			util.TestCleanup();
		}

		TEST_METHOD(Convert_an_imm_and_pam_to_bmp)
		{
			utils::Util util("Convert_an_imm_and_pam_to_bmp");
			util.CopyFile(ASSETS "pal8qnt.bmp");
			util.CopyFile(ASSETS "pal8out.imm");
			util.CopyFile(ASSETS "pal8out.pam");
			util.CopyFile(DEBUG "ImmFile.exe");

			int retval = util.Run(CD "ImmFile -ib pal8out.imm pal8out.pam out.bmp > out.txt 2> err.txt");

			Assert::AreEqual(0, retval, L"Return value should be 0");
			Assert::IsTrue(util.Exists("out.bmp"), L"Bmp file should exist");
			Assert::IsTrue(util.Compare("pal8qnt.bmp", "out.bmp"), L"Bmp files differ");
			Assert::IsTrue(util.IsEmpty("out.txt"), L"Output file should be empty");
			Assert::IsTrue(util.IsEmpty("err.txt"), L"Error file should be empty");

			util.TestCleanup();
		}
	};
}
