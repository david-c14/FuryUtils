#include "pch.h"
#include "CppUnitTest.h"
#include <fstream>
#include <vector>
#include "../../../headers/header_Bmp.hpp"
#include "utils.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BmpTests
{
	TEST_CLASS(BmpTests)
	{
	public:

		TEST_METHOD(TestMethod1)
		{
			Bmp bmp(utils::ReadFile("FBI.BMP"));
			Assert::AreEqual(768u, bmp.PamSize(), L"PAM Size is not expected size");
		}
	};
}
