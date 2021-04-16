#include "pch.h"
#include "CppUnitTest.h"
#include "clitest.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#define ASSETS "../../../testassets/"
#define DEBUG "./"
#define CD ".\\"

namespace Imm_Tests
{
	TEST_CLASS(Imm_Tests)
	{
	public:

		const std::string usage = "ImmFile usage:\n\n"
			"\tThis Message                : ImmFile -?\n"
			"\tConvert IMM/PAM to BMP      : ImmFile -ib immfile pamfile bmpfile\n"
			"\tConvert BMP to IMM/PAM      : ImmFile -bi bmpfile immfile pamfile\n\n";

		CLITEST_BEGIN(No_parameters_should_yield_usage_message)
			ADDFILE(DEBUG "ImmFile.exe")

			EXEC(CD "ImmFile")

			RETURNVALUE(0)
			ISEMPTY("err.txt")
			FILECONTENT("out.txt", usage)
		CLITEST_END

		CLITEST_BEGIN(Convert_a_bmp_to_imm_and_pam)
			ADDFILE(ASSETS "pal8out.bmp")
			ADDFILE(ASSETS "pal8out.imm")
			ADDFILE(ASSETS "pal8out.pam")
			ADDFILE(DEBUG "ImmFile.exe")
			
			EXEC(CD "ImmFile -bi pal8out.bmp out.imm out.pam")

			RETURNVALUE(0)
			EXISTS("out.imm")
			EXISTS("out.pam")
			FILECOMPARE("pal8out.imm", "out.imm")
			FILECOMPARE("pal8out.pam", "out.pam")
			ISEMPTY("out.txt")
			ISEMPTY("err.txt")
		CLITEST_END

		CLITEST_BEGIN(Convert_an_imm_and_pam_to_bmp)
			ADDFILE(ASSETS "pal8qnt.bmp")
			ADDFILE(ASSETS "pal8out.imm")
			ADDFILE(ASSETS "pal8out.pam")
			ADDFILE(DEBUG "ImmFile.exe")

			EXEC(CD "ImmFile -ib pal8out.imm pal8out.pam out.bmp")

			RETURNVALUE(0)
			EXISTS("out.bmp")
			FILECOMPARE("pal8qnt.bmp", "out.bmp")
			ISEMPTY("out.txt")
			ISEMPTY("err.txt")
		CLITEST_END

	};
}
