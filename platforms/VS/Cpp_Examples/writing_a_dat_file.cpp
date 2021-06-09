//
// Writing a DAT archive file
//
// This example creates a new DAT archive containing two files
// The first file is compressed and the second is uncompressed
//

#include <fstream>
#include "../../../include/FuryUtils.hpp"

void writing_a_dat_file() {

	// Construct an empty DAT archive
	Dat dat;

	{
		// Load the first file to be archived into memory
		std::ifstream inputFile1("..\\..\\..\\testassets\\pal8.bmp", std::ios::binary | std::ios::ate);
		std::streamsize inputSize1 = inputFile1.tellg();
		inputFile1.seekg(0, std::ios::beg);
		std::vector<uint8_t> inputBuffer1((uint32_t)inputSize1);
		inputFile1.read((char *)(inputBuffer1.data()), inputSize1);
		inputFile1.close();

		// Add the file to the DAT archive
		dat.Add("PAL8.BMP", inputBuffer1, true);
	}

	{
		// Load the second file to be archive into memory
		std::ifstream inputFile2("..\\..\\..\\testassets\\tooshort.bmp", std::ios::binary | std::ios::ate);
		std::streamsize inputSize2 = inputFile2.tellg();
		inputFile2.seekg(0, std::ios::beg);
		std::vector<uint8_t> inputBuffer2((uint32_t)inputSize2);
		inputFile2.read((char *)(inputBuffer2.data()), inputSize2);
		inputFile2.close();

		// Add the file to the DAT archive
		dat.Add("Short.bmp", inputBuffer2, false);
	}

	{
		// Extract the DAT archive into memory
		std::vector<uint8_t> outputBuffer;
		dat.Buffer(outputBuffer);

		// Save the archive to disk
		std::ofstream outputFile("writing_a_dat_file.EXAMPLE", std::ios::binary | std::ios::ate);
		outputFile.write((char *)(outputBuffer.data()), outputBuffer.size());
		outputFile.close();
	}

}