#include "pch.h"
#include "CppUnitTest.h"
#include "utils.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_MODULE_INITIALIZE(moduleInitialize) {
	utils::Util::Init();
}

TEST_MODULE_CLEANUP(moduleCleanup) {
	utils::Util::Cleanup();
}
