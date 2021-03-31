#include <fstream>
#include "../classes/class_BinaryIO.cpp"
#include "../classes/class_Imm.cpp"
#include "../classes/class_Bmp.cpp"
#include "../modules/module_Exceptions.cpp"

int Usage(char *arg0) {
	printf("%s usage:\n\n", arg0);
	printf("\tThis Message                : %s -?\n", arg0);
	printf("\tList entries                : %s -l datfile\n", arg0);
	printf("\tList entries in brief form  : %s -b datfile\n", arg0);
	printf("\tExtract entry               : %s -x datfile entry\n", arg0);
	printf("\tExtract all entries         : %s -X datfile\n", arg0);
	printf("\tCreate a compressed file    : %s -c datfile entry [...]", arg0);
	printf("\tCreate an uncompressed file : %s -u datfile entry [...]", arg0);
	printf("\n");
	return 0;
}

int ToBmp(int argc, char* argv[]) {
	if (argc != 4) {
		return Usage(argv[0]);
	}
	try
	{
		std::ifstream pamFile(argv[2], std::ios::binary | std::ios::ate);
		if (!pamFile) {
			printf("%s Error:\n\n File \"%s\" could not be opened\n", argv[0], argv[2]);
			return 1;
		}
		std::streamsize size = pamFile.tellg();
		pamFile.seekg(0, std::ios::beg);

		std::vector<uint8_t> pamBuffer((uint32_t)size);
		if (!pamFile.read((char *)(pamBuffer.data()), size))
		{
			printf("%s Error:\n\nFile \"%s\" could not be read\n", argv[0], argv[2]);
			return 1;
		}

		std::ifstream immFile(argv[3], std::ios::binary | std::ios::ate);
		if (!immFile) {
			printf("%s Error:\n\n File \"%s\" could not be opened\n", argv[0], argv[3]);
			return 1;
		}
		size = immFile.tellg();
		immFile.seekg(0, std::ios::beg);

		std::vector<uint8_t> immBuffer((uint32_t)size);
		if (!immFile.read((char *)(immBuffer.data()), size))
		{
			printf("%s Error:\n\nFile \"%s\" could not be read\n", argv[0], argv[3]);
			return 1;
		}

		Bmp bmp(pamBuffer, immBuffer);

		std::vector<uint8_t> outBuffer;
		bmp.Buffer(outBuffer);

		std::ofstream outFile("Output.bmp", std::ios::binary | std::ios::trunc);
		if (outFile) {
			outFile.write((char *)(outBuffer.data()), outBuffer.size());
		}
		else {
			printf("Error: Could not write file \"%s\"\n", "Output.bmp");
			return 1;
		}
		return 0;
	}
	catch (int error)
	{
		printf("%s Error:\n\n%d\n", argv[0], error);
		return 1;
	}
	return 1;
}


int main(int argc, char* argv[]) {
	if (argc == 1) {
		return Usage(argv[0]);
	}
	if (!strncmp(argv[1], "-?", 2)) {
		return Usage(argv[0]);
	}
	if (!strncmp(argv[1], "-b", 2)) {
		return ToBmp(argc, argv);
	}
	return 0;
}

