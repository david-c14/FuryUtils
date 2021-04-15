#include "pch.h"
#include "utils.hpp"
#include <Windows.h>
#include <array>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <cstdlib>
#include <fstream>
#include "CppUnitTestLogger.h"

namespace utils {
	std::filesystem::path moduleDir;

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

	void Util::ModuleInit(std::string initTestDir) {
		moduleDir = std::filesystem::absolute(initTestDir);
		if (!std::filesystem::exists(moduleDir)) {
			std::filesystem::create_directory(moduleDir);
		}
		moduleDir.append(getDateTime() + "_" + getGUID());
		std::filesystem::create_directory(moduleDir);

	}

	void Util::ModuleCleanup() {
		if (moduleDir == "") {
			return;
		}
		try {
			std::filesystem::remove(moduleDir);
		}
		catch (std::filesystem::filesystem_error ex) {
			Microsoft::VisualStudio::CppUnitTestFramework::Logger::WriteMessage(ex.what());
		}
	}

	Util::Util(const std::string & testName) {
		_testName = testName;
		testDir = moduleDir;
		testDir.append(testName + "_" + getGUID());
		std::filesystem::create_directory(testDir);
		cleaned = false;
	}

	Util::~Util() {
		if (!cleaned) {
			_testName.append(" was not cleaned up");
			Microsoft::VisualStudio::CppUnitTestFramework::Logger::WriteMessage(_testName.c_str());
		}
	}

	void Util::TestCleanup() {
		std::filesystem::remove_all(testDir);
		cleaned = true;
	}

#undef CopyFile

	void Util::CopyFile(const std::filesystem::path & sourceFile) {
		CopyFile(sourceFile, testDir / sourceFile.filename());
	}

	void Util::CopyFile(const std::filesystem::path & sourceFile, const std::filesystem::path & destinationFile) {
		std::filesystem::path _dest = testDir / destinationFile;
		std::filesystem::path _src = std::filesystem::absolute(sourceFile);
		std::filesystem::copy_file(_src, _dest);
	}

	int Util::Run(const std::string & commandLine) {
		std::filesystem::path cp = std::filesystem::current_path();
		int retval = -1;
		try {
			std::filesystem::current_path(testDir);
			retval = std::system(commandLine.c_str());
		}
		catch (...) {
		}
		std::filesystem::current_path(cp);
		return retval;
	}

	bool Util::Compare(const std::filesystem::path & sourceFile, const std::filesystem::path & destinationFile) {
		std::filesystem::path _src = testDir / sourceFile;
		std::filesystem::path _dest = testDir / destinationFile;
		return ReadFile(_src.generic_string()) == ReadFile(_dest.generic_string());
	}

	bool Util::Content(const std::filesystem::path & sourceFile, const std::string & content) {
		std::filesystem::path _src = testDir / sourceFile;
		std::vector<uint8_t> vec(content.length());
		memcpy(vec.data(), content.c_str(), vec.size());
		return ReadText(_src.generic_string()) == vec;
	}

	bool Util::Exists(const std::filesystem::path & sourceFile) {
		return std::filesystem::exists(testDir / sourceFile);
	}

	bool Util::IsEmpty(const std::filesystem::path & sourceFile) {
		return std::filesystem::is_empty(testDir / sourceFile);
	}


	std::vector<uint8_t> Util::ReadFile(const std::string & fileName) {
		std::ifstream file(fileName, std::ios::binary | std::ios::ate);

		std::streamsize size = file.tellg();
		file.seekg(0, std::ios::beg);

		std::vector<uint8_t> buffer((uint32_t)size);
		file.read((char *)(buffer.data()), size);

		return buffer;
	}

	std::vector<uint8_t> Util::ReadText(const std::string & fileName) {
		std::ifstream file(fileName, std::ios::ate);

		std::streamsize size = file.tellg();
		file.seekg(0, std::ios::beg);

		std::vector<uint8_t> buffer((uint32_t)size);
		file.read((char *)(buffer.data()), size);
		buffer.resize((int)(file.gcount()));
		return buffer;
	}

}