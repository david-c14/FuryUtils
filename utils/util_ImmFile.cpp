#include <fstream>
#include "../classes/class_BinaryIO.cpp"
#include "../classes/class_Imm.cpp"
#include "../classes/class_Bmp.cpp"
#include "../modules/module_Exceptions.cpp"

int Usage(char *arg0) {
	printf("%s usage:\n\n", arg0);
	printf("\tThis Message                : %s -?\n", arg0);
	printf("\tConvert IMM/PAM to BMP      : %s -ib immfile pamfile bmpfile\n", arg0);
	printf("\tConvert BMP to IMM/PAM      : %s -bi bmpfile immfile pamfile\n", arg0);
	printf("\n");
	return 0;
}

int ImmToBmp(int argc, char* argv[]) {
	if (argc != 5) {
		return Usage(argv[0]);
	}
	try
	{
		std::ifstream pamFile(argv[2], std::ios::binary | std::ios::ate);
		if (!pamFile) {
			printf("%s Error:\n\n File \"%s\" could not be opened\n", argv[0], argv[2]);
			return Exceptions::Codes::IO_ERROR;
		}
		std::streamsize size = pamFile.tellg();
		pamFile.seekg(0, std::ios::beg);

		std::vector<uint8_t> pamBuffer((uint32_t)size);
		if (!pamFile.read((char *)(pamBuffer.data()), size))
		{
			printf("%s Error:\n\nFile \"%s\" could not be read\n", argv[0], argv[2]);
			return Exceptions::Codes::IO_ERROR;
		}

		std::ifstream immFile(argv[3], std::ios::binary | std::ios::ate);
		if (!immFile) {
			printf("%s Error:\n\n File \"%s\" could not be opened\n", argv[0], argv[3]);
			return Exceptions::Codes::IO_ERROR;
		}
		size = immFile.tellg();
		immFile.seekg(0, std::ios::beg);

		std::vector<uint8_t> immBuffer((uint32_t)size);
		if (!immFile.read((char *)(immBuffer.data()), size))
		{
			printf("%s Error:\n\nFile \"%s\" could not be read\n", argv[0], argv[3]);
			return Exceptions::Codes::IO_ERROR;
		}

		Bmp bmp(pamBuffer, immBuffer);

		std::vector<uint8_t> outBuffer;
		bmp.Buffer(outBuffer);

		std::ofstream outFile(argv[4], std::ios::binary | std::ios::trunc);
		if (outFile) {
			outFile.write((char *)(outBuffer.data()), outBuffer.size());
		}
		else {
			printf("%s Error: Could not write output file \"%s\"\n", argv[0], argv[4]);
			return Exceptions::Codes::IO_ERROR;
		}
		return 0;
	}
	catch (int error)
	{
		printf("%s Error:\n\n%d\n", argv[0], error);
		return error;
	}
	return Exceptions::Codes::UNKNOWN_ERROR;
}

int BmpToImm(int argc, char* argv[]) {
	if (argc != 5) {
		return Usage(argv[0]);
	}
	try
	{
		std::ifstream bmpFile(argv[2], std::ios::binary | std::ios::ate);
		if (!bmpFile) {
			printf("%s Error:\n\n File \"%s\" could not be opened\n", argv[0], argv[2]);
			return Exceptions::Codes::IO_ERROR;
		}
		std::streamsize size = bmpFile.tellg();
		bmpFile.seekg(0, std::ios::beg);

		std::vector<uint8_t> bmpBuffer((uint32_t)size);
		if (!bmpFile.read((char *)(bmpBuffer.data()), size))
		{
			printf("%s Error:\n\nFile \"%s\" could not be read\n", argv[0], argv[2]);
			return Exceptions::Codes::IO_ERROR;
		}

		Bmp bmp(bmpBuffer);

		std::vector<uint8_t> pamBuffer;
		bmp.PamBuffer(pamBuffer);

		std::ofstream pamFile(argv[3], std::ios::binary | std::ios::trunc);
		if (pamFile) {
			pamFile.write((char *)(pamBuffer.data()), pamBuffer.size());
		}
		else {
			printf("%s Error: Could not write output file \"%s\"\n", argv[0], argv[3]);
			return Exceptions::Codes::IO_ERROR;
		}

		std::vector<uint8_t> immBuffer;
		bmp.ImmBuffer(immBuffer);

		std::ofstream immFile(argv[4], std::ios::binary | std::ios::trunc);
		if (immFile) {
			immFile.write((char *)(immBuffer.data()), immBuffer.size());
		}
		else {
			printf("%s Error: Could not write output file \"%s\"\n", argv[0], argv[4]);
			return Exceptions::Codes::IO_ERROR;
		}
		return 0;
	}
	catch (int error)
	{
		printf("%s Error:\n\n%d\n", argv[0], error);
		return error;
	}
	return Exceptions::Codes::UNKNOWN_ERROR;
}

int main(int argc, char* argv[]) {
	if (argc == 1) {
		return Usage(argv[0]);
	}
	if (!strncmp(argv[1], "-?", 2)) {
		return Usage(argv[0]);
	}
	if (!strncmp(argv[1], "-ib", 3)) {
		return ImmToBmp(argc, argv);
	}
	if (!strncmp(argv[1], "-bi", 3)) {
		return BmpToImm(argc, argv);
	}
	return 0;
}

