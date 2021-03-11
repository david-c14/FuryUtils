#include <fstream>
#include <vector>
#include <cstdint>
#include <filesystem>
#include "../classes/class_BinaryRead.cpp"
#include "../classes/class_DatFile.cpp"

int Usage(char *arg0) {
	printf("%s usage:\n\n", arg0);
	printf("\tThis Message                : %s -?\n", arg0);
	printf("\tList entries                : %s -l datfile\n", arg0);
	printf("\tExtract entry               : %s -x datfile entry\n", arg0);
	printf("\tExtract all entries         : %s -X datfile\n", arg0);
	printf("\tCreate a compressed file    : %s -c datfile entry [...]", arg0);
	printf("\tCreate an uncompressed file : %s -u datfile entry [...]", arg0);
	printf("\n");
	return 0;
}

int List(int argc, char* argv[]) {
	if (argc != 3) {
		return Usage(argv[0]);
	}
	try
	{
		std::ifstream file(argv[2], std::ios::binary | std::ios::ate);
		if (!file) {
			printf("%s Error:\n\n File \"%s\" could not be opened\n", argv[0], argv[2]);
			return 1;
		}
		std::streamsize size = file.tellg();
		file.seekg(0, std::ios::beg);

		std::vector<char> buffer((uint32_t)size);
		if (file.read(buffer.data(), size))
		{
			DatFile df(buffer);
			DatFileHeader *dfh;
			printf("%s: Contents of %s\n\n", argv[0], argv[2]);
			printf("  Filename     Compressed   Uncompressed\n");
			printf("------------  ------------  ------------\n");
			while (dfh = df.Next()) {
				printf("%-12s  %12d  %12d\n", dfh->FileName, dfh->CompressedSize, dfh->UncompressedSize);
			}
			return 0;
		}
		printf("%s Error:\n\nFile \"%s\" could not be read\n", argv[0], argv[2]);
		return 1;
	}
	catch (const std::exception& ex)
	{
		printf("%s Error:\n\n%s\n", argv[0], ex.what());
		return 1;
	}
	return 1;
}

int Extract(int argc, char* argv[]) {
	if (argc != 4) {
		return Usage(argv[0]);
	}
	try
	{
		std::ifstream file(argv[2], std::ios::binary | std::ios::ate);
		if (!file) {
			printf("%s Error:\n\n File \"%s\" could not be opened\n", argv[0], argv[2]);
			return 1;
		}
		std::streamsize size = file.tellg();
		file.seekg(0, std::ios::beg);

		std::vector<char> buffer((uint32_t)size);
		if (file.read(buffer.data(), size))
		{
			DatFile df(buffer);
			DatFileHeader *dfh;
			printf("%s: Extracting \"%s\" from \"%s\"\n\n", argv[0], argv[3], argv[2]);
			while (dfh = df.Next()) {
				if (strncmp(argv[3], dfh->FileName, 12)) {
					continue;
				}
				std::vector<char> uncompressedBuffer;
				df.Entry(uncompressedBuffer);
				std::ofstream outFile(dfh->FileName, std::ios::binary | std::ios::trunc);
				if (outFile) {
					outFile.write(uncompressedBuffer.data(), uncompressedBuffer.size());
				}
				else {
					printf("Error: Could not write file \"%s\"\n", dfh->FileName);
					return 1;
				}
				return 0;
			}
			printf("Error: \"%s\" not found in \"%s\"\n", argv[3], argv[2]);
			return 1;
		}
		printf("%s Error:\n\nFile \"%s\" could not be read\n", argv[0], argv[2]);
		return 1;
	}
	catch (const std::exception& ex)
	{
		printf("%s Error:\n\n%s\n", argv[0], ex.what());
		return 1;
	}
	return 1;
}

int ExtractAll(int argc, char* argv[]) {
	if (argc != 3) {
		return Usage(argv[0]);
	}
	try
	{
		std::ifstream file(argv[2], std::ios::binary | std::ios::ate);
		if (!file) {
			printf("%s Error:\n\n File \"%s\" could not be opened\n", argv[0], argv[2]);
			return 1;
		}
		std::streamsize size = file.tellg();
		file.seekg(0, std::ios::beg);

		std::vector<char> buffer((uint32_t)size);
		if (file.read(buffer.data(), size))
		{
			DatFile df(buffer);
			DatFileHeader *dfh;
			printf("%s: Extracting all entries from \"%s\"\n\n", argv[0], argv[2]);
			while (dfh = df.Next()) {
				printf("%s\n", dfh->FileName);
				std::vector<char> uncompressedBuffer;
				df.Entry(uncompressedBuffer);
				std::ofstream outFile(dfh->FileName, std::ios::binary | std::ios::trunc);
				if (outFile) {
					outFile.write(uncompressedBuffer.data(), uncompressedBuffer.size());
				}
				else {
					printf("%s Error: Could not write file \"%s\"\n", argv[0], dfh->FileName);
				}
			}
			return 0;
		}
		printf("%s Error:\n\nFile \"%s\" could not be read\n", argv[0], argv[2]);
		return 1;
	}
	catch (const std::exception& ex)
	{
		printf("%s Error:\n\n%s\n", argv[0], ex.what());
		return 1;
	}
	return 1;
}


std::string GetFileName(const std::string &s) {

	char sep = '/';

#ifdef _WIN32
	sep = '\\';
#endif

	size_t i = s.rfind(sep, s.length());
	if (i != std::string::npos) {
		return(s.substr(i + 1, s.length() - i));
	}

	return("");
}

int Create(int argc, char* argv[], bool compress) {
	if (argc < 4) {
		return Usage(argv[0]);
	}

	DatFile df;

	for (int i = 3; i < argc; i++) {
		std::string path = argv[i];
		std::string filename = GetFileName(path);

		std::ifstream file(argv[i], std::ios::binary | std::ios::ate);
		if (!file) {
			printf("%s Error:\n\n Input file \"%s\" could not be opened\n", argv[0], filename.c_str());
			return 1;
		}
		std::streamsize size = file.tellg();
		file.seekg(0, std::ios::beg);
		std::vector<char> buffer((uint32_t)size);
		if (file.read(buffer.data(), size))
		{
			df.Add(filename.c_str(), buffer, compress);
		}
		else {
			printf("%s Error:\n\n Input file \"%s\" could not be read\n", argv[0], filename.c_str());
		}
	}
	std::ofstream outFile(argv[2], std::ios::binary | std::ios::trunc);
	if (outFile) {
		std::vector<char> buffer;
		df.Buffer(buffer);
		outFile.write(buffer.data(), buffer.size());
	}
	else {
		printf("%s Error: Could not output write file \"%s\"\n", argv[0], argv[2]);
		return 1;
	}
	return 0;
}

int main(int argc, char* argv[]) {
	if (argc == 1) {
		return Usage(argv[0]);
	}
	if (!strncmp(argv[1], "-?", 2)) {
		return Usage(argv[0]);
	}
	if (!strncmp(argv[1], "-l", 2)) {
		return List(argc, argv);
	}
	if (!strncmp(argv[1], "-x", 2)) {
		return Extract(argc, argv);
	}
	if (!strncmp(argv[1], "-X", 2)) {
		return ExtractAll(argc, argv);
	}
	if (!strncmp(argv[1], "-c", 2)) {
		return Create(argc, argv, true);
	}
	if (!strncmp(argv[1], "-u", 2)) {
		return Create(argc, argv, false);
	}
	return 0;
}