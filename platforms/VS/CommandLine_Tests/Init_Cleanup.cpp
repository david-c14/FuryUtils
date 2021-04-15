#include "pch.h"
#include "CppUnitTest.h"
#include "utils.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_MODULE_INITIALIZE(moduleInitialize) {
	utils::Util::ModuleInit("./cliTests");
}

TEST_MODULE_CLEANUP(moduleCleanup) {
	utils::Util::ModuleCleanup();
}
