#include "pch.h"
#include "utils.hpp"
#include <Windows.h>
#include <array>
#include <filesystem>
#include <chrono>
#include <ctime>
#include <iomanip>

namespace utils {
	std::filesystem::path testDir;

	std::string getGUID()
	{
		std::string result{};
		GUID guid;

		if (S_OK == CoCreateGuid(&guid))
		{
			std::array<char, 37> buffer{};   //32 characters of guid + 4 '-' in-between

			snprintf(buffer.data(), buffer.size(), "%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
				guid.Data1, guid.Data2, guid.Data3, guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
			result = std::string(buffer.data());
		}

		return result;
	}

	std::string getDateTime()
	{
		std::array<char, 20> buffer{};

		auto now = std::chrono::system_clock::now();
		std::time_t t = std::chrono::system_clock::to_time_t(now);
		std::tm tm;
		localtime_s(&tm, &t);

		strftime(buffer.data(), buffer.size(), "%Y-%m-%d_%H-%M-%S", &tm);
		return std::string(buffer.data());
	}

	void Util::Init() {
		if (!std::filesystem::exists("./cliTests")) {
			std::filesystem::create_directory("./cliTests");
		}
		testDir = "./cliTests/";
		testDir.append(getDateTime() + "_" + getGUID());
		testDir = std::filesystem::absolute(testDir);
		std::filesystem::create_directory(testDir);

	}

	void Util::Cleanup() {
		if (testDir == "") {
			return;
		}
		std::filesystem::remove_all(testDir);
	}
}