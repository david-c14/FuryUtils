#pragma once
#include <string>
#include <filesystem>

namespace utils {
	struct Util {

	private:
		std::filesystem::path testDir;
		bool cleaned;
		std::string _testName;

	public:
		static void ModuleInit(std::string initTestDir);
		static void ModuleCleanup();

		Util(const std::string & testName);
		~Util();

		void TestCleanup();

		void CopyFile(const std::filesystem::path & sourceFile);
		void CopyFile(const std::filesystem::path & sourceFile, const std::filesystem::path & destination);

		int Run(const std::string & commandLine);
		
		bool Compare(const std::filesystem::path & sourceFile, const std::filesystem::path & destination);
		bool Content(const std::filesystem::path & sourceFile, const std::string & content);
		bool Exists(const std::filesystem::path & sourceFile);
		bool IsEmpty(const std::filesystem::path & sourceFile);

		std::vector<uint8_t> ReadFile(const std::string & fileName);
		std::vector<uint8_t> ReadText(const std::string & fileName);
	};
}